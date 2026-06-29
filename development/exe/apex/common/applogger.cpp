#include "applogger.h"

#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QHostInfo>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>

#include <cstdlib>

namespace {

    struct AppLoggerState {
        QMutex mutex;

        QFile file;
        QTextStream out;

        bool isOpen = false;
        bool enabled = true;

        QString fullPath; // absolute file path when chosen/opened

        AppLogStatusCallback cb = nullptr;

        AppLoggerState()
            : file()
            , out(&file)
        {
        }
    };

    void cleanupLogger();

    AppLoggerState& loggerState()
    {
        static AppLoggerState s;
        static bool s_cleanupInstalled = false;

        if (!s_cleanupInstalled) {
            s_cleanupInstalled = true;
            std::atexit(&cleanupLogger);
        }

        return s;
    }

    static AppLogSeverity classifySeverity(const QString& title, const QString& mess)
    {
        const QString t = title;
        const QString m = mess;

        // Case-insensitive contains checks
        // if (t.contains("EXIT", Qt::CaseInsensitive) || m.contains("EXIT", Qt::CaseInsensitive))
        if (t.contains("EXIT") || m.contains("EXIT"))
            return APPLOG_SEV_EXIT;

        if (t.contains("ERROR") || m.contains("ERROR"))
            return APPLOG_SEV_ERROR;

        if (t.contains("WARNING") || m.contains("WARNING"))
            return APPLOG_SEV_WARNING;

        return APPLOG_SEV_INFO;
    }

    static QString currentLogFileName()
    {
        // One log file per day
        return QString("log.%1.txt").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd"));
    }

    static QString toAbsolutePath(const QString& fileName)
    {
        // Use current working directory; adjust if you prefer AppData or a fixed folder.
        return QDir::current().absoluteFilePath(fileName);
    }

    static bool ensureLogFileOpen_locked(AppLoggerState& s, QString* openFailReason)
    {
        if (s.isOpen) {
            return true;
        }

        const QString fileName = currentLogFileName();
        s.fullPath = toAbsolutePath(fileName);
        s.file.setFileName(s.fullPath);

        if (!s.file.open(QIODevice::Append | QIODevice::Text)) {
            s.isOpen = false;
            if (openFailReason) {
                *openFailReason = s.file.errorString();
            }
            return false;
        }

        s.isOpen = true;
        return true;
    }

    void cleanupLogger()
    {
        AppLoggerState& s = loggerState();
        AppLogStatusCallback cb = nullptr;
        QString fullPathCopy;

        {
            QMutexLocker locker(&s.mutex);
            cb = s.cb;
            fullPathCopy = s.fullPath;

            if (s.isOpen) {
                s.out.flush();
                s.file.close();
                s.isOpen = false;
            }
        }

        if (cb) {
            cb(APPLOG_EVENT_CLOSED, APPLOG_SEV_INFO, fullPathCopy, QString(), QString());
        }
    }

} // namespace

// Define the globals declared with extern in the header
bool theOneBCToRuleThemAll = false;
QString appHostname;

QString getAppHostname()
{
    if (appHostname.isEmpty()) {
        appHostname = QHostInfo::localHostName();
    }
    return appHostname;
}

bool isAppHostEnabled(const QString& hostname)
{
    QFile file("enable.cfg");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (QString::compare(line, hostname, Qt::CaseInsensitive) == 0)
            return true;
    }

    return false;
}

void applog_setStatusCallback(AppLogStatusCallback cb)
{
    AppLoggerState& s = loggerState();
    QMutexLocker locker(&s.mutex);
    s.cb = cb;
}

bool applog_isOpen()
{
    AppLoggerState& s = loggerState();
    QMutexLocker locker(&s.mutex);
    return s.isOpen;
}

QString applog_fullPath()
{
    AppLoggerState& s = loggerState();
    QMutexLocker locker(&s.mutex);
    return s.fullPath;
}

void applog_setEnabled(bool enabled)
{
    AppLoggerState& s = loggerState();
    QMutexLocker locker(&s.mutex);
    s.enabled = enabled;
}

bool applog_isEnabled()
{
    AppLoggerState& s = loggerState();
    QMutexLocker locker(&s.mutex);
    return s.enabled;
}

void applog(QString title, QString mess)
{
    AppLoggerState& s = loggerState();

    // Classify severity up front (used for UI latching rules)
    const AppLogSeverity sev = classifySeverity(title, mess);

    if (sev == APPLOG_SEV_EXIT || sev == APPLOG_SEV_ERROR) {
        // We're going to log...
    } else {
       // For testing, if it's not an EXIT, don't bother logging it.
       return;
	}


    AppLogStatusCallback cb = nullptr;
    QString fullPathCopy;
    bool didOpenNow = false;
    bool openFailed = false;

    {
        QMutexLocker locker(&s.mutex);
        cb = s.cb;

        // Open file if we are going to write (enabled) OR if a callback wants the full path/open-state.
        // This makes the UI able to show BLUE (opened) and tooltip path even before first write, if logging happens.
        QString openFailReason;
        if (!s.isOpen) {
            const bool wantOpen = s.enabled; // open only when enabled; you can change to "always open on first applog"
            if (wantOpen) {
                if (ensureLogFileOpen_locked(s, &openFailReason)) {
                    didOpenNow = true;
                }
                else {
                    openFailed = true;
                }
            }
            else {
                // Even if not enabled, still compute the path deterministically for tooltip/click-open
                s.fullPath = toAbsolutePath(currentLogFileName());
            }
        }

        fullPathCopy = s.fullPath;

        // Notify OPEN_FAILED or OPENED (do it after releasing lock)
        // Perform write if enabled and open succeeded
        if (s.enabled && s.isOpen) {
            s.out << QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss\t");
            s.out << " [" << QString::number(sev) << "] -" + title + "-\t -" + mess + "\n";
            s.out.flush();
        }
    }

    // Call callback outside mutex to avoid deadlocks / UI re-entrancy issues.
    if (cb) {
        if (openFailed) {
            cb(APPLOG_EVENT_OPEN_FAILED, APPLOG_SEV_ERROR, fullPathCopy, title, mess);
            return; // no WRITE event possible if we couldn't open
        }

        if (didOpenNow) {
            cb(APPLOG_EVENT_OPENED, APPLOG_SEV_INFO, fullPathCopy, QString(), QString());
        }

        // Even if file writing is disabled, a log call is still meaningful for UI latches (ERROR/WARNING).
        cb(APPLOG_EVENT_WRITE, sev, fullPathCopy, title, mess);
    }
}
#include "applogger.h"

#include <QCoreApplication>
#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHostInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QStandardPaths>
#include <QTextStream>
#include <QThread>
#include <QtGlobal>

#include <cstdlib>
#include <cstdio>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace {

struct AppLoggerState {
    QMutex mutex;
    QFile file;
    QTextStream out;
    bool isOpen;
    bool enabled;
    QString fullPath;
    QString logDirectory;
    QString applicationName;
    QDate openDate;
    int retentionDays;
    qint64 maxFileBytes;
    int maxBackupFiles;
    AppLogSeverity minimumSeverity;
    AppLogStatusCallback cb;
    AppLogStatusCallback errorCb;

    AppLoggerState()
        : file()
        , out(&file)
        , isOpen(false)
        , enabled(true)
        , applicationName("basket-crane")
        , retentionDays(30)
        , maxFileBytes(25LL * 1024LL * 1024LL)
        , maxBackupFiles(5)
        , minimumSeverity(APPLOG_SEV_INFO)
        , cb(0)
        , errorCb(0)
    {
    }
};

void cleanupLogger();

AppLoggerState& loggerState()
{
    static AppLoggerState s;
    static bool cleanupInstalled = false;
    if (!cleanupInstalled) {
        cleanupInstalled = true;
        std::atexit(&cleanupLogger);
    }
    return s;
}

AppLogSeverity classifySeverity(const QString& title, const QString& message)
{
    const QString normalizedTitle = title.trimmed();
    const QString normalizedMessage = message.trimmed();
    if (normalizedTitle.startsWith("EXIT", Qt::CaseInsensitive)
        || normalizedMessage.startsWith("EXIT", Qt::CaseInsensitive))
        return APPLOG_SEV_EXIT;
    if (normalizedTitle.startsWith("ERROR", Qt::CaseInsensitive)
        || normalizedMessage.startsWith("ERROR", Qt::CaseInsensitive))
        return APPLOG_SEV_ERROR;
    if (normalizedTitle.startsWith("WARNING", Qt::CaseInsensitive)
        || normalizedMessage.startsWith("WARNING", Qt::CaseInsensitive))
        return APPLOG_SEV_WARNING;
    if (normalizedTitle.startsWith("DEBUG", Qt::CaseInsensitive)
        || normalizedMessage.startsWith("DEBUG", Qt::CaseInsensitive)
        || title.compare("mainTimerSlot called", Qt::CaseInsensitive) == 0)
        return APPLOG_SEV_DEBUG;
    return APPLOG_SEV_INFO;
}

QString severityName(AppLogSeverity severity)
{
    switch (severity) {
    case APPLOG_SEV_DEBUG: return "DEBUG";
    case APPLOG_SEV_WARNING: return "WARNING";
    case APPLOG_SEV_ERROR: return "ERROR";
    case APPLOG_SEV_EXIT: return "EXIT";
    default: return "INFO";
    }
}

QString singleLine(QString value, int maxCharacters)
{
    if (value.length() > maxCharacters) {
        const int omitted = value.length() - maxCharacters;
        value = value.left(maxCharacters) + QString("... [truncated %1 characters]").arg(omitted);
    }
    value.replace('\r', "\\r");
    value.replace('\n', "\\n");
    return value;
}

QString logFilePath(const AppLoggerState& s, const QDate& date)
{
    const QString fileName = QString("%1.%2.log")
        .arg(s.applicationName)
        .arg(date.toString("yyyy-MM-dd"));
    return QDir(s.logDirectory).absoluteFilePath(fileName);
}

void removeExpiredLogs_locked(const AppLoggerState& s)
{
    if (s.retentionDays <= 0 || s.logDirectory.isEmpty())
        return;

    const QDir dir(s.logDirectory);
    const QStringList files = dir.entryList(
        QStringList() << (s.applicationName + ".*.log*"), QDir::Files, QDir::Name);
    const QDate cutoff = QDate::currentDate().addDays(-s.retentionDays);
    for (int i = 0; i < files.count(); ++i) {
        const QFileInfo info(dir.absoluteFilePath(files[i]));
        if (info.lastModified().date() < cutoff)
            QFile::remove(info.absoluteFilePath());
    }
}

bool ensureLogFileOpen_locked(AppLoggerState& s, QString* openFailReason)
{
    const QDate today = QDate::currentDate();
    if (s.isOpen && s.openDate == today)
        return true;

    if (s.isOpen) {
        s.out.flush();
        s.file.close();
        s.isOpen = false;
        removeExpiredLogs_locked(s);
    }

    s.fullPath = logFilePath(s, today);
    s.file.setFileName(s.fullPath);
    if (!s.file.open(QIODevice::Append | QIODevice::Text)) {
        if (openFailReason)
            *openFailReason = s.file.errorString();
        return false;
    }

    s.out.setCodec("UTF-8");
    s.isOpen = true;
    s.openDate = today;
    return true;
}

bool rotateOversizedLog_locked(AppLoggerState& s, QString* openFailReason)
{
    if (!s.isOpen || s.file.size() < s.maxFileBytes)
        return true;

    s.out.flush();
    s.file.close();
    s.isOpen = false;

    const QString basePath = s.fullPath;
    for (int index = s.maxBackupFiles; index >= 1; --index) {
        const QString destination = basePath + "." + QString::number(index);
        if (index == s.maxBackupFiles)
            QFile::remove(destination);
        const QString source = index == 1
            ? basePath
            : basePath + "." + QString::number(index - 1);
        if (QFile::exists(source) && !QFile::rename(source, destination)) {
            if (openFailReason)
                *openFailReason = QString("Cannot rotate '%1' to '%2'").arg(source).arg(destination);
            return ensureLogFileOpen_locked(s, 0);
        }
    }

    return ensureLogFileOpen_locked(s, openFailReason);
}

void reportOpenFailure(const QString& path, const QString& reason)
{
    const QString message = QString("Basket Crane logging failed for '%1': %2\n").arg(path).arg(reason);
    const QByteArray localMessage = message.toLocal8Bit();
    std::fputs(localMessage.constData(), stderr);
#ifdef Q_OS_WIN
    OutputDebugStringW(reinterpret_cast<const wchar_t*>(message.utf16()));
#endif
}

void cleanupLogger()
{
    AppLoggerState& s = loggerState();
    AppLogStatusCallback cb = 0;
    QString path;
    bool wasOpen = false;
    {
        QMutexLocker locker(&s.mutex);
        cb = s.cb;
        path = s.fullPath;
        wasOpen = s.isOpen;
        if (s.isOpen) {
            s.out.flush();
            s.file.close();
            s.isOpen = false;
        }
    }
    if (cb && wasOpen)
        cb(APPLOG_EVENT_CLOSED, APPLOG_SEV_INFO, path, QString(), QString());
}

void appQtMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    AppLogSeverity severity = APPLOG_SEV_DEBUG;
    if (type == QtWarningMsg)
        severity = APPLOG_SEV_WARNING;
    else if (type == QtCriticalMsg || type == QtFatalMsg)
        severity = APPLOG_SEV_ERROR;

    QString source = "QT";
    if (context.file) {
        source += QString(" | %1:%2").arg(QString::fromLocal8Bit(context.file)).arg(context.line);
        if (context.function)
            source += QString(" | %1").arg(QString::fromLocal8Bit(context.function));
    }
    applog_write(severity, source, message);
    if (type == QtFatalMsg)
        std::abort();
}

} // namespace

bool theOneBCToRuleThemAll = false;
QString appHostname;

QString getAppHostname()
{
    if (appHostname.isEmpty())
        appHostname = QHostInfo::localHostName();
    return appHostname;
}

bool isAppHostEnabled(const QString& hostname)
{
    QFile file("enable.cfg");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (QString::compare(line, hostname, Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

void applog_initialize(const QString& applicationName, int retentionDays)
{
    AppLoggerState& s = loggerState();
    QString openFailReason;
    bool opened = false;
    {
        QMutexLocker locker(&s.mutex);
        QString safeName = applicationName.trimmed().toLower();
        safeName.replace(' ', '-');
        if (!safeName.isEmpty())
            s.applicationName = safeName;
        s.retentionDays = retentionDays;
        const QString configuredLevel = QString::fromLocal8Bit(qgetenv("BASKET_CRANE_LOG_LEVEL")).trimmed();
        s.minimumSeverity = configuredLevel.compare("DEBUG", Qt::CaseInsensitive) == 0
            ? APPLOG_SEV_DEBUG : APPLOG_SEV_INFO;

        QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        if (basePath.isEmpty())
            basePath = QCoreApplication::applicationDirPath();
        QDir baseDir(basePath);
        if (!baseDir.mkpath("logs")) {
            baseDir = QDir::current();
            baseDir.mkpath("logs");
        }
        s.logDirectory = baseDir.absoluteFilePath("logs");
        removeExpiredLogs_locked(s);
        opened = ensureLogFileOpen_locked(s, &openFailReason);
    }

    if (!opened) {
        reportOpenFailure(applog_fullPath(), openFailReason);
        return;
    }

    applog_write(APPLOG_SEV_INFO, "APPLICATION",
        QString("START | version=%1 | host=%2 | pid=%3 | executable=%4 | workingDirectory=%5 | log=%6")
            .arg(QCoreApplication::applicationVersion().isEmpty() ? "unknown" : QCoreApplication::applicationVersion())
            .arg(getAppHostname())
            .arg(QCoreApplication::applicationPid())
            .arg(QCoreApplication::applicationFilePath())
            .arg(QDir::currentPath())
            .arg(applog_fullPath()));
}

void applog_shutdown()
{
    applog_write(APPLOG_SEV_INFO, "APPLICATION", "STOP");
    qInstallMessageHandler(0);
    cleanupLogger();
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

void applog_setErrorCallback(AppLogStatusCallback cb)
{
    AppLoggerState& s = loggerState();
    QMutexLocker locker(&s.mutex);
    s.errorCb = cb;
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
    applog_write(classifySeverity(title, mess), title, mess);
}

void applog_write(AppLogSeverity severity, const QString& title, const QString& mess)
{
    AppLoggerState& s = loggerState();
    AppLogStatusCallback cb = 0;
    AppLogStatusCallback errorCb = 0;
    QString path;
    QString openFailReason;
    bool openedNow = false;
    bool openFailed = false;

    {
        QMutexLocker locker(&s.mutex);
        if (severity < s.minimumSeverity)
            return;
        cb = s.cb;
        errorCb = s.errorCb;
        if (!s.isOpen || s.openDate != QDate::currentDate()) {
            if (s.enabled) {
                openedNow = ensureLogFileOpen_locked(s, &openFailReason);
                openFailed = !openedNow;
            }
            else if (s.fullPath.isEmpty() && !s.logDirectory.isEmpty()) {
                s.fullPath = logFilePath(s, QDate::currentDate());
            }
        }
        path = s.fullPath;

        if (s.enabled && s.isOpen) {
            if (!rotateOversizedLog_locked(s, &openFailReason)) {
                openFailed = true;
            }
            path = s.fullPath;
        }

        if (s.enabled && s.isOpen && !openFailed) {
            s.out << QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz")
                  << " | " << severityName(severity)
                  << " | pid=" << QCoreApplication::applicationPid()
                  << " | thread=" << QString::number(reinterpret_cast<quintptr>(QThread::currentThreadId()), 16)
                  << " | " << singleLine(title, 512)
                  << " | " << singleLine(mess, 16384) << "\n";
            s.out.flush();
        }
    }

    if (openFailed)
        reportOpenFailure(path, openFailReason);
    if (errorCb && severity >= APPLOG_SEV_ERROR)
        errorCb(APPLOG_EVENT_WRITE, severity, path, title, mess);
    if (cb) {
        if (openFailed) {
            cb(APPLOG_EVENT_OPEN_FAILED, APPLOG_SEV_ERROR, path, title, mess);
            return;
        }
        if (openedNow)
            cb(APPLOG_EVENT_OPENED, APPLOG_SEV_INFO, path, QString(), QString());
        cb(APPLOG_EVENT_WRITE, severity, path, title, mess);
    }
}

void applog_installQtMessageHandler()
{
    qInstallMessageHandler(appQtMessageHandler);
}

void utilitiesAppLog(const QString& title, const QString& mess, bool fatal)
{
    applog_write(fatal ? APPLOG_SEV_EXIT : APPLOG_SEV_WARNING, title, mess);
}

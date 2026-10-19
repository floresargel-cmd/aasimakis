#include <QCoreApplication>
#include <QTemporaryDir>
#include <sys/utime.h>
#include <cstdio>
#include <cstdlib>
#include "../../common/applogger.cpp"

static void check(bool passed, const char* message)
{
    if (!passed) { std::fprintf(stderr, "FAIL: %s\n", message); std::exit(1); }
}

static void writeFile(const QString& path, const QByteArray& contents)
{
    QFile file(path);
    check(file.open(QIODevice::WriteOnly), "open test file");
    check(file.write(contents) == contents.size(), "write test file");
}

static QByteArray readFile(const QString& path)
{
    QFile file(path);
    check(file.open(QIODevice::ReadOnly), "open retained file");
    return file.readAll();
}

static void ageFile(const QString& path, int days)
{
    const __time64_t timestamp = QDateTime(QDate::currentDate().addDays(-days), QTime(12, 0)).toTime_t();
    struct __utimbuf64 times = { timestamp, timestamp };
    check(_wutime64(reinterpret_cast<const wchar_t*>(path.utf16()), &times) == 0, "set test file age");
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QTemporaryDir directory;
    check(directory.isValid(), "temporary logs directory");
    AppLoggerState state;
    state.logDirectory = directory.path();
    state.maxFileBytes = 1;
    QString error;
    check(ensureLogFileOpen_locked(state, &error), "open current log");
    const QString base = state.fullPath;
    // Existing five-backup installations must keep all of their old chunks.
    for (int i = 1; i <= 5; ++i)
        writeFile(base + "." + QString::number(i), QByteArray::number(i));
    for (int i = 6; i <= 13; ++i) {
        state.out << i;
        state.out.flush();
        check(rotateOversizedLog_locked(state, &error), "rotate beyond five backups");
    }
    for (int i = 1; i <= 13; ++i)
        check(readFile(base + "." + QString::number(i)) == QByteArray::number(i), "all chunks retained unchanged");
    state.file.close(); state.isOpen = false;
    // A fresh logger instance must not overwrite archives after restart.
    AppLoggerState restarted;
    restarted.logDirectory = directory.path(); restarted.maxFileBytes = 1;
    check(ensureLogFileOpen_locked(restarted, &error), "reopen after restart");
    restarted.out << "restart"; restarted.out.flush();
    check(rotateOversizedLog_locked(restarted, &error), "rotate after restart");
    check(readFile(base + ".14") == "restart", "restart uses unused suffix");
    check(readFile(base + ".1") == "1", "oldest archive survives restart");
    restarted.file.close(); restarted.isOpen = false;

    const QDir logDir(directory.path());
    const QString expired = logDir.filePath("basket-crane.expired.log.8");
    const QString boundary = logDir.filePath("basket-crane.boundary.log.9");
    const QString unrelated = logDir.filePath("other-app.expired.log");
    writeFile(expired, "old"); ageFile(expired, 31);
    writeFile(boundary, "keep"); ageFile(boundary, 30);
    writeFile(unrelated, "keep"); ageFile(unrelated, 31);
    removeExpiredLogs_locked(restarted);
    check(!QFile::exists(expired), "delete backup beyond 30 days");
    check(QFile::exists(boundary), "retain backup at 30-day boundary");
    check(QFile::exists(base + ".1"), "retain recent archives");
    check(QFile::exists(unrelated), "leave other applications alone");
    writeFile(expired, "old"); ageFile(expired, 31);
    check(ensureLogFileOpen_locked(restarted, &error), "open log before midnight rollover");
    restarted.openDate = QDate::currentDate().addDays(-1);
    check(ensureLogFileOpen_locked(restarted, &error), "daily rollover");
    check(!QFile::exists(expired), "daily rollover cleans expired files");
    restarted.file.close(); restarted.isOpen = false;
    std::puts("PASS: rotation, restart, legacy backups, retention boundary, daily cleanup");
    return 0;
}

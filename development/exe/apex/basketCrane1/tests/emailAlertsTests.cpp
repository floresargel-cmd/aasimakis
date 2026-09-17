#include "../emailAlerts.h"
#include "../configuration/ConfigurationManager.h"
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTemporaryDir>
#include <QSettings>
#include <QFile>
#include <QMutexLocker>
#include <QAtomicInt>
#include <cstdio>
#include <functional>
#include <stdexcept>

static void require(bool pass, const char* message)
{
    if (!pass) throw std::runtime_error(message);
}

static bool waitUntil(const std::function<bool()>& condition, int timeout = 35000)
{
    QElapsedTimer timer;
    timer.start();
    while (!condition() && timer.elapsed() < timeout) QThread::msleep(20);
    return condition();
}

// Loopback only: no production PLC, database or external mail server is used.
class SmtpFixture : public QThread
{
public:
    SmtpFixture() : port_(0), reject_(0), attempts_(0), connections_(0), stall_(false) { start(); }
    ~SmtpFixture() { requestInterruption(); wait(); }
    int port() { QMutexLocker l(&lock_); return port_; }
    int count() { QMutexLocker l(&lock_); return messages_.size(); }
    int attempts() { QMutexLocker l(&lock_); return attempts_; }
    int connections() { QMutexLocker l(&lock_); return connections_; }
    void stall(bool value) { QMutexLocker l(&lock_); stall_ = value; }
    QByteArray message(int index) { QMutexLocker l(&lock_); return messages_.at(index); }
    void rejectNext() { QMutexLocker l(&lock_); ++reject_; }
protected:
    void run() override {
        QTcpServer server;
        if (!server.listen(QHostAddress::LocalHost, 0)) return;
        { QMutexLocker l(&lock_); port_ = server.serverPort(); }
        while (!isInterruptionRequested()) {
            if (!server.waitForNewConnection(100)) continue;
            QTcpSocket* socket = server.nextPendingConnection();
            bool stall;
            { QMutexLocker l(&lock_); ++connections_; stall = stall_; }
            if (!stall) reply(socket, "220 localhost test SMTP\r\n");
            bool data = false;
            QByteArray body;
            while (!isInterruptionRequested() && socket->state() == QAbstractSocket::ConnectedState) {
                if (!socket->canReadLine() && !socket->waitForReadyRead(100)) continue;
                if (!socket->canReadLine()) continue;
                QByteArray line = socket->readLine();
                if (data) {
                    if (line == ".\r\n") {
                        bool reject;
                        { QMutexLocker l(&lock_); ++attempts_; reject = reject_ > 0;
                          if (reject) --reject_; else messages_.append(body); }
                        reply(socket, reject ? "451 try later\r\n" : "250 accepted\r\n");
                        data = false;
                    } else body += line;
                } else if (line.startsWith("EHLO") || line.startsWith("HELO")) reply(socket, "250 localhost\r\n");
                else if (line.startsWith("DATA")) { data = true; reply(socket, "354 send data\r\n"); }
                else if (line.startsWith("QUIT")) { reply(socket, "221 bye\r\n"); break; }
                else reply(socket, "250 OK\r\n");
            }
            delete socket;
        }
    }
private:
    static void reply(QTcpSocket* s, const QByteArray& text) { s->write(text); s->waitForBytesWritten(1000); }
    QMutex lock_;
    int port_, reject_, attempts_, connections_;
    bool stall_;
    QList<QByteArray> messages_;
};

static int errorCallbacks = 0, statusCallbacks = 0;
static QAtomicInt deliveryFailures(0);
static void deliveryStatus(AppLogEventType event, AppLogSeverity severity, const QString&, const QString& title, const QString&) {
    if (event == APPLOG_EVENT_WRITE && severity == APPLOG_SEV_WARNING && title == "EMAIL ALERT")
        deliveryFailures.ref();
}
static QByteArray decodedBody(const QByteArray& message) {
    return QByteArray::fromBase64(message.mid(message.indexOf("\r\n\r\n") + 4));
}
static void errorCallback(AppLogEventType, AppLogSeverity, const QString&, const QString&, const QString&) { ++errorCallbacks; }
static void statusCallback(AppLogEventType e, AppLogSeverity, const QString&, const QString&, const QString&) {
    if (e == APPLOG_EVENT_WRITE) ++statusCallbacks;
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("BasketCraneEmailTests");
    QCoreApplication::setApplicationVersion("test-version");
    applog_setEnabled(false);
    try {
        require(argc == 2 || argc == 3, "Supply the source basketCrane.ini path (optional --quick skips timed batching checks)");
        const bool quick = argc == 3 && QByteArray(argv[2]) == "--quick";
        const int expectedDelivered = quick ? 2 : 4;
        QTemporaryDir directory;
        require(directory.isValid(), "Temporary directory");
        const QString config = directory.path() + "/test.ini";
        require(QFile::copy(QString::fromLocal8Bit(argv[1]), config), "Copy config fixture");
        SmtpFixture smtp;
        require(waitUntil([&] { return smtp.port() != 0; }), "Local SMTP startup");
        QSettings ini(config, QSettings::IniFormat);
        ini.setValue("EmailAlerts/EnableErrorEmailNotification", true);
        ini.setValue("EmailAlerts/SmtpHost", "127.0.0.1");
        ini.setValue("EmailAlerts/SmtpPort", smtp.port());
        ini.setValue("EmailAlerts/SmtpEnableSsl", false);
        ini.setValue("EmailAlerts/SmtpUsername", "");
        ini.setValue("EmailAlerts/SmtpPassword", "");
        ini.setValue("EmailAlerts/SmtpFrom", "crane@example.test");
        ini.setValue("EmailAlerts/SmtpTo", "operator@example.test;maintenance@example.test");
        ini.setValue("EmailAlerts/ErrorEmailIntervalMinutes", 1);
        ini.setValue("EmailAlerts/SmtpTimeoutSeconds", 20);
        ini.setValue("EmailAlerts/SmtpSubjectPrefix", "[Basket Crane 1 Error] : ");
        ini.sync();
        auto& configuration = BasketCrane::Configuration::ConfigurationManager::Instance();
        QString error;
        require(configuration.Load(config, &error), "Load configuration");
        require(configuration.EmailAlerts().ValidationError().isEmpty(), "Validate email settings");

        applog_setStatusCallback(statusCallback);
        applog_setErrorCallback(errorCallback);
        LOG("INFO", "ignore"); LOG("WARNING", "ignore"); LOG("ERROR: DATABASE", "test");
        applog_write(APPLOG_SEV_EXIT, "fatal", "test");
        require(errorCallbacks == 2 && statusCallbacks == 4, "Independent callbacks and error severity filtering");
        applog_setErrorCallback(0);
        applog_setStatusCallback(0);
        {
            EmailAlerts alerts(configuration.EmailAlerts());
            LOG("WARNING", "do not email");
            QThread::msleep(300);
            require(smtp.count() == 0, "Warning was not emailed");
            QElapsedTimer nonblocking; nonblocking.start();
            LOG("ERROR: DATABASE", QString::fromUtf8("first-error <&> $() ` literal \xC3\xA9"));
            require(nonblocking.elapsed() < 200, "Error logging must not block on SMTP");
            require(waitUntil([&] { return smtp.count() == 1; }), "First email delivery");
            const QByteArray first = smtp.message(0);
            require(first.contains("Subject: [Basket Crane 1 Error] : Application Error"), "Exact subject");
            require(first.contains("operator@example.test") && first.contains("maintenance@example.test"), "Configured recipients");
            require(decodedBody(first).contains("first-error <&> $() ` literal \xC3\xA9")
                && decodedBody(first).contains("test-version"), "Unicode and literal message content");
            std::puts("PASS: first delivery, subject, recipients, Unicode, nonblocking severity filtering"); std::fflush(stdout);
            if (!quick) {
            LOG("ERROR", "batch-one"); LOG("ERROR", "batch-two");
            QThread::msleep(200);
            require(smtp.count() == 1, "Cooldown prevents immediate repeats");
            require(waitUntil([&] { return smtp.count() == 2; }, 75000), "Batched delivery");
            require(decodedBody(smtp.message(1)).contains("Error events in this batch: 2")
                && decodedBody(smtp.message(1)).contains("batch-one")
                && decodedBody(smtp.message(1)).contains("batch-two"), "Batched errors retained");
            std::puts("PASS: one-minute cooldown and combined error batch"); std::fflush(stdout);
            smtp.rejectNext();
            LOG("ERROR", "retry-me");
            require(waitUntil([&] { return smtp.attempts() >= 3; }, 75000), "Simulated SMTP rejection");
            require(waitUntil([&] { return smtp.count() == 3; }, 75000), "Failed batch retried");
            require(decodedBody(smtp.message(2)).contains("retry-me"), "Retry retained the original error");
            std::puts("PASS: rejected message retried without recursive error emails"); std::fflush(stdout);
            }
            LOG("ERROR", QString(40000, 'x'));
            alerts.stop();
            require(smtp.count() == expectedDelivered, "Shutdown flushes pending batch");
            require(smtp.message(expectedDelivered - 1).size() < 30000, "Email body stays bounded");
        }
        ini.setValue("EmailAlerts/SmtpTimeoutSeconds", 1); ini.sync();
        require(configuration.Load(config, &error), "Timeout config");
        smtp.stall(true);
        applog_setStatusCallback(deliveryStatus);
        {
            EmailAlerts timeout(configuration.EmailAlerts());
            QElapsedTimer deadline; deadline.start();
            LOG("ERROR", QString(40000, 'z'));
            require(waitUntil([] { return deliveryFailures.load() > 0; }, 12000), "Stalled SMTP times out");
            require(deadline.elapsed() < 12000, "Delivery deadline remains bounded");
            deadline.restart();
            timeout.stop();
            require(deadline.elapsed() < 8000, "Shutdown is bounded with stalled SMTP");
        }
        smtp.stall(false);
        require(smtp.count() == expectedDelivered, "Stalled server cannot produce a successful delivery");
        ini.setValue("EmailAlerts/SmtpEnableSsl", true); ini.sync(); configuration.Load(config, &error);
        deliveryFailures.store(0);
        const int connections = smtp.connections();
        {
            EmailAlerts tls(configuration.EmailAlerts());
            LOG("ERROR", "Require TLS; do not downgrade to plaintext");
            require(waitUntil([&] { return smtp.connections() > connections && deliveryFailures.load() > 0; }), "Missing STARTTLS is rejected");
        }
        require(smtp.count() == expectedDelivered, "STARTTLS failure sent no plaintext mail");
        applog_setStatusCallback(0);
        const int delivered = smtp.count();
        ini.setValue("EmailAlerts/EnableErrorEmailNotification", false); ini.sync();
        require(configuration.Load(config, &error), "Disabled config");
        { EmailAlerts disabled(configuration.EmailAlerts()); LOG("ERROR", "disabled"); }
        QThread::msleep(200);
        require(smtp.count() == delivered, "Disabled means no email");
        ini.setValue("EmailAlerts/EnableErrorEmailNotification", true);
        ini.setValue("EmailAlerts/SmtpEnableSsl", "invalid"); ini.sync();
        require(configuration.Load(config, &error), "Bad email config does not prevent crane startup");
        require(!configuration.EmailAlerts().ValidationError().isEmpty(), "Reject malformed TLS setting");
        { EmailAlerts invalid(configuration.EmailAlerts()); LOG("ERROR", "invalid config"); }
        require(smtp.count() == delivered, "Invalid config means no email");
        ini.setValue("EmailAlerts/SmtpEnableSsl", true);
        ini.setValue("EmailAlerts/SmtpUsername", "test");
        qputenv("BASKET_CRANE_EMAIL_TEST_SECRET", "test-secret");
        ini.setValue("EmailAlerts/SmtpPassword", "${ENV:BASKET_CRANE_EMAIL_TEST_SECRET}"); ini.sync();
        require(configuration.Load(config, &error), "Environment password configuration");
        require(configuration.EmailAlerts().Password() == "test-secret", "Environment password expansion");
        require(configuration.EmailAlerts().ValidationError().isEmpty(), "STARTTLS credentials accepted");
        ini.setValue("EmailAlerts/SmtpEnableSsl", false); ini.sync(); configuration.Load(config, &error);
        require(!configuration.EmailAlerts().ValidationError().isEmpty(), "Reject credentials without TLS");
        ini.setValue("EmailAlerts/SmtpUsername", "");
        ini.setValue("EmailAlerts/SmtpSubjectPrefix", "bad\r\nBcc: injected@example.test"); ini.sync(); configuration.Load(config, &error);
        require(!configuration.EmailAlerts().ValidationError().isEmpty(), "Reject header injection");
        ini.remove("EmailAlerts"); ini.sync();
        require(configuration.Load(config, &error) && !configuration.EmailAlerts().Enabled(), "Existing INIs without email settings remain supported");
        std::puts("PASS: delivery, subject, recipients, nonblocking logging, severity filtering, batching, retry, timeout, shutdown, bounded payload, disabled/invalid configuration, password expansion and STARTTLS rejection");
        return 0;
    } catch (const std::exception& failure) {
        std::fprintf(stderr, "FAIL: %s\n", failure.what());
        return 1;
    }
}

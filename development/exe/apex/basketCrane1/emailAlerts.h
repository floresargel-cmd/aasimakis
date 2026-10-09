#pragma once

#include "configuration/Settings.h"
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>
#include "../common/applogger.h"

// A bounded queue and a separate worker keep SMTP off the crane/UI threads.
class EmailAlerts : public QThread
{
public:
    explicit EmailAlerts(const BasketCrane::Configuration::EmailAlertSettings& settings);
    ~EmailAlerts();
    void stop();

protected:
    void run() override;

private:
    static void onError(AppLogEventType event, AppLogSeverity severity,
        const QString& path, const QString& title, const QString& message);
    bool send(const QString& body);
    BasketCrane::Configuration::EmailAlertSettings settings_;
    QMutex mutex_;
    QWaitCondition wake_;
    QString pending_;
    quint64 count_;
    bool stopping_;
    QElapsedTimer lastAttempt_;
};

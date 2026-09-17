#pragma once

#include <QString>

// Existing globals (kept for compatibility)
extern bool theOneBCToRuleThemAll; // true if this host is enabled in enable.cfg
extern QString appHostname;

QString getAppHostname();
bool isAppHostEnabled(const QString& hostname);

// Logging API (kept for compatibility)
void applog(QString title, QString mess);
#define LOG(title, mess) (applog((title), (mess)))

// Initialize once, after QApplication has been created. Logs are written to a
// per-user, writable "logs" directory and retained for retentionDays.
void applog_initialize(const QString& applicationName, int retentionDays = 30);
void applog_shutdown();

// ---- New: log status API for UI ----

enum AppLogSeverity {
    APPLOG_SEV_DEBUG = 0,
    APPLOG_SEV_INFO = 1,
    APPLOG_SEV_WARNING = 2,
    APPLOG_SEV_ERROR = 3,
	APPLOG_SEV_EXIT = 4
};

enum AppLogEventType {
    APPLOG_EVENT_CLOSED = 0,    // file closed (process exit cleanup)
    APPLOG_EVENT_OPENED = 1,    // file opened successfully
    APPLOG_EVENT_WRITE = 2,     // a log line was written
    APPLOG_EVENT_OPEN_FAILED = 3 // file failed to open for append
};

// Callback signature:
// - eventType: opened/write/open_failed/closed
// - severity: derived from title/mess ("ERROR" => error; "WARNING" => warning; else info). For OPEN_FAILED it's error.
// - fullPath: absolute file path when known, else empty
// - title/mess: the original log call content (may be empty for CLOSED)
typedef void (*AppLogStatusCallback)(
    AppLogEventType eventType,
    AppLogSeverity severity,
    const QString& fullPath,
    const QString& title,
    const QString& mess
    );

// Set a callback that is invoked synchronously from inside applog().
// Keep the callback fast (update a label, set flags, enqueue UI update, etc.).
void applog_setStatusCallback(AppLogStatusCallback cb);

// Independent notification sink; called outside the logger lock for errors and
// fatal entries even when file logging is disabled/unavailable. Must not block.
void applog_setErrorCallback(AppLogStatusCallback cb);

// Query helpers (safe to call anytime)
bool applog_isOpen();
QString applog_fullPath();

// Optional: enable/disable actual file writing (callbacks still fire)
void applog_setEnabled(bool enabled);
bool applog_isEnabled();

// Write with an explicit severity. Existing call sites can continue to use
// LOG(); this API is for Qt and cross-cutting error handlers.
void applog_write(AppLogSeverity severity, const QString& title, const QString& mess);

// Send qDebug/qWarning/qCritical/qFatal output to the central log.
void applog_installQtMessageHandler();

// Opt-in hook for the shared utilities headers.
void utilitiesAppLog(const QString& title, const QString& mess, bool fatal);

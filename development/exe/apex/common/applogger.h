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

// ---- New: log status API for UI ----

enum AppLogSeverity {
    APPLOG_SEV_INFO = 0,
    APPLOG_SEV_WARNING = 1,
    APPLOG_SEV_ERROR = 2,
	APPLOG_SEV_EXIT = 3
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

// Query helpers (safe to call anytime)
bool applog_isOpen();
QString applog_fullPath();

// Optional: enable/disable actual file writing (callbacks still fire)
void applog_setEnabled(bool enabled);
bool applog_isEnabled();
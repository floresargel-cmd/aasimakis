# Basket Crane troubleshooting log

## Error email alerts

Set `[EmailAlerts]` in the same `basketCrane.ini` used by the application (including
when selected with `--config`). Restart after editing. The checked-in settings
enable alerts through the configured relay:

```ini
[EmailAlerts]
EnableErrorEmailNotification=true
SmtpHost=vitrum-ca.mail.protection.outlook.com
SmtpPort=25
SmtpEnableSsl=false
SmtpFrom=BasketCrane1Application@lorvalcapital.ca
SmtpTo="lcmit@lorvalcapital.atlassian.net"
SmtpSubjectPrefix="[Basket Crane 1 Error] : "
SmtpUsername=
SmtpPassword=${ENV:BASKET_CRANE_SMTP_PASSWORD}
ErrorEmailIntervalMinutes=60
SmtpTimeoutSeconds=20
```

The configured subject is **[Basket Crane 1 Error] : Application Error**.
`SmtpSubjectPrefix` is prepended to `Application Error`. Without a prefix, the
subject defaults to **Basket Crane 1 Application Error**. The body contains the
computer name, application version, timestamp, error details and local log path.
Recipients are a single email address or a quoted, semicolon-separated list of
plain email addresses. The password may be entered directly in the INI or supplied
through an environment variable available to the Windows account running the app.
Credentials are not put in process arguments or alert delivery logs.

When `SmtpEnableSsl=true`, the Windows PowerShell/.NET sender uses STARTTLS with
certificate validation and TLS 1.2 because this application's Qt build has SSL disabled. Use an SMTP service
that permits username/password authentication, or an approved anonymous relay with
blank username/password. `SmtpEnableSsl=false` is supported only without credentials.
Implicit TLS on port 465 and OAuth-only mail accounts are not supported.
See [Microsoft's SMTP TLS documentation](https://learn.microsoft.com/en-us/dotnet/api/system.net.mail.smtpclient.enablessl).

All central logger `ERROR` and `EXIT` entries trigger alerts, including final
database failures and Qt critical messages. Warnings and informational messages do
not. PLC alarms written only to `logErrorPlc` are not application-log errors and
are not included. Valid email settings are loaded before validating other startup
settings so those configuration errors can also be reported. If email settings
are invalid, a warning is logged and email is disabled without stopping the crane.

Sending runs on a separate worker; SMTP waits do not block crane or UI operations.
The first error is queued immediately. Subsequent errors are batched at most once
per `ErrorEmailIntervalMinutes` (1–1440 minutes, default 60). A batch holds at most
16,384 characters of error details, plus a count of all events; use the local log
for the complete history.
Failed batches retry at the same interval while the app runs. Delivery success
means the SMTP server accepted the message, not guaranteed inbox delivery. Failures
are logged as warnings and do not recursively generate alert emails.

`SmtpTimeoutSeconds` (1–60, default 20) bounds SMTP work; a process watchdog adds five
seconds for helper startup/cleanup. Normal shutdown attempts to flush pending
errors, allowing up to five seconds for each of an in-flight and final batch.
The queue is in memory: unsent alerts are lost on exit. Hard crashes, forced
termination and `std::abort` cannot guarantee delivery.

### Local verification

Build `tests/EmailAlerts.Tests.vcxproj` with Debug/Win32, then run
`development/#builds/EmailAlerts.Tests/EmailAlerts.Tests.exe` with the full path to
the source `basketCrane.ini`. Tests use a temporary INI and a loopback SMTP server;
they do not connect to production PLCs, databases or email recipients.

## Local logs

Basket Crane writes application diagnostics to one daily UTF-8 log:

`%LOCALAPPDATA%\Lorval Capital Management\ApexBasketCrane\logs\basket-crane.YYYY-MM-DD.log`

The log includes the timestamp, severity, process ID, thread ID, component, and message. It combines application events, Qt messages, configuration failures, database failures, slow database queries, unhandled C++ exceptions, and stacker PLC state diagnostics.

After each database connection opens, a `DATABASE CONNECTION` entry records the connection name, selected profile, driver, DSN, SQL Server name, active database, user, and the principal tables read through that connection. Passwords are never logged. In particular, the entry for `mainConnection` identifies where the `positions` and `baskets` tables are read and written.

Logs automatically roll over at midnight and when the current file reaches 25 MB. Up to five size-based backups are retained for the day, individual messages are truncated at 16,384 characters, and files older than 30 days are removed when the logger starts or rolls over.

The default level is `INFO`. To include high-volume timer and Qt debug messages, set this environment variable before starting the application:

`BASKET_CRANE_LOG_LEVEL=DEBUG`

The existing database tables (`logCranePlc`, `logResetPlc`, and `logErrorPlc`) remain available as operational history, but standalone text files such as `stackerBitsLog.*.txt`, `warning.txt`, and `errorLogs/error.*.txt` are no longer created by Basket Crane.

At startup, basket details, aging timestamps, and temper mappings are loaded with three bulk queries and cached in memory. A basket arriving from the stacker refreshes only its own cache entry.

Database writes run inside explicit SQL transactions when the driver supports them. SQL Server deadlock victims (`1205`), lock timeouts (`1222`), and retryable SQLSTATE timeout/serialization failures are rolled back and retried up to three times with exponential backoff. Every retry, rollback failure, final SQL failure, and slow transaction is written to the centralized log. Multi-statement operations can use `execDBTransaction()` so the complete operation commits or rolls back as one unit; a deadlock retries the entire transaction.

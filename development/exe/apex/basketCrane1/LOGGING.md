# Basket Crane troubleshooting log

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

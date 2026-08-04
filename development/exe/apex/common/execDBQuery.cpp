#include "execDBQuery.h"
#include "applogger.h"
#include <QElapsedTimer>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QThread>

namespace {
const int DEFAULT_MAX_RETRIES = 3;
const int INITIAL_RETRY_DELAY_MS = 200;

struct QueryResult
{
    bool success;
    QSqlError error;
    QVector<QVariant> values;
    QVector<QVector<QVariant> > rows;

    QueryResult() : success(false) {}
};

QString sqlDiagnostic(const QSqlError& error)
{
    return QString("native=%1; driver=%2; database=%3")
        .arg(error.nativeErrorCode())
        .arg(error.driverText())
        .arg(error.databaseText());
}

bool isRetryableSqlError(const QSqlError& error)
{
    const QString nativeCode = error.nativeErrorCode().trimmed().toUpper();
    const QString diagnostic = sqlDiagnostic(error).toUpper();

    // SQL Server: 1205 = deadlock victim, 1222 = lock request timeout.
    // SQLSTATE 40001 = serialization/deadlock; HYT00/HYT01 = timeout.
    return nativeCode == "1205"
        || nativeCode == "1222"
        || nativeCode.contains("40001")
        || nativeCode.contains("HYT00")
        || nativeCode.contains("HYT01")
        || diagnostic.contains("DEADLOCK VICTIM")
        || diagnostic.contains("WAS DEADLOCKED")
        || diagnostic.contains("LOCK REQUEST TIME OUT PERIOD EXCEEDED");
}

bool isDeadlockSqlError(const QSqlError& error)
{
    const QString nativeCode = error.nativeErrorCode().trimmed().toUpper();
    const QString diagnostic = sqlDiagnostic(error).toUpper();
    return nativeCode == "1205"
        || diagnostic.contains("DEADLOCK VICTIM")
        || diagnostic.contains("WAS DEADLOCKED");
}

bool isWriteStatement(const QString& statement)
{
    const QString firstWord = statement.trimmed().section(QChar(' '), 0, 0).toLower();
    return firstWord == "insert"
        || firstWord == "update"
        || firstWord == "delete"
        || firstWord == "merge";
}

void waitBeforeRetry(int retryNumber)
{
    // Short exponential backoff: 200 ms, 400 ms, 800 ms. The small
    // deterministic offset prevents two Basket Crane processes from repeatedly
    // colliding on exactly the same schedule.
    const unsigned long delayMs = static_cast<unsigned long>(
        INITIAL_RETRY_DELAY_MS * (1 << (retryNumber - 1))
        + ((QCoreApplication::applicationPid() + retryNumber * 37) % 101));
    QThread::msleep(delayMs);
}

QueryResult executeOnce(QSqlDatabase& database, const QString& statement, bool returnTable)
{
    QueryResult result;
    QSqlQuery query(database);
    query.setForwardOnly(true);

    if (!query.exec(statement)) {
        result.error = query.lastError();
        return result;
    }

    while (query.next()) {
        if (returnTable) {
            QVector<QVariant> row;
            const QSqlRecord record = query.record();
            for (int column = 0; column < record.count(); ++column)
                row.append(record.value(column));
            result.rows.append(row);
        }
        else {
            result.values.append(query.value(0));
        }
    }

    if (query.lastError().isValid()) {
        result.error = query.lastError();
        return result;
    }

    result.success = true;
    return result;
}

QueryResult executeWithRetry(const QString& statement, const QString& connection, bool returnTable)
{
    QElapsedTimer timer;
    timer.start();
    QueryResult result;

    for (int attempt = 0; attempt <= DEFAULT_MAX_RETRIES; ++attempt) {
        QSqlDatabase database = QSqlDatabase::database(connection);
        const bool useTransaction = isWriteStatement(statement)
            && database.driver()
            && database.driver()->hasFeature(QSqlDriver::Transactions);
        bool transactionStarted = false;

        if (useTransaction) {
            transactionStarted = database.transaction();
            if (!transactionStarted) {
                result = QueryResult();
                result.error = database.lastError();
            }
        }

        if (!useTransaction || transactionStarted)
            result = executeOnce(database, statement, returnTable);

        if (result.success && transactionStarted && !database.commit()) {
            result.success = false;
            result.error = database.lastError();
        }

        bool rollbackSucceeded = true;
        if (!result.success && transactionStarted) {
            rollbackSucceeded = database.rollback();
            // SQL Server automatically rolls back a 1205 victim. In that case
            // the driver's follow-up rollback can legitimately report that no
            // transaction exists, and retrying is still safe.
            if (!rollbackSucceeded && !isDeadlockSqlError(result.error)) {
                LOG("ERROR: DATABASE", QString("Rollback failed on connection '%1': %2")
                    .arg(connection).arg(sqlDiagnostic(database.lastError())));
            }
        }

        if (result.success) {
            const qint64 elapsedMs = timer.elapsed();
            if (elapsedMs >= 1000) {
                LOG("WARNING: DATABASE", QString("Slow query (%1 ms) on connection '%2': %3")
                    .arg(elapsedMs).arg(connection).arg(statement));
            }
            return result;
        }

        const bool canRetry = (rollbackSucceeded || isDeadlockSqlError(result.error))
            && isRetryableSqlError(result.error)
            && attempt < DEFAULT_MAX_RETRIES;
        if (!canRetry)
            break;

        const int retryNumber = attempt + 1;
        LOG("WARNING: DATABASE", QString("Transient SQL failure on connection '%1'; retry %2/%3. %4; statement: %5")
            .arg(connection).arg(retryNumber).arg(DEFAULT_MAX_RETRIES)
            .arg(sqlDiagnostic(result.error)).arg(statement));
        waitBeforeRetry(retryNumber);
    }

    LOG("ERROR: DATABASE", QString("SQL query failed after retries on connection '%1': %2; statement: %3")
        .arg(connection).arg(sqlDiagnostic(result.error)).arg(statement));
    uExit(errorStr, QString("Cannot execute SQL statement: %1\nSQL server: %2")
        .arg(statement).arg(result.error.text()));
    return result;
}
}

QVector<QVariant> execDBQuery(QString stmnt, QString connection)
{
    return executeWithRetry(stmnt, connection, false).values;
}

QVector<QVector<QVariant> > execDBTableQuery(QString stmnt, QString connection)
{
    return executeWithRetry(stmnt, connection, true).rows;
}

bool execDBTransaction(const QStringList& statements, QString connection, int maxRetries)
{
    if (statements.isEmpty())
        return true;

    if (maxRetries < 0)
        maxRetries = 0;
    if (maxRetries > 6)
        maxRetries = 6;

    QElapsedTimer timer;
    timer.start();

    for (int attempt = 0; attempt <= maxRetries; ++attempt) {
        QSqlDatabase database = QSqlDatabase::database(connection);
        if (!database.driver() || !database.driver()->hasFeature(QSqlDriver::Transactions)) {
            LOG("ERROR: DATABASE", QString("Connection '%1' does not support SQL transactions")
                .arg(connection));
            return false;
        }

        if (!database.transaction()) {
            LOG("ERROR: DATABASE", QString("Could not begin transaction on connection '%1': %2")
                .arg(connection).arg(sqlDiagnostic(database.lastError())));
            return false;
        }

        QSqlError failure;
        QString failedStatement;
        for (int i = 0; i < statements.count(); ++i) {
            const QueryResult result = executeOnce(database, statements[i], false);
            if (!result.success) {
                failure = result.error;
                failedStatement = statements[i];
                break;
            }
        }

        if (!failure.isValid() && database.commit()) {
            const qint64 elapsedMs = timer.elapsed();
            if (elapsedMs >= 1000) {
                LOG("WARNING: DATABASE", QString("Slow transaction (%1 ms, %2 statements) on connection '%3'")
                    .arg(elapsedMs).arg(statements.count()).arg(connection));
            }
            return true;
        }

        if (!failure.isValid()) {
            failure = database.lastError();
            failedStatement = "COMMIT";
        }

        const bool rollbackSucceeded = database.rollback();
        const bool serverRolledBackDeadlock = isDeadlockSqlError(failure);
        if (!rollbackSucceeded && !serverRolledBackDeadlock) {
            LOG("ERROR: DATABASE", QString("Transaction rollback failed on connection '%1': %2")
                .arg(connection).arg(sqlDiagnostic(database.lastError())));
            return false;
        }

        if (!isRetryableSqlError(failure) || attempt >= maxRetries) {
            LOG("ERROR: DATABASE", QString("Transaction failed on connection '%1': %2; failed statement: %3")
                .arg(connection).arg(sqlDiagnostic(failure)).arg(failedStatement));
            return false;
        }

        const int retryNumber = attempt + 1;
        LOG("WARNING: DATABASE", QString("Transaction deadlock/timeout on connection '%1'; rolled back and retrying %2/%3. %4")
            .arg(connection).arg(retryNumber).arg(maxRetries).arg(sqlDiagnostic(failure)));
        waitBeforeRetry(retryNumber);
    }

    return false;
}

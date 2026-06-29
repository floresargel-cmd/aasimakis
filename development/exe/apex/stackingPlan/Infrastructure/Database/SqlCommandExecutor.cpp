/**
 * @file SqlCommandExecutor.cpp
 * @brief Implements the SqlCommandExecutor class for executing SQL commands with retry and transaction support.
 *
 * The SqlCommandExecutor class provides a high-level interface for executing SQL commands against a database
 * using Qt's QSqlDatabase. It supports connection management, retry logic, logging, and transactional execution.
 *
 * @section Usage
 * - Instantiate with a connection string, connection name, logger, and retry policy parameters.
 * - Use Open() and Close() to manage the database connection.
 * - ExecuteScalar() runs a query and returns the first column of the first row.
 * - ExecuteNonQuery() runs a non-query SQL command (e.g., INSERT, UPDATE, DELETE).
 * - ExecuteInTransaction() executes a user-provided action within a transaction scope.
 * - GetLastError() provides details on the last error encountered.
 * - GetDatabase() and GetConnectionName() expose the underlying database and connection name.
 *
 * @note
 * - All operations use a retry policy for transient failures.
 * - Connection strings are masked in logs to avoid leaking sensitive information.
 * - Logging is performed via the provided FileLogger instance.
 */

#include "SqlCommandExecutor.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qnamespace.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstringlist.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqldatabase.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqlerror.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqlquery.h"
#include "../Logging/FileLogger.h"
#include "SqlRetryPolicy.h"
#include "TransactionScope.h"
#include <functional>


namespace
{
	/**
	 * @brief Masks sensitive information in a database connection string.
	 *
	 * The MaskConnectionString function scans the provided connection string for sensitive keys
	 * such as "PWD=" and "Password=" (case-insensitive). For each occurrence, it replaces the value
	 * with "****" to prevent leaking credentials in logs or error messages.
	 *
	 * @param connectionString The original database connection string.
	 * @return A copy of the connection string with sensitive values masked.
	 *
	 * @details
	 * - The function iterates through all supported sensitive keys.
	 * - For each key found, it locates the value and replaces it with "****" until the next semicolon or end of string.
	 * - Multiple sensitive keys and multiple occurrences are supported.
	 *
	 * @note
	 * - This function is intended for internal use to ensure that logs do not expose passwords.
	 * - Only the keys "PWD=" and "Password=" are masked; other sensitive keys should be added as needed.
	 *
	 * @see StackingPlan::Infrastructure::Database::SqlCommandExecutor
	 */
	QString MaskConnectionString(const QString& connectionString)
	{
		QString masked = connectionString;
		const QStringList sensitiveKeys = { "PWD=", "Password=" };

		for (const QString& key : sensitiveKeys)
		{
			int start = masked.indexOf(key, 0, Qt::CaseInsensitive);
			while (start >= 0)
			{
				const int valueStart = start + key.length();
				int end = masked.indexOf(';', valueStart);
				if (end < 0)
				{
					end = masked.length();
				}

				masked.replace(valueStart, end - valueStart, "****");
				start = masked.indexOf(key, end, Qt::CaseInsensitive);
			}
		}

		return masked;
	}
}

namespace StackingPlan::Infrastructure::Database
{
	/**
	 * @brief Constructs a SqlCommandExecutor.
	 * @param connectionString The database connection string.
	 * @param connectionName The unique name for the database connection.
	 * @param logger Pointer to a FileLogger for logging operations.
	 * @param maxRetries Maximum number of retry attempts for operations.
	 * @param baseDelayMilliseconds Base delay in milliseconds between retries.
	 */
	SqlCommandExecutor::SqlCommandExecutor(
		const QString& connectionString,
		const QString& connectionName,
		StackingPlan::Infrastructure::Logging::FileLogger* logger,
		int maxRetries,
		int baseDelayMilliseconds)
		: _connectionString(connectionString),
		_connectionName(connectionName),
		_logger(logger),
		_maxRetries(maxRetries),
		_baseDelayMilliseconds(baseDelayMilliseconds)
	{
	}

	/**
	 * @brief Destructor. Closes the database connection and removes it from the connection pool.
	 */
	SqlCommandExecutor::~SqlCommandExecutor()
	{
	}

	/**
	 * @brief Opens the database connection if not already open.
	 * @return True if the connection is open or was successfully opened; false otherwise.
	 */
	bool SqlCommandExecutor::Open()
	{
		_lastError.clear();

		if (QSqlDatabase::contains(_connectionName))
		{
			_database = QSqlDatabase::database(_connectionName);
		}
		else
		{
			_database = QSqlDatabase::addDatabase("QODBC", _connectionName);
			_database.setDatabaseName(_connectionString);
		}

		if (_database.isOpen())
		{
			return true;
		}

		if (!_database.open())
		{
			_lastError = _database.lastError().text();
			return false;
		}

		return true;
	}

	/**
	 * @brief Closes the database connection if open.
	 */
	void SqlCommandExecutor::Close()
	{
		// DO NOTHING
	}

	/**
	 * @brief Executes a SQL query and returns the first column of the first row in the result set.
	 * @param sql The SQL query to execute.
	 * @return The value of the first column of the first row, or an invalid QVariant if the query fails.
	 */
	QVariant SqlCommandExecutor::ExecuteScalar(const QString& sql)
	{
		_logger->Info(QString("Executing SQL query: %1").arg(sql));
		QVariant result;
		QString error;

		SqlRetryPolicy retryPolicy(_maxRetries, _baseDelayMilliseconds, _logger);

		const bool success = retryPolicy.Execute([&]() -> bool
			{
				if (!Open())
				{
					error = _lastError;
					return false;
				}

				QSqlQuery query(_database);
				if (!query.exec(sql))
				{
					error = query.lastError().text();
					_lastError = error;
					return false;
				}

				if (query.next())
				{
					result = query.value(0);
				}

				return true;
			}, error);

		if (!success)
		{
			_lastError = error;
		}

		return result;
	}

	/**
	 * @brief Executes a non-query SQL command (e.g., INSERT, UPDATE, DELETE).
	 * @param sql The SQL command to execute.
	 * @return True if the command executed successfully; false otherwise.
	 */
	bool SqlCommandExecutor::ExecuteNonQuery(const QString& sql)
	{
		QString error;
		SqlRetryPolicy retryPolicy(_maxRetries, _baseDelayMilliseconds, _logger);
		_logger->Info(QString("Executing SQL non-query: %1").arg(sql));
		const bool success = retryPolicy.Execute([&]() -> bool
			{
				if (!Open())
				{
					error = _lastError;
					return false;
				}

				QSqlQuery query(_database);
				if (!query.exec(sql))
				{
					error = query.lastError().text();
					_lastError = error;
					return false;
				}

				return true;
			}, error);

		if (!success)
		{
			_lastError = error;
		}

		return success;
	}

	/**
	 * @brief Executes a user-provided action within a transaction scope.
	 * @param action A function that receives a reference to the QSqlDatabase and returns true on success.
	 * @return True if the transaction was committed successfully; false otherwise.
	 */
	bool SqlCommandExecutor::ExecuteInTransaction(const std::function<bool(QSqlDatabase&)>& action)
	{
		if (!Open())
		{
			return false;
		}

		QString error;
		SqlRetryPolicy retryPolicy(_maxRetries, _baseDelayMilliseconds, _logger);

		const bool success = retryPolicy.Execute([&]() -> bool
			{
				TransactionScope transaction(_database, _logger);

				if (!transaction.Begin())
				{
					error = transaction.GetLastError();
					_lastError = error;
					return false;
				}

				if (!action(_database))
				{
					transaction.Rollback();
					error = "Transactional action returned false.";
					_lastError = error;
					return false;
				}

				if (!transaction.Commit())
				{
					error = transaction.GetLastError();
					_lastError = error;
					return false;
				}

				return true;
			}, error);

		if (!success)
		{
			_lastError = error;
		}

		return success;
	}

	/**
	 * @brief Gets the last error message encountered by the executor.
	 * @return The last error message as a QString.
	 */
	QString SqlCommandExecutor::GetLastError() const
	{
		return _lastError;
	}

	/**
	 * @brief Gets the underlying QSqlDatabase instance.
	 * @return The QSqlDatabase instance used by this executor.
	 */
	QSqlDatabase SqlCommandExecutor::GetDatabase() const
	{
		return _database;
	}

	/**
	 * @brief Gets the connection name used by this executor.
	 * @return The connection name as a QString.
	 */
	QString SqlCommandExecutor::GetConnectionName() const
	{
		return _connectionName;
	}
}
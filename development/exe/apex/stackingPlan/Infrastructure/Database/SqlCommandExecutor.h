#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqldatabase.h"
#include <functional>


namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger;
}

namespace StackingPlan::Infrastructure::Database
{
	/**
	 * @brief Executes SQL commands against a database connection with retry and logging support.
	 *
	 * The SqlCommandExecutor class provides methods to open and close a database connection,
	 * execute SQL queries (scalar and non-query), and perform transactional operations.
	 * It supports configurable retry logic for transient failures and optional logging.
	 */
	class SqlCommandExecutor
	{
	public:
		/**
		 * @brief Constructs a SqlCommandExecutor with the specified connection parameters.
		 * @param connectionString The database connection string.
		 * @param connectionName The unique name for the database connection.
		 * @param logger Optional pointer to a FileLogger for logging database operations and errors.
		 * @param maxRetries The maximum number of retry attempts for transient failures (default: 3).
		 * @param baseDelayMilliseconds The base delay in milliseconds between retries (default: 200).
		 */
		SqlCommandExecutor(
			const QString& connectionString,
			const QString& connectionName,
			StackingPlan::Infrastructure::Logging::FileLogger* logger = nullptr,
			int maxRetries = 3,
			int baseDelayMilliseconds = 200);

		/**
		 * @brief Destructor. Closes the database connection if open.
		 */
		~SqlCommandExecutor();

		/**
		 * @brief Opens the database connection.
		 * @return True if the connection was successfully opened; otherwise, false.
		 */
		bool Open();

		/**
		 * @brief Closes the database connection.
		 */
		void Close();

		/**
		 * @brief Executes a SQL query and returns the first column of the first row in the result set.
		 * @param sql The SQL query to execute.
		 * @return The result as a QVariant, or an invalid QVariant if execution fails.
		 */
		QVariant ExecuteScalar(const QString& sql);

		/**
		 * @brief Executes a SQL non-query command (e.g., INSERT, UPDATE, DELETE).
		 * @param sql The SQL command to execute.
		 * @return True if the command executed successfully; otherwise, false.
		 */
		bool ExecuteNonQuery(const QString& sql);

		/**
		 * @brief Executes a function within a database transaction.
		 *        Commits the transaction if the function returns true; otherwise, rolls back.
		 * @param action The function to execute, receiving a reference to the QSqlDatabase.
		 * @return True if the transaction was committed; false if rolled back or an error occurred.
		 */
		bool ExecuteInTransaction(const std::function<bool(QSqlDatabase&)>& action);

		/**
		 * @brief Gets the last error message encountered by the executor.
		 * @return The last error message as a QString.
		 */
		QString GetLastError() const;

		/**
		 * @brief Gets the underlying QSqlDatabase instance.
		 * @return The QSqlDatabase object.
		 */
		QSqlDatabase GetDatabase() const;

		/**
		 * @brief Gets the name of the database connection.
		 * @return The connection name as a QString.
		 */
		QString GetConnectionName() const;

	private:
		QString _connectionString; ///< The database connection string.
		QString _connectionName; ///< The unique name for the database connection.
		QSqlDatabase _database; ///< The underlying Qt SQL database object.
		QString _lastError; ///< The last error message encountered.
		StackingPlan::Infrastructure::Logging::FileLogger* _logger; ///< Optional logger for database operations.
		int _maxRetries; ///< Maximum number of retry attempts for transient failures.
		int _baseDelayMilliseconds; ///< Base delay in milliseconds between retries.
	};
}
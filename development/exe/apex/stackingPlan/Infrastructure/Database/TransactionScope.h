#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqldatabase.h"


namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger;
}

namespace StackingPlan::Infrastructure::Database
{
	/**
	 * @brief Manages a database transaction scope using Qt's QSqlDatabase.
	 *
	 * The TransactionScope class provides a RAII-style mechanism for handling
	 * database transactions. It encapsulates the logic for beginning, committing,
	 * and rolling back transactions, and logs transaction events and errors using
	 * the provided FileLogger.
	 *
	 * Typical usage:
	 * @code
	 * TransactionScope scope(database, logger);
	 * if (scope.Begin()) {
	 *     // Perform database operations
	 *     if (!scope.Commit()) {
	 *         // Handle commit failure
	 *     }
	 * } else {
	 *     // Handle begin failure
	 * }
	 * @endcode
	 */
	class TransactionScope
	{
	public:
		/**
		 * @brief Constructs a TransactionScope for the given database and logger.
		 * @param database The QSqlDatabase connection to use for the transaction.
		 * @param logger Pointer to a FileLogger for logging transaction events and errors.
		 */
		TransactionScope(
			QSqlDatabase database,
			StackingPlan::Infrastructure::Logging::FileLogger* logger);

		/**
		 * @brief Begins a new database transaction.
		 * @return true if the transaction was started successfully; false otherwise.
		 */
		bool Begin();

		/**
		 * @brief Commits the current transaction.
		 * @return true if the transaction was committed successfully; false otherwise.
		 */
		bool Commit();

		/**
		 * @brief Rolls back the current transaction.
		 */
		void Rollback();

		/**
		 * @brief Retrieves the last error message encountered during transaction operations.
		 * @return A QString containing the last error message.
		 */
		QString GetLastError() const;

	private:
		QSqlDatabase _database; ///< The database connection used for the transaction.
		StackingPlan::Infrastructure::Logging::FileLogger* _logger; ///< Logger for transaction events and errors.
		QString _lastError; ///< Stores the last error message.
		bool _isActive; ///< Indicates whether a transaction is currently active.
	};
}
/**
 * @file TransactionScope.cpp
 * @brief Implements the TransactionScope class for managing database transactions.
 *
 * This file defines the methods for the TransactionScope class, which provides
 * a scoped mechanism for handling database transactions using Qt's QSqlDatabase.
 * It supports beginning, committing, and rolling back transactions, and logs
 * rollback events if a logger is provided.
 */

#include "TransactionScope.h"

#include "../Logging/FileLogger.h"

namespace StackingPlan::Infrastructure::Database
{
	/**
	 * @brief Constructs a TransactionScope object.
	 * @param database The QSqlDatabase instance to manage transactions on.
	 * @param logger Optional pointer to a FileLogger for logging transaction events.
	 *
	 * Initializes the transaction scope with the given database and logger.
	 * The transaction is not active upon construction.
	 */
	TransactionScope::TransactionScope(
		QSqlDatabase database,
		StackingPlan::Infrastructure::Logging::FileLogger* logger)
		: _database(database),
		_logger(logger),
		_isActive(false)
	{
	}

	/**
	 * @brief Begins a new database transaction.
	 * @return true if the transaction was successfully started; false otherwise.
	 *
	 * Checks if the database connection is valid and open before starting a transaction.
	 * Sets an error message if the transaction cannot be started.
	 */
	bool TransactionScope::Begin()
	{
		if (!_database.isValid() || !_database.isOpen())
		{
			_lastError = "Transaction begin failed. Database is not open.";
			return false;
		}

		if (!_database.transaction())
		{
			_lastError = "Transaction begin failed.";
			return false;
		}

		_isActive = true;
		return true;
	}

	/**
	 * @brief Commits the current transaction.
	 * @return true if the transaction was successfully committed or not active; false otherwise.
	 *
	 * If no transaction is active, returns true. Otherwise, attempts to commit the transaction.
	 * Sets an error message if the commit fails.
	 */
	bool TransactionScope::Commit()
	{
		if (!_isActive)
		{
			return true;
		}

		if (!_database.commit())
		{
			_lastError = "Transaction commit failed.";
			return false;
		}

		_isActive = false;
		return true;
	}

	/**
	 * @brief Rolls back the current transaction.
	 *
	 * If a transaction is active, rolls it back and logs a warning if a logger is available.
	 * Marks the transaction as inactive after rollback.
	 */
	void TransactionScope::Rollback()
	{
		if (_isActive)
		{
			_database.rollback();
			_isActive = false;

			if (_logger)
			{
				_logger->Warning("Transaction rolled back.");
			}
		}
	}

	/**
	 * @brief Retrieves the last error message.
	 * @return The last error message as a QString.
	 *
	 * Returns the error message set by the last failed transaction operation.
	 */
	QString TransactionScope::GetLastError() const
	{
		return _lastError;
	}
}

/**
 * @file DatabaseInitializer.cpp
 * @brief Implements the DatabaseInitializer class responsible for initializing the application's database schema.
 *
 * This file defines the DatabaseInitializer class, which ensures that required database tables exist
 * and are ready for use by the application. It uses a SqlCommandExecutor to execute SQL scripts and a FileLogger
 * for logging initialization progress and errors.
 */

#include "DatabaseInitializer.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../Logging/FileLogger.h"
#include "../SqlScripts/DatabaseInitializerSqlScripts.h"
#include "SqlCommandExecutor.h"
#include <memory>

namespace StackingPlan::Infrastructure::Database
{
	using StackingPlan::Infrastructure::SqlScripts::DatabaseInitializerSqlScripts;

	/**
	 * @class DatabaseInitializer
	 * @brief Handles database schema initialization tasks.
	 *
	 * The DatabaseInitializer class is responsible for ensuring that the application's database contains
	 * the required tables. It logs the initialization process and errors using the provided FileLogger.
	 */
	DatabaseInitializer::DatabaseInitializer(
		std::shared_ptr<SqlCommandExecutor> sqlCommandExecutor,
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger)
		: _sqlCommandExecutor(sqlCommandExecutor),
		_logger(logger)
	{
	}

	/**
	 * @brief Initializes the database by ensuring required tables exist.
	 *
	 * This method attempts to open the database connection and ensures that the ErrorLog and sync_config tables
	 * are present by executing the corresponding SQL scripts. Logs progress and errors.
	 *
	 * @return true if initialization succeeds; false otherwise.
	 */
	bool DatabaseInitializer::Initialize()
	{
		_logger->Info(QString("Database initialization started."));

		if (!_sqlCommandExecutor->Open())
		{
			_logger->Error(QString("Database open failed: ") + _sqlCommandExecutor->GetLastError());
			return false;
		}

		if (!EnsureErrorLogTable())
		{
			_logger->Error(QString("Failed to ensure ErrorLog table exists. ") + _sqlCommandExecutor->GetLastError());
			return false;
		}

		if (!EnsureSyncConfigTable())
		{
			_logger->Error(QString("Failed to ensure sync_config table exists. ") + _sqlCommandExecutor->GetLastError());
			return false;
		}

		_logger->Info(QString("Database initialization completed."));
		return true;
	}

	/**
	 * @brief Ensures the ErrorLog table exists in the database.
	 *
	 * Executes the SQL script to create the ErrorLog table if it does not already exist.
	 *
	 * @return true if the operation succeeds; false otherwise.
	 */
	bool DatabaseInitializer::EnsureErrorLogTable()
	{
		return _sqlCommandExecutor->ExecuteNonQuery(
			DatabaseInitializerSqlScripts::EnsureErrorLogTable());
	}

	/**
	 * @brief Ensures the sync_config table exists in the database.
	 *
	 * Executes the SQL script to create the sync_config table if it does not already exist.
	 *
	 * @return true if the operation succeeds; false otherwise.
	 */
	bool DatabaseInitializer::EnsureSyncConfigTable()
	{
		return _sqlCommandExecutor->ExecuteNonQuery(
			DatabaseInitializerSqlScripts::EnsureSyncConfigTable());
	}
}
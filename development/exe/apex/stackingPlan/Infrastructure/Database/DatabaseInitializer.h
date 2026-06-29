#pragma once

#include <memory>

namespace StackingPlan::Infrastructure::Database
{
	class SqlCommandExecutor;
}

namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger;
}

namespace StackingPlan::Infrastructure::Database
{
	/**
	 * @class DatabaseInitializer
	 * @brief Responsible for initializing and ensuring the existence of required database tables.
	 *
	 * The DatabaseInitializer class provides methods to set up and verify the presence of
	 * essential tables in the application's database, such as error logging and synchronization
	 * configuration tables. It uses a SqlCommandExecutor to execute SQL commands and a FileLogger
	 * for logging operations and errors during initialization.
	 */
	class DatabaseInitializer
	{
	public:
		/**
		 * @brief Constructs a DatabaseInitializer with the required dependencies.
		 * @param sqlCommandExecutor Shared pointer to a SqlCommandExecutor for executing SQL commands.
		 * @param logger Shared pointer to a FileLogger for logging initialization events and errors.
		 */
		DatabaseInitializer(
			std::shared_ptr<SqlCommandExecutor> sqlCommandExecutor,
			std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger);

		/**
		 * @brief Initializes the database by ensuring all required tables exist.
		 * @return True if initialization succeeds; false otherwise.
		 */
		bool Initialize();

	private:
		/**
		 * @brief Ensures the error log table exists in the database.
		 * @return True if the table exists or is created successfully; false otherwise.
		 */
		bool EnsureErrorLogTable();

		/**
		 * @brief Ensures the synchronization configuration table exists in the database.
		 * @return True if the table exists or is created successfully; false otherwise.
		 */
		bool EnsureSyncConfigTable();

	private:
		std::shared_ptr<SqlCommandExecutor> _sqlCommandExecutor; ///< Executor for SQL commands.
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger; ///< Logger for initialization events and errors.
	};
}
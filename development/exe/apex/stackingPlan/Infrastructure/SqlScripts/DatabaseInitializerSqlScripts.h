#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"


namespace StackingPlan::Infrastructure::SqlScripts
{
	/**
	 * @class DatabaseInitializerSqlScripts
	 * @brief Provides SQL script generators for initializing essential database tables.
	 *
	 * This utility class contains static methods that return SQL statements
	 * required to ensure the existence of core tables used by the application.
	 * The scripts are intended for use during database initialization or migration
	 * processes to create tables if they do not already exist.
	 */
	class DatabaseInitializerSqlScripts
	{
	public:
		/**
		 * @brief Generates a SQL script to ensure the error log table exists.
		 * @return QString containing the SQL statement to create the error log table if it does not exist.
		 */
		static QString EnsureErrorLogTable();

		/**
		 * @brief Generates a SQL script to ensure the sync configuration table exists.
		 * @return QString containing the SQL statement to create the sync configuration table if it does not exist.
		 */
		static QString EnsureSyncConfigTable();
	};
}

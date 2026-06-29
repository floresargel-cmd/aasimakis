#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
/**
 * @file Ap * @brief Defines the AppSettings class for application configuration management.
 *
 * The AppSettings class encapsulates configuration parameters required by the
 * StackingPlan application, such as database connection details, retry logic,
 * and logging directory. This class is intended to be used as a container for
 * settings loaded from configuration files or other sources.
 */

#pragma once


namespace StackingPlan::Application::Services
{
	/**
	 * @class AppSettings
	 * @brief Stores application configuration settings.
	 *
	 * This class provides members for storing essential configuration values:
	 * - Database connection string and name.
	 * - Retry policy parameters for operations (max retries and base delay).
	 * - Directory path for application logs.
	 */
	class AppSettings
	{
	public:
		/**
		 * @brief The database connection string.
		 */
		QString connectionString;

		/**
		 * @brief The name of the database connection.
		 */
		QString connectionName;

		QString basketCraneConnectionString;
		QString basketCraneConnectionName;

		/**
		 * @brief Maximum number of retry attempts for operations.
		 * @details Default value is 3.
		 */
		int maxRetries = 3;

		/**
		 * @brief Base delay in milliseconds between retry attempts.
		 * @details Default value is 200 ms.
		 */
		int baseDelayMilliseconds = 200;

		/**
		 * @brief Directory path where application logs are stored.
		 */
		QString logDirectory;
	};
}
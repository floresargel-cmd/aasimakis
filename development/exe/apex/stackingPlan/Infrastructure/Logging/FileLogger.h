#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/thread/qmutex.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
// FileLogger.h// ---------------//  FileLogger class for the StackingPlan application.
//  Provides thread-safe logging functionality to write informational, warning,
//  and error messages to log files within a specified directory.
//
//  Namespace: StackingPlan::Infrastructure::Logging
// -----------------------------------------------------------------------------
#pragma once


namespace StackingPlan::Infrastructure::Logging
{
	/**
	 * @class FileLogger
	 * @brief Thread-safe logger for writing messages to files.
	 *
	 * The FileLogger class enables logging of informational, warning, and error
	 * messages to files in a specified directory. It uses a mutex to ensure
	 * thread safety when writing log entries.
	 */
	class FileLogger
	{
	public:
		/**
		 * @brief Constructs a FileLogger.
		 * @param logDirectory Directory where log files will be stored.
		 *        If empty, a default directory may be used.
		 */
		explicit FileLogger(const QString& logDirectory = "");

		/**
		 * @brief Logs an informational message.
		 * @param message The message to log.
		 */
		void Info(const QString& message);

		/**
		 * @brief Logs a warning message.
		 * @param message The message to log.
		 */
		void Warning(const QString& message);

		/**
		 * @brief Logs an error message.
		 * @param message The message to log.
		 */
		void Error(const QString& message);

	private:
	private:
		void Write(const QString& level, const QString& message);

		void SendErrorEmail(const QString& message);
		bool IsErrorEmailEnabled() const;
		QString GetConfigValue(const QString& key, const QString& defaultValue = "") const;
		QString CaptureAppScreenshot();

	private:
		QString _logDirectory; ///< Directory for log files.
		QMutex _mutex;         ///< Mutex for thread-safe logging.
	};
}
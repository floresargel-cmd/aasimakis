#pragma once

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include <functional>

namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger;
}

namespace StackingPlan::Infrastructure::Database
{
	/**
	 * @class SqlRetryPolicy
	 * @brief Implements a retry policy for SQL operations with exponential backoff.
	 *
	 * The SqlRetryPolicy class provides a mechanism to automatically retry failed SQL operations
	 * that are considered transient or retryable. It supports configurable maximum retries and
	 * base delay for exponential backoff between attempts. Logging is supported via the FileLogger.
	 */
	class SqlRetryPolicy
	{
	public:
		/**
		 * @brief Constructs a SqlRetryPolicy instance.
		 * @param maxRetries The maximum number of retry attempts for a failed operation.
		 * @param baseDelayMilliseconds The base delay in milliseconds for exponential backoff between retries.
		 * @param logger Pointer to a FileLogger instance for logging retry attempts and errors.
		 */
		SqlRetryPolicy(
			int maxRetries,
			int baseDelayMilliseconds,
			StackingPlan::Infrastructure::Logging::FileLogger* logger);

		/**
		 * @brief Executes the provided action with retry logic.
		 *
		 * The action is executed up to maxRetries times if it fails with a retryable error.
		 * The method applies exponential backoff between retries and logs attempts and errors.
		 *
		 * @param action The function to execute. Should return true on success, false on failure.
		 * @param errorMessage Reference to a QString to receive the error message if all retries fail.
		 * @return true if the action succeeds within the allowed retries; false otherwise.
		 */
		bool Execute(const std::function<bool()>& action, QString& errorMessage);

	private:
		/**
		 * @brief Determines if the given error message represents a retryable error.
		 * @param errorMessage The error message to evaluate.
		 * @return true if the error is considered retryable; false otherwise.
		 */
		bool IsRetryableError(const QString& errorMessage) const;

		/**
		 * @brief Sleeps for a duration based on the current attempt, implementing exponential backoff.
		 * @param attempt The current retry attempt number.
		 */
		void SleepWithBackoff(int attempt) const;

	private:
		int _maxRetries; ///< Maximum number of retry attempts.
		int _baseDelayMilliseconds; ///< Base delay in milliseconds for exponential backoff.
		StackingPlan::Infrastructure::Logging::FileLogger* _logger; ///< Logger for retry events.
	};
}

/**
 * @file SqlRetryPolicy.cpp
 * @brief Implements a retry policy for SQL operations with exponential backoff and logging.
 *
 * This file defines the `SqlRetryPolicy` class, which provides mechanisms to automatically retry
 * SQL operations that fail due to transient errors such as deadlocks or timeouts. The policy
 * supports configurable maximum retries, base delay for backoff, and integrates with a logger
 * for diagnostic output.
 */

#include "SqlRetryPolicy.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/thread/qthread.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../Logging/FileLogger.h"
#include <functional>


namespace StackingPlan::Infrastructure::Database
{
	/**
	 * @brief Constructs a SqlRetryPolicy instance.
	 * @param maxRetries The maximum number of retry attempts for a failed SQL operation.
	 * @param baseDelayMilliseconds The base delay in milliseconds used for exponential backoff between retries.
	 * @param logger Pointer to a FileLogger instance for logging retry attempts and outcomes.
	 */
	SqlRetryPolicy::SqlRetryPolicy(
		int maxRetries,
		int baseDelayMilliseconds,
		StackingPlan::Infrastructure::Logging::FileLogger* logger)
		: _maxRetries(maxRetries),
		_baseDelayMilliseconds(baseDelayMilliseconds),
		_logger(logger)
	{
	}

	/**
	 * @brief Executes the provided SQL action with retry logic for transient errors.
	 *
	 * The method attempts to execute the given action up to the configured maximum number of retries.
	 * If the action fails due to a retryable error, it waits for an exponentially increasing delay
	 * before retrying. Successes and retryable failures are logged if a logger is provided.
	 *
	 * @param action The SQL operation to execute, provided as a std::function returning bool (true on success).
	 * @param errorMessage Reference to a QString that receives the error message if the action fails.
	 * @return true if the action succeeds within the allowed attempts; false otherwise.
	 */
	bool SqlRetryPolicy::Execute(const std::function<bool()>& action, QString& errorMessage)
	{
		for (int attempt = 0; attempt <= _maxRetries; ++attempt)
		{
			if (action())
			{
				if (attempt > 0 && _logger)
				{
					_logger->Info(QString("SQL execution succeeded after retry. Attempt=%1")
						.arg(attempt + 1));
				}

				return true;
			}

			if (attempt == _maxRetries)
			{
				break;
			}

			if (!IsRetryableError(errorMessage))
			{
				break;
			}

			if (_logger)
			{
				_logger->Warning(QString("Retryable SQL error. Attempt=%1 Error=%2")
					.arg(attempt + 1)
					.arg(errorMessage));
			}

			SleepWithBackoff(attempt);
		}

		return false;
	}

	/**
	 * @brief Determines if an error message corresponds to a retryable SQL error.
	 *
	 * Checks for common transient error keywords such as "deadlock", "timeout", or "temporarily unavailable".
	 *
	 * @param errorMessage The error message to evaluate.
	 * @return true if the error is considered retryable; false otherwise.
	 */
	bool SqlRetryPolicy::IsRetryableError(const QString& errorMessage) const
	{
		const QString error = errorMessage.toLower();

		return error.contains("deadlock")
			|| error.contains("timeout")
			|| error.contains("timed out")
			|| error.contains("transport-level error")
			|| error.contains("temporarily unavailable");
	}

	/**
	 * @brief Sleeps for a duration based on the current attempt, implementing exponential backoff.
	 *
	 * The delay increases linearly with each retry attempt, calculated as baseDelayMilliseconds * (attempt + 1).
	 *
	 * @param attempt The current retry attempt (zero-based).
	 */
	void SqlRetryPolicy::SleepWithBackoff(int attempt) const
	{
		const int delay = _baseDelayMilliseconds * (attempt + 1);
		QThread::msleep(delay);
	}
}
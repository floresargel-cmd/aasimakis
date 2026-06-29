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
	class DatabaseInitializer
	{
	public:
		DatabaseInitializer(
			std::shared_ptr<SqlCommandExecutor> sqlCommandExecutor,
			std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger);

		bool Initialize();

	private:
		bool EnsureErrorLogTable();

	private:
		std::shared_ptr<SqlCommandExecutor> _sqlCommandExecutor;
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger;
	};
}
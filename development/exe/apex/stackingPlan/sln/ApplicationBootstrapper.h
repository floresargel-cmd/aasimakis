#pragma once

#include <memory>

namespace StackingPlan::Application::Services
{
	class AppSettings;
	class UserContextService;
}

namespace StackingPlan::Infrastructure::Database
{
	class SqlCommandExecutor;
	class DatabaseInitializer;
}

namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger;
}

namespace StackingPlan::App
{
	class ApplicationBootstrapper
	{
	public:
		ApplicationBootstrapper();

		bool Initialize();

		std::shared_ptr<StackingPlan::Application::Services::AppSettings> GetAppSettings() const;
		std::shared_ptr<StackingPlan::Application::Services::UserContextService> GetUserContextService() const;
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> GetLogger() const;
		std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> GetSqlCommandExecutor() const;

	private:
		void BuildSettings();
		void BuildServices();

	private:
		std::shared_ptr<StackingPlan::Application::Services::AppSettings> _appSettings;
		std::shared_ptr<StackingPlan::Application::Services::UserContextService> _userContextService;
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger;
		std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> _sqlCommandExecutor;
		std::shared_ptr<StackingPlan::Infrastructure::Database::DatabaseInitializer> _databaseInitializer;
	};
}

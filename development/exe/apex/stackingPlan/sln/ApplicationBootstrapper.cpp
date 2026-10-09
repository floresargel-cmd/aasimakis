#include "ApplicationBootstrapper.h"

#include "../Application/Services/AppSettings.h"
#include "../Application/Services/UserContextService.h"
#include "../Infrastructure/Database/DatabaseInitializer.h"
#include "../Infrastructure/Database/SqlCommandExecutor.h"
#include "../Infrastructure/Logging/FileLogger.h"

namespace StackingPlan::App
{
	ApplicationBootstrapper::ApplicationBootstrapper()
	{
	}

	bool ApplicationBootstrapper::Initialize()
	{
		BuildSettings();
		BuildServices();

		_logger->Info("Application bootstrapper initialized.");

		return _databaseInitializer->Initialize();
	}

	void ApplicationBootstrapper::BuildSettings()
	{
		_appSettings = std::make_shared<StackingPlan::Application::Services::AppSettings>();

		// Replace with your real ODBC / DSN / connection string later.
		_appSettings->connectionString = "DSN=YourDsnName;";
		_appSettings->logDirectory = "logs";
	}

	void ApplicationBootstrapper::BuildServices()
	{
		_logger = std::make_shared<StackingPlan::Infrastructure::Logging::FileLogger>(
			_appSettings->logDirectory);

		_userContextService =
			std::make_shared<StackingPlan::Application::Services::UserContextService>();

		_sqlCommandExecutor =
			std::make_shared<StackingPlan::Infrastructure::Database::SqlCommandExecutor>(
				_appSettings->connectionString,
				"StackingPlanConnection");

		_databaseInitializer =
			std::make_shared<StackingPlan::Infrastructure::Database::DatabaseInitializer>(
				_sqlCommandExecutor,
				_logger);
	}

	std::shared_ptr<StackingPlan::Application::Services::AppSettings>
		ApplicationBootstrapper::GetAppSettings() const
	{
		return _appSettings;
	}

	std::shared_ptr<StackingPlan::Application::Services::UserContextService>
		ApplicationBootstrapper::GetUserContextService() const
	{
		return _userContextService;
	}

	std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger>
		ApplicationBootstrapper::GetLogger() const
	{
		return _logger;
	}

	std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor>
		ApplicationBootstrapper::GetSqlCommandExecutor() const
	{
		return _sqlCommandExecutor;
	}
}

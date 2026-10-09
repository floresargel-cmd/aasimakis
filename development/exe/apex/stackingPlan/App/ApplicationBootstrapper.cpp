#include "ApplicationBootstrapper.h"

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qdir.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qsettings.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qcoreapplication.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"

#include "../Application/Services/AppSettings.h"
#include "../Application/Services/BasketApplicationService.h"
#include "../Application/Services/MainWindowOrchestrator.h"
#include "../Application/Services/ProductionApplicationService.h"
#include "../Application/Services/ProductionMonitorService.h"
#include "../Application/Services/UserContextService.h"
#include "../legacy/LegacyGlobalState.h"
#include "../legacy/LegacySqlBridge.h"

#include "../Infrastructure/Database/DatabaseInitializer.h"
#include "../Infrastructure/Database/SqlCommandExecutor.h"
#include "../Infrastructure/Logging/FileLogger.h"
#include "../Infrastructure/Repositories/BasketRepository.h"
#include "../Infrastructure/Repositories/ProductionMonitorRepository.h"
#include "../Infrastructure/Repositories/ProductionRepository.h"

#include "../Application/Services/LegacyRuntimeContext.h"
#include <memory>

namespace StackingPlan::App
{
	ApplicationBootstrapper::ApplicationBootstrapper()
	{
	}

	bool ApplicationBootstrapper::Initialize()
	{

		BuildSettings();
		ApplyLegacyRuntimeContext();
		BuildServices();

		_logger->Info(QString("Application bootstrapper initialized."));

		const bool databaseInitialized = _databaseInitializer->Initialize();

		_logger->Info(databaseInitialized
			? QString("Database initializer returned true.")
			: QString("Database initializer returned false."));

		return databaseInitialized;
		_logger->Info(QString("Application bootstrapper initialization completed."));
	}

	void ApplicationBootstrapper::ApplyLegacyRuntimeContext()
	{

		const QString configPath =
			QDir(QCoreApplication::applicationDirPath()).filePath("app.ini");

		QSettings settings(configPath, QSettings::IniFormat);

		const bool useLocalDb =
			settings.value("Environment/UseLocalDb", false).toBool();

		const QString mainDbProfile = useLocalDb
			? "Database.Main.Local"
			: "Database.Main.Remote";

		const QString basketDbProfile = useLocalDb
			? "Database.BasketCrane.Local"
			: "Database.BasketCrane.Remote";

		const QString mainDriver =
			settings.value(mainDbProfile + "/Driver", "SQL Server").toString();
		const QString mainServer =
			settings.value(mainDbProfile + "/Server", "").toString();
		const QString mainDatabase =
			settings.value(mainDbProfile + "/Database", "master").toString();
		const QString mainUser =
			settings.value(mainDbProfile + "/User", "").toString();
		const QString mainPassword =
			settings.value(mainDbProfile + "/Password", "").toString();

		const QString basketDriver =
			settings.value(basketDbProfile + "/Driver", "SQL Server").toString();
		const QString basketServer =
			settings.value(basketDbProfile + "/Server", "").toString();
		const QString basketDatabase =
			settings.value(basketDbProfile + "/Database", "").toString();
		const QString basketUser =
			settings.value(basketDbProfile + "/User", "").toString();
		const QString basketPassword =
			settings.value(basketDbProfile + "/Password", "").toString();

		StackingPlan::Application::Services::LegacyRuntimeContext runtimeContext;

		runtimeContext.useLocalDb = useLocalDb;

		runtimeContext.loginName =
			settings.value("Runtime/LoginName", "admin").toString();

		runtimeContext.privilegeType =
			settings.value("Runtime/PrivilegeType", 0).toInt();

		runtimeContext.maxNoOfProfilesToLoad =
			settings.value("Runtime/MaxNoOfProfilesToLoad", 1).toInt();

		runtimeContext.arcAngleForLine =
			settings.value("Runtime/ArcAngleForLine", 20.0).toDouble();

		runtimeContext.layerMinSupportLength =
			settings.value("Runtime/LayerMinSupportLength", 75).toInt();

		runtimeContext.globalPressNumber =
			settings.value("Runtime/GlobalPressNumber", 1).toInt();

		runtimeContext.globalColorIndex =
			settings.value("Runtime/GlobalColorIndex", -1).toInt();

		runtimeContext.havePlcComms =
			settings.value("Runtime/HavePlcComms", false).toBool();

		runtimeContext.screenWidth =
			settings.value("Startup/ScreenWidth", 1920).toInt();

		runtimeContext.screenHeight =
			settings.value("Startup/ScreenHeight", 1080).toInt();

		runtimeContext.startPress1 =
			settings.value("Startup/Press1", true).toBool();

		runtimeContext.startPress2 =
			settings.value("Startup/Press2", false).toBool();

		runtimeContext.hideToolbarWhenSinglePress =
			settings.value("Startup/HideToolbarWhenSinglePress", true).toBool();

		runtimeContext.mainDbConnectionName =
			settings.value(mainDbProfile + "/ConnectionName", "MainDbConnectionRemote").toString();
		runtimeContext.mainDbDriver = "QODBC";
		runtimeContext.mainDbServer = mainServer;
		runtimeContext.mainDbDatabase = mainDatabase;
		runtimeContext.mainDbUser = mainUser;
		runtimeContext.mainDbPassword = mainPassword;
		runtimeContext.mainDbConnectionString =
			QString("Driver={%1};Server=%2;Database=%3;UID=%4;PWD=%5;")
			.arg(mainDriver)
			.arg(mainServer)
			.arg(mainDatabase)
			.arg(mainUser)
			.arg(mainPassword);

		runtimeContext.basketCraneConnectionName =
			settings.value(basketDbProfile + "/ConnectionName", "basketCraneConnectionRemote").toString();
		runtimeContext.basketCraneDriver = "QODBC";
		runtimeContext.basketCraneServer = basketServer;
		runtimeContext.basketCraneDatabase = basketDatabase;
		runtimeContext.basketCraneUser = basketUser;
		runtimeContext.basketCranePassword = basketPassword;
		runtimeContext.basketCraneConnectionString =
			QString("Driver={%1};Server=%2;Database=%3;UID=%4;PWD=%5;")
			.arg(basketDriver)
			.arg(basketServer)
			.arg(basketDatabase)
			.arg(basketUser)
			.arg(basketPassword);

		LegacyGlobals::ApplyRuntimeContext(runtimeContext);

	}

	void ApplicationBootstrapper::BuildSettings()
	{

		_userContextService =
			std::make_shared<StackingPlan::Application::Services::UserContextService>();

		_appSettings = std::make_shared<StackingPlan::Application::Services::AppSettings>();

		const QString configPath =
			QDir(QCoreApplication::applicationDirPath()).filePath("app.ini");

		QSettings settings(configPath, QSettings::IniFormat);

		const bool useLocalDb =
			settings.value("Environment/UseLocalDb", false).toBool();

		const QString mainDbProfile = useLocalDb
			? "Database.Main.Local"
			: "Database.Main.Remote";

		const QString basketDbProfile = useLocalDb
			? "Database.BasketCrane.Local"
			: "Database.BasketCrane.Remote";

		const QString mainDriver =
			settings.value(mainDbProfile + "/Driver", "SQL Server").toString();
		const QString mainServer =
			settings.value(mainDbProfile + "/Server", "").toString();
		const QString mainDatabase =
			settings.value(mainDbProfile + "/Database", "master").toString();
		const QString mainUser =
			settings.value(mainDbProfile + "/User", "").toString();
		const QString mainPassword =
			settings.value(mainDbProfile + "/Password", "").toString();

		const QString basketDriver =
			settings.value(basketDbProfile + "/Driver", "SQL Server").toString();
		const QString basketServer =
			settings.value(basketDbProfile + "/Server", "").toString();
		const QString basketDatabase =
			settings.value(basketDbProfile + "/Database", "").toString();
		const QString basketUser =
			settings.value(basketDbProfile + "/User", "").toString();
		const QString basketPassword =
			settings.value(basketDbProfile + "/Password", "").toString();

		_appSettings->connectionString =
			QString("Driver={%1};Server=%2;Database=%3;UID=%4;PWD=%5;")
			.arg(mainDriver)
			.arg(mainServer)
			.arg(mainDatabase)
			.arg(mainUser)
			.arg(mainPassword);

		_appSettings->connectionName =
			settings.value(mainDbProfile + "/ConnectionName", "MainDbConnectionRemote").toString();

		_appSettings->basketCraneConnectionString =
			QString("Driver={%1};Server=%2;Database=%3;UID=%4;PWD=%5;")
			.arg(basketDriver)
			.arg(basketServer)
			.arg(basketDatabase)
			.arg(basketUser)
			.arg(basketPassword);

		_appSettings->basketCraneConnectionName =
			settings.value(basketDbProfile + "/ConnectionName", "basketCraneConnectionRemote").toString();

		_appSettings->maxRetries =
			settings.value("Sql/MaxRetries", 3).toInt();

		_appSettings->baseDelayMilliseconds =
			settings.value("Sql/BaseDelayMilliseconds", 200).toInt();

		_appSettings->logDirectory =
			settings.value("Logging/LogDirectory", "").toString();

		QString loginName =
			settings.value("Runtime/LoginName", "admin").toString();

		int privilegeType =
			settings.value("Runtime/PrivilegeType", 0).toInt();

		int pressNumber =
			settings.value("Runtime/GlobalPressNumber", 1).toInt();

		bool havePlcComms =
			settings.value("Runtime/HavePlcComms", false).toBool();

		_userContextService->SetUser(loginName, privilegeType);
		_userContextService->SetGlobalPressNumber(pressNumber);
		_userContextService->SetHavePlcComms(havePlcComms);

	}

	void ApplicationBootstrapper::BuildServices()
	{
		_logger = std::make_shared<StackingPlan::Infrastructure::Logging::FileLogger>(
			_appSettings->logDirectory);

		_sqlCommandExecutor =
			std::make_shared<StackingPlan::Infrastructure::Database::SqlCommandExecutor>(
				_appSettings->connectionString,
				_appSettings->connectionName,
				_logger.get(),
				_appSettings->maxRetries,
				_appSettings->baseDelayMilliseconds);

		std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> basketCraneExecutor =
			std::make_shared<StackingPlan::Infrastructure::Database::SqlCommandExecutor>(
				_appSettings->basketCraneConnectionString,
				_appSettings->basketCraneConnectionName,
				_logger.get(),
				_appSettings->maxRetries,
				_appSettings->baseDelayMilliseconds);

		LegacyBridge::SetSqlCommandExecutor(_appSettings->connectionName, _sqlCommandExecutor);
		LegacyBridge::SetSqlCommandExecutor("MAIN_DB_CONNECTION", _sqlCommandExecutor);
		LegacyBridge::SetSqlCommandExecutor("MainDbConnectionRemote", _sqlCommandExecutor);
		LegacyBridge::SetSqlCommandExecutor("MainDbConnectionLocal", _sqlCommandExecutor);

		LegacyBridge::SetSqlCommandExecutor(_appSettings->basketCraneConnectionName, basketCraneExecutor);
		LegacyBridge::SetSqlCommandExecutor("BASKET_CRANE_DB_CONNECTION", basketCraneExecutor);
		LegacyBridge::SetSqlCommandExecutor("basketCraneConnectionRemote", basketCraneExecutor);
		LegacyBridge::SetSqlCommandExecutor("basketCraneConnectionLocal", basketCraneExecutor);

		_databaseInitializer =
			std::make_shared<StackingPlan::Infrastructure::Database::DatabaseInitializer>(
				_sqlCommandExecutor,
				_logger);

		_productionRepository =
			std::make_shared<StackingPlan::Infrastructure::Repositories::ProductionRepository>(
				_sqlCommandExecutor,
				_logger);

		_basketRepository =
			std::make_shared<StackingPlan::Infrastructure::Repositories::BasketRepository>(
				_sqlCommandExecutor,
				_logger);

		_productionMonitorRepository =
			std::make_shared<StackingPlan::Infrastructure::Repositories::ProductionMonitorRepository>(
				_sqlCommandExecutor,
				_logger);

		_productionApplicationService =
			std::make_shared<StackingPlan::Application::Services::ProductionApplicationService>(
				_productionRepository,
				_logger);

		_basketApplicationService =
			std::make_shared<StackingPlan::Application::Services::BasketApplicationService>(
				_basketRepository,
				_logger);

		_productionMonitorService =
			std::make_shared<StackingPlan::Application::Services::ProductionMonitorService>(
				_productionMonitorRepository,
				_logger);

		_mainWindowOrchestrator =
			std::make_shared<StackingPlan::Application::Services::MainWindowOrchestrator>(
				_productionApplicationService,
				_basketApplicationService,
				_productionMonitorService,
				_userContextService,
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

	std::shared_ptr<StackingPlan::Application::Services::ProductionApplicationService>
		ApplicationBootstrapper::GetProductionApplicationService() const
	{
		return _productionApplicationService;
	}

	std::shared_ptr<StackingPlan::Application::Services::BasketApplicationService>
		ApplicationBootstrapper::GetBasketApplicationService() const
	{
		return _basketApplicationService;
	}

	std::shared_ptr<StackingPlan::Application::Services::ProductionMonitorService>
		ApplicationBootstrapper::GetProductionMonitorService() const
	{
		return _productionMonitorService;
	}

	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator>
		ApplicationBootstrapper::GetMainWindowOrchestrator() const
	{
		return _mainWindowOrchestrator;
	}
}

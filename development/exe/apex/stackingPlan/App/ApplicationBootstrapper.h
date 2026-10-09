#pragma once

#include <memory>

namespace StackingPlan::Application::Services
{
	class AppSettings;
	class UserContextService;
	class ProductionApplicationService;
	class BasketApplicationService;
	class ProductionMonitorService;
	class MainWindowOrchestrator;
}

namespace StackingPlan::Application::Interfaces
{
	class IProductionRepository;
	class IBasketRepository;
	class IProductionMonitorRepository;
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
	/**
	 * @namespace StackingPlan::App
	 * Contains application-level classes for the StackingPlan application.
	 */

	 /**
	  * @class ApplicationBootstrapper
	  * @brief Responsible for initializing and wiring up core application services and infrastructure components.
	  *
	  * The ApplicationBootstrapper class acts as the composition root for the application, managing the
	  * creation and lifetime of key services, repositories, and infrastructure objects. It provides
	  * accessors for retrieving shared instances of these components, ensuring consistent configuration
	  * and dependency management throughout the application's lifecycle.
	  *
	  * Typical usage involves constructing an ApplicationBootstrapper instance, calling Initialize(),
	  * and then retrieving the required services via the provided getter methods.
	  */
	class ApplicationBootstrapper
	{
	public:
		/**
		 * @brief Constructs the ApplicationBootstrapper.
		 *
		 * Initializes internal pointers to null. Actual service construction is performed in Initialize().
		 */
		ApplicationBootstrapper();

		/**
		 * @brief Initializes all application services, repositories, and infrastructure components.
		 * @return true if initialization succeeds; false otherwise.
		 *
		 * This method sets up configuration, logging, database access, and domain services.
		 * It must be called before accessing any service via the getter methods.
		 */
		bool Initialize();

		/**
		 * @brief Gets the application settings instance.
		 * @return Shared pointer to AppSettings.
		 */
		std::shared_ptr<StackingPlan::Application::Services::AppSettings> GetAppSettings() const;

		/**
		 * @brief Gets the user context service.
		 * @return Shared pointer to UserContextService.
		 */
		std::shared_ptr<StackingPlan::Application::Services::UserContextService> GetUserContextService() const;

		/**
		 * @brief Gets the file logger instance.
		 * @return Shared pointer to FileLogger.
		 */
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> GetLogger() const;

		/**
		 * @brief Gets the SQL command executor for database operations.
		 * @return Shared pointer to SqlCommandExecutor.
		 */
		std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> GetSqlCommandExecutor() const;

		/**
		 * @brief Gets the production application service.
		 * @return Shared pointer to ProductionApplicationService.
		 */
		std::shared_ptr<StackingPlan::Application::Services::ProductionApplicationService> GetProductionApplicationService() const;

		/**
		 * @brief Gets the basket application service.
		 * @return Shared pointer to BasketApplicationService.
		 */
		std::shared_ptr<StackingPlan::Application::Services::BasketApplicationService> GetBasketApplicationService() const;

		/**
		 * @brief Gets the production monitor service.
		 * @return Shared pointer to ProductionMonitorService.
		 */
		std::shared_ptr<StackingPlan::Application::Services::ProductionMonitorService> GetProductionMonitorService() const;

		/**
		 * @brief Gets the main window orchestrator.
		 * @return Shared pointer to MainWindowOrchestrator.
		 */
		std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> GetMainWindowOrchestrator() const;

	private:
		/**
		 * @brief Builds and configures application settings.
		 */
		void BuildSettings();

		/**
		 * @brief Constructs and wires up all application services and repositories.
		 */
		void BuildServices();

		/**
		 * @brief Applies legacy runtime context for backward compatibility.
		 */
		void ApplyLegacyRuntimeContext();

	private:
		// Core configuration and infrastructure
		std::shared_ptr<StackingPlan::Application::Services::AppSettings> _appSettings;
		std::shared_ptr<StackingPlan::Application::Services::UserContextService> _userContextService;
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger;
		std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> _sqlCommandExecutor;
		std::shared_ptr<StackingPlan::Infrastructure::Database::DatabaseInitializer> _databaseInitializer;

		// Domain repositories
		std::shared_ptr<StackingPlan::Application::Interfaces::IProductionRepository> _productionRepository;
		std::shared_ptr<StackingPlan::Application::Interfaces::IBasketRepository> _basketRepository;
		std::shared_ptr<StackingPlan::Application::Interfaces::IProductionMonitorRepository> _productionMonitorRepository;

		// Domain services
		std::shared_ptr<StackingPlan::Application::Services::ProductionApplicationService> _productionApplicationService;
		std::shared_ptr<StackingPlan::Application::Services::BasketApplicationService> _basketApplicationService;
		std::shared_ptr<StackingPlan::Application::Services::ProductionMonitorService> _productionMonitorService;
		std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> _mainWindowOrchestrator;
	};
}

#include "LegacyLoggingBridge.h"

#include "../Application/Services/MainWindowOrchestrator.h"
#include "../Infrastructure/Logging/FileLogger.h"
#include "mW.h"

namespace
{
	std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> GetFallbackLogger()
	{
		static std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger =
			std::make_shared<StackingPlan::Infrastructure::Logging::FileLogger>();
		return logger;
	}

	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> GetOrchestrator()
	{
		mainWindowClass* mainWindow = mainWindowClass::GetInstance();
		if (mainWindow == NULL)
		{
			return std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator>();
		}

		return mainWindow->getOrchestrator();
	}
}

namespace LegacyBridge
{
	void LogInfo(const QString& message)
	{
		std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator = GetOrchestrator();

		if (orchestrator && orchestrator->GetLogger())
		{
			orchestrator->GetLogger()->Info(message);
			return;
		}

		GetFallbackLogger()->Info(message);
	}

	void LogWarning(const QString& message)
	{
		std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator = GetOrchestrator();

		if (orchestrator && orchestrator->GetLogger())
		{
			orchestrator->GetLogger()->Warning(message);
			return;
		}

		GetFallbackLogger()->Warning(message);
	}

	void LogError(const QString& message)
	{
		std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator = GetOrchestrator();

		if (orchestrator && orchestrator->GetLogger())
		{
			orchestrator->GetLogger()->Error(message);
			return;
		}

		GetFallbackLogger()->Error(message);
	}

	void NotifyOthers()
	{
		std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator = GetOrchestrator();
		if (!orchestrator)
		{
			return;
		}

		orchestrator->MarkUsersNeedsUpdate();
	}
}

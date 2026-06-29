#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"

#include <memory>

namespace StackingPlan::Application::Interfaces
{
	class IProductionMonitorRepository;
}

namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger;
}

namespace StackingPlan::Application::Services
{
	class ProductionMonitorService
	{
	public:
		ProductionMonitorService(
			std::shared_ptr<StackingPlan::Application::Interfaces::IProductionMonitorRepository> repository,
			std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger);

		bool RunPeriodicChecks();
		bool DoesUserNeedRefresh(const QString& loginName);
		bool ResetUserNeedsUpdate(const QString& loginName);
		bool CreateVirtualProduction(int dieNumber);
		bool MarkProductionAsNext(int tid);

	private:
		std::shared_ptr<StackingPlan::Application::Interfaces::IProductionMonitorRepository> _repository;
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger;
	};
}
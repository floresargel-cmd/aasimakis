#include "ProductionMonitorService.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../Infrastructure/Logging/FileLogger.h"
#include "../Interfaces/IProductionMonitorRepository.h"
#include <memory>

namespace StackingPlan::Application::Services
{
	ProductionMonitorService::ProductionMonitorService(
		std::shared_ptr<StackingPlan::Application::Interfaces::IProductionMonitorRepository> repository,
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger)
		: _repository(repository),
		_logger(logger)
	{
	}

	bool ProductionMonitorService::RunPeriodicChecks()
	{
		_logger->Info(QString("Running periodic checks for production monitor service."));
		const bool step1 = _repository->NormalizeStatusMinus2Production();
		const bool step2 = _repository->ActivateInitialProduction();
		return step1 && step2;
	}

	bool ProductionMonitorService::DoesUserNeedRefresh(const QString& loginName)
	{
		_logger->Info(QString("Checking if user '%1' needs refresh.").arg(loginName));
		return _repository->DoesUserNeedRefresh(loginName);
	}

	bool ProductionMonitorService::ResetUserNeedsUpdate(const QString& loginName)
	{
		_logger->Info(QString("Resetting user needs update for '%1'.").arg(loginName));
		return _repository->ResetUserNeedsUpdate(loginName);
	}

	bool ProductionMonitorService::CreateVirtualProduction(int dieNumber)
	{
		_logger->Info(QString("Creating virtual production for die number %1.").arg(dieNumber));
		return _repository->InsertVirtualProduction(dieNumber);
	}

	bool ProductionMonitorService::MarkProductionAsNext(int tid)
	{
		_logger->Info(QString("Marking production with TID %1 as next.").arg(tid));
		return _repository->MarkProductionAsNext(tid);
	}
}

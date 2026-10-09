#include "../../legacy/LegacyGlobalState.h"
#include "MainWindowOrchestrator.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../Domain/Entities/ProductionProfile.h"
#include "../../Infrastructure/Logging/FileLogger.h"
#include "BasketApplicationService.h"
#include "ProductionApplicationService.h"
#include "ProductionMonitorService.h"
#include "UserContextService.h"
#include <memory>

namespace StackingPlan::Application::Services
{
	MainWindowOrchestrator::MainWindowOrchestrator(
		std::shared_ptr<ProductionApplicationService> productionApplicationService,
		std::shared_ptr<BasketApplicationService> basketApplicationService,
		std::shared_ptr<ProductionMonitorService> productionMonitorService,
		std::shared_ptr<UserContextService> userContextService,
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger)
		: _productionApplicationService(productionApplicationService),
		_basketApplicationService(basketApplicationService),
		_productionMonitorService(productionMonitorService),
		_userContextService(userContextService),
		_logger(logger)
	{
	}

	std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger>
		MainWindowOrchestrator::GetLogger() const
	{
		return _logger;
	}

	QVector<StackingPlan::Domain::Entities::ProductionProfile>
		MainWindowOrchestrator::LoadProductionForPress(int pressNumber)
	{
		if (_logger)
		{
			_logger->Info(QString("MainWindowOrchestrator loading production for press %1.")
				.arg(pressNumber));
		}

		return _productionApplicationService->LoadByPress(pressNumber);
	}

	StackingPlan::Domain::Entities::ProductionProfile
		MainWindowOrchestrator::GetProductionById(int id)
	{
		_logger->Info(QString("MainWindowOrchestrator getting production by id %1.").arg(id));
		return _productionApplicationService->GetById(id);
	}

	bool MainWindowOrchestrator::ChangeProductionStatus(int id, int status)
	{
		_logger->Info(QString("MainWindowOrchestrator changing production status for id %1 to status %2.")
			.arg(id).arg(status));
		return _productionApplicationService->ChangeStatus(id, status);
	}

	bool MainWindowOrchestrator::ChangeProductionQuantities(int id, int totalQuantity, int quantityInBaskets)
	{
		_logger->Info(QString("MainWindowOrchestrator changing production quantities for id %1 to total quantity %2 and quantity in baskets %3.")
			.arg(id).arg(totalQuantity).arg(quantityInBaskets));
		return _productionApplicationService->ChangeQuantities(id, totalQuantity, quantityInBaskets);
	}

	bool MainWindowOrchestrator::ChangeProductionTemper(int id, const QString& temper)
	{
		_logger->Info(QString("MainWindowOrchestrator changing production temper for id %1 to temper %2.")
			.arg(id).arg(temper));
		return _productionApplicationService->ChangeTemper(id, temper);
	}

	bool MainWindowOrchestrator::ChangeProductionLength(int id, int length)
	{
		_logger->Info(QString("MainWindowOrchestrator changing production length for id %1 to length %2.")
			.arg(id).arg(length));
		return _productionApplicationService->ChangeLength(id, length);
	}

	bool MainWindowOrchestrator::ChangeProductionLinearDensity(int id, double linearDensity)
	{
		_logger->Info(QString("MainWindowOrchestrator changing production linear density for id %1 to linear density %2.")
			.arg(id).arg(linearDensity));
		return _productionApplicationService->ChangeLinearDensity(id, linearDensity);
	}

	bool MainWindowOrchestrator::ChangeProductionDoubleLength(int id, int doubleLength)
	{
		_logger->Info(QString("MainWindowOrchestrator changing production double length for id %1 to double length %2.")
			.arg(id).arg(doubleLength));
		return _productionApplicationService->ChangeDoubleLength(id, doubleLength);
	}

	bool MainWindowOrchestrator::ChangeProductionOnNextBasket(int id, int onNextBasket)
	{
		_logger->Info(QString("MainWindowOrchestrator changing production on next basket for id %1 to on next basket %2.")
			.arg(id).arg(onNextBasket));
		return _productionApplicationService->ChangeOnNextBasket(id, onNextBasket);
	}

	bool MainWindowOrchestrator::ChangeProductionOnNextLayer(int id, int onNextLayer)
	{
		_logger->Info(QString("MainWindowOrchestrator changing production on next layer for id %1 to on next layer %2.")
			.arg(id).arg(onNextLayer));
		return _productionApplicationService->ChangeOnNextLayer(id, onNextLayer);
	}

	QString MainWindowOrchestrator::GetSoNumberByTid(int tid)
	{
		return _basketApplicationService->GetSoNumberByTid(tid);
	}

	QString MainWindowOrchestrator::GetDieByTid(int tid)
	{
		return _basketApplicationService->GetDieByTid(tid);
	}

	bool MainWindowOrchestrator::ReorderProductionSequence(int beforeTid, int droppedTid)
	{
		return _basketApplicationService->ReorderProductionSequence(beforeTid, droppedTid);
	}

	int MainWindowOrchestrator::GetBasketTableIdByPositionId(int positionId)
	{
		return _basketApplicationService->GetBasketTableIdByPositionId(positionId);
	}

	bool MainWindowOrchestrator::MarkUsersNeedsUpdate()
	{
		return _basketApplicationService->MarkUsersNeedsUpdate();
	}

	int MainWindowOrchestrator::GetPendingBasketQuantityByTidAndBasket(int tid, int basketId)
	{
		return _basketApplicationService->GetPendingBasketQuantityByTidAndBasket(tid, basketId);
	}

	bool MainWindowOrchestrator::DeleteBasketSpacers(int basketId)
	{
		return _basketApplicationService->DeleteBasketSpacers(basketId);
	}

	bool MainWindowOrchestrator::InsertBasketSpacer(
		int basketId,
		int layer,
		int inComb,
		int sp1,
		int sp2,
		int sp3,
		int sp4,
		int sp5,
		int sp6,
		int sp7,
		int sp8)
	{
		return _basketApplicationService->InsertBasketSpacer(
			basketId, layer, inComb, sp1, sp2, sp3, sp4, sp5, sp6, sp7, sp8);
	}

	bool MainWindowOrchestrator::IncrementQuantityInBaskets(int tid, int quantityToAdd)
	{
		return _basketApplicationService->IncrementQuantityInBaskets(tid, quantityToAdd);
	}

	bool MainWindowOrchestrator::RunPeriodicChecks()
	{
		return _productionMonitorService->RunPeriodicChecks();
	}

	bool MainWindowOrchestrator::DoesUserNeedRefresh()
	{
		return _productionMonitorService->DoesUserNeedRefresh(
			_userContextService->GetUserName());
	}

	bool MainWindowOrchestrator::ResetUserNeedsUpdate()
	{
		return _productionMonitorService->ResetUserNeedsUpdate(
			_userContextService->GetUserName());
	}

	bool MainWindowOrchestrator::CreateVirtualProduction(int dieNumber)
	{
		return _productionMonitorService->CreateVirtualProduction(dieNumber);
	}

	bool MainWindowOrchestrator::MarkProductionAsNext(int tid)
	{
		return _productionMonitorService->MarkProductionAsNext(tid);
	}

	QString MainWindowOrchestrator::GetLoginName() const
	{
		return _userContextService->GetUserName();
	}

	int MainWindowOrchestrator::GetPrivilegeType() const
	{
		return _userContextService->GetPrivilegeType();
	}

	int MainWindowOrchestrator::GetGlobalPressNumber() const
	{
		return _userContextService->GetGlobalPressNumber();
	}

	void MainWindowOrchestrator::SetHavePlcComms(bool value)
	{
		_userContextService->SetHavePlcComms(value);
	}

	void MainWindowOrchestrator::SetGlobalPressNumber(int value)
	{
		_userContextService->SetGlobalPressNumber(value);
		LegacyGlobals::GlobalPressNumber = value;
	}

	bool MainWindowOrchestrator::GetHavePlcComms() const
	{
		return _userContextService->GetHavePlcComms();
	}
}

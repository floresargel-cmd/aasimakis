#pragma once

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../Domain/Entities/ProductionProfile.h"

#include <memory>

namespace StackingPlan::Application::Services
{
	class ProductionApplicationService;
	class BasketApplicationService;
	class ProductionMonitorService;
	class UserContextService;
}

namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger;
}

namespace StackingPlan::Application::Services
{
	class MainWindowOrchestrator
	{
	public:
		MainWindowOrchestrator(
			std::shared_ptr<ProductionApplicationService> productionApplicationService,
			std::shared_ptr<BasketApplicationService> basketApplicationService,
			std::shared_ptr<ProductionMonitorService> productionMonitorService,
			std::shared_ptr<UserContextService> userContextService,
			std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger);

		QVector<StackingPlan::Domain::Entities::ProductionProfile> LoadProductionForPress(int pressNumber);
		StackingPlan::Domain::Entities::ProductionProfile GetProductionById(int id);

		bool ChangeProductionStatus(int id, int status);
		bool ChangeProductionQuantities(int id, int totalQuantity, int quantityInBaskets);
		bool ChangeProductionTemper(int id, const QString& temper);
		bool ChangeProductionLength(int id, int length);
		bool ChangeProductionLinearDensity(int id, double linearDensity);
		bool ChangeProductionDoubleLength(int id, int doubleLength);
		bool ChangeProductionOnNextBasket(int id, int onNextBasket);
		bool ChangeProductionOnNextLayer(int id, int onNextLayer);

		QString GetSoNumberByTid(int tid);
		QString GetDieByTid(int tid);
		bool ReorderProductionSequence(int beforeTid, int droppedTid);
		int GetBasketTableIdByPositionId(int positionId);
		bool MarkUsersNeedsUpdate();
		int GetPendingBasketQuantityByTidAndBasket(int tid, int basketId);
		bool DeleteBasketSpacers(int basketId);
		bool InsertBasketSpacer(
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
			int sp8);
		bool IncrementQuantityInBaskets(int tid, int quantityToAdd);

		bool RunPeriodicChecks();
		bool DoesUserNeedRefresh();
		bool ResetUserNeedsUpdate();
		bool CreateVirtualProduction(int dieNumber);
		bool MarkProductionAsNext(int tid);

		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> GetLogger() const;
		QString GetLoginName() const;
		int GetPrivilegeType() const;
		int GetGlobalPressNumber() const;
		void SetGlobalPressNumber(int value);
		bool GetHavePlcComms() const;
		void SetHavePlcComms(bool value);

	private:
		std::shared_ptr<ProductionApplicationService> _productionApplicationService;
		std::shared_ptr<BasketApplicationService> _basketApplicationService;
		std::shared_ptr<ProductionMonitorService> _productionMonitorService;
		std::shared_ptr<UserContextService> _userContextService;
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger;
	};

}

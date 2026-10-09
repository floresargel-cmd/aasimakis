#pragma once

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../Application/Interfaces/IProductionMonitorRepository.h"
#include <memory>

namespace StackingPlan::Infrastructure::Database
{
	class SqlCommandExecutor;
}

namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger;
}

namespace StackingPlan::Infrastructure::Repositories
{
	class ProductionMonitorRepository final
		: public StackingPlan::Application::Interfaces::IProductionMonitorRepository
	{
	public:
		ProductionMonitorRepository(
			std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> sqlCommandExecutor,
			std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger);

		bool NormalizeStatusMinus2Production() override;
		bool ActivateInitialProduction() override;
		bool DoesUserNeedRefresh(const QString& loginName) override;
		bool ResetUserNeedsUpdate(const QString& loginName) override;
		bool InsertVirtualProduction(int dieNumber) override;
		bool MarkProductionAsNext(int tid) override;

	private:
		std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> _sqlCommandExecutor;
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger;
	};
}

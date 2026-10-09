#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"


namespace StackingPlan::Application::Interfaces
{
	class IProductionMonitorRepository
	{
	public:
		virtual ~IProductionMonitorRepository() = default;

		virtual bool NormalizeStatusMinus2Production() = 0;
		virtual bool ActivateInitialProduction() = 0;
		virtual bool DoesUserNeedRefresh(const QString& loginName) = 0;
		virtual bool ResetUserNeedsUpdate(const QString& loginName) = 0;
		virtual bool InsertVirtualProduction(int dieNumber) = 0;
		virtual bool MarkProductionAsNext(int tid) = 0;
	};
}

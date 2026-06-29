#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"


namespace StackingPlan::Infrastructure::SqlScripts
{
	class ProductionMonitorSqlScripts
	{
	public:
		static QString NormalizeStatusMinus2Production();
		static QString ActivateInitialProduction();
		static QString SelectUserNeedsUpdate();
		static QString ResetUserNeedsUpdate();
		static QString InsertVirtualProduction();
		static QString MarkProductionAsNext();
	};
}
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "ProductionMonitorSqlScripts.h"

namespace StackingPlan::Infrastructure::SqlScripts
{
	QString ProductionMonitorSqlScripts::NormalizeStatusMinus2Production()
	{
		return
			"if ((select count(tid) from epicsProduction2 where status=-2)=1) begin "
			"declare @productionOrderOfCurrent int; "
			"set @productionOrderOfCurrent=(select min(productionOrder) from epicsProduction2 where (status=0 or status=-2) and quantityInBaskets=0); "
			"declare @tidOfNext int; "
			"if ((select count(tid) from epicsProduction2 where status=-2 and quantityInBaskets=0)>0) begin "
			"set @tidOfNext=(select tid from epicsProduction2 where status=-2 and quantityInBaskets=0) "
			"update epicsProduction2 set productionOrder=@productionOrderOfCurrent where tid=@tidOfNext; "
			"update epicsProduction2 set quantityInBaskets=0,doubleLength=0,onNextBasket=0,onNextLayer=0,status=0 where tid=@tidOfNext; "
			"update users set needsUpdate=1 "
			"DECLARE @tid int; "
			"DECLARE @incrementOrder int; "
			"set @incrementOrder=@productionOrderOfCurrent; "
			"DECLARE crs CURSOR READ_ONLY "
			"FOR SELECT tid FROM epicsProduction2 where status=0 and tid<>@tidOfNext and quantityInBaskets=0 order by productionOrder "
			"OPEN crs "
			"FETCH NEXT FROM crs INTO @tid "
			"WHILE @@FETCH_STATUS = 0 "
			"BEGIN "
			"set @incrementOrder=@incrementOrder+1; "
			"update epicsProduction2 set productionOrder=@incrementOrder where tid=@tid; "
			"FETCH NEXT FROM crs INTO @tid "
			"END "
			"CLOSE crs "
			"DEALLOCATE crs "
			"end "
			"end";
	}

	QString ProductionMonitorSqlScripts::ActivateInitialProduction()
	{
		return
			"if ((select count(tid) from epicsProduction2 where status=-1)>0) begin "
			"update epicsProduction2 "
			"set quantityInBaskets=0,doubleLength=0,onNextBasket=0,onNextLayer=0,status=0 "
			"where status=-1 "
			"update users set needsUpdate=1 "
			"end";
	}

	QString ProductionMonitorSqlScripts::SelectUserNeedsUpdate()
	{
		return
			"select needsUpdate "
			"from users "
			"where login=:loginName";
	}

	QString ProductionMonitorSqlScripts::ResetUserNeedsUpdate()
	{
		return
			"update users "
			"set needsUpdate=0 "
			"where login=:loginName";
	}

	QString ProductionMonitorSqlScripts::InsertVirtualProduction()
	{
		return
			"insert into epicsProduction2 "
			"(die,productionOrder,status,totalQuantity,quantityInBaskets,temper,length,linearDencity,doubleLength,onNextBasket,onNextLayer,scheduleDate,scheduleShift,soNum,profileSpacing,inComb,isVirtual) "
			"values "
			"(:die,(select (select max(productionOrder) from epicsProduction2)+1),-1,1,0,'T1',6000,0.1,0,0,0,'virtual','.','null',0,0,1)";
	}

	QString ProductionMonitorSqlScripts::MarkProductionAsNext()
	{
		return
			"update epicsProduction2 "
			"set status=-2 "
			"where tid=:tid";
	}
}

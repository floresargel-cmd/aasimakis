#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "BasketSqlScripts.h"

namespace StackingPlan::Infrastructure::SqlScripts
{
	QString BasketSqlScripts::GetSoNumberByTid()
	{
		return
			"SELECT soNum "
			"FROM epicsProduction2 "
			"WHERE tid = :tid";
	}

	QString BasketSqlScripts::GetDieByTid()
	{
		return
			"SELECT die "
			"FROM epicsProduction2 "
			"WHERE tid = :tid";
	}

	QString BasketSqlScripts::ReorderProductionSequence()
	{
		return
			"DECLARE @tidOfThis INT; "
			"SET @tidOfThis = :beforeTid; "

			"DECLARE @tidOfDropped INT; "
			"SET @tidOfDropped = :droppedTid; "

			"DECLARE @productionOrderOfThis INT; "
			"SET @productionOrderOfThis = ("
			"    SELECT productionOrder "
			"    FROM [productionPlan].[dbo].[epicsProduction2] "
			"    WHERE tid = @tidOfThis"
			"); "

			"UPDATE [productionPlan].[dbo].[epicsProduction2] "
			"SET productionOrder = @productionOrderOfThis "
			"WHERE tid = @tidOfDropped; "

			"DECLARE @tid INT; "
			"DECLARE @incrementOrder INT; "
			"SET @incrementOrder = @productionOrderOfThis; "

			"DECLARE crs CURSOR READ_ONLY FOR "
			"    SELECT tid "
			"    FROM [productionPlan].[dbo].[epicsProduction2] "
			"    WHERE productionOrder > @productionOrderOfThis "
			"       OR tid = @tidOfThis "
			"    ORDER BY productionOrder; "

			"OPEN crs; "
			"FETCH NEXT FROM crs INTO @tid; "

			"WHILE @@FETCH_STATUS = 0 "
			"BEGIN "
			"    SET @incrementOrder = @incrementOrder + 1; "
			"    UPDATE [productionPlan].[dbo].[epicsProduction2] "
			"    SET productionOrder = @incrementOrder "
			"    WHERE tid = @tid; "
			"    FETCH NEXT FROM crs INTO @tid; "
			"END; "

			"CLOSE crs; "
			"DEALLOCATE crs;";
	}

	QString BasketSqlScripts::GetBasketTableIdByPositionId()
	{
		return
			"SELECT basketTableId "
			"FROM positions "
			"WHERE pid = :positionId";
	}

	QString BasketSqlScripts::MarkUsersNeedsUpdate()
	{
		return
			"UPDATE users "
			"SET needsUpdate = 1";
	}

	QString BasketSqlScripts::GetPendingBasketQuantityByTidAndBasket()
	{
		return
			"SELECT TOP 1 quantity "
			"FROM PEBaskets "
			"WHERE soNum = (SELECT soNum FROM epicsProduction2 WHERE tid = :tid) "
			"  AND processed = 0 "
			"  AND basketID = :basketId "
			"ORDER BY tid";
	}

	QString BasketSqlScripts::DeleteBasketSpacers()
	{
		return
			"DELETE FROM basketSpacers "
			"WHERE basket = :basketId";
	}

	QString BasketSqlScripts::InsertBasketSpacer()
	{
		return
			"INSERT INTO basketSpacers "
			"(basket, layer, inComb, sp1, sp2, sp3, sp4, sp5, sp6, sp7, sp8) "
			"VALUES "
			"(:basketId, :layer, :inComb, :sp1, :sp2, :sp3, :sp4, :sp5, :sp6, :sp7, :sp8)";
	}

	QString BasketSqlScripts::IncrementQuantityInBaskets()
	{
		return
			"UPDATE epicsProduction2 "
			"SET quantityInBaskets = quantityInBaskets + :quantityToAdd "
			"WHERE tid = :tid";
	}
}
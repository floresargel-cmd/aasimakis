#include "LegacyProductionRepository.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../legacy/ppG.h"
#include "../Database/SqlHelper.h"
#include "ProductionRow.h"

namespace
{


	template<typename T>
	T ValueOrDefault(const QVariant& value, const T& defaultValue)
	{
		if (!value.isValid() || value.isNull())
		{
			return defaultValue;
		}

		return value.value<T>();
	}
}

QString GetCurrentJobNumber(const int pressNum)
{
	QString query =
		QString(
			"SELECT TOP 1 SONUM, SOITEMNUM, LOT "
			"FROM [SAE01PRESS0%1]."
			"[apex].[dbo].[SAW_JOB] "
			"WHERE START_DT IS NOT NULL "
			"AND STOP_DT IS NULL; ")
		.arg(pressNum);

	const QVector<QVariant> rows = SqlHelper::ExecQuery(query);

	const int columnCount = 3;

	if (rows.isEmpty())
	{
		return QString();
	}

	QString soNum = rows[0].toString();
	QString soItemNum = rows[1].toString();
	QString lot = rows[2].toString();

	return QString("%1-%2-%3")
		.arg(soNum)
		.arg(soItemNum)
		.arg(lot);
}

QVector<ProductionRow> LegacyProductionRepository::GetScheduledProductionByPress(int pressNum)
{
	QVector<ProductionRow> result;
	QString currentJobNumber = GetCurrentJobNumber(pressNum);
	// Refresh epicsProduction2.totalQuantity from Epics.foy.SOItem.OrdQty
	// soNum format assumed: <SonumBase>-<SOItemNum>-<CopyNum>
	// Example: 95538-1-1  => SonumBase=95538, SOItemNum=1
	SqlHelper::ExecNonQuery(
		QString(
			"UPDATE ep "
			"SET ep.totalQuantity = src.RunPc "
			"FROM[StackPlan].dbo.epicsProduction2 ep "
			"OUTER APPLY "
			"( "
			"SELECT TOP(1) "
			"sid.RunPc "
			"FROM[Epics].[foy].[SOItemDept] sid "
			"WHERE sid.SONUM = TRY_CONVERT(int, PARSENAME(REPLACE(LTRIM(RTRIM(ep.soNum)), '-', '.'), 3)) "
			"AND sid.SOItemNum = TRY_CONVERT(int, PARSENAME(REPLACE(LTRIM(RTRIM(ep.soNum)), '-', '.'), 2)) "
			"AND sid.deptNum = 'ORDER' "
			") src "));

	// Refresh epicsProduction2.quantityInBaskets from LayerSummary
	// using the full soNum
	SqlHelper::ExecNonQuery(
		QString(
			"UPDATE ep "
			"SET ep.quantityInBaskets = ISNULL(src.total_in_baskets, 0) "
			"FROM StackPlan.dbo.epicsProduction2 ep "
			"OUTER APPLY ( "
			"    SELECT SUM(ls.total_pcs) AS total_in_baskets "
			"    FROM StackPlan.dbo.LayerSummary ls "
			"    WHERE ls.sonum = ep.soNum "
			") src "
			"WHERE ep.pressNum = %1 "
			"  AND ep.status = %2")
		.arg(pressNum)
		.arg(PRODUCTION_STATUS_SCHEDULED));

	const QVector<QVariant> rows = SqlHelper::ExecQuery(
		QString(
			"WITH DedupedProduction AS ("
			"    SELECT "
			"        tid,"
			"        productionOrder,"
			"        die,"
			"        profileSpacing,"
			"        inComb,"
			"        totalQuantity,"
			"        quantityInBaskets,"
			"        linearDencity,"
			"        length,"
			"        doubleLength,"
			"        status,"
			"        onNextBasket,"
			"        onNextLayer,"
			"        temper,"
			"        soNum,"
			"        scheduleDate,"
			"        scheduleShift,"
			"        isVirtual,"
			"        billetsNo,"
			"        billetLength,"
			"        billetButtLength,"
			"        scrapFrontLength,"
			"        scrapRearLength,"
			"        runOutLength,"
			"        dieMultiplicity,"
			"        ROW_NUMBER() OVER ("
			"            PARTITION BY LTRIM(RTRIM(soNum)), die, pressNum "
			"            ORDER BY productionOrder ASC, tid ASC"
			"        ) AS rn "
			"    FROM [stackplan].[dbo].epicsProduction2 "
			"    WHERE status=%1 "
			"      AND quantityInBaskets < totalQuantity "
			"      AND pressNum=%2 "
			"      AND ("
			"          '%3' = '' "
			"          OR productionOrder >= ISNULL(("
			"              SELECT TOP 1 productionOrder "
			"              FROM [stackplan].[dbo].epicsProduction2 "
			"              WHERE pressNum = %2 "
			"                AND LTRIM(RTRIM(soNum)) = '%3' "
			"              ORDER BY productionOrder ASC"
			"          ), productionOrder)"
			"      ) "
			") "
			"SELECT "
			"    tid,"
			"    productionOrder,"
			"    die,"
			"    profileSpacing,"
			"    inComb,"
			"    totalQuantity,"
			"    quantityInBaskets,"
			"    linearDencity,"
			"    length,"
			"    doubleLength,"
			"    status,"
			"    onNextBasket,"
			"    onNextLayer,"
			"    temper,"
			"    soNum,"
			"    scheduleDate,"
			"    scheduleShift,"
			"    isVirtual,"
			"    billetsNo,"
			"    billetLength,"
			"    billetButtLength,"
			"    scrapFrontLength,"
			"    scrapRearLength,"
			"    runOutLength,"
			"    dieMultiplicity "
			"FROM DedupedProduction "
			"WHERE rn = 1 "
			"ORDER BY productionOrder")
		.arg(PRODUCTION_STATUS_SCHEDULED)
		.arg(pressNum)
		.arg(currentJobNumber.replace("'", "''")));

	const int columnCount = 25;
	if (rows.isEmpty())
	{
		return result;
	}

	const int rowCount = rows.count() / columnCount;
	result.reserve(rowCount);

	for (int i = 0; i < rowCount; ++i)
	{
		const int base = i * columnCount;

		ProductionRow row;
		row.tid = ValueOrDefault<int>(rows[base + 0], 0);
		row.productionOrder = ValueOrDefault<int>(rows[base + 1], 0);
		row.die = ValueOrDefault<int>(rows[base + 2], 0);
		row.profileSpacing = ValueOrDefault<int>(rows[base + 3], 0);
		row.inComb = ValueOrDefault<int>(rows[base + 4], 0);
		row.totalQuantity = ValueOrDefault<int>(rows[base + 5], 0);
		row.quantityInBaskets = ValueOrDefault<int>(rows[base + 6], 0);
		row.linearDencity = ValueOrDefault<double>(rows[base + 7], 0.0);
		row.length = ValueOrDefault<int>(rows[base + 8], 0);
		row.doubleLength = ValueOrDefault<int>(rows[base + 9], 0);
		row.status = ValueOrDefault<int>(rows[base + 10], 0);
		row.onNextBasket = ValueOrDefault<int>(rows[base + 11], 0);
		row.onNextLayer = ValueOrDefault<int>(rows[base + 12], 0);
		row.temper = ValueOrDefault<QString>(rows[base + 13], QString());
		row.soNum = ValueOrDefault<QString>(rows[base + 14], QString());
		row.scheduleDate = ValueOrDefault<QString>(rows[base + 15], QString());
		row.scheduleShift = ValueOrDefault<int>(rows[base + 16], 0);
		row.isVirtual = ValueOrDefault<bool>(rows[base + 17], false);
		row.billetsNo = ValueOrDefault<int>(rows[base + 18], 0);
		row.billetLength = ValueOrDefault<double>(rows[base + 19], 0.0);
		row.billetButtLength = ValueOrDefault<double>(rows[base + 20], 0.0);
		row.scrapFrontLength = ValueOrDefault<double>(rows[base + 21], 0.0);
		row.scrapRearLength = ValueOrDefault<double>(rows[base + 22], 0.0);
		row.runOutLength = ValueOrDefault<double>(rows[base + 23], 0.0);
		row.dieMultiplicity = ValueOrDefault<int>(rows[base + 24], 0);

		result << row;
	}

	return result;
}

void LegacyProductionRepository::EnsureProfileCodeExists(int die, int profileSpacing, int inComb)
{
	SqlHelper::ExecNonQuery(
		QString(
			"if not exists (select die from profileCode where die=%1) "
			"insert into profileCode "
			"(die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) "
			"values (%1,%2,0,1,%3,0,0,0,0,0,0,0,0)")
		.arg(die)
		.arg(profileSpacing)
		.arg(inComb));
}

QVector<QVariant> LegacyProductionRepository::GetTemperDescriptions()
{

	return SqlHelper::ExecQuery(
		QString("select RTRIM(temper) from temperTypes "),
		BASKET_CRANE_DB_CONNECTION);
}

QVector<QVariant> LegacyProductionRepository::GetTemperTypes()
{
	return SqlHelper::ExecQuery(
		QString("select temperType from temperTypes "),
		BASKET_CRANE_DB_CONNECTION);
}
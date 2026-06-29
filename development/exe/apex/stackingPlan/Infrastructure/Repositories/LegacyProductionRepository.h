#pragma once

#include "ProductionRow.h"

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"

class LegacyProductionRepository
{
public:
	static QVector<ProductionRow> GetScheduledProductionByPress(int pressNum);
	static void EnsureProfileCodeExists(int die, int profileSpacing, int inComb);

	static QVector<QVariant> GetTemperDescriptions();
	static QVector<QVariant> GetTemperTypes();
};
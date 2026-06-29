#pragma once

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"

struct ProductionRow
{
	int tid = 0;
	int productionOrder = 0;
	int die = 0;
	int profileSpacing = 0;
	int inComb = 0;
	int totalQuantity = 0;
	int quantityInBaskets = 0;
	double linearDencity = 0.0;
	int length = 0;
	int doubleLength = 0;
	int status = 0;
	int onNextBasket = 0;
	int onNextLayer = 0;
	QString temper;
	QString soNum;
	QString scheduleDate;
	int scheduleShift = 0;
	bool isVirtual = false;
	int billetsNo = 0;
	double billetLength = 0.0;
	double billetButtLength = 0.0;
	double scrapFrontLength = 0.0;
	double scrapRearLength = 0.0;
	double runOutLength = 0.0;
	int dieMultiplicity = 0;
};
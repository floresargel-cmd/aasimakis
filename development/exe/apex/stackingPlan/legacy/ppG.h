#ifndef PPG_H
#define PPG_H
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qdir.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qsettings.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qcoreapplication.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlabel.h"

#define dEmulate
//#undef dEmulate

#ifdef VB_PLC_COMMS
#include "../../common/tcp.h"
#endif
#ifdef TUX_PLC_COMMS
#endif

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qcolor.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpainterpath.h"

#include <cmath>
#include <dl_codes.h>
#include <TuxDef.h>

#include "LegacyGlobalState.h"
#include "LegacyLoggingBridge.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"

#define MAIN_DB_CONNECTION LegacyGlobals::MainDbConnectionName
#define BASKET_CRANE_DB_CONNECTION LegacyGlobals::BasketCraneConnectionName
#define DXFCOLOR(color) QColor(dxfColors[color][0] * 255., dxfColors[color][1] * 255., dxfColors[color][2] * 255.)




#define LAYER_MIN_SUPORT_LENGTH LegacyGlobals::LayerMinSupportLength
#define ARC_ANGLE_FOR_LINE LegacyGlobals::ArcAngleForLine

#define MAX_NO_OF_PROFILES_TO_LOAD LegacyGlobals::MaxNoOfProfilesToLoad


#define STARTUP_SCREEN_WIDTH LegacyGlobals::ScreenWidth
#define STARTUP_SCREEN_HEIGHT LegacyGlobals::ScreenHeight
#define STARTUP_PRESS1 LegacyGlobals::StartPress1
#define STARTUP_PRESS2 LegacyGlobals::StartPress2
#define STARTUP_HIDE_TOOLBAR_WHEN_SINGLE_PRESS LegacyGlobals::HideToolbarWhenSinglePress

#define USE_SPACERS_INFO_FROM_PLC

#define STACKER_TABLE_WRITE "XIN"
#define STACKER_TABLE_READ "XOUT"

#define PRODUCTION_STATUS_INITIAL -1
#define PRODUCTION_STATUS_SCHEDULED 0
#define PRODUCTION_STATUS_ON_PRESS 1
#define PRODUCTION_STATUS_FINISHED 2

static double GetStackPlanIniDouble(const QString& key, double defaultValue)
{
	QString iniPath = QDir(QCoreApplication::applicationDirPath()).filePath("app.ini");
	QSettings settings(iniPath, QSettings::IniFormat);

	bool ok = false;
	double value = settings.value("StackPlan/" + key, defaultValue).toDouble(&ok);

	return ok ? value : defaultValue;
}

static int GetStackPlanIniInt(const QString& key, int defaultValue)
{
	QString iniPath = QDir(QCoreApplication::applicationDirPath()).filePath("app.ini");
	QSettings settings(iniPath, QSettings::IniFormat);

	bool ok = false;
	int value = settings.value("StackPlan/" + key, defaultValue).toInt(&ok);

	return ok ? value : defaultValue;
}

#define MAX_HEIGHT_OF_LAYERS_IN_A_BASKET GetStackPlanIniDouble("MaxHeightOfLayersInBasket", 750.0)
static double GetMaxWeightOfBasket()
{
	int press = LegacyGlobals::GlobalPressNumber;

	if (press == 1)
	{
		return GetStackPlanIniDouble("MAX_WEIGHT_OF_BASKET_CRANE1", 1750.0);
	}
	else if (press == 2)
	{
		return GetStackPlanIniDouble("MAX_WEIGHT_OF_BASKET_CRANE2", 1750.0);
	}

	return GetStackPlanIniDouble("MAX_WEIGHT_OF_BASKET_CRANE1", 1750.0);
}

#define MAX_WEIGHT_OF_BASKET GetMaxWeightOfBasket()
#define MAX_LAYER_WIDTH GetStackPlanIniInt("MaxLayerWidth", 1200)
#define MAX_LAYER_WIDTH_P1 GetStackPlanIniInt("MaxLayerWidthP1", 1175)
#define MAX_LAYER_WIDTH_P2 GetStackPlanIniInt("MaxLayerWidthP2", 1175)

#define FIRST_LAYER_HEIGTH GetStackPlanIniDouble("FirstLayerHeight", 117.48)
#define SPACER_WIDTH GetStackPlanIniDouble("SpacerWidth", 1350.0)
#define SPACER_THIKNESS GetStackPlanIniDouble("SpacerThickness", 19.2)
#define SPACER_WEIGHT GetStackPlanIniDouble("SpacerWeight", 2.6)

#define PROFILE_SINGLE_LENGTH 1
#define PROFILE_DOUBLE_LENGTH 2

#define MAX_COMBS GetStackPlanIniInt("MaxCombs", 13)

#define COMB_LAYER_HEIGTH_01 GetStackPlanIniDouble("CombLayerHeight01", 117.5)
#define COMB_LAYER_HEIGTH_02 GetStackPlanIniDouble("CombLayerHeight02", 167.5)
#define COMB_LAYER_HEIGTH_03 GetStackPlanIniDouble("CombLayerHeight03", 217.5)
#define COMB_LAYER_HEIGTH_04 GetStackPlanIniDouble("CombLayerHeight04", 267.5)
#define COMB_LAYER_HEIGTH_05 GetStackPlanIniDouble("CombLayerHeight05", 317.5)
#define COMB_LAYER_HEIGTH_06 GetStackPlanIniDouble("CombLayerHeight06", 367.5)
#define COMB_LAYER_HEIGTH_07 GetStackPlanIniDouble("CombLayerHeight07", 417.5)
#define COMB_LAYER_HEIGTH_08 GetStackPlanIniDouble("CombLayerHeight08", 467.5)
#define COMB_LAYER_HEIGTH_09 GetStackPlanIniDouble("CombLayerHeight09", 517.5)
#define COMB_LAYER_HEIGTH_10 GetStackPlanIniDouble("CombLayerHeight10", 567.5)
#define COMB_LAYER_HEIGTH_11 GetStackPlanIniDouble("CombLayerHeight11", 617.5)
#define COMB_LAYER_HEIGTH_12 GetStackPlanIniDouble("CombLayerHeight12", 667.5)

#define PRIVILEGE_TYPE_OFFICE  0x00001
#define PRIVILEGE_TYPE_PRESS   0x00002
#define PRIVILEGE_TYPE_STACKER 0x00004
#include <algorithm>
#ifndef TUX_PLC_COMMS
#define MIN(a,b) std::min(a,b)
#endif
#define MAX(a,b) std::max(a,b)

#ifdef dEmulate
#define dxfDirPath "I://ProductionPlanDxfs//"
#else
#define dxfDirPath "I://ProductionPlanDxfs//"
#endif

#define NOTIFY_OTHERS LegacyBridge::NotifyOthers()







static int getCombWithHeightMoreThan(double h)
{
	if (COMB_LAYER_HEIGTH_01 > h) return 1;
	else if (COMB_LAYER_HEIGTH_02 > h) return 2;
	else if (COMB_LAYER_HEIGTH_03 > h) return 3;
	else if (COMB_LAYER_HEIGTH_04 > h) return 4;
	else if (COMB_LAYER_HEIGTH_05 > h) return 5;
	else if (COMB_LAYER_HEIGTH_06 > h) return 6;
	else if (COMB_LAYER_HEIGTH_07 > h) return 7;
	else if (COMB_LAYER_HEIGTH_08 > h) return 8;
	else if (COMB_LAYER_HEIGTH_09 > h) return 9;
	else if (COMB_LAYER_HEIGTH_10 > h) return 10;
	else if (COMB_LAYER_HEIGTH_11 > h) return 11;
	else if (COMB_LAYER_HEIGTH_12 > h) return 12;
	return 13;
}

static double getHeightOfComb(int c)
{
	if (c == 1) return COMB_LAYER_HEIGTH_01;
	else if (c == 2) return COMB_LAYER_HEIGTH_02;
	else if (c == 3) return COMB_LAYER_HEIGTH_03;
	else if (c == 4) return COMB_LAYER_HEIGTH_04;
	else if (c == 5) return COMB_LAYER_HEIGTH_05;
	else if (c == 6) return COMB_LAYER_HEIGTH_06;
	else if (c == 7) return COMB_LAYER_HEIGTH_07;
	else if (c == 8) return COMB_LAYER_HEIGTH_08;
	else if (c == 9) return COMB_LAYER_HEIGTH_09;
	else if (c == 10) return COMB_LAYER_HEIGTH_10;
	else if (c == 11) return COMB_LAYER_HEIGTH_11;
	else if (c == 12) return COMB_LAYER_HEIGTH_12;
	return COMB_LAYER_HEIGTH_12 + 1000.;
}

static QColor getGlobalColor()
{
	LegacyGlobals::GlobalColorIndex++;

	if (LegacyGlobals::GlobalColorIndex == 0) return QColor(29, 139, 209);
	else if (LegacyGlobals::GlobalColorIndex == 1) return QColor(143, 188, 139);
	else if (LegacyGlobals::GlobalColorIndex == 2) return QColor(210, 180, 140);
	else if (LegacyGlobals::GlobalColorIndex == 3) return QColor(32, 178, 170);
	else if (LegacyGlobals::GlobalColorIndex == 4) return QColor(176, 196, 222);
	else if (LegacyGlobals::GlobalColorIndex == 5) return QColor(221, 160, 221);
	else if (LegacyGlobals::GlobalColorIndex == 6) return QColor(99, 0, 99);
	else if (LegacyGlobals::GlobalColorIndex == 7) return QColor(255, 130, 132);
	else if (LegacyGlobals::GlobalColorIndex == 8) return QColor(0, 101, 206);
	else if (LegacyGlobals::GlobalColorIndex == 9) return QColor(206, 207, 255);
	else if (LegacyGlobals::GlobalColorIndex == 10) return QColor(0, 0, 132);
	else if (LegacyGlobals::GlobalColorIndex == 11) return QColor(255, 0, 255);
	else if (LegacyGlobals::GlobalColorIndex == 12) return QColor(255, 255, 0);
	else if (LegacyGlobals::GlobalColorIndex == 13) return QColor(0, 255, 255);
	else if (LegacyGlobals::GlobalColorIndex == 14) return QColor(132, 0, 132);
	else if (LegacyGlobals::GlobalColorIndex == 15) return QColor(132, 0, 0);
	else if (LegacyGlobals::GlobalColorIndex == 16) return QColor(0, 0, 255);
	else if (LegacyGlobals::GlobalColorIndex == 17) return QColor(0, 207, 255);
	else if (LegacyGlobals::GlobalColorIndex == 18) return QColor(206, 255, 255);
	else if (LegacyGlobals::GlobalColorIndex == 19) return QColor(156, 207, 255);
	else if (LegacyGlobals::GlobalColorIndex == 20) return QColor(255, 154, 206);
	else if (LegacyGlobals::GlobalColorIndex == 21) return QColor(206, 154, 255);
	else if (LegacyGlobals::GlobalColorIndex == 22) return QColor(255, 207, 156);
	else if (LegacyGlobals::GlobalColorIndex == 23) return QColor(49, 101, 255);
	else if (LegacyGlobals::GlobalColorIndex == 24) return QColor(49, 207, 206);
	else if (LegacyGlobals::GlobalColorIndex == 25) return QColor(255, 207, 0);
	else if (LegacyGlobals::GlobalColorIndex == 26) return QColor(255, 101, 0);
	else
	{
		LegacyGlobals::GlobalColorIndex = -1;
		return QColor(0, 0, 0);
	}
}

static double getMinXDistanceOfPaths(
	QPainterPath p1,
	QPainterPath p2,
	int profileCode,
	const QString& drawingPath = "")
{
	double stepDist = 0.;
	double maxDist = 0.;
	double minDist = 0.;
	double dist = 0.;
	double validDist = 0.;
	double oldValidDist = 0.;

	double maxDistance = MAX(p1.boundingRect().width(), p2.boundingRect().width());
	double step = 0.5 * MIN(p1.boundingRect().width(), p2.boundingRect().width());

	bool found = false;
	int safetyCounter = 0;
	for (stepDist = -1.0; stepDist < (maxDistance + step + 2.0); stepDist += step)
	{
		if (++safetyCounter > 3)
		{
			QString errorMessage =
				QString("ALERT: Drawing/profile spacing calculation exceeded safety limit. "
					"Profile: %1 | Drawing: %2")
				.arg(profileCode)
				.arg(drawingPath);

			LegacyBridge::LogError(errorMessage);



			return 0.0;
		}

		if (p1.intersects(p2.translated(-stepDist, 0.0)))
			break;

		found = true;
	}

	if (!found)
		return 0.0;

	maxDist = stepDist;
	minDist = stepDist - step;

	for (int i = 0; i < 50; i++)
	{
		dist = 0.5 * (maxDist + minDist);

		if (p1.intersects(p2.translated(-dist, 0.0)))
		{
			maxDist = dist;
		}
		else
		{
			minDist = dist;
			validDist = dist;

			if ((i != 0) && (fabs(validDist - oldValidDist) < 0.1))
				break;

			oldValidDist = validDist;
		}
	}

	return validDist;
}

#endif

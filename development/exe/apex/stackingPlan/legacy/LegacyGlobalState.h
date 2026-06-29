#pragma once

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../Application/Services/LegacyRuntimeContext.h"

namespace LegacyGlobals
{
	extern bool HavePlcComms;
	extern int GlobalPressNumber;
	extern int GlobalColorIndex;
	extern int LayerMinSupportLength;
	extern double ArcAngleForLine;
	extern QString LoginName;
	extern int MaxNoOfProfilesToLoad;
	extern int PrivilegeType;

	extern int ScreenWidth;
	extern int ScreenHeight;
	extern bool StartPress1;
	extern bool StartPress2;
	extern bool HideToolbarWhenSinglePress;

	extern QString MainDbConnectionName;
	extern QString MainDbDriver;
	extern QString MainDbServer;
	extern QString MainDbDatabase;
	extern QString MainDbUser;
	extern QString MainDbPassword;
	extern QString MainDbConnectionString;

	extern QString BasketCraneConnectionName;
	extern QString BasketCraneDriver;
	extern QString BasketCraneServer;
	extern QString BasketCraneDatabase;
	extern QString BasketCraneUser;
	extern QString BasketCranePassword;
	extern QString BasketCraneConnectionString;



	void ApplyRuntimeContext(
		const StackingPlan::Application::Services::LegacyRuntimeContext& runtimeContext);
}
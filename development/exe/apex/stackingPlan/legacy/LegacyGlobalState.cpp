#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../Application/Services/LegacyRuntimeContext.h"
#include "LegacyGlobalState.h"

namespace LegacyGlobals
{
	bool HavePlcComms = false;
	int GlobalPressNumber = 1;
	int GlobalColorIndex = -1;
	int LayerMinSupportLength = 75;
	double ArcAngleForLine = 20.0;
	QString LoginName;
	int MaxNoOfProfilesToLoad = 1;
	int PrivilegeType = 0;

	int ScreenWidth = 1920;
	int ScreenHeight = 1080;
	bool StartPress1 = true;
	bool StartPress2 = false;
	bool HideToolbarWhenSinglePress = true;

	QString MainDbConnectionName = "MainDbConnectionNew";
	QString MainDbDriver = "QODBC";
	QString MainDbServer;
	QString MainDbDatabase;
	QString MainDbUser;
	QString MainDbPassword;
	QString MainDbConnectionString;

	QString BasketCraneConnectionName = "basketCraneConnectionNew";
	QString BasketCraneDriver = "QODBC";
	QString BasketCraneServer;
	QString BasketCraneDatabase;
	QString BasketCraneUser;
	QString BasketCranePassword;
	QString BasketCraneConnectionString;

	void ApplyRuntimeContext(
		const StackingPlan::Application::Services::LegacyRuntimeContext& runtimeContext)
	{
		LoginName = runtimeContext.loginName;
		PrivilegeType = runtimeContext.privilegeType;
		MaxNoOfProfilesToLoad = runtimeContext.maxNoOfProfilesToLoad;
		ArcAngleForLine = runtimeContext.arcAngleForLine;
		LayerMinSupportLength = runtimeContext.layerMinSupportLength;
		GlobalPressNumber = runtimeContext.globalPressNumber;
		GlobalColorIndex = runtimeContext.globalColorIndex;
		HavePlcComms = runtimeContext.havePlcComms;

		ScreenWidth = runtimeContext.screenWidth;
		ScreenHeight = runtimeContext.screenHeight;
		StartPress1 = runtimeContext.startPress1;
		StartPress2 = runtimeContext.startPress2;
		HideToolbarWhenSinglePress = runtimeContext.hideToolbarWhenSinglePress;

		MainDbConnectionName = runtimeContext.mainDbConnectionName;
		MainDbDriver = runtimeContext.mainDbDriver;
		MainDbServer = runtimeContext.mainDbServer;
		MainDbDatabase = runtimeContext.mainDbDatabase;
		MainDbUser = runtimeContext.mainDbUser;
		MainDbPassword = runtimeContext.mainDbPassword;
		MainDbConnectionString = runtimeContext.mainDbConnectionString;

		BasketCraneConnectionName = runtimeContext.basketCraneConnectionName;
		BasketCraneDriver = runtimeContext.basketCraneDriver;
		BasketCraneServer = runtimeContext.basketCraneServer;
		BasketCraneDatabase = runtimeContext.basketCraneDatabase;
		BasketCraneUser = runtimeContext.basketCraneUser;
		BasketCranePassword = runtimeContext.basketCranePassword;
		BasketCraneConnectionString = runtimeContext.basketCraneConnectionString;
	}
}
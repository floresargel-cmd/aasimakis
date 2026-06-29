#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"


namespace StackingPlan
{
	namespace Application
	{
		namespace Services
		{
			class LegacyRuntimeContext
			{
			public:
				bool useLocalDb = false;

				QString loginName;
				int privilegeType = 0;
				int maxNoOfProfilesToLoad = 1;
				double arcAngleForLine = 20.0;
				int layerMinSupportLength = 75;
				int globalPressNumber = 1;
				int globalColorIndex = -1;
				bool havePlcComms = false;

				int screenWidth = 1920;
				int screenHeight = 1080;
				bool startPress1 = true;
				bool startPress2 = false;
				bool hideToolbarWhenSinglePress = true;

				QString mainDbConnectionName = "MainDbConnectionNew";
				QString mainDbDriver = "QODBC";
				QString mainDbServer;
				QString mainDbDatabase;
				QString mainDbUser;
				QString mainDbPassword;
				QString mainDbConnectionString;

				QString basketCraneConnectionName = "basketCraneConnectionNew";
				QString basketCraneDriver = "QODBC";
				QString basketCraneServer;
				QString basketCraneDatabase;
				QString basketCraneUser;
				QString basketCranePassword;
				QString basketCraneConnectionString;
			};
		}
	}
}
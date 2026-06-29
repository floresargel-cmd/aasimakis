#pragma once

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstringlist.h"
#include <vector>

namespace StackingPlan
{
	namespace Infrastructure
	{
		namespace SqlScripts
		{
			class LegacyDatabaseTableDefinition
			{
			public:
				QString tableName;
				QStringList columnNames;
				QStringList columnTypes;
			};

			class LegacyDatabaseTableDefinitions
			{
			public:
				static std::vector<LegacyDatabaseTableDefinition> GetMainTableDefinitions();
			};
		}
	}
}
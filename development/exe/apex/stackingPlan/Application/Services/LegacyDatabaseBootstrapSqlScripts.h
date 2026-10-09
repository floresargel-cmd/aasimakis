#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstringlist.h"


namespace StackingPlan::Infrastructure::SqlScripts
{
	class LegacyDatabaseBootstrapSqlScripts
	{
	public:
		static QString CreateTableStatement(
			const QString& tableName,
			const QStringList& columnNames,
			const QStringList& columnTypes);

		static QString EnsureStackPlanDatabase();
		static QString UseStackPlanDatabase();
		static QString UseBasketCraneDatabase();

		static QString EnsureAdminUser();
		static QString EnsureLayerMinSupportLengthSetting();
		static QString EnsureArcAngleSetting();
		static QString TrimUserLog();

		static QString SelectArcAngleForLine();
		static QString SelectMaxProfilesToLoad(const QString& loginName);
	};
}

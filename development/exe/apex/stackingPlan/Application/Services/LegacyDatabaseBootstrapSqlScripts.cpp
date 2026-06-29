#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstringlist.h"
#include "LegacyDatabaseBootstrapSqlScripts.h"

namespace StackingPlan::Infrastructure::SqlScripts
{
	QString LegacyDatabaseBootstrapSqlScripts::CreateTableStatement(
		const QString& tableName,
		const QStringList& columnNames,
		const QStringList& columnTypes)
	{
		QString statement =
			QString("IF NOT EXISTS (SELECT [name] FROM sys.tables WHERE [name] ='%1')")
			.arg(tableName);

		statement += QString("CREATE TABLE %1 (").arg(tableName);

		for (int i = 0; i < columnNames.length(); i++)
		{
			statement += columnNames[i] + " " + columnTypes[i];
			if (i != (columnNames.length() - 1))
			{
				statement += ",";
			}
		}

		statement += ")";
		return statement;
	}

	QString LegacyDatabaseBootstrapSqlScripts::EnsureStackPlanDatabase()
	{
		return "if not exists(select * from sys.databases where name='StackPlan') create database StackPlan";
	}

	QString LegacyDatabaseBootstrapSqlScripts::UseStackPlanDatabase()
	{
		return "USE StackPlan";
	}

	QString LegacyDatabaseBootstrapSqlScripts::UseBasketCraneDatabase()
	{
		return "USE basketCraneData";
	}

	QString LegacyDatabaseBootstrapSqlScripts::EnsureAdminUser()
	{
		return
			"if not exists(select login from users where login = 'admin') "
			"INSERT INTO users ( login, password, office, press, stacker, needsUpdate, maxNoOfProfilesToLoad) "
			"VALUES ('admin', '123', '1', 1, 1, 0, 20)";
	}

	QString LegacyDatabaseBootstrapSqlScripts::EnsureLayerMinSupportLengthSetting()
	{
		return
			"if not exists (select name from settings where name='namelayerMinSuportLength      ') "
			"insert into settings (name,value) values ('namelayerMinSuportLength','75')";
	}

	QString LegacyDatabaseBootstrapSqlScripts::EnsureArcAngleSetting()
	{
		return
			"if not exists (select name from settings where name='arcAngleForLine               ') "
			"insert into settings (name,value) values ('arcAngleForLine','20')";
	}

	QString LegacyDatabaseBootstrapSqlScripts::TrimUserLog()
	{
		return "delete from userLog where tid not in(select top 100000 tid from userLog order by sec desc)";
	}

	QString LegacyDatabaseBootstrapSqlScripts::SelectArcAngleForLine()
	{
		return "select value from settings where name='arcAngleForLine               '";
	}

	QString LegacyDatabaseBootstrapSqlScripts::SelectMaxProfilesToLoad(const QString& loginName)
	{
		return QString("select maxNoOfProfilesToLoad from users where login='%1'")
			.arg(loginName);
	}
}
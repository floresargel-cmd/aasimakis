#include "LegacySqlBridge.h"

#include "../Infrastructure/Database/SqlCommandExecutor.h"
#include "LegacyLoggingBridge.h"

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qhash.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqldatabase.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqlerror.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqlquery.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqlrecord.h"
#include "ppG.h"
#include <memory>

namespace
{
	QHash<QString, std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor>> g_sqlExecutors;
}

namespace LegacyBridge
{
	void SetSqlCommandExecutor(
		const QString& connectionName,
		const std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor>& sqlCommandExecutor)
	{
		g_sqlExecutors.insert(connectionName, sqlCommandExecutor);
	}

	std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor>
		GetSqlCommandExecutor(const QString& connectionName)
	{
		if (g_sqlExecutors.contains(connectionName))
			return g_sqlExecutors.value(connectionName);

		return std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor>();
	}

	bool ExecuteNonQuery(const QString& sql)
	{
		return ExecuteNonQuery(sql, MAIN_DB_CONNECTION);
	}

	QVariant ExecuteScalar(const QString& sql)
	{
		return ExecuteScalar(sql, MAIN_DB_CONNECTION);
	}

	QVector<QVariant> ExecuteQuery(const QString& sql)
	{
		return ExecuteQuery(sql, MAIN_DB_CONNECTION);
	}

	bool ExecuteNonQuery(const QString& sql, const QString& connectionName)
	{
		auto executor = GetSqlCommandExecutor(connectionName);

		if (!executor)
		{
			LegacyBridge::LogError("No executor for connection: " + connectionName);
			return false;
		}

		const bool success = executor->ExecuteNonQuery(sql);

		if (!success)
		{
			LegacyBridge::LogError(
				QString("ExecuteNonQuery failed: %1 | SQL: %2 | Connection: %3")
				.arg(executor->GetLastError())
				.arg(sql)
				.arg(connectionName));
		}

		return success;
	}

	QVariant ExecuteScalar(const QString& sql, const QString& connectionName)
	{
		auto executor = GetSqlCommandExecutor(connectionName);

		if (!executor)
		{
			LegacyBridge::LogError("No executor for connection: " + connectionName);
			return QVariant();
		}

		const QVariant result = executor->ExecuteScalar(sql);

		if (!executor->GetLastError().isEmpty())
		{
			LegacyBridge::LogWarning(
				QString("ExecuteScalar warning: %1 | SQL: %2 | Connection: %3")
				.arg(executor->GetLastError())
				.arg(sql)
				.arg(connectionName));
		}

		return result;
	}

	QVector<QVariant> ExecuteQuery(const QString& sql, const QString& connectionName)
	{
		LegacyBridge::LogInfo("Executing query: " + sql + " | Connection: " + connectionName);
		QVector<QVariant> results;

		auto executor = GetSqlCommandExecutor(connectionName);

		if (!executor)
		{
			LegacyBridge::LogError("No executor for connection: " + connectionName);
			return results;
		}

		if (!executor->Open())
		{
			LegacyBridge::LogError(
				QString("ExecuteQuery open failed: %1 | SQL: %2 | Connection: %3")
				.arg(executor->GetLastError())
				.arg(sql)
				.arg(connectionName));
			return results;
		}

		QSqlDatabase db = executor->GetDatabase();
		QSqlQuery query(db);

		if (!query.exec(sql))
		{
			LegacyBridge::LogError(
				QString("ExecuteQuery failed: %1 | SQL: %2 | Connection: %3")
				.arg(query.lastError().text())
				.arg(sql)
				.arg(connectionName));
			return results;
		}

		while (query.next())
		{
			const int columnCount = query.record().count();

			for (int i = 0; i < columnCount; ++i)
			{
				results.push_back(query.value(i));
			}
		}

		return results;
	}
}
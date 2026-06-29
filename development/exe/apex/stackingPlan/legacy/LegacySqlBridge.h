#pragma once

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include <memory>

namespace StackingPlan
{
	namespace Infrastructure
	{
		namespace Database
		{
			class SqlCommandExecutor;
		}
	}
}

namespace LegacyBridge
{
	void SetSqlCommandExecutor(
		const QString& connectionName,
		const std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor>& sqlCommandExecutor);

	std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor>
		GetSqlCommandExecutor(const QString& connectionName);

	bool ExecuteNonQuery(const QString& sql);
	QVariant ExecuteScalar(const QString& sql);
	QVector<QVariant> ExecuteQuery(const QString& sql);
	QVector<QVariant> ExecuteQuery(const QString& sql, const QString& connectionName);
	bool ExecuteNonQuery(const QString& sql, const QString& connectionName);
	QVariant ExecuteScalar(const QString& sql, const QString& connectionName);
}
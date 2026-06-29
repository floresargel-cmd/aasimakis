#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../legacy/LegacySqlBridge.h"
#include "SqlHelper.h"

bool SqlHelper::ExecNonQuery(const QString& sql)
{
	return LegacyBridge::ExecuteNonQuery(sql);
}

bool SqlHelper::ExecNonQuery(const QString& sql, const QString& connectionName)
{
	(void)connectionName;
	return LegacyBridge::ExecuteNonQuery(sql);
}

QVariant SqlHelper::GetFirst(const QString& sql)
{
	return LegacyBridge::ExecuteScalar(sql);
}

QVariant SqlHelper::GetFirst(const QString& sql, const QString& connectionName)
{
	(void)connectionName;
	return LegacyBridge::ExecuteScalar(sql);
}

QVector<QVariant> SqlHelper::ExecQuery(const QString& sql)
{

	return LegacyBridge::ExecuteQuery(sql);
}

QVector<QVariant> SqlHelper::ExecQuery(const QString& sql, const QString& connectionName)
{
	(void)connectionName;
	return LegacyBridge::ExecuteQuery(sql, connectionName);
}

QVariant SqlHelper::GetScalar(const QString& sql)
{
	return LegacyBridge::ExecuteScalar(sql);
}
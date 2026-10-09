#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"


class SqlHelper
{
public:
	static bool ExecNonQuery(const QString& sql);
	static bool ExecNonQuery(const QString& sql, const QString& connectionName);

	static QVariant GetFirst(const QString& sql);
	static QVariant GetFirst(const QString& sql, const QString& connectionName);

	static QVector<QVariant> ExecQuery(const QString& sql);
	static QVector<QVariant> ExecQuery(const QString& sql, const QString& connectionName);

	static QVariant GetScalar(const QString& sql);
};

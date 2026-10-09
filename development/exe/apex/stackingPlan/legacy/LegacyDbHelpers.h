#pragma once
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqlquery.h"


inline QVariant GETONE(const QString& sql)
{
	QSqlQuery query;

	if (!query.exec(sql))
	{
		return QVariant();
	}

	if (query.next())
	{
		return query.value(0);
	}

	return QVariant();
}

inline bool apexExecQuery(const QString& sql)
{
	QSqlQuery query;
	return query.exec(sql);
}

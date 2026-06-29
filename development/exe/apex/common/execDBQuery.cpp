#include "execDBQuery.h"
#include "applogger.h"

QVector<QVariant> execDBQuery(QString stmnt, QString connection)
{
    // Place any cross-cutting behavior here (logging, timing, retry, etc.)
	LOG("execDBQuery", QString("INFO: Executing query on connection '%1': %2 [%3]").arg(connection).arg(stmnt).arg(QDateTime::currentDateTime().toString("HH:mm:ss,zzz")));
    QVector<QVariant> ans = execQuery(stmnt, connection);
    LOG("execDBQuery", QString("INFO: Completed query on connection '%1': %2 [%3]").arg(connection).arg(stmnt).arg(QDateTime::currentDateTime().toString("HH:mm:ss,zzz")));
    return ans;
}
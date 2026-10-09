#include "pch.h"
#include "Gsw.h"
void initializeSbWarehouseGlobals()
{
}
QVector<QVariant> noVitalExecQuery(QString stmnt,QString connection,QString *errorString)
{
	QSqlQuery query(QSqlDatabase::database(connection));
	query.setForwardOnly(true);
	if (!query.exec(stmnt))
	{
		(*errorString)=query.lastError().text();
		return QVector<QVariant>();
	}
	QVector<QVariant> ans;
	while (query.next())
		ans.append(query.value(0));
	return ans;
}
QVariant noVitalGetFirst(QString stmnt,QString connection,QString *errorString)
{
	QVector<QVariant> resArray=noVitalExecQuery(stmnt,connection,errorString);
	if (resArray.count()>0)
		return resArray[0];
	else
		return 0.;
}
void logV(QString description,QString type)
{
	logV(description,type,mDb);
}
void log(QString description,QString type)
{
	log(description,type,mDb);
}
void logGui(QString mess,QString description,QString from,QString type)
{
#ifdef _DEBUG
	if (mess.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	execQuery(QString("insert into logGui (mess,details,messenger,type,time) values('%1','%2','%3','%4','%5')").arg(mess.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(from.left(10)).arg(type.left(10)).arg(timeForLog),mDb);
	logV(QString ("%1(%2) sender:%3").arg(mess).arg(description).arg(from),type,mDb);
}

QVector<QVariant> getIdleCassettes(QString dbName)
{
	return execQuery(QString("select cassette from pos where cassette<>0 and (cassette in (%1)) order by cassette").arg(selectIdleCassettes),dbName);
}
QVector<QVariant> getIdleEmptyCassettes(QString dbName)
{
	return execQuery(selectIdleCassettes +QString("and ticket=0 and cassette<>0 order by cassette"),dbName);
}
QVector<QVariant> getNotIdleCassettes(QString dbName)
{
	return execQuery(QString("select cassette from pos where cassette<>0 and (cassette not in (%1)) order by cassette").arg(selectIdleCassettes),dbName);
}
bool getPassword(QString title,QString text,QString pass,QWidget *p)
{
	if (adminMode)
		return true;
#ifdef _DEBUG
	return true;
#endif
	QString userTxt=QInputDialog::getText(p,title,title,QLineEdit::Password);
	if (uCheckPassword(userTxt))
		return true;
	if (pass.length()>0)
	{
		if (userTxt.toLower()==pass)
			return true;
		if (userTxt.toLower()==passStrong)
			return true;
	}
	if (userTxt.length()!=0)
	{
		QMessageBox::critical(p,"Wrong password","This is the wrong password.");
		return false;
	}
	return false;
}

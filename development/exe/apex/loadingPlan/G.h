#ifndef GLOBAL_H
#include "utilities.h"

#define GLOBAL_H
#include <QMessageBox>
//#define useLocalDb
#define whDb QString("warehouseDbConection")
#define eDb QString("epicsDbConection")

#define PACKETSCALE 0.250
#define MAXCOLUMNWIDTH 500.
#define woodWidth 50

extern int *minAvailableManualOutOrder;
#define MAXCOLUMNGUIWIDTH MAXCOLUMNWIDTH*PACKETSCALE
////
//static QString plcConnectionName="plcCommConnection";
//static QString plcOdbcName="remotePCMsSql";
//static QString plcDatabaseName="packRobotPlcComm";
////
//static QString cosmosConnectionName="cosmosConnection";
////
/////////////////////////////////////////////////////////////////////
//static QString manualExportsTable="_manualExports";
//static QString positionsTable="_positions";
//static QString loadingsTableName="loadings";
/////////////////////////////////////////////////////////////////////
//static QString cosmosLoadingsTableName="loadings";
//static QString cosmosDimensionsTableName="dimension";
///////////////////////////////////////////////////////////////////
#define CRANE_EXPORT_STATION_1 1
#define CRANE_EXPORT_STATION_2 2
#define MANUAL_EXPORTS_STATION 3
#include <QtGui>
#include <QFile>
///////////////////////////////////////////////////////////////////
//static QString packetCodeToString(int code)
//{
//	if (code<=0)
//		return "0";
//	QString ans=QVariant(code).toString();
//	while (ans.length()<10)
//		ans.prepend("0");
//	return ans;
//}
//static QString to6digitProfile(int code)
//{
//	if (code<=0)
//		return "0";
//	QString ans=QVariant(code).toString();
//	while (ans.length()<6)
//		ans.prepend("0");
//	return ans;
//}
static void exit(QString title,QString mess)
{
	QString logFileName=QString("error.%1.txt").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd")+QTime::currentTime().toString(".hh.mm.ss"));
	QFile file(logFileName);
  if (!file.open(QIODevice::Append|QIODevice::Text))
		exit(0);
	QTextStream out(&file);
	out<<QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss\t");
	out<<"-"+title+"-\t -"+mess+"\n";
	file.close();
	QDesktopServices::openUrl(logFileName);
	//ShellExecute(0,"open",logFileName.toLatin1(), NULL, NULL, SW_SHOWNORMAL);
	exit(0);
}
#define EXIT(title,mess)       (exit(title,mess))
//static QVector<QVariant> execQuery(QString stmnt,QString connection)
//{
//	QSqlQuery query(QSqlDatabase::database(connection));
//	query.setForwardOnly(true);
//	if (!query.exec(stmnt))
//		EXIT(("Error"),query.lastError().text()+"\nstmnt: "+stmnt);
//	//
//	QVector<QVariant> ans;
//	while (query.next())
//	{
//		ans.append(query.value(0));
//	}
//	return ans;
//}
static QVariant GETFIRST(QString query,QString connectionName)
{
	QVector<QVariant> resArray=execQuery(query,connectionName);
	if (resArray.count()>0)
		return resArray[0];
	else
		return 0.;
}
static QVariant GETONEATMOST(QString query,QString connectionName)
{
	QVector<QVariant> resArray=execQuery(query,connectionName);
	if (resArray.count()==0)
		return 0;
	if (resArray.count()==1)
		return resArray[0];
	EXIT("database error",QString("query: '%1' returned %2 results").arg(query).arg(resArray.count()));
	return -1;
}
static QVariant GETONE(QString query,QString connectionName)
{
	QVector<QVariant> resArray=execQuery(query,connectionName);
	if (resArray.count()==0)
		EXIT("database error",QString("query: '%1' returned no results").arg(query));
	if (resArray.count()==1)
		return resArray[0];
	EXIT("database error",QString("query: '%1' returned %2 results").arg(query).arg(resArray.count()));
	return -1;
}


static QString getImageOfProfile(QString profile)
{
	QString ans;
#ifdef useLocalDb
	QString imgDir=QDir::currentPath()+"/pics/";
#else
	QString imgDir="\\\\apexsrv02.apexhq.apexextrusions.ca\\files\\Profile Drawings\\Press Drawings\\";
#endif
	int profileCode=profile.toInt();
	     if (profileCode<1100)ans=imgDir+QString("1000\\%1.JPG").arg(profileCode);
	else if (profileCode<1200)ans=imgDir+QString("1100\\%1.JPG").arg(profileCode);
	else if (profileCode<1300)ans=imgDir+QString("1200\\%1.JPG").arg(profileCode);
	else if (profileCode<1400)ans=imgDir+QString("1300\\%1.JPG").arg(profileCode);
	else if (profileCode<1500)ans=imgDir+QString("1400\\%1.JPG").arg(profileCode);
	else if (profileCode<1600)ans=imgDir+QString("1500\\%1.JPG").arg(profileCode);
	else if (profileCode<1700)ans=imgDir+QString("1600\\%1.JPG").arg(profileCode);
	else if (profileCode<1800)ans=imgDir+QString("1700\\%1.JPG").arg(profileCode);
	else if (profileCode<1900)ans=imgDir+QString("1800\\%1.JPG").arg(profileCode);
	else if (profileCode<2000)ans=imgDir+QString("1900\\%1.JPG").arg(profileCode);
	if (!QFile().exists(ans))
		ans="./default.png";
	return ans;
}
#endif

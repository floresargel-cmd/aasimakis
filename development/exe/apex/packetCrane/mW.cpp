#include "pch.h"
#include "mW.h"
extern QSplashScreen *splash;
#define inMyLaptop
#undef inMyLaptop
void createTable(QString tableName,QStringList colNames,QStringList colTypes)
{
	QString stmnt=QString("if not exists (select [name] from sys.tables where [name]='%1') create table %1 (").arg(tableName);
	for (int i=0;i<colNames.length();i++)
	{
		stmnt+=colNames[i]+" "+colTypes[i];
		if (i!=(colNames.length()-1))
			stmnt+=",";
	}
	stmnt+=")";
	apexExecQuery(stmnt,MAIN_DB_CONNECTION);
}
void initializeTables()
{
	if (getFirst("select count(tid) from positions",MAIN_DB_CONNECTION).toInt()==0)
	{
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('fork'									,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_FORK							).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
#ifdef keepPacketCraneOldFunctionality
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('newSB'									,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_NEWSB							).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('newPacket'							,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_NEWPACKET					).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('sbExit'								,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_SBEXIT						).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('packetExit'						,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_PACKETEXIT				).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
#endif
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('sbPacketEntrance'			,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_SB_PACKET_IMPORT	).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('crane'									,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_CRANE							).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('westConv'							,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_WESTCONV					).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('eastConv'							,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_EASTCONV					).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
#ifdef keepPacketCraneOldFunctionality
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('storeExit'							,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_STOREEXIT					).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
#endif

		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('station exit 1'				,%1,0,0,%2,-1,-1,-1)").arg(stationExport1PlcId			).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('station import'				,%1,0,0,%2,-1,-1,-1)").arg(stationImportPlcId				).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('station exit 2'				,%1,0,0,%2,-1,-1,-1)").arg(stationExport2PlcId			).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('station exit back 1'   ,%1,0,0,%2,-1,-1,-1)").arg(stationBackExport1PlcId	).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('station back import'   ,%1,0,0,%2,-1,-1,-1)").arg(stationBackImportPlcId		).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('station back exit 2'   ,%1,0,0,%2,-1,-1,-1)").arg(stationBackExport2PlcId	).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));

		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('packing exit 1'				,%1,0,0,%2,-1,-1,-1)").arg(packingExport1PlcId			).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('packing import'				,%1,0,0,%2,-1,-1,-1)").arg(packingImportPlcId				).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('packing exit 2'				,%1,0,0,%2,-1,-1,-1)").arg(packingExport2PlcId			).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('packing exit back 1'   ,%1,0,0,%2,-1,-1,-1)").arg(packingBackExport1PlcId	).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('packing back import'   ,%1,0,0,%2,-1,-1,-1)").arg(packingBackImportPlcId		).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('packing back exit 2'   ,%1,0,0,%2,-1,-1,-1)").arg(packingBackExport2PlcId	).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		int cell=0;
		for (int col=1;col<=60;col++)
		{
			for (int row=1;row<=13;row++)
			{
				cell++;
				int acceptsLengthType=LEFT_AND_CENTER_PACKET_PLACING_CASSETTE;
				if (( (col==15)||(col==16)||(col==29)||(col==30)||(col==43)||(col==44)||(col==57)||(col==58) ) ||(row<=4))
					acceptsLengthType=CENTER_ONLY_PACKET_PLACING_CASSETTE;
				int storagingType=STORAGE_TYPE_SHORT_TERM;
				if ((col>=21)&&(col<=40))
					storagingType=STORAGE_TYPE_MID_TERM;
				if ((col>=41)&&(col<=60))
					storagingType=STORAGE_TYPE_LONG_TERM;
				//west
				apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,storagingType,col,row) values('w %1',%2,0,%3,%4,%5,%6,%7,%8)").arg(-cell).arg(-cell).arg(-cell).arg(acceptsLengthType)     .arg(IS_NOT_LOCKED).arg(storagingType).arg(col).arg(row));
				//east
				apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,storagingType,col,row) values('w %1',%2,0,%3,%4,%5,%6,%7,%8)").arg( cell).arg( cell).arg( cell).arg(CENTER_ONLY_PACKET_PLACING_CASSETTE).arg(IS_NOT_LOCKED).arg(storagingType).arg(col).arg(row));
			}
		}
	}
	if (GETONE(QString("select count(tid) from positions where posId=%1").arg(stationBackExport2PlcId)).toInt()==0)
	{
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values(1576,'station exit 1'			,%1,0,0,%2,-1,-1,-1)").arg(stationExport1PlcId			).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values(1577,'station import'			,%1,0,0,%2,-1,-1,-1)").arg(stationImportPlcId				).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values(1578,'station exit 2'			,%1,0,0,%2,-1,-1,-1)").arg(stationExport2PlcId			).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values(1579,'station exit b 1'   ,%1,0,0,%2,-1,-1,-1)").arg(stationBackExport1PlcId	).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values(1580,'station import b'   ,%1,0,0,%2,-1,-1,-1)").arg(stationBackImportPlcId		).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values(1581,'station back b 2'   ,%1,0,0,%2,-1,-1,-1)").arg(stationBackExport2PlcId	).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
	}
	if (GETONE(QString("select count(tid) from positions where posId=%1").arg(packingExport1PlcId)).toInt()==0)
	{
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,acceptableLength,isLocked,col,row) values(1576,'packing exit 1'		,%1,0,0,%2,7660,-1,-1,-1)").arg(packingExport1PlcId		).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,acceptableLength,isLocked,col,row) values(1577,'packing import'		,%1,0,0,%2,7660,-1,-1,-1)").arg(packingImportPlcId			).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,acceptableLength,isLocked,col,row) values(1578,'packing exit 2'		,%1,0,0,%2,7660,-1,-1,-1)").arg(packingExport2PlcId		).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,acceptableLength,isLocked,col,row) values(1579,'packing exit b 1'  ,%1,0,0,%2,7660,-1,-1,-1)").arg(packingBackExport1PlcId).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,acceptableLength,isLocked,col,row) values(1580,'packing import b'  ,%1,0,0,%2,7660,-1,-1,-1)").arg(packingBackImportPlcId	).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,acceptableLength,isLocked,col,row) values(1581,'packing back b 2'  ,%1,0,0,%2,7660,-1,-1,-1)").arg(packingBackExport2PlcId).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
	}
	if (GETONE(QString("select count(tid) from positions where posId=%1").arg(packingOldStrappingExitPosId)).toInt()==0)
	{
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,acceptableLength,isLocked,storagingType,col,row) values(1582,'Old Strapping',%1,0,0,%2,7660,0,1,-1,-1)").arg(packingOldStrappingExitPosId).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,acceptableLength,isLocked,storagingType,col,row) values(1583,'New Strapping',%1,0,0,%2,7660,0,1,-1,-1)").arg(packingNewStrappingExitPosId).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
	}
	if (GETONE(QString("select count(tid) from positions where posId=%1").arg(positionInputOutputPosId)).toInt()==0)
	{
		apexExecQuery(QString("insert into positions (tid,posName,posId,packetId,cassette,acceptableContents,acceptableLength,isLocked,storagingType,col,row) values(1582,'Input/Output',%1,0,0,%2,7660,0,1,-1,-1)").arg(positionInputOutputPosId).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
	}
	if (GETONE("select count(tid) from cassettes").toInt()==0)
	{
		for (int cas=1;cas<=780;cas++)
		{
			//west
			apexExecQuery(QString("insert into cassettes (cassette,status) values(%1,%2)").arg(-cas).arg(CASSETTE_IDLE));
			//east
			apexExecQuery(QString("insert into cassettes (cassette,status) values(%1,%2)").arg( cas).arg(CASSETTE_IDLE));
		}
	}
	if(GETONE("select count(name) from state").toInt()==0)
	{
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("forkFromPos").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("forkToPos").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("craneFromPos").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("craneToPos").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("westConveyorActivePacketExport").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("westConveyorActivePacketImport").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("westConveyorActiveCassetteImport").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("eastConveyorActivePacketExport").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("eastConveyorActivePacketImport").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("eastConveyorActiveCassetteImport").arg("0"));
	}
}
QString databaseServerIp;
mainWindowClass::mainWindowClass()
{
	splash->showMessage(tr("Starting..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	//QSound(QDir::current().absolutePath()+"//failed.wav").play();
	setWindowIcon(QIcon(":/assimakis.png"));
	cranePlcConnected=forkPlcConnected=false;
	///////////////////////////dataBase
	splash->showMessage(tr("Connecting to database..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
#ifdef inMyLaptop
	databaseServerIp="127.0.0.1";
	QString databaseName="wareHouseData";
	QString port="1433";
	QString user="sa";
	QString password=QString::fromLocal8Bit(qgetenv("ASSIMAKIS_PACKETCRANE_PASSWORD"));
	QString connectString="Driver={SQL Server};";
	connectString.append(QString("Server=%1,%2;").arg(databaseServerIp).arg(port));
	connectString.append(QString("Uid=%1;").arg(user));
	connectString.append(QString("Pwd=%1;").arg(password));
	mainDb.setDatabaseName(connectString);
#else
	databaseServerIp="apexPacketCraneMainOdbc";
	mainDb.setDatabaseName("apexPacketCraneMainOdbc");
	mainDb.setUserName("sa");
	mainDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_PACKETCRANE_MAINDB_PASSWORD")));
#endif
	if (!mainDb.open())
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(mainDb.databaseName()).arg(mainDb.lastError().text()));

	QSqlDatabase epicsDb=QSqlDatabase::addDatabase("QODBC",eDb);
	epicsDb.setDatabaseName("epicsConnection");
	epicsDb.setUserName("BasketCrane2");
	epicsDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_PACKETCRANE_EPICSDB_PASSWORD")));
	if (!epicsDb.open())
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(epicsDb.databaseName()).arg(epicsDb.lastError().text()));

	//QVector<QVector<QVariant>> aVV=execTableQuery(QString("[EDW].[dbo].[getLWHWoodsByTicket]  @ticket = 103763"),eDb);
	//QVector<QVector<QVariant>> bVV=execTableQuery(QString("select packetId,length,width,height,woods from edw.dbo.packets where packetId=103763;"),eDb);
//
//	declare @return_value int
//
//EXEC @return_value = [EDW].[dbo].[getLWHWoodsByTicket]  @ticket = 103763


	apexExecQuery("if not exists(select * from sys.databases where name='wareHouseData') create database wareHouseData");
	apexExecQuery("use wareHouseData",MAIN_DB_CONNECTION);
	QStringList colNames;QStringList colTypes;
	//positions Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"posName " <<"posId"<<"packetId"<<"cassette"<<"acceptableContents"<<"isLocked"<<"storagingType"<<"col"<<"row"<<"acceptableLength"<<"xCorr"<<"zCorr";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"char(30)" <<"int"	 <<"int"	   <<"int"	   <<"int"							 <<"int"	   <<"int"					<<"int"<<"int"<<						 "int"<<  "int"<<  "int";
	createTable("positions",colNames,colTypes);
	//state Table
	colNames.clear();colTypes.clear();
	colNames<<"name"    <<"value"   ;
	colTypes<<"char(50)"<<"char(15)";
	createTable("state",colNames,colTypes);
	//cassettes Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"cassette"<<"status";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"int"	   <<"int"	 ;
	createTable("cassettes",colNames,colTypes);
	initializeTables();
	//packets Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"packetId"<<"woods"<<"length"<<"width"<<"height"<<"weight"<<"sb" <<"destination"<<"storagingType";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"int"	   <<"int"	 <<"int"	 <<"int"	<<"int"	  <<"real"	<<"int"<<"int"        <<"int"					 ;
	createTable("packets",colNames,colTypes);
	//exports Table
	colNames.clear();colTypes.clear();			//packet/sb no  'sb'/'packet'   0/1
	colNames<<"tid"											 <<"contents "<<"type"	<<"status"<<"orderSequence"<<"contentsSequence";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"int"			<<"int"		<<"int"   <<"int"	         <<"int"        ;
	createTable("exports",colNames,colTypes);
	//exportsStation1 Table
	colNames.clear();colTypes.clear();			//packet/sb no  'sb'/'packet'   0/1
	colNames<<"tid"											 <<"contents "<<"type"	<<"status"<<"orderSequence"<<"contentsSequence";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"int"			<<"int"		<<"int"   <<"int"	         <<"int"        ;
	createTable("exportsStation1",colNames,colTypes);
	//exportsStation2 Table
	colNames.clear();colTypes.clear();			//packet/sb no  'sb'/'packet'   0/1
	colNames<<"tid"											 <<"contents "<<"type"	<<"status"<<"orderSequence"<<"contentsSequence";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"int"			<<"int"		<<"int"   <<"int"	         <<"int"        ;
	createTable("exportsStation2",colNames,colTypes);
	//exportsWest Table
	colNames.clear();colTypes.clear();			//packet/sb no  'sb'/'packet'   0/1
	colNames<<"tid"											 <<"contents "<<"type"	<<"status"<<"orderSequence"<<"contentsSequence";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"int"			<<"int"		<<"int"   <<"int"	         <<"int"        ;
	createTable("exportsPacking",colNames,colTypes);
	//log Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"type"     <<"status"  <<"fromPosId"<<"toPosId"<<"posId"<<"newValue"<<"oldValue"<<"date"	   <<"sec";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"char(10)"<<"int"      <<"int"    <<"int"	 <<"int"	   <<"int"	   <<"char(20)"<<"int";
	createTable("log",colNames,colTypes);
	apexExecQuery(QString("delete from log where tid not in(select top 100000 tid from log order by sec desc)"));
	//logCranePlc Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"address"  <<"value"<<"X"     <<"Y"     <<"Z"     <<"date"	   <<"sec";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"int"  <<"float" <<"float" <<"float" <<"char(20)"<<"int";
	createTable("logCranePlc",colNames,colTypes);
	apexExecQuery(QString("delete from logCranePlc where tid not in(select top 50000 tid from logCranePlc order by sec desc)"));
	//logForkPlc Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"address"  <<"value"<<"X"     <<"Y"     <<"Z"     <<"date"	   <<"sec";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"int"  <<"float" <<"float" <<"float" <<"char(20)"<<"int";
	createTable("logForkPlc",colNames,colTypes);
	apexExecQuery(QString("delete from logForkPlc where tid not in(select top 50000 tid from logForkPlc order by sec desc)"));
	//logResetPlc Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"type"	    <<"value"<<"date"	   <<"sec";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"float"<<"char(20)"<<"int";
	createTable("logResetPlc",colNames,colTypes);
	apexExecQuery(QString("delete from logResetPlc where tid not in(select top 50000 tid from logResetPlc order by sec desc)"));
	//logErrorPlc Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"type"	    <<"bit"     <<"description"<<"date"	   <<"sec";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"char(10)"<<"char(100)"  <<"char(20)"<<"int";
	createTable("logErrorPlc",colNames,colTypes);
	apexExecQuery(QString("delete from logErrorPlc where tid not in(select top 10000 tid from logErrorPlc order by sec desc)"));
	//vLog Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"type"	    <<"title"	   <<"description"<<"date"	   ;
	colTypes<<"int PRIMARY KEY IDENTITY" <<"char(50)" <<"char(50)" <<"char(150)"  <<"char(20)";
	createTable("vLog",colNames,colTypes);
	apexExecQuery(QString("delete from vLog where tid not in(select top 10000 tid from vLog order by tid desc)"));
//users Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"name"	    <<"pass"    ;
	colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"char(10)";
	createTable("users",colNames,colTypes);
	apexExecQuery("if not exists (select name from users where name='admin') insert into users (name,pass) values('admin','123')");
	if (false)
	{
		for (int r=1;r<=13;r++)
		{
			for (int c=1;c<=60;c++)
			{
				if ((r<=3)||(c==15)||(c==29)||(c==30)||(c==44)||(c==58))
				{
					apexExecQuery(QString("update positions set acceptableLength=%1 where row=%2 and col=%3 and posId<0").arg(smallLength).arg(r).arg(c));
					apexExecQuery(QString("update positions set acceptableLength=%1 where row=%2 and col=%3 and posId>0").arg(smallLength).arg(r).arg(c));
					apexExecQuery(QString("update positions set acceptableContents=%1 where row=%2 and col=%3 and posId>0").arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE).arg(r).arg(c));
					apexExecQuery(QString("update positions set acceptableContents=%1 where row=%2 and col=%3 and posId<0").arg(CENTER_ONLY_PACKET_PLACING_CASSETTE).arg(r).arg(c));
				}
				else
				{
					apexExecQuery(QString("update positions set acceptableLength=%1 where row=%2 and col=%3").arg(bigLength).arg(r).arg(c));
					apexExecQuery(QString("update positions set acceptableContents=%1 where row=%2 and col=%3").arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE).arg(r).arg(c));
				}
			}
		}
	}
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();
	QHBoxLayout *upLayout=new QHBoxLayout();mainLayout->addLayout(upLayout);
	setLayout(mainLayout);
	allWidget=new allWidgetClass(this);
	mainLayout->addWidget(allWidget);
	splash->showMessage(tr("Creating Manual Movements..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	manualCassetteMovement=new manualCassetteMovementClass(allWidget->getCraneClass());
	splash->showMessage(tr("Creating Manual Missions..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	manualMissions=new manualMissionsClass(allWidget->getCraneClass(),allWidget->getForkClass(),allWidget->getAllPosClass(),this);
	//
	splash->showMessage(tr("Connecting to plc..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	plc=new plcClass(packetCraneIp,forkIp,crane3Ip,false,this);

	allWidget->setPlcClass(plc);
	connect(plc->getCraneTuxClass(),SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromCraneTuxPlcSlot(QString,QString,int,QVariant)));
	connect(plc->getForkTuxClass(),SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromForkTuxPlcSlot(QString,QString,int,QVariant)));
	connect(plc->getCrane3TuxClass(),SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromCrane3TuxPlcSlot(QString,QString,int,QVariant)));
	//
	splash->showMessage(tr("Creating gui..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	autoMissionsExportsCheckBox=new QCheckBox(tr("Exports"));upLayout->addWidget(autoMissionsExportsCheckBox);
	connect(autoMissionsExportsCheckBox,SIGNAL(stateChanged(int)),this,SLOT(autoMissionsExportsCheckBoxStateChangedSlot(int)));
	autoMissionsImportsCheckBox=new QCheckBox(tr("Imports"));upLayout->addWidget(autoMissionsImportsCheckBox);
	connect(autoMissionsImportsCheckBox,SIGNAL(stateChanged(int)),this,SLOT(autoMissionsImportsCheckBoxStateChangedSlot(int)));
	//autoMissionsExportsCheckBox->setEnabled(false);
	//autoMissionsImportsCheckBox->setEnabled(false);

	autoMissionsExportsStation1CheckBox=new QCheckBox(tr("Exports station 1"));upLayout->addWidget(autoMissionsExportsStation1CheckBox);
	connect(autoMissionsExportsStation1CheckBox,SIGNAL(stateChanged(int)),this,SLOT(autoMissionsExportsStation1CheckBoxStateChangedSlot(int)));
	autoMissionsExportsStation2CheckBox=new QCheckBox(tr("Exports station 2"));upLayout->addWidget(autoMissionsExportsStation2CheckBox);
	connect(autoMissionsExportsStation2CheckBox,SIGNAL(stateChanged(int)),this,SLOT(autoMissionsExportsStation2CheckBoxStateChangedSlot(int)));
	autoMissionsImportsStationCheckBox=new QCheckBox(tr("Imports station"));upLayout->addWidget(autoMissionsImportsStationCheckBox);
	connect(autoMissionsImportsStationCheckBox,SIGNAL(stateChanged(int)),this,SLOT(autoMissionsImportsStationCheckBoxStateChangedSlot(int)));

	autoMissionsPackingStationCheckBox=new QCheckBox(tr("Packing"));upLayout->addWidget(autoMissionsPackingStationCheckBox);
	connect(autoMissionsPackingStationCheckBox,SIGNAL(stateChanged(int)),this,SLOT(autoMissionsPackingStationCheckBoxStateChangedSlot(int)));
	//autoMissionsPackingStationCheckBox->setEnabled(false);

	definePacketInIOutStationBtn=new QPushButton(QIcon(":/picking.png"),tr("Define"),this);upLayout->addWidget(definePacketInIOutStationBtn);
	definePacketInIOutStationBtn->setToolTip(tr("Define Packet in In/Out Station"));
	connect(definePacketInIOutStationBtn,SIGNAL(clicked()),this,SLOT(definePacketInIOutStationBtnClickedSlot()));

	manualMissionsBtn=new QPushButton(tr("State"),this);upLayout->addWidget(manualMissionsBtn);
	connect(manualMissionsBtn,SIGNAL(clicked()),this,SLOT(manualMissionsBtnClickedSlot()));

	plcPushButton=new QPushButton(tr("HMI"),this);upLayout->addWidget(plcPushButton);//plcPushButton->setGeometry(10,10,100,20);
	connect(plcPushButton,SIGNAL(clicked()),this,SLOT(plcButtonClickedSlot()));
	QPushButton *forkPackingButton=new QPushButton(tr("Packing Fork"),this);upLayout->addWidget(forkPackingButton);//exportsButton->setGeometry(10,40,100,20);
	connect(forkPackingButton,SIGNAL(clicked()),this,SLOT(forkPackingButtonClickedSlot()));
	//exportsClass
	exportsDlg=new exportsDlgClass(this);
	QPushButton *exportsButton=new QPushButton(tr("Exports"),this);upLayout->addWidget(exportsButton);//exportsButton->setGeometry(10,40,100,20);
	connect(exportsButton,SIGNAL(clicked()),this,SLOT(exportsButtonClickedSlot()));
	//plcCommsLabel
	cranePlcCommsLabel=new QLabel("",this);upLayout->addWidget(cranePlcCommsLabel);//cranePlcCommsLabel->setGeometry(10,70,200,20);
	forkPlcCommsLabel=new QLabel("",this);upLayout->addWidget(forkPlcCommsLabel);//forkPlcCommsLabel->setGeometry(10,100,200,20);
	//
	exportsTo23Radio=new QRadioButton(tr("Exports to 23"));upLayout->addWidget(exportsTo23Radio);connect(exportsTo23Radio,SIGNAL(toggled(bool)),this,SLOT(exportsTo23RadioToggledSlot(bool)));
	// exportsTo22Radio=new QRadioButton(tr("Exports to 22"));upLayout->addWidget(exportsTo22Radio);connect(exportsTo22Radio,SIGNAL(toggled(bool)),this,SLOT(exportsTo22RadioToggledSlot(bool)));
	exportsTo22Radio=new QRadioButton(tr("Exports to 9"));upLayout->addWidget(exportsTo22Radio);connect(exportsTo22Radio,SIGNAL(toggled(bool)),this,SLOT(exportsTo22RadioToggledSlot(bool)));
	exportsTo21Radio=new QRadioButton(tr("Exports to 21"));upLayout->addWidget(exportsTo21Radio);connect(exportsTo21Radio,SIGNAL(toggled(bool)),this,SLOT(exportsTo21RadioToggledSlot(bool)));
	exportsTo23Radio->setEnabled(false);
	//exportsTo22Radio->setEnabled(false);
	exportsTo21Radio->setEnabled(false);
	//
	userBackupPushButton=new QPushButton(tr("backup database"),this);upLayout->addWidget(userBackupPushButton);//userBackupPushButton->setGeometry(10,130,100,20);
	connect(userBackupPushButton,SIGNAL(clicked()),this,SLOT(onUserBackupSlot()));

	type1EmptyCassettesLabel=new QLabel();upLayout->addWidget(type1EmptyCassettesLabel);type1EmptyCassettesLabel->setStyleSheet(QString("QLabel{font-size:14px;background-color:#00aaaa;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}"));
	type2EmptyCassettesLabel=new QLabel();upLayout->addWidget(type2EmptyCassettesLabel);type2EmptyCassettesLabel->setStyleSheet(QString("QLabel{font-size:14px;background-color:#0000aa;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}"));
	type3EmptyCassettesLabel=new QLabel();upLayout->addWidget(type3EmptyCassettesLabel);type3EmptyCassettesLabel->setStyleSheet(QString("QLabel{font-size:14px;background-color:#00aa00;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}"));
	//
	plc->setManualCassetteMovementClass(manualCassetteMovement);
	upLayout->addStretch(1);
	//
	//plc->getPacketCraneTcpServer()->sendCurrentValuesToPlc();
	//plc->getForkLiftTcpServer()->sendCurrentValuesToPlc();
	allWidget->startTimers();
	lastCranePlcHandShakeValue=lastForkPlcHandShakeValue=0;
	connect(&commsTimer,SIGNAL(timeout()),this,SLOT(onCommsTimerSlot()));
	commsTimer.start(5000);
	connect(&backupTimer,SIGNAL(timeout()),this,SLOT(onBackupTimerSlot()));
	onBackupTimerSlot();
	backupTimer.start(1*60*60*1000);
	//backupTimer.start(60*1000);
	QAction *showServerAction=new QAction("",this);
	addAction(showServerAction);
	showServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	connect(showServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));

	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);

	plc->setIsAutoUpdatingFromPlc(true);
	vLog("info","mainWindowClass::mainWindowClass():","programm started");
	exportsTo22Radio->setChecked(true);  // 22 == 9 (20190227)

	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);

#ifdef tempForMove
	QAction *tempForMoveAction=new QAction(this);
	tempForMoveAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_T));
	connect(tempForMoveAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(tempForMoveAction);
#endif
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimer()));
	mainTimer.start(1000);

	QString portStr="COM3";
	packetInInOutStationSerialPort=new QSerialPort(this);
	packetInInOutStationSerialPort->setPortName(portStr);
	packetInInOutStationSerialPort->setBaudRate(QSerialPort::Baud9600);
	packetInInOutStationSerialPort->setFlowControl(QSerialPort::NoFlowControl);
	packetInInOutStationSerialPort->setParity(QSerialPort::NoParity);
	packetInInOutStationSerialPort->setDataBits(QSerialPort::Data8);
	packetInInOutStationSerialPort->setStopBits(QSerialPort::OneStop);
	if (!packetInInOutStationSerialPort->open(QIODevice::ReadOnly))
		QMessageBox::critical(this, tr("Error"),tr("Error opening serial port %1.\nError:%2").arg(portStr).arg(packetInInOutStationSerialPort->errorString()));
	connect(packetInInOutStationSerialPort,SIGNAL(readyRead()),this,SLOT(readyReadPacketInInOutStationSerialPortSlot()));
}
mainWindowClass::~mainWindowClass()
{
	onBackupTimerSlot();
}
void mainWindowClass::onCommsTimerSlot()
{
	if (lastCranePlcHandShakeValue!=plc->getLastCranePlcHandShakeValue())
	{
		lastCranePlcHandShakeValue=plc->getLastCranePlcHandShakeValue();
		cranePlcCommsLabel->setText(QString("crane comms ok %1").arg(lastCranePlcHandShakeValue));
		cranePlcCommsLabel->setStyleSheet("QLabel{font-size:14px;color:#ffffff;background-color:#00ff00;border-radius:7px;border-color:beige;padding-left:5px;}");
	}
	else
	{
//		QSound(QDir::current().absolutePath()+"//failed.wav").play();
		cranePlcCommsLabel->setText(QString("crane comms error %1").arg(lastCranePlcHandShakeValue));
		cranePlcCommsLabel->setStyleSheet("QLabel{font-size:14px;color:#ffffff;background-color:#ff0000;border-radius:7px;border-color:beige;padding-left:5px;}");
	}
	if (lastForkPlcHandShakeValue!=plc->getLastForkPlcHandShakeValue())
	{
		lastForkPlcHandShakeValue=plc->getLastForkPlcHandShakeValue();
		forkPlcCommsLabel->setText(QString("fork comms ok %1").arg(lastForkPlcHandShakeValue));
		forkPlcCommsLabel->setStyleSheet("QLabel{font-size:14px;color:#ffffff;background-color:#00ff00;border-radius:7px;border-color:beige;padding-left:5px;}");
	}
	else
	{
//		QSound(QDir::current().absolutePath()+"//failed.wav").play();
		forkPlcCommsLabel->setText(QString("fork comms error %1").arg(lastForkPlcHandShakeValue));
		forkPlcCommsLabel->setStyleSheet("QLabel{font-size:14px;color:#ffffff;background-color:#ff0000;border-radius:7px;border-color:beige;padding-left:5px;}");
	}
}
void mainWindowClass::onUserBackupSlot()
{
	QString cmd;
	QString backupDir=QDir::current().absolutePath()+"/";backupDir=backupDir.replace("/","\\");
	QString backupFile=QString("packetCraneUserBackup_")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+"_"+QTime::currentTime().toString("hh.mm.ss")+".bak";
	QString backupFileAbsPath=backupDir+backupFile;
	cmd=QString("sqlcmd -S .\\SQLExpress -Q \"BACKUP DATABASE wareHouseData TO DISK='"+backupFileAbsPath+"' WITH INIT\"");
	QProcess *backupProcess=new QProcess(this);
	backupProcess->start(cmd);
	QMessageBox::information(this,"information",tr("Backup created in file : \n%1").arg(backupFileAbsPath));
}
void deleteOldBackups(int backupsToKeep)
{
	QDir bakFilesDir=QDir::current().absolutePath()+"/backup/";
	QStringList filelist=bakFilesDir.entryList(QStringList()<<"packetCraneDBBackup_*.bak",QDir::Files,QDir::Name|QDir::Reversed);
	for (int i=(backupsToKeep-1);i<filelist.count();i++)
	{
		bakFilesDir.remove(filelist[i]);
	}
}
void mainWindowClass::onBackupTimerSlot()
{
//#ifndef _DEBUG
	deleteOldBackups(25);
	QString cmd;
	QString backupDir=QDir::current().absolutePath()+"/backup/";backupDir=backupDir.replace("/","\\");
	QString backupFile=QString("packetCraneDBBackup_")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+"_"+QTime::currentTime().toString("hh.mm.ss")+".bak";
	QString backupFileAbsPath=backupDir+backupFile;
	cmd=QString("sqlcmd -S .\\SQLExpress -Q \"BACKUP DATABASE wareHouseData TO DISK='"+backupFileAbsPath+"' WITH INIT\"");
	QProcess *backupProcess=new QProcess(this);
	backupProcess->start(cmd);
	backupProcess->waitForFinished();
	backupProcess->close();
//#endif
}
void mainWindowClass::exportsButtonClickedSlot()
{
	exportsDlg->exec();
}
void mainWindowClass::plcButtonClickedSlot()
{
#ifndef _DEBUG
	passwordClass *pass=new passwordClass(this);
	pass->exec();
	if (pass->getPasswordIsCorrect())
		plc->show();
	delete pass;
#else
	plc->show();
#endif
}
#ifdef tempForMove
int fropPos=-1;
int toPos=-1;
QVector<int> globalPosToPush;
bool first=true;
#endif
void mainWindowClass::onMainTimer()
{
#ifndef tempForMove
	type1EmptyCassettesLabel->setText(tr("%1").arg(allWidget->getType1EmptyCassettes()));
	type2EmptyCassettesLabel->setText(tr("%1").arg(allWidget->getType2EmptyCassettes()));
	type3EmptyCassettesLabel->setText(tr("%1").arg(allWidget->getType3EmptyCassettes()));
	setWindowTitle(tr("Packet crane, assimakis sa %1:%2","main window title..").arg(timeVerbose).arg(databaseServerIp));
#endif
#ifdef tempForMove
	QString t;
	for (int i=0;i<globalPosToPush.count();i++)
		t+=tr("  %1  ").arg(globalPosToPush[i]);
	setWindowTitle(t+QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss"));
		if ((fropPos==0)&&(toPos==0))
		{
			if (globalPosToPush.count()>0)
			{
				int pos=globalPosToPush.first();
				plc->getCraneTuxClass()->writeInteger("N150",0,pos);
				plc->getCraneTuxClass()->writeInteger("N150",1,pos);
				globalPosToPush.removeAt(0);
			}
		}
#endif
}
void mainWindowClass::tempForMoveSlot()
{
}
void mainWindowClass::signalFromCraneTuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
//	if (!plc->getTuxServer()->getHasError())
	signalFromPacketCranePlcSlot(table,index,value.toString());
	plc->dataInPlcChanged(ip,table,index,value);
}

void mainWindowClass::signalFromPacketCranePlcSlot(QString table,int index,QString value)
{
	allWidget->signalFromCranePlc(table,index,value);
	if (table=="N150")
	{
		plc->packetCranePlcIntMemory150Changed(index,value.toInt());
#ifdef tempForMove
		if (index==0)
			fropPos=value.toInt();
		if (index==1)
			toPos=value.toInt();
#endif
	}
	if (table=="N152")
	{
		allWidget->signalFromCranePlc(table,index,value);
		plc->packetCranePlcIntMemory152Changed(index,value.toInt());
	}
	if (table=="F151")
	{
		allWidget->signalFromCranePlc(table,index,value);
		plc->packetCranePlcFloatMemoryChanged(index,value.toFloat());
		if (index==14)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('craneX',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		if (index==15)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('craneZ',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		if (index==16)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('craneY',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	}
}
void mainWindowClass::signalFromForkTuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
	if (!plc->getTuxServer()->getHasError())
		signalFromForkLiftPlcSlot(table,index,value.toString());
	plc->dataInPlcChanged(ip,table,index,value);
}
void mainWindowClass::signalFromForkLiftPlcSlot(QString table,int index,QString value)
{
	if (table=="N160")
	{
		allWidget->signalFromForkPlc(table,index,value);
		plc->forkLiftPlcIntMemoryChanged(index,value.toInt());
	}
	if (table=="F161")
	{
		allWidget->signalFromForkPlc(table,index,value);
		plc->forkLiftPlcFloatMemoryChanged(index,value.toFloat());
	}
	if (table=="F162")
	{
		if (index==0)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('forkX',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		if (index==1)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('forkZ',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		if (index==2)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('forkY',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	}
}
void mainWindowClass::signalFromCrane3TuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
	allWidget->signalFromCrane3Plc(table,index,value);
	if ((ip==crane3Ip)&&(table=="fc1ToPcI")&&(index==11))
		definePacketInIOutStationBtn->setStyleSheet(getValueOfBit(value.toInt(),0)?buttonStyleActive:buttonStyleOrange);
}
void mainWindowClass::showServerSlot()
{
	plc->showTuxServerCommsDialog();
}
void mainWindowClass::stopSlot()
{
	plc->getTuxServer()->getTuxipClassWithIp(packetCraneIp)->writeInteger("N150",20,166);
	plc->getTuxServer()->getTuxipClassWithIp(packetCraneIp)->writeInteger("N150",20,0);
}
void mainWindowClass::exportsTo23RadioToggledSlot(bool checked)
{
#ifdef keepPacketCraneOldFunctionality
	if (checked)
	{
		exportsTo21Radio->setChecked(false);
		exportsTo22Radio->setChecked(false);
	}
	allWidget->setExportsDestination(managerClass::exportsDestinationEnum::exportsTo23);
#endif
}
void mainWindowClass::exportsTo22RadioToggledSlot(bool checked)
{
#ifdef keepPacketCraneOldFunctionality
	// 22 == 9 (20190227) RAM
	if (checked)
	{
		exportsTo21Radio->setChecked(false);
		exportsTo23Radio->setChecked(false);
	}
	allWidget->setExportsDestination(managerClass::exportsDestinationEnum::exportsTo22);
#endif
}
void mainWindowClass::exportsTo21RadioToggledSlot(bool checked)
{
#ifdef keepPacketCraneOldFunctionality
	if (checked)
	{
		exportsTo22Radio->setChecked(false);
		exportsTo23Radio->setChecked(false);
	}
	allWidget->setExportsDestination(managerClass::exportsDestinationEnum::exportsTo21);
#endif
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}
void mainWindowClass::autoMissionsExportsCheckBoxStateChangedSlot(int state)
{
#ifdef keepPacketCraneOldFunctionality
	bool hasAutoChecked=autoMissionsExportsCheckBox->isChecked()||autoMissionsImportsCheckBox->isChecked()||autoMissionsExportsStation1CheckBox->isChecked()||autoMissionsExportsStation2CheckBox->isChecked()||autoMissionsImportsStationCheckBox->isChecked()||autoMissionsPackingStationCheckBox->isChecked();
	allWidget->enableAutoExportsMissions(autoMissionsExportsCheckBox->isChecked());
	manualMissionsBtn->setEnabled(!hasAutoChecked);
#endif
}
void mainWindowClass::autoMissionsImportsCheckBoxStateChangedSlot(int state)
{
	bool hasAutoChecked=autoMissionsExportsCheckBox->isChecked()||autoMissionsImportsCheckBox->isChecked()||autoMissionsExportsStation1CheckBox->isChecked()||autoMissionsExportsStation2CheckBox->isChecked()||autoMissionsImportsStationCheckBox->isChecked()||autoMissionsPackingStationCheckBox->isChecked();
	allWidget->enableAutoImportsMissions(autoMissionsImportsCheckBox->isChecked());
	manualMissionsBtn->setEnabled(!hasAutoChecked);
}
void mainWindowClass::autoMissionsExportsStation1CheckBoxStateChangedSlot(int state)
{
	bool hasAutoChecked=autoMissionsExportsCheckBox->isChecked()||autoMissionsImportsCheckBox->isChecked()||autoMissionsExportsStation1CheckBox->isChecked()||autoMissionsExportsStation2CheckBox->isChecked()||autoMissionsImportsStationCheckBox->isChecked()||autoMissionsPackingStationCheckBox->isChecked();
	allWidget->enableAutoExportsStation1Missions(autoMissionsExportsStation1CheckBox->isChecked());
	manualMissionsBtn->setEnabled(!hasAutoChecked);
}
void mainWindowClass::autoMissionsExportsStation2CheckBoxStateChangedSlot(int state)
{
	bool hasAutoChecked=autoMissionsExportsCheckBox->isChecked()||autoMissionsImportsCheckBox->isChecked()||autoMissionsExportsStation1CheckBox->isChecked()||autoMissionsExportsStation2CheckBox->isChecked()||autoMissionsImportsStationCheckBox->isChecked()||autoMissionsPackingStationCheckBox->isChecked();
	allWidget->enableAutoExportsStation2Missions(autoMissionsExportsStation2CheckBox->isChecked());
	manualMissionsBtn->setEnabled(!hasAutoChecked);
}
void mainWindowClass::autoMissionsImportsStationCheckBoxStateChangedSlot(int state)
{
	bool hasAutoChecked=autoMissionsExportsCheckBox->isChecked()||autoMissionsImportsCheckBox->isChecked()||autoMissionsExportsStation1CheckBox->isChecked()||autoMissionsExportsStation2CheckBox->isChecked()||autoMissionsImportsStationCheckBox->isChecked()||autoMissionsPackingStationCheckBox->isChecked();
	allWidget->enableAutoImportsStationMissions(autoMissionsImportsStationCheckBox->isChecked());
	manualMissionsBtn->setEnabled(!hasAutoChecked);
}
void mainWindowClass::autoMissionsPackingStationCheckBoxStateChangedSlot(int state)
{
	bool hasAutoChecked=autoMissionsExportsCheckBox->isChecked()||autoMissionsImportsCheckBox->isChecked()||autoMissionsExportsStation1CheckBox->isChecked()||autoMissionsExportsStation2CheckBox->isChecked()||autoMissionsImportsStationCheckBox->isChecked()||autoMissionsPackingStationCheckBox->isChecked();
	allWidget->enableAutoWestStationMissions(autoMissionsPackingStationCheckBox->isChecked());
	manualMissionsBtn->setEnabled(!hasAutoChecked);
}
void mainWindowClass::readyReadPacketInInOutStationSerialPortSlot()
{
	QString str=packetInInOutStationSerialPort->readAll();
	packetInInOutStationSerialPortTicket=str.toInt();
}
void mainWindowClass::definePacketInIOutStationBtnClickedSlot()
{
	posDlgPackingForkClass *posDlg=new posDlgPackingForkClass(tr("Define Packet"),0,true,true,this);
	int packetId=getFirst(QString("select packetId from positions where posId=%1").arg(positionInputOutputPosId),MAIN_DB_CONNECTION).toInt();
	if (packetInInOutStationSerialPortTicket)
		packetId=packetInInOutStationSerialPortTicket;
	bool isPacketId=(packetId>minTicketIdId)&&(packetId<maxTicketIdId);
	QVector<QVector<QVariant>> dataVV;
	if (packetId&&isPacketId)
	{
		if (packetInInOutStationSerialPortTicket)
		{
			//dataVV=execTableQuery(QString("select packetId,length,width,height,woods from edw.dbo.packets where packetId=%1").arg(packetId),eDb);
			QVector<QVector<QVariant>> tempVV=execTableQuery(QString("EXEC [EPICS].[dbo].[uspa_getLWHWoodsByTicket] @ticket=%1").arg(packetId),eDb);
			dataVV<<(QVector<QVariant>()<<tempVV[0][1]<<tempVV[0][2]<<tempVV[0][3]<<tempVV[0][4]<<tempVV[0][5]);
		}
		else
			dataVV=execTableQuery(QString("select length,woods,width,height,weight from packets where packetId=%1").arg(packetId),pDb);
	}
	packetInInOutStationSerialPortTicket=0;
	if (dataVV.count())
		posDlg->setData(packetId,dataVV[0][0].toInt(),dataVV[0][1].toInt(),dataVV[0][2].toInt(),dataVV[0][3].toInt(),dataVV[0][4].toInt());
	else
		posDlg->setData(packetId,0,0,0,0,0);
	if (posDlg->exec()==QDialog::Accepted)
	{
		plc->getCrane3TuxClass()->writeInteger("packetId",inputOutputPlcId,posDlg->getPacketTicket());
		plc->getCrane3TuxClass()->writeInteger("packetLength",inputOutputPlcId,posDlg->getPacketLength());
		plc->getCrane3TuxClass()->writeInteger("packetWoods",inputOutputPlcId,posDlg->getPacketWoods());
	}
}
void mainWindowClass::manualMissionsBtnClickedSlot()
{
	passwordClass *pass=new passwordClass(this);
	pass->exec();
	if (pass->getPasswordIsCorrect())
		manualMissions->exec();
}
void mainWindowClass::forkPackingButtonClickedSlot()
{
	allWidget->showForkPackingWdgt();
}

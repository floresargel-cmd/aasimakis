#include "pch.h"
#define inMyLaptop
#define doNotTakeBackup
#include "mW.h"
extern QSplashScreen *splash;
allPossClass *globalAllPosPtr;
extern bool adminMode;
QMap<QString,double> contentSmallWeightMap;
QMap<QString,double> contentSmallWidthMap;
QMap<QString,double> contentAllWeightMap;
QMap<QString,double> contentAllWidthMap;
QString dieIp;
//QString autoIp;
#ifdef inMyLaptop
QString backupDir=QDir::currentPath()+"\\databaseBackups\\";
#else
QString backupDir="D:\\assimakis backup\\";
#endif
QString backupFilePrefix="apexDieWareHouse";
void createTable(QString tableName,QStringList colNames,QStringList colTypes,QString connectionName)
{
	QString stmnt=QString("IF NOT EXISTS (SELECT [name] FROM sys.tables WHERE [name] ='")+tableName+"') ";
	stmnt+=QString("CREATE TABLE ")+tableName+" (";
	for (int i=0;i<colNames.length();i++)
	{
		stmnt+=colNames[i]+" "+colTypes[i];
		if (i!=(colNames.length()-1))
			stmnt+=",";
	}
	stmnt+=")";
	execQuery(stmnt,connectionName);
}
mainWindowClass::mainWindowClass(bool useTranslation):QMainWindow()
{
	resize(1920,1080);
	new specialDiesClass();
#ifdef inMyLaptop
	QString odbcName="local";
	QString odbcUser="sa";
	QString odbcPassword=QString::fromLocal8Bit(qgetenv("ASSIMAKIS_DIEWAREHOUSE_ODBCPASSWORD"));
#else
	QString odbcName="sae01mssql01";
	QString odbcUser="DieWarehouse";
	QString odbcPassword=QString::fromLocal8Bit(qgetenv("ASSIMAKIS_DIEWAREHOUSE_ODBCPASSWORD"));
#endif
	dieIp=QString("20.20.20.50");
#ifdef useSettings
	//read settings file
	QString settingsFile=QString("settings.txt");
	QFile file(settingsFile);
	if (file.open(QIODevice::ReadWrite|QIODevice::Text))
	{
		QString read;
		QTextStream in(&file);
		read=in.readLine();
		QStringList odbcList=read.split(",");
		if (odbcList.count()==3)
		{
			odbcName=odbcList[0];
			odbcUser=odbcList[1];
			odbcPassword=odbcList[2];
		}
		read=in.readLine();
		if (read.split(".").count()==4)
			craneIp=read;
		read=in.readLine();
		if (read.split(".").count()==4)
			autoIp=read;
	}
	file.close();
	//get new settings
	if (adminMode)
	{
		settingsDlgClass *settingsDlg=new  settingsDlgClass(odbcName,odbcUser,odbcPassword,craneIp,autoIp,this);
		if (settingsDlg->exec())
		{
			odbcName=settingsDlg->getOdbcName();
			odbcUser=settingsDlg->getOdbcUser();
			odbcPassword=settingsDlg->getOdbcPassword();
			craneIp=settingsDlg->getBasketCranePlcIp();
			autoIp=settingsDlg->getAutoCranePlcIp();
		}
		if (file.open(QIODevice::WriteOnly))
		{
			file.flush();
			QTextStream out(&file);
			out<<odbcName<<","<<odbcUser<<","<<odbcPassword<<"\n";
			out<<craneIp<<"\n";
			out<<autoIp;
		}
		file.close();
	}
#endif
	splash->showMessage(tr("Starting..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	setWindowIcon(QIcon(":/assimakis.png"));
	setStyleSheet(QString("QLabel,QAction,QAction,QToolButton,QToolBar,QPushButton,QToolBox::tab,QToolBox,QCheckBox,QTabWidget,QSpinBox,QTableWidget,QTableWidgetItem,QListWidget{font-size:13px;color:#000033;}"));
	splash->showMessage(tr("Connecting to plc..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxipServer=new tuxipServerClass(this);
	bool noPlc=false;
#ifdef inMyLaptop
	noPlc=true;
#endif
	tuxipClass *basketTuxipConnection=tuxipServer->addConnection(dieIp,"warehouse");
	tuxipServer->addTable("bRobotToPcI",plcTableClass::TYPE_INT,0,75,100,dieIp);
	tuxipServer->addTable("bRobotToPcF",plcTableClass::TYPE_FLOAT,0,50,250,dieIp);

	splash->showMessage(tr("Backing up database..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	takeDatabaseBackup(backupFilePrefix);
	//
	splash->showMessage(tr("Connecting to database..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QSqlDatabase mainDb;
	QStringList a=QSqlDatabase::drivers();
	mainDb=QSqlDatabase::addDatabase("QODBC",db);
	mainDb.setDatabaseName(odbcName);//odbc name
	mainDb.setUserName(odbcUser);
	mainDb.setPassword(odbcPassword);
	if (!mainDb.open())
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(mainDb.databaseName()).arg(mainDb.lastError().text()));
#ifdef inMyLaptop
	execQuery("if not exists(select * from sys.databases where name='DieWareHouse') create database DieWareHouse",db);
	execQuery("USE DieWareHouse",db);
	//execQuery("if not exists(select * from sys.databases where name='apexDieWareHouse') create database apexDieWareHouse",db);
	//execQuery("USE apexDieWareHouse",db);
#else
	execQuery("if not exists(select * from sys.databases where name='DieWareHouse') create database DieWareHouse",db);
	execQuery("USE DieWareHouse",db);
#endif
//////	QSqlDatabase epicsData;
//////	epicsData = QSqlDatabase::addDatabase("QODBC",epicsDb);
//////#ifdef inMyLaptop
//////	epicsData.setDatabaseName(odbcName);//odbc name
//////	epicsData.setUserName(odbcUser);
//////	epicsData.setPassword(odbcPassword);
//////#else
//////	//epicsData.setDatabaseName("epics");//odbc name
//////	//epicsData.setUserName("foy");
//////	//epicsData.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_DIEWAREHOUSE_EPICSDATA_PASSWORD")));
//////	epicsData.setDatabaseName("epics2");//odbc name
//////	epicsData.setUserName("DieWH");
//////	epicsData.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_DIEWAREHOUSE_EPICSDATA_PASSWORD")));
//////#endif
//////	if (!epicsData.open())
//////		uExit(errorStr, QString("Cannot open %1 database. Error:%2").arg(epicsData.databaseName()).arg(epicsData.lastError().text()));
//////#ifdef inMyLaptop
//////	execQuery("if not exists(select * from sys.databases where name='Epics') create database Epics",db);
//////#endif
//////	execQuery("USE Epics", epicsDb);

	/*
#ifdef inMyLaptop
	createTable("assimakisDieWarehouse",QStringList()<<"tid"<<"DieNum"<<"CopyNum"<<"SchedSeq"<<"SchedDt"<<"Press",QStringList()<<"int PRIMARY KEY IDENTITY"<<"int"<<"int"<<"int"<<QString("char(20)")<<QString("char(20)"),db);
	//test cases
	QString sqlQueryPrefix{"insert into assimakisDieWarehouse (DieNum,CopyNum,SchedSeq,SchedDt,Press)  values(%1,%2,%3,'%4','%5')"};
	execQuery(sqlQueryPrefix.arg(1705).arg(6).arg(1).arg("t2024 05 19 08:00").arg("-"),db);
	execQuery(sqlQueryPrefix.arg(1034).arg(8).arg(1).arg("t2024 05 20 11:00").arg("-"),db);
	execQuery(sqlQueryPrefix.arg(1034).arg(8).arg(1).arg("t2024 05 21 12:00").arg("-"),db);
	execQuery(sqlQueryPrefix.arg(2444).arg(3).arg(1).arg("t2024 06 21 07:00").arg("-"),db);
	execQuery(sqlQueryPrefix.arg(1019).arg(9).arg(1).arg("t2024 05 21 12:00").arg("-"),db);
	execQuery(sqlQueryPrefix.arg(1019).arg(9).arg(2).arg("t2024 05 22 13:00").arg("-"),db); //seq=2 same date
#endif
*/

	//"log"
	createTable("log",QStringList()<<"tid"<<"type"<<"description"<<"time",QStringList()<<"int PRIMARY KEY IDENTITY"<<QString("char(%1)").arg(maxMessageLength)<<QString("char(%1)").arg(maxDescriptionLength)<<"char(50)",db);
	execQuery(QString("delete from log where tid not in(select top 10000 tid from log order by tid desc)"),db);
	//"logV"
	createTable("logV",QStringList()<<"tid"<<"type"<<"description"<<"time",QStringList()<<"int PRIMARY KEY IDENTITY"<<QString("char(%1)").arg(maxMessageLength)<<QString("char(%1)").arg(maxDescriptionLength)<<"char(50)",db);
	execQuery(QString("delete from logV where tid not in(select top 10000 tid from logV order by tid desc)"),db);
	//"logGui"
	createTable("logGui",QStringList()<<"tid"<<"mess"<<"details"<<"messenger"<<"type"<<"time",QStringList()<<"int PRIMARY KEY IDENTITY"<<QString("char(%1)").arg(maxMessageLength)<<QString("char(%1)").arg(maxDescriptionLength)<<"char(10)"<<"char(10)"<<"char(50)",db);
	execQuery(QString("delete from logGui where tid not in(select top 10000 tid from logGui order by tid desc)"),db);
	//"plcMessages"
	createTable("plcMessages",QStringList()<<"tid"<<"plcAddress"<<"plcIndex"<<"plcBit"<<"bitStatus"<<"type"<<"description"<<"time",QStringList()<<"int PRIMARY KEY IDENTITY"<<"char(15)"<<"int"<<"int"<<"int"<<"char(15)"<<"char(600)"<<"char(50)",db);
	execQuery(QString("delete from plcMessages where tid not in(select top 2000 tid from plcMessages order by tid desc)"),db);
	//"logPos"
	createTable("logPos",QStringList()<<"tid"<<"pos"<<"basket"<<"info"<<"time",QStringList()<<"int PRIMARY KEY IDENTITY"<<"int"<<"int"<<"char(50)"<<"char(50)",db);
	execQuery(QString("delete from logPos where tid not in(select top 10000 tid from logPos order by tid desc)"),db);
	//pos
	createTable("pos",QStringList()<<"tid"<<"plcId"<<"basket"<<"type"<<"col"<<"row"<<"dx"<<"dy"<<"dz"<<"locked"<<"description",QStringList()<<"int PRIMARY KEY IDENTITY"<<"int"<<"int"<<"int"<<"int"<<"int"<<"real"<<"real"<<"real"<<"int"<<"char(30)",db);
	//missionsTimeLog
	createTable("missionsTimeLog",QStringList()<<"tid"<<"fromPos"<<"toPos"<<"startSec"<<"endSec"<<"time",QStringList()<<"int PRIMARY KEY IDENTITY"<<"int"<<"int"<<"int"<<"int"<<"char(19)",db);
	execQuery(QString("delete from missionsTimeLog where tid not in(select top 30000 tid from missionsTimeLog order by tid desc)"),db);
	//users
	createTable("users",QStringList()<<"tid"<<"name"<<"pass",QStringList()<<"int PRIMARY KEY IDENTITY"<<"char(15)"<<"char(10)",db);
	execQuery("if not exists (select name from users where name='admin') insert into users (name,pass) values('admin','123')",db);
	//missions
	createTable("missions",QStringList()<<"status"<<"fromPos"<<"toPos",QStringList()<<"char(30)"<<"int"<<"int"<<"int",db);
	if (getOne("select count(status) from missions",db).toInt()==0)
	{
		execQuery(QString("insert into missions (status,fromPos,toPos) values('%1',0,0)").arg(nextStr),db);
		execQuery(QString("insert into missions (status,fromPos,toPos) values('%1',0,0)").arg(activeStr),db);
	}
	//contents
	createTable("basketsContents",QStringList()<<"tId"<<"basket"<<"contentName"<<"contentType",QStringList()<<"int PRIMARY KEY IDENTITY"<<"int"<<"char(50)"<<"char(50)",db);
	//contentTypes
	createTable("contentTypes",QStringList()<<"tId"<<"content"<<"weight"<<"width"<<"cellType",QStringList()<<"int PRIMARY KEY IDENTITY"<<"char(50)"<<"real"<<"real"<<"int",db);
	if (getOne("select count(tid) from contentTypes",db).toInt()==0)
	{
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Solid 254X50.8").arg(20.41165665).arg(50.8).arg(cellTypeNormal),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Solid 254X178").arg(20.41165665).arg(178).arg(cellTypeNormal),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Solid 304.5X50.8").arg(29.48350405).arg(50.8).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Solid 304.8X178").arg(30.84428116).arg(178).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Solid 355.6X228.6").arg(39.91612856).arg(228.6).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Hollow 254X178").arg(20.41165665).arg(178).arg(cellTypeNormal),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Hollow 254X228").arg(20.41165665).arg(228).arg(cellTypeNormal),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Hollow 304.5X178").arg(71.21400209).arg(178).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Hollow 304.8X228").arg(91.62565874).arg(228).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Hollow 355.6X178").arg(101.1510985).arg(178).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Hollow 355.6x228").arg(131.5417873).arg(228).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Hollow 355.6X279").arg(139.2528576).arg(279).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Hollow 355.6X178").arg(179.1689862).arg(178).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Backer 254x127").arg(50.80234544).arg(127).arg(cellTypeNormal),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Backer 254x178").arg(70.76040972).arg(178).arg(cellTypeNormal),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Backer 304x127").arg(73.02837157).arg(127).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Backer 304x178").arg(102.0582833).arg(178).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Backer 355x127").arg(99.7903214).arg(127).arg(cellTypeBig),db);
		execQuery(QString("insert into contentTypes (content,weight,width,cellType) values('%1',%2,%3,%4)").arg("Backer 355x178").arg(119.2947933).arg(178).arg(cellTypeBig),db);
	}
	QVector<QVector<QVariant>> normalVV=execTableQuery(QString("select content,weight,width from contentTypes where cellType=%1 order by content").arg(cellTypeNormal),db);
	for (int i=0;i<normalVV.count();i++)
	{
		contentSmallWeightMap[normalVV[i][0].toString().trimmed()]=normalVV[i][1].toDouble();
		contentSmallWidthMap[normalVV[i][0].toString().trimmed()]=normalVV[i][2].toDouble();
	}
	QVector<QVector<QVariant>> allVV=execTableQuery(QString("select content,weight,width from contentTypes order by content"),db);
	for (int i=0;i<allVV.count();i++)
	{
		contentAllWeightMap[allVV[i][0].toString().trimmed()]=allVV[i][1].toDouble();
		contentAllWidthMap[allVV[i][0].toString().trimmed()]=allVV[i][2].toDouble();
	}
	//createTable("userTransactions",QStringList()<<"tId"<<"userName"<<"type"<<"basket"<<"content"<<"diameter"<<"width"<<"weight"<<"date",QStringList()<<"int PRIMARY KEY IDENTITY"<<"char(30)"<<"char(10)"<<"int"<<"char(30)"<<"int"<<"int"<<"float"<<"char(20)",db);
	//execQuery(QString("delete from userTransactions where tId not in(select top 10000 tId from userTransactions order by date desc)"),db);
	//
	if (getOne("select count(tid) from pos",db).toInt()==0)
	{
		execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,0,'crane'   ,-1,-1,0,0,0,0,%2)").arg(cranePlcId).arg(cellTypeBig),db);
		execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,0,'export'   ,-1,-1,0,0,0,0,%2)").arg(exitPlcId).arg(cellTypeBig),db);
		execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,0,'spare' ,-1,-1,0,0,0,0,%2)").arg(10011).arg(cellTypeBig),db);
		execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,0,'spare' ,-1,-1,0,0,0,0,%2)").arg(10012).arg(cellTypeBig),db);
		execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,0,'spare' ,-1,-1,0,0,0,0,%2)").arg(10013).arg(cellTypeBig),db);
		execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,0,'spare' ,-1,-1,0,0,0,0,%2)").arg(10014).arg(cellTypeBig),db);
		execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,0,'spare' ,-1,-1,0,0,0,0,%2)").arg(10015).arg(cellTypeBig),db);
		int pMinus=0;
		int pPluss=0;
		int type=cellTypeNormal;
		for (int c=1;c<=15;c++)
		{
			for (int r=1;r<=17;r++)
			{
				if (((c!=1)&&(c!=2))||(r<=13))
				{
					pPluss++;
					if ((pPluss<=26)||(r==17))
						type=cellTypeBig;
					else
						type=cellTypeNormal;
					execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,%2,'%1',%3,%4,0,0,0,0,%5)").arg(pPluss).arg(pPluss).arg(c).arg(r).arg(type),db);
				}
				pMinus++;
				execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,%2,'%1',%3,%4,0,0,0,0,%5)").arg(-pMinus).arg(-pMinus).arg(c).arg(r).arg(type),db);
			}
		}
	}
	if (getOne("select count(plcId) from pos where plcId<0",db).toInt()==255)
	{
		execQuery(QString("delete from pos where plcId<0"),db);
		execQuery(QString("update basketsContents set basket=(247-basket) where basket<0"),db);
		int plcId=247;
		for (int col=16;col<=31;col++)
		{
			if ((col>=16)&&(col<=24))
			{
				for (int row=1;row<=16;row++)
				{
					plcId++;
					execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,%2,'%1',%3,%4,0,0,0,0,%5)").arg(plcId).arg(plcId).arg(col).arg(row).arg(cellTypeNormal),db);
				}
				plcId++;
				execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,%2,'%1',%3,%4,0,0,0,0,%5)").arg(plcId).arg(plcId).arg(col).arg(17).arg(cellTypeBig),db);
			}
			if (col>24)
			{
				for (int row=1;row<=13;row++)
				{
					plcId++;
					execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,%2,'%1',%3,%4,0,0,0,0,%5)").arg(plcId).arg(plcId).arg(col).arg(row).arg(cellTypeBig),db);
				}
			}
		}
	}
	if (getOne(QString("select max(plcId) from pos where plcId<%1").arg(cranePlcId),db).toInt()==491)
	{
		int plcId=492;
		for (int col=32;col<=51;col++)
		{
			for (int row=1;row<=13;row++)
			{
				execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,%2,'%1',%3,%4,0,0,0,0,%5)").arg(plcId).arg(plcId).arg(col).arg(row).arg(cellTypeBig),db);
				plcId++;
			}
		}
		for (int col=52;col<=61;col++)
		{
			for (int row=1;row<=17;row++)
			{
				execQuery(QString("insert into pos (plcId,basket,description,col,row,dx,dy,dz,locked,type) values(%1,%2,'%1',%3,%4,0,0,0,0,%5)").arg(plcId).arg(plcId).arg(col).arg(row).arg(cellTypeNormal),db);
				plcId++;
			}
		}
	}
	//exportsE1Table
	createTable(exportsTable,QStringList()<<"tId"<<"basket"<<"priority",QStringList()<<"int PRIMARY KEY IDENTITY"<<"int UNIQUE"<<"int UNIQUE",db);
	log("programm started",infoStr);
	//
	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) || (GetAsyncKeyState(VK_LCONTROL) & 0x8000) || (GetAsyncKeyState(VK_RCONTROL) & 0x8000))
	{
		passwordClass *pass = new passwordClass();
		pass->exec();
		if (pass->getPasswordIsCorrect())
			adminMode = true;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//layoutGraphicsView
	layoutGraphicsView=new myQGraphicsViewClass(true,this);
	layoutGraphicsView->scale(0.05,0.05);
	QGraphicsScene *layoutScene=new QGraphicsScene(this);

	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr("Zoom extents"),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));

	dxfGroupOfGraphicsItemClass *layoutGraphicsItem=new dxfGroupOfGraphicsItemClass(":dxfs/layout.dxf",QPen(QColor(200,200,200)),false,false,QColor(),this);
	layoutGraphicsItem->setZValue(-10.);layoutScene->addItem(layoutGraphicsItem);

	allPos=new allPossClass(basketTuxipConnection,layoutScene,this);
	globalAllPosPtr=allPos;
	crane=new craneClass(tuxipServer,basketTuxipConnection,allPos,layoutScene,useTranslation,this);
	splash->showMessage(tr("Creating gui..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));

	crane->setStations(allPos->getWorkStation());
	allPos->setCraneClass(crane);
	autoWidget=new autoWidgetClass(crane,allPos->getWorkStation(),basketTuxipConnection,this);

	layoutGraphicsView->setScene(layoutScene);
	//
	connect(tuxipServer,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(dataInPlcChangedSlot(QString,QString,int,QVariant)));
	//mainToolBar
	QToolBar *mainToolBar=new QToolBar(tr("Tools"));mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
	addToolBar(Qt::TopToolBarArea,mainToolBar);
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	QAction *createReportAction=new QAction(QIcon(":/pdf.png"),tr("Report"),this);
	createReportAction->setToolTip(tr("Creates a pdf file report of the warehouse contents.\nCtrl+R"));
	createReportAction->setStatusTip(tr("Creates a pdf file report of the warehouse contents."));
	createReportAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_R));
	connect(createReportAction,SIGNAL(triggered()),this,SLOT(createReportActionSlot()));

	showTuxServerAction=new QAction(QIcon(":/notOk.png"),tr("Plc comms"),this);
	showTuxServerAction->setToolTip(tr("Shows the plc comunication table dialog.\nCtrl+P"));
	showTuxServerAction->setStatusTip(tr("Shows the plc comunication table dialog."));
	showTuxServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	connect(showTuxServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));

	QAction *userDatabaseBackupAction=new QAction(QIcon(":/backup.png"),tr("Backup"),this);
	userDatabaseBackupAction->setToolTip(tr("Creates a backup of the database.\nCtrl+B"));
	userDatabaseBackupAction->setStatusTip(tr("Creates a backup of the database."));
	userDatabaseBackupAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_B));
	connect(userDatabaseBackupAction,SIGNAL(triggered()),this,SLOT(userDatabaseBackupActionSlot()));

	QAction *showUsageDialogAction=new QAction(QIcon(":/usage.png"),tr("Usage"),this);
	showUsageDialogAction->setToolTip(tr("Show crane usage.\nCtrl+U"));
	showUsageDialogAction->setStatusTip(tr("Show crane usage."));
	showUsageDialogAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
	connect(showUsageDialogAction,SIGNAL(triggered()),this,SLOT(showUsageDialogSlot()));

	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);

	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);

	mainToolBar->addSeparator();
	//mainToolBar->addAction(createReportAction);
	mainToolBar->addAction(showTuxServerAction);
	mainToolBar->addAction(userDatabaseBackupAction);
	//mainToolBar->addAction(showUsageDialogAction);
	QWidget *spacerWdgt=new QWidget();
	spacerWdgt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	mainToolBar->addWidget(spacerWdgt);
	QPushButton *hmiDlgBasicButton=new QPushButton(QIcon(":/androidRed.png"),tr("Basic HMI"));
	mainToolBar->addWidget(hmiDlgBasicButton);
	connect(hmiDlgBasicButton,SIGNAL(clicked()),this,SLOT(hmiDlgBasicButtonClickedSlot()));
	hmiDlgBasic=new hmiDlgBasicClass(tuxipServer,basketTuxipConnection,useTranslation,this);
	QWidget *toolBoxWidget=new QWidget();QVBoxLayout *toolBoxWidgetLayout=new QVBoxLayout();toolBoxWidget->setLayout(toolBoxWidgetLayout);toolBoxWidgetLayout->setMargin(3);
	QToolBox *toolBox=new QToolBox(this);toolBoxWidgetLayout->addWidget(toolBox);
	toolBox->setStyleSheet(""
	" QToolBox::tab {																																				"
 "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,															"
 "                               stop: 0 #E1E1E9, stop: 0.4 #aaaacc,											"
 "                               stop: 0.5 #9999bb, stop: 1.0 #D3D3D9);										"
 "   border-radius: 5px;																																	"
 "   color: black;																																			"
 "}"
	""
 "QToolBox::tab:selected { 		 "
 "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,															"
 "                               stop: 0 #E1E1E9, stop: 0.4 #aaaaDD,											"
 "                               stop: 0.5 #9999D8, stop: 1.0 #D3D3D9);										"
 "    color: red;																					 "
 "}"
		);
	int index=0;
	toolBox->addItem(autoWidget,QIcon(":/apexIcon.png"),tr("Work"));
	toolBox->widget(index)->setToolTip(tr("Work position"));
	//toolBox->addItem(crane->getCraneWidget(),QIcon(":/assimakis.png"),tr("Crane"));
	//toolBox->widget(1)->setToolTip(tr("Crane Data"));
	//index++;
	//toolBox->addItem(exportStation->getExportStationWidget(),QIcon(":/export.png"),tr("Exports to Picking"));
	//toolBox->widget(index)->setToolTip(tr("Exports"));
	//exportStation->setToolBox(toolBox,index);
	index++;
	toolBox->addItem(new searchWidgetClass(allPos,this),QIcon(":/search.png"),tr("Search"));
	toolBox->widget(index)->setToolTip(tr("Search the warehouse."));

	QSplitter *centralSplitter=new QSplitter(Qt::Horizontal);
	centralSplitter->setStyleSheet(splitterHorizontalStyle);
	setCentralWidget(centralSplitter);
	//
	statusBar();
	centralSplitter->addWidget(toolBoxWidget);
	centralSplitter->addWidget(layoutGraphicsView);
	centralSplitter->addWidget(crane->getCraneWidget());
	centralSplitter->setSizes(QList<int>()<<450<<1850-750<<300);
	//
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer->start(60*1000);
	//
	QTimer *backupTimer=new QTimer(this);
	backupTimer->start(1000*60*60*8);
	connect(backupTimer,SIGNAL(timeout()),this,SLOT(onBackupDatabaseTimerSlot()));
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	splash->showMessage(tr("Reading plc tables..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	//exit( 0);
	handShake=new handShakeClass(basketTuxipConnection,dieIp,"bRobotToPcI",25,"bRobotToPcI",26,2000);
	connect(handShake,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeStateChangedSlot(bool)));
	tuxipServer->setIsAutoUpdatingFromPlc(true);
	splash->showMessage(tr("Checking for open missions..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	crane->checkForNextMissionInDatabase();
	//
	clearBackups(backupDir);
	mainTimerSlot();
}
mainWindowClass::~mainWindowClass()
{
	takeDatabaseBackup(backupFilePrefix);
	log("programm ended",infoStr);
}
void mainWindowClass::handShakeStateChangedSlot(bool s)
{
	if (s)
		showTuxServerAction->setIcon(QIcon(":/ok.png"));
	else
		showTuxServerAction->setIcon(QIcon(":/notOk.png"));
}
void mainWindowClass::openWorkStationDlg()
{
}
void mainWindowClass::onBackupDatabaseTimerSlot()
{
	logV("database backup",infoStr);
	takeDatabaseBackup(backupFilePrefix);
}
void mainWindowClass::userDatabaseBackupActionSlot()
{
	logV("user database backup",infoStr);
	QString f=takeDatabaseBackup("user");
	QMessageBox::information(this,tr("Information"),tr("Database backup was created:\n%1").arg(f));
}
void mainWindowClass::showUsageDialogSlot()
{
	usageDialogClass *usage=new usageDialogClass(this);
	usage->exec();
}
void mainWindowClass::hmiDlgBasicButtonClickedSlot()
{
	hmiDlgBasic->exec();
}
void mainWindowClass::clearBackups(QString dir)
{
	QDir bakFilesDir=dir;
	//new orders
	QStringList bakFilesList=bakFilesDir.entryList(QStringList()<<"*.bak",QDir::Files,QDir::Name);
	int currentYear=QDateTime::currentDateTime().toString("yyyy").toInt();
	int currentMonth=QDateTime::currentDateTime().toString("MM").toInt();
	int currentDay=QDateTime::currentDateTime().toString("dd").toInt();
	int keep=15;
	for (int i=0;i<bakFilesList.count();i++)
	{
		if (bakFilesList[i].left(21)!=backupFilePrefix)
		{
			bakFilesList.removeAt(i);
			i--;
		}
	}
	if (bakFilesList.count()>keep)
	{
		for (int i=0;i<(bakFilesList.count()-keep);i++)
		{
			QString fileName=bakFilesList[i];
			QStringList fileNameList=fileName.split('.');
			int y=fileNameList[1].toInt();
			int m=fileNameList[2].toInt();
			int d=fileNameList[3].toInt();
			if (y<currentYear)
				bakFilesDir.remove(dir+fileName);
			else if (m<currentMonth)
				bakFilesDir.remove(dir+fileName);
			else if (d<(currentDay-2))
				bakFilesDir.remove(dir+fileName);
		}
	}
}
QString mainWindowClass::takeDatabaseBackup(QString fName)
{
#ifdef inMyLaptop
	return QString();
#endif
#ifdef doNotTakeBackup
	return QString();
#endif
	QDir::current().mkdir("databaseBackups");
	QString cmd,backupFile,backupFileAbsPath;
	//backupDir=QDir::current().absolutePath()+"/databaseBackups/";
	backupFile=QString("%1.%2.bak").arg(fName).arg(timeForFile);
	backupFileAbsPath=backupDir+backupFile;
	cmd=QString("sqlcmd -S 192.168.27.199 -U assimakis -P 123 -Q \"BACKUP DATABASE apexDieWareHouse TO DISK='"+backupFileAbsPath+"' WITH INIT\"");
	QProcess *backupDataProcess=new QProcess(this);
	connect(backupDataProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(backupFinishedSlot(int,QProcess::ExitStatus)));
	backupDataProcess->start(cmd);
	backupDataProcess->waitForFinished();
	backupDataProcess->close();
	return backupFileAbsPath;
}
void mainWindowClass::backupFinishedSlot(int exitCode,QProcess::ExitStatus exitStatus)
{
	if (exitCode!=0)
		uExit("Error","Cannot take database backup");
}
void mainWindowClass::dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value)
{
	allPos->dataInPlcChanged(ip,table,index,value);
	if (ip==dieIp)
	{
		crane->dataInPlcChanged(ip,table,index,value);
		hmiDlgBasic->dataInPlcChanged(ip,table,index,value);
		handShake->dataInPlcChanged(ip,table,index,value);
	}
}
void mainWindowClass::showServerSlot()
{
	if (adminMode)
	{
		tuxipServer->showDialog();
		return;
	}
	passwordClass *pass=new passwordClass(this);
	pass->exec();
	if (pass->getPasswordIsCorrect())
		tuxipServer->showDialog();
}
void mainWindowClass::createReportActionSlot()
{
	QDir::current().mkdir("reports");
	QString pdfFile=QDir::current().absolutePath()+QString("/reports/wareHouseReport.%1").arg(timeForFile+".pdf");
	allPos->drawForReport(pdfFile);
	QDesktopServices::openUrl(pdfFile);
}
void mainWindowClass::qtAboutSlot()
{
	 QMessageBox::aboutQt(this,tr("apex die warehouse build: %1 ").arg(uCompilerData));
}
void mainWindowClass::stopSlot()
{
	tuxipServer->writeInteger("bRobotToPcI",22,226,dieIp);
	tuxipServer->writeInteger("bRobotToPcI",22,0,dieIp);
}
void mainWindowClass::resetGraphicsViewZoom()
{
	layoutGraphicsView->scene()->setSceneRect(layoutGraphicsView->scene()->itemsBoundingRect());
	layoutGraphicsView->fitInView(layoutGraphicsView->sceneRect(),Qt::KeepAspectRatio);
}
void mainWindowClass::mainTimerSlot()
{
	setWindowTitle(tr("apex die warehouse %1 %2 (%3 dies)", "main window title..").arg(timeVerbose).arg(adminMode ? tr("(Administrator mode)") : "").arg(getOne("select count(tId) from basketsContents where contentName<>''", db).toInt()));
}

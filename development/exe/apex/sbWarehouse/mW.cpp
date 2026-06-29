#include "pch.h"
#include "mW.h"
//#define inMyLaptop
//#define noMission
bool adminMode=false;
allPossClass *globalAllPosPtr;
extern bool adminMode;
QString remoteBackupDb="remote";
QStatusBar *globalStatusBar;
#ifdef inMyLaptop
QString backupDir=QDir::currentPath()+"//databaseBackups//";
#else
QString backupDir="C://assimakis//workingExes//packetCrane//databaseBackups//";
#endif
QString mainDataBaseName="apexSbWarehouse";
QString backupFilePrefix="apexSbWarehouse";
float scaleReading;
int ticketInScaleStation;

int emptySbWarningLimit=10;
int emptySbOkLimit=15;
bool sbAreLessThanWarningLimitEmailSend=false;
bool sbAreMoreThanOkLimitEmailSend=false;
QStringList sbEmailList;
void createTable(QString tableName,QStringList colNamesTypes,QString connectionName,QString primaryKeyStr=",primary key (tId)")
{
	QString stmnt=QString("create table if not exists %1 (").arg(tableName);
	for (int i=0;i<colNamesTypes.length();i++)
	{
		stmnt+=colNamesTypes[i];
		if (i!=(colNamesTypes.length()-1))
			stmnt+=",";
	}
	stmnt+=primaryKeyStr+")";
	execQuery(stmnt,connectionName);
}
mainWindowClass::mainWindowClass():QMainWindow()
{
	globalStatusBar=QMainWindow::statusBar();
	splash->showMessage(tr("Backing up..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	backupMessage="backup from apex";
	takeDatabaseBackup("exeStarted");
	resize(1880,980);
#ifdef inMyLaptop
	QString mySqlServer="127.0.0.1";
#else
	QString mySqlServer=sbDbIp;
#endif
	QString mySqlUser="user";
	QString mySqlPassword="1234";
	sbAreLessThanWarningLimitEmailSend=false;
	sbAreMoreThanOkLimitEmailSend=false;
	splash->showMessage(tr("Starting..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	setWindowIcon(QIcon(":/assimakis.png"));
	setStyleSheet(QString("QLabel,QAction,QToolButton,QToolBar,QPushButton,QToolBox::tab,QToolBox,QCheckBox,QTabWidget,QSpinBox,QTableWidget,QTableWidgetItem,QListWidget{font-size:13px;color:#000033;}"));
	setStyleSheet(QString("QLineEdit:disabled{font-size:13px;color:#330000;}"));
	tuxipServer=new tuxipServerClass(this);
	splash->showMessage(tr("Connecting to %1").arg(sbWarehouseIp),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxipClass *sbTuxipConnection=tuxipServer->addConnection(sbWarehouseIp,"sb warehouse",false);
	sbTuxipConnection->addTable("bRobotToPcI",plcTableClass::TYPE_INT,0,75,100);
	sbTuxipConnection->addTable("bRobotToPcF",plcTableClass::TYPE_FLOAT,0,50,100);
	sbTuxipConnection->addTable("posCassette",plcTableClass::TYPE_INT,0,75,100);
	sbTuxipConnection->addTable("posSb",plcTableClass::TYPE_INT,0,75,100);
	sbTuxipConnection->addTable("posTicket",plcTableClass::TYPE_INT,0,75,100);
	sbTuxipConnection->addTable("PC2_PCcomDint",plcTableClass::TYPE_INT,0,49,100);
	sbTuxipConnection->addTable("PC2_PCcomReal",plcTableClass::TYPE_FLOAT,0,49,100);
	sbTuxipConnection->addTable("rConveyorState",plcTableClass::TYPE_INT,0,53,100);
	sbTuxipConnection->addTable("pcCom",plcTableClass::TYPE_INT,0,9,100);

	tuxipClass *autoCraneTuxipConnection=tuxipServer->addConnection(crane2Ip,"auto crane",false);
	autoCraneTuxipConnection->addTable("posCassette",plcTableClass::TYPE_INT,0,75,100);
	autoCraneTuxipConnection->addTable("posSb",plcTableClass::TYPE_INT,0,75,100);
	autoCraneTuxipConnection->addTable("posTicket",plcTableClass::TYPE_INT,0,75,100);
	autoCraneTuxipConnection->addTable("pc1ToPCI",plcTableClass::TYPE_INT,0,50,100);
	autoCraneTuxipConnection->addTable("pc1ToPCF",plcTableClass::TYPE_FLOAT,0,50,100);
	autoCraneTuxipConnection->addTable("rConveyorState",plcTableClass::TYPE_INT,0,55,100);
	autoCraneTuxipConnection->addTable("rPcCom",plcTableClass::TYPE_INT,0,9,100);
	autoCraneTuxipConnection->addTable("toPcI",plcTableClass::TYPE_INT,0,26,100);
	setPlcComments();
	splash->showMessage(tr("Backing up database..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	///////////////////////////////////////////////////////////////////////////////////////mDb
	splash->showMessage(tr("Connecting to main database..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QSqlDatabase mainDb;
	mainDb=QSqlDatabase::addDatabase("QMYSQL",mDb);
	mainDb.setHostName(mySqlServer);
	mainDb.setUserName(mySqlUser);
	mainDb.setPassword(mySqlPassword);
	if (!mainDb.open()) 
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(mainDb.databaseName()).arg(mainDb.lastError().text()));
	execQuery(QString("create database if not exists %1").arg(mainDataBaseName),mDb);
	execQuery(QString("use %1").arg(mainDataBaseName),mDb);
	//"log"
	createTable("log",QStringList()<<"tId int not null auto_increment"<<QString("type char(%1)").arg(maxMessageLength)<<QString("description char(%1)").arg(maxDescriptionLength)<<"time char(50)",mDb);
	execQuery(QString("delete from %1 where tId<((select m from (select max(tId) as m from %1) as t)-%2)").arg("log").arg(10000),mDb);
	//"logV"
	createTable("logV",QStringList()<<"tId int not null auto_increment"<<QString("type char(%1)").arg(maxMessageLength)<<QString("description char(%1)").arg(maxDescriptionLength)<<"time char(50)",mDb);
	execQuery(QString("delete from %1 where tId<((select m from (select max(tId) as m from %1) as t)-%2)").arg("logV").arg(10000),mDb);
	//"logGui"
	createTable("logGui",QStringList()<<"tId int not null auto_increment"<<QString("mess char(%1)").arg(maxMessageLength)<<QString("details char(%1)").arg(maxDescriptionLength)<<"messenger char(10)"<<"type char(10)"<<"time char(50)",mDb);
	execQuery(QString("delete from %1 where tId<((select m from (select max(tId) as m from %1) as t)-%2)").arg("logGui").arg(10000),mDb);
	//"plcMessages"
	createTable("plcMessages",QStringList()<<"tId int not null auto_increment"<<"plcAddress char(15)"<<"plcIndex int"<<"plcBit int"<<"bitStatus int"<<"type char(15)"<<"description char(150)"<<"time char(50)",mDb);
	execQuery(QString("delete from %1 where tId<((select m from (select max(tId) as m from %1) as t)-%2)").arg("plcMessages").arg(10000),mDb);
	////"logPos"
	createTable("positionsLog",QStringList()<<"tId int not null auto_increment"<<"posId int"<<"cassette int"<<"sbNumber int"<<"ticket int"<<"info char(20)"<<"time char(20)",mDb);
	//missionsTimeLog
	createTable("missionsTimeLog",QStringList()<<"tId int not null auto_increment"<<"fromPos int"<<"toPos int"<<"startSec int"<<"endSec int"<<"time char(19)",mDb);
	execQuery(QString("delete from %1 where tId<((select m from (select max(tId) as m from %1) as t)-%2)").arg("missionsTimeLog").arg(10000),mDb);
	//missions
	createTable("missions",QStringList()<<"tId int not null auto_increment"<<"conveyorPos char(30)"<<"fromPos int"<<"toPos int",mDb);
	if (getOne(QString("select count(conveyorPos) from missions where conveyorPos='%1'").arg(activeStr),mDb).toInt()==0)
		execQuery(QString("insert into missions (conveyorPos,fromPos,toPos) values('%1',0,0)").arg(activeStr),mDb);
	if (getOne(QString("select count(conveyorPos) from missions where conveyorPos='%1'").arg(nextStr),mDb).toInt()==0)
		execQuery(QString("insert into missions (conveyorPos,fromPos,toPos) values('%1',0,0)").arg(nextStr),mDb);
	createTable("userTransactions",QStringList()<<"tId int not null auto_increment"<<"userName char(30)"<<"type char(10)"<<"cassette int"<<"packet int"<<"date char(20)",mDb);
	execQuery(QString("delete from %1 where tId<((select m from (select max(tId) as m from %1) as t)-%2)").arg("userTransactions").arg(10000),mDb);
	createTable("tickets",QStringList()<<"tId int not null auto_increment"<<"ticket int"<<"weight float"<<"date char(20)",mDb);
	//pos
	createTable("pos",QStringList()<<"tId int not null auto_increment"<<"posId int"<<"cassette int"<<"sbNumber int"<<"ticket int"<<"colNo int"<<"rowNo int"<<"dx float"<<"dy float"<<"dz float"<<"locked int"<<"description char(40)",mDb);
	//
	createTable(exportsFrontTable,QStringList()<<"tId int not null auto_increment"<<"cassette int"<<"priority int",mDb);
	createTable(exportsTriple1Table,QStringList()<<"tId int not null auto_increment"<<"cassette int"<<"priority int",mDb);
	createTable(exportsTriple2Table,QStringList()<<"tId int not null auto_increment"<<"cassette int"<<"priority int",mDb);
	createTable(exportsDoubleTable,QStringList()<<"tId int not null auto_increment"<<"cassette int"<<"priority int",mDb);

	createTable("importExportLog",QStringList()<<"tId int not null auto_increment"<<QString("ticket int")<<QString("sbNumber int")<<"dateAndTime timestamp"<<"importLocation char(50)"<<"exportLocation char(50)"<<QString("triggerStatus int"),mDb);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (getOne("select count(tId) from pos",mDb).toInt()==0)
	{
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(craneId)  .arg("Sb crane")  ,mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1Id).arg("Old pincer"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2Id).arg("New pincer"),mDb);

		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(conveyor__1_id).arg("West side cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(conveyor__2_id).arg("West side cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(conveyor__4_id).arg("West side cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(conveyor__5_id).arg("West side cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(conveyor__6_id).arg("West side cassette conveyor"),mDb);

		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,43,0,0,0,0,0)").arg(cassetteTripleExit__8_id).arg("East side triple cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,43,0,0,0,0,0)").arg(cassetteTripleExit__9_id).arg("East side triple cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,44,0,0,0,0,0)").arg(cassetteTripleExit_10_id).arg("East side triple cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,44,0,0,0,0,0)").arg(cassetteTripleExit_11_id).arg("East side triple cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,45,0,0,0,0,0)").arg(cassetteTripleExit_12_id).arg("East side triple cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,45,0,0,0,0,0)").arg(cassetteTripleExit_13_id).arg("East side triple cassette conveyor"),mDb);

		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,54,0,0,0,0,0)").arg(cassetteDoubleExit_15_id).arg("East side double cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,55,0,0,0,0,0)").arg(cassetteDoubleExit_16_id).arg("East side double cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,55,0,0,0,0,0)").arg(cassetteDoubleExit_17_id).arg("East side double cassette conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,54,0,0,0,0,0)").arg(cassetteDoubleExit_18_id).arg("East side double cassette conveyor"),mDb);

		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_41_id).arg("Old pincer blue baskets buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_42_id).arg("Old pincer blue baskets buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_43_id).arg("Old pincer blue baskets buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_44_id).arg("Old pincer blue baskets buffer position"),mDb);

		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_45_id).arg("Old pincer blue baskets buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_46_id).arg("Old pincer blue baskets buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_47_id).arg("Old pincer blue baskets buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_48_id).arg("Old pincer blue baskets buffer position"),mDb);

		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_49_id).arg("Old pincer destacker buffer sb conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_50_id).arg("Old pincer destacker working sb conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_51_id).arg("Old pincer destacker scrup sb position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_52_id).arg("Old pincer to warehouse conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_53_id).arg("Old pincer system side moving conveyor"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_54_id).arg("Old pincer system import/export conveyor"),mDb);

		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_21_id).arg("New pincer import buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_24_id).arg("New pincer import buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_25_id).arg("New pincer import buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_26_id).arg("New pincer import buffer position"),mDb);

		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_22_id).arg("New pincer import buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_27_id).arg("New pincer import buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_28_id).arg("New pincer import buffer position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_29_id).arg("New pincer import buffer position"),mDb);

		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_20_id).arg("New pincer unload position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_31_id).arg("New pincer unload position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_32_id).arg("New pincer unload position"),mDb);
		execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer2_33_id).arg("New pincer unload position"),mDb);
		for (int i=1;i<=10;i++)
			execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'%2',0,0,0,0,0,0,0,0,0)").arg(pincer1_53_id+i).arg("spare"),mDb);

		//cells
		int posId=0;
		for (int col=1;col<=55;col++)
		{
			for (int row=1;row<=9;row++)
			{
				posId++;
				execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'cell %1',%1,%2,0,%3,%4,0,0,0,0)") .arg(posId).arg(0).arg(col).arg(row),mDb);
				execQuery(QString("insert into pos (posId,description,cassette,sbNumber,ticket,colNo,rowNo,dx,dy,dz,locked) values(%1,'cell %1',%1,%2,0,%3,%4,0,0,0,0)").arg(-posId).arg(0).arg(col).arg(row),mDb);
			}
		}
	}
	initializeSbWarehouseGlobals();
	splash->showMessage(tr("Creating layout..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	//layoutGraphicsView
	layoutGraphicsView=new myQGraphicsViewClass(true,this);
	layoutGraphicsView->scale(0.015,0.015);
	layoutScene=new QGraphicsScene(this);
	layoutGraphicsView->setScene(layoutScene);

	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr("Zoom extents"),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	//zoomExtentsBtn->setMaximumWidth(150);
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));

	QPushButton *dtmBtn=new QPushButton(QIcon(":/maintenance.png"),tr("DTM"),layoutGraphicsView);
	dtmBtn->setToolTip(tr("Show dtm log."));
	dtmBtn->move(125,5);
	connect(dtmBtn,SIGNAL(clicked(bool)),this,SLOT(dtmBtnSlot()));

	gItemClass *layoutGraphicsItem=new gItemClass(":dxfs/layout.dxf",QPen(QColor(200,200,200)),this,Qt::NoBrush,Qt::NoBrush);
	layoutGraphicsItem->setZValue(-10.);layoutScene->addItem(layoutGraphicsItem);

	
	splash->showMessage(tr("Creating positions..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	allPos=new allPossClass(sbTuxipConnection,autoCraneTuxipConnection,allPossClass::showCranePos|allPossClass::showPincer1Pos|allPossClass::showPincer2Pos,layoutScene,this);
	globalAllPosPtr=allPos; 

	splash->showMessage(tr("Creating pincer 1..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	pincer1Dlg=new QDialog(this);pincer1Dlg->resize(1600,1000);
	pincer1Dlg->setWindowTitle(tr("Pincer 1"));
	QVBoxLayout *pincer1DlgL=new QVBoxLayout();pincer1DlgL->setMargin(2);pincer1Dlg->setLayout(pincer1DlgL);
	pincer1=new pincer1Class(allPos,autoCraneTuxipConnection,this);pincer1DlgL->addWidget(pincer1);

	splash->showMessage(tr("Creating pincer 1 conveyors..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	pincer1ConvDlg=new QDialog(this);pincer1ConvDlg->resize(1600,1000);
	pincer1ConvDlg->setWindowTitle(tr("Pincer 1 conveyors"));
	QVBoxLayout *pincer1ConvDlgL=new QVBoxLayout();pincer1ConvDlgL->setMargin(2);pincer1ConvDlg->setLayout(pincer1ConvDlgL);
	pincer1Conv=new pincer1ConvPositionsClass(autoCraneTuxipConnection,allPos,this);pincer1ConvDlgL->addWidget(pincer1Conv);

#ifndef sbWarehouse
	splash->showMessage(tr("Creating pincer 2..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	pincer2Dlg=new QDialog(this);pincer2Dlg->resize(1600,1000);
	pincer2Dlg->setWindowTitle(tr("Pincer 2"));
	QVBoxLayout *pincer2DlgL=new QVBoxLayout();pincer2DlgL->setMargin(2);pincer2Dlg->setLayout(pincer2DlgL);
	pincer2=new pincer2Class(allPos,sbTuxipConnection,this);pincer2DlgL->addWidget(pincer2);

	splash->showMessage(tr("Creating pincer 2 conveyors..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	pincer2ConvDlg=new QDialog(this);pincer2ConvDlg->resize(1600,1000);
	pincer2ConvDlg->setWindowTitle(tr("Pincer 2 conveyors"));
	QVBoxLayout *pincer2ConvDlgL=new QVBoxLayout();pincer2ConvDlgL->setMargin(2);pincer2ConvDlg->setLayout(pincer2ConvDlgL);
	pincer2Conv=new pincer2ConvPositionsClass(sbTuxipConnection,allPos,this);pincer2ConvDlgL->addWidget(pincer2Conv);
#endif
	splash->showMessage(tr("Creating crane..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	crane=new craneClass(sbTuxipConnection,autoCraneTuxipConnection,allPos,layoutScene,false,this);

	allPos->setCraneClass(crane);
	splash->showMessage(tr("Creating auto missions gui..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	autoWidget=new autoWidgetClass(tuxipServer,crane,this);

	//
	connect(tuxipServer,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(dataInPlcChangedSlot(QString,QString,int,QVariant)));
	splash->showMessage(tr("Creating toolbars..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	//mainToolBar
	QToolBar *mainToolBar=new QToolBar(tr("Tools"));mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
//	addToolBar(Qt::TopToolBarArea,mainToolBar);
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	//QAction *createReportAction=new QAction(QIcon(":/pdf.png"),tr("Report"),this);
	//createReportAction->setToolTip(tr("Creates a pdf file report of the warehouse contents.\nCtrl+R"));
	//createReportAction->setStatusTip(tr("Creates a pdf file report of the warehouse contents."));
	//createReportAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_R));
	//connect(createReportAction,SIGNAL(triggered()),this,SLOT(createReportActionSlot()));

	showPincer1DlgBtn=new QPushButton(QIcon(":/crane.png"),tr("Pincer 1"),this);
	showPincer1DlgBtn->setToolTip(tr("Shows Pincer 1 dialog."));
	showPincer1DlgBtn->setStatusTip(tr("Shows Pincer 1 dialog."));
	connect(showPincer1DlgBtn,SIGNAL(clicked()),this,SLOT(showPincer1DlgSlot()));

	showPincer1ConvDlgBtn=new QPushButton(QIcon(":/chain.png"),tr("Pincer 1 conveyors"),this);
	showPincer1ConvDlgBtn->setToolTip(tr("Shows Pincer 1 dialog."));
	showPincer1ConvDlgBtn->setStatusTip(tr("Shows Pincer 1 dialog."));
	connect(showPincer1ConvDlgBtn,SIGNAL(clicked()),this,SLOT(showPincer1ConvDlgActionDlgSlot()));
	
#ifndef sbWarehouse
	showPincer2DlgBtn=new QPushButton(QIcon(":/crane.png"),tr("Pincer 2"),this);
	showPincer2DlgBtn->setToolTip(tr("Shows Pincer 2 dialog."));
	showPincer2DlgBtn->setStatusTip(tr("Shows Pincer 2 dialog."));
	connect(showPincer2DlgBtn,SIGNAL(clicked()),this,SLOT(showPincer2DlgSlot()));

	showPincer2ConvDlgBtn=new QPushButton(QIcon(":/chain.png"),tr("Pincer 2 conveyors"),this);
	showPincer2ConvDlgBtn->setToolTip(tr("Shows Pincer 2 dialog."));
	showPincer2ConvDlgBtn->setStatusTip(tr("Shows Pincer 2 dialog."));
	connect(showPincer2ConvDlgBtn,SIGNAL(clicked()),this,SLOT(showPincer2ConvDlgActionDlgSlot()));
#endif
	showTuxServerAction=new QAction(QIcon(":/notOk.png"),tr("Plc comms"),this);
	showTuxServerAction->setToolTip(tr("Shows the plc communication table dialog.\nCtrl+P"));
	showTuxServerAction->setStatusTip(tr("Shows the plc communication table dialog."));
	showTuxServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	connect(showTuxServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));

	QAction *showUsageDialogAction=new QAction(QIcon(":/usage.png"),tr("Usage"),this);
	showUsageDialogAction->setToolTip(tr("Show crane usage.\nCtrl+U"));
	showUsageDialogAction->setStatusTip(tr("Show crane usage."));
	showUsageDialogAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
	connect(showUsageDialogAction,SIGNAL(triggered()),this,SLOT(showUsageDialogSlot()));
#ifdef sbWarehouse
	destackerSbReadyAction=new QAction(QIcon(":/apexIcon.png"),tr("SB ready"),this);
	connect(destackerSbReadyAction,SIGNAL(triggered()),this,SLOT(destackerSbReadyActionSlot()));
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Send scrap out"),"",autoCraneTuxipConnection,"pc1ToPCI",22,51,0,"","pc1ToPCI",21,3,"","","","","",buttonStyle,buttonStyleActive,tr("Send scrap out"),this);
	hmiPushButtonWidgetClass *sendScarpPtn=pushButtonsVector.last();
	sendScarpPtn->setMaximumWidth(100);
#endif
	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
	
	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);
	
	//mainToolBar->addAction(createReportAction);
	mainToolBar->addAction(showTuxServerAction);
	mainToolBar->addAction(showUsageDialogAction);
	mainToolBar->addWidget(showPincer1DlgBtn);
	mainToolBar->addWidget(showPincer1ConvDlgBtn);
#ifndef sbWarehouse
	mainToolBar->addWidget(showPincer2DlgBtn);
	mainToolBar->addWidget(showPincer2ConvDlgBtn);
#endif
#ifdef sbWarehouse
	mainToolBar->addAction(destackerSbReadyAction);
	mainToolBar->addWidget(sendScarpPtn);
#endif
	mainToolBar->addSeparator();
	mainToolBar->addWidget(messageLabel=new QLabel());
	mainToolBar->addSeparator();
	mainToolBar->addWidget(remoteBackupCheckLabel=new QLabel());
	mainToolBar->addWidget(serialPortCheckLabel=new QLabel());

	QWidget *spacerWdgt=new QWidget();
	spacerWdgt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	mainToolBar->addWidget(spacerWdgt);
	QPushButton *hmiDlgBasicButton=new QPushButton(QIcon(":/androidRed.png"),tr("Basic HMI"));
	mainToolBar->addWidget(hmiDlgBasicButton);
	connect(hmiDlgBasicButton,SIGNAL(clicked()),this,SLOT(hmiDlgBasicButtonClickedSlot()));
	splash->showMessage(tr("Creating basic hmi..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	hmiDlgBasic=new hmiDlgBasicClass(sbTuxipConnection,this);
	QWidget *toolBoxWidget=new QWidget();QVBoxLayout *toolBoxWidgetLayout=new QVBoxLayout();toolBoxWidget->setLayout(toolBoxWidgetLayout);toolBoxWidgetLayout->setMargin(3);
	QToolBox *toolBox=new QToolBox(this);toolBoxWidgetLayout->addWidget(toolBox);
	toolBox->setStyleSheet("QToolBox::tab{background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #E1E1E9,stop: 0.4 #aaaacc,stop:0.5 #9999bb,stop:1.0 #D3D3D9);border-radius:5px;color:black;}"
												 "QToolBox::tab:selected{background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #E1E1E9,stop: 0.4 #aaaaDD,stop:0.5 #9999D8,stop:1.0 #D3D3D9);color: red;}"
												);
	int index=0;
	splash->showMessage(tr("Creating toolbox..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	toolBox->addItem(autoWidget,QIcon(":/etemIcon.png"),tr("Automatic missions"));
	toolBox->widget(index)->setToolTip(tr("Enables automatic missions"));

	index++;
	splash->showMessage(tr("Exports gui..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	exportFront=new exportStationWidgetClass(exportsFrontTable,tr("Exports to destacker"),toolBox,index,NULL,true,this);
	index++;
	toolBox->addItem(exportFront,QIcon(":/export.png"),tr("Exports to destacker"));
	exportTriple1=new exportStationWidgetClass(exportsTriple1Table,tr("Exports to triple west"),toolBox,index,NULL,true,this);
	index++;
	toolBox->addItem(exportTriple1,QIcon(":/export.png"),tr("Exports to triple west"));
	exportTriple2=new exportStationWidgetClass(exportsTriple2Table,tr("Exports to triple east"),toolBox,index,NULL,true,this);
	index++;
	toolBox->addItem(exportTriple2,QIcon(":/export.png"),tr("Exports to triple east"));
	index++;
	exportDouble=new exportStationWidgetClass(exportsDoubleTable,tr("Exports to double"),toolBox,index,NULL,true,this);
	toolBox->addItem(exportDouble,QIcon(":/export.png"),tr("Exports to double"));

	splash->showMessage(tr("Creating search dlg..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	searchWidgetClass *searchWidget=new searchWidgetClass(allPos,this);
	toolBox->addItem(searchWidget,QIcon(":/search.png"),tr("Search"));
	connect(searchWidget,SIGNAL(posSelectedSignal(int)),this,SLOT(zoomToPositionSlot(int)));
	toolBox->widget(index)->setToolTip(tr("Search the warehouse."));
	//
	QWidget *mainWidget=new QWidget();
	setCentralWidget(mainWidget);
	QVBoxLayout *mainL=new QVBoxLayout();mainWidget->setLayout(mainL);mainL->setMargin(0);
	QSplitter *centralSplitter=new QSplitter(Qt::Horizontal);
	centralSplitter->setStyleSheet(splitterHorizontalStyle);
	mainL->addWidget(mainToolBar);
	mainL->addWidget(centralSplitter);
	//
	connect(exportFront,SIGNAL(listChangedSignal()),crane,SLOT(missionsTimerSlot()));
	connect(exportTriple1,SIGNAL(listChangedSignal()),crane,SLOT(missionsTimerSlot()));
	connect(exportTriple2,SIGNAL(listChangedSignal()),crane,SLOT(missionsTimerSlot()));
	connect(exportDouble,SIGNAL(listChangedSignal()),crane,SLOT(missionsTimerSlot()));
	//statusBar();
	centralSplitter->addWidget(toolBoxWidget);
	centralSplitter->addWidget(layoutGraphicsView);
	centralSplitter->addWidget(crane->getCraneWidget());
	centralSplitter->setSizes(QList<int>()<<300<<1850-600<<300);
	//
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer->start(1000);
	QTimer *statisticsTimer=new QTimer(this);
	connect(statisticsTimer,SIGNAL(timeout()),this,SLOT(statisticsTimerSlot()));
	statisticsTimer->start(30000);
	QTimer *backupTimer=new QTimer(this);
	connect(backupTimer,SIGNAL(timeout()),this,SLOT(backupTimerSlot()));
	//backupTimer->start(60*60*1000);
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	splash->showMessage(tr("Creating hand shake..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));

	handShakeSbCrane=new handShakeClass(sbTuxipConnection,sbWarehouseIp,"",-1,"bRobotToPcI",26,2000,handShakeClass::typeReadOnly);
	connect(handShakeSbCrane,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeSbCraneStateChangedSlot(bool)));

	handShakeCrane2=new handShakeClass(tuxipServer->getTuxipClassWithIp(crane2Ip),crane2Ip,"",-1,"toPcI",26,2000,handShakeClass::typeReadOnly);
	connect(handShakeCrane2,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeCrane2ChangedSlot(bool)));

	splash->showMessage(tr("Checking for open missions..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	crane->checkForNextMissionInDatabase();
	crane->startMissions();
	//
	splash->showMessage(tr("Clearing backups..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	clearBackups(backupDir);
	//
	splash->showMessage(tr("Creating DTM qui..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	dtmDlg=new QDialog(this);
	dtmDlg->setWindowIcon(QIcon(":/assimakis.png"));
	dtmDlg->setWindowTitle(tr("DTM"));
	dtmDlg->resize(800,900);
	QVBoxLayout *dtmDlgLayout=new QVBoxLayout();
	dtmDlg->setLayout(dtmDlgLayout);
	QTabWidget *dtmDlgTabWidget=new QTabWidget();
	dtmDlgLayout->addWidget(dtmDlgTabWidget);
	for (int i=0;i<logWidgets.count();i++)
		dtmDlgTabWidget->addTab(logWidgets[i],logWidgets[i]->getDisplayText());
	splash->showMessage(tr("Reading data from plc..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxipServer->setIsAutoUpdatingFromPlc(true);
	splash->showMessage(tr("Remote backup..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
#ifndef inMyLaptop
	QSqlDatabase  remoteDb=QSqlDatabase::addDatabase("QMYSQL",remoteBackupDb);
	remoteDb.setHostName("192.168.105.105");
	remoteDb.setUserName("user");
	remoteDb.setPassword("1234");
	isRemoteBackupServerOnline=false;
	if (!remoteDb.open())
		showRemoteBackupError(remoteDb.lastError().text());
	else
	{
		isRemoteBackupServerOnline=true;
		QString errorMessage;
		noVitalExecQuery(QString("use %1;").arg(mainDataBaseName),remoteBackupDb,&errorMessage);
		showRemoteBackupError(errorMessage);
		onCheckRemoteBackupTimerSlot();
		QTimer *checkRemoteBackupTimer=new QTimer(this);
		connect(checkRemoteBackupTimer,SIGNAL(timeout()),this,SLOT(onCheckRemoteBackupTimerSlot()));
		checkRemoteBackupTimer->start(5*60*60*1000);
	}
#endif
#ifdef sbWarehouse
	serialPort=new QSerialPort(this);
	serialPort->setPortName("COM3");
	serialPort->setBaudRate(QSerialPort::Baud9600);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);
	serialPort->setParity(QSerialPort::NoParity);    
	serialPort->setDataBits(QSerialPort::Data8);   
	serialPort->setStopBits(QSerialPort::OneStop);
	if (!serialPort->open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(this, tr("Error"),tr("Error opening serial port %1.\nError:%2").arg(serialPort->portName()).arg(serialPort->errorString()));
	}
	connect(serialPort,SIGNAL(readyRead()),this,SLOT(readyReadSerialPortSlot()));
	serialPortNotReadingTimer=new QTimer(this);
	connect(serialPortNotReadingTimer,SIGNAL(timeout()),this,SLOT(serialPortNotReadingTimerSlot()));
	serialPortNotReadingTimer->start(2000);

	QTimer *checkTicketWeightTimer=new QTimer(this);
	connect(checkTicketWeightTimer,SIGNAL(timeout()),this,SLOT(checkTicketWeightTimerSlot()));
	checkTicketWeightTimer->start(5000);
#endif
	//email
	if (!QFileInfo("./sbWarningSettings.txt").exists())
	{
		QFile file("sbWarningSettings.txt");
		if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
			exit(0);
		QTextStream out(&file);
		out<<"10\n";
		out<<"15\n";
		out<<"csilva@apexextrusions.ca\n";
		out<<"jzamora@apexextrusions.ca\n";
		out<<"cdesrocher@apexextrusions.ca\n";
		//		out<<"tdasilva@apexextrusions.ca\n";
		file.close();
	}
	QFile file("sbWarningSettings.txt");
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
		exit(0);
	QTextStream in(&file);
	in>>emptySbWarningLimit;
	if (emptySbWarningLimit==0)
		emptySbWarningLimit=10;
	in>>emptySbOkLimit;
	if (emptySbOkLimit==0)
		emptySbOkLimit=15;
	while (!in.atEnd())
	{
		QString email;
		in>>email;
		if (email.contains('@'))
			sbEmailList<<email;
	}
	file.close();
	//QString subject=tr("exe started : I have %1 empty shipping baskets").arg(getFirst(QString("select count(tId) from pos where posId<%1 and locked=0 and sbNumber<>0 and ticket=0").arg(craneId),mDb).toInt());
	//QString emailText=tr("You will get an email if emty shipping baskets become less than %1 and when they become more than %2").arg(emptySbWarningLimit).arg(emptySbOkLimit);
	//sendEmptySbEmail(subject,emailText);
	//
	statisticsTimerSlot();
	splash->showMessage(tr("Starting..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
}
mainWindowClass::~mainWindowClass()
{
	delete splash;
	delete handShakeSbCrane;
	delete crane;
	log("programm ended",infoStr);
}
void mainWindowClass::readyReadSerialPortSlot()
{
	serialPortStr+=QString(serialPort->readAll());
	int startIndex=serialPortStr.indexOf('\x2');
	if (startIndex<0)
		return;
	int endIndex=0;
	QString str;
	bool found=false;
	for (int i=startIndex+1;i<serialPortStr.count();i++)
	{
		if (serialPortStr.at(i)=='\r')
		{
			found=true;
			break;
		}
		str+=serialPortStr.at(i);
	}
	if (found&&(!str.isEmpty()))
	{
		str.remove('K').remove('G').remove('Z').remove(' ');
		scaleReading=str.toFloat();
		serialPortCheckLabel->setText(tr("%1 Kgr").arg(scaleReading));
		serialPortStr.clear();
		serialPortNotReadingTimer->start(2000);
	}
}
void mainWindowClass::serialPortNotReadingTimerSlot()
{
	scaleReading=-1;
	serialPortCheckLabel->setText(tr("no weight"));
	serialPortStr.clear();
}
void mainWindowClass::checkTicketWeightTimerSlot()
{
	int ticket=tuxipServer->readIntegerValue("posTicket",4,crane2Ip);
	if ((ticket!=0)&&(ticketInScaleStation!=ticket))
	{
		ticketInScaleStation=ticket;
		if (scaleReading>200.)
		{
			int tId=getFirst(QString("select tId from tickets where ticket=%1").arg(ticketInScaleStation),mDb).toInt();
			if (tId!=0)
				execQuery(QString("update tickets set weight=%1,date='%2' where tId=%3").arg(scaleReading).arg(timeForLog).arg(tId),mDb);
			else
				execQuery(QString("insert into tickets (ticket,weight,date) values(%1,%2,'%3')").arg(ticketInScaleStation).arg(scaleReading).arg(timeForLog),mDb);
			allPos->getPosWithId(conveyor__4_id)->loadFromDatabase();
			tuxipServer->writeBit("rConveyorState",4,24,true,crane2Ip);
		}
	}
	tuxipServer->writeInteger("pc1ToPCI",8,(scaleReading>0)?scaleReading:0,crane2Ip);
}

void mainWindowClass::onCheckRemoteBackupTimerSlot()
{
	if (!isRemoteBackupServerOnline)
		return;
	QString errorMessage;
	QString lastLogDateStr=getFirst(QString("select max(time) from logV"),mDb).toString();
	QString remoteLastLogDateStr=noVitalGetFirst(QString("select max(time) from logV"),remoteBackupDb,&errorMessage).toString();

	QDateTime lastLogDate=QDateTime::fromString(lastLogDateStr,"yyyy/MM/dd hh:mm:ss");
	QDateTime remoteLastLogDate=QDateTime::fromString(remoteLastLogDateStr,"yyyy/MM/dd hh:mm:ss");
	showRemoteBackupError(errorMessage);
	if (lastLogDate<(remoteLastLogDate.addSecs(60)))
	{
		remoteBackupCheckLabel->setStyleSheet(styleLabelG);
		remoteBackupCheckLabel->setText(tr("backup ok"));
		remoteBackupCheckLabel->setToolTip(tr("Last backup:%1\nChecked:%2").arg(remoteLastLogDateStr).arg(timeForLog));
	}
	else if (lastLogDate<(remoteLastLogDate.addSecs(3600)))
	{
		remoteBackupCheckLabel->setStyleSheet(styleLabelO);
		remoteBackupCheckLabel->setText(tr("backup ok"));
		remoteBackupCheckLabel->setToolTip(tr("Last backup:%1\nChecked:%2").arg(remoteLastLogDateStr).arg(timeForLog));
	}
	else
	{
		remoteBackupCheckLabel->setStyleSheet(styleLabelR);
		remoteBackupCheckLabel->setText(tr("backup problem"));
		remoteBackupCheckLabel->setToolTip(tr("Last backup:%1\nChecked:%2").arg(remoteLastLogDateStr).arg(timeForLog));
	}
}
void mainWindowClass::showRemoteBackupError(QString message)
{
	if (message.length()>0)
	{
		remoteBackupCheckLabel->setStyleSheet(styleLabelR);
		remoteBackupCheckLabel->setText(tr("backup error"));
		remoteBackupCheckLabel->setToolTip(message);
	}
	else
	{
		remoteBackupCheckLabel->setStyleSheet(styleLabelG);
		remoteBackupCheckLabel->setText(tr("backup ok"));
		remoteBackupCheckLabel->setToolTip(tr("Last update:\n%1").arg(timeVerbose));
	}
}
void mainWindowClass::setPlcComments()
{
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF", 0,tr("Horizontal position of crane in mm."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF", 1,tr("Vertical position of crane in mm."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF", 2,tr("Horizontal target of crane in mm."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF", 3,tr("Vertical target of crane in mm."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF", 4,tr("Hooks on A position in mm."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF", 5,tr("Hooks on B position in mm."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF", 6,tr("Hooks on A target in mm."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF", 7,tr("Hooks on B target in mm."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF", 8,tr("Horizontal Hz."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF", 9,tr("Horizontal velocity in m/min."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF",10,tr("Vertical Hz."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF",11,tr("Vertical velocity in m/min."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF",12,tr("Hooks on A Hz."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF",14,tr("Hooks on B Hz."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF",15,tr("Hooks on B velocity in m/min."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcF",17,tr("Hooks on A velocity in m/min."));

	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",0,tr("Next mission on A: from position."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",1,tr("Next mission on A: to position."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",2,tr("Next mission on A\nsingle mission:1\ndouble mission:2."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",3,tr("Next mission on B: from position."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",4,tr("Next mission on B: to position."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",5,tr("Next mission on B\nsingle mission:1\ndouble mission:2."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",6,tr("Conveyor A next missions cassette is empty"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",7,tr("Conveyor B next missions cassette is empty"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",10,tr("Active mission on A: from position."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",11,tr("Active mission on A: to position."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",12,tr("Active mission on A:PC status"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",13,tr("Active mission on A:PLC status"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",14,tr("Active mission on B: from position."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",15,tr("Active mission on B: to position."));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",16,tr("Active mission on B:PC status"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",17,tr("Active mission on B:PLC status"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",18,tr("Active mission on A:PLC current step"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",19,tr("Active mission on B:PLC current step"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",22,tr("Commands from PC to PLC"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",23,tr("Status bits"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",24,tr("Status bits"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",26,tr("PLC hand shake"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",30,tr("Next mision on A: From position x correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",31,tr("Next mision on A: From position y correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",32,tr("Next mision on A: From position z correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",33,tr("Next mision on A: To position x correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",34,tr("Next mision on A: To position y correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",35,tr("Next mision on A: To position z correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",36,tr("Next mision on B: From position x correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",37,tr("Next mision on B: From position y correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",38,tr("Next mision on B: From position z correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",39,tr("Next mision on B: To position x correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",40,tr("Next mision on B: To position y correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",41,tr("Next mision on B: To position z correction"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",42,tr("Alarms"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",43,tr("Alarms"));
	//tuxipServer->setVarComment(sbWarehouseIp,"bRobotToPcI",43,tr("Warnings"));

	//tuxipServer->setBitComment(sbWarehouseIp,"bRobotToPcI",21,21,tr("Crane is ready for missions."));
	//tuxipServer->setBitComment(sbWarehouseIp,"bRobotToPcI",24,0,tr("Crane in auto mode."));
	//tuxipServer->setBitComment(sbWarehouseIp,"bRobotToPcI",24,1,tr("Crane in manual mode."));
}
void mainWindowClass::handShakeSbCraneStateChangedSlot(bool s)
{
	handShakeSbCommsOk=s;
	bool isOk=handShakeSbCommsOk&&handShakeCrane2CommsOk;
#ifndef noMission
	crane->setIsPlcConnected(isOk);
#endif
	showTuxServerAction->setIcon(QIcon(isOk?":/ok.png":":/notOk.png"));
}
void mainWindowClass::handShakeCrane2ChangedSlot(bool s)
{
	handShakeCrane2CommsOk=s;
	bool isOk=handShakeCrane2CommsOk&&handShakeSbCommsOk;
	showTuxServerAction->setIcon(QIcon(isOk?":/ok.png":":/notOk.png"));
}
void mainWindowClass::showUsageDialogSlot()
{
	usageDialogClass *usage=new usageDialogClass(this);
	usage->exec();
	delete usage;
}
void mainWindowClass::destackerSbReadyActionSlot()
{
	allPos->getPosWithId(pincer1_50_id)->showPositionDlg(false);;
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
		if (bakFilesList[i].left(backupFilePrefix.length())!=backupFilePrefix)
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
QString lastBackupFileName;
QString mainWindowClass::takeDatabaseBackup(QString fName)
{
#ifdef inMyLaptop
	return QString();
#endif
	QDir::current().mkdir("databaseBackups");
	QString backupFile,backupFileAbsPath;
	//backupDir=QDir::current().absolutePath()+"/databaseBackups/";
	backupFile=QString("%1.%2.sql").arg(fName).arg(timeForFile);
	backupFileAbsPath=backupDir+backupFile;
	QString cmd=QString("\"C:\\Program Files\\MySQL\\MySQL Server 5.7\\bin\\mysqldump\"");
	QStringList args;args<<"-uroot"<<"-p1234"<<"--databases"<<mainDataBaseName<<">"<<QString("\"%1\"").arg("C:\\ProgramData\\MySQL\\MySQL Server 5.7\\Data\\aa.sql");
	cmd=QString("\"C:\\Program Files\\MySQL\\MySQL Server 5.7\\bin\\mysqldump.exe\" -uroot -p1234 --databases %1 > \"%2\"").arg(mainDataBaseName).arg(backupFileAbsPath);
	QFile file("cmd.bat");
	if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
		exit(0);
	QTextStream out(&file);
	out<<cmd+"\n";
	file.close();
	QProcess *backupDataProcess=new QProcess(this);
	backupDataProcess->start("cmd.bat");	
	backupDataProcess->waitForFinished();
	if (JlCompress::compressFile(backupFileAbsPath+".rar",backupFileAbsPath))
		return backupFileAbsPath;
	else
		return QString();
}
void mainWindowClass::dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value)
{
	allPos->dataInPlcChanged(ip,table,index,value);
	if (ip==sbWarehouseIp)
	{
#ifndef noMission
		crane->dataInPlcChanged(ip,table,index,value);
#endif
		handShakeSbCrane->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<logWidgets.count();i++)
			logWidgets[i]->dataInPlcChanged(ip,table,index,value.toInt());
		hmiDlgBasic->dataInPlcChanged(ip,table,index,value);

#ifndef sbWarehouse
		pincer2Conv->dataInPlcChanged(ip,table,index,value);
		pincer2->dataInPlcChanged(ip,table,index,value);
		if ((table=="PC2_PCcomDint")&&(index==6))
			showPincer2DlgBtn->setStyleSheet(getValueOfBit(value.toInt(),29)?buttonStyleRed:buttonStyleActive);
		if ((table=="rConveyorState")&&(index==8))
			showPincer2ConvDlgBtn->setStyleSheet(getValueOfBit(value.toInt(),8)?buttonStyleActive:buttonStyleRed);
#endif
	}
	if (ip==crane2Ip)
	{
		handShakeCrane2->dataInPlcChanged(ip,table,index,value);
		pincer1->dataInPlcChanged(ip,table,index,value);
		pincer1->dataInPlcChanged(ip,table,index,value);
		pincer1Conv->dataInPlcChanged(ip,table,index,value);
		if ( (table=="posTicket")&&(index==4) )
			checkTicketWeightTimerSlot();
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		if ((table=="pc1ToPCI")&&(index==24))
			showPincer1DlgBtn->setStyleSheet(getValueOfBit(value.toInt(),2)?buttonStyleRed:buttonStyleActive);
		if ((table=="rConveyorState")&&(index==1))
			showPincer1ConvDlgBtn->setStyleSheet(getValueOfBit(value.toInt(),6)?buttonStyleActive:buttonStyleRed);
	}
}
void mainWindowClass::showServerSlot()
{
	if (getPassword("Password is needed to open this window.","Please give the password",passStrong))
		tuxipServer->showDialog();
}
void mainWindowClass::showPincer1DlgSlot()
{
	pincer1Dlg->exec();
}
void mainWindowClass::showPincer1ConvDlgActionDlgSlot()
{
	pincer1ConvDlg->exec();
}
void mainWindowClass::showPincer2DlgSlot()
{
#ifndef sbWarehouse
	pincer2Dlg->exec();
#endif
}
void mainWindowClass::showPincer2ConvDlgActionDlgSlot()
{
#ifndef sbWarehouse
	pincer2ConvDlg->exec();
#endif
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
	QMessageBox::aboutQt(this,tr("apes SB warehouse %1").arg(uCompilerData));
}
void mainWindowClass::stopSlot()
{
	tuxipServer->writeInteger("bRobotToPcI",22,226,sbWarehouseIp);
	tuxipServer->writeInteger("bRobotToPcI",22,0,sbWarehouseIp);
}
void mainWindowClass::resetGraphicsViewZoom()
{
	layoutScene->setSceneRect(layoutScene->itemsBoundingRect());
	layoutGraphicsView->fitInView(layoutGraphicsView->sceneRect(),Qt::KeepAspectRatio);
}
void mainWindowClass::zoomToPositionSlot(int posId)
{
	if (posId==0)
		resetGraphicsViewZoom();
	else
	{
		posClass *p=allPos->getPosWithId(posId,false);
		if (p)
		{
			QPointF pos=p->getGPos();
			float d=2000.f;
			layoutGraphicsView->fitInView(pos.x()-0.5*d,pos.y()-0.5*d,d,d,Qt::KeepAspectRatio);
		}
		else
			resetGraphicsViewZoom();
	}
}
void mainWindowClass::mainTimerSlot()
{
	setWindowTitle(tr("apex SBwarehouse %1 %2","main window title..").arg(timeVerbose).arg(adminMode?tr("(Administrator mode)"):""));
	QDir bakFilesDir=backupDir;
	QStringList bakFilesList=bakFilesDir.entryList(QStringList()<<"*.rar",QDir::Files,QDir::Name);
	for (int i=0;i<bakFilesList.count();i++)
	{
		smtpClass *m=new smtpClass(backupMessage.left(20)+" "+bakFilesList[i],backupMessage,this,"gvoudouris@yahoo.com","ultrasharp","packetWarehouse@apexSB.gr","apexSB packetWarehouse"
			,QStringList()<<"gvoudouris@yahoo.com",QStringList()<<"george@yahoo",QStringList()<<backupDir+bakFilesList[i]);
		delete m;
		QDir("").rename(backupDir+bakFilesList[i],backupDir+bakFilesList[i]+".send");
	}
}
void mainWindowClass::statisticsTimerSlot()
{
	int emptySb=getFirst(QString("select count(tId) from pos where posId<%1 and locked=0 and sbNumber<>0 and ticket=0").arg(craneId),mDb).toInt();
	QString txt;
	txt+=tr("Total:%1 ").arg(getFirst(QString("select count(tId) from pos where posId<%1").arg(craneId),mDb).toInt()-5);//triple and double exots
	txt+=tr("Available:%1 ").arg(getFirst(QString("select count(tId) from pos where posId<%1 and locked=0").arg(craneId),mDb).toInt()-5);
	txt+=tr("Empty cassettes:%1 ").arg(getFirst(QString("select count(tId) from pos where posId<%1 and locked=0 and sbNumber=0 and cassette<>0").arg(craneId),mDb).toInt());
	txt+=tr("Empty SB:%1 ")       .arg(emptySb);
	txt+=tr("Number of tickets:%1 ").arg(getFirst(QString("select count(tId) from pos where posId<%1 and ticket<>0").arg(craneId),mDb).toInt());
	globalStatusBar->showMessage(txt);

	QString messStr;
	QVector<QVector<QVariant>> doubleCassettesVV=execTableQuery(QString("select cassette,count(cassette) from pos where cassette<>0 group by cassette having count(cassette)>1"),mDb);
	for (int i=0;i<doubleCassettesVV.count();i++)
	{
		int cassette=doubleCassettesVV[i][0].toInt();
		QVector<QVector<QVariant>> doubleCassettesVV=execTableQuery(QString("select posId,description from pos where cassette=%1").arg(cassette),mDb);
		messStr+=tr("Cassette %1 is in positions:").arg(cassette);
		for (int j=0;j<doubleCassettesVV.count();j++)
			messStr+=tr("%1(%2) ").arg(doubleCassettesVV[j][0].toInt()).arg(doubleCassettesVV[j][1].toString().trimmed());
	}
	QVector<QVector<QVariant>> doubleTicketVV=execTableQuery(QString("select ticket,count(ticket) from pos where ticket<>0 group by ticket having count(ticket)>1"),mDb);
	for (int i=0;i<doubleTicketVV.count();i++)
	{
		int ticket=doubleTicketVV[i][0].toInt();
		QVector<QVector<QVariant>> doubleticketsVV=execTableQuery(QString("select posId,description from pos where ticket=%1").arg(ticket),mDb);
		messStr+=tr("Ticket %1 is in positions:").arg(ticket);
		for (int j=0;j<doubleticketsVV.count();j++)
			messStr+=tr("%1(%2) ").arg(doubleticketsVV[j][0].toInt()).arg(doubleticketsVV[j][1].toString().trimmed());
	}
	QVector<QVector<QVariant>> noSbTicketVV=execTableQuery(QString("select posId,description from pos where ticket<>0 and sbNumber=0"),mDb);
	for (int i=0;i<noSbTicketVV.count();i++)
		messStr+=tr("Position %1(%2) has ticket and does not have a SB ").arg(noSbTicketVV[i][0].toInt()).arg(noSbTicketVV[i][1].toString().trimmed());
	QVector<QVector<QVariant>> noCasssetreSbVV=execTableQuery(QString("select posId,description from pos where ticket<>0 and sbNumber=0"),mDb);
	for (int i=0;i<noCasssetreSbVV.count();i++)
		messStr+=tr("Position %1(%2) has SB and does not have a cassette ").arg(noCasssetreSbVV[i][0].toInt()).arg(noCasssetreSbVV[i][1].toString().trimmed());

	if (emptySb<emptySbWarningLimit)
		messStr+=tr("You have only %1 empty SB").arg(emptySb);


	if (!messStr.isEmpty())
		messageLabel->setStyleSheet(QString(styleLabelR));
	else
	{
		messStr=tr("Ok");
		messageLabel->setStyleSheet(QString("QLabel{color:#005500;}"));
	}
	messageLabel->setText(messStr);
	///email
	if (emptySb<emptySbWarningLimit)
	{
		if (!sbAreLessThanWarningLimitEmailSend)
		{
			//QString subject=tr("I have %1 empty shipping baskets").arg(getFirst(QString("select count(tId) from pos where posId<%1 and locked=0 and sbNumber<>0 and ticket=0").arg(craneId),mDb).toInt());
			QString subject=tr("Honeycomb B ALERT!");
			QString emailText=tr("There are less than %1 empty shipping basket in honeycomb B!").arg(emptySbWarningLimit);
			sendEmptySbEmail(subject,emailText);
			sbAreLessThanWarningLimitEmailSend=true;
		}
		sbAreMoreThanOkLimitEmailSend=false;
	}
	if (emptySb>=emptySbOkLimit)
	{
		if (!sbAreMoreThanOkLimitEmailSend)
		{
			//QString subject=tr("I have %1 empty shipping baskets").arg(getFirst(QString("select count(tId) from pos where posId<%1 and locked=0 and sbNumber<>0 and ticket=0").arg(craneId),mDb).toInt());
			QString subject=tr("Honeycomb B Notification");
			QString emailText=tr("There are now %1 empty shipping basket in Honeycomb B.").arg(emptySbOkLimit);
			sendEmptySbEmail(subject,emailText);
			sbAreMoreThanOkLimitEmailSend=true;
		}
		sbAreLessThanWarningLimitEmailSend=false;
	}
}
void mainWindowClass::sendEmptySbEmail(QString subject,QString emailText)
{
	QStringList emailNames;
	for (int i=0;i<sbEmailList.count();i++)
		emailNames<<"";
	smtpClass *m=new smtpClass(subject,emailText,this,"gvoudouris@yahoo.com","ultrasharp","honeyCombB@apex.ca","Honeycomb B"
		,sbEmailList,emailNames,QStringList());
	delete m;
}
void mainWindowClass::userDatabaseBackupActionSlot()
{
	logV("user database backup",infoStr);
	backupMessage=QInputDialog::getText(this,tr("Backup message"),tr("Message"));
	QString f=takeDatabaseBackup("apexSBWarehouse");
	//QMessageBox::information(this,tr("Information"),tr("Database backup was created:\n%1").arg(f));
}
void mainWindowClass::backupTimerSlot()
{
	backupMessage="backup from apexSBWarehouse";
	takeDatabaseBackup("auto");
}
void mainWindowClass::dtmBtnSlot()
{
	for (int i=0;i<logWidgets.count();i++)
		logWidgets[i]->updateFromDataBase();
	dtmDlg->exec();
}

#include "pch.h"
#include "mW.h"
//#define inMyLaptop
#ifdef emulateCrane3
#define inMyLaptop
#endif
void createTable(QString tableName,QStringList colNamesTypes,QString connectionName)
{
	QString stmnt=QString("if not exists (select [name] from sys.tables where [name] ='%1') create table %1 (").arg(tableName);
	for (int i=0;i<colNamesTypes.length();i++)
	{
		stmnt+=colNamesTypes[i];
		if (i!=(colNamesTypes.length()-1))
			stmnt+=",";
	}
	stmnt+=")";
	execQuery(stmnt,connectionName);
};
void insertPos(int posNumber,int posIndex,int basketNumber,QString description)
{
	execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('%1',%2,%3,%4,%5,0,0)").arg(description).arg(posNumber).arg(posIndex).arg(basketNumber).arg(1),bDb);
}
mainWindowClass::mainWindowClass():QMainWindow()
{
#ifdef emulateCrane3
	QMessageBox::critical(this,"Emulate","emulateCrane3");
#endif
	resize(1920,1080);
#ifdef inMyLaptop
	isConnectedToEpicsDbG=false;
	inMyLatopG=true;
	QString basketOdbcName="local";
	QString basketOdbcUser="sa";
	QString basketOdbcPassword=QString::fromLocal8Bit(qgetenv("ASSIMAKIS_BASKETCRANE3_BASKETODBCPASSWORD"));

	QString epicsName="local";
	QString epicsUser="sa";
	QString epicsPassword=QString::fromLocal8Bit(qgetenv("ASSIMAKIS_BASKETCRANE3_EPICSPASSWORD"));
#else
	QString basketOdbcName="apexBasketCraneMainOdbc";
	QString basketOdbcUser="sa";
	QString basketOdbcPassword=QString::fromLocal8Bit(qgetenv("ASSIMAKIS_BASKETCRANE3_BASKETODBCPASSWORD"));

	QString epicsName="epics";
	QString epicsUser="BasketCrane2";
	QString epicsPassword=QString::fromLocal8Bit(qgetenv("ASSIMAKIS_BASKETCRANE3_EPICSPASSWORD"));
#endif
bool emulatePlc=false;
#ifdef inMyLaptop
	emulatePlc=true;
#endif

	splash->showMessage(tr("Starting..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	setWindowIcon(QIcon(":/assimakis.png"));
	setStyleSheet(QString("QLabel,QAction,QAction,QToolButton,QToolBar,QPushButton,QToolBox::tab,QToolBox,QCheckBox,QTabWidget,QSpinBox,QTableWidget,QTableWidgetItem,QListWidget{font-size:13px;color:#000033;}"));
	splash->showMessage(tr("Connecting to plc..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxipServer=new tuxipServerClass(this);
	tuxipClass *crane3TuxipConnection=tuxipServer->addConnection(crane3Ip,"Crane 3",emulatePlc);
	crane3TuxipConnection->addTable("CR3_PCcomDint",plcTableClass::TYPE_INT,0,75,100);// tables to appear under button "Plc comms", for Tab of Crane 3
	crane3TuxipConnection->addTable("CR3_PCcomReal",plcTableClass::TYPE_FLOAT,0,50,100);
	crane3TuxipConnection->addTable("PcCom",plcTableClass::TYPE_INT,0,9,100);
	crane3TuxipConnection->addTable("fc1ToPcI",plcTableClass::TYPE_INT,28,30,100);
//crane3TuxipConnection->addTable("bConveyorBasket",plcTableClass::TYPE_INT,0,29,100);// Keep them commented. Maybe we will need them later
	//crane3TuxipConnection->addTable("bConveyorState",plcTableClass::TYPE_INT,0,29,100);
	//crane3TuxipConnection->addTable("bConveyorData",plcTableClass::TYPE_INT,0,29,100);
	tuxipClass *crane2TuxipConnection=tuxipServer->addConnection(crane2Ip,"Crane 2",emulatePlc);
	crane2TuxipConnection->addTable("PcCom",plcTableClass::TYPE_INT,0,9,100);// tables to appear under button "Plc comms", for Tab of Crane 2
	crane2TuxipConnection->addTable("bConveyorBasket",plcTableClass::TYPE_INT,0,29,100);
	crane2TuxipConnection->addTable("bConveyorState",plcTableClass::TYPE_INT,0,29,100);
	crane2TuxipConnection->addTable("bConveyorData",plcTableClass::TYPE_INT,0,29,100);
	crane2TuxipConnection->addTable("toPcI",plcTableClass::TYPE_INT,25,27,100);
	splash->showMessage(tr("Connecting to database..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QSqlDatabase mainDb;//basket crane database
	mainDb=QSqlDatabase::addDatabase("QODBC",bDb);//Open Database Connectivity for Microsoft SQL Server
	mainDb.setDatabaseName(basketOdbcName);//odbc name
	mainDb.setUserName(basketOdbcUser);
	mainDb.setPassword(basketOdbcPassword);
	if (!mainDb.open())
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(mainDb.databaseName()).arg(mainDb.lastError().text()));
	execQuery("use basketCraneData",bDb);

	//epics
	QSqlDatabase epicsDb;
	epicsDb=QSqlDatabase::addDatabase("QODBC",eDb);
	epicsDb.setDatabaseName(epicsName);//odbc name
	epicsDb.setUserName(epicsUser);
	epicsDb.setPassword(epicsPassword);
	isConnectedToEpicsDbG=epicsDb.open();
#if !defined(emulateCrane3) && !defined(inMyLaptop)
	if (!isConnectedToEpicsDbG)
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(epicsDb.databaseName()).arg(epicsDb.lastError().text()));
#endif
	execQuery("use epics",eDb);
	//crane3 log Gui
	createTable("c3logGui",QStringList()<<"tId int primary key identity"<<QString("mess char(%1)").arg(maxMessageLength)<<QString("details char(%1)").arg(maxDescriptionLength)<<"messenger char(10)"<<"type char(10)"<<"time char(50)",bDb);
	execQuery(QString("delete from c3logGui where tId not in(select top 10000 tId from c3logGui order by tId desc)"),bDb);
	//"c3logV"
	createTable("c3logV",QStringList()<<"tId int primary key identity"<<QString("type char(%1)").arg(maxMessageLength)<<QString("description char(%1)").arg(maxDescriptionLength)<<"time char(50)",bDb);
	execQuery(QString("delete from c3logV where tId not in(select top 10000 tId from c3logV order by tId desc)"),bDb);
	//"c3log"
	createTable("c3log",QStringList()<<"tId int primary key identity"<<QString("type char(%1)").arg(maxMessageLength)<<QString("description char(%1)").arg(maxDescriptionLength)<<"time char(20)",bDb);
	execQuery(QString("delete from c3log where tId not in(select top 10000 tId from c3log order by tId desc)"),bDb);
	//"c3logPos"
	createTable("c3logPos",QStringList()<<"tId int primary key identity"<<"pos char(10)"<<"basket int"<<"info char(50)"<<"time char(20)",bDb);
	execQuery(QString("delete from c3logPos where tId not in(select top 10000 tId from c3logPos order by tId desc)"),bDb);
	//crane3MissionsTimeLog for crane 3
	createTable("crane3MissionsTimeLog",QStringList()<<"tid int primary key identity"<<"fromPos int"<<"toPos int"<<"startSec int"<<"endSec int"<<"time char(20)",bDb);
	execQuery(QString("delete from crane3MissionsTimeLog where tid not in(select top 30000 tid from crane3MissionsTimeLog order by tid desc)"),bDb);
	//"plcMessages"
	createTable("c3plcMessages",QStringList()<<"tId int primary key identity"<<"plcAddress char(15)"<<"plcIndex int"<<"plcBit int"<<"bitStatus int"<<"type char(100)"<<"description char(300)"<<"time char(50)",bDb);
	execQuery(QString("delete from c3plcMessages where tId not in(select top 10000 tId from c3plcMessages order by tId desc)"),bDb);
	//c3missions
	createTable("c3missions",QStringList()<<"tId int primary key identity"<<"fromPosNumber int"<<"fromPosIndex int"<<"toPosNumber int"<<"toPosIndex int"<<"status char(30)",bDb);
	if (getFirst(QString("select count(tId) from c3missions"),bDb).toInt()==0)
	{
		execQuery(QString("insert into c3missions (status,fromPosNumber,fromPosIndex,toPosNumber,toPosIndex) values('%1',0,0,0,0)").arg(nextStr),bDb);
		execQuery(QString("insert into c3missions (status,fromPosNumber,fromPosIndex,toPosNumber,toPosIndex) values('%1',0,0,0,0)").arg(activeStr),bDb);
	}
	createTable(crane3ExportsToCrane2,QStringList()<<"tId int primary key identity"<<"basket int unique"<<"priority int unique",bDb);
	createTable(crane3ExportsDestackerPositions1,QStringList()<<"tId int primary key identity"<<"basket int unique"<<"priority int unique",bDb);
	createTable(crane3ExportsDestackerPositions2,QStringList()<<"tId int primary key identity"<<"basket int unique"<<"priority int unique",bDb);
	if (getFirst(QString("select max(posNumber) from positions"),bDb).toInt()<crane3PlcId)
	{
		insertPos(crane3PlcId,0,0,"Crane 3");
		insertPos(destackerWorkingPos1PlcId,1,0,"Working pos 1");
		insertPos(destackerWorkingPos2PlcId,1,0,"Working pos 1");
		insertPos(bufferApos,1,0,"Buffer");
		insertPos(bufferApos,2,0,"Buffer");
		insertPos(bufferApos,3,0,"Buffer");
		insertPos(bufferBpos,1,0,"Buffer");
		insertPos(bufferBpos,2,0,"Buffer");
		insertPos(bufferBpos,3,0,"Buffer");
		insertPos(bufferCpos,1,0,"Buffer");
		insertPos(bufferCpos,2,0,"Buffer");
		insertPos(bufferCpos,3,0,"Buffer");
		insertPos(bufferCpos,4,0,"Buffer");
	}
	initializeGlobal();
	layoutGraphicsView=new myQGraphicsViewClass(true,this);
	QGraphicsScene *layoutScene=new QGraphicsScene(this);

	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr("Zoom extents"),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));
	resetGraphicsViewZoom();

	gItemClass *layoutGraphicsItem=new gItemClass(":/dxfs/layout.dxf",QPen(QColor(200,200,200)),false,false,QColor(),this);
	layoutGraphicsView->setScene(layoutScene);
	layoutGraphicsItem->setZValue(-10.);layoutScene->addItem(layoutGraphicsItem);
	layoutGraphicsView->scale(0.05,0.05);
	splash->showMessage(tr("Creating postions..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	allPos=new allPosClass(crane2TuxipConnection,crane3TuxipConnection,layoutScene,this);
	splash->showMessage(tr("Creating crane3..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	crane=new craneClass(tuxipServer,crane3TuxipConnection,allPos,layoutScene,this);

	splash->showMessage(tr("Creating Conveyors..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	conveyorsWidget=new conveyorsWidgetClass(crane2TuxipConnection,this);
	conveyorsDlg=new QDialog(this);
	conveyorsDlg->setWindowTitle("Conveyors");
	QHBoxLayout *conveyorsDlgL=new QHBoxLayout();conveyorsDlg->setLayout(conveyorsDlgL);conveyorsDlgL->setMargin(2);
	conveyorsDlgL->addWidget(conveyorsWidget);
	conveyorsDlg->resize(1800,900);

	splash->showMessage(tr("Creating exports..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	autoWidget=new autoWidgetClass(crane,this);
	exportsToCrane2Wdgt=new exportStationWidgetClass(crane3ExportsToCrane2,tr("Empty To Crane 2"),this);
	exportStationWidgetDestackerPosition1Wdgt=new exportStationWidgetClass(crane3ExportsDestackerPositions1,tr("Destacker Position 1 exports"),this);
	exportStationWidgetDestackerPosition2Wdgt=new exportStationWidgetClass(crane3ExportsDestackerPositions2,tr("Destacker Position 2 exports"),this);
	basketCrane3UserWdgt=new basketCrane3UserWdgtClass(conveyorsWidget->getPosWithPlcIndex(1),this);
	allPos->setCraneClass(crane);
	crane->setStations();
	connect(exportsToCrane2Wdgt,SIGNAL(listChangedSignal()),allPos,SLOT(exportListChangedSlot()));
	connect(exportStationWidgetDestackerPosition1Wdgt,SIGNAL(listChangedSignal()),allPos,SLOT(exportListChangedSlot()));
	connect(exportStationWidgetDestackerPosition2Wdgt,SIGNAL(listChangedSignal()),allPos,SLOT(exportListChangedSlot()));
	connect(crane->getCarriage(),SIGNAL(exportListChangedSignal()),allPos,SLOT(exportListChangedSlot()));
	connect(allPos,SIGNAL(exportListChangedSignal()),exportsToCrane2Wdgt,SLOT(dbUpdateTimerSlot()));
	connect(allPos,SIGNAL(exportListChangedSignal()),exportStationWidgetDestackerPosition1Wdgt,SLOT(dbUpdateTimerSlot()));
	connect(allPos,SIGNAL(exportListChangedSignal()),exportStationWidgetDestackerPosition2Wdgt,SLOT(dbUpdateTimerSlot()));

	destackerWidget=new destackerWidgetClass(crane3TuxipConnection,this);
	destackerDlg=new QDialog(this);
	destackerDlg->setWindowTitle("Destacker");
	QHBoxLayout *destackerDlgL=new QHBoxLayout();destackerDlg->setLayout(destackerDlgL);destackerDlgL->setMargin(2);
	destackerDlgL->addWidget(destackerWidget);
//	destackerDlg->resize(1200,800);
	connect(tuxipServer,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(dataInPlcChangedSlot(QString,QString,int,QVariant)));
	//mainToolBar
	QToolBar *mainToolBar=new QToolBar(tr("Tools"));
	mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
	addToolBar(Qt::TopToolBarArea,mainToolBar);
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	showTuxServerAction=new QAction(QIcon(":/notOk.png"),tr("Plc comms"),this);
	showTuxServerAction->setToolTip(tr("Shows the plc comunication table dialog.\nCtrl+P"));
	showTuxServerAction->setStatusTip(tr("Shows the plc comunication table dialog."));
	mainToolBar->addAction(showTuxServerAction);
	connect(showTuxServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));

	QAction *showConVDlgAction=new QAction(QIcon(":/autoDisabled.png"),tr("Show conveyors dialog"),this);
	connect(showConVDlgAction,SIGNAL(triggered()),this,SLOT(showConVDlgActionTriggeredSlot()));
	addAction(showConVDlgAction);

	QAction *showDestackerDlgAction=new QAction(QIcon(":/crane.png"),tr("Show destacker dialog"),this);
	connect(showDestackerDlgAction,SIGNAL(triggered()),this,SLOT(showDestackerDlgActionTriggeredSlot()));
	addAction(showDestackerDlgAction);

	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);

	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);

	mainToolBar->addSeparator();
	mainToolBar->addAction(showTuxServerAction);
#ifdef logMissionsTimes
	mainToolBar->addAction(showUsageDialogAction);
#endif
	mainToolBar->addAction(showConVDlgAction);
	mainToolBar->addAction(showDestackerDlgAction);
	mainToolBar->addWidget(returnDestacker1PosBasketBtn=new QPushButton(QIcon(":/return.png"),tr("Return 1")));
	mainToolBar->addWidget(returnDestacker2PosBasketBtn=new QPushButton(QIcon(":/return.png"),tr("Return 2")));
	QWidget *spacerWdgt=new QWidget();
	spacerWdgt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	mainToolBar->addWidget(spacerWdgt);
	mainToolBar->addWidget(missionsRadioButton=new QRadioButton(tr("Missions mode")));
	mainToolBar->addWidget(modifyRadioButton=new QRadioButton(tr("Modify mode")));
	missionsRadioButton->setChecked(true);

	QPushButton *hmiDlgBasicButton=new QPushButton(QIcon(":/androidRed.png"),tr("Basic HMI"));
	mainToolBar->addWidget(hmiDlgBasicButton);
	connect(hmiDlgBasicButton,SIGNAL(clicked()),this,SLOT(hmiDlgBasicButtonClickedSlot()));
	hmiDlgBasic=new hmiDlgBasicClass(tuxipServer,crane3TuxipConnection,this);
	QWidget *toolBoxWidget=new QWidget();
	QVBoxLayout *toolBoxWidgetLayout=new QVBoxLayout();
	toolBoxWidget->setLayout(toolBoxWidgetLayout);
	toolBoxWidgetLayout->setMargin(3);
	toolBox=new QToolBox(this);
	toolBoxWidgetLayout->addWidget(toolBox);
	toolBox->setStyleSheet("QToolBox::tab {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E1E1E9, stop: 0.4 #aaaacc,stop: 0.5 #9999bb, stop: 1.0 #D3D3D9);border-radius: 5px;color: black;}QToolBox::tab:selected {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E1E1E9, stop: 0.4 #aaaaDD,stop: 0.5 #9999D8, stop: 1.0 #D3D3D9);color: red;}");
	QSplitter *missionsSplitter=new QSplitter(Qt::Vertical);
	missionsSplitter->addWidget(autoWidget);
	missionsSplitter->addWidget(exportsToCrane2Wdgt);
	missionsSplitter->addWidget(exportStationWidgetDestackerPosition1Wdgt);
	missionsSplitter->addWidget(exportStationWidgetDestackerPosition2Wdgt);

	toolBox->addItem(missionsSplitter,QIcon(":/apexIcon.png"),tr("Automatic missions"));
	toolBox->addItem(basketCrane3UserWdgt,QIcon(":/autoEnabled.png"),tr("Basket on Conveyor Destination"));

	QSplitter *centralSplitter=new QSplitter(Qt::Horizontal);
	setCentralWidget(centralSplitter);
	statusBar();
	centralSplitter->addWidget(toolBoxWidget);
	centralSplitter->addWidget(layoutGraphicsView);
	centralSplitter->addWidget(crane->getCraneWidget());
	centralSplitter->setSizes(QList<int>()<<300<<1850-600<<300);

	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerTimeOutSlot()));
	mainTimer->start(1000);
	QTimer *updateGuiTimer=new QTimer(this);
	connect(updateGuiTimer,SIGNAL(timeout()),this,SLOT(updateGuiTimerTimeOutSlot()));
	updateGuiTimer->start(5000);

	splash->showMessage(tr("Reading plc tables..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	handShakeCrane3=new handShakeClass(tuxipServer->getTuxipClassWithIp(crane3Ip),crane3Ip,"",-1,"fc1ToPcI",29,2000,handShakeClass::typeReadOnly);
	connect(handShakeCrane3,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeCrane3ChangedSlot()));
	handShakeCrane2=new handShakeClass(tuxipServer->getTuxipClassWithIp(crane2Ip),crane2Ip,"",-1,"toPcI",26,2000,handShakeClass::typeReadOnly);
  connect(handShakeCrane2,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeCrane2ChangedSlot()));
	splash->showMessage(tr("Checking for open missions..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	crane->checkForNextMissionInDatabase();

	connect(returnDestacker1PosBasketBtn,SIGNAL(clicked()),this,SLOT(returnDestacker1PosBasketBtnClicked()));
	connect(returnDestacker2PosBasketBtn,SIGNAL(clicked()),this,SLOT(returnDestacker2PosBasketBtnClicked()));
	connect(missionsRadioButton,SIGNAL(clicked(bool)),this,SLOT(missionsRadioButtonClickedSlot(bool)));
	connect(modifyRadioButton,SIGNAL(clicked(bool)),this,SLOT(modifyRadioButtonClickedSlot(bool)));

	tuxipServer->setIsAutoUpdatingFromPlc(true);
}
mainWindowClass::~mainWindowClass()
{
	log("program ended",infoStr);
}
void mainWindowClass::plcCommsCheckStatus()
{
	showTuxServerAction->setIcon(QIcon((handShakeCrane2->getComsOk()&&handShakeCrane3->getComsOk())?":/ok.png":":/notOk.png"));
}
void mainWindowClass::handShakeCrane3ChangedSlot()
{
	plcCommsCheckStatus();
}
void mainWindowClass::handShakeCrane2ChangedSlot()
{
	plcCommsCheckStatus();
}
void mainWindowClass::hmiDlgBasicButtonClickedSlot()
{
	hmiDlgBasic->exec();
}
void mainWindowClass::missionsRadioButtonClickedSlot(bool s)
{
	allPos->setSelectMode(allPosClass::selectForMission);
}
void mainWindowClass::modifyRadioButtonClickedSlot(bool s)
{
	allPos->setSelectMode(allPosClass::selectToModify);
}
void mainWindowClass::returnDestacker1PosBasketBtnClicked()
{
	allPos->returnDestacker1PosBasket();
	exportStationWidgetDestackerPosition1Wdgt->updateFromDatabase();
	updateGuiTimerTimeOutSlot();
}
void mainWindowClass::returnDestacker2PosBasketBtnClicked()
{
	allPos->returnDestacker2PosBasket();
	exportStationWidgetDestackerPosition2Wdgt->updateFromDatabase();
	updateGuiTimerTimeOutSlot();
}
void mainWindowClass::dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane3Ip)
	{
		allPos->dataInPlcChanged(ip,table,index,value);
		crane->dataInPlcChanged(ip,table,index,value);
		hmiDlgBasic->dataInPlcChanged(ip,table,index,value);
		destackerWidget->dataInPlcChanged(ip,table,index,value);
		handShakeCrane3->dataInPlcChanged(ip,table,index,value);
	}
	else if (ip==crane2Ip)
	{
		allPos->dataInPlcChanged(ip,table,index,value);
		conveyorsWidget->dataInPlcChanged(ip,table,index,value);
		handShakeCrane2->dataInPlcChanged(ip,table,index,value);
	}
}
void mainWindowClass::showServerSlot()
{
	if (getPassword())
		tuxipServer->showDialog();
}
void mainWindowClass::qtAboutSlot()
{
	 QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}
void mainWindowClass::showConVDlgActionTriggeredSlot()
{
	conveyorsDlg->exec();
}
void mainWindowClass::showDestackerDlgActionTriggeredSlot()
{
	destackerDlg->exec();
}
void mainWindowClass::stopSlot()
{
	tuxipServer->writeInteger("CR3_PCcomDint",22,226,crane3Ip);
	tuxipServer->writeInteger("CR3_PCcomDint",22,0,crane3Ip);
}
void mainWindowClass::resetGraphicsViewZoom()
{
	layoutGraphicsView->fitInView(-3000.f,-1750.,24000.f,14000.f,Qt::KeepAspectRatio);
}
void mainWindowClass::updateGuiTimerTimeOutSlot()
{
	if (getFirst(QString("select count(tId) from %1 where basket in (select basketTableId from positions where posNumber=%2)").arg(crane3ExportsToCrane2).arg(destackerWorkingPos1PlcId),bDb).toInt()!=0)
		returnDestacker1PosBasketBtn->setStyleSheet(buttonStyleOrange);
	else if (allPos->getPosWithPosIndex(destackerWorkingPos1PlcId,1)->getHasBasket())
		returnDestacker1PosBasketBtn->setStyleSheet(buttonStyleActive);
	else
		returnDestacker1PosBasketBtn->setStyleSheet(buttonStyle);
	if (getFirst(QString("select count(tId) from %1 where basket in (select basketTableId from positions where posNumber=%2)").arg(crane3ExportsToCrane2).arg(destackerWorkingPos2PlcId),bDb).toInt()!=0)
		returnDestacker2PosBasketBtn->setStyleSheet(buttonStyleOrange);
	else if (allPos->getPosWithPosIndex(destackerWorkingPos2PlcId,1)->getHasBasket())
		returnDestacker2PosBasketBtn->setStyleSheet(buttonStyleActive);
	else
		returnDestacker2PosBasketBtn->setStyleSheet(buttonStyle);
}
void mainWindowClass::mainTimerTimeOutSlot()
{
	setWindowTitle(tr("apex basket crane 3.... %1","main window title..").arg(timeVerbose));
}

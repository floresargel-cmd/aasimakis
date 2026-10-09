#include "pch.h"
#include "mW.h"
extern QSplashScreen *splash;
#define inMyLaptop
#undef inMyLaptop
QString databaseServerIp;
mainWindowClass::mainWindowClass():QMainWindow()
{
	QMainWindow::resize(1900,1000);
	setWindowIcon(QIcon(":/assimakis.png"));
	///////////////////////////dataBase
	splash->showMessage(tr("Connecting to database..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC");
#ifdef inMyLaptop
	databaseServerIp="127.0.0.1";
	QString databaseName="wareHouseData";
	QString port="1433";
	QString user="sa";
	QString password=QString::fromLocal8Bit(qgetenv("ASSIMAKIS_PACKETCRANESTATION_PASSWORD"));
	QString connectString="Driver={SQL Server};";
	connectString.append(QString("Server=%1,%2;").arg(databaseServerIp).arg(port));
	connectString.append(QString("Uid=%1;").arg(user));
	connectString.append(QString("Pwd=%1;").arg(password));
	mainDb.setDatabaseName(connectString);
#else
	databaseServerIp="apexPacketCraneMainOdbc";
	mainDb.setDatabaseName("apexPacketCraneMainOdbc");
	mainDb.setUserName("sa");
	mainDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_PACKETCRANESTATION_MAINDB_PASSWORD")));
#endif
	if (!mainDb.open())
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(mainDb.databaseName()).arg(mainDb.lastError().text()));
	apexExecQuery("use wareHouseData",MAIN_DB_CONNECTION);

	QTabWidget *mainTabWidget=new QTabWidget();setCentralWidget(mainTabWidget);
	tuxipServer=new tuxipServerClass(this);
	connect(tuxipServer,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(dataInPlcChangedSlot(QString,QString,int,QVariant)));
	tuxipClass *tuxIpCrane=tuxipServer->addConnection(packetCraneIp,"packet crane");
	tuxIpCrane->addTable("ConveyorBasket",plcTableClass::TYPE_INT,30,45,200);
	tuxIpCrane->addTable("ConveyorState",plcTableClass::TYPE_INT,30,45,200);
	tuxIpCrane->addTable("ConveyorData",plcTableClass::TYPE_INT,30,45,200);
	tuxIpCrane->addTable("pcCom",plcTableClass::TYPE_INT,0,35,200);

	tuxipClass *tuxIpCrane3=tuxipServer->addConnection(crane3Ip,"crane 3");
	tuxIpCrane3->addTable("packetId",plcTableClass::TYPE_INT,0,75,100);
	tuxIpCrane3->addTable("packetLength",plcTableClass::TYPE_INT,0,75,100);

#ifdef eastStationExe
	stationEastWidget=new stationEastWidgetClass(tuxIpCrane,this);mainTabWidget->addTab(stationEastWidget,tr("Work"));
	allConvPositions=new allConvPositionsClass(tuxIpCrane,allConvPositionsClass::eastGroup,this);mainTabWidget->addTab(allConvPositions,tr("Hmi"));
#endif
#ifdef westStationExe
	stationWestWidget=new stationWestWidgetClass(tuxIpCrane,this);mainTabWidget->addTab(stationWestWidget,tr("Work"));
	allConvPositions=new allConvPositionsClass(tuxIpCrane,tuxIpCrane3,allConvPositionsClass::packingGroup,this);mainTabWidget->addTab(allConvPositions,tr("Hmi"));
#endif
	QToolBar *mainToolBar=new QToolBar(tr("Tools"));mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
	addToolBar(Qt::TopToolBarArea,mainToolBar);
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	QAction *showServerAction=new QAction(QIcon(":/plcComms.png"),"",this);
	mainToolBar->addAction(showServerAction);
	showServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	connect(showServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));

#ifdef eastStationExe
	splash->showMessage(tr("Creating Scale..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	scaleDlg=new scaleDlgClass(tuxIpCrane,this);
	QAction *showScaleDlgAction=new QAction(QIcon(":/scale.png"),tr("Scale"),this);
	showScaleDlgAction->setToolTip(tr("Shows scale dialog."));
	showScaleDlgAction->setStatusTip(tr("Shows scale dialog."));
	connect(showScaleDlgAction,SIGNAL(triggered()),this,SLOT(showScaleDlgTriggeredSlot()));
	scaleDlg->setMyAction(showScaleDlgAction);
	mainToolBar->addAction(showScaleDlgAction);
#endif
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Return North Cassette Empty"),"",tuxIpCrane,"pcCom",30,4542,0,"","ConveyorState",45,12,"","","","","",buttonStyle,buttonStyleActive,tr("Return North Cassette Empty"),this);mainToolBar->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Return North Cassette Full") ,"",tuxIpCrane,"pcCom",30,45420,0,"","ConveyorState",45,12,"","","","","",buttonStyle,buttonStyleActive,tr("Return North Cassette Full"),this);mainToolBar->addWidget(pushButtonsVector.last());
#ifdef eastStationExe
	bitVector<<new bitClass("pcCom",8,2,bitClass::TYPE_CHANGE_COLOR,tr("Mission to station 1"),tr("Mission to station 1"),styleLabelG,styleLabelGr,"","","","",this);mainToolBar->addWidget(bitVector.last());
	bitVector<<new bitClass("pcCom",8,1,bitClass::TYPE_CHANGE_COLOR,tr("Mission to station 2"),tr("Mission to station 2"),styleLabelG,styleLabelGr,"","","","",this);mainToolBar->addWidget(bitVector.last());
	bitVector<<new bitClass("pcCom",8,4,bitClass::TYPE_CHANGE_COLOR,tr("Station alarm"),tr("Station no alarm"),styleLabelR,styleLabelG,"","","","",this);mainToolBar->addWidget(bitVector.last());
#endif
	QWidget *spacer=new QWidget();spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);mainToolBar->addWidget(spacer);
	bitVector<<new bitClass("pcCom",8,0,bitClass::TYPE_CHANGE_COLOR,tr("Crane not ok"),tr("Crane ok"),styleLabelR,styleLabelG,"","","","",this);mainToolBar->addWidget(bitVector.last());
	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer->start(1000);
	tuxipServer->setIsAutoUpdatingFromPlc(true);
	splash->showMessage(tr("Starting Up..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
}
mainWindowClass::~mainWindowClass()
{
}
void mainWindowClass::mainTimerSlot()
{
#ifdef eastStationExe
	setWindowTitle(tr("Packet Station East, assimakis sa %1:%2","main window title..").arg(timeVerbose).arg(databaseServerIp));
#endif
#ifdef westStationExe
	setWindowTitle(tr("Packet Station West, assimakis sa %1:%2","main window title..").arg(timeVerbose).arg(databaseServerIp));
#endif
}
void mainWindowClass::dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value)
{
	allConvPositions->dataInPlcChanged(ip,table,index,value);
	if (stationEastWidget)
		stationEastWidget->dataInPlcChanged(ip,table,index,value);
	if (stationWestWidget)
		stationWestWidget->dataInPlcChanged(ip,table,index,value);
	for (int i=0;i<bitVector.count();i++)
		bitVector[i]->updateValue(table,index,value.toInt());
	if (scaleDlg)
		scaleDlg->dataInPlcChanged(ip,table,index,value);
	for (int i=0;i<pushButtonsVector.count();i++)
		pushButtonsVector[i]->updateValue(table,index,value.toInt());
}
void mainWindowClass::showServerSlot()
{
	if (getPassword("Password is needed to open comms window.","Please give the password to open the comms window","asim",this))
		tuxipServer->showDialog();
}
void mainWindowClass::qtAboutSlot()
{
#ifdef eastStationExe
	QMessageBox::aboutQt(this,tr("Packet Crane East Station %1").arg(uCompilerData));
#endif
#ifdef westStationExe
	QMessageBox::aboutQt(this,tr("Packet Crane West Station %1").arg(uCompilerData));
#endif
}
void mainWindowClass::showScaleDlgTriggeredSlot()
{
	scaleDlg->exec();
}

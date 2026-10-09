#include "pch.h"
#include "mW.h"
//#define inMyLaptop
QString databaseServerIp;

mainWindowClass::mainWindowClass():QMainWindow()
{
	QMainWindow::setWindowIcon(QIcon(":/assimakis.png"));
	if (strappingStationExeEnumG==strappingStationExeEnumClass::oldStrapping)
		QMainWindow::resize(1600,800);
	if (strappingStationExeEnumG==strappingStationExeEnumClass::newStrapping)
		QMainWindow::resize(1700,1000);
	splash->showMessage(tr("Connecting to database..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",pDb);
#ifdef inMyLaptop
	databaseServerIp="127.0.0.1";
	QString databaseName="wareHouseData";
	QString port="1433";
	QString user="sa";
	QString password=QString::fromLocal8Bit(qgetenv("ASSIMAKIS_PACKINGSTRAPPINGSTATIONS_PASSWORD"));
	QString connectString="Driver={SQL Server};";
	connectString.append(QString("Server=%1,%2;").arg(databaseServerIp).arg(port));
	connectString.append(QString("Uid=%1;").arg(user));
	connectString.append(QString("Pwd=%1;").arg(password));
	mainDb.setDatabaseName(connectString);
#else
	databaseServerIp="apexPacketCraneMainOdbc";
	mainDb.setDatabaseName("apexPacketCraneMainOdbc");
	mainDb.setUserName("sa");
	mainDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_PACKINGSTRAPPINGSTATIONS_MAINDB_PASSWORD")));
#endif
	if (!mainDb.open())
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(mainDb.databaseName()).arg(mainDb.lastError().text()));
	execQuery("use wareHouseData",pDb);
bool emulatePlc=false;
#ifdef inMyLaptop
	//emulatePlc=true;
#endif

	splash->showMessage(tr("Starting..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	splash->showMessage(tr("Connecting to plc..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxipServer=new tuxipServerClass(this);
	tuxipClass *forkConnection=tuxipServer->addConnection(crane3Ip,"Packing",emulatePlc);
	forkConnection->addTable("packetId",plcTableClass::TYPE_INT,0,75,100);
	forkConnection->addTable("packetLength",plcTableClass::TYPE_INT,0,75,100);
	forkConnection->addTable("packetWoods",plcTableClass::TYPE_INT,0,75,100);
	forkConnection->addTable("fc1ToPcI",plcTableClass::TYPE_INT,0,50,100);
	forkConnection->addTable("fc1ToPcF",plcTableClass::TYPE_FLOAT,0,50,100);
	forkConnection->addTable("pcCom",plcTableClass::TYPE_INT,0,9,100);
	//strapping
	if (strappingStationExeEnumG==strappingStationExeEnumClass::newStrapping)
	{
		forkConnection->addTable("N36",plcTableClass::TYPE_INT,0,49,100);
		forkConnection->addTable("F163",plcTableClass::TYPE_FLOAT,0,49,100);
	}
	tuxipClass *oldStrappingConnection=nullptr;
	if (strappingStationExeEnumG==strappingStationExeEnumClass::oldStrapping)
	{
		oldStrappingConnection=tuxipServer->addConnection(oldStrappingPlc,"Old Strapping",emulatePlc);
		oldStrappingConnection->addTable("N162",plcTableClass::TYPE_INT,0,49,100);
		oldStrappingConnection->addTable("F163",plcTableClass::TYPE_FLOAT,0,49,100);
	}
	initializeGlobalStr();
	connect(tuxipServer,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(dataInPlcChangedSlot(QString,QString,int,QVariant)));
	//mainToolBar
	QToolBar *mainToolBar=new QToolBar(tr("Tools"));
	mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
	addToolBar(Qt::TopToolBarArea,mainToolBar);
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	showTuxServerAction=new QAction(QIcon(":/notOk.png"),tr("Plc comms"),this);
	showTuxServerAction->setToolTip(tr("Shows the plc comunication table dialog.\nCtrl+P"));
	showTuxServerAction->setStatusTip(tr("Shows the plc comunication table dialog."));
	connect(showTuxServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));

	showSerialDlgAction=new QAction(QIcon(":/barcode.png"),tr("Log"),this);
	connect(showSerialDlgAction,SIGNAL(triggered()),this,SLOT(showSerialDlgSlot()));

	QAction* definePacketOutAction=new QAction(QIcon(":/picking.png"),tr("Define Packet In Exit"),this);
	connect(definePacketOutAction,SIGNAL(triggered()),this,SLOT(definePacketInExitActionTriggeredSlot()));

	if (strappingStationExeEnumG==strappingStationExeEnumClass::oldStrapping)
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Move Fork"),":/ok.png",forkConnection,"fc1ToPcI",22,34,0,"","fc1ToPcI",13,10,"","","","","",buttonStyleActive,buttonStyleO,tr("Move Fork"),this);
	if (strappingStationExeEnumG==strappingStationExeEnumClass::newStrapping)
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Move Fork"),":/ok.png",forkConnection,"fc1ToPcI",22,35,0,"","fc1ToPcI",13,11,"","","","","",buttonStyleActive,buttonStyleO,tr("Move Fork"),this);

	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);

	mainToolBar->addSeparator();
	mainToolBar->addAction(showTuxServerAction);
	mainToolBar->addAction(showSerialDlgAction);
	mainToolBar->addAction(definePacketOutAction);
	QWidget *spacerWdgt=new QWidget();spacerWdgt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);mainToolBar->addWidget(spacerWdgt);
	pushButtonsVector.last()->setMaximumWidth(150);
	mainToolBar->addWidget(pushButtonsVector.last());

	QMainWindow::setCentralWidget(stationWdgt=new stationWdgtClass(oldStrappingConnection,forkConnection,this));
	serialPortDlg=new serialPortDlgClass(forkConnection,oldStrappingConnection,readSerialPortNameFromSettingsG("ticket","COM1"),this);
	if (strappingStationExeEnumG==strappingStationExeEnumClass::newStrapping)
		serialPortDlg->setTypeEnum(serialPortDlgClass::strappingNewExit);
	if (strappingStationExeEnumG==strappingStationExeEnumClass::oldStrapping)
		serialPortDlg->setTypeEnum(serialPortDlgClass::strappingOldExit);
	statusBar();

	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerTriggeredSlot()));
	mainTimer->start(1000);

	splash->showMessage(tr("Reading plc tables..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	//exit(0);
	handShakeCrane3=new handShakeClass(tuxipServer->getTuxipClassWithIp(crane3Ip),crane3Ip,"",-1,"fc1ToPcI",29,2000,handShakeClass::typeReadOnly);
	connect(handShakeCrane3,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeChangedSlot(bool)));
	if (strappingStationExeEnumG==strappingStationExeEnumClass::oldStrapping)
	{
		handShakeOldStrapping=new handShakeClass(tuxipServer->getTuxipClassWithIp(oldStrappingPlc),oldStrappingPlc,"",-1,"N162",20,2000,handShakeClass::typeReadOnly);
		connect(handShakeOldStrapping,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeChangedSlot(bool)));
	}
	tuxipServer->setIsAutoUpdatingFromPlc(true);
}
mainWindowClass::~mainWindowClass()
{
}
void mainWindowClass::handShakeChangedSlot(bool s)
{
	bool crane3Ok=handShakeCrane3->getComsOk();
	bool oldStrappingOk=(handShakeOldStrapping?handShakeOldStrapping->getComsOk():true);
	showTuxServerAction->setIcon(QIcon((crane3Ok&&oldStrappingOk)?":/ok.png":":/notOk.png"));
}
void mainWindowClass::dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value)
{
	handShakeCrane3->dataInPlcChanged(ip,table,index,value);
	if (handShakeOldStrapping)handShakeOldStrapping->dataInPlcChanged(ip,table,index,value);
	stationWdgt->dataInPlcChanged(ip,table,index,value);
	if (ip==crane3Ip)
	{
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
	}
}
void mainWindowClass::definePacketInExitActionTriggeredSlot()
{
	stationWdgt->definePacketInExit();
}
void mainWindowClass::showServerSlot()
{
	if (getPassword())
		tuxipServer->showDialog();
}
void mainWindowClass::showSerialDlgSlot()
{
	serialPortDlg->exec();
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex Strapping station build:  %1 ").arg(uCompilerData));
}
void mainWindowClass::mainTimerTriggeredSlot()
{
	setWindowTitle(tr("%1 %2:%3").arg(stationWdgt->getName()).arg(timeVerbose).arg(databaseServerIp));
}

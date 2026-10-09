#include "pch.h"
#include "mW.h"
//#define inMyLaptop
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
QString databaseServerIp;
mainWindowClass::mainWindowClass():QMainWindow()
{
	resize(1920,1080);
	splash->showMessage(tr("Connecting to database..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",pDb);
#ifdef inMyLaptop
	databaseServerIp="127.0.0.1";
	QString databaseName="wareHouseData";
	QString port="1433";
	QString user="sa";
	QString password=QString::fromLocal8Bit(qgetenv("ASSIMAKIS_PACKINGFORK_PASSWORD"));
	QString connectString="Driver={SQL Server};";
	connectString.append(QString("Server=%1,%2;").arg(databaseServerIp).arg(port));
	connectString.append(QString("Uid=%1;").arg(user));
	connectString.append(QString("Pwd=%1;").arg(password));
	mainDb.setDatabaseName(connectString);
#else
	databaseServerIp="apexPacketCraneMainOdbc";
	mainDb.setDatabaseName("apexPacketCraneMainOdbc");
	mainDb.setUserName("sa");
	mainDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_PACKINGFORK_MAINDB_PASSWORD")));
#endif
	if (!mainDb.open())
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(mainDb.databaseName()).arg(mainDb.lastError().text()));
	execQuery("use wareHouseData",pDb);
	bool emulatePlc=false;
#ifndef inMyLaptop
	QSqlDatabase epicsDb=QSqlDatabase::addDatabase("QODBC",eDb);
	epicsDb.setDatabaseName("epics");
	epicsDb.setUserName("BasketCrane2");
	epicsDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_PACKINGFORK_EPICSDB_PASSWORD")));
	if (!epicsDb.open())
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(epicsDb.databaseName()).arg(epicsDb.lastError().text()));
	isConnectedToEpics=true;
#endif
#ifdef inMyLaptop
	//emulatePlc=true;
#endif

	splash->showMessage(tr("Starting..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	setWindowIcon(QIcon(":/assimakis.png"));
	splash->showMessage(tr("Connecting to plc..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxipServer=new tuxipServerClass(this);
	tuxipClass *forkConnection=tuxipServer->addConnection(crane3Ip,"Packing",emulatePlc);
	forkConnection->addTable("packetId",plcTableClass::TYPE_INT,0,75,100);
	forkConnection->addTable("packetLength",plcTableClass::TYPE_INT,0,75,100);
	forkConnection->addTable("packetWoods",plcTableClass::TYPE_INT,0,75,100);
	forkConnection->addTable("fc1ToPcI",plcTableClass::TYPE_INT,0,50,100);
	forkConnection->addTable("fc1ToPcF",plcTableClass::TYPE_FLOAT,0,50,100);
	forkConnection->addTable("pcCom",plcTableClass::TYPE_INT,0,9,100);
	tuxipClass *craneConnection=tuxipServer->addConnection(packetCraneIp,"Packet Crane",emulatePlc);
	craneConnection->addTable("ConveyorData",plcTableClass::TYPE_INT,0,75,100);
	craneConnection->addTable("ConveyorBasket",plcTableClass::TYPE_INT,39,42,100);
	craneConnection->addTable("ConveyorState",plcTableClass::TYPE_INT,39,42,100);
	craneConnection->addTable("pcCom",plcTableClass::TYPE_INT,0,19,100);

	//"packingForkPlcMessages"
	createTable("packingForkPlcMessages",QStringList()<<"tId int primary key identity"<<"plcAddress char(15)"<<"plcIndex int"<<"plcBit int"<<"bitStatus int"<<"type char(100)"<<"description char(300)"<<"time char(50)",pDb);
	execQuery(QString("delete from packingForkPlcMessages where tId not in (select top 10000 tId from packingForkPlcMessages order by tId desc)"),pDb);
	createTable("packingForkBarcodeScansLog",QStringList()<<"tId int primary key identity"<<"message char(50)"<<"dateStr char(20)",pDb);

	initializeGlobalF();
	rollersDlg=new rollersDlgClass(forkConnection,this);
	serialPortStation1Dlg=new serialPortDlgClass(forkConnection,nullptr,readSerialPortNameFromSettingsG("station1","COM1"),this);serialPortStation1Dlg->setTypeEnum(serialPortDlgClass::formingStation1);
	serialPortStation2Dlg=new serialPortDlgClass(forkConnection,nullptr,readSerialPortNameFromSettingsG("station2","COM2"),this);serialPortStation2Dlg->setTypeEnum(serialPortDlgClass::formingStation2);
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

	showSerialPortStation1DlgAction=new QAction(QIcon(":/barcode1.png"),tr("Log"),this);
	connect(showSerialPortStation1DlgAction,SIGNAL(triggered()),this,SLOT(showSerialPortStation1DlgSlot()));
	showSerialPortStation2DlgAction=new QAction(QIcon(":/barcode2.png"),tr("Log"),this);
	connect(showSerialPortStation2DlgAction,SIGNAL(triggered()),this,SLOT(showSerialPortStation2DlgSlot()));

	//setSerialPortForForming1Action=new QAction(QIcon(":/barcode.png"),tr("Station 1"),this);
	//setSerialPortForForming1Action->setCheckable(true);
	//connect(setSerialPortForForming1Action,SIGNAL(triggered()),this,SLOT(setSerialPortForForming1StationActionTriggeredSlot()));

	//setSerialPortForForming2Action=new QAction(QIcon(":/barcode.png"),tr("Station 2"),this);
	//setSerialPortForForming2Action->setCheckable(true);
	//connect(setSerialPortForForming2Action,SIGNAL(triggered()),this,SLOT(setSerialPortForForming2StationActionTriggeredSlot()));

	laserScanerOkProgressBar=new QProgressBar(this);
	laserScanerOkProgressBar->setFormat("Time To Reset Laser Scaner %p sec");
	laserScanerOkProgressBar->setMinimum(0);
	laserScanerOkProgressBar->setMaximum(laserScanerTimeToReset);
	laserScanerOkProgressBar->setMaximumWidth(200);
	laserScanerOkProgressBar->setAlignment(Qt::AlignCenter);


	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);

	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);

	QPushButton *hmiDlgBasicButton=new QPushButton(QIcon(":/androidRed.png"),tr("HMI"));
	connect(hmiDlgBasicButton,SIGNAL(clicked()),this,SLOT(hmiDlgButtonClickedSlot()));
	QPushButton *rollersDlgBasicButton=new QPushButton(QIcon(":/chain.png"),tr("Rollers"));
	connect(rollersDlgBasicButton,SIGNAL(clicked()),this,SLOT(rollersDlgButtonClickedSlot()));

	mainToolBar->addAction(showTuxServerAction);
	mainToolBar->addWidget(hmiDlgBasicButton);
	mainToolBar->addWidget(rollersDlgBasicButton);
	mainToolBar->addSeparator();
	mainToolBar->addAction(showSerialPortStation1DlgAction);
	mainToolBar->addAction(showSerialPortStation2DlgAction);
	//mainToolBar->addAction(setSerialPortForForming1Action);
	//mainToolBar->addAction(setSerialPortForForming2Action);
	QWidget *spacerWdgt=new QWidget();
	spacerWdgt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	mainToolBar->addWidget(spacerWdgt);
	mainToolBar->addWidget(laserScanerOkProgressBar);

	QSplitter *centralSplitter=new QSplitter(Qt::Horizontal);
	setCentralWidget(centralSplitter);

	forkPackingWdgt=new forkPackingWdgtClass(craneConnection,forkConnection,this);
	forkUserWdgt=new forkUserWdgtClass(forkPackingWdgt->getAllPosPtr(),forkPackingWdgt->getForkPtr(),forkConnection,this);

	centralSplitter->addWidget(forkUserWdgt);
	centralSplitter->addWidget(forkPackingWdgt);
	centralSplitter->setSizes(QList<int>()<<300<<1850-300);
	statusBar();

	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerTriggeredSlot()));
	mainTimer->start(1000);

	splash->showMessage(tr("Reading plc tables..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	//exit(0);
	handShakeCrane3=new handShakeClass(tuxipServer->getTuxipClassWithIp(crane3Ip),crane3Ip,"",-1,"fc1ToPcI",29,2000,handShakeClass::typeReadOnly);
	connect(handShakeCrane3,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeCrane3ChangedSlot(bool)));

	tuxipServer->setIsAutoUpdatingFromPlc(true);
}
mainWindowClass::~mainWindowClass()
{
}
void mainWindowClass::handShakeCrane3ChangedSlot(bool s)
{
	showTuxServerAction->setIcon(QIcon(s?":/ok.png":":/notOk.png"));
}
void mainWindowClass::dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane3Ip)
	{
		forkPackingWdgt->dataInPlcChanged(ip,table,index,value);
		rollersDlg->dataInPlcChanged(ip,table,index,value);
		handShakeCrane3->dataInPlcChanged(ip,table,index,value);
		forkUserWdgt->dataInPlcChanged(ip,table,index,value);
		QString color;
		if ((table=="fc1ToPcI")&&(index==35))
		{
			int progressBarValue=(value.toFloat()/1000.);
			if (progressBarValue==0)
			{
				progressBarValue=laserScanerTimeToReset;
				laserScanerOkProgressBar->setFormat(tr("In/Out Laser Scaner Ok"));
				color="green";
			}
			else
			{
				laserScanerOkProgressBar->setFormat(tr("Time To Reset Laser Scaner %1 sec").arg(progressBarValue));
				color="red";
			}
			laserScanerOkProgressBar->setStyleSheet("QProgressBar{border:2px solid grey;border-radius:5px;background:white;text-align:center;} QProgressBar::chunk{background:"+color+";width:20px;}");
			laserScanerOkProgressBar->setValue(progressBarValue);
		}
	}
	if (ip==packetCraneIp)
	{
		forkUserWdgt->dataInPlcChanged(ip,table,index,value);
	}
}
void mainWindowClass::showSerialPortStation1DlgSlot()
{
	serialPortStation1Dlg->exec();
}
void mainWindowClass::showSerialPortStation2DlgSlot()
{
	serialPortStation2Dlg->exec();
}
void mainWindowClass::setSerialPortForForming1StationActionTriggeredSlot()
{
	//serialPortDlg->setTypeEnum(serialPortDlgClass::formingStation1);
	//setSerialPortForForming1Action->setChecked(true);
	//setSerialPortForForming2Action->setChecked(false);
}
void mainWindowClass::setSerialPortForForming2StationActionTriggeredSlot()
{
	//serialPortDlg->setTypeEnum(serialPortDlgClass::formingStation2);
	//setSerialPortForForming1Action->setChecked(false);
	//setSerialPortForForming2Action->setChecked(true);
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

void mainWindowClass::stopSlot()
{
	tuxipServer->writeInteger("fc1ToPcI",22,226,crane3Ip);
	tuxipServer->writeInteger("fc1ToPcI",22,0,crane3Ip);
}
void mainWindowClass::hmiDlgButtonClickedSlot()
{
	forkPackingWdgt->showHmiDlg();
}
void mainWindowClass::rollersDlgButtonClickedSlot()
{
	rollersDlg->exec();
}
void mainWindowClass::mainTimerTriggeredSlot()
{
	setWindowTitle(tr("Packing fork %1:%2").arg(timeVerbose).arg(databaseServerIp));
}

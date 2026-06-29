#include "pch.h"
#include "mW.h"
bool adminMode=false;
//#define inMyLaptop
mainWindowClass::mainWindowClass():QMainWindow()
{
	resize(300,20);
#ifdef inMyLaptop
	QString basketOdbcName="local";
	QString basketOdbcUser="sa";
	QString basketOdbcPassword="123";
#else
	QString basketOdbcName="apexBasketCraneMainOdbc";
	QString basketOdbcUser="sa";
	QString basketOdbcPassword="123";
#endif
#ifdef inMyLaptop
	QString mySqlServer="127.0.0.1";
	QString mySqlUser="user";
	QString mySqlPassword="1234";
#else
	QString mySqlServer=sbDbIp;
	QString mySqlUser="user";
	QString mySqlPassword="1234";
#endif

	splash->showMessage(tr("Starting..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	setWindowIcon(QIcon(":/assimakis.png"));
	setStyleSheet(QString("QLabel,QAction,QAction,QToolButton,QToolBar,QPushButton,QToolBox::tab,QToolBox,QCheckBox,QTabWidget,QSpinBox,QTableWidget,QTableWidgetItem,QListWidget{font-size:13px;color:#000033;}"));
	splash->showMessage(tr("Connecting to plc..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxipServer=new tuxipServerClass(this);
	basketTuxipConnection=tuxipServer->addConnection(crane2Ip,"Crane 2");
	tuxipServer->addTable("toPcI",plcTableClass::TYPE_INT,0,75,100,crane2Ip);
	tuxipServer->addTable("bConveyorBasket",plcTableClass::TYPE_INT,0,29,100,crane2Ip);
	tuxipServer->addTable("bConveyorData",plcTableClass::TYPE_INT,0,29,100,crane2Ip);
	tuxipServer->addTable("posSb",plcTableClass::TYPE_INT,0,75,100,crane2Ip);
	tuxipServer->addTable("posTicket",plcTableClass::TYPE_INT,0,75,100,crane2Ip);
	tuxipServer->addTable("rConveyorState",plcTableClass::TYPE_INT,0,75,100,crane2Ip);
	//
	splash->showMessage(tr("Connecting to database..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	//basket crane database
	QSqlDatabase mainDb;
	mainDb=QSqlDatabase::addDatabase("QODBC",bDb);
	mainDb.setDatabaseName(basketOdbcName);//odbc name
	mainDb.setUserName(basketOdbcUser); 
	mainDb.setPassword(basketOdbcPassword);
	if (!mainDb.open()) 
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(mainDb.databaseName()).arg(mainDb.lastError().text()));
	execQuery("use basketCraneData",bDb);
	//sb database
	QSqlDatabase sbDb;
	sbDb=QSqlDatabase::addDatabase("QMYSQL",mDb);
	sbDb.setHostName(mySqlServer);
	sbDb.setUserName(mySqlUser);
	sbDb.setPassword(mySqlPassword);
	if (!sbDb.open()) 
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(sbDb.databaseName()).arg(sbDb.lastError().text()));
	execQuery(QString("create database if not exists %1").arg("apexSbWarehouse"),mDb);
	execQuery(QString("use %1").arg("apexSbWarehouse"),mDb);
	//
	initializeGlobal();
	connect(tuxipServer,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(dataInPlcChangedSlot(QString,QString,int,QVariant)));
	//mainToolBar
	QToolBar *mainToolBar=new QToolBar(tr("Tools"));mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
	addToolBar(Qt::TopToolBarArea,mainToolBar);
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	showTuxServerAction=new QAction(QIcon(":/notOk.png"),tr("Plc comms"),this);
	showTuxServerAction->setToolTip(tr("Shows the plc comunication table dialog.\nCtrl+P"));
	showTuxServerAction->setStatusTip(tr("Shows the plc comunication table dialog."));
	mainToolBar->addAction(showTuxServerAction);
	connect(showTuxServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));

	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
	
	mainToolBar->addSeparator();
	mainToolBar->addAction(showTuxServerAction);
	//
	QTabWidget *centralTabWidget=new QTabWidget();
	setCentralWidget(centralTabWidget);
	statusBar();
	//
	basketCrane=new basketCraneClass(basketTuxipConnection,this);
	centralTabWidget->addTab(basketCrane,tr("Basket crane"));
	sbWarehouse=new sbWarehouseClass(basketTuxipConnection,this);
	centralTabWidget->addTab(sbWarehouse,tr("Sb warehouse"));
	//
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer->start(1000);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	splash->showMessage(tr("Reading plc tables..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	//
	//exit(0);
	handShakeCrane2=new handShakeClass(tuxipServer->getTuxipClassWithIp(crane2Ip),crane2Ip,"",-1,"toPcI",26,2000,handShakeClass::typeReadOnly);
	connect(handShakeCrane2,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeCrane2ChangedSlot(bool)));

	tuxipServer->setIsAutoUpdatingFromPlc(true);
}
mainWindowClass::~mainWindowClass()
{
	log("programm ended",infoStr);
}
void mainWindowClass::dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane2Ip)
	{
		basketCrane->dataInPlcChanged(ip,table,index,value);
		sbWarehouse->dataInPlcChanged(ip,table,index,value);
		handShakeCrane2->dataInPlcChanged(ip,table,index,value);
	}
}
void mainWindowClass::showServerSlot()
{
	if (getPassword())
		tuxipServer->showDialog();
}
void mainWindowClass::handShakeCrane2ChangedSlot(bool s)
{
	showTuxServerAction->setIcon(QIcon(s?":/ok.png":":/notOk.png"));
}
void mainWindowClass::qtAboutSlot()
{
	 QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}
void mainWindowClass::mainTimerSlot()
{
	setWindowTitle(tr("apex basket crane 2 exit.... %1","main window title..").arg(timeVerbose));
}
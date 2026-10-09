#include "mW.h"
//#define useLocalDb
mainWindowClass::mainWindowClass()
{
	setWindowIcon(QIcon(":/assimakis16x16.png"));
	//
	initializeDatabase();
	//
	QGridLayout *mainLayout=new QGridLayout();
	setLayout(mainLayout);

	emptyBaskets=new emptyBasketsClass();mainLayout->addWidget(emptyBaskets,0,0);
	totalUnagedBaskets=new totalUnagedBasketsClass();mainLayout->addWidget(totalUnagedBaskets,1,0);
	longUnagedBaskets=new longUnagedBasketsClass();mainLayout->addWidget(longUnagedBaskets,2,0);
	availableBasketsForPackaging=new availableBasketsForPackagingClass();mainLayout->addWidget(availableBasketsForPackaging,0,1);
	basketsInOven=new basketsInOvenClass();mainLayout->addWidget(basketsInOven,1,1);
	basketsAfterOven=new basketsAfterOvenClass();mainLayout->addWidget(basketsAfterOven,2,1);
	emptyBasketsNeeded=new emptyBasketsNeededClass();mainLayout->addWidget(emptyBasketsNeeded,0,2);

	//
	QAction *showServerAction=new QAction("",this);
	addAction(showServerAction);
	showServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	connect(showServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));
	//
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimerSlot()));
	mainTimer->start(1000);
	//
	QTimer *databaseTimer=new QTimer(this);
	connect(databaseTimer,SIGNAL(timeout()),this,SLOT(onDatabaseTimerSlot()));
	databaseTimer->start(1000*10);
	//
	initializePlc();
	onDatabaseTimerSlot();
	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
}
mainWindowClass::~mainWindowClass()
{
}
void mainWindowClass::initializePlc()
{
	tuxipServer=new tuxipServerClass(this);
	tuxipClass *tuxIpConveyors=tuxipServer->addConnection(conveyorsIp,"crane");
	tuxIpConveyors->addTable("N11",plcTableClass::TYPE_INT,0,36,750);

	connect(tuxipServer,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(dataInPlcChangedSlot(QString,QString,int,QVariant)));
	tuxipServer->setIsAutoUpdatingFromPlc(true);
}
void mainWindowClass::showServerSlot()
{
	tuxipServer->showDialog();
}
void mainWindowClass::initializeDatabase()
{
#ifdef useLocalDb
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("local");//odbc name
	mainDb.setUserName("sa");
	mainDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_BASKETCRANEOVERVIEW_MAINDB_PASSWORD")));
	if (!mainDb.open())
		EXIT(tr("Error in database"),mainDb.lastError().text());
	execQuery("if not exists(select * from sys.databases where name='basketCraneData') create database basketCraneData",MAIN_DB_CONNECTION);
	execQuery("USE basketCraneData",MAIN_DB_CONNECTION);

	QSqlDatabase apexDb=QSqlDatabase::addDatabase("QODBC",APEX_DB_CONNECTION);
	apexDb.setDatabaseName("local");//odbc name
	apexDb.setUserName("sa");
	apexDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_BASKETCRANEOVERVIEW_APEXDB_PASSWORD")));
	execQuery("USE PEManager_tmp",APEX_DB_CONNECTION);

	QSqlDatabase productionPlanDb=QSqlDatabase::addDatabase("QODBC",productionPlanDbConnection);
	productionPlanDb.setDatabaseName("local");//odbc name
	productionPlanDb.setUserName("sa");
	productionPlanDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_BASKETCRANEOVERVIEW_PRODUCTIONPLANDB_PASSWORD")));
	execQuery("USE productionPlan",productionPlanDbConnection);
#else
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("apexBasketCraneMainOdbc");//odbc name
	mainDb.setUserName("sa");
	mainDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_BASKETCRANEOVERVIEW_MAINDB_PASSWORD")));
	if (!mainDb.open())
		EXIT(tr("Error in database"),mainDb.lastError().text());
	execQuery("if not exists(select * from sys.databases where name='basketCraneData') create database basketCraneData",MAIN_DB_CONNECTION);
	execQuery("USE basketCraneData",MAIN_DB_CONNECTION);

	QSqlDatabase apexDb=QSqlDatabase::addDatabase("QODBC",APEX_DB_CONNECTION);
	apexDb.setDatabaseName("apexEpicsOdbc");//odbc name
	apexDb.setUserName("peadm2");
	apexDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_BASKETCRANEOVERVIEW_APEXDB_PASSWORD")));
	execQuery("USE PEManager_tmp",APEX_DB_CONNECTION);

	QSqlDatabase productionPlanDb=QSqlDatabase::addDatabase("QODBC",productionPlanDbConnection);
	productionPlanDb.setDatabaseName("epics");//odbc name
	productionPlanDb.setUserName("peadm2");
	productionPlanDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_BASKETCRANEOVERVIEW_PRODUCTIONPLANDB_PASSWORD")));
	execQuery("USE productionPlan",productionPlanDbConnection);
#endif
}
void mainWindowClass::onMainTimerSlot()
{
	setWindowTitle(tr("apex basketCraneOverview, assimakis sa ","main window title..")+QDateTime::currentDateTime().toString("yyyy-MMMM-dd")+QTime::currentTime().toString(" hh:mm:ss"));
}
void mainWindowClass::onDatabaseTimerSlot()
{
	emptyBaskets->updateFromDatabase();
	totalUnagedBaskets->updateFromDatabase();
	longUnagedBaskets->updateFromDatabase();
	availableBasketsForPackaging->updateFromDatabase();
	basketsInOven->updateFromDatabase();
	basketsAfterOven->updateFromDatabase();
	emptyBasketsNeeded->updateFromDatabase();
}
void mainWindowClass::dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value)
{
	if ((ip==conveyorsIp)&&(table=="N11"))
	{
		basketsInOven->dataInPlcChanged(ip,table,index,value);
	}
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}

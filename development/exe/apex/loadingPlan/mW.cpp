#include "mW.h"
int *minAvailableManualOutOrder;
//#define useLocalDb
mainWindow::mainWindow()
{
	minAvailableManualOutOrder=new int;
	*minAvailableManualOutOrder=0;
	initializeDatabase();
	//
	packets=new packetsClass(this);
	options=new optionsClass();
	loadingPlan=new loadingPlanClass(packets,options,this);
	packets->setLoadingPlan(loadingPlan);
	//
	options->setForGroupOfOrders(loadingPlan->getForGroupOfOrders());
	options->setAftGroupOfOrders(loadingPlan->getAftGroupOfOrders());
	//
	QTabWidget *mainTabWidget=new QTabWidget();
	mainTabWidget->addTab(packets,tr("packets"));
	mainTabWidget->addTab(options,tr("options"));
	mainTabWidget->addTab(loadingPlan,tr("loading"));
	QVBoxLayout *mainLayout=new QVBoxLayout();
	mainLayout->addWidget(mainTabWidget);
	setLayout(mainLayout);
	//
	connect(&mainTimer, SIGNAL(timeout()), this, SLOT(onMainTimer()));
	mainTimer.start(1*1000);

	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
}
void mainWindow::initializeDatabase()
{
#ifdef useLocalDb
	QSqlDatabase epicsDadabase=QSqlDatabase::addDatabase("QODBC",eDb);
	epicsDadabase.setDatabaseName("local");//odbc name apexPacketCraneMainOdbc
	epicsDadabase.setUserName("sa");
	epicsDadabase.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_LOADINGPLAN_EPICSDADABASE_PASSWORD")));
	if (!epicsDadabase.open())
		EXIT(tr("Error in database"),epicsDadabase.lastError().text());
	execQuery("USE epics",eDb);

	QSqlDatabase whDadabase=QSqlDatabase::addDatabase("QODBC",whDb);
	whDadabase.setDatabaseName("local");//odbc name apexPacketCraneMainOdbc
	whDadabase.setUserName("sa");
	whDadabase.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_LOADINGPLAN_WHDADABASE_PASSWORD")));
	if (!whDadabase.open())
		EXIT(tr("Error in database"),whDadabase.lastError().text());
	execQuery("USE wareHouseData",whDb);
#else
	QSqlDatabase epicsDadabase=QSqlDatabase::addDatabase("QODBC",eDb);
	epicsDadabase.setDatabaseName("apexEpicsOdbc");//odbc name apexPacketCraneMainOdbc
	epicsDadabase.setUserName("reports");
	epicsDadabase.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_LOADINGPLAN_EPICSDADABASE_PASSWORD")));
	if (!epicsDadabase.open())
		EXIT(tr("Error in database"),epicsDadabase.lastError().text());
	execQuery("USE epics",eDb);

	QSqlDatabase whDadabase=QSqlDatabase::addDatabase("QODBC",whDb);
	whDadabase.setDatabaseName("apexBasketCraneMainOdbc");//odbc name apexPacketCraneMainOdbc
	whDadabase.setUserName("sa");
	whDadabase.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_LOADINGPLAN_WHDADABASE_PASSWORD")));
	if (!whDadabase.open())
		EXIT(tr("Error in database"),whDadabase.lastError().text());
	execQuery("USE wareHouseData",whDb);
#endif
}
void mainWindow::onMainTimer()
{
	this->setWindowTitle(QString("Apex truck loading  ")+QDateTime::currentDateTime().toString("yyyy/MMMM/dd")+QTime::currentTime().toString(" hh:mm:ss"));
}
void mainWindow::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}

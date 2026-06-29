#include "mW.h"
QVector<QProcess*> externalProcessesVector; 
mainWindowClass::mainWindowClass():QWidget()
{
	setWindowIcon(QIcon(":/logo_16x16.png"));
	setFocusPolicy(Qt::StrongFocus);
	//db
	//MAIN_DB_CONNECTION
#ifdef LOCAL_DB
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("local");//odbc name apexPacketCraneMainOdbc
	mainDb.setUserName("sa"); 
	mainDb.setPassword("123");
	if (!mainDb.open()) 
		EXIT(tr("Error in database"),mainDb.lastError().text());
	execQuery("if not exists(select * from sys.databases where name='productionPlan') create database productionPlan",MAIN_DB_CONNECTION);
	execQuery("USE productionPlan",MAIN_DB_CONNECTION);
#else
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("epics");//odbc name apexPacketCraneMainOdbc
	mainDb.setUserName("peadm2"); 
	mainDb.setPassword("peadm2");
	if (!mainDb.open()) 
		EXIT(tr("Error in database"),mainDb.lastError().text());
	execQuery("if not exists(select * from sys.databases where name='productionPlan') create database productionPlan",MAIN_DB_CONNECTION);
	execQuery("USE productionPlan",MAIN_DB_CONNECTION);
#endif
	//BASKET_CRANE_DB_CONNECTION
	QSqlDatabase basketCraneDb=QSqlDatabase::addDatabase("QODBC",BASKET_CRANE_DB_CONNECTION);
#ifdef LOCAL_DB
	basketCraneDb.setDatabaseName("local");//
#else
	basketCraneDb.setDatabaseName("apexBasketCraneMainOdbc");//
#endif
	basketCraneDb.setUserName("sa"); 
	basketCraneDb.setPassword("123");
	if (!basketCraneDb.open()) 
		EXIT(tr("Error in database"),basketCraneDb.lastError().text());
	execQuery("if not exists(select * from sys.databases where name='basketCraneData') create database basketCraneData",BASKET_CRANE_DB_CONNECTION);
	execQuery("USE basketCraneData",BASKET_CRANE_DB_CONNECTION);
	//
	int profileNumber=QInputDialog::getInt(this,tr("Production plan dxf"),tr("Input the profile that you want to check"),1001,1001);
//
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);mainLayout->setMargin(0);
	QSplitter *mainSlider=new QSplitter(Qt::Vertical);mainLayout->addWidget(mainSlider);
	//basketStacking
	basket=NULL;
	QWidget *basketWidget=new QWidget();
	basketLayout=new QVBoxLayout();basketWidget->setLayout(basketLayout);
	mainSlider->addWidget(basketWidget);
	profileProduction=new profileProductionClass(profileNumber);
	connect(profileProduction,SIGNAL(productionChangedSignal()),this,SLOT(productionChangedSlot()));
	mainSlider->addWidget(profileProduction);
	//
	//actions
	QAction *f5Action=new QAction("",this);
	addAction(f5Action);
	f5Action->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F5));
	connect(f5Action,SIGNAL(triggered()),this,SLOT(f5Slot()));

	//
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimerSlot();
	mainTimer.start(1000);
	//
	f5Slot();
	//
	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
}
mainWindowClass::~mainWindowClass()
{
}
//
void mainWindowClass::productionChangedSlot()
{
	basket->setNeedsToBeCalculated(true);
}
void mainWindowClass::f5Slot()
{
	if (basket)
	{
		basketLayout->removeWidget(basket);
		delete basket;
	}
	basket=new basketClass(this);basketLayout->addWidget(basket);
	basket->addProfiles(profileProduction,1000);
}
void mainWindowClass::mainTimerSlot()
{
	setWindowTitle(tr("production plan dxf apex, assimakis sa ","main window title..")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+(QTime::currentTime().addSecs(-36000)).toString(" hh:mm:ss"));
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex production plan dxf build: %1 ").arg(uCompilerData));
}
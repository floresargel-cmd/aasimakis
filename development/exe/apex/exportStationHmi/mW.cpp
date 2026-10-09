#include "mW.h"
QVector<QProcess*> externalProcessesVector;
#define LOCAL_DB
#ifdef LOCAL_DB
QString pincerIp="192.168.0.111";
QString packingIp="192.168.0.111";
#else
QString pincerIp="20.20.20.10";
QString packingIp="20.20.20.20";
#endif
mainWindowClass::mainWindowClass()
{
	setWindowIcon(QIcon(":/logo_16x16.png"));
	//////////////////////////////////
#ifdef LOCAL_DB
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("local");//odbc name apexPacketCraneMainOdbc
	mainDb.setUserName("sa");
	mainDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_EXPORTSTATIONHMI_MAINDB_PASSWORD")));
	if (!mainDb.open())
		EXIT(tr("Error in database"),mainDb.lastError().text());
	apexExecQuery("if not exists(select * from sys.databases where name='productionPlan') create database productionPlan",MAIN_DB_CONNECTION);
	execQuery("USE productionPlan",MAIN_DB_CONNECTION);
#else
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("epics");//odbc name apexPacketCraneMainOdbc
	mainDb.setUserName("peadm2");
	mainDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_EXPORTSTATIONHMI_MAINDB_PASSWORD")));
	if (!mainDb.open())
		EXIT(tr("Error in database"),mainDb.lastError().text());
	execQuery("if not exists(select * from sys.databases where name='productionPlan') create database productionPlan",MAIN_DB_CONNECTION);
	execQuery("USE productionPlan",MAIN_DB_CONNECTION);
#endif
	//////////////////////////////////
#ifdef VB_PLC_COMMS
	QWidget *pincerTcpServerWidget=new QWidget();
#ifdef EMULATE
	pincerTcpServer=new tcpServerClass(PINCER_READ_PORT,PINCER_WRITE_PORT,pincerTcpServerWidget,true,false);
#else
	pincerTcpServer=new tcpServerClass(PINCER_READ_PORT,PINCER_WRITE_PORT,pincerTcpServerWidget,false,true);
#endif
	connect(pincerTcpServer,SIGNAL(incomingSignal(QString,int,QString)),this,SLOT(signalFromPincerPlcSlot(QString,int,QString)));
#endif
#ifdef VB_PLC_COMMS
	pincerTcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,"N22",25);
	pincerTcpServer->addTableMonitor(tableMonitorClass::TYPE_DOUBLE,"F23",18);
#endif

	/////////////////////////////
#ifdef VB_PLC_COMMS
	QWidget *packingTcpServerWidget=new QWidget();
#ifdef EMULATE
	packingTcpServer=new tcpServerClass(PACKING_READ_PORT,PACKING_WRITE_PORT,packingTcpServerWidget,true,false);
#else
	packingTcpServer=new tcpServerClass(PACKING_READ_PORT+10,PACKING_WRITE_PORT+10,packingTcpServerWidget,false,true);
#endif
	connect(packingTcpServer,SIGNAL(incomingSignal(QString,int,QString)),this,SLOT(signalFromPackingPlcSlot(QString,int,QString)));
	packingTcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,"N100",40);
	packingTcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,"N163",21);
#endif
	////////////////////////////////////////////////////////
	//int fontSize=16;
	//this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px} QToolButton{font-size:12px} QListView{font-size:%1px} QHeaderView{font-size:%1px} QTableView{font-size:%1px} QLineEdit{font-size:%1px}  QDoubleSpinBox{font-size:%1px}  QSpinBox{font-size:%1px}  QRadioButton{font-size:%1px}  QComboBox{font-size:%1px} QToolTip{background-color:white;}").arg(fontSize));
	//
	tuxipServer=new tuxipServerClass(this);
	tuxipServer->addConnection(pincerIp,"Pincer");
	tuxIpPincer=tuxipServer->getTuxipClassWithIp(pincerIp);
	pincerTcpServer=new tcpServerClass(tuxIpPincer);
	tuxipServer->addConnection(packingIp,"Packing");
	tuxIpPacking=tuxipServer->getTuxipClassWithIp(packingIp);
	packingTcpServer=new tcpServerClass(tuxIpPacking);

#ifdef TUX_PLC_COMMS
	QAction *showServerAction=new QAction("",this);
	addAction(showServerAction);
	showServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	connect(showServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));
#endif
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimer()));
	mainTimer.start(1000);
#ifdef TUX_PLC_COMMS
	connect(tuxIpPincer,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromPincerTuxPlcSlot(QString,QString,int,QVariant)));
	connect(tuxIpPacking,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromPackingTuxPlcSlot(QString,QString,int,QVariant)));
	tuxIpPincer->addTable("N22",plcTableClass::TYPE_INT,0,24,750);
	tuxIpPincer->addTable("F23",plcTableClass::TYPE_FLOAT,0,17,750);
	tuxIpPacking->addTable("N100",plcTableClass::TYPE_INT,0,39,750);
	tuxIpPacking->addTable("N163",plcTableClass::TYPE_INT,0,19,750);
#endif

	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	pincer=new pincerClass(pincerTcpServer);
	conveyors=new conveyorsClass(packingTcpServer,this);
	destacker=new destackerClass(packingTcpServer,this);
	rollers=new rollersClass(packingTcpServer,this);

	QTabWidget *mainTabWidget=new QTabWidget();
	mainLayout->addWidget(mainTabWidget);
	mainTabWidget->addTab(pincer,tr("Pincer"));
	mainTabWidget->addTab(conveyors,tr("Conveyors"));
	mainTabWidget->addTab(destacker,tr("Destacker"));
	mainTabWidget->addTab(rollers,tr("Rollers"));
#ifdef VB_PLC_COMMS
	mainTabWidget->addTab(pincerTcpServerWidget,tr("Pincer comms"));
	mainTabWidget->addTab(packingTcpServerWidget,tr("Packing comms"));
	//
	QProcess *pincerProc=new QProcess();
	QStringList pincerArgs;
	pincerArgs<<pincerIp<<pincerTcpServer->getIp()<<QString().number(PINCER_WRITE_PORT)<<QString().number(PINCER_READ_PORT)<<QString().number(100)<<"N22"<<QString().number(25)<<QString().number(100)<<"F23"<<QString().number(18)<<QString().number(100);
	pincerProc->start(QDir::currentPath()+"//pincerHmi//pincerHmi.exe ",pincerArgs);
	externalProcessesVector.append(pincerProc);
	QProcess *packingProc=new QProcess();
	QStringList packingArgs;
	packingArgs<<packingIp<<packingTcpServer->getIp()<<QString().number(PACKING_WRITE_PORT+10)<<QString().number(PACKING_READ_PORT+10)<<QString().number(100)<<"N100"<<QString().number(40)<<QString().number(100)<<"N163"<<QString().number(21)<<QString().number(100);
	packingProc->start(QDir::currentPath()+"//packingHmi//packingHmi.exe ",packingArgs);
	externalProcessesVector.append(packingProc);
#endif

	tuxipServer->setIsAutoUpdatingFromPlc(true);
	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
}
mainWindowClass::~mainWindowClass()
{
#ifdef VB_PLC_COMMS
	terminateExternalProccesses();
#endif
}
void mainWindowClass::showServerSlot()
{
#ifdef TUX_PLC_COMMS
	tuxipServer->showDialog();
#endif
}
void mainWindowClass::onMainTimer()
{
	setWindowTitle(tr("hmi.  apex packing area hmi, assimakis sa ","main window title..")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss"));
}
void mainWindowClass::signalFromPincerTuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
#ifdef TUX_PLC_COMMS
	signalFromPincerPlcSlot(table,index,value.toString());
#endif
}
void mainWindowClass::signalFromPincerPlcSlot(QString table,int index,QString value)
{
	if (table=="N22")
		pincer->plcStatusChanged(index,value.toInt());
	if (table=="F23")
		pincer->plcFloatValuesChanged(index,value.toFloat());
}
void mainWindowClass::signalFromPackingTuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
#ifdef TUX_PLC_COMMS
	signalFromPackingPlcSlot(table,index,value.toString());
#endif
}
void mainWindowClass::signalFromPackingPlcSlot(QString table,int index,QString value)
{
	if (table=="N100")
	{
		if ((index<=22)||(index==30))
		conveyors->plcMemoryChanged(index,value.toInt());
	if ((index==35)||(index==37)||(index==38)||(index==30))
		rollers->plcMemoryChanged(index,value.toInt());
	if ((index>=25)&&(index<=32))
		destacker->plcMemoryChanged(index,value.toInt());
	if (index==11)
		destacker->destackerBasketChanged(value.toInt());
	}
	if (table=="N163")
	{
		destacker->spacersMemoryChanged(index,value.toInt());
	}
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}

#include "mW.h"
void createTable(QString tableName,QStringList colNames,QStringList colTypes,QString conn=MAIN_DB_CONNECTION)
{
	QString stmnt=QString("IF NOT EXISTS (SELECT [name] FROM sys.tables WHERE [name] ='")+tableName+"') ";
	stmnt+=QString("CREATE TABLE ")+tableName+" (";
	for (int i=0;i<colNames.length();i++)
	{
		stmnt+=colNames[i]+" "+colTypes[i];
		if (i!=(colNames.length()-1))
			stmnt+=",";
	}
	stmnt+=")";
	apexExecQuery(stmnt,conn);
}
QVector<QProcess*> externalProcessesVector; 
void printNumber(int c)
{
	if (c<=0)
		return;
	QPrinter printer(QPrinterInfo::defaultPrinter());
	QPainter painter(&printer);
	painter.setPen(Qt::black);
	painter.setFont(QFont("helvetica",175));
	painter.rotate(90);
	painter.drawText(50,-20,QString("%1").arg(c));
}
mainWindowClass::mainWindowClass()
{
	lastCurrentTabIndex=0;
	//printNumber(12345);
	//exit(0);
	setWindowIcon(QIcon(":/logo_16x16.png"));
	barcodeReadCom=new CSyncSerialComm("COM1");
	barcodeReadCom->Open();
	barcodeReadCom->ConfigPort(9600,50);

	/////////////////////////////
#ifdef VB_PLC_COMMS
	QWidget *strapingTcpServerWidget=new QWidget();
#ifdef EMULATE
	tcpServer=new tcpServerClass(STRAPING_READ_PORT_FOR_STRAPING_PC,STRAPING_WRITE_PORT_FOR_STRAPING_PC,strapingTcpServerWidget,true);
#else
	tcpServer=new tcpServerClass(STRAPING_READ_PORT_FOR_STRAPING_PC,STRAPING_WRITE_PORT_FOR_STRAPING_PC,strapingTcpServerWidget,false);
	connect(tcpServer,SIGNAL(incomingSignal(QString,int,QString)),this,SLOT(signalFromPlcSlot(QString,int,QString)));
#endif
#endif
	
#ifdef TUX_PLC_COMMS
	tuxipServer=new tuxipServerClass(false);
	tuxipServer->addConnection(forkIp,"fork");
	tuxipClass* tuxIp=tuxipServer->getTuxipClassWithIp(forkIp);
	connect(tuxIp,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromTuxPlcSlot(QString,QString,int,QVariant)));
	tcpServer=new tcpServerClass(tuxIp);
	//
	QAction *showServerAction=new QAction("",this);
	addAction(showServerAction);
	showServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	connect(showServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));
#endif
	////////////////////////////////////////////////////////
	//int fontSize=16;
	//this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px} QToolButton{font-size:12px} QListView{font-size:%1px} QHeaderView{font-size:%1px} QTableView{font-size:%1px} QLineEdit{font-size:%1px}  QDoubleSpinBox{font-size:%1px}  QSpinBox{font-size:%1px}  QRadioButton{font-size:%1px}  QComboBox{font-size:%1px} QToolTip{background-color:white;}").arg(fontSize));
	//
	mainTabWidget=new QTabWidget();
	connect(mainTabWidget,SIGNAL(currentChanged(int)),this,SLOT(currentTabChangedSlot(int)));
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	straping=new strapingClass(tcpServer,this);mainTabWidget->addTab(straping,tr("Straping"));
	fork=new forkClass(tcpServer,this);mainTabWidget->addTab(fork,tr("Fork lift"));
#ifdef VB_PLC_COMMS
	mainTabWidget->addTab(strapingTcpServerWidget,tr("PLC comms"));
#endif	
	mainLayout->addWidget(mainTabWidget);
#ifdef EMULATE
	strapingTcpServer->sendCurrentValuesToPlc();
#endif	
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimerSlot()));
	mainTimer.start(1000);
	connect(&barcodeReadTimer,SIGNAL(timeout()),this,SLOT(onBarcodeReadTimerSlot()));
	barcodeReadTimer.start(500);

#ifdef VB_PLC_COMMS
	tcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,"N160",23);
	tcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,"N162",10);
	tcpServer->addTableMonitor(tableMonitorClass::TYPE_DOUBLE,"F161",11);
	tcpServer->addTableMonitor(tableMonitorClass::TYPE_DOUBLE,"F163",20);
#endif
#ifdef VB_PLC_COMMS
#ifndef EMULATE
	QProcess *strapingProc=new QProcess();
	QStringList strapingArgs;
	strapingArgs<<forkIp<<tcpServer->getIp()<<QString().number(STRAPING_WRITE_PORT_FOR_STRAPING_PC)<<QString().number(STRAPING_READ_PORT_FOR_STRAPING_PC)<<QString().number(100)
		<<"N160"<<QString().number(24)<<QString().number(100)
		<<"N162"<<QString().number(10)<<QString().number(100)
		<<"F161"<<QString().number(12)<<QString().number(100)
		<<"F163"<<QString().number(20)<<QString().number(100);
	strapingProc->start(QDir::currentPath()+"//strapingHmi//strapingHmi.exe ",strapingArgs);
	externalProcessesVector.append(strapingProc);
#endif	
#endif
#ifdef TUX_PLC_COMMS
	tuxIp->addTable("N160",plcTableClass::TYPE_INT,0,23,750);
	tuxIp->addTable("N162",plcTableClass::TYPE_INT,0,9,750);
	tuxIp->addTable("F161",plcTableClass::TYPE_FLOAT,0,11,750);
	tuxIp->addTable("F163",plcTableClass::TYPE_FLOAT,0,19,750);
#endif
	///////////////////////////dataBase
	//MAIN_DB_CONNECTION
	{
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
#ifdef LOCAL_DB
	mainDb.setDatabaseName("local");//odbc name
#else
	mainDb.setDatabaseName("apexPacketCraneMainOdbc");//odbc name
#endif
	mainDb.setUserName("sa"); 
	mainDb.setPassword("123");
	if (!mainDb.open()) 
		EXIT(tr("Error in database"),mainDb.lastError().text());
	//QMessageBox::critical(0,"Fatal","connected to apexPacketCraneMainOdbc");
	execQuery("USE wareHouseData",MAIN_DB_CONNECTION);
	}
	//STRAPING_DB_CONNECTION
	{
	QSqlDatabase strapingDb=QSqlDatabase::addDatabase("QODBC",STRAPING_DB_CONNECTION);
#ifdef LOCAL_DB
	strapingDb.setDatabaseName("local");//odbc name
#else
	//QMessageBox::critical(0,"Fatal","trying to connect to apexPacketCraneMainOdbc");
	strapingDb.setDatabaseName("apexPacketCraneMainOdbc");//odbc name
#endif
	strapingDb.setUserName("sa"); 
	strapingDb.setPassword("123");
	if (!strapingDb.open()) 
		EXIT(tr("Error in database"),strapingDb.lastError().text());
	execQuery("if not exists(select * from sys.databases where name='strapingData') create database strapingData",STRAPING_DB_CONNECTION);
	execQuery("USE strapingData",STRAPING_DB_CONNECTION);
	}
#ifdef USE_STORAGE_TYPE
	//epicsConnection
	{
	QSqlDatabase epicsDb=QSqlDatabase::addDatabase("QODBC",EPICS_DB_CONNECTION);
#ifdef LOCAL_DB
	epicsDb.setDatabaseName("local");//odbc name
	epicsDb.setUserName("sa"); 
	epicsDb.setPassword("123");
#else
	epicsDb.setDatabaseName("epicsConnection");//odbc name
	epicsDb.setUserName("reports"); 
	epicsDb.setPassword("reports");
#endif
	if (!epicsDb.open()) 
		EXIT(tr("Error in database"),epicsDb.lastError().text());
	execQuery("USE epics",EPICS_DB_CONNECTION);
	}
#endif
	//positionsTable
	QStringList colNames;QStringList colTypes;
	colNames.clear();colTypes.clear();
	colNames<<"pid"											 <<"die"<<"tension";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"int"<<"int"		 ;
	createTable("tension",colNames,colTypes,STRAPING_DB_CONNECTION);
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
void mainWindowClass::currentTabChangedSlot(int index)
{
	if (lastCurrentTabIndex!=index)
	{
		if (index==1)
		{
			passwordClass *pass=new passwordClass(this);
			pass->exec();
			if (!pass->getPasswordIsCorrect())
				mainTabWidget->setCurrentIndex(lastCurrentTabIndex);
			else
				lastCurrentTabIndex=index;
			delete pass;
		}
		else
			lastCurrentTabIndex=index;
	}
}
void mainWindowClass::onBarcodeReadTimerSlot()
{
	if (straping->getDefineNewPacketDlg())
	{
		QString msg="";
		char *ppszBuf=new char[100];
		DWORD length=0;
		barcodeReadCom->Read(&ppszBuf,length);
		if (length>0)
		{
			msg=QString(ppszBuf);
			QStringList	arr=msg.split("\n");
			if (arr.count()>=1)
			{
				msg=arr.at(0);
			}
		}
		straping->getDefineNewPacketDlg()->messageFromBarcode(msg);
	}
}
void mainWindowClass::onMainTimerSlot()
{
	setWindowTitle(tr("apex straping area hmi, assimakis sa ","main window title..")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss"));
}
//for backwards compatibility
void mainWindowClass::signalFromTuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
#ifdef TUX_PLC_COMMS
	signalFromPlcSlot(table,index,value.toString());
#endif
}
void mainWindowClass::showServerSlot()
{
#ifdef TUX_PLC_COMMS
	tuxipServer->showDialog();
#endif
}
void mainWindowClass::signalFromPlcSlot(QString table,int index,QString value)
{
	if (table=="N160")
	{
		fork->plcMemoryChanged(index,value.toInt());
		if ((index==5)||(index==14))
			straping->plcMemoryN160Changed(index,value.toInt());
	}
	if (table=="N162")
	{
		straping->plcMemoryN162Changed(index,value.toInt());
	}
	if (table=="F161")
	{
		fork->plcFloatMemoryChanged(index,value.toDouble());
	}
	if (table=="F163")
	{
		straping->plcFloatMemoryChanged(index,value.toDouble());
	}
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}
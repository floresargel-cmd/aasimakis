#include "mw.h"

QVector<QProcess*> externalProcessesVector;

mainWindowClass::mainWindowClass()
{
	setWindowIcon(QIcon(":/logo_16x16.png"));

#ifdef inMyLaptop
	//MAIN_DB_CONNECTION
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC", MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("local");//odbc name
	mainDb.setUserName("sa");
	mainDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_AGINGOVENAPEXCODE_MAINDB_PASSWORD")));
	if (!mainDb.open())
		EXIT(tr("Error in database"),mainDb.lastError().text());
	execQuery("USE basketCraneData", MAIN_DB_CONNECTION);

	// Epics
	QSqlDatabase apexDb=QSqlDatabase::addDatabase("QODBC", APEX_DB_CONNECTION);
	apexDb.setDatabaseName("local");//odbc name
	apexDb.setUserName("peadm2");
	apexDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_AGINGOVENAPEXCODE_APEXDB_PASSWORD")));
	if (!apexDb.open())
		EXIT(tr("Error in database"),apexDb.lastError().text());
	execQuery("USE PEManager_tmp", APEX_DB_CONNECTION);

#else
	// MAIN_DB_CONNECTION
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("apexBasketCraneMainOdbc");//odbc name
	mainDb.setUserName("sa");
	mainDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_AGINGOVENAPEXCODE_MAINDB_PASSWORD")));
	if (!mainDb.open())
		EXIT(tr("Error in database"),mainDb.lastError().text());
	execQuery("USE basketCraneData",MAIN_DB_CONNECTION);

	// Epics
	QSqlDatabase apexDb=QSqlDatabase::addDatabase("QODBC",APEX_DB_CONNECTION);
	apexDb.setDatabaseName("apexEpicsOdbc");//odbc name
	apexDb.setUserName("peadm2");
	apexDb.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_AGINGOVENAPEXCODE_APEXDB_PASSWORD")));
	//db.setPassword(QString::fromLocal8Bit(qgetenv("ASSIMAKIS_AGINGOVENAPEXCODE_DB_PASSWORD")));
	if (!apexDb.open())
		EXIT(tr("Error in database"),apexDb.lastError().text());
	execQuery("USE PEManager_tmp",APEX_DB_CONNECTION);
#endif


#ifdef VB_PLC_COMMS
	QWidget *tcpServerWidget=new QWidget();
#ifdef EMULATE
	tcpServer=new tcpServerClass(AGEING_OVEN_READ_PORT2,AGEING_OVEN_WRITE_PORT2,tcpServerWidget,true);
#else
	tcpServer=new tcpServerClass(AGEING_OVEN_READ_PORT2,AGEING_OVEN_WRITE_PORT2,tcpServerWidget,false);
	connect(tcpServer,SIGNAL(incomingSignal(QString,int,QString)),this,SLOT(signalFromPlcSlot(QString,int,QString)));
#endif
#endif

#ifdef TUX_PLC_COMMS
	//QString ovenIp="192.168.0.111";

	tuxipServer = new tuxipServerClass(this);

	tuxipClass* tuxIp = tuxipServer->addConnection(conveyorsIp, "oven");
	connect(tuxIp, SIGNAL(dataInPlcChangedSignal(QString, QString, int, QVariant)), this, SLOT(signalFromTuxPlcSlot(QString, QString, int, QVariant)));

	tuxipClass* newOventuxIp = tuxipServer->addConnection(newOvenIp, "new oven");
	connect(newOventuxIp, SIGNAL(dataInPlcChangedSignal(QString, QString, int, QVariant)), this, SLOT(signalFromNewOvenTuxPlcSlot(QString, QString, int, QVariant)));

	tcpServer = new tcpServerClass(tuxIp);
	oven2TCPServer = new tcpServerClass(newOventuxIp);
	//
	QAction *showServerAction = new QAction("", this);
	addAction(showServerAction);
	showServerAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	connect(showServerAction, SIGNAL(triggered()), this, SLOT(showServerSlot()));
#endif

	area = new areaClass(tcpServer, oven2TCPServer, this);

	//commandsWidget
	QWidget *commandsWidget = new QWidget(); QVBoxLayout *commandsWidgetLayout = new QVBoxLayout(); commandsWidget->setLayout(commandsWidgetLayout);
	commandsGroup = new commandsGroupClass(tcpServer, commandsWidget); commandsWidgetLayout->addWidget(commandsGroup);

	QHBoxLayout *commandsWidgetDownLayout = new QHBoxLayout();
	commandsWidgetLayout->addLayout(commandsWidgetDownLayout);

	QWidget *autoCircleWidget = new QWidget();
	autoCircleGroup = new autoCircleGroupClass(tcpServer, autoCircleWidget);
	commandsWidgetDownLayout->addWidget(autoCircleGroup, 1);
	commandsWidgetLayout->addStretch(1);

	QGroupBox *autoUnloadOvenGroupBox = new QGroupBox(tr("Auto unload oven"), commandsWidget);
	commandsWidgetDownLayout->addWidget(autoUnloadOvenGroupBox, 0);

	QVBoxLayout *autoUnloadOvenGroupBoxLayout = new QVBoxLayout();
	autoUnloadOvenGroupBox->setLayout(autoUnloadOvenGroupBoxLayout);
	autoUnloadOvenGroupBoxLayout->addWidget(new myQPushButton(tcpServer, "N11|0|57", "N11|0|0", tr("Activate"), NULL));
	autoUnloadOvenGroupBoxLayout->addWidget(autoUnloadOvenLabel = new QLabel(tr("Deactivated")), 1, Qt::AlignHCenter); autoUnloadOvenLabel->setMinimumWidth(100);
	autoUnloadOvenGroupBoxLayout->addWidget(new myQPushButton(tcpServer, "N11|0|58", "N11|0|0", tr("Deactivate"), NULL));

	QWidget *faultsListsWidget=new QWidget();
	faultsLists=new faultsListsClass(tcpServer,faultsListsWidget);

	QTabWidget *tabWidget=new QTabWidget();
	tabWidget->addTab(commandsWidget,tr("Commands"));

#ifdef VB_PLC_COMMS
	tabWidget->addTab(tcpServerWidget,tr("Communications"));
#endif

	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);

	QSplitter *mainSplitter=new QSplitter(Qt::Vertical,this);mainLayout->addWidget(mainSplitter);
	QTabWidget *mainTabWidget=new QTabWidget();

	mainSplitter->addWidget(area);
	mainSplitter->addWidget(tabWidget);
	mainSplitter->addWidget(faultsLists);
	QList<int> sizes;sizes<<520<<400<<200;
	mainSplitter->setSizes(sizes);

#ifdef VB_PLC_COMMS
	tcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,"N11",49);
#ifdef EMULATE
	tcpServer->sendCurrentValuesToPlc();
#endif
#endif

#ifdef VB_PLC_COMMS
#ifndef EMULATE
	QProcess *proc=new QProcess();
	QStringList args;
	//args<<"20.20.20.40"<<tcpServer->getIp()<<QString().number(AGEING_OVEN_WRITE_PORT2)<<QString().number(AGEING_OVEN_READ_PORT2)<<QString().number(100)<<"N11"<<QString().number(49)<<QString().number(100);
	args<<"192.168.0.111"<<tcpServer->getIp()<<QString().number(AGEING_OVEN_WRITE_PORT2)<<QString().number(AGEING_OVEN_READ_PORT2)<<QString().number(100)<<"N11"<<QString().number(49)<<QString().number(100);
	proc->start(QDir::currentPath()+"//agingOvenHmi//agingOvenHmi.exe ",args);
	externalProcessesVector.append(proc);
#endif
#endif

#ifdef TUX_PLC_COMMS
	tuxIp->addTable("N11", plcTableClass::TYPE_INT, 0, 49, 500);
	newOventuxIp->addTable("N11", plcTableClass::TYPE_INT, 0, 9, 500);
	newOventuxIp->addTable("N12", plcTableClass::TYPE_INT, 0, 36, 500);
#endif

	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimer()));
	onMainTimer();
	mainTimer.start(1000);
	tuxIp->setIsAutoUpdatingFromPlc(true);
	newOventuxIp->setIsAutoUpdatingFromPlc(true);

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

//for backwards compatibility
void mainWindowClass::signalFromTuxPlcSlot(QString ip, QString table, int index, QVariant value)
{
#ifdef TUX_PLC_COMMS
	signalFromPlcSlot(ip, table, index, value.toString());
#endif
}

void mainWindowClass::signalFromNewOvenTuxPlcSlot(QString ip, QString table, int index, QVariant value)
{
#ifdef TUX_PLC_COMMS
	signalFromPlcSlot(ip, table, index, value.toString());
#endif
}

void mainWindowClass::showServerSlot()
{
#ifdef TUX_PLC_COMMS
	tuxipServer->showDialog();
#endif
}

void mainWindowClass::signalFromPlcSlot(QString ip, QString table, int index, QString value)
{
	if (ip == conveyorsIp)
	{
		if (table == "N11")
		{
			// if ((index == 23) || (index == 30) || (index == 31) || (index == 32) || (index == 33) || (index == 34) || (index == 35) || (index == 36))
			if ((index == 30) || (index == 31) || (index == 32) || (index == 33) || (index == 34) || (index == 35) || (index == 36))
				area->plcStatusChanged(index, value.toInt());

			if (((index >= 1) && (index <= 22)) || (index == 27) || ((index >= 38) && (index <= 39)) || ((index >= 44) && (index <= 47)))
				area->plcMemoryChanged(ip, table, index, value.toInt());

			if (index == 23)
			{
				commandsGroup->plcStatusChanged(index, value.toInt());
				autoCircleGroup->plcStatusChanged(index, value.toInt());
			}

			if ((index == 24) || (index == 25) || (index == 26))
				faultsLists->plcStatusChanged(index, value.toInt());

			if (index == 26)
			{
				QVector<bool> bitsVector = deprecatedCalculateBits(value.toInt());
				if (bitsVector[8])
				{
					autoUnloadOvenLabel->setStyleSheet("QLabel{background-color:green;}");
					autoUnloadOvenLabel->setText(tr("Active"));
					autoUnloadOvenLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
				}
				else
				{
					autoUnloadOvenLabel->setStyleSheet("QLabel{}");
					autoUnloadOvenLabel->setText(tr("Inactive"));
					autoUnloadOvenLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
				}
				commandsGroup->activateStop(bitsVector[9]);
			}

			if (index == 28)
				area->setCrossTransferPos(value.toInt());
		}
	}

	if (ip == newOvenIp)
	{
		area->plcMemoryChanged(ip, table, index, value.toInt());
	}
}

void mainWindowClass::onMainTimer()
{
	setWindowTitle(tr("Aging Oven - APEX ALUMINUM EXTRUSIONS ","main window title..")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss"));
	//writeOvenLoadReport();
}

void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}

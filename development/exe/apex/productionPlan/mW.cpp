 #include "mW.h"
QString LOGINNAME;
int PRIVILEGETYPE;
int LAYER_MIN_SUPORT_LENGTH;
int MAX_NO_OF_PROFILES_TO_LOAD;
double ARC_ANGLE_FOR_LINE;
bool HAVE_PLC_COMMS;
int globalColorIndex;
int globalPressNumber=1;
#ifdef VB_PLC_COMMS
QVector<QProcess*> externalProcessesVector; 
#endif
#ifdef USE_LOCAL_DB
QString omavStacketIp="192.168.0.111";
#else
QString omavStacketIp="192.168.105.235";
#endif
void createTable(QString tableName,QStringList colNames,QStringList colTypes)
{
	QString stmnt=QString("IF NOT EXISTS (SELECT [name] FROM sys.tables WHERE [name] ='")+tableName+"')";
	stmnt+=QString("CREATE TABLE ")+tableName+" (";
	for (int i=0;i<colNames.length();i++)
	{
		stmnt+=colNames[i]+" "+colTypes[i];
		if (i!=(colNames.length()-1))
			stmnt+=",";
	}
	stmnt+=")";
	execQuery(stmnt,MAIN_DB_CONNECTION);
}
mainWindowClass::mainWindowClass():QMainWindow()
{
	plcHartBitCounter=0;
	oven=NULL;
	setWindowIcon(QIcon(":/logo_16x16.png"));
	setFocusPolicy(Qt::StrongFocus);
#ifdef TUX_PLC_COMMS
	if (QFile("settings.txt").exists())
	{
		QString settingsFileName=QString("settings.txt");
		QFile file(settingsFileName);
		if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
			exit(0);
		QTextStream in(&file);
		QString line=in.readLine();
		file.close();
		if (line=="stacker pc")
			HAVE_PLC_COMMS=true;
		else
			HAVE_PLC_COMMS=false;
	}
	else
	{
		QMessageBox msgBox(this);
		msgBox.setWindowIcon(QIcon(":/logo_16x16.png"));
		msgBox.setText(QString("Is this the stacker PC?"));
		msgBox.setInformativeText(QString("If you select 'Yes', data will be sent to the stacker plc.\nThis answer is stored in the settings.txt file, located in the executable's directory.\nIf you need to change this answer delete the settings.txt file."));
		msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ans=msgBox.exec();
		QString settingsFileName=QString("settings.txt");
		QFile file(settingsFileName);
		if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
			exit(0);
		QTextStream out(&file);
		if (ans==QMessageBox::Yes)
		{
			out<<"stacker pc";
			HAVE_PLC_COMMS=true;
		}
		else
		{
			out<<"no stacker pc";
			HAVE_PLC_COMMS=false;
		}
		file.close();
	}
#endif
#ifdef VB_PLC_COMMS
	HAVE_PLC_COMMS=QDir("plcComms").exists();
#endif
	//db
	//MAIN_DB_CONNECTION
#ifdef USE_LOCAL_DB
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("local");//odbc name apexPacketCraneMainOdbc
	mainDb.setUserName("sa"); 
	mainDb.setPassword("123");
	if (!mainDb.open()) 
		EXIT(tr("Error in database"),mainDb.lastError().text());
	apexExecQuery("if not exists(select * from sys.databases where name='StackPlan') create database StackPlan");
	execQuery("USE StackPlan",MAIN_DB_CONNECTION);
#else
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	//mainDb.setDatabaseName("apexEpicsOdbc");//odbc name apexPacketCraneMainOdbc
	mainDb.setDatabaseName("epics");//odbc name apexPacketCraneMainOdbc
	mainDb.setUserName("peadm2"); 
	mainDb.setPassword("peadm2");
	if (!mainDb.open())
		EXIT(tr("Error in database"),QString("Error opening epics odbc. Error:%1").arg(mainDb.lastError().text()));
	apexExecQuery("if not exists(select * from sys.databases where name='StackPlan') create database StackPlan");
	execQuery("USE StackPlan",MAIN_DB_CONNECTION);
#endif

	//BASKET_CRANE_DB_CONNECTION
	QSqlDatabase basketCraneDb=QSqlDatabase::addDatabase("QODBC",BASKET_CRANE_DB_CONNECTION);
#ifdef USE_LOCAL_DB
	basketCraneDb.setDatabaseName("local");//
#else
	basketCraneDb.setDatabaseName("apexBasketCraneMainOdbc");//
#endif
	basketCraneDb.setUserName("peadm2");
	basketCraneDb.setPassword("peadm2");
	//basketCraneDb.setUserName("sa");
	//basketCraneDb.setPassword("123");
	if (!basketCraneDb.open()) 
		EXIT(tr("Error in database"),QString("Error opening basketCrane odbc. Error:%1").arg(basketCraneDb.lastError().text()));
	// execQuery("if not exists(select * from sys.databases where name='basketCraneData') create database basketCraneData",BASKET_CRANE_DB_CONNECTION);
	execQuery("USE basketCraneData",BASKET_CRANE_DB_CONNECTION);
	//
	QStringList colNames;QStringList colTypes;
	//epicsProduction2 Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"pressNum"<<"die"<<"productionOrder"<<"status"<<"totalQuantity"<<"quantityInBaskets"<<"temper"  <<"length"<<"linearDencity"<<"profileSpacing"<<"doubleLength"<<"inComb"<<"onNextBasket"<<"onNextLayer"<<"scheduleDate"<<"scheduleShift"<<"soNum"      <<"isVirtual"<<"billetsNo"<<"billetLength"<<"billetButtLength"<<"scrapFrontLength"<<"scrapRearLength"<<"runOutLength"<<"dieMultiplicity"<<"baskets";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"int"		 <<"int"<<"int"					   <<"int"	 <<"int"	        <<"int"	             <<"char(16)"<<"int"	 <<"real"         <<"real"          <<"int"				  <<"int"		<<"int"				  <<"int"				 <<"varchar(12)" <<"char(1)"      <<"varchar(20)"<<"int"			<<"int"			 <<"int"			   <<"int"			       <<"int"			       <<"int"			      <<"int"			    <<"int"			       <<"int"		;
	//for status values see epicsProduction2 table statu on ppG.h
	//temper from temper table on basketCraneData database
	//the onNextBasket status is 0 or 1 and is defined by the user
	//doubleLength is 1 when there are 2 profiles aligned in the layer
	//quantityInBaskets is defined explicity by the user or implicitly when a basket is defined
	createTable("epicsProduction2",colNames,colTypes);
	if (false)
	{
		for (int i=1;i<=15;i++)
			apexExecQuery(QString("insert into epicsProduction2 (die,totalQuantity,quantityInBaskets,length,linearDencity,doubleLength,productionOrder,status,onNextBasket,onNextLayer,temper,scheduleDate,scheduleShift,soNum) values (%1,%2,0,%3,1,0,%4,0,0,0,'T52','date.str','a','soNum.str')").arg(1000+i).arg(5).arg(6000+i).arg(i));
	}
	//basketStaking Table
	//colNames.clear();colTypes.clear();
	//colNames<<"tid"											 <<"basket"<<"layer"<<"die"<<"combNo"<<"epicsTid";
	//colTypes<<"int PRIMARY KEY IDENTITY" <<"int"   <<"int"	<<"int"<<"int"   <<"int";
	////combNo=1-MAX_COMBS->the profile is placed on the combs
	////basket is initially -uniqe number when basket is after stacker gets the number of the basket when the basket becomes empty delete the records
	////thare can be more than 1 record with the same layer in order to account for layers with more than 1 type of profiles
	////combNo=1-MAX_COMBS->the profile is placed on the combs
	//createTable("basketStaking",colNames,colTypes);
	//basketSpacers Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"basket"<<"layer"<<"inComb"<<"sp1"<<"sp2"<<"sp3"<<"sp4"<<"sp5"<<"sp6"<<"sp7"<<"sp8";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"int"	 <<"int"	<<"int"	  <<"int"<<"int"<<"int"<<"int"<<"int"<<"int"<<"int"<<"int";
	//this is the table that the spacers of each basket are keptm it is defined/confirmed by the user or by the stacker plc, when the user selects define basket the spacers form basketStaking fill the qui for the user to confirm
	//crane will make an additional check before it takes the basket after the stacker: that the basket number is defined in this table
	createTable("basketSpacers",colNames,colTypes);
	//profileCode Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"die"<<"interProfileSpace"<<"angle"<<"multiplicity"<<"inComb"<<"sp1"<<"sp2"<<"sp3"<<"sp4"<<"sp5"<<"sp6"<<"sp7"<<"sp8";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"int"<<"int"	            <<"float"<<"int"         <<"int"	 <<"int"<<"int"<<"int"<<"int"<<"int"<<"int"<<"int"<<"int";
	//inComb=1->the profile is placed on the combs
	//rotation angle: the angle that the profile must be rotated to match the profile that comes out of the press
	//sp1-8 the usage of spacers for the profile
	//multiplicity number of profiles that exist in the given dxf
	createTable("profileCode",colNames,colTypes);
	//users Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											<<"login"   <<"password"<<"office"<<"press"<<"stacker"<<"needsUpdate"<<"maxNoOfProfilesToLoad";
	colTypes<<"int PRIMARY KEY IDENTITY"<<"char(30)"<<"char(30)"<<"int"	  <<"int"	 <<"int"	  <<"int"	       <<"int"	       ;
	createTable("users",colNames,colTypes);
	
		QString query = QString("if not exists(select login from users where login = 'admin')"
			"INSERT INTO users ( login, password, office, press, stacker, needsUpdate, maxNoOfProfilesToLoad) "
			"VALUES ('%1', '%2', '%3', %4, %5, %6, %7)")

			.arg("admin")
			.arg("123")
			.arg(1)
			.arg(1)
			.arg(1)
			.arg(0)
			.arg(20);

		apexExecQuery(query);



	
	//settings Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											<<"name"    <<"value";
	colTypes<<"int PRIMARY KEY IDENTITY"<<"char(30)"<<"char(30)";
	createTable("settings",colNames,colTypes);
	apexExecQuery(QString("if not exists (select name from settings where name='namelayerMinSuportLength      ') insert into settings (name,value) values ('namelayerMinSuportLength','75')"));
	apexExecQuery(QString("if not exists (select name from settings where name='arcAngleForLine               ') insert into settings (name,value) values ('arcAngleForLine','20')"));
	ARC_ANGLE_FOR_LINE=GETONE(QString("select value from settings where name='arcAngleForLine               '")).toDouble();
	//
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"userName"	   <<"description"<<"date"	   <<"sec";
	colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)"<<"char(100)"  <<"char(20)"<<"int";
	createTable("userLog",colNames,colTypes);
	apexExecQuery(QString("delete from userLog where tid not in(select top 100000 tid from userLog order by sec desc)"));

	// errorLog Table
	colNames.clear();
	colTypes.clear();
	colNames  <<"rid" << "tid" << "die" << "emessage" << "ctime";
	colTypes  << "int PRIMARY KEY IDENTITY(1,1)" << "int " << "int" << "char(250)" << "char(25)";
	createTable("errorLog", colNames, colTypes);
	if (false) {
		// Set default value for ctime to current date/time (GETDATE())
		apexExecQuery("ALTER TABLE errorLog ADD CONSTRAINT DF_errorLog_ctime DEFAULT GETDATE() FOR ctime");
		//
	}
#ifdef USE_LOCAL_DB
	PRIVILEGETYPE=PRIVILEGE_TYPE_OFFICE|PRIVILEGE_TYPE_PRESS|PRIVILEGE_TYPE_STACKER;
	LOGINNAME="admin";
#else

	PRIVILEGETYPE = PRIVILEGE_TYPE_OFFICE | PRIVILEGE_TYPE_PRESS | PRIVILEGE_TYPE_STACKER;
	LOGINNAME = "admin";
	//passwordClass *passDlg=new passwordClass(this);
	//passDlg->exec();
#endif
	LOG("user loged in");
	//
	MAX_NO_OF_PROFILES_TO_LOAD=getFirst(QString("select maxNoOfProfilesToLoad from users where login='%1'").arg(LOGINNAME),MAIN_DB_CONNECTION).toInt();
	if (MAX_NO_OF_PROFILES_TO_LOAD<1)
		MAX_NO_OF_PROFILES_TO_LOAD=1;
	//
	QWidget *centralWidget=new QWidget();
	setCentralWidget(centralWidget);
	mainLayout=new QVBoxLayout();
	centralWidget->setLayout(mainLayout);mainLayout->setMargin(0);
	QSplitter *mainSlider=new QSplitter(Qt::Vertical);mainLayout->addWidget(mainSlider);
	//basketStacking
	basketStacking=new basketStackingClass(this);
	mainSlider->addWidget(basketStacking);
	profileProductionLayout=new profileProductionLayoutClass();
	connect(profileProductionLayout,SIGNAL(productionChangedSignal(bool)),this,SLOT(productionChangedSlot(bool)));
	mainSlider->addWidget(profileProductionLayout);
	mainSlider->setSizes(QList<int>()<<900<<200);

	//
	connect(basketStacking,SIGNAL(basketSelectedSignal(basketClass*)),this,SLOT(basketSelectedSlot(basketClass*)));
	connect(profileProductionLayout,SIGNAL(productionProfileSelectedSignal(profileProductionClass*)),this,SLOT(productionProfileSelectedSlot(profileProductionClass*)));
	
	//actions
	QAction *f5Action=new QAction("",this);
	addAction(f5Action);
	f5Action->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F5));
	connect(f5Action,SIGNAL(triggered()),this,SLOT(f5Slot()));

	QAction *optionsAction=new QAction("",this);
	addAction(optionsAction);
	optionsAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
	connect(optionsAction,SIGNAL(triggered()),this,SLOT(optionsSlot()));

	QAction *keyLeftAction=new QAction("",this);    
	addAction(keyLeftAction);
	keyLeftAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Left));
	connect(keyLeftAction,SIGNAL(triggered()),this,SLOT(keyLeftSlot()));

	QAction *keyRightAction=new QAction("",this);
	addAction(keyRightAction);
	keyRightAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Right));
	connect(keyRightAction,SIGNAL(triggered()),this,SLOT(keyRightSlot()));

	QAction *delAction=new QAction("",this);
	addAction(delAction);
	delAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Delete));
	connect(delAction,SIGNAL(triggered()),this,SLOT(keyDelSlot()));

	QAction *plusAction=new QAction("",this);
	addAction(plusAction);
	plusAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Plus));
	connect(plusAction,SIGNAL(triggered()),this,SLOT(keyPlusSlot()));

	QAction *nAction=new QAction("",this);
	addAction(nAction);
	nAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
	connect(nAction,SIGNAL(triggered()),this,SLOT(keyNSlot()));

	QAction *pAction=new QAction("",this);
	addAction(pAction);
	pAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	connect(pAction,SIGNAL(triggered()),this,SLOT(keyPSlot()));
	/////
#ifdef VB_PLC_COMMS
	if (HAVE_PLC_COMMS)
	{
		QAction *plcCommsAction=new QAction("",this);
		addAction(plcCommsAction);
		plcCommsAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
		connect(plcCommsAction,SIGNAL(triggered()),this,SLOT(showPlcComsSlot()));
	}
#endif
	QAction *helpAction=new QAction("",this);
	addAction(helpAction);
	helpAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(helpAction,SIGNAL(triggered()),this,SLOT(helpActionSlot()));
	//
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimerSlot();
	mainTimer.start(10000);

	//
	connect(&checkDatabaseTimer,SIGNAL(timeout()),this,SLOT(checkDatabaseTimerSlot()));
	checkDatabaseTimerSlot();
	checkDatabaseTimer.start(1000);
	//////////////////////////////////////////////
	/////////////////////////////
	helpDlg=new helpClass(this);
	if (HAVE_PLC_COMMS)
	{
#ifdef VB_PLC_COMMS
		QWidget *tcpServerWidget=new QWidget(this);
		tcpServer=new tcpServerClass(STACKER_READ_PORT,STACKER_WRITE_PORT,tcpServerWidget,false);
		basketStacking->setTcpServer(tcpServer);
		connect(tcpServer,SIGNAL(incomingSignal(QString,int,QString)),this,SLOT(signalFromPlcSlot(QString,int,QString)));
		tcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,STACKER_TABLE_WRITE,40);
		tcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,STACKER_TABLE_READ,40);
		//
		tcpDialog=new QDialog(this);
		QVBoxLayout *tcpDialogLayout=new QVBoxLayout();
		tcpDialogLayout->addWidget(tcpServerWidget);
		tcpDialog->setLayout(tcpDialogLayout);
		//tcpServer->sendCurrentValuesToPlc();
		QProcess *stackerProc=new QProcess();
		QStringList stackerArgs;
		stackerArgs<<omavStacketIp<<tcpServer->getIp()<<QString().number(STACKER_WRITE_PORT)<<QString().number(STACKER_READ_PORT)<<QString().number(100)
			<<STACKER_TABLE_WRITE<<QString().number(50)<<QString().number(100)
			<<STACKER_TABLE_READ<<QString().number(50)<<QString().number(100);
		stackerProc->start(QDir::currentPath()+"//plcComms//omavStackerPlcComms.exe ",stackerArgs);
		externalProcessesVector.append(stackerProc);
#endif
#ifdef TUX_PLC_COMMS
		tuxipServer=NULL;
		tcpServer=NULL;
		//tuxipServer=new tuxipServerClass(false);
		//tuxipClass *omavTuxipClass=tuxipServer->addConnection(omavStacketIp,"omavStacker",true);
		//tuxipServer->addTable(STACKER_TABLE_WRITE,plcTableClass::TYPE_INT,0,39,750,omavStacketIp);
		//tuxipServer->addTable(STACKER_TABLE_READ,plcTableClass::TYPE_INT,0,39,750,omavStacketIp);

		//connect(omavTuxipClass,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromTuxPlcSlot(QString,QString,int,QVariant)));
		//tcpServer=new tcpServerClass(omavTuxipClass,this);
		//basketStacking->setTcpServer(tcpServer);

		//QAction *showServerAction=new QAction("",this);
		//addAction(showServerAction);
		//showServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
		//connect(showServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));
		//tuxipServer->setIsAutoUpdatingFromPlc(true);
#endif
	if (HAVE_PLC_COMMS)
		connect(&plcHartBitTimer,SIGNAL(timeout()),this,SLOT(plcHartBitTimerSlot()));
	plcHartBitTimer.start(5000);
	}
	//
	QToolBar *mainToolBar=new QToolBar(tr("Tools"));mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
	addToolBar(Qt::TopToolBarArea,mainToolBar);
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	press1RadioBtn=new QRadioButton(tr("Press 1"));
	press1RadioBtn->setChecked(true);
	mainToolBar->addWidget(press1RadioBtn);
	press2RadioBtn=new QRadioButton(tr("Press 2"));
	mainToolBar->addWidget(press2RadioBtn);

	connect(press1RadioBtn,SIGNAL(clicked(bool)),this,SLOT(press1RadioBtnClickedSlot(bool)));
	connect(press2RadioBtn,SIGNAL(clicked(bool)),this,SLOT(press2RadioBtnClickedSlot(bool)));

	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);


	f5Slot();



}


mainWindowClass::~mainWindowClass()
{
	LOG("user loged out");

#ifdef VB_PLC_COMMS
	terminateExternalProccesses();
#endif
	//closeOpenDatabases();
	// 
	//delete helpDlg;
	//delete oven;
	//delete basketStacking;
	//delete profileProductionLayout;
	////exit(0);
	//delete tcpServer;
	//delete tcpDialog;
}
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
//
void mainWindowClass::signalFromPlcSlot(QString table,int index,QString value)
{
	if (HAVE_PLC_COMMS)
	{
		basketStacking->stackerPlcMemoryChanged(table,index,value);
	}
}
void mainWindowClass::showPlcComsSlot()
{
#ifdef VB_PLC_COMMS
	tcpDialog->show();
#endif
}
void mainWindowClass::helpActionSlot()
{
	helpDlg->show();
}
void mainWindowClass::f5Slot()
{
	apexExecQuery(QString("update users set needsUpdate=0 where login='%1'").arg(LOGINNAME));
	if (oven)
		oven->clearLayout();
	profileProductionLayout->clearAllProfileProduction();
	//profileProductionLayout->getSelectedProfileProduction();
	
	profileProductionLayout->loadFromDatabase();
	basketStacking->clearAllBaskets();

	basketStacking->calculateLayout(profileProductionLayout);
	if (oven)
		oven->recalculate(basketStacking->getBasketsVector());
}
void mainWindowClass::optionsSlot()
{
	optionsClass *options=new optionsClass(this);
	options->exec();
	delete options;
}
void mainWindowClass::keyLeftSlot()
{
	profileProductionLayout->moveProfileLeft();
	if (profileProductionLayout->getNeedsToBeLoadedFromDatabase())
		f5Slot();
}
void mainWindowClass::keyRightSlot()
{
	profileProductionLayout->moveProfileRight();
	if (profileProductionLayout->getNeedsToBeLoadedFromDatabase())
		f5Slot();
}
void mainWindowClass::keyDelSlot()
{
	
	profileProductionClass *selected=profileProductionLayout->getSelectedProfileProduction();
	if (selected)
	{
		QMessageBox msgBox(this);
		msgBox.setWindowIcon(QIcon(":/logo_16x16.png"));
		msgBox.setText(QString("You pressed Ctrl+Del."));
		msgBox.setInformativeText(QString("Do you want to delete production order for die %1?").arg(selected->getProfileCode()));
		msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		if (msgBox.exec()==QMessageBox::Yes)
		{
			profileProductionLayout->deleteProfile();
			basketStacking->setNeedsToBeCalculated(true);
			profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
			f5Slot();
		}
	}
	
}
void mainWindowClass::keyPlusSlot()
{
	int dieNumb=0;
	dieNumb=QInputDialog::getInt(this, tr("This will create a virtual production"),tr("Specify the die that you want to add into the production schedule:"),1001,1001,INT_MAX);
	if (dieNumb>0)
	{
		apexExecQuery(QString("insert into epicsProduction2 (die,productionOrder,status,totalQuantity,quantityInBaskets,temper,length,linearDencity,doubleLength,onNextBasket,onNextLayer,scheduleDate,scheduleShift,soNum,profileSpacing,inComb,isVirtual) values (%1,(select (select max(productionOrder) from epicsProduction2)+1),-1,1,0,'T1',6000,0.1,0,0,0,'virtual','.','null',0,0,1)").arg(dieNumb));
		basketStacking->setNeedsToBeCalculated(true);
		profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
	}
}
void mainWindowClass::keyNSlot()
{
	profileProductionClass *selected=profileProductionLayout->getSelectedProfileProduction();
	if (selected)
	{
		if (QMessageBox::question(this,tr("Change profile sequence?"),tr("Is the profile %1 the next to be added into the basket?").arg(selected->getProfileCode()),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes)==QMessageBox::Yes)
			apexExecQuery(QString(QString("update epicsProduction2 set status=-2 where tid=%1").arg(selected->getProfilesEpicsTableId())));
	}
}
void mainWindowClass::keyPSlot()
{
	oven=new ovenGroupLayoutClass(this);
	connect(oven,SIGNAL(productionSequenceChangedSignal()),this,SLOT(productionChangedFromOvenLayoutSlot()));
	connect(oven,SIGNAL(f5Signal()),this,SLOT(f5Slot()));
	oven->recalculate(basketStacking->getBasketsVector());
	//oven->setWindowState(oven->windowState()|Qt::WindowMaximized);
	//oven->setModal(true);
	oven->show();
}
void mainWindowClass::mainTimerSlot()
{
	setWindowTitle(tr("Production Stacking plan APEX, (In Development live testing phase    *******   THIS IS EVALUTION ONLY ********) ", "main window title..") + QDateTime::currentDateTime().toString("yyyy-MM-dd") + (QTime::currentTime().addSecs(-36000)).toString(" hh:mm:ss"));
	//if (oven)
	//	oven->productionSequenceChangedSlot();
}
void mainWindowClass::checkDatabaseTimerSlot()
{
	//if status=-2 -> the profile is on the final saw->make it second
	apexExecQuery(
		"if ((select count(tid) from epicsProduction2 where status=-2)=1) begin "
		"declare @productionOrderOfCurrent int; "
		"set @productionOrderOfCurrent=(select min(productionOrder) from epicsProduction2 where (status=0 or status=-2) and quantityInBaskets=0); "
		"declare @tidOfNext int; "
			"if ((select count(tid) from epicsProduction2 where status=-2 and quantityInBaskets=0)>0) begin "
				"set @tidOfNext=(select tid from epicsProduction2 where status=-2 and quantityInBaskets=0) "
				"update epicsProduction2 set productionOrder=@productionOrderOfCurrent where tid=@tidOfNext; "
				"update epicsProduction2 set quantityInBaskets=0,doubleLength=0,onNextBasket=0,onNextLayer=0,status=0 where tid=@tidOfNext; "
				"update users set needsUpdate=1"
				" "
				"DECLARE @tid int; "
				"DECLARE @incrementOrder int; "
				"set @incrementOrder=@productionOrderOfCurrent; "
				"DECLARE crs CURSOR READ_ONLY "
				"FOR "
				"	SELECT tid FROM epicsProduction2 where status=0 and tid<>@tidOfNext and quantityInBaskets=0 order by productionOrder "
				"		OPEN crs "
				"		FETCH NEXT FROM crs INTO @tid "
				"		WHILE @@FETCH_STATUS = 0 "
				"		BEGIN "
				"			set @incrementOrder=@incrementOrder+1; "
				"			update epicsProduction2 set productionOrder=@incrementOrder where tid=@tid; "
				"			FETCH NEXT FROM crs INTO @tid "
				"		END "
				"CLOSE crs "
				"DEALLOCATE crs "
			"end "
		"end "
	);
	//check if there are any new entries in the epicsProduction2 table status=PRODUCTION_STATUS_INITIAL
	apexExecQuery("if ((select count(tid) from epicsProduction2  where status=-1)>0) begin update epicsProduction2 set quantityInBaskets=0,doubleLength=0,onNextBasket=0,onNextLayer=0,status=0 where status=-1 update users set needsUpdate=1 end");
	//check if someone changed something
	if (GETONE(QString("select needsUpdate from users where login='%1'").arg(LOGINNAME)).toBool())
	{
		basketStacking->setNeedsToBeCalculated(true);
		profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
		f5Slot();
	}
}
void mainWindowClass::productionChangedFromOvenLayoutSlot()
{
	//profileProductionLayout->clearAllProfileProduction();
	//basketStacking->clearAllBaskets();
	basketStacking->setNeedsToBeCalculated(true);
	profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
	//f5Slot();
}
void mainWindowClass::plcHartBitTimerSlot()
{
	if (HAVE_PLC_COMMS)
	{
		if (plcHartBitCounter>=10000)
			plcHartBitCounter=0;
		plcHartBitCounter++;
		if (tcpServer)
			tcpServer->sendString(QString("%1|0|%2").arg(STACKER_TABLE_WRITE).arg(plcHartBitCounter));
	}
}
void mainWindowClass::productionChangedSlot(bool needsReloadFromDatabase)
{
	NOTIFY_OTHERS
	if (needsReloadFromDatabase)
	{
		profileProductionLayout->loadFromDatabase();
		profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
	}
	basketStacking->setNeedsToBeCalculated(true);
	if (oven)
		oven->setNeedsToBeCalculated(true);
}
void mainWindowClass::basketSelectedSlot(basketClass* b)
{
	profileProductionLayout->setVisibleProfile(b->getFirstProfileCode());
}
void mainWindowClass::productionProfileSelectedSlot(profileProductionClass* p)
{
	basketStacking->setVisibleBasketWithProfile(p->getProfileCode());
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}
void mainWindowClass::press1RadioBtnClickedSlot(bool checked)
{
	if (globalPressNumber==2)
	{
		globalPressNumber=1;
		basketStacking->setNeedsToBeCalculated(true);
		profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
		f5Slot();
//		press2RadioBtnClickedSlot(true);
	}
}
void mainWindowClass::press2RadioBtnClickedSlot(bool checked)
{
	if (globalPressNumber==1)
	{
		globalPressNumber=2;
		basketStacking->setNeedsToBeCalculated(true);
		profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
		f5Slot();
//		press1RadioBtnClickedSlot(true);
	}
}
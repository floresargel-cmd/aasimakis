#include "mW.h"
extern QSplashScreen *splash;
void createTable(QString tableName,QStringList colNames,QStringList colTypes)
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
	apexExecQuery(stmnt,MAIN_DB_CONNECTION);
}
void initializeTables()
{
	if(GETONE("select count(tid) from positions").toInt()==0)
	{
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('fork'            ,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_FORK							).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('newSB'           ,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_NEWSB							).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('newPacket'       ,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_NEWPACKET					).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('sbExit'          ,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_SBEXIT						).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('packetExit'      ,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_PACKETEXIT				).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('sbPacketEntrance',%1,0,0,%2,-1,-1,-1)").arg(POS_ID_SB_PACKET_IMPORT	).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('crane'           ,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_CRANE							).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('westConv'        ,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_WESTCONV					).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('eastConv'        ,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_EASTCONV					).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,col,row) values('storeExit'       ,%1,0,0,%2,-1,-1,-1)").arg(POS_ID_STOREEXIT					).arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE));
		int cell=0;
		for (int col=1;col<=60;col++)
		{
			for (int row=1;row<=13;row++)
			{
				cell++;
				int acceptsLengthType=LEFT_AND_CENTER_PACKET_PLACING_CASSETTE;
				if (( (col==15)||(col==16)||(col==29)||(col==30)||(col==43)||(col==44)||(col==57)||(col==58) ) ||(row<=4))
					acceptsLengthType=CENTER_ONLY_PACKET_PLACING_CASSETTE;
				int storagingType=STORAGE_TYPE_SHORT_TERM;
				if ((col>=21)&&(col<=40))
					storagingType=STORAGE_TYPE_MID_TERM;
				if ((col>=41)&&(col<=60))
					storagingType=STORAGE_TYPE_LONG_TERM;
				//west
				apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,storagingType,col,row) values('w %1',%2,0,%3,%4,%5,%6,%7,%8)").arg(-cell).arg(-cell).arg(-cell).arg(acceptsLengthType)     .arg(IS_NOT_LOCKED).arg(storagingType).arg(col).arg(row));
				//east
				apexExecQuery(QString("insert into positions (posName,posId,packetId,cassette,acceptableContents,isLocked,storagingType,col,row) values('w %1',%2,0,%3,%4,%5,%6,%7,%8)").arg( cell).arg( cell).arg( cell).arg(CENTER_ONLY_PACKET_PLACING_CASSETTE).arg(IS_NOT_LOCKED).arg(storagingType).arg(col).arg(row));
			}
		}
	}
	if(GETONE("select count(tid) from cassettes").toInt()==0)
	{
		for (int cas=1;cas<=780;cas++)
		{
			//west
			apexExecQuery(QString("insert into cassettes (cassette,status) values(%1,%2)").arg(-cas).arg(CASSETTE_IDLE));
			//east
			apexExecQuery(QString("insert into cassettes (cassette,status) values(%1,%2)").arg( cas).arg(CASSETTE_IDLE));
		}
	}
	if(GETONE("select count(name) from state").toInt()==0)
	{
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("forkFromPos").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("forkToPos").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("craneFromPos").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("craneToPos").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("westConveyorActivePacketExport").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("westConveyorActivePacketImport").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("westConveyorActiveCassetteImport").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("eastConveyorActivePacketExport").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("eastConveyorActivePacketImport").arg("0"));
		apexExecQuery(QString("insert into state (name,value) values('%1','%2')").arg("eastConveyorActiveCassetteImport").arg("0"));
	}
}
mainWindowClass::mainWindowClass()
{
	splash->showMessage(tr("Starting..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	//QSound(QDir::current().absolutePath()+"//failed.wav").play();
	setWindowIcon(QIcon(":/logo_16x16.png"));
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimer()));
	mainTimer.start(1000);
	cranePlcConnected=forkPlcConnected=false;
	///////////////////////////dataBase
	splash->showMessage(tr("Connecting to databasse..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	if (true)
	{
		//MAIN_DB_CONNECTION
		QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
#ifdef USE_LOCAL_DB
		mainDb.setDatabaseName("local");//odbc name apexPacketCraneMainOdbc
#else
		mainDb.setDatabaseName("apexPacketCraneMainOdbc");//odbc name apexBasketCran_eMainOdbc
#endif
		mainDb.setUserName("sa"); 
		mainDb.setPassword("123");
		if (!mainDb.open()) 
			EXIT(tr("Error in database"),mainDb.lastError().text());
		apexExecQuery("if not exists(select * from sys.databases where name='wareHouseData') create database wareHouseData");
		apexExecQuery("USE wareHouseData",MAIN_DB_CONNECTION);
		QStringList colNames;QStringList colTypes;
		//positions Table
		colNames.clear();colTypes.clear();
		colNames<<"tid"											 <<"posName " <<"posId"<<"packetId"<<"cassette"<<"acceptableContents"<<"isLocked"<<"storagingType"<<"col"<<"row";
		colTypes<<"int PRIMARY KEY IDENTITY" <<"char(16)" <<"int"	 <<"int"	   <<"int"	   <<"int"							 <<"int"	   <<"int"					<<"int"<<"int";
		createTable("positions",colNames,colTypes);
		//state Table
		colNames.clear();colTypes.clear();
		colNames<<"name"    <<"value"   ;
		colTypes<<"char(50)"<<"char(15)";
		createTable("state",colNames,colTypes);
		//cassettes Table
		colNames.clear();colTypes.clear();
		colNames<<"tid"											 <<"cassette"<<"status";
		colTypes<<"int PRIMARY KEY IDENTITY" <<"int"	   <<"int"	 ;
		createTable("cassettes",colNames,colTypes);
		initializeTables();
		//packets Table
		colNames.clear();colTypes.clear();
		colNames<<"tid"											 <<"packetId"<<"woods"<<"length"<<"width"<<"height"<<"weight"<<"sb" <<"destination"<<"storagingType";
		colTypes<<"int PRIMARY KEY IDENTITY" <<"int"	   <<"int"	 <<"int"	 <<"int"	<<"int"	  <<"real"	<<"int"<<"int"        <<"int"					 ;
		createTable("packets",colNames,colTypes);
		//exports Table
		colNames.clear();colTypes.clear();			//packet/sb no  'sb'/'packet'   0/1
		colNames<<"tid"											 <<"contents "<<"type"	<<"status"<<"orderSequence"<<"contentsSequence";
		colTypes<<"int PRIMARY KEY IDENTITY" <<"int"			<<"int"		<<"int"   <<"int"	         <<"int"        ;
		createTable("exports",colNames,colTypes);
		//log Table
		colNames.clear();colTypes.clear();
		colNames<<"tid"											 <<"type"     <<"status"  <<"fromPosId"<<"toPosId"<<"posId"<<"newValue"<<"oldValue"<<"date"	   <<"sec";
		colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"char(10)"<<"int"      <<"int"    <<"int"	 <<"int"	   <<"int"	   <<"char(20)"<<"int";
		createTable("log",colNames,colTypes);
		apexExecQuery(QString("delete from log where tid not in(select top 100000 tid from log order by sec desc)"));
		//logCranePlc Table
		colNames.clear();colTypes.clear();
		colNames<<"tid"											 <<"address"  <<"value"<<"X"     <<"Y"     <<"Z"     <<"date"	   <<"sec";
		colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"int"  <<"float" <<"float" <<"float" <<"char(20)"<<"int";
		createTable("logCranePlc",colNames,colTypes);
		apexExecQuery(QString("delete from logCranePlc where tid not in(select top 50000 tid from logCranePlc order by sec desc)"));
		//logForkPlc Table
		colNames.clear();colTypes.clear();
		colNames<<"tid"											 <<"address"  <<"value"<<"X"     <<"Y"     <<"Z"     <<"date"	   <<"sec";
		colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"int"  <<"float" <<"float" <<"float" <<"char(20)"<<"int";
		createTable("logForkPlc",colNames,colTypes);
		apexExecQuery(QString("delete from logForkPlc where tid not in(select top 50000 tid from logForkPlc order by sec desc)"));
		//logResetPlc Table
		colNames.clear();colTypes.clear();
		colNames<<"tid"											 <<"type"	    <<"value"<<"date"	   <<"sec";
		colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"float"<<"char(20)"<<"int";
		createTable("logResetPlc",colNames,colTypes);
		apexExecQuery(QString("delete from logResetPlc where tid not in(select top 50000 tid from logResetPlc order by sec desc)"));
		//logErrorPlc Table
		colNames.clear();colTypes.clear();
		colNames<<"tid"											 <<"type"	    <<"bit"     <<"description"<<"date"	   <<"sec";
		colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"char(10)"<<"char(100)"  <<"char(20)"<<"int";
		createTable("logErrorPlc",colNames,colTypes);
		apexExecQuery(QString("delete from logErrorPlc where tid not in(select top 10000 tid from logErrorPlc order by sec desc)"));
		//vLog Table
		colNames.clear();colTypes.clear();
		colNames<<"tid"											 <<"type"	    <<"title"	   <<"description"<<"date"	   ;
		colTypes<<"int PRIMARY KEY IDENTITY" <<"char(50)" <<"char(50)" <<"char(150)"  <<"char(20)";
		createTable("vLog",colNames,colTypes);
		apexExecQuery(QString("delete from vLog where tid not in(select top 10000 tid from vLog order by tid desc)"));
	//users Table
		colNames.clear();colTypes.clear();
		colNames<<"tid"											 <<"name"	    <<"pass"    ;
		colTypes<<"int PRIMARY KEY IDENTITY" <<"char(15)" <<"char(10)";
		createTable("users",colNames,colTypes);
		apexExecQuery("if not exists (select name from users where name='admin') insert into users (name,pass) values('admin','123')");
	}
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();
	QHBoxLayout *upLayout=new QHBoxLayout();mainLayout->addLayout(upLayout);
	setLayout(mainLayout);
	openGLWidget=new openGLWidgetClass();
	mainLayout->addWidget(openGLWidget);
	manualCassetteMovement=new manualCassetteMovementClass(openGLWidget->getCraneClass());
	manualMissions=new manualMissionsClass(openGLWidget->getCraneClass(),openGLWidget->getForkClass(),openGLWidget->getAllPosClass(),this);
	//
	splash->showMessage(tr("Connecting to plc..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	plc=new plcClass(this);
	openGLWidget->setPlcClass(plc);
#ifdef VB_PLC_COMMS
	connect(plc->getPacketCraneTcpServer(),SIGNAL(incomingSignal(QString,int,QString)),this,SLOT(signalFromPacketCranePlcSlot(QString,int,QString)));
	connect(plc->getForkLiftTcpServer(),SIGNAL(incomingSignal(QString,int,QString)),this,SLOT(signalFromForkLiftPlcSlot(QString,int,QString)));
	//connect(plc->getPacketCraneTcpServer(),SIGNAL(newConnectionSignal()),this,SLOT(newConnectionFromCranePlcSlot()));
	//connect(plc->getForkLiftTcpServer(),SIGNAL(newConnectionSignal()),this,SLOT(newConnectionFromForkPlcSlot()));
#endif
#ifdef TUX_PLC_COMMS
	connect(plc->getCraneTuxClass(),SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromCraneTuxPlcSlot(QString,QString,int,QVariant)));
	connect(plc->getForkTuxClass(),SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromForkTuxPlcSlot(QString,QString,int,QVariant)));
#endif
	//
	splash->showMessage(tr("Creating gui..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	autoMissionsCheckBox=new QCheckBox(tr("Auto"));upLayout->addWidget(autoMissionsCheckBox);
	connect(autoMissionsCheckBox,SIGNAL(stateChanged(int)),this,SLOT(autoMissionsCheckBoxStateChangedSlot(int)));

	manualMissionsBtn=new QPushButton(tr("Manual missions"),this);upLayout->addWidget(manualMissionsBtn);
	connect(manualMissionsBtn,SIGNAL(clicked()),this,SLOT(manualMissionsBtnClickedSlot()));

	plcPushButton=new QPushButton(tr("HMI"),this);upLayout->addWidget(plcPushButton);//plcPushButton->setGeometry(10,10,100,20);
	connect(plcPushButton,SIGNAL(clicked()),this,SLOT(plcButtonClickedSlot()));
	//exportsClass
	exports=new exportsClass(this);
	QPushButton *exportsButton=new QPushButton(tr("Exports"),this);upLayout->addWidget(exportsButton);//exportsButton->setGeometry(10,40,100,20);
	connect(exportsButton,SIGNAL(clicked()),this,SLOT(exportsButtonClickedSlot()));
	//plcCommsLabel
	cranePlcCommsLabel=new QLabel("",this);upLayout->addWidget(cranePlcCommsLabel);//cranePlcCommsLabel->setGeometry(10,70,200,20);
	forkPlcCommsLabel=new QLabel("",this);upLayout->addWidget(forkPlcCommsLabel);//forkPlcCommsLabel->setGeometry(10,100,200,20);
	//
	exportsTo23Radio=new QRadioButton(tr("Exports to 23"));upLayout->addWidget(exportsTo23Radio);connect(exportsTo23Radio,SIGNAL(toggled(bool)),this,SLOT(exportsTo23RadioToggledSlot(bool)));
	exportsTo22Radio=new QRadioButton(tr("Exports to 22"));upLayout->addWidget(exportsTo22Radio);connect(exportsTo22Radio,SIGNAL(toggled(bool)),this,SLOT(exportsTo22RadioToggledSlot(bool)));
	//
	userBackupPushButton=new QPushButton(tr("backup database"),this);upLayout->addWidget(userBackupPushButton);//userBackupPushButton->setGeometry(10,130,100,20);
	connect(userBackupPushButton,SIGNAL(clicked()),this,SLOT(onUserBackupSlot()));
	//
	plc->setManualCassetteMovementClass(manualCassetteMovement);
	upLayout->addStretch(1);
	//
	//plc->getPacketCraneTcpServer()->sendCurrentValuesToPlc();
	//plc->getForkLiftTcpServer()->sendCurrentValuesToPlc();
	openGLWidget->startTimers();
	lastCranePlcHandShakeValue=lastForkPlcHandShakeValue=0;
	connect(&commsTimer,SIGNAL(timeout()),this,SLOT(onCommsTimerSlot()));
	commsTimer.start(5000);
	connect(&backupTimer,SIGNAL(timeout()),this,SLOT(onBackupTimerSlot()));
	onBackupTimerSlot();
	backupTimer.start(1*60*60*1000);
	//backupTimer.start(60*1000);
#ifdef TUX_PLC_COMMS
	QAction *showServerAction=new QAction("",this);
	addAction(showServerAction);
	showServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	connect(showServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));
	plc->addTables();
#endif
	vLog("info","mainWindowClass::mainWindowClass():","programm started");
	exportsTo23Radio->setChecked(true);

	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
}
mainWindowClass::~mainWindowClass()
{
	onBackupTimerSlot();
	vLog("info","mainWindowClass::~mainWindowClass():","exit programm");
	QFile file("log.txt");
  if (!file.open(QIODevice::Append|QIODevice::Text))
		return;
	QTextStream out(&file);
	out<<QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss\t");
	out<<"terminated by the user\n";
	file.close();
#ifdef VB_PLC_COMMS
	terminateExternalProccesses();
#endif
	return;
	closeOpenDatabases();
	delete plc;
	delete openGLWidget;
	delete exports;
	delete manualCassetteMovement;
}
void mainWindowClass::onCommsTimerSlot()
{
	if (lastCranePlcHandShakeValue!=plc->getLastCranePlcHandShakeValue())
	{
		lastCranePlcHandShakeValue=plc->getLastCranePlcHandShakeValue();
		cranePlcCommsLabel->setText(QString("crane comms ok %1").arg(lastCranePlcHandShakeValue));
		cranePlcCommsLabel->setStyleSheet("QLabel{font-size:14px;color:#ffffff;background-color:#00ff00;border-radius:7px;border-color:beige;padding-left:5px;}");
	}
	else
	{
//		QSound(QDir::current().absolutePath()+"//failed.wav").play();
		cranePlcCommsLabel->setText(QString("crane comms error %1").arg(lastCranePlcHandShakeValue));
		cranePlcCommsLabel->setStyleSheet("QLabel{font-size:14px;color:#ffffff;background-color:#ff0000;border-radius:7px;border-color:beige;padding-left:5px;}");
	}
	if (lastForkPlcHandShakeValue!=plc->getLastForkPlcHandShakeValue())
	{
		lastForkPlcHandShakeValue=plc->getLastForkPlcHandShakeValue();
		forkPlcCommsLabel->setText(QString("fork comms ok %1").arg(lastForkPlcHandShakeValue));
		forkPlcCommsLabel->setStyleSheet("QLabel{font-size:14px;color:#ffffff;background-color:#00ff00;border-radius:7px;border-color:beige;padding-left:5px;}");
	}
	else
	{
//		QSound(QDir::current().absolutePath()+"//failed.wav").play();
		forkPlcCommsLabel->setText(QString("fork comms error %1").arg(lastForkPlcHandShakeValue));
		forkPlcCommsLabel->setStyleSheet("QLabel{font-size:14px;color:#ffffff;background-color:#ff0000;border-radius:7px;border-color:beige;padding-left:5px;}");
	}
}
void mainWindowClass::onUserBackupSlot()
{
	QString cmd;
	QString backupDir=QDir::current().absolutePath()+"/";backupDir=backupDir.replace("/","\\");
	QString backupFile=QString("packetCraneUserBackup_")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+"_"+QTime::currentTime().toString("hh.mm.ss")+".bak";
	QString backupFileAbsPath=backupDir+backupFile;
	cmd=QString("sqlcmd -S .\\SQLExpress -Q \"BACKUP DATABASE wareHouseData TO DISK='"+backupFileAbsPath+"' WITH INIT\"");
	QProcess *backupProcess=new QProcess(this);
	backupProcess->start(cmd);
	QMessageBox::information(this,"information",tr("Backup created in file : \n%1").arg(backupFileAbsPath));
}
void deleteOldBackups(int backupsToKeep)
{
	QDir bakFilesDir=QDir::current().absolutePath()+"/backup/";
	QStringList filelist=bakFilesDir.entryList(QStringList()<<"packetCraneDBBackup_*.bak",QDir::Files,QDir::Name|QDir::Reversed);
	for (int i=(backupsToKeep-1);i<filelist.count();i++)
	{
		bakFilesDir.remove(filelist[i]);
	}
}
void mainWindowClass::onBackupTimerSlot()
{
//#ifndef _DEBUG
	deleteOldBackups(25);
	QString cmd;
	QString backupDir=QDir::current().absolutePath()+"/backup/";backupDir=backupDir.replace("/","\\");
	QString backupFile=QString("packetCraneDBBackup_")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+"_"+QTime::currentTime().toString("hh.mm.ss")+".bak";
	QString backupFileAbsPath=backupDir+backupFile;
	cmd=QString("sqlcmd -S .\\SQLExpress -Q \"BACKUP DATABASE wareHouseData TO DISK='"+backupFileAbsPath+"' WITH INIT\"");
	QProcess *backupProcess=new QProcess(this);
	backupProcess->start(cmd);
//#endif
}
void mainWindowClass::exportsButtonClickedSlot()
{
	exports->show();
}
void mainWindowClass::plcButtonClickedSlot()
{
#ifndef _DEBUG
	passwordClass *pass=new passwordClass(this);
	pass->exec();
	if (pass->getPasswordIsCorrect())
		plc->show();
	delete pass;
#else
	plc->show();
#endif
}
void mainWindowClass::onMainTimer()
{
	setWindowTitle(tr("Packet crane, assimakis sa ","main window title..")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss"));
}
void mainWindowClass::newConnectionFromForkPlcSlot()
{
	//forkPlcConnected=true;
	//if (cranePlcConnected)
	//	openGLWidget->startTimers();
}
void mainWindowClass::newConnectionFromCranePlcSlot()
{
	//cranePlcConnected=true;
	//if (forkPlcConnected)
	//	openGLWidget->startTimers();
}
void mainWindowClass::signalFromCraneTuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
#ifdef TUX_PLC_COMMS
	if (!plc->getTuxServer()->getHasError())
		signalFromPacketCranePlcSlot(table,index,value.toString());
#endif
}
void mainWindowClass::signalFromPacketCranePlcSlot(QString table,int index,QString value)
{
	//if ((table=="N150")||(table=="N152"))
	//	apexExecQuery(QString("insert into logCranePlc (address,value,X,Y,Z,date,sec) values('%1[%2]',%3,%4,%5,%6,'%7',%8)")
	//	.arg(table).arg(index).arg(value).arg(openGLWidget->getPacketCraneX()).arg(openGLWidget->getPacketCraneY()).arg(openGLWidget->getPacketCraneZ()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	if (table=="N150")
	{
		openGLWidget->signalFromCranePlc(table,index,value);
		plc->packetCranePlcIntMemory150Changed(index,value.toInt());
	}
	if (table=="N152")
	{
		openGLWidget->signalFromCranePlc(table,index,value);
		plc->packetCranePlcIntMemory152Changed(index,value.toInt());
	}
	if (table=="F151")
	{
		openGLWidget->signalFromCranePlc(table,index,value);
		plc->packetCranePlcFloatMemoryChanged(index,value.toFloat());
		if (index==14)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('craneX',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		if (index==15)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('craneZ',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		if (index==16)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('craneY',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	}
}
void mainWindowClass::signalFromForkTuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
#ifdef TUX_PLC_COMMS
	if (!plc->getTuxServer()->getHasError())
		signalFromForkLiftPlcSlot(table,index,value.toString());
#endif
}
void mainWindowClass::signalFromForkLiftPlcSlot(QString table,int index,QString value)
{
	//if (table=="N160")
	//	apexExecQuery(QString("insert into logForkPlc (address,value,X,Y,Z,date,sec) values('%1[%2]',%3,%4,%5,%6,'%7',%8)")
	//	.arg(table).arg(index).arg(value).arg(openGLWidget->getForkCraneX()).arg(openGLWidget->getForkCraneY()).arg(openGLWidget->getForkCraneZ()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	if (table=="N160")
	{
		openGLWidget->signalFromForkPlc(table,index,value);
		plc->forkLiftPlcIntMemoryChanged(index,value.toInt());
	}
	if (table=="F161")
	{
		openGLWidget->signalFromForkPlc(table,index,value);
		plc->forkLiftPlcFloatMemoryChanged(index,value.toFloat());
	}
	if (table=="F162")
	{
		if (index==0)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('forkX',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		if (index==1)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('forkZ',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		if (index==2)apexExecQuery(QString("insert into logResetPlc (type,value,date,sec) values('forkY',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	}
}
void mainWindowClass::showServerSlot()
{
#ifdef TUX_PLC_COMMS
	plc->showTuxServerCommsDialog();
#endif
}
void mainWindowClass::exportsTo23RadioToggledSlot(bool checked)
{
	if (checked)
		exportsTo22Radio->setChecked(false);
	openGLWidget->setExportsDestination(managerClass::exportsDestinationEnum::exportsTo23);
}
void mainWindowClass::exportsTo22RadioToggledSlot(bool checked)
{
	if (checked)
		exportsTo23Radio->setChecked(false);
	openGLWidget->setExportsDestination(managerClass::exportsDestinationEnum::exportsTo22);
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}
void mainWindowClass::autoMissionsCheckBoxStateChangedSlot(int state)
{
	openGLWidget->enableAutoMissions(autoMissionsCheckBox->isChecked());
	manualMissionsBtn->setEnabled(!autoMissionsCheckBox->isChecked());
}
void mainWindowClass::manualMissionsBtnClickedSlot()
{
	passwordClass *pass = new passwordClass(this);
	pass->exec();
	if (pass->getPasswordIsCorrect())
		manualMissions->exec();
}
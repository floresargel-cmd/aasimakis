#include "pch.h"
#include "mW.h"
#include "environment.h"
//#define inMyLaptop
#ifdef emulateCrane2
#define inMyLaptop
#endif
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
#define BASKET_CONTENS_LESS_THAN_24 2							//7315mm
#define BASKET_CONTENS_LESS_THAN_30 3							//9144mm
#define BASKET_CONTENS_MORE_THAN_30 4
void insertPos(int posNumber,int posIndex,int basketNumber,QString description,int typeOfProfiles)
{
	execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('%1',%2,%3,%4,%5,0,0)").arg(description).arg(posNumber).arg(posIndex).arg(basketNumber).arg(1),bDb);
}
mainWindowClass::mainWindowClass():QMainWindow()
{
	const Crane2::Environment& environment = Crane2::Environment::instance();
	if (!environment.loaded() || environment.isObserver())
		uExit("Environment", "The control window requires a configured Test or Live environment.");
	isConnectedToEpicsDbG=false;
#ifdef emulateCrane2
	QMessageBox::critical(this,"Emulate","emulateCrane2");
#endif
	resize(1920,1080);
const bool emulatePlc = !environment.livePlcEnabled();
	crane2Ip = environment.plcAddress(0);
	oldOvenIp = environment.plcAddress(1);
	newOvenIp = environment.plcAddress(2);

	splash->showMessage(tr("Starting...."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	setWindowIcon(QIcon(":/assimakis.png"));
	setStyleSheet(QString("QLabel,QAction,QAction,QToolButton,QToolBar,QPushButton,QToolBox::tab,QToolBox,QCheckBox,QTabWidget,QSpinBox,QTableWidget,QTableWidgetItem,QListWidget{font-size:13px;color:#000033;}"));
	splash->showMessage(tr("Connecting to plc..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxipServer=new tuxipServerClass(this);
//	tuxipServer->setPacketRequestInterval(10000);
	tuxipClass *basketTuxipConnection=tuxipServer->addConnection(crane2Ip,"Crane 2",emulatePlc);
	tuxipServer->addTable("toPcI",plcTableClass::TYPE_INT,0,75,100,crane2Ip);
	tuxipServer->addTable("toPcF",plcTableClass::TYPE_FLOAT,0,50,100,crane2Ip);

	tuxipServer->addTable("PcCom",plcTableClass::TYPE_INT,0,9,100,crane2Ip);
	tuxipServer->addTable("bConveyorBasket",plcTableClass::TYPE_INT,0,29,100,crane2Ip);
	tuxipServer->addTable("bConveyorState",plcTableClass::TYPE_INT,0,29,100,crane2Ip);
	tuxipServer->addTable("bConveyorData",plcTableClass::TYPE_INT,0,29,100,crane2Ip);
	//
	tuxipClass *oldOvenConnection=tuxipServer->addConnection(oldOvenIp,"Old oven",emulatePlc);
	tuxipServer->addTable("N11",plcTableClass::TYPE_INT,0,20,100,oldOvenIp);
	//
	tuxipClass *newOvenConnection=tuxipServer->addConnection(newOvenIp,"New oven",emulatePlc);
	newOvenConnection->addTable("N11",plcTableClass::TYPE_INT,0,10,100);
	//
	// Databases were opened and their catalogs verified before constructing this window.
	isConnectedToEpicsDbG = true;
	//c2logGui
	createTable("c2logGui",QStringList()<<"tId int primary key identity"<<QString("mess char(%1)").arg(maxMessageLength)<<QString("details char(%1)").arg(maxDescriptionLength)<<"messenger char(10)"<<"type char(10)"<<"time char(50)",bDb);
	execQuery(QString("delete from c2logGui where tId not in(select top 10000 tId from c2logGui order by tId desc)"),bDb);
	//"c2logV"
	createTable("c2logV",QStringList()<<"tId int primary key identity"<<QString("type char(%1)").arg(maxMessageLength)<<QString("description char(%1)").arg(maxDescriptionLength)<<"time char(50)",bDb);
	execQuery(QString("delete from c2logV where tId not in(select top 10000 tId from c2logV order by tId desc)"),bDb);
	//"c2log"
	createTable("c2log",QStringList()<<"tId int primary key identity"<<QString("type char(%1)").arg(maxMessageLength)<<QString("description char(%1)").arg(maxDescriptionLength)<<"time char(20)",bDb);
	execQuery(QString("delete from c2log where tId not in(select top 10000 tId from c2log order by tId desc)"),bDb);
	//"c2logPos"
	createTable("c2logPos",QStringList()<<"tId int primary key identity"<<"pos char(10)"<<"basket int"<<"info char(50)"<<"time char(20)",bDb);
	execQuery(QString("delete from c2logPos where tId not in(select top 10000 tId from c2logPos order by tId desc)"),bDb);
	//"plcMessages"
	createTable("c2plcMessages",QStringList()<<"tId int primary key identity"<<"plcAddress char(15)"<<"plcIndex int"<<"plcBit int"<<"bitStatus int"<<"type char(100)"<<"description char(300)"<<"time char(50)",bDb);
	execQuery(QString("delete from c2plcMessages where tId not in(select top 10000 tId from c2plcMessages order by tId desc)"),bDb);
	//c2missions
	createTable("c2missions",QStringList()<<"tId int primary key identity"<<"fromPosNumber int"<<"fromPosIndex int"<<"toPosNumber int"<<"toPosIndex int"<<"status char(30)",bDb);
	if (getFirst(QString("select count(tId) from c2missions"),bDb).toInt()==0)
	{
		execQuery(QString("insert into c2missions (status,fromPosNumber,fromPosIndex,toPosNumber,toPosIndex) values('%1',0,0,0,0)").arg(nextStr),bDb);
		execQuery(QString("insert into c2missions (status,fromPosNumber,fromPosIndex,toPosNumber,toPosIndex) values('%1',0,0,0,0)").arg(activeStr),bDb);
	}
	createTable(crane2ExportsDestackerForSb,						QStringList()<<"tId int primary key identity"<<"basket int unique"<<"priority int unique",bDb);
	createTable(crane2ExportsAnodizing,									QStringList()<<"tId int primary key identity"<<"basket int unique"<<"priority int unique",bDb);
	createTable(crane2ExportsPackingDestacker1Position,	QStringList()<<"tId int primary key identity"<<"basket int unique"<<"priority int unique",bDb);
	createTable(crane2ExportsPackingDestacker2Position,	QStringList()<<"tId int primary key identity"<<"basket int unique"<<"priority int unique",bDb);

	if (true)
	{
//		createTable("positions",QStringList()<<"pid int primary key identity"<<"description char(20)"<<"posNumber int" <<"posIndex int" <<"basketTableId int"	<<"locked int"<<"targetNumber int" <<"targetIndex int",bDb);
		if (getFirst(QString("select max(posNumber) from positions"),bDb).toInt()<toRotatingSouthPlcId)
		{
			insertPos(bufferNoprthPlcId,1,0,"buffer",BASKET_CONTENS_LESS_THAN_24);
			insertPos(bufferNoprthPlcId,2,0,"buffer",BASKET_CONTENS_LESS_THAN_24);
			insertPos(bufferNoprthPlcId,3,0,"buffer",BASKET_CONTENS_LESS_THAN_24);
			insertPos(bufferNoprthPlcId,4,0,"buffer",BASKET_CONTENS_LESS_THAN_24);
			insertPos(bufferSouthPlcId,1,0,"buffer",BASKET_CONTENS_LESS_THAN_24);
			insertPos(bufferSouthPlcId,2,0,"buffer",BASKET_CONTENS_LESS_THAN_24);
			insertPos(bufferSouthPlcId,3,0,"buffer",BASKET_CONTENS_LESS_THAN_24);
			insertPos(bufferSouthPlcId,4,0,"buffer",BASKET_CONTENS_LESS_THAN_24);

			insertPos(toRotatingSouthPlcId,1		,0,"To rotating south",BASKET_CONTENS_LESS_THAN_24);
			insertPos(toRotatingNorthPlcId,1		,0,"To rotating north",BASKET_CONTENS_LESS_THAN_24);
			insertPos(rotatingSouthPlcId,1			,0,"Rotating south",BASKET_CONTENS_LESS_THAN_24);
			insertPos(rotatingNorthPlcId,1			,0,"Rotating north",BASKET_CONTENS_LESS_THAN_24);
			insertPos(stackerWorkingingPlcId,1	,0,"Workinging",BASKET_CONTENS_LESS_THAN_24);
			insertPos(stackerWaitingPlcId,1			,0,"Waiting",BASKET_CONTENS_LESS_THAN_24);
			insertPos(exit1PlcId,1							,0,"Exit1",BASKET_CONTENS_LESS_THAN_24);
			insertPos(exit2PlcId,1							,0,"Exit2",BASKET_CONTENS_LESS_THAN_24);

			insertPos(crane2PlcId,0							,0		,"Crane",BASKET_CONTENS_LESS_THAN_24);
		}
	}
	initializeGlobal();
	layoutGraphicsView=new myQGraphicsViewClass(true,this);
	QGraphicsScene *layoutScene=new QGraphicsScene(this);

	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr("Zoom extents"),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));
	resetGraphicsViewZoom();

	gItemClass *layoutGraphicsItem=new gItemClass(":/dxfs/layout.dxf",QPen(QColor(200,200,200)),false,false,QColor(),this);
	layoutGraphicsView->setScene(layoutScene);
	layoutGraphicsItem->setZValue(-10.);layoutScene->addItem(layoutGraphicsItem);
	layoutGraphicsView->scale(0.05,0.05);
	splash->showMessage(tr("Creating postions..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	allPos=new allPossClass(basketTuxipConnection,oldOvenConnection,newOvenConnection,layoutScene,this);
	splash->showMessage(tr("Creating crane..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	crane=new craneClass(tuxipServer,basketTuxipConnection,allPos,layoutScene,this);
	splash->showMessage(tr("Creating exports..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	exportStationWidgetDestackerForSb=new exportStationWidgetClass(crane2ExportsDestackerForSb,tr("Destacker exports"),this);
	exportStationWidgetAnodizing=new exportStationWidgetClass(crane2ExportsAnodizing,tr("Anodizing"),this);
	exportStationWidgetPackingDestacker1=new exportStationWidgetClass(crane2ExportsPackingDestacker1Position,tr("Packing Destacker 1"),this);
	exportStationWidgetPackingDestacker2=new exportStationWidgetClass(crane2ExportsPackingDestacker2Position,tr("Packing Destacker 2"),this);

	allPos->setCraneClass(crane);
	crane->setStations();
	connect(exportStationWidgetDestackerForSb,SIGNAL(listChangedSignal()),allPos,SLOT(exportListChangedSlot()));
	connect(exportStationWidgetAnodizing,SIGNAL(listChangedSignal()),allPos,SLOT(exportListChangedSlot()));
	connect(exportStationWidgetPackingDestacker1,SIGNAL(listChangedSignal()),allPos,SLOT(exportListChangedSlot()));
	connect(exportStationWidgetPackingDestacker2,SIGNAL(listChangedSignal()),allPos,SLOT(exportListChangedSlot()));

	connect(crane->getCarriage(),SIGNAL(exportListChangedSignal()),allPos,SLOT(exportListChangedSlot()));
	connect(allPos,SIGNAL(exportListChangedSignal()),exportStationWidgetDestackerForSb,SLOT(dbUpdateTimerSlot()));
	connect(allPos,SIGNAL(exportListChangedSignal()),exportStationWidgetAnodizing,SLOT(dbUpdateTimerSlot()));
	connect(allPos,SIGNAL(exportListChangedSignal()),exportStationWidgetPackingDestacker1,SLOT(dbUpdateTimerSlot()));
	connect(allPos,SIGNAL(exportListChangedSignal()),exportStationWidgetPackingDestacker2,SLOT(dbUpdateTimerSlot()));
	//
	autoWidget=new autoWidgetClass(crane,this);
//	connect(allPos,SIGNAL(sendBasketsToCrane1Signal()),autoWidget,SLOT(sendBasketsToCrane1Slot()));
//	connect(autoWidget,SIGNAL(calculateExportGroupsSignal()),crane,SLOT(calculateExportGroupsSLot()));
	//conveyorsWidget
	conveyorsWidget=new conveyorsWidgetClass(basketTuxipConnection,this);
	conveyorsDlg=new QDialog(this);
	conveyorsDlg->setWindowTitle("Conveyors");
	QHBoxLayout *conveyorsDlgL=new QHBoxLayout();conveyorsDlg->setLayout(conveyorsDlgL);conveyorsDlgL->setMargin(2);
	conveyorsDlgL->addWidget(conveyorsWidget);
	conveyorsDlg->resize(1800,900);
	//destackerWidget
	destackerWidget=new destackerWidgetClass(basketTuxipConnection,this);
	destackerDlg=new QDialog(this);
	destackerDlg->setWindowTitle("Destacker");
	QHBoxLayout *destackerDlgL=new QHBoxLayout();destackerDlg->setLayout(destackerDlgL);destackerDlgL->setMargin(2);
	destackerDlgL->addWidget(destackerWidget);
//	destackerDlg->resize(1200,800);
	//
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

	QAction *writeBasketToReturningConveyorAction=new QAction(QIcon(":/ok.png"),tr("Send baskets to crane 1"),this);
	writeBasketToReturningConveyorAction->setToolTip(tr("Writes the basket number to position 700 for the roller to move the basket to crane 1."));
	writeBasketToReturningConveyorAction->setStatusTip(tr("Writes the basket number to position 700 for the roller to move the basket to crane 1."));
	mainToolBar->addAction(writeBasketToReturningConveyorAction);
	connect(writeBasketToReturningConveyorAction,SIGNAL(triggered()),this,SLOT(writeBasketToReturningConveyorActionSlot()));

	QAction *showConVDlgAction=new QAction(QIcon(":/autoDisabled.png"),tr("Show conveyors dialog"),this);
	connect(showConVDlgAction,SIGNAL(triggered()),this,SLOT(showConVDlgActionTriggeredSlot()));
	addAction(showConVDlgAction);

	QAction *showDestackerDlgAction=new QAction(QIcon(":/crane.png"),tr("Show destacker dialog"),this);
	connect(showDestackerDlgAction,SIGNAL(triggered()),this,SLOT(showDestackerDlgActionTriggeredSlot()));
	addAction(showDestackerDlgAction);

	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("24 feet"),"",basketTuxipConnection,"PcCom",9,6,0,"","toPcI",23,29,"","","","","",buttonStyleActive,buttonStyleO,tr("more than 24 feet"),this);mainToolBar->addWidget(pushButtonsVector.last());pushButtonsVector.last()->setMaximumWidth(150);

	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);

	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);

	mainToolBar->addSeparator();
	mainToolBar->addAction(showTuxServerAction);
	mainToolBar->addAction(showConVDlgAction);
	mainToolBar->addAction(showDestackerDlgAction);
	mainToolBar->addWidget(missionsRadioButton	=new QRadioButton(tr("Missions mode")));missionsRadioButton->setChecked(true);
	mainToolBar->addWidget(exportsRadioButton		=new QRadioButton(tr("Exports mode")));
	QWidget *spacerWdgt=new QWidget();
	spacerWdgt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	mainToolBar->addWidget(spacerWdgt);
	mainToolBar->addWidget(modifyRadioButton		=new QRadioButton(tr("Modify mode")));

	QPushButton *hmiDlgBasicButton=new QPushButton(QIcon(":/androidRed.png"),tr("Basic HMI"));
	mainToolBar->addWidget(hmiDlgBasicButton);
	connect(hmiDlgBasicButton,SIGNAL(clicked()),this,SLOT(hmiDlgBasicButtonClickedSlot()));
	hmiDlgBasic=new hmiDlgBasicClass(tuxipServer,basketTuxipConnection,this);
	QWidget *toolBoxWidget=new QWidget();QVBoxLayout *toolBoxWidgetLayout=new QVBoxLayout();toolBoxWidget->setLayout(toolBoxWidgetLayout);toolBoxWidgetLayout->setMargin(3);
	toolBox=new QToolBox(this);toolBoxWidgetLayout->addWidget(toolBox);
	toolBox->setStyleSheet("QToolBox::tab {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E1E1E9, stop: 0.4 #aaaacc,stop: 0.5 #9999bb, stop: 1.0 #D3D3D9);border-radius: 5px;color: black;}QToolBox::tab:selected {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E1E1E9, stop: 0.4 #aaaaDD,stop: 0.5 #9999D8, stop: 1.0 #D3D3D9);color: red;}");
	//QSplitter *missionsSplitter=new QSplitter(Qt::Vertical);
	//missionsSplitter->addWidget(autoWidget);
	//missionsSplitter->addWidget(exportStationWidgetDestackerForSb);
	//missionsSplitter->addWidget(exportStationWidgetAnodizing);
	//missionsSplitter->addWidget(exportStationWidgetPackingDestacker1);
	//missionsSplitter->addWidget(exportStationWidgetPackingDestacker1);
	//toolBox->addItem(missionsSplitter,QIcon(":/apexIcon.png"),tr("Automatic missions"));
	int index=0;
	toolBox->addItem(autoWidget,QIcon(":/apexIcon.png"),tr("Automatic missions"));
	toolBox->widget(index)->setToolTip(tr("Enables automatic missions"));
	index++;
	toolBox->addItem(exportStationWidgetDestackerForSb,QIcon(":/export.png"),tr("Exports to Destacker For SB"));
	toolBox->widget(index)->setToolTip(tr("Exports to destacker For SB"));
	exportStationWidgetDestackerForSb->setToolBoxClass(toolBox,index);
	index++;
	toolBox->addItem(exportStationWidgetAnodizing,QIcon(":/export.png"),tr("Exports to Anodizing"));
	toolBox->widget(index)->setToolTip(tr("Exports to Anodizing"));
	exportStationWidgetAnodizing->setToolBoxClass(toolBox,index);
	index++;
	toolBox->addItem(exportStationWidgetPackingDestacker1,QIcon(":/export.png"),tr("Exports to Packing Destacker 1"));
	toolBox->widget(index)->setToolTip(tr("Exports to Packing Destacker"));
	exportStationWidgetPackingDestacker1->setToolBoxClass(toolBox,index);
	index++;
	toolBox->addItem(exportStationWidgetPackingDestacker2,QIcon(":/export.png"),tr("Exports to Packing Destacker 2"));
	toolBox->widget(index)->setToolTip(tr("Exports to Packing Destacker"));
	exportStationWidgetPackingDestacker2->setToolBoxClass(toolBox,index);

	QSplitter *centralSplitter=new QSplitter(Qt::Horizontal);
	setCentralWidget(centralSplitter);
	statusBar();
	centralSplitter->addWidget(toolBoxWidget);
	centralSplitter->addWidget(layoutGraphicsView);
	centralSplitter->addWidget(crane->getCraneWidget());
	centralSplitter->setSizes(QList<int>()<<300<<1850-600<<300);
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
	//handShakeR2=new handShakeClass(tuxipServer->getTuxipClassWithIp(r2Ip),r2Ip,"",-1,aw1Int,30,2000,handShakeClass::typeReadOnly);
	//connect(handShakeR2,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeStateR2ChangedSlot(bool)));

	splash->showMessage(tr("Checking for open missions..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	crane->checkForNextMissionInDatabase();

	connect(missionsRadioButton,SIGNAL(clicked(bool)),this,SLOT(missionsRadioButtonClickedSlot(bool)));
	connect(modifyRadioButton,SIGNAL(clicked(bool)),this,SLOT(modifyRadioButtonClickedSlot(bool)));
	connect(exportsRadioButton,SIGNAL(clicked(bool)),this,SLOT(exportsRadioButtonClickedSlot(bool)));
	connect(toolBox,SIGNAL(currentChanged(int)),this,SLOT(toolBoxCurrentChangedSlot(int)));

	tuxipServer->setIsAutoUpdatingFromPlc(true);
	QLabel* environmentBanner = new QLabel(environment.description(), this);
	environmentBanner->setStyleSheet(environment.livePlcEnabled()
		? "font-weight:bold;padding:6px;background:#ffe2d9;color:#8a1600;"
		: "font-weight:bold;padding:6px;background:#fff3c4;color:#4b3b00;");
	statusBar()->addPermanentWidget(environmentBanner, 1);
	mainTimerSlot();
//	resetGraphicsViewZoom();
}
mainWindowClass::~mainWindowClass()
{
	log("programm ended",infoStr);
}
void mainWindowClass::handShakeCrane2ChangedSlot(bool s)
{
	showTuxServerAction->setIcon(QIcon(s?":/ok.png":":/notOk.png"));
}
void mainWindowClass::hmiDlgBasicButtonClickedSlot()
{
	hmiDlgBasic->exec();
}
void mainWindowClass::toolBoxCurrentChangedSlot(int index)
{
	if (index==0)
	{
		missionsRadioButton->setChecked(true);
		allPos->setSelectMode(allPossClass::selectForMission);
	}
	else if (index==1)
	{
		if (exportsRadioButton->isChecked())
			allPos->setSelectMode(allPossClass::selectToExportToDestackerForSb);
	}
	else if (index==2)
	{
		if (exportsRadioButton->isChecked())
			allPos->setSelectMode(allPossClass::selectToExportToAnodizing);
	}
	else if (index==3)
	{
		if (exportsRadioButton->isChecked())
			allPos->setSelectMode(allPossClass::selectToExportToPackingDestacker1Pos);
	}
	else if (index==4)
	{
		if (exportsRadioButton->isChecked())
			allPos->setSelectMode(allPossClass::selectToExportToPackingDestacker2Pos);
	}
}
void mainWindowClass::missionsRadioButtonClickedSlot(bool s)
{
	allPos->setSelectMode(allPossClass::selectForMission);
}
void mainWindowClass::modifyRadioButtonClickedSlot(bool s)
{
	allPos->setSelectMode(allPossClass::selectToModify);
}
void mainWindowClass::exportsRadioButtonClickedSlot(bool s)
{
	if (toolBox->currentIndex()==1)
		allPos->setSelectMode(allPossClass::selectToExportToDestackerForSb);
	if (toolBox->currentIndex()==2)
		allPos->setSelectMode(allPossClass::selectToExportToAnodizing);
	if (toolBox->currentIndex()==3)
		allPos->setSelectMode(allPossClass::selectToExportToPackingDestacker1Pos);
	if (toolBox->currentIndex()==4)
		allPos->setSelectMode(allPossClass::selectToExportToPackingDestacker2Pos);
	if (toolBox->currentIndex()==0)
		missionsRadioButton->setChecked(true);
}

void mainWindowClass::dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane2Ip)
	{
		allPos->dataInPlcChanged(ip,table,index,value);
		crane->dataInPlcChanged(ip,table,index,value);
		hmiDlgBasic->dataInPlcChanged(ip,table,index,value);
		handShakeCrane2->dataInPlcChanged(ip,table,index,value);
		conveyorsWidget->dataInPlcChanged(ip,table,index,value);
		destackerWidget->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
	}
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
void mainWindowClass::showConVDlgActionTriggeredSlot()
{
	conveyorsDlg->exec();
}
void mainWindowClass::showDestackerDlgActionTriggeredSlot()
{
	destackerDlg->exec();
}
void mainWindowClass::stopSlot()
{
	tuxipServer->writeInteger("toPcI",22,226,crane2Ip);
	tuxipServer->writeInteger("toPcI",22,0,crane2Ip);
}
void mainWindowClass::resetGraphicsViewZoom()
{
//	layoutGraphicsView->resetView();
	layoutGraphicsView->fitInView(-3000.f,-1750.,24000.f,14000.f,Qt::KeepAspectRatio);
}
void mainWindowClass::writeBasketToReturningConveyorActionSlot()
{
	allPos->writeBasketToReturningConveyor();
}
void mainWindowClass::mainTimerSlot()
{
	setWindowTitle(tr("Apex Basket Crane 2 [%1] - %2").arg(Crane2::Environment::instance().name()).arg(timeVerbose));
}

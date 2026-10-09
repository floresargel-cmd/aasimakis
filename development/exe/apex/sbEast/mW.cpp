#include "pch.h"
#include "mW.h"
#define inMyLaptop
bool adminMode=false;
allPossClass *globalAllPosPtr;
QStatusBar *globalStatusBar;
#ifdef inMyLaptop
QString backupDir=QDir::currentPath()+"//databaseBackups//";
#else
QString backupDir="C://assimakis//workingExes//packetCrane//databaseBackups//";
#endif
QString mainDataBaseName="apexSbWarehouse";
bool makeTicketSpinReadOnly=false;
mainWindowClass::mainWindowClass():QMainWindow()
{
	globalStatusBar=QMainWindow::statusBar();
	resize(1880,980);
#ifdef inMyLaptop
	QString mySqlServer="127.0.0.1";
#else
	QString mySqlServer=sbDbIp;
#endif
	QString mySqlUser="user";
	QString mySqlPassword=QString::fromLocal8Bit(qgetenv("ASSIMAKIS_SBEAST_MYSQLPASSWORD"));

	splash->showMessage(tr("Starting..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	setWindowIcon(QIcon(":/assimakis.png"));
	setStyleSheet(QString("QLabel,QAction,QToolButton,QToolBar,QPushButton,QToolBox::tab,QToolBox,QCheckBox,QTabWidget,QSpinBox,QTableWidget,QTableWidgetItem,QListWidget{font-size:13px;color:#000033;}"));
	setStyleSheet(QString("QLineEdit:disabled{font-size:13px;color:#330000;}"));
	tuxipServer=new tuxipServerClass(this);
	splash->showMessage(tr("Connecting to %1").arg(sbWarehouseIp),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxipClass *sbTuxipConnection=tuxipServer->addConnection(sbWarehouseIp,"sb warehouse",false);
	sbTuxipConnection->addTable("posCassette",plcTableClass::TYPE_INT,0,75,100);
	sbTuxipConnection->addTable("posSb",plcTableClass::TYPE_INT,0,75,100);
	sbTuxipConnection->addTable("posTicket",plcTableClass::TYPE_INT,0,75,100);
	sbTuxipConnection->addTable("PC2_PCcomDint",plcTableClass::TYPE_INT,0,49,100);
	sbTuxipConnection->addTable("PC2_PCcomReal",plcTableClass::TYPE_FLOAT,0,49,100);
	sbTuxipConnection->addTable("rConveyorState",plcTableClass::TYPE_INT,0,50,100);
	sbTuxipConnection->addTable("pcCom",plcTableClass::TYPE_INT,0,20,100);
	///////////////////////////////////////////////////////////////////////////////////////mDb
	splash->showMessage(tr("Connecting to main database..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QSqlDatabase mainDb;
	mainDb=QSqlDatabase::addDatabase("QMYSQL",mDb);
	mainDb.setHostName(mySqlServer);
	mainDb.setUserName(mySqlUser);
	mainDb.setPassword(mySqlPassword);
	if (!mainDb.open())
		uExit(errorStr,QString("Cannot open %1 database. Error:%2").arg(mainDb.databaseName()).arg(mainDb.lastError().text()));
	execQuery(QString("use %1").arg(mainDataBaseName),mDb);
	//layoutGraphicsView
	layoutGraphicsView=new myQGraphicsViewClass(true,this);
	layoutGraphicsView->scale(0.015,0.015);
	layoutScene=new QGraphicsScene(this);
	layoutGraphicsView->setScene(layoutScene);

	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr("Zoom extents"),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	//zoomExtentsBtn->setMaximumWidth(150);
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));

	gItemClass *layoutGraphicsItem=new gItemClass(":dxfs/layout.dxf",QPen(QColor(200,200,200)),this,Qt::NoBrush,Qt::NoBrush);
	layoutGraphicsItem->setZValue(-10.);layoutScene->addItem(layoutGraphicsItem);
	splash->showMessage(tr("Creating positions..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	allPos=new allPossClass(sbTuxipConnection,NULL,allPossClass::showPincer2Pos,layoutScene,this);
	globalAllPosPtr=allPos;

	splash->showMessage(tr("Creating pincer 2..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	pincer2Dlg=new QDialog(this);pincer2Dlg->resize(1600,1000);
	pincer2Dlg->setWindowTitle(tr("Pincer 2"));
	QVBoxLayout *pincer2DlgL=new QVBoxLayout();pincer2DlgL->setMargin(2);pincer2Dlg->setLayout(pincer2DlgL);
	pincer2=new pincer2Class(allPos,sbTuxipConnection,this);pincer2DlgL->addWidget(pincer2);

	splash->showMessage(tr("Creating pincer 2 conveyors..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	pincer2ConvDlg=new QDialog(this);pincer2ConvDlg->resize(1600,1000);
	pincer2ConvDlg->setWindowTitle(tr("Pincer 2 conveyors"));
	QVBoxLayout *pincer2ConvDlgL=new QVBoxLayout();pincer2ConvDlgL->setMargin(2);pincer2ConvDlg->setLayout(pincer2ConvDlgL);
	pincer2Conv=new pincer2ConvPositionsClass(sbTuxipConnection,allPos,this);pincer2ConvDlgL->addWidget(pincer2Conv);

	//
	connect(tuxipServer,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(dataInPlcChangedSlot(QString,QString,int,QVariant)));
	splash->showMessage(tr("Creating toolbars..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	//mainToolBar
	QToolBar *mainToolBar=new QToolBar(tr("Tools"));mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	showTuxServerAction=new QAction(QIcon(":/notOk.png"),tr("Plc comms"),this);
	showTuxServerAction->setToolTip(tr("Shows the plc communication table dialog.\nCtrl+P"));
	showTuxServerAction->setStatusTip(tr("Shows the plc communication table dialog."));
	showTuxServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	connect(showTuxServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));

	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Return west cassette"),":/arrowUp.png",sbTuxipConnection,"pcCom",9, 940,0,"","rConveyorState", 9,23,"","","","","",buttonStyle,buttonStyleActive,tr("Return west cassette"),this);
	pushButtonsVector.last()->setMaximumWidth(175);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Return east cassette"),":/arrowUp.png",sbTuxipConnection,"pcCom",9,1340,0,"","rConveyorState",13,23,"","","","","",buttonStyle,buttonStyleActive,tr("Return east cassette"),this);
	pushButtonsVector.last()->setMaximumWidth(175);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Take SB out"),":/arrowLeft.png",sbTuxipConnection,"pcCom",10,20,0,"","rConveyorState",20,22,"","","","","",buttonStyle,buttonStyleActive,tr("Take SB out"),this);
	pushButtonsVector.last()->setMaximumWidth(175);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("By pass scale check"),":/scale.png",sbTuxipConnection,"pcCom",9,1010,0,"","rConveyorState",13,24,"","","","","",buttonStyle,buttonStyleActive,tr("By pass scale check"),this);
	pushButtonsVector.last()->setMaximumWidth(175);

	showPincer2DlgBtn=new QPushButton(QIcon(":/crane.png"),tr("Pincer 2"),this);
	showPincer2DlgBtn->setToolTip(tr("Shows Pincer 2 dialog."));
	showPincer2DlgBtn->setStatusTip(tr("Shows Pincer 2 dialog."));
	connect(showPincer2DlgBtn,SIGNAL(clicked()),this,SLOT(showPincer2DlgSlot()));

	showPincer2ConvDlgBtn=new QPushButton(QIcon(":/chain.png"),tr("Pincer 2 conveyors"),this);
	showPincer2ConvDlgBtn->setToolTip(tr("Shows Pincer 2 dialog."));
	showPincer2ConvDlgBtn->setStatusTip(tr("Shows Pincer 2 dialog."));
	connect(showPincer2ConvDlgBtn,SIGNAL(clicked()),this,SLOT(showPincer2ConvDlgSlot()));

	scaleDlg=new scaleDlgClass(sbTuxipConnection,this);
	QAction *showScaleDlgAction=new QAction(QIcon(":/scale.png"),tr("Scale"),this);
	showScaleDlgAction->setToolTip(tr("Shows scale dialog."));
	showScaleDlgAction->setStatusTip(tr("Shows scale dialog."));
	connect(showScaleDlgAction,SIGNAL(triggered()),this,SLOT(showScaleDlgTriggeredSlot()));
	scaleDlg->setMyAction(showScaleDlgAction);

	newSbDlg=new newSbDlgClass(sbTuxipConnection,this);
	QAction *showNewSbDlgAction=new QAction(QIcon(":/backup.png"),tr("New SB"),this);
	showNewSbDlgAction->setToolTip(tr("Shows new SB dialog."));
	showNewSbDlgAction->setStatusTip(tr("Shows new SB dialog."));
	connect(showNewSbDlgAction,SIGNAL(triggered()),this,SLOT(showNewSbDlgTriggeredSlot()));

	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);

	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);

	mainToolBar->addAction(showTuxServerAction);
	mainToolBar->addWidget(showPincer2DlgBtn);
	mainToolBar->addWidget(showPincer2ConvDlgBtn);
	mainToolBar->addAction(showNewSbDlgAction);
	mainToolBar->addWidget(pushButtonsVector[0]);
	mainToolBar->addWidget(pushButtonsVector[1]);
	mainToolBar->addWidget(pushButtonsVector[2]);
	mainToolBar->addWidget(pushButtonsVector[3]);
	mainToolBar->addAction(showScaleDlgAction);

	mainToolBar->addSeparator();
	mainToolBar->addWidget(messageLabel=new QLabel());
	mainToolBar->addSeparator();
	QWidget *toolBoxWidget=new QWidget();QVBoxLayout *toolBoxWidgetLayout=new QVBoxLayout();toolBoxWidget->setLayout(toolBoxWidgetLayout);toolBoxWidgetLayout->setMargin(3);
	QToolBox *toolBox=new QToolBox(this);toolBoxWidgetLayout->addWidget(toolBox);
	toolBox->setStyleSheet("QToolBox::tab{background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #E1E1E9,stop: 0.4 #aaaacc,stop:0.5 #9999bb,stop:1.0 #D3D3D9);border-radius:5px;color:black;}"
												 "QToolBox::tab:selected{background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #E1E1E9,stop: 0.4 #aaaaDD,stop:0.5 #9999D8,stop:1.0 #D3D3D9);color: red;}"
												);
	int index=0;
	exportTriple1=new exportStationWidgetClass(exportsTriple1Table,tr("Exports to triple west"),toolBox,index,NULL,true,this);
	toolBox->addItem(exportTriple1,QIcon(":/export.png"),tr("Exports to triple west"));

	index++;
	exportTriple2=new exportStationWidgetClass(exportsTriple2Table,tr("Exports to triple east"),toolBox,index,NULL,true,this);
	toolBox->addItem(exportTriple2,QIcon(":/export.png"),tr("Exports to triple east"));
	index++;
	exportDouble=new exportStationWidgetClass(exportsDoubleTable,tr("Exports to double"),toolBox,index,NULL,true,this);
	toolBox->addItem(exportDouble,QIcon(":/export.png"),tr("Exports to double"));

	splash->showMessage(tr("Creating search dlg..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	searchWidgetClass *searchWidget=new searchWidgetClass(allPos,this);
	toolBox->addItem(searchWidget,QIcon(":/search.png"),tr("Search"));
	toolBox->widget(index)->setToolTip(tr("Search the warehouse."));
	//
	QWidget *mainWidget=new QWidget();
	setCentralWidget(mainWidget);
	QVBoxLayout *mainL=new QVBoxLayout();mainWidget->setLayout(mainL);mainL->setMargin(0);
	QSplitter *centralSplitter=new QSplitter(Qt::Horizontal);
	centralSplitter->setStyleSheet(splitterHorizontalStyle);
	mainL->addWidget(mainToolBar);
	mainL->addWidget(centralSplitter);
	//statusBar();
	centralSplitter->addWidget(toolBoxWidget);
	centralSplitter->addWidget(layoutGraphicsView);
	centralSplitter->setSizes(QList<int>()<<300<<1600);
	//
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer->start(1000);
	QTimer *statisticsTimer=new QTimer(this);
	connect(statisticsTimer,SIGNAL(timeout()),this,SLOT(statisticsTimerSlot()));
	statisticsTimer->start(30000);
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	splash->showMessage(tr("Creating hand shake..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));

	handShakeSbCrane=new handShakeClass(sbTuxipConnection,sbWarehouseIp,"",-1,"pcCom",11,2000,handShakeClass::typeReadOnly);
	connect(handShakeSbCrane,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeSbCraneStateChangedSlot(bool)));
	splash->showMessage(tr("Reading data from plc..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxipServer->setIsAutoUpdatingFromPlc(true);
	statisticsTimerSlot();
	splash->showMessage(tr("Remote backup..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	splash->showMessage(tr("Starting..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
}
mainWindowClass::~mainWindowClass()
{
	delete splash;
	delete handShakeSbCrane;
	log("programm ended",infoStr);
}
void mainWindowClass::setPlcComments()
{
}
void mainWindowClass::showScaleDlgTriggeredSlot()
{
	scaleDlg->exec();
}
void mainWindowClass::showNewSbDlgTriggeredSlot()
{
	newSbDlg->exec();
}
void mainWindowClass::handShakeSbCraneStateChangedSlot(bool s)
{
	bool isOk=s;
	showTuxServerAction->setIcon(QIcon(isOk?":/ok.png":":/notOk.png"));
}
void mainWindowClass::dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value)
{
	if (ip==sbWarehouseIp)
	{
		allPos->dataInPlcChanged(ip,table,index,value);
		handShakeSbCrane->dataInPlcChanged(ip,table,index,value);
		pincer2->dataInPlcChanged(ip,table,index,value);
		pincer2Conv->dataInPlcChanged(ip,table,index,value);
		scaleDlg->dataInPlcChanged(ip,table,index,value);
		newSbDlg->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		if ((table=="PC2_PCcomDint")&&(index==6))
			showPincer2DlgBtn->setStyleSheet(getValueOfBit(value.toInt(),29)?buttonStyleRed:buttonStyleActive);
		if ((table=="rConveyorState")&&(index==8))
			showPincer2ConvDlgBtn->setStyleSheet(getValueOfBit(value.toInt(),8)?buttonStyleActive:buttonStyleRed);
	}
}
void mainWindowClass::showServerSlot()
{
	if (getPassword("Password is needed to open this window.","Please give the password",passStrong))
		tuxipServer->showDialog();
}
void mainWindowClass::showPincer2DlgSlot()
{
	pincer2Dlg->exec();
}
void mainWindowClass::showPincer2ConvDlgSlot()
{
	pincer2ConvDlg->exec();
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("apes SB warehouse %1").arg(uCompilerData));
}
void mainWindowClass::stopSlot()
{
	tuxipServer->writeInteger("bRobotToPcI",22,226,sbWarehouseIp);
	tuxipServer->writeInteger("bRobotToPcI",22,0,sbWarehouseIp);
}
void mainWindowClass::resetGraphicsViewZoom()
{
	layoutScene->setSceneRect(layoutScene->itemsBoundingRect());
	layoutGraphicsView->fitInView(layoutGraphicsView->sceneRect(),Qt::KeepAspectRatio);
}
void mainWindowClass::mainTimerSlot()
{
	setWindowTitle(tr("apex east station %1 %2","main window title..").arg(timeVerbose).arg(adminMode?tr("(Administrator mode)"):""));
}
void mainWindowClass::statisticsTimerSlot()
{
	QString txt;
	txt+=tr("Total:%1 ").arg(getFirst(QString("select count(tId) from pos where posId<%1").arg(craneId),mDb).toInt());
	txt+=tr("Available:%1 ").arg(getFirst(QString("select count(tId) from pos where posId<%1 and locked=0").arg(craneId),mDb).toInt());
	txt+=tr("Empty cassettes:%1 ").arg(getFirst(QString("select count(tId) from pos where posId<%1 and locked=0 and sbNumber=0").arg(craneId),mDb).toInt());
	txt+=tr("Empty SB:%1 ").arg(getFirst(QString("select count(tId) from pos where posId<%1 and locked=0 and sbNumber<>0 and ticket=0").arg(craneId),mDb).toInt());
	txt+=tr("Number of tickets:%1 ").arg(getFirst(QString("select count(tId) from pos where posId<%1 and ticket<>0").arg(craneId),mDb).toInt());
	globalStatusBar->showMessage(txt);

	QString messStr;
	QVector<QVector<QVariant>> doubleCassettesVV=execTableQuery(QString("select cassette,count(cassette) from pos where cassette<>0 group by cassette having count(cassette)>1"),mDb);
	for (int i=0;i<doubleCassettesVV.count();i++)
	{
		int cassette=doubleCassettesVV[i][0].toInt();
		QVector<QVector<QVariant>> doubleCassettesVV=execTableQuery(QString("select posId,description from pos where cassette=%1").arg(cassette),mDb);
		messStr+=tr("Cassette %1 is in positions:").arg(cassette);
		for (int j=0;j<doubleCassettesVV.count();j++)
			messStr+=tr("%1(%2) ").arg(doubleCassettesVV[j][0].toInt()).arg(doubleCassettesVV[j][1].toString().trimmed());
	}
	QVector<QVector<QVariant>> doubleTicketVV=execTableQuery(QString("select ticket,count(ticket) from pos where ticket<>0 group by ticket having count(ticket)>1"),mDb);
	for (int i=0;i<doubleTicketVV.count();i++)
	{
		int ticket=doubleTicketVV[i][0].toInt();
		QVector<QVector<QVariant>> doubleticketsVV=execTableQuery(QString("select posId,description from pos where ticket=%1").arg(ticket),mDb);
		messStr+=tr("Ticket %1 is in positions:").arg(ticket);
		for (int j=0;j<doubleticketsVV.count();j++)
			messStr+=tr("%1(%2) ").arg(doubleticketsVV[j][0].toInt()).arg(doubleticketsVV[j][1].toString().trimmed());
	}
	QVector<QVector<QVariant>> noSbTicketVV=execTableQuery(QString("select posId,description from pos where ticket<>0 and sbNumber=0"),mDb);
	for (int i=0;i<doubleTicketVV.count();i++)
		messStr+=tr("Position %1(%2) has ticket and does not have a SB ").arg(noSbTicketVV[i][0].toInt()).arg(noSbTicketVV[i][1].toString().trimmed());
	QVector<QVector<QVariant>> noCasssetreSbVV=execTableQuery(QString("select posId,description from pos where ticket<>0 and sbNumber=0"),mDb);
	for (int i=0;i<noCasssetreSbVV.count();i++)
		messStr+=tr("Position %1(%2) has SB and does not have a cassette ").arg(noCasssetreSbVV[i][0].toInt()).arg(noCasssetreSbVV[i][1].toString().trimmed());

	if (!messStr.isEmpty())
		messageLabel->setStyleSheet(QString("QLabel{color:#550000;}"));
	else
	{
		messStr=tr("Ok");
		messageLabel->setStyleSheet(QString("QLabel{color:#005500;}"));
	}
	messageLabel->setText(messStr);
}

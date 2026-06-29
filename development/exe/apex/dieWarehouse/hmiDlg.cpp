#include "pch.h"
#include "hmiDlg.h"
hmiDlgClass::hmiDlgClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip_,bool useTranslation,craneClass *crane_):QDialog(crane_)
{
	crane=crane_;
	setWindowTitle(tr("Basket crane HMI"));
	tuxip=tuxip_;
	resize(1800,1000);
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);
	setLayout(mainLayout);
	QSplitter *centralSplitter=new QSplitter(Qt::Vertical);mainLayout->addWidget(centralSplitter);
	centralSplitter->setStyleSheet(splitterVerticalStyle);

	QScrollArea *mainScrollArea=new QScrollArea();mainScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	mainScrollArea->setWidgetResizable(true);
	QWidget *mainScrollAreaWidget=new QWidget();
	QHBoxLayout *mainScrollAreaLayout=new QHBoxLayout();mainScrollAreaLayout->setMargin(2);mainScrollAreaWidget->setLayout(mainScrollAreaLayout);
	mainScrollArea->setWidget(mainScrollAreaWidget);
	centralSplitter->addWidget(mainScrollArea);
	QVBoxLayout *layout0=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout0);
	QVBoxLayout *layout1=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout1);
	QVBoxLayout *layout2=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout2);
	QWidget *warningsErrorsWidget=new QWidget();
	QHBoxLayout *warningsErrorsLayout=new QHBoxLayout();warningsErrorsWidget->setLayout(warningsErrorsLayout);
	bitErrorsVector=new bitLogVectorClass("bRobotToPcI","plcAlarms.txt",bitClass::TYPE_SHOW_HIDE,STYLE_R,STYLE_GRAY,db,"Errors","plcMessages","",useTranslation,bitLogHisoryClass::typeMsSql,this);
	bitWarningsVector=new bitLogVectorClass("bRobotToPcI","plcWarnings.txt",bitClass::TYPE_SHOW_HIDE,STYLE_O,STYLE_GRAY,db,"Warnings","plcMessages","",useTranslation,bitLogHisoryClass::typeMsSql,this);
	warningsErrorsLayout->addWidget(bitWarningsVector);
	warningsErrorsLayout->addWidget(bitErrorsVector);
	centralSplitter->addWidget(warningsErrorsWidget);

	//layout0
	//Horizontal movement
	QVBoxLayout *horizontalPosGroupBoxMainLayout=new QVBoxLayout();layout0->addLayout(horizontalPosGroupBoxMainLayout);
	QGroupBox *horizontalPosGroupBox=new QGroupBox(tr("Horizontal movement"));horizontalPosGroupBoxMainLayout->addWidget(horizontalPosGroupBox);
	QGridLayout *horizontalPosGroupBoxLayout=new QGridLayout();horizontalPosGroupBox->setLayout(horizontalPosGroupBoxLayout);
	int row=0;
	progressVector<<new progressClass(tuxip,dieIp,"bRobotToPcF",9,tr("Velocity"),tr("m/min"),-120.,120.,20.,200,200,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","","","",180,1,this);horizontalPosGroupBoxLayout->addWidget(progressVector.last(),row,0,1,2,Qt::AlignHCenter|Qt::AlignTop);
	plotsVector<<new plotClass(dieIp,"bRobotToPcF",8,tr("Horizontal Hz"),this);
	plotsVector.last()->addNewGraph(dieIp,"bRobotToPcF",0,tr("Position[mm]"),1,0);
	progressVector.last()->setPlotClass(plotsVector.last());

	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",2,0,100000.,tr("Target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);horizontalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0);
	bitVector<<new bitClass("bRobotToPcI",23,22,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),STYLE_G,STYLE_GRAY,"","","","",this);horizontalPosGroupBoxLayout->addWidget(bitVector.last(),row,1);
	bitVector.last()->setMinimumWidth(150);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",0,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);horizontalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",8,0,100.,tr("Hz"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);horizontalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Fwd"),":/arrowUp.png",tuxip,"bRobotToPcI",22,1,0,"","bRobotToPcI",24,3,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Fwd"),this);horizontalPosGroupBoxLayout->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Bwd"),":/arrowDown.png",tuxip,"bRobotToPcI",22,2,0,"","bRobotToPcI",24,4,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Bwd"),this);horizontalPosGroupBoxLayout->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset"),":/androidRed.png",tuxip,"bRobotToPcI",22,100,0,"","bRobotToPcI",21,2,"","","","","",BUTTON_STYLE,BUTTON_STYLE_RED,tr("Reset"),this);horizontalPosGroupBoxLayout->addWidget(pushButtonsVector.last(),row,0);
	bitVector<<new bitClass("bRobotToPcI",21,2,bitClass::TYPE_CHANGE_COLOR,tr("Reseting..."),tr("Not reseting"),STYLE_R,STYLE_GRAY,"","","","",this);horizontalPosGroupBoxLayout->addWidget(bitVector.last(),row,1);
	bitVector.last()->setMinimumWidth(150);
	//Vertical movement
	QVBoxLayout *verticalPosGroupBoxMainLayout=new QVBoxLayout();layout0->addLayout(verticalPosGroupBoxMainLayout);
	QGroupBox *verticalPosGroupBox=new QGroupBox(tr("Vertical movement"));verticalPosGroupBoxMainLayout->addWidget(verticalPosGroupBox);
	QGridLayout *verticalPosGroupBoxLayout=new QGridLayout();verticalPosGroupBox->setLayout(verticalPosGroupBoxLayout);
	row=0;
	progressVector<<new progressClass(tuxip,dieIp,"bRobotToPcF",11,tr("Velocity"),tr("m/min"),-40.,40.,10.,200,200,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","","","",180,1,this);verticalPosGroupBoxLayout->addWidget(progressVector.last(),row,0,1,2,Qt::AlignHCenter|Qt::AlignTop);
	plotsVector<<new plotClass(dieIp,"bRobotToPcF",10,tr("Vertical Hz"),this);
	progressVector.last()->setPlotClass(plotsVector.last());
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",3,0,100000.,tr("Target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);verticalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0);
	bitVector<<new bitClass("bRobotToPcI",23,23,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),STYLE_G,STYLE_GRAY,"","","","",this);verticalPosGroupBoxLayout->addWidget(bitVector.last(),row,1);
	bitVector.last()->setMinimumWidth(150);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",1,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);verticalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",10,0,100.,tr("Hz"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);verticalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Up"),":/arrowUp.png",tuxip,"bRobotToPcI",22,3,0,"","bRobotToPcI",24,5,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Up"),this);verticalPosGroupBoxLayout->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Down"),":/arrowDown.png",tuxip,"bRobotToPcI",22,4,0,"","bRobotToPcI",24,6,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Down"),this);verticalPosGroupBoxLayout->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset"),":/androidRed.png",tuxip,"bRobotToPcI",22,101,0,"","bRobotToPcI",21,1,"","","","","",BUTTON_STYLE,BUTTON_STYLE_RED,tr("Reset"),this);verticalPosGroupBoxLayout->addWidget(pushButtonsVector.last(),row,0);
	bitVector<<new bitClass("bRobotToPcI",21,1,bitClass::TYPE_CHANGE_COLOR,tr("Reseting..."),tr("Not reseting"),STYLE_R,STYLE_GRAY,"","","","",this);verticalPosGroupBoxLayout->addWidget(bitVector.last(),row,1);
	//Left anchor
	QVBoxLayout *leftAnchorGroupBoxMainLayout=new QVBoxLayout();layout0->addLayout(leftAnchorGroupBoxMainLayout);
	QGroupBox *leftAnchorGroupBox=new QGroupBox(tr("Anchor"));leftAnchorGroupBoxMainLayout->addWidget(leftAnchorGroupBox);
	QHBoxLayout *leftAnchorGroupBoxLayout=new QHBoxLayout();leftAnchorGroupBox->setLayout(leftAnchorGroupBoxLayout);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Out"),":/arrowLeft.png",tuxip,"bRobotToPcI",22,17,0,"","bRobotToPcI",24,22,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Out"),this);leftAnchorGroupBoxLayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("In"),":/arrowRight.png",tuxip,"bRobotToPcI",22,18,0,"","bRobotToPcI",24,21,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("In"),this);leftAnchorGroupBoxLayout->addWidget(pushButtonsVector.last());
	////Right anchor
	//QVBoxLayout *rightAnchorGroupBoxMainLayout=new QVBoxLayout();layout0->addLayout(rightAnchorGroupBoxMainLayout);
	//QGroupBox *rightAnchorGroupBox=new QGroupBox(tr("Right Anchor"));rightAnchorGroupBoxMainLayout->addWidget(rightAnchorGroupBox);
	//QHBoxLayout *rightAnchorGroupBoxLayout=new QHBoxLayout();rightAnchorGroupBox->setLayout(rightAnchorGroupBoxLayout);
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Out"),":/arrowRight.png",tuxip,"bRobotToPcI",22,20,0,"","bRobotToPcI",24,24,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Out"),this);rightAnchorGroupBoxLayout->addWidget(pushButtonsVector.last());
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("In"),":/arrowLeft.png",tuxip,"bRobotToPcI",22,19,0,"","bRobotToPcI",24,23,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("In"),this);rightAnchorGroupBoxLayout->addWidget(pushButtonsVector.last());

	layout0->addStretch(1);
	//layout1
	QSplitter *layout1Splitter=new QSplitter(Qt::Vertical);layout1->addWidget(layout1Splitter);
	QWidget *layout1SplitterUpWidget=new QWidget();layout1Splitter->addWidget(layout1SplitterUpWidget);
	QWidget *layout1SplitterDownWidget=new QWidget();layout1Splitter->addWidget(layout1SplitterDownWidget);
	QVBoxLayout *layout1SplitterUpWidgetL=new QVBoxLayout();layout1SplitterUpWidget->setLayout(layout1SplitterUpWidgetL);
	QVBoxLayout *layout1SplitterDownWidgetL=new QVBoxLayout();layout1SplitterDownWidget->setLayout(layout1SplitterDownWidgetL);
	myQGraphicsViewClass *layoutGraphicsView=new myQGraphicsViewClass(false,this);
	layoutGraphicsView->scale(0.1,0.1);
	layoutGraphicsView->resize(1800,800);
	//layoutGraphicsView->setMinimumWidth(750);
	//layoutGraphicsView->setMinimumHeight(750);
	QGraphicsScene *layoutScene=new QGraphicsScene(this);

	dxfGroupOfGraphicsItemClass *layoutGraphicsItem=new dxfGroupOfGraphicsItemClass(":dxfs/hmiTopView.dxf",QPen(QColor(200,200,200)),false,false,QColor(),this);
	layoutGraphicsItem->setZValue(-10.);layoutScene->addItem(layoutGraphicsItem);
	hooksItem=new dxfGroupOfGraphicsItemClass(":dxfs/hmiHook.dxf",QPen(QColor(150,150,255)),false,false,QColor(),this);
	layoutScene->addItem(hooksItem);

	dxfGroupOfGraphicsItemClass *proxOnItem=new dxfGroupOfGraphicsItemClass(":dxfs/proximitySwitchOn.dxf",QPen(QColor(75,150,75),25),false,false,QColor(),this);
	dxfGroupOfGraphicsItemClass *proxOnRedItem=new dxfGroupOfGraphicsItemClass(":dxfs/proximitySwitchOn.dxf",QPen(QColor(150,75,75),25),false,false,QColor(),this);
	dxfGroupOfGraphicsItemClass *proxOffItem=new dxfGroupOfGraphicsItemClass(":dxfs/proximitySwitchOff.dxf",QPen(QColor(150,150,150),10),false,false,QColor(),this);
	//reset
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.1 on","23.1 off",23,1,+1900.f,500.f,tr("X travel reset proxmity switch"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.0 on","23.0 off",23,0,-1800.f,1300.f,tr("Z travel reset proxmity switch"),this);
	//anchor left 
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.2 on","23.2 off",23,2,-500.f,-450.f,tr("Anchors left in magnetic switch"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.3 on","23.3 off",23,3,-750.f,-550.f,tr("Anchors left out magnetic switch"),this);
	//anchor right 
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.4 on","23.4 off",23,4,+500.f,-450.f,tr("Anchors right in magnetic switch"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.5 on","23.5 off",23,5,+750.f,-550.f,tr("Anchors right out magnetic switch"),this);
	//basket A
//	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.6 on","23.6 off",23,6,-1500.f,300.f,tr("Basket number 1 proxmity switch"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.7 on","23.7 off",23,7,-750.f,300.f,tr("Basket number 1 proxmity switch"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.8 on","23.8 off",23,8,+750.f,300.f,tr("Basket number 2 proxmity switch"),this);
//	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.9 on","23.9 off",23,9,+750.f,300.f,tr("Basket number 4 proxmity switch"),this);
	//hooks reset A
	proxVector << new proxItemClass(layoutScene, proxOnItem, proxOffItem, "23.14 on", "23.14 off", 23, 14, -2000.f, 200.f, tr("Hooks left reset proximity switch"), this);
	proxVector << new proxItemClass(layoutScene, proxOnItem, proxOffItem, "23.15 on", "23.15 off", 23, 15, +2000.f, 200.f, tr("Hooks right reset proximity switch"), this);
	//hooks up/down A
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.18 on","23.18 off",23,18,-200.f,+500.f,tr("Hooks up magnetic switch"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.19 on","23.19 off",23,19,-0.f,+400.f,tr("Hooks down magnetic proximity switch"),this);
	//vertical safety
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.29 on","23.29 off",23,29,-1500.f,800.f,tr("Left vertical safety off magnetic switch"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.30 on","23.30 off",23,30,-1500.f,700.f,tr("Left vertical safety on magnetic switch"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.29 on","23.29 off",23,29,+1500.f,800.f,tr("Right vertical safety off magnetic switch"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"23.30 on","23.30 off",23,30,+1500.f,700.f,tr("Right vertical safety on magnetic switch"),this);
	//photocell
	proxVector << new proxItemClass(layoutScene, proxOnRedItem, proxOnItem, "27.27 on", "27.27 off", 27, 27, -2000.f,-450.f, tr("Cassette position photo cell"), this);
	layoutGraphicsView->setScene(layoutScene);
	layout1SplitterUpWidgetL->addWidget(layoutGraphicsView);
	//auto manual stop alarm
	QHBoxLayout *autoLayout=new QHBoxLayout();layout1SplitterUpWidgetL->addLayout(autoLayout);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",tuxip,"bRobotToPcI",22,21,0,"","bRobotToPcI",24,0,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Auto"),this);autoLayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Maintenance"),"",tuxip,"bRobotToPcI",22,22,0,"","bRobotToPcI",24,1,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Maintenance"),this);autoLayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Home position"),"",tuxip,"bRobotToPcI",22,166,0,"","bRobotToPcI",27,28,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Home position"),this);autoLayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Active Stop"),"",tuxip,"bRobotToPcI",22,226,0,"","bRobotToPcI",23,24,"","","","","",BUTTON_STYLE_ACTIVE,BUTTON_STYLE_RED,tr("No Active Stop"),this);autoLayout->addWidget(pushButtonsVector.last());
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Stop Recall"),"",tuxip,"bRobotToPcI",22,27,0,"","bRobotToPcI",-1,-1,"","","","","",BUTTON_STYLE,BUTTON_STYLE_RED,tr("Stop Recall"),this);autoLayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Alarms"),"",tuxip,"bRobotToPcI",22,23,0,"","bRobotToPcI",24,2,"","","","","",BUTTON_STYLE_ACTIVE,BUTTON_STYLE_RED,tr("No Alarms"),this);autoLayout->addWidget(pushButtonsVector.last());
	//camera-missions
	missionsTabWidget=new QTabWidget();layout1SplitterDownWidgetL->addWidget(missionsTabWidget);
	QWidget *missionsWidget=new QWidget();missionsTabWidget->addTab(missionsWidget,tr("Missions"));
	connect(missionsTabWidget,SIGNAL(tabBarDoubleClicked(int)),this,SLOT(cameraMissionsTabWidgetTabBarDoubleClickedSlot(int)));
	missionsTabWidget->tabBar()->setTabIcon(0,QIcon(":/lock.png"));
	QList<QTabBar*> list=missionsTabWidget->findChildren<QTabBar*>();
  list.at(0)->setCursor(Qt::PointingHandCursor);
	//cameras
	//missions
	QHBoxLayout *missionsMainLayout=new QHBoxLayout();missionsWidget->setLayout(missionsMainLayout);
	//missions A
	QGroupBox *missionGroupBox=new QGroupBox(tr("Mission"));missionsMainLayout->addWidget(missionGroupBox);
	QVBoxLayout *missionGroupBoxL=new QVBoxLayout();missionGroupBox->setLayout(missionGroupBoxL);

	QGroupBox *missionNextGroupBox=new QGroupBox(tr("Next Mission"));missionGroupBoxL->addWidget(missionNextGroupBox);
	QVBoxLayout *missionNextGroupBoxL=new QVBoxLayout();missionNextGroupBox->setLayout(missionNextGroupBoxL);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,dieIp,"bRobotToPcI",0,1,3000,tr("Next from:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionNextGroupBoxL->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,dieIp,"bRobotToPcI",1,1,3000,tr("Next to:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionNextGroupBoxL->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,dieIp,"bRobotToPcI",2,1,5,tr("Status:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionNextGroupBoxL->addWidget(intPlcMemoryVector.last());
	QPushButton *clearNextMissionBtn=new QPushButton(tr("Clear Next Mission"));missionNextGroupBoxL->addWidget(clearNextMissionBtn);
	connect(clearNextMissionBtn,SIGNAL(clicked()),this,SLOT(clearNextMissionBtnClickedSlot()));

	QGroupBox *missionActiveGroupBox=new QGroupBox(tr("Active Mission"));missionGroupBoxL->addWidget(missionActiveGroupBox);
	QVBoxLayout *missionActiveGroupBoxL=new QVBoxLayout();missionActiveGroupBox->setLayout(missionActiveGroupBoxL);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,dieIp,"bRobotToPcI",10,1,3000,tr("Active From:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionActiveGroupBoxL->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,dieIp,"bRobotToPcI",11,1,3000,tr("Active To:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionActiveGroupBoxL->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,dieIp,"bRobotToPcI",12,1,5,tr("PC Status:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionActiveGroupBoxL->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,dieIp,"bRobotToPcI",13,1,5,tr("PLC Status:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionActiveGroupBoxL->addWidget(intPlcMemoryVector.last());
	craneStep=new QLabel();missionActiveGroupBoxL->addWidget(craneStep);
	hooksStep=new QLabel();missionActiveGroupBoxL->addWidget(hooksStep);
	QHBoxLayout *endAbordLayout=new QHBoxLayout();missionActiveGroupBoxL->addLayout(endAbordLayout);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Abord Active Mission"),"",tuxip,"bRobotToPcI",22,26,0,"","bRobotToPcI",21,15,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Abord Active Mission"),this);endAbordLayout->addWidget(pushButtonsVector.last());
	abordActiveMission=pushButtonsVector.last();
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Continue Active Mission"),"",tuxip,"bRobotToPcI",22,48,0,"","bRobotToPcI",21,16,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Continue Active Mission"),this);endAbordLayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("End Active Mission"),"",tuxip,"bRobotToPcI",22,24,0,"","bRobotToPcI",21,17,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("End Active Mission"),this);endAbordLayout->addWidget(pushButtonsVector.last());
	endActiveMission=pushButtonsVector.last();
	clearActiveMission=new QPushButton(tr("Clear Active Mission"));missionActiveGroupBoxL->addWidget(clearActiveMission);
	connect(clearActiveMission,SIGNAL(clicked()),this,SLOT(clearActiveMissionBtnClickedSlot()));
	//layout1->addStretch(1);
//layout2
	//
	QVBoxLayout *hooksL=new QVBoxLayout();layout2->addLayout(hooksL);
	QGroupBox *hooksGroupBox=new QGroupBox(tr("Hooks"));hooksL->addWidget(hooksGroupBox);
	QGridLayout *hooksGroupBoxL=new QGridLayout();hooksGroupBox->setLayout(hooksGroupBoxL);
	row=0;
	progressVector<<new progressClass(tuxip,dieIp,"bRobotToPcF",17,tr("Velocity"),tr("m/min"),-80.,80.,20.,200,200,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","","","",180,1,this);hooksGroupBoxL->addWidget(progressVector.last(),row,0,1,2,Qt::AlignHCenter|Qt::AlignTop);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",6,0,100000.,tr("Target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);hooksGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,0);
	bitVector<<new bitClass("bRobotToPcI",24,7,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),STYLE_G,STYLE_GRAY,"","","","",this);hooksGroupBoxL->addWidget(bitVector.last(),row,1);
	bitVector.last()->setMinimumWidth(150);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",4,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);hooksGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",12,0,100.,tr("Hz"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);hooksGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Left"),":/arrowLeft.png",tuxip,"bRobotToPcI",22,7,0,"","bRobotToPcI",24,9,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Left"),this);hooksGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Right"),":/arrowRight.png",tuxip,"bRobotToPcI",22,8,0,"","bRobotToPcI",24,10,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Right"),this);hooksGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Up"),":/arrowUp.png",tuxip,"bRobotToPcI",22,5,0,"","bRobotToPcI",24,17,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Up"),this);hooksGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Down"),":/arrowDown.png",tuxip,"bRobotToPcI",22,6,0,"","bRobotToPcI",24,18,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Down"),this);hooksGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	//
	//QVBoxLayout *dieCarL=new QVBoxLayout();layout2->addLayout(dieCarL);
	//QGroupBox *dieCarGroupBox=new QGroupBox(tr("Die car"));dieCarL->addWidget(dieCarGroupBox);
	//QGridLayout *dieCarGroupBoxL=new QGridLayout();dieCarGroupBox->setLayout(dieCarGroupBoxL);
	//row=0;
	//bitVector<<new bitClass("bRobotToPcI",24,28,bitClass::TYPE_CHANGE_COLOR,tr("Operator position"),tr("Operator position"),STYLE_G,STYLE_GRAY,"","","","",this);dieCarGroupBoxL->addWidget(bitVector.last(),row,0);
	//bitVector.last()->setMinimumWidth(150);
	//bitVector<<new bitClass("bRobotToPcI",24,29,bitClass::TYPE_CHANGE_COLOR,tr("Warehouse position"),tr("Warehouse position"),STYLE_G,STYLE_GRAY,"","","","",this);dieCarGroupBoxL->addWidget(bitVector.last(),row,1);
	//bitVector.last()->setMinimumWidth(150);
	//row++;
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("To operator"),":/arrowDown.png",tuxip,"bRobotToPcI",22,30,0,"","bRobotToPcI",24,30,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("To operator"),this);dieCarGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("To warehouse"),":/arrowUp.png",tuxip,"bRobotToPcI",22,31,0,"","bRobotToPcI",24,31,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("To warehouse"),this);dieCarGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	//
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auxiliary on"),":/ok.png",tuxip,"bRobotToPcI",22,104,0,"","bRobotToPcI",24,27,"","","","","",BUTTON_STYLE_RED,BUTTON_STYLE_ACTIVE,tr("Auxiliary off"),this);
	layout2->addWidget(pushButtonsVector.last());
	QPushButton *settingsBtn=new QPushButton(QIcon(":/autoEnabled.png"),tr("Settings"),this);
	layout2->addWidget(settingsBtn);
	connect(settingsBtn,SIGNAL(clicked(bool)),this,SLOT(settingsBtnClickedSlot()));
	layout2->addStretch(1);
	//
	centralSplitter->setSizes(QList<int>()<<height()-100<<100);
	settings=new hmiSettingsClass(tuxip,this);
	//

	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);
}
hmiDlgClass::~hmiDlgClass()
{
}
void hmiDlgClass::cameraMissionsTabWidgetTabBarDoubleClickedSlot(int index)
{
	if (index==0)
		lockAdvancedBtns(!areAdvancedButtonsLocked);
}
void hmiDlgClass::lockAdvancedBtns(bool l)
{
	if ((!l)&&(!adminMode))
	{
		passwordClass *pass=new passwordClass(this);
		pass->exec();
		if (!pass->getPasswordIsCorrect())
			return;
	}
	areAdvancedButtonsLocked=l;
	if (areAdvancedButtonsLocked)
	{
		clearActiveMission->setEnabled(false);
		abordActiveMission->setEnabled(false);
		endActiveMission->setEnabled(false);
		missionsTabWidget->tabBar()->setTabIcon(0,QIcon(":/lock.png"));
	}
	else
	{
		clearActiveMission->setEnabled(true);
		abordActiveMission->setEnabled(true);
		endActiveMission->setEnabled(true);
		missionsTabWidget->tabBar()->setTabIcon(0,QIcon(":/unLock.png"));
	}
}
void hmiDlgClass::stopSlot()
{
	tuxip->writeInteger("bRobotToPcI",22,226);
	tuxip->writeInteger("bRobotToPcI",22,0);
}
void hmiDlgClass::setCraneConveyorClass(craneConveyorClass *c)
{
	craneConveyor=c;
}
int hmiDlgClass::exec()
{
	//if (adminMode)
	//	return QDialog::exec();
	lockAdvancedBtns(true);
	//passwordClass *pass=new passwordClass(this);
	//pass->exec();
	//if (pass->getPasswordIsCorrect())
		return QDialog::exec();
	//return 0;
}
void hmiDlgClass::clearNextMissionBtnClickedSlot()
{
	craneConveyor->setNextFromPos(0);
	craneConveyor->setNextToPos(0);
	tuxip->writeInteger("bRobotToPcI",0,0);
	tuxip->writeInteger("bRobotToPcI",1,0);
	tuxip->writeInteger("bRobotToPcI",2,0);
}
void hmiDlgClass::clearActiveMissionBtnClickedSlot()
{
	if (adminMode)
		craneConveyor->clearMissionInDb("user");
	passwordClass *pass=new passwordClass(this);
	pass->exec();
	if (pass->getPasswordIsCorrect())
		craneConveyor->clearMissionInDb("user");
}
void hmiDlgClass::settingsBtnClickedSlot()
{
	settings->exec();
}
void hmiDlgClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (dieIp==ip)
	{
		if("bRobotToPcI"==table)
		{
			if ((index==21)||(index==23)||(index==24)||(index==27))
			{
				for (int i=0;i<proxVector.count();i++)
					proxVector[i]->dataInPlcChanged(ip,table,index,value);
				for (int i=0;i<bitVector.count();i++)
					bitVector[i]->updateValue(table,index,value.toInt());
				for (int i=0;i<pushButtonsVector.count();i++)
					pushButtonsVector[i]->updateValue(table,index,value.toInt());
			}
			if (index==18)
			{
				craneStep->setText(QString("Step:%1:%2").arg(value.toInt()).arg(crane->getCraneConveyor()->getCraneStepText(value.toInt())));
				craneStep->setToolTip(crane->getCraneConveyor()->getCraneStepDescription(value.toInt()));
			}
			if (index==19)
			{
				hooksStep->setText(QString("Step:%1:%2").arg(value.toInt()).arg(crane->getCraneConveyor()->getHooksStepText(value.toInt())));
				hooksStep->setToolTip(crane->getCraneConveyor()->getHooksStepDescription(value.toInt()));
			}
			for (int i=0;i<intPlcMemoryVector.count();i++)
				intPlcMemoryVector[i]->updateValue(ip,table,index,value.toInt());
			bitErrorsVector->updateValue(table,index,value.toInt());
			bitWarningsVector->updateValue(table,index,value.toInt());
			if ((index==27))
			{
				hooksItem->setPen(HAVECOMMONBIT(value.toInt(),BITS[25])?QPen(QColor(150,255,150)):QPen(QColor(255,100,100)));
			}
		}
		if("bRobotToPcF"==table)
		{
			if (index==4)
				hooksItem->setPos(value.toFloat()-1656.5,0.f);
			for (int i=0;i<progressVector.count();i++)
				progressVector[i]->updateValue(ip,table,index,value.toFloat());
			for (int i=0;i<floatPlcMemoryVector.count();i++)
				floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toFloat());
			for (int i=0;i<plotsVector.count();i++)
				plotsVector[i]->dataInPlcChanged(ip,table,index,value.toFloat());
			settings->dataInPlcChanged(ip,table,index,value);
		}
	}
}


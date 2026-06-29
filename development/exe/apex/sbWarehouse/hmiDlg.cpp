#include "pch.h"
#include "hmiDlg.h"
hmiDlgClass::hmiDlgClass(tuxipClass *tuxip_,bool useTranslation,craneClass *crane_):QDialog(crane_)
{
	splash->showMessage(tr("Creating hmi dialog..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	crane=crane_;
	setWindowTitle(tr("Cassette crane HMI"));
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
	bitErrorsVector=new bitLogVectorClass("bRobotToPcI","sbAlarms.txt",bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,mDb,"Errors","plcMessages","",useTranslation,bitLogHisoryClass::typeMySql,this);
	bitWarningsVector=new bitLogVectorClass("bRobotToPcI","sbWarnings.txt",bitClass::TYPE_SHOW_HIDE,styleLabelO,styleLabelGr,mDb,"Warnings","plcMessages","",useTranslation,bitLogHisoryClass::typeMySql,this);
	warningsErrorsLayout->addWidget(bitWarningsVector);
	warningsErrorsLayout->addWidget(bitErrorsVector);
	centralSplitter->addWidget(warningsErrorsWidget);

	//layout0
	//Horizontal movement
	splash->showMessage(tr("Horizontal movement widgets..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QVBoxLayout *horizontalPosGroupBoxMainLayout=new QVBoxLayout();layout0->addLayout(horizontalPosGroupBoxMainLayout);
	QGroupBox *horizontalPosGroupBox=new QGroupBox(tr("Horizontal movement"));horizontalPosGroupBoxMainLayout->addWidget(horizontalPosGroupBox);
	QGridLayout *horizontalPosGroupBoxLayout=new QGridLayout();horizontalPosGroupBox->setLayout(horizontalPosGroupBoxLayout);
	int row=0;
	progressVector<<new progressClass(tuxip,sbWarehouseIp,"bRobotToPcF",2,tr("Velocity"),tr("m/min"),-140.,140.,20.,200,200,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","","","",180,1,this);horizontalPosGroupBoxLayout->addWidget(progressVector.last(),row,0,1,2,Qt::AlignHCenter|Qt::AlignTop);
	plotsVector<<new plotClass(sbWarehouseIp,"bRobotToPcF",3,tr("Hz"),this);
	plotsVector.last()->addNewGraph(sbWarehouseIp,"bRobotToPcF",0,tr("Position[mm]"),1,0);
	plotsVector.last()->addNewGraph(sbWarehouseIp,"bRobotToPcF",2,tr("Velocity[m/min]"),2,0);
	progressVector.last()->setPlotClass(plotsVector.last());

	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",1,0,100000.,tr("Target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);horizontalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0);
	bitVector<<new bitClass("bRobotToPcI",23,22,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);horizontalPosGroupBoxLayout->addWidget(bitVector.last(),row,1);
	bitVector.last()->setMinimumWidth(150);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",0,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);horizontalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",3,0,100.,tr("Hz"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);horizontalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",4,0,100.,tr("Amps"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);horizontalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Fwd"),":/arrowUp.png",tuxip,"bRobotToPcI",22,1,0,"","bRobotToPcI",24,3,"","","","","",buttonStyle,buttonStyleActive,tr("Fwd"),this);horizontalPosGroupBoxLayout->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Bwd"),":/arrowDown.png",tuxip,"bRobotToPcI",22,2,0,"","bRobotToPcI",24,4,"","","","","",buttonStyle,buttonStyleActive,tr("Bwd"),this);horizontalPosGroupBoxLayout->addWidget(pushButtonsVector.last(),row,1);
	row++;
	laserIntensityProgressBar=new QProgressBar(this);
	horizontalPosGroupBoxLayout->addWidget(laserIntensityProgressBar,row,0,1,2);
	laserIntensityProgressBar->setFormat("Laser intensity %p%");
	laserIntensityProgressBar->setMinimum(0);
	laserIntensityProgressBar->setMaximum(100.);
	laserIntensityProgressBar->setAlignment(Qt::AlignCenter);
	//Vertical movement
	splash->showMessage(tr("Vertical movement widgets..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QVBoxLayout *verticalPosGroupBoxMainLayout=new QVBoxLayout();layout0->addLayout(verticalPosGroupBoxMainLayout);
	QGroupBox *verticalPosGroupBox=new QGroupBox(tr("Vertical movement"));verticalPosGroupBoxMainLayout->addWidget(verticalPosGroupBox);
	QGridLayout *verticalPosGroupBoxLayout=new QGridLayout();verticalPosGroupBox->setLayout(verticalPosGroupBoxLayout);
	row=0;
	progressVector<<new progressClass(tuxip,sbWarehouseIp,"bRobotToPcF",8,tr("Velocity"),tr("m/min"),-40.,40.,10.,200,200,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","","","",180,1,this);verticalPosGroupBoxLayout->addWidget(progressVector.last(),row,0,1,2,Qt::AlignHCenter|Qt::AlignTop);
	plotsVector<<new plotClass(sbWarehouseIp,"bRobotToPcF",9,tr("Hz"),this);
	plotsVector.last()->addNewGraph(sbWarehouseIp,"bRobotToPcF",8,tr("Velocity[m/min]"),1,0);
	progressVector.last()->setPlotClass(plotsVector.last());
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",7,0,100000.,tr("Target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);verticalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0);
	bitVector<<new bitClass("bRobotToPcI",23,23,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);verticalPosGroupBoxLayout->addWidget(bitVector.last(),row,1);
	bitVector.last()->setMinimumWidth(150);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",6,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);verticalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",9,0,100.,tr("Hz"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);verticalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",10,0,100.,tr("Amps"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);verticalPosGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Up"),":/arrowUp.png",tuxip,"bRobotToPcI",22,3,0,"","bRobotToPcI",24,5,"","","","","",buttonStyle,buttonStyleActive,tr("Up"),this);verticalPosGroupBoxLayout->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Down"),":/arrowDown.png",tuxip,"bRobotToPcI",22,4,0,"","bRobotToPcI",24,6,"","","","","",buttonStyle,buttonStyleActive,tr("Down"),this);verticalPosGroupBoxLayout->addWidget(pushButtonsVector.last(),row,1);
	row++;
	bitVector<<new bitClass("bRobotToPcI",23,0,bitClass::TYPE_CHANGE_COLOR,tr("Reset on"),tr("Reset off"),styleLabelG,styleLabelGr,"","","","",this);verticalPosGroupBoxLayout->addWidget(bitVector.last(),row,1);
	layout0->addStretch(1);
	//layout1
	QSplitter *layout1Splitter=new QSplitter(Qt::Vertical);layout1->addWidget(layout1Splitter);
	QWidget *layout1SplitterUpWidget=new QWidget();layout1Splitter->addWidget(layout1SplitterUpWidget);
	QWidget *layout1SplitterDownWidget=new QWidget();layout1Splitter->addWidget(layout1SplitterDownWidget);
	QVBoxLayout *layout1SplitterUpWidgetL=new QVBoxLayout();layout1SplitterUpWidget->setLayout(layout1SplitterUpWidgetL);
	QVBoxLayout *layout1SplitterDownWidgetL=new QVBoxLayout();layout1SplitterDownWidget->setLayout(layout1SplitterDownWidgetL);
	myQGraphicsViewClass *layoutGraphicsView=new myQGraphicsViewClass(false,this);
	layoutGraphicsView->scale(0.1,0.1);
	//layoutGraphicsView->resize(1800,800);
	layoutGraphicsView->setMinimumWidth(1000);
	//layoutGraphicsView->setMinimumHeight(750);
	splash->showMessage(tr("layoutScene widget..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QGraphicsScene *layoutScene=new QGraphicsScene(this);

	gItemClass *layoutGraphicsItem=new gItemClass(":dxfs/craneTopViewHmi.dxf",QPen(QColor(200,200,200)),this,Qt::NoBrush,Qt::NoBrush);
	layoutGraphicsItem->setZValue(-10.);layoutScene->addItem(layoutGraphicsItem);
	drawerItem=new			gItemClass(":dxfs/craneHooksHmi.dxf",	QPen(QColor(150,150,255)),this,Qt::NoBrush,Qt::NoBrush);layoutScene->addItem(drawerItem);

	gItemClass *proxOnItem=new gItemClass(":dxfs/proximitySwitchOn.dxf",QPen(QColor(75,150,75),25),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *proxOffRedItem=new gItemClass(":dxfs/proximitySwitchOn.dxf",QPen(QColor(150,75,75),25),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *proxOffItem=new gItemClass(":dxfs/proximitySwitchOff.dxf",QPen(QColor(150,150,150),10),this,Qt::NoBrush,Qt::NoBrush);
	//reset
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"I100 on","I100 off",sbWarehouseIp,"bRobotToPcI",23,0,-1700.f,1650.f,tr("Z travel reset proxmity switch"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"I101 on","I101 off",sbWarehouseIp,"bRobotToPcI",23,1,-2100.f,-500.f,tr("X travel lazer check proxmity switch"),this);

	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"I107 on","I107 off",sbWarehouseIp,"bRobotToPcI",23,18,-2190.f,+75.f,tr("Hooks up"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"I108 on","I108 off",sbWarehouseIp,"bRobotToPcI",23,19,-2190.f,-75.f,tr("Hooks down"),this);
	
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"Ixxx on","Ixxx off",sbWarehouseIp,"bRobotToPcI",23,14,-3700.f,-400.f,tr("Hooks left drop position"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"I102 on","I102 off",sbWarehouseIp,"bRobotToPcI",23,15,+3700.f,-400.f,tr("Hooks right reset position"),this);

	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffRedItem,"I109 on","I109 off",sbWarehouseIp,"bRobotToPcI",24,31,-4200.f,  0.f,tr("Safety light curtain, left"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffRedItem,"I109 on","I109 off",sbWarehouseIp,"bRobotToPcI",24,31,-4200.f,100.f,tr("Safety light curtain, left"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffRedItem,"I109 on","I109 off",sbWarehouseIp,"bRobotToPcI",24,31,-4200.f,200.f,tr("Safety light curtain, left"),this);
	
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffRedItem,"I110 on","I110 off",sbWarehouseIp,"bRobotToPcI",24,30,+4200.f,  0.f,tr("Safety light curtain, right"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffRedItem,"I110 on","I110 off",sbWarehouseIp,"bRobotToPcI",24,30,+4200.f,100.f,tr("Safety light curtain, right"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffRedItem,"I110 on","I110 off",sbWarehouseIp,"bRobotToPcI",24,30,+4200.f,200.f,tr("Safety light curtain, right"),this);

	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"I103 on","I103 off",sbWarehouseIp,"bRobotToPcI",23,6,-3100.f,-430.f,tr("Cassette in position 1"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"I104 on","I104 off",sbWarehouseIp,"bRobotToPcI",23,7,-1050.f,-430.f,tr("Cassette in position 2"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"I105 on","I105 off",sbWarehouseIp,"bRobotToPcI",23,8,+1050.f,-430.f,tr("Cassette in position 3"),this);
	proxVector<<new proxItemClass(layoutScene,proxOnItem,proxOffItem,"I106 on","I106 off",sbWarehouseIp,"bRobotToPcI",23,9,+3100.f,-430.f,tr("Cassette in position 4"),this);

	
	layoutGraphicsView->setScene(layoutScene);
	layout1SplitterUpWidgetL->addWidget(layoutGraphicsView);
	//auto manual stop alarm
	QHBoxLayout *autoLayout=new QHBoxLayout();layout1SplitterUpWidgetL->addLayout(autoLayout);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",tuxip,"bRobotToPcI",22,21,0,"","bRobotToPcI",24,0,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);autoLayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",tuxip,"bRobotToPcI",22,22,0,"","bRobotToPcI",24,1,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);autoLayout->addWidget(pushButtonsVector.last());
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Maintenance"),"",tuxip,"bRobotToPcI",22,29,0,"","bRobotToPcI",21,14,"","","","","",buttonStyle,buttonStyleActive,tr("Maintenance"),this);autoLayout->addWidget(pushButtonsVector.last());
	bitVector<<new bitClass("bRobotToPcI",21,14,bitClass::TYPE_CHANGE_COLOR,tr("Maintenance"),tr("Maintenance"),styleLabelG,styleLabelGr,"","","","",this);autoLayout->addWidget(bitVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Active Stop"),"",tuxip,"bRobotToPcI",22,226,0,"","bRobotToPcI",23,24,"","","","","",buttonStyleActive,buttonStyleRed,tr("No Active Stop"),this);autoLayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Alarms"),"",tuxip,"bRobotToPcI",22,23,0,"","bRobotToPcI",24,2,"","","","","",buttonStyleActive,buttonStyleRed,tr("No Alarms"),this);autoLayout->addWidget(pushButtonsVector.last());
	//camera-missions
	missionsTabWidget=new QTabWidget();layout1SplitterDownWidgetL->addWidget(missionsTabWidget);
	QWidget *camerasWidget=new QWidget();
	QWidget *missionsWidget=new QWidget();missionsTabWidget->addTab(missionsWidget,tr("Missions"));
	connect(missionsTabWidget,SIGNAL(tabBarDoubleClicked(int)),this,SLOT(cameraMissionsTabWidgetTabBarDoubleClickedSlot(int)));
	//missions
	splash->showMessage(tr("Missions widgets..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200)); 
	QHBoxLayout *missionsMainLayout=new QHBoxLayout();missionsWidget->setLayout(missionsMainLayout);
	//missions A
	QGroupBox *missionAGroupBox=new QGroupBox(tr("Mission"));missionsMainLayout->addWidget(missionAGroupBox);
	QVBoxLayout *missionAMainGroupBoxLayout=new QVBoxLayout();missionAGroupBox->setLayout(missionAMainGroupBoxLayout);

	QGroupBox *missionANextGroupBox=new QGroupBox(tr("Next Mission"));missionAMainGroupBoxLayout->addWidget(missionANextGroupBox);
	QVBoxLayout *missionANextGroupBoxLayout=new QVBoxLayout();missionANextGroupBox->setLayout(missionANextGroupBoxLayout);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcI",0,1,3000,tr("Next from:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionANextGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcI",1,1,3000,tr("Next to:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionANextGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcI",2,1,5,tr("Status:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionANextGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	QPushButton *clearNextMission=new QPushButton(tr("Clear Next Mission"));missionANextGroupBoxLayout->addWidget(clearNextMission);
	connect(clearNextMission,SIGNAL(clicked()),this,SLOT(clearNextMissionBtnClickedSlot()));

	QGroupBox *missionAActiveGroupBox=new QGroupBox(tr("Active Mission"));missionAMainGroupBoxLayout->addWidget(missionAActiveGroupBox);
	QVBoxLayout *missionAActiveGroupBoxLayout=new QVBoxLayout();missionAActiveGroupBox->setLayout(missionAActiveGroupBoxLayout);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcI",10,1,3000,tr("Active From:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionAActiveGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcI",11,1,3000,tr("Active To:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionAActiveGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcI",12,1,5,tr("PC Status:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionAActiveGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcI",13,1,5,tr("PLC Status:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionAActiveGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	missionStep=new QLabel();missionAActiveGroupBoxLayout->addWidget(missionStep);
	QHBoxLayout *endAbortALayout=new QHBoxLayout();missionAActiveGroupBoxLayout->addLayout(endAbortALayout);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Abort Active Mission"),"",tuxip,"bRobotToPcI",22,26,0,"","bRobotToPcI",21,15,"","","","","",buttonStyle,buttonStyleActive,tr("Abort Active Mission"),this);endAbortALayout->addWidget(pushButtonsVector.last());
	abortActiveMission=pushButtonsVector.last();
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Continue Active Mission"),"",tuxip,"bRobotToPcI",22,48,0,"","bRobotToPcI",21,16,"","","","","",buttonStyle,buttonStyleActive,tr("Continue Active Mission"),this);endAbortALayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("End Active Mission"),"",tuxip,"bRobotToPcI",22,24,0,"","bRobotToPcI",21,17,"","","","","",buttonStyle,buttonStyleActive,tr("End Active Mission"),this);endAbortALayout->addWidget(pushButtonsVector.last());
	endActiveMission=pushButtonsVector.last();
	clearActiveMission=new QPushButton(tr("Clear Active Mission"));missionAActiveGroupBoxLayout->addWidget(clearActiveMission);
	connect(clearActiveMission,SIGNAL(clicked()),this,SLOT(clearActiveMissionBtnClickedSlot()));
	layout1Splitter->setSizes(QList<int>()<<500<<200);
	//layout1->addStretch(1);
	//hooks
	QGroupBox *hooksGroupBox=new QGroupBox(tr("Hooks"));layout2->addWidget(hooksGroupBox);
	QGridLayout *hooksGroupBoxL=new QGridLayout();hooksGroupBox->setLayout(hooksGroupBoxL);
	row=0;
	progressVector<<new progressClass(tuxip,sbWarehouseIp,"bRobotToPcF",14,tr("Velocity"),tr("m/min"),-80.,80.,20.,200,200,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","","","",180,1,this);hooksGroupBoxL->addWidget(progressVector.last(),row,0,1,2,Qt::AlignHCenter|Qt::AlignTop);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",13,0,100000.,tr("Target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);hooksGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,0);
	bitVector<<new bitClass("bRobotToPcI",23,25,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);hooksGroupBoxL->addWidget(bitVector.last(),row,1);
	bitVector.last()->setMinimumWidth(150);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",12,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);hooksGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",15,0,100.,tr("Hz"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);hooksGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",16,0,100.,tr("Amps"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);hooksGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Left"),":/arrowLeft.png",tuxip,"bRobotToPcI",22,7,0,"","bRobotToPcI",24,9,"","","","","",buttonStyle,buttonStyleActive,tr("Left"),this);hooksGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Right"),":/arrowRight.png",tuxip,"bRobotToPcI",22,8,0,"","bRobotToPcI",24,10,"","","","","",buttonStyle,buttonStyleActive,tr("Right"),this);hooksGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Up"),":/arrowUp.png",tuxip,"bRobotToPcI",22,5,0,"","bRobotToPcI",24,17,"","","","","",buttonStyle,buttonStyleActive,tr("Up"),this);hooksGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Down"),":/arrowDown.png",tuxip,"bRobotToPcI",22,6,0,"","bRobotToPcI",24,18,"","","","","",buttonStyle,buttonStyleActive,tr("Down"),this);hooksGroupBoxL->addWidget(pushButtonsVector.last(),row,1);

	plotsVector<<new plotClass(sbWarehouseIp,"bRobotToPcF",12,tr("Hz"),this);
	plotsVector.last()->addNewGraph(sbWarehouseIp,"bRobotToPcF",17,tr("Velocity[m/min]"),1,0);
	progressVector.last()->setPlotClass(plotsVector.last());
	//conveyor
	QGroupBox *convGroupBox=new QGroupBox(tr("Conveyor"));layout2->addWidget(convGroupBox);
	QGridLayout *convGroupBoxL=new QGridLayout();convGroupBox->setLayout(convGroupBoxL);
	row=0;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Left"),":/arrowLeft.png",tuxip,"bRobotToPcI",22,9,0,"","bRobotToPcI",24,13,"","","","","",buttonStyle,buttonStyleActive,tr("Left"),this);convGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Right"),":/arrowRight.png",tuxip,"bRobotToPcI",22,10,0,"","bRobotToPcI",24,14,"","","","","",buttonStyle,buttonStyleActive,tr("Right"),this);convGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	//Safety light curtain 
	QGroupBox *lightGroupBox=new QGroupBox(tr("Safety light curtain"));layout2->addWidget(lightGroupBox);
	QGridLayout *lightGroupBoxL=new QGridLayout();lightGroupBox->setLayout(lightGroupBoxL);

	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Left in"),":/arrowRight.png",tuxip,		"bRobotToPcI",22,18,0,"","bRobotToPcI",23, 2,"","","","","",buttonStyle,buttonStyleActive,tr("Left in"),this);lightGroupBoxL->addWidget(pushButtonsVector.last(),0,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Left out"),":/arrowLeft.png",tuxip,		"bRobotToPcI",22,17,0,"","bRobotToPcI",23, 3,"","","","","",buttonStyle,buttonStyleActive,tr("Left out"),this);lightGroupBoxL->addWidget(pushButtonsVector.last(),1,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Right in"),":/arrowLeft.png",tuxip,		"bRobotToPcI",22,19,0,"","bRobotToPcI",23, 4,"","","","","",buttonStyle,buttonStyleActive,tr("Right in"),this);lightGroupBoxL->addWidget(pushButtonsVector.last(),0,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Right out"),":/arrowRight.png",tuxip,	"bRobotToPcI",22,20,0,"","bRobotToPcI",23, 5,"","","","","",buttonStyle,buttonStyleActive,tr("Right out"),this);lightGroupBoxL->addWidget(pushButtonsVector.last(),1,1);

	QGroupBox *presureGroupBox=new QGroupBox(tr("Presure"));layout2->addWidget(presureGroupBox);
	QVBoxLayout *presureGroupBoxL=new QVBoxLayout();presureGroupBox->setLayout(presureGroupBoxL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Add oil"),":/arrowUp.png",tuxip,"bRobotToPcI",22,32,0,"","bRobotToPcI",23,13,"","","","","",buttonStyle,buttonStyleActive,tr("Add oil"),this);presureGroupBoxL->addWidget(pushButtonsVector.last());
	bitVector<<new bitClass("bRobotToPcI",23,12,bitClass::TYPE_CHANGE_COLOR,tr("Hydraulic pressure 65 bar"),tr("Hydraulic pressure 65 bar"),styleLabelG,styleLabelR,"","","","",this);presureGroupBoxL->addWidget(bitVector.last());
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Remove oil"),":/arrowDown.png",tuxip,"bRobotToPcI",22,33,0,"","bRobotToPcI",23,17,"","","","","",buttonStyle,buttonStyleActive,tr("Remove oil"),this);presureGroupBoxL->addWidget(pushButtonsVector.last());

	bitVector<<new bitClass("bRobotToPcI",23,16,bitClass::TYPE_CHANGE_COLOR,tr("Air pressure"),tr("Air pressure"),styleLabelG,styleLabelR,"","","","",this);presureGroupBoxL->addWidget(bitVector.last());


	QPushButton *settingsBtn=new QPushButton(QIcon(":/autoEnabled.png"),tr("Settings"),this);
	layout2->addWidget(settingsBtn);
	connect(settingsBtn,SIGNAL(clicked(bool)),this,SLOT(settingsBtnClickedSlot()));

	layout2->addStretch(1);
	//
	centralSplitter->setSizes(QList<int>()<<height()-100<<100);
	settings=new hmiSettingsClass(tuxip,this);
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
	if (!l)
	{
		if (!getPassword(tr("Password is needed activate advanced buttons."),tr("Please give the password"),passWeak,this))
			return;
	}
	areAdvancedButtonsLocked=l;
	if (areAdvancedButtonsLocked)
	{
		clearActiveMission->setEnabled(false);
		abortActiveMission->setEnabled(false);
		endActiveMission->setEnabled(false);
		missionsTabWidget->tabBar()->setTabIcon(0,QIcon(":/lock.png"));
	}
	else
	{
		clearActiveMission->setEnabled(true);
		abortActiveMission->setEnabled(true);
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
	lockAdvancedBtns(true);
	QDialog::show();
	return 1;
}
void hmiDlgClass::clearNextMissionBtnClickedSlot()
{
	crane->getCraneConveyor()->setNextFromPos(0);
	crane->getCraneConveyor()->setNextToPos(0);
	tuxip->writeInteger("bRobotToPcI",0,0);
	tuxip->writeInteger("bRobotToPcI",1,0);
	tuxip->writeInteger("bRobotToPcI",2,0);
}
void hmiDlgClass::clearActiveMissionBtnClickedSlot()
{
	if (getPassword(tr("Password is needed to end an active mission.")),tr("Please give the password"),passStrong,this)
		craneConveyor->clearMissionInDb("user");
}
void hmiDlgClass::settingsBtnClickedSlot()
{
	settings->exec();
}
void hmiDlgClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (sbWarehouseIp==ip)
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
				missionStep->setText(QString("Step:%1:%2").arg(value.toInt()).arg(crane->getCraneConveyor()->getStepText(value.toInt())));
				missionStep->setToolTip(crane->getCraneConveyor()->getStepDescription(value.toInt()));
			}
			for (int i=0;i<intPlcMemoryVector.count();i++)
				intPlcMemoryVector[i]->updateValue(ip,table,index,value.toInt());
			bitErrorsVector->updateValue(table,index,value.toInt());
			bitWarningsVector->updateValue(table,index,value.toInt());
			if (index==27)
			{
				drawerItem->setPen(HAVECOMMONBIT(value.toInt(),BITS[25])?QPen(QColor(150,255,150)):QPen(QColor(255,100,100)));
			}
		}
		if("bRobotToPcF"==table)
		{
			if (index==12)
				drawerItem->setPos(-value.toFloat()+3588.,0.f);
			for (int i=0;i<progressVector.count();i++)
				progressVector[i]->updateValue(ip,table,index,value.toFloat());
			for (int i=0;i<floatPlcMemoryVector.count();i++)
				floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toFloat());
			for (int i=0;i<plotsVector.count();i++)
				plotsVector[i]->dataInPlcChanged(ip,table,index,value.toFloat());
			if (index==18)
				laserIntensityProgressBar->setValue(value.toFloat());
		}
		settings->dataInPlcChanged(ip,table,index,value);
	}
}
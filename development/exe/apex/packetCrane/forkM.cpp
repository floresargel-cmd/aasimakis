#include "pch.h"
#include "forkM.h"
forkLiftManualClass::forkLiftManualClass(tcpServerClass *tcpServer_,QWidget *p):QWidget(p)
{
	//this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px}  QCheckBox{font-size:%1px}").arg(12));
	tcpServer=tcpServer_;
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	QSplitter *mainSplitter=new QSplitter(Qt::Vertical,this);mainLayout->addWidget(mainSplitter);
	/////////////////////////////////////////////////////////////////////////area
	QImage image(":/forkLiftLayout.png");
	QLabel *imageLabel = new QLabel;
	imageLabel->setMinimumWidth(1600);
	imageLabel->setMinimumHeight(377);
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setAlignment(Qt::AlignCenter);

	QWidget *areaWidget=new QWidget();
	QVBoxLayout *areaLayout=new QVBoxLayout();
	areaLayout->addWidget(imageLabel);
	areaWidget->setLayout(areaLayout);
	QScrollArea *area=new QScrollArea();
	area->setAlignment(Qt::AlignCenter);
	area->setBackgroundRole(QPalette::Shadow);
	area->setWidget(areaWidget);
	//
	int X=20;int Y=470;int W=100;int H=30;int m=10;
	autoPushbutton=new myQPushButton(tcpServer,"N160|23|419","N160|23|0",tr("Auto"),imageLabel);
	autoPushbutton->setGeometry(X,Y,W,H);
	X+=W+m;
	manualPushbutton=new myQPushButton(tcpServer,"N160|23|420","N160|23|0",tr("Manual"),imageLabel);
	manualPushbutton->setGeometry(X,Y,W,H);
	X+=W+m;
	maitenancePushbutton=new myQPushButton(tcpServer,"N160|23|421","N160|23|0",tr("Maitenance"),imageLabel);
	maitenancePushbutton->setGeometry(X,Y,W,H);
	X+=W+m;
	myQPushButton *resetAlarmsPushButton=new myQPushButton(tcpServer,"N160|23|441","N160|23|0",tr("Reset Alarms"),imageLabel);
	resetAlarmsPushButton->setGeometry(X,Y,W,H);
	X+=W+m;
	myQPushButton *runInMaintenaceModeAlarmsPushButton=new myQPushButton(tcpServer,"N160|23|436","N160|23|0",tr(""),imageLabel);
	runInMaintenaceModeAlarmsPushButton->setIcon(QIcon(":/running.png"));
	runInMaintenaceModeAlarmsPushButton->setIconSize(QSize(30,30));
	runInMaintenaceModeAlarmsPushButton->setGeometry(X,Y,35,35);
	X+=W+m;
	stopLabel_5_9=new QLabel("",imageLabel);
	stopLabel_5_9->setGeometry(X,Y,W,H);
	W=75;
	areaButtonsVector<<new myQPushButton(tcpServer,"N160|23|409","N160|23|0",tr("  Reset"),imageLabel);areaButtonsVector.last()->setIcon(QIcon(":/arrowMoveLeft.png"));areaButtonsVector.last()->setGeometry(500,265,W,H);
	areaButtonsVector<<new myQPushButton(tcpServer,"N160|23|412","N160|23|0",tr("  Reset"),imageLabel);areaButtonsVector.last()->setIcon(QIcon(":/arrowMoveUp.png"));areaButtonsVector.last()->setGeometry(720,210,W,H);
	areaButtonsVector<<new myQPushButton(tcpServer,"N160|23|410","N160|23|0",tr("  Reset"),imageLabel);areaButtonsVector.last()->setIcon(QIcon(":/arrowMoveUp.png"));areaButtonsVector.last()->setGeometry(170,150,W,H);

	areaButtonsVector<<new myQPushButton(tcpServer,"N160|23|413","N160|23|0",tr(""),imageLabel);areaButtonsVector.last()->setIcon(QIcon(":/arrowMoveUp.png"));areaButtonsVector.last()->setGeometry(210,250,30,30);
	areaButtonsVector<<new myQPushButton(tcpServer,"N160|23|414","N160|23|0",tr(""),imageLabel);areaButtonsVector.last()->setIcon(QIcon(":/arrowMoveDown.png"));areaButtonsVector.last()->setGeometry(210,290,30,30);

	areaButtonsVector<<new myQPushButton(tcpServer,"N160|23|416","N160|23|0",tr(""),imageLabel);areaButtonsVector.last()->setIcon(QIcon(":/arrowMoveUp.png"));areaButtonsVector.last()->setGeometry(775,250,30,30);
	areaButtonsVector<<new myQPushButton(tcpServer,"N160|23|415","N160|23|0",tr(""),imageLabel);areaButtonsVector.last()->setIcon(QIcon(":/arrowMoveDown.png"));areaButtonsVector.last()->setGeometry(775,280,30,30);
	areaButtonsVector<<new myQPushButton(tcpServer,"N160|23|418","N160|23|0",tr(""),imageLabel);areaButtonsVector.last()->setIcon(QIcon(":/arrowMoveLeft.png"));areaButtonsVector.last()->setGeometry(745,265,30,30);
	areaButtonsVector<<new myQPushButton(tcpServer,"N160|23|417","N160|23|0",tr(""),imageLabel);areaButtonsVector.last()->setIcon(QIcon(":/arrowMoveRight.png"));areaButtonsVector.last()->setGeometry(805,265,30,30);
	//int X,Y;
	myQPushButton *conveyor1PushButton=new myQPushButton(tcpServer,"N160|23|425","N160|23|0",tr(""),imageLabel);conveyor1PushButton->setIcon(QIcon(":/arrowMoveDown.png"));conveyor1PushButton->setGeometry( 945,300,30,30);
	myQPushButton *conveyor2PushButton=new myQPushButton(tcpServer,"N160|23|426","N160|23|0",tr(""),imageLabel);conveyor2PushButton->setIcon(QIcon(":/arrowMoveDown.png"));conveyor2PushButton->setGeometry(1105,300,30,30);
	areaButtonsVector<<new myQPushButton(tcpServer,"N160|23|427","N160|23|0",tr(""),imageLabel);areaButtonsVector.last()->setIcon(QIcon(":/arrowMoveDown.png"));areaButtonsVector.last()->setGeometry(1390,350,30,30);

	//X=1085;Y=45;
	//areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_UP   ,"N100|30|603","N100|30|0",tr("Roller way 5 up"          ), X+54,Y+10,imageLabel);
	//areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_DOWN ,"N100|30|604","N100|30|0",tr("Roller way 5 down"           ), X   ,Y+10,imageLabel);

	//
	mainSplitter->addWidget(area);
	/////////////////////////////////////////////////////////////////////////buttons
	QWidget *btnsWidget=new QWidget();
	QHBoxLayout *btnsLayout=new QHBoxLayout();btnsWidget->setLayout(btnsLayout);
	QVBoxLayout *btnsMainLayout0=new QVBoxLayout();btnsLayout->addLayout(btnsMainLayout0);
	QVBoxLayout *btnsMainLayout1=new QVBoxLayout();btnsLayout->addLayout(btnsMainLayout1);
	QVBoxLayout *btnsMainLayout2=new QVBoxLayout();btnsLayout->addLayout(btnsMainLayout2);
	QVBoxLayout *btnsMainLayout3=new QVBoxLayout();btnsLayout->addLayout(btnsMainLayout3);
	QVBoxLayout *btnsMainLayout4=new QVBoxLayout();btnsLayout->addLayout(btnsMainLayout4);
	QVBoxLayout *btnsMainLayout5=new QVBoxLayout();btnsLayout->addLayout(btnsMainLayout5);
	QVBoxLayout *btnsMainLayout6=new QVBoxLayout();btnsLayout->addLayout(btnsMainLayout6);
	//btnsMainLayout0
	QGroupBox *pcCommsGroupBox=new QGroupBox(tr("Pc comms"));btnsMainLayout0->addWidget(pcCommsGroupBox);
	QGridLayout *pcCommsGroupBoxLayout=new QGridLayout();pcCommsGroupBox->setLayout(pcCommsGroupBoxLayout);

	QLabel *memory160_0_labelLebel=new QLabel(tr("Load position: "));						pcCommsGroupBoxLayout->addWidget(memory160_0_labelLebel,0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	memory160_0_label=new QLabel(tr("..." ));																		pcCommsGroupBoxLayout->addWidget(memory160_0_label     ,0,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);memory160_0_label->setMinimumWidth(75);

	QLabel *memory160_1_labelLebel=new QLabel(tr("UnLoad position: "));					pcCommsGroupBoxLayout->addWidget(memory160_1_labelLebel,1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	memory160_1_label=new QLabel(tr("..." ));																		pcCommsGroupBoxLayout->addWidget(memory160_1_label     ,1,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);memory160_1_label->setMinimumWidth(75);

	QLabel *memory160_2_labelLebel=new QLabel(tr("PLC status: "));							pcCommsGroupBoxLayout->addWidget(memory160_2_labelLebel,2,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	memory160_2_label=new QLabel(tr("..." ));																		pcCommsGroupBoxLayout->addWidget(memory160_2_label     ,2,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);memory160_2_label->setMinimumWidth(75);

	QLabel *memory160_3_labelLebel=new QLabel(tr("PC status: "));								pcCommsGroupBoxLayout->addWidget(memory160_3_labelLebel,3,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	memory160_3_label=new QLabel(tr("..." ));																		pcCommsGroupBoxLayout->addWidget(memory160_3_label     ,3,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);memory160_3_label->setMinimumWidth(75);

	QLabel *memory160_4_labelLebel=new QLabel(tr("Packet Type: "));							pcCommsGroupBoxLayout->addWidget(memory160_4_labelLebel,4,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	memory160_4_label=new QLabel(tr("..." ));																		pcCommsGroupBoxLayout->addWidget(memory160_4_label     ,4,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);memory160_4_label->setMinimumWidth(75);

	QGroupBox *missionsGroupBox=new QGroupBox(tr("Missions"));btnsMainLayout0->addWidget(missionsGroupBox);
	QVBoxLayout *missionsGroupBoxLayout=new QVBoxLayout();missionsGroupBox->setLayout(missionsGroupBoxLayout);

	myQPushButton *abordMissionBtn=new myQPushButton(tcpServer,"","N160|23|0",tr("Abort	 mission"),NULL);
	missionsGroupBoxLayout->addWidget(abordMissionBtn);
	connect(abordMissionBtn,SIGNAL(clicked()),this,SLOT(abordMissionBtnSlot()));
	myQPushButton *closeMissionBtn=new myQPushButton(tcpServer,"","N160|23|0",tr("Close mission"),NULL);
	missionsGroupBoxLayout->addWidget(closeMissionBtn);
	connect(closeMissionBtn,SIGNAL(clicked()),this,SLOT(closeMissionBtnSlot()));
	//
	continueMissionBtn=new QPushButton(tr("Continue"));
	missionsGroupBoxLayout->addWidget(continueMissionBtn);
	connect(continueMissionBtn,SIGNAL(clicked()),this,SLOT(continueMissionBtnSlot()));

	missionsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N160|23|423","N160|23|0",tr("S.B. to conveyor 1"),NULL));
	missionsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N160|23|424","N160|23|0",tr("Packet to conveyor 2"),NULL));
	takePacketQPushButton=new myQPushButton(tcpServer,"N160|23|429","N160|23|0",tr("Take packet"),NULL);
	missionsGroupBoxLayout->addWidget(takePacketQPushButton);
	missionsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N160|23|440","N160|23|0",tr("1-3 status"),NULL));
	missionFaultsBtn=new bitsVectorBtnClass(tr("Mission messages"),forkIp,"N160",7,15,"forkMissionFaults.txt","N160",bitClass::TYPE_SHOW_HIDE,BUTTON_STYLE_RED,BUTTON_STYLE_GREEN,STYLE_R,STYLE_G,"","","",false,bitLogHisoryClass::typeMsSql,this);
	missionsGroupBoxLayout->addWidget(missionFaultsBtn);

	btnsMainLayout0->addStretch(1);
	//btnsMainLayout1
	QGroupBox *ppositionsGroupBox=new QGroupBox(tr("Positions"));btnsMainLayout1->addWidget(ppositionsGroupBox);
	QVBoxLayout *ppositionsGroupBoxLayout=new QVBoxLayout();ppositionsGroupBox->setLayout(ppositionsGroupBoxLayout);
	signal_05_16=new QLabel(tr("Shiping basket"		));signal_05_16->setMargin(5);		ppositionsGroupBoxLayout->addWidget(signal_05_16,Qt::AlignHCenter|Qt::AlignCenter);signal_05_16->setMargin(7);
	signal_05_17=new QLabel(tr("Package"		      ));signal_05_17->setMargin(5);		ppositionsGroupBoxLayout->addWidget(signal_05_17,Qt::AlignHCenter|Qt::AlignCenter);signal_05_17->setMargin(7);
	signal_05_11=new QLabel(tr("Conveyor 1"				));signal_05_11->setMargin(5);		ppositionsGroupBoxLayout->addWidget(signal_05_11,Qt::AlignHCenter|Qt::AlignCenter);signal_05_11->setMargin(7);
	signal_05_12=new QLabel(tr("Conveyor 2"				));signal_05_12->setMargin(5);		ppositionsGroupBoxLayout->addWidget(signal_05_12,Qt::AlignHCenter|Qt::AlignCenter);signal_05_12->setMargin(7);
	signal_05_13=new QLabel(tr("Conveyor 3"				));signal_05_13->setMargin(5);		ppositionsGroupBoxLayout->addWidget(signal_05_13,Qt::AlignHCenter|Qt::AlignCenter);signal_05_13->setMargin(7);
	signal_05_14=new QLabel(tr("Warehouse 1"			));signal_05_14->setMargin(5);		ppositionsGroupBoxLayout->addWidget(signal_05_14,Qt::AlignHCenter|Qt::AlignCenter);signal_05_14->setMargin(7);
	signal_05_15=new QLabel(tr("Warehouse 2"			));signal_05_15->setMargin(5);		ppositionsGroupBoxLayout->addWidget(signal_05_15,Qt::AlignHCenter|Qt::AlignCenter);signal_05_15->setMargin(7);
	signal_05_18=new QLabel(tr("Pos 23 full"			));signal_05_18->setMargin(5);		ppositionsGroupBoxLayout->addWidget(signal_05_18,Qt::AlignHCenter|Qt::AlignCenter);signal_05_18->setMargin(7);
	//signal_05_19=new QLabel(tr("Pos 22 full"			));signal_05_19->setMargin(5);		ppositionsGroupBoxLayout->addWidget(signal_05_19,Qt::AlignHCenter|Qt::AlignCenter);signal_05_19->setMargin(7);
//	signal_05_22=new QLabel(tr("Pos 21 full"			));signal_05_22->setMargin(5);		ppositionsGroupBoxLayout->addWidget(signal_05_22,Qt::AlignHCenter|Qt::AlignCenter);signal_05_22->setMargin(7);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Pos 22 full"),":/arrowUp.png",tcpServer->getTuxipClass(),"N160",23,426,0,"","N160",5,19,"","","","","",BUTTON_STYLE_GREEN,BUTTON_STYLE_RED,tr("Pos 22 not full"),this);
	ppositionsGroupBoxLayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Pos 21 full"),":/arrowUp.png",tcpServer->getTuxipClass(),"N160",23,425,0,"","N160",5,22,"","","","","",BUTTON_STYLE_GREEN,BUTTON_STYLE_RED,tr("Pos 21 not full"),this);
	ppositionsGroupBoxLayout->addWidget(pushButtonsVector.last());
	btnsMainLayout1->addStretch(1);	btnsMainLayout1->addStretch(1);
	//btnsMainLayout2

	//btnsMainLayout3
	QGroupBox *proximitySwitchesGroupBox=new QGroupBox(tr("Proximity switches"));btnsMainLayout3->addWidget(proximitySwitchesGroupBox);
	QVBoxLayout *proximitySwitchesGroupBoxLayout=new QVBoxLayout();proximitySwitchesGroupBox->setLayout(proximitySwitchesGroupBoxLayout);
	signal_05_03=new QLabel(tr("Reset up (1503)"		));signal_05_03->setMargin(5);		proximitySwitchesGroupBoxLayout->addWidget(signal_05_03,Qt::AlignHCenter|Qt::AlignCenter);signal_05_03->setMinimumWidth(75);
	signal_05_04=new QLabel(tr("Reset east (1504)"	));signal_05_04->setMargin(5);		proximitySwitchesGroupBoxLayout->addWidget(signal_05_04,Qt::AlignHCenter|Qt::AlignCenter);signal_05_04->setMinimumWidth(75);
	signal_05_05=new QLabel(tr("Reset north (1505)" ));signal_05_05->setMargin(5);		proximitySwitchesGroupBoxLayout->addWidget(signal_05_05,Qt::AlignHCenter|Qt::AlignCenter);signal_05_05->setMinimumWidth(75);

	QGroupBox *magneticSwitchesGroupBox=new QGroupBox(tr("Magnetic switches"));btnsMainLayout3->addWidget(magneticSwitchesGroupBox);
	QVBoxLayout *magneticSwitchesGroupBoxLayout=new QVBoxLayout();magneticSwitchesGroupBox->setLayout(magneticSwitchesGroupBoxLayout);
	signal_05_06=new QLabel(tr("Pin are in (1506)"				));signal_05_06->setMargin(5);		magneticSwitchesGroupBoxLayout->addWidget(signal_05_06,Qt::AlignHCenter|Qt::AlignCenter); signal_05_06->setMinimumWidth(75);
	signal_05_07=new QLabel(tr("Pin are out (1507)"			));signal_05_07->setMargin(5);		magneticSwitchesGroupBoxLayout->addWidget(signal_05_07,Qt::AlignHCenter|Qt::AlignCenter);   signal_05_07->setMinimumWidth(75);
	signal_05_08=new QLabel(tr("Aligner is open (1508)" ));signal_05_08->setMargin(5);		magneticSwitchesGroupBoxLayout->addWidget(signal_05_08,Qt::AlignHCenter|Qt::AlignCenter);   signal_05_08->setMinimumWidth(75);
	btnsMainLayout3->addStretch(1);
	//btnsMainLayout4
	int minWidth=175;
	QGroupBox *positionsGroupBox=new QGroupBox(tr("Curent positions"));btnsMainLayout4->addWidget(positionsGroupBox);
	QVBoxLayout *positionsGroupBoxLayout=new QVBoxLayout();positionsGroupBox->setLayout(positionsGroupBoxLayout);
	posXLabel=new QLabel(tr(""));posXLabel->setMargin(5);posXLabel->setMinimumWidth(minWidth);		positionsGroupBoxLayout->addWidget(posXLabel,Qt::AlignHCenter|Qt::AlignCenter);posXLabel->setMinimumWidth(175);
	posZLabel=new QLabel(tr(""));posZLabel->setMargin(5);posZLabel->setMinimumWidth(minWidth);		positionsGroupBoxLayout->addWidget(posZLabel,Qt::AlignHCenter|Qt::AlignCenter);posZLabel->setMinimumWidth(175);
	posYLabel=new QLabel(tr(""));posYLabel->setMargin(5);posYLabel->setMinimumWidth(minWidth);		positionsGroupBoxLayout->addWidget(posYLabel,Qt::AlignHCenter|Qt::AlignCenter);posYLabel->setMinimumWidth(175);
	QGroupBox *targetGroupBox=new QGroupBox(tr("Target positions"));btnsMainLayout4->addWidget(targetGroupBox);
	QVBoxLayout *targetGroupBoxLayout=new QVBoxLayout();targetGroupBox->setLayout(targetGroupBoxLayout);
	targetPosXLabel=new QLabel(tr(""));targetPosXLabel->setMargin(5);targetPosXLabel->setMinimumWidth(minWidth);		targetGroupBoxLayout->addWidget(targetPosXLabel,Qt::AlignHCenter|Qt::AlignCenter);targetPosXLabel->setMinimumWidth(175);
	targetPosZLabel=new QLabel(tr(""));targetPosZLabel->setMargin(5);targetPosZLabel->setMinimumWidth(minWidth);		targetGroupBoxLayout->addWidget(targetPosZLabel,Qt::AlignHCenter|Qt::AlignCenter);targetPosZLabel->setMinimumWidth(175);
	targetPosYLabel=new QLabel(tr(""));targetPosYLabel->setMargin(5);targetPosYLabel->setMinimumWidth(minWidth);		targetGroupBoxLayout->addWidget(targetPosYLabel,Qt::AlignHCenter|Qt::AlignCenter);targetPosYLabel->setMinimumWidth(175);
	btnsMainLayout4->addStretch(1);
	//btnsMainLayout5
	QGroupBox *velocityGroupBox=new QGroupBox(tr("Curent velocity"));btnsMainLayout5->addWidget(velocityGroupBox);
	QVBoxLayout *velocityGroupBoxLayout=new QVBoxLayout();velocityGroupBox->setLayout(velocityGroupBoxLayout);
	velocityXLabel=new QLabel(tr(""));velocityXLabel->setMargin(5);velocityXLabel->setMinimumWidth(minWidth);		velocityGroupBoxLayout->addWidget(velocityXLabel,Qt::AlignHCenter|Qt::AlignCenter);velocityXLabel->setMinimumWidth(175);
	velocityZLabel=new QLabel(tr(""));velocityZLabel->setMargin(5);velocityZLabel->setMinimumWidth(minWidth);		velocityGroupBoxLayout->addWidget(velocityZLabel,Qt::AlignHCenter|Qt::AlignCenter);velocityZLabel->setMinimumWidth(175);
	velocityYLabel=new QLabel(tr(""));velocityYLabel->setMargin(5);velocityYLabel->setMinimumWidth(minWidth);		velocityGroupBoxLayout->addWidget(velocityYLabel,Qt::AlignHCenter|Qt::AlignCenter);velocityYLabel->setMinimumWidth(175);
	QGroupBox *targetVelocityGroupBox=new QGroupBox(tr("Reference Hz"));btnsMainLayout5->addWidget(targetVelocityGroupBox);
	QVBoxLayout *targetVelocityGroupBoxLayout=new QVBoxLayout();targetVelocityGroupBox->setLayout(targetVelocityGroupBoxLayout);
	targetVelocityXLabel=new QLabel(tr(""));targetVelocityXLabel->setMargin(5);targetVelocityXLabel->setMinimumWidth(minWidth);		targetVelocityGroupBoxLayout->addWidget(targetVelocityXLabel,Qt::AlignHCenter|Qt::AlignCenter);targetVelocityXLabel->setMinimumWidth(175);
	targetVelocityZLabel=new QLabel(tr(""));targetVelocityZLabel->setMargin(5);targetVelocityZLabel->setMinimumWidth(minWidth);		targetVelocityGroupBoxLayout->addWidget(targetVelocityZLabel,Qt::AlignHCenter|Qt::AlignCenter);targetVelocityZLabel->setMinimumWidth(175);
	targetVelocityYLabel=new QLabel(tr(""));targetVelocityYLabel->setMargin(5);targetVelocityYLabel->setMinimumWidth(minWidth);		targetVelocityGroupBoxLayout->addWidget(targetVelocityYLabel,Qt::AlignHCenter|Qt::AlignCenter);targetVelocityYLabel->setMinimumWidth(175);
	btnsMainLayout5->addStretch(1);
	//btnsMainLayout6
	QGroupBox *alarmsSignalsGroupBox=new QGroupBox(tr("Alarms"));btnsMainLayout6->addWidget(alarmsSignalsGroupBox);
	QVBoxLayout *alarmsSignalsGroupBoxLayout=new QVBoxLayout();alarmsSignalsGroupBox->setLayout(alarmsSignalsGroupBoxLayout);
	signal_07_00=new QLabel(tr("Difference between reference\nand actual speed on up-down motion ."							));signal_07_00->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_07_00,Qt::AlignHCenter|Qt::AlignCenter);signal_07_00->setMinimumWidth(125);
	signal_07_01=new QLabel(tr("Difference between reference\nand actual speed on forward-backward motion ."		));signal_07_01->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_07_01,Qt::AlignHCenter|Qt::AlignCenter);signal_07_01->setMinimumWidth(125);
	signal_07_02=new QLabel(tr("Difference between reference\nand actual speed on left-right motion."						));signal_07_02->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_07_02,Qt::AlignHCenter|Qt::AlignCenter);signal_07_02->setMinimumWidth(125);
	signal_07_03=new QLabel(tr("Thermal overload or fault\nU61,U62,Q61,Q621,Q62 ."															));signal_07_03->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_07_03,Qt::AlignHCenter|Qt::AlignCenter);signal_07_03->setMinimumWidth(125);
	signal_07_04=new QLabel(tr("Thermal overload\nQ63,Q64,Q65 ."																								));signal_07_04->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_07_04,Qt::AlignHCenter|Qt::AlignCenter);signal_07_04->setMinimumWidth(125);
	signal_07_05=new QLabel(tr("Stop forward-backward ."																												));signal_07_05->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_07_05,Qt::AlignHCenter|Qt::AlignCenter);signal_07_05->setMinimumWidth(125);
	signal_07_06=new QLabel(tr("Stop up-down."																																	));signal_07_06->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_07_06,Qt::AlignHCenter|Qt::AlignCenter);signal_07_06->setMinimumWidth(125);
	signal_07_07=new QLabel(tr("Stop left-right."																																));signal_07_07->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_07_07,Qt::AlignHCenter|Qt::AlignCenter);signal_07_07->setMinimumWidth(125);
	signal_07_08=new QLabel(tr("Reset demand  after power-up."																									));signal_07_08->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_07_08,Qt::AlignHCenter|Qt::AlignCenter);signal_07_08->setMinimumWidth(125);
	signal_07_09=new QLabel(tr("Packet photosensor activated on packet export. Check packet position on forks."	));signal_07_09->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_07_09,Qt::AlignHCenter|Qt::AlignCenter);signal_07_09->setMinimumWidth(125);
	signal_07_10=new QLabel(tr("Packet photosensor activated on packet import. Forks should be empty."					));signal_07_10->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_07_10,Qt::AlignHCenter|Qt::AlignCenter);signal_07_10->setMinimumWidth(125);
	showAlarmsHistoryBtn=new QPushButton(tr("Show history"));alarmsSignalsGroupBoxLayout->addWidget(showAlarmsHistoryBtn,Qt::AlignHCenter|Qt::AlignCenter);showAlarmsHistoryBtn->setMinimumWidth(125);
	connect(showAlarmsHistoryBtn,SIGNAL(clicked()),this,SLOT(showAlarmsHistoryBtnSlot()));
	alarmsSignalsGroupBox->setLayout(alarmsSignalsGroupBoxLayout);
	btnsMainLayout6->addStretch(1);
	/////////////////
	//QScrollArea *btnsArea=new QScrollArea();
	//btnsArea->setWidget(btnsWidget);
	//QWidget *btnsAreaWidget=new QWidget();QVBoxLayout *btnsAreaWidgetLayout=new QVBoxLayout();btnsAreaWidget->setLayout(btnsAreaWidgetLayout);btnsAreaWidgetLayout->addWidget(btnsArea);

	mainSplitter->addWidget(btnsWidget);
	///////////////////////////////////////////////////////////////////////////faults
	QList<int> sizes;sizes<<750<<500;
	mainSplitter->setSizes(sizes);
	area->verticalScrollBar()->setValue(area->verticalScrollBar()->maximum());
}
void forkLiftManualClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	for (int i=0;i<pushButtonsVector.count();i++)
		pushButtonsVector[i]->updateValue(table,index,value.toInt());
}

void forkLiftManualClass::plcIntMemoryChanged(int index,int value)
{
	QVector<bool> bitsVector=deprecatedCalculateBits(value);
	if (index==0)
	{
		vLog("info","forkLiftManualClass::plcIntMemoryChanged:",QString("Load position:%1").arg(value));
		memory160_0_label->setText(tr("%1").arg(value));
	}
	if (index==1)
	{
		vLog("info","forkLiftManualClass::plcIntMemoryChanged:",QString("UnLoad position:%1").arg(value));
		memory160_1_label->setText(tr("%1").arg(value));
	}
	if (index==2)
	{
		vLog("info","forkLiftManualClass::plcIntMemoryChanged:",QString("PLC status:%1").arg(value));
		memory160_2_label->setText(tr("%1").arg(value));
	}
	if (index==3)
	{
		vLog("info","forkLiftManualClass::plcIntMemoryChanged:",QString("PC status:%1").arg(value));
		memory160_3_label->setText(tr("%1").arg(value));

	}
	if (index==4)
	{
		vLog("info","forkLiftManualClass::plcIntMemoryChanged:",QString("Packet Type:%1").arg(value));
		memory160_4_label->setText(tr("%1").arg(value));
	}
	if (index==5)
	{
		if (bitsVector[0])
		{
			hideButtons();
			vLog("info","fork plc mode:","auto");
			autoPushbutton->setStyleSheet("QPushButton{text-align:center;color:blue}");
		}
		else
			autoPushbutton->setStyleSheet("QPushButton{text-align:center;color:black}");
		if (bitsVector[1])
		{
			vLog("info","fork plc mode:","manual");
			manualPushbutton->setStyleSheet("QPushButton{text-align:center;color:blue}");
			showButtons();
		}
		else
			manualPushbutton->setStyleSheet("QPushButton{text-align:center;color:black}");
		if (bitsVector[2])
		{
			vLog("info","fork plc mode:","maitenance");
			maitenancePushbutton->setStyleSheet("QPushButton{text-align:center;color:blue}");
			showButtons();
		}
		else
			maitenancePushbutton->setStyleSheet("QPushButton{text-align:center;color:black}");
		//
		if (bitsVector[9])
		{
			vLog("info","fork plc:","active stop");
			stopLabel_5_9->setText("  Active stop");
			stopLabel_5_9->setStyleSheet("QLabel{text-align:center;color:white; border: 4px solid red;border-radius: 4px;}");
		}
		else
		{
			vLog("info","fork plc:","no active stop");
			stopLabel_5_9->setText("  No active stop");
			stopLabel_5_9->setStyleSheet("QLabel{text-align:center;color:gray}");
		}
		if (bitsVector[ 3])signal_05_03->setStyleSheet("QLabel{background-color:green}");else signal_05_03->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 4])signal_05_04->setStyleSheet("QLabel{background-color:green}");else signal_05_04->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 5])signal_05_05->setStyleSheet("QLabel{background-color:green}");else signal_05_05->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 6])signal_05_06->setStyleSheet("QLabel{background-color:green}");else signal_05_06->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 7])signal_05_07->setStyleSheet("QLabel{background-color:green}");else signal_05_07->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 8])signal_05_08->setStyleSheet("QLabel{background-color:green}");else signal_05_08->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[10])takePacketQPushButton->setEnabled(true);else takePacketQPushButton->setEnabled(false);
		if (bitsVector[11])signal_05_11->setStyleSheet("QLabel{background-color:green}");else signal_05_11->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[12])signal_05_12->setStyleSheet("QLabel{background-color:green}");else signal_05_12->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[13])signal_05_13->setStyleSheet("QLabel{background-color:green}");else signal_05_13->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[14])signal_05_14->setStyleSheet("QLabel{background-color:green}");else signal_05_14->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[15])signal_05_15->setStyleSheet("QLabel{background-color:green}");else signal_05_15->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[16])signal_05_16->setStyleSheet("QLabel{background-color:green}");else signal_05_16->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[17])signal_05_17->setStyleSheet("QLabel{background-color:green}");else signal_05_17->setStyleSheet("QLabel{background-color:lightgray}");
		//"N160[5].18"
		plcCan23TakeBasket=bitsVector[20];
		if (!plcCan23TakeBasket)signal_05_18->setStyleSheet("QLabel{background-color:red}");else signal_05_18->setStyleSheet("QLabel{background-color:lightgray}");
		plcCan22TakeBasket=bitsVector[19];
		//if (!plcCan22TakeBasket)signal_05_19->setStyleSheet("QLabel{background-color:red}");else signal_05_19->setStyleSheet("QLabel{background-color:lightgray}");
		plcCan21TakeBasket=bitsVector[22];
		//if (!plcCan21TakeBasket)signal_05_22->setStyleSheet("QLabel{background-color:red}");else signal_05_22->setStyleSheet("QLabel{background-color:lightgray}");
		continueMissionBtn->setEnabled(bitsVector[21]);
	}
	else if (index==7)
	{
		if (bitsVector[ 0]){signal_07_00->setStyleSheet("QLabel{background-color:red}");apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('forkError','N160[7].%1','%2','%3',%4)").arg( 0).arg("Difference between reference\nand actual speed on up-down motion."							).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));}else signal_07_00->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 1]){signal_07_01->setStyleSheet("QLabel{background-color:red}");apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('forkError','N160[7].%1','%2','%3',%4)").arg( 1).arg("Difference between reference\nand actual speed on forward-backward motion."			).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));}else signal_07_01->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 2]){signal_07_02->setStyleSheet("QLabel{background-color:red}");apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('forkError','N160[7].%1','%2','%3',%4)").arg( 2).arg("Difference between reference\nand actual speed on left-right motion."						).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));}else signal_07_02->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 3]){signal_07_03->setStyleSheet("QLabel{background-color:red}");apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('forkError','N160[7].%1','%2','%3',%4)").arg( 3).arg("Thermal overload or fault\nU61,U62,Q61,Q621,Q62."																).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));}else signal_07_03->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 4]){signal_07_04->setStyleSheet("QLabel{background-color:red}");apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('forkError','N160[7].%1','%2','%3',%4)").arg( 4).arg("Thermal overload\nQ63,Q64,Q65."																									).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));}else signal_07_04->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 5]){signal_07_05->setStyleSheet("QLabel{background-color:red}");apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('forkError','N160[7].%1','%2','%3',%4)").arg( 5).arg("Stop forward-backward."																													).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));}else signal_07_05->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 6]){signal_07_06->setStyleSheet("QLabel{background-color:red}");apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('forkError','N160[7].%1','%2','%3',%4)").arg( 6).arg("Stop up-down."																																	).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));}else signal_07_06->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 7]){signal_07_07->setStyleSheet("QLabel{background-color:red}");apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('forkError','N160[7].%1','%2','%3',%4)").arg( 7).arg("Stop left-right."																																).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));}else signal_07_07->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 8]){signal_07_08->setStyleSheet("QLabel{background-color:red}");apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('forkError','N160[7].%1','%2','%3',%4)").arg( 8).arg("Reset demand  after power-up."																									).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));}else signal_07_08->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 9]){signal_07_09->setStyleSheet("QLabel{background-color:red}");apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('forkError','N160[7].%1','%2','%3',%4)").arg( 9).arg("Packet photosensor activated on packet export. Check packet position on forks."	).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));}else signal_07_09->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[10]){signal_07_10->setStyleSheet("QLabel{background-color:red}");apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('forkError','N160[7].%1','%2','%3',%4)").arg(10).arg("Packet photosensor activated on packet import. Forks should be empty."					).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));}else signal_07_10->setStyleSheet("QLabel{background-color:lightgray}");
	}
	if ((index==8)||(index==7))
		missionFaultsBtn->dataInPlcChanged(forkIp,"N160",index,value);
}
void forkLiftManualClass::plcFloatMemoryChanged(int index,double value)
{
	if (index== 0)posXLabel->setText(tr("East-west position %1 mm").arg(value,8,'f',1));
	if (index== 1)posZLabel->setText(tr("Up-down position %1 mm").arg(value,8,'f',1));
	if (index== 2)posYLabel->setText(tr("North-south position %1 mm").arg(value,8,'f',1));
	if (index== 3)velocityXLabel->setText(tr("East-west velocity %1 m/min").arg(value,8,'f',1));
	if (index== 4)velocityZLabel->setText(tr("Up-down velocity %1 m/min").arg(value,8,'f',1));
	if (index== 5)velocityYLabel->setText(tr("North-south velocity %1 m/min").arg(value,8,'f',1));
	if (index== 6)targetPosXLabel->setText(tr("East-west position %1 mm").arg(value,8,'f',1));
	if (index== 7)targetPosZLabel->setText(tr("Up-down position %1 mm").arg(value,8,'f',1));
	if (index== 8)targetPosYLabel->setText(tr("North-south position %1 mm").arg(value,8,'f',1));
	if (index== 9)targetVelocityXLabel->setText(tr("East-west velocity %1 Hz").arg(value,8,'f',1));
	if (index==10)targetVelocityZLabel->setText(tr("Up-down velocity %1 Hz").arg(value,8,'f',1));
	if (index==11)targetVelocityYLabel->setText(tr("North-south velocity %1 Hz").arg(value,8,'f',1));
}
bool forkLiftManualClass::get23CanTakeBasket()
{
	return plcCan23TakeBasket;
}
bool forkLiftManualClass::get22CanTakeBasket()
{
	return plcCan22TakeBasket;
}
bool forkLiftManualClass::get21CanTakeBasket()
{
	return plcCan21TakeBasket;
}
void forkLiftManualClass::hideButtons()
{
	for (int i=0;i<areaButtonsVector.count();i++)
		areaButtonsVector[i]->hide();
}
void forkLiftManualClass::showButtons()
{
	for (int i=0;i<areaButtonsVector.count();i++)
		areaButtonsVector[i]->show();
}
void forkLiftManualClass::abordMissionBtnSlot()
{
	QString passStr=QInputDialog::getText(this,tr("Password neaded for this operation"),tr("Verify that the packet remain in the initial position.\nPlease input the password"),QLineEdit::Password,"");
	if(checkStrongPass(passStr))
		tcpServer->sendString(QString("N160|23|428"));
	else
	{
		if (!passStr.isEmpty())
			QMessageBox::warning(0,tr("Warning"),tr("The password that you typed is incorrect"));
	}
}
void forkLiftManualClass::closeMissionBtnSlot()
{
	QString passStr=QInputDialog::getText(this,tr("Password neaded for this operation"),tr("Verify that the packet is on the target position and the mission finished succesfully.\nPlease input the password"),QLineEdit::Password,"");
	if(checkStrongPass(passStr))
		tcpServer->sendString(QString("N160|23|430"));
	else
	{
		if (!passStr.isEmpty())
			QMessageBox::warning(0,tr("Warning"),tr("The password that you typed is incorrect"));
	}
}
void forkLiftManualClass::continueMissionBtnSlot()
{
	QString passStr=QInputDialog::getText(this,tr("Password neaded for this operation"),tr("Verify forklift can continue mission.\nPlease input the password"),QLineEdit::Password,"");
	if(checkStrongPass(passStr))
		tcpServer->sendString(QString("N160|23|435"));
	else
	{
		if (!passStr.isEmpty())
			QMessageBox::warning(0,tr("Warning"),tr("The password that you typed is incorrect"));
	}
}
void forkLiftManualClass::showAlarmsHistoryBtnSlot()
{
	QVector<QVariant> bitsVector=apexExecQuery("select top 100 bit from logErrorPlc where type='forkError' order by tid desc");
	QVector<QVariant> descriptionVector=apexExecQuery("select top 100 description from logErrorPlc where type='forkError' order by tid desc");
	QVector<QVariant> dateVector=apexExecQuery("select top 100 date from logErrorPlc where type='forkError' order by tid desc");
	QDialog dlg(this);
	QVBoxLayout *mainLayout=new QVBoxLayout();dlg.setLayout(mainLayout);
	QTableWidget *errorsTableWidget=new QTableWidget();mainLayout->addWidget(errorsTableWidget);
	errorsTableWidget->setAlternatingRowColors(true);
	errorsTableWidget->horizontalHeader()->setStretchLastSection(true);
	errorsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	errorsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	errorsTableWidget->setColumnCount(2);
	QStringList ch;ch<<tr("Date")<<tr("Description");
	errorsTableWidget->setHorizontalHeaderLabels(ch);
	errorsTableWidget->setRowCount(bitsVector.count());
	for (int i=0;i<bitsVector.count();i++)
	{
		errorsTableWidget->setItem(i,0,new QTableWidgetItem(dateVector[i].toString(),Qt::DisplayRole));
		errorsTableWidget->setItem(i,1,new QTableWidgetItem(bitsVector[i].toString()+":"+descriptionVector[i].toString(),Qt::DisplayRole));
	}
	errorsTableWidget->resizeColumnsToContents();
	dlg.resize(600,900);
	dlg.exec();
}

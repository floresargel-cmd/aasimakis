#include "p.h"
int titleHeight=20;
pincerClass::pincerClass(tcpServerClass *tcpServer_)
{
	//this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px}  QCheckBox{font-size:%1px}").arg(12));
	tcpServer=tcpServer_;
	fromPosition=toPosition=NULL;
	//
	plcModeEnum=NO_MODE;
	N22_12=0;
	N22_21_10=false;
	N22_21_11=false;
	N22_14_08=false;
	N22_14_09=false;
	N22_14_10=false;
	secondSpliterPaneWidth=0;
	QHBoxLayout *topLayout=new QHBoxLayout();
	setLayout(topLayout);
	mainSplitter=new QSplitter(Qt::Horizontal,this);topLayout->addWidget(mainSplitter);
	QScrollArea *leftScrollArea=new QScrollArea();mainSplitter->addWidget(leftScrollArea);
	leftScrollArea->setWidgetResizable(true);
	QWidget *leftScrollAreaWidget=new QWidget();
	QHBoxLayout *topTopLeftLayout=new QHBoxLayout();leftScrollAreaWidget->setLayout(topTopLeftLayout);
	QWidget *topLeftWidget=new QWidget();topTopLeftLayout->addWidget(topLeftWidget);
	QScrollArea *rightScrollArea=new QScrollArea();mainSplitter->addWidget(rightScrollArea);
	rightScrollArea->setWidgetResizable(true);
	QWidget *topRightWidget=new QWidget();
	rightScrollArea->setBackgroundRole(QPalette::Dark);
	connect(mainSplitter,SIGNAL(splitterMoved(int,int)),this,SLOT(splitterMovedSlot(int,int)));


	QVBoxLayout *topLeftLayout=new QVBoxLayout();topLeftWidget->setLayout(topLeftLayout);
		QGridLayout *leftUpLayout=new QGridLayout();topLeftLayout->addLayout(leftUpLayout);
		QHBoxLayout *leftMidLayout=new QHBoxLayout();topLeftLayout->addLayout(leftMidLayout);//positions
		QHBoxLayout *leftDownLayout=new QHBoxLayout();topLeftLayout->addLayout(leftDownLayout);
			QVBoxLayout *leftDownLayout0=new QVBoxLayout();leftDownLayout->addLayout(leftDownLayout0);
			QVBoxLayout *leftDownLayout1=new QVBoxLayout();leftDownLayout->addLayout(leftDownLayout1);
																										 leftDownLayout->addStretch(1);

	QHBoxLayout *topRightLayout=new QHBoxLayout();topRightWidget->setLayout(topRightLayout);
	//
	QImage image(":/plcPos.png");
	QLabel *imageLabel=new QLabel();
	imageLabel->setMinimumWidth(624);
	imageLabel->setMinimumHeight(306);
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setAlignment(Qt::AlignCenter);
	leftDownLayout1->addWidget(imageLabel);

	//upLayout
	leftUpLayout->addWidget(new QLabel(tr("From PC position")),0,0);
	fromLabel=new QLabel("...");
	fromLabel->setMinimumWidth(50);
	fromLabel->setStyleSheet("QLabel{color:#0000aa;text-align:center;font-size:16px;}");
	leftUpLayout->addWidget(fromLabel,0,1);

	leftUpLayout->addWidget(new QLabel(tr("To PC position")),0,2);
	toLabel=new QLabel("...");
	toLabel->setMinimumWidth(50);
	toLabel->setStyleSheet("QLabel{color:#0000aa;text-align:center;font-size:16px;}");
	leftUpLayout->addWidget(toLabel,0,3);
	//
	executeBtn=new myQPushButton(tcpServer,"N22|3|27","N22|3|0",tr(""),NULL);executeBtn->setIcon(QIcon(":/start.png"));
	executeBtn->setIconSize(QSize(50,50));
	executeBtn->setGeometry(0,0,50,50);
	connect(executeBtn,SIGNAL(clicked()),this, SLOT(onExecuteBtnClickedSlot()));
	leftUpLayout->addWidget(executeBtn,0,4,3,1);

	clearBtn=new QPushButton(tr(""));clearBtn->setIcon(QIcon(":/clear.png"));
	clearBtn->setIconSize(QSize(50,50));
	clearBtn->setGeometry(0,0,50,50);
	connect(clearBtn,SIGNAL(clicked()),this, SLOT(onClearBtnClickedSlot()));
	leftUpLayout->addWidget(clearBtn,0,5,3,1);
	clearBtn->setEnabled(false);
	///
	leftUpLayout->addWidget(new QLabel(tr("From Plc pos")),1,0);
	fromPlcLabel=new QLabel("...");
	fromPlcLabel->setMinimumWidth(50);
	fromPlcLabel->setStyleSheet("QLabel{color:#0000aa;text-align:center;font-size:16px;}");
	leftUpLayout->addWidget(fromPlcLabel,1,1);

	leftUpLayout->addWidget(new QLabel(tr("To Plc pos")),1,2);
	toPlcLabel=new QLabel("...");
	toPlcLabel->setMinimumWidth(50);
	toPlcLabel->setStyleSheet("QLabel{color:#0000aa;text-align:center;font-size:16px;}");
	leftUpLayout->addWidget(toPlcLabel,1,3);

	fromToPlcStatus=new QLabel("...");
	fromToPlcStatus->setStyleSheet("QLabel{color:#0000aa;text-align:center;font-size:12px;}");
	leftUpLayout->addWidget(fromToPlcStatus,2,0,1,4,Qt::AlignHCenter);
	leftUpLayout->setColumnStretch(6,1);
	//
	QScrollArea *posScrollArea=new QScrollArea();
	posScrollArea->setAlignment(Qt::AlignCenter);posScrollArea->setBackgroundRole(QPalette::Shadow);
	leftMidLayout->addWidget(posScrollArea);
	posScrollArea->setWidgetResizable(true);
	QWidget *posScrollAreaWidget=new QWidget();
	QHBoxLayout *posScrollAreaWidgetLayout=new QHBoxLayout();posScrollAreaWidget->setLayout(posScrollAreaWidgetLayout);
	posScrollArea->setWidget(posScrollAreaWidget);

	QLabel *posLayoutImageLabel=new QLabel;
	posLayoutImageLabel->setMinimumWidth(1750);posLayoutImageLabel->setMinimumHeight(331);
	posLayoutImageLabel->setMaximumWidth(1750);posLayoutImageLabel->setMaximumHeight(331);
	posLayoutImageLabel->setPixmap(QPixmap::fromImage(QImage(":/pincerLayout.png")));
	posLayoutImageLabel->setAlignment(Qt::AlignCenter);
	posScrollAreaWidgetLayout->addWidget(posLayoutImageLabel);

	mainPosition12=createPosition(tr("Pack 1 belts a")		,120   ,16,16,16,-1,-1,-1,-1,-1,-1,            0, 1, 2,-1,-1,-1,-1,-1,-1,          1, 2, 3,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);mainPosition12->move(230, 95-22);
	mainPosition11=createPosition(tr("Pack 1 belts b")		,110   ,16,16,16,-1,-1,-1,-1,-1,-1,            3, 4, 5,-1,-1,-1,-1,-1,-1,          4, 5, 6,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);mainPosition11->move(230,130-22);
	mainPosition10=createPosition(tr("Pack 1 down")				,100   ,16,16,16,-1,-1,-1,-1,-1,-1,            6, 7, 8,-1,-1,-1,-1,-1,-1,          7, 8, 9,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);mainPosition10->move(230,165-22);
	mainPosition30=createPosition(tr("Pack 1 scrap")			,300   ,16,16,16,-1,-1,-1,-1,-1,-1,            9,10,11,-1,-1,-1,-1,-1,-1,         10,11,12,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);mainPosition30->move(230,230-22);

	mainPosition21=createPosition(tr("Pack 2 belts")			,210   ,16,16,16,-1,-1,-1,-1,-1,-1,           12,13,14,-1,-1,-1,-1,-1,-1,         13,14,15,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);mainPosition21->move(740, 45-22);
	mainPosition20=createPosition(tr("Pack 2 down")				,200   ,16,17,17,-1,-1,-1,-1,-1,-1,           15, 0, 1,-1,-1,-1,-1,-1,-1,         16,17,18,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);mainPosition20->move(740, 85-22);
	mainPosition40=createPosition(tr("Pack 2 scrap")			,400   ,17,17,17,-1,-1,-1,-1,-1,-1,            2, 3, 4,-1,-1,-1,-1,-1,-1,         19,20,21,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);mainPosition40->move(740,160-22);
	mainPosition61=createPosition(tr("From out")					,610   ,17,17,17,17,17,17,17,17,17,            5, 6, 7, 8, 9,10,11,12,13,         22,23,24,25,26,27,28,29,30,posLayoutImageLabel);mainPosition61->move(720,227-22);
	mainPosition60=createPosition(tr("New empty")					,600   ,17,17,18,18,18,18,18,18,18,           14,15, 0, 1, 2, 3, 4, 5, 6,         31,32,33,34,35,36,37,38,39,posLayoutImageLabel);mainPosition60->move(720,275-22);

	moveFrom60to61Label=new QLabel("",posLayoutImageLabel);
	moveFrom60to61Label->move(650,228);

	moveFrom60to61Pushbutton=new myQPushButton(tcpServer,"N22|3|26","N22|3|0","",posLayoutImageLabel);
	moveFrom60to61Pushbutton->setIcon(QIcon(":/arrowMoveUpSmall.png"));
	moveFrom60to61Pushbutton->setIconSize(QSize(25,25));moveFrom60to61Pushbutton->move(680,228);
	if (true)//all positions
	{
		mainPosition54=createPosition(tr("From out")					,540   ,18,18,18,18,18,18,-1,-1,-1,            7, 8, 9,10,11,12,-1,-1,-1,         40,41,42,43,44,45,-1,-1,-1,posLayoutImageLabel);
		mainPosition53=createPosition(tr("Empty")							,530   ,18,18,18,19,19,19,19,19,19,           13,14,15, 0, 1, 2, 3, 4, 5,         46,47,48,49,50,51,52,53,54,posLayoutImageLabel);
		mainPosition52=createPosition(tr("Empty")							,520	 ,19,19,19,19,19,19,19,19,19,            6, 7, 8, 9,10,11,12,13,14,         55,56,57,58,59,60,61,62,63,posLayoutImageLabel);
		mainPosition51=createPosition(tr("Full")							,510	 ,19,20,20,20,20,20,20,20,20,           15, 0, 1, 2, 3, 4, 5, 6, 7,         64,65,66,67,68,69,70,71,72,posLayoutImageLabel);
		mainPosition50=createPosition(tr("Full")							,500	 ,20,20,20,20,20,20,20,20,21,            8, 9,10,11,12,13,14,15, 0,         73,74,75,76,77,78,79,80,81,posLayoutImageLabel);
	}
	else //only the bottom positions
	{
		mainPosition54=createPosition(tr("From out")					,540   ,18,18,18,-1,-1,-1,-1,-1,-1,             7, 8, 9,-1,-1,-1,-1,-1,-1,         40,41,42,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);
		mainPosition53=createPosition(tr("Empty")							,530   ,18,18,18,-1,-1,-1,-1,-1,-1,            13,14,15,-1,-1,-1,-1,-1,-1,         46,47,48,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);
		mainPosition52=createPosition(tr("Empty")							,520	 ,19,19,19,-1,-1,-1,-1,-1,-1,             6, 7, 8,-1,-1,-1,-1,-1,-1,         55,56,57,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);
		mainPosition51=createPosition(tr("Full")							,510	 ,19,20,20,-1,-1,-1,-1,-1,-1,            15, 0, 1,-1,-1,-1,-1,-1,-1,         64,65,66,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);
		mainPosition50=createPosition(tr("Full")							,500	 ,20,20,20,-1,-1,-1,-1,-1,-1,             8, 9,10,-1,-1,-1,-1,-1,-1,         73,74,75,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);
	}
	mainPosition54->move(1145, 48-22);
	mainPosition53->move(1130, 93-22);
	mainPosition52->move(1130,137-22);
	mainPosition51->move(1130,182-22);
	mainPosition50->move(1130,227-22);

	mainPosition55=createPosition(tr("From anodizing")			,550   ,21,21,21,-1,-1,-1,-1,-1,-1,            1, 2, 3,-1,-1,-1,-1,-1,-1,         82,83,84,-1,-1,-1,-1,-1,-1,posLayoutImageLabel);mainPosition55->move(1665, 30);

	removeBasketBtn=new myQPushButton(tcpServer,"N22|3|28","N22|3|0","",posLayoutImageLabel);removeBasketBtn->move(1634, 0);
	connect(&removeBasketBlinkingBtnTimer,SIGNAL(timeout()),this,SLOT(removeBasketBlinkingBtnTimerSlot()));
	removeBasketBtn->setMinimumSize(100,20);

	moveFrom54to55Pushbutton=new myQPushButton(tcpServer,"N22|3|24","N22|3|0","",posLayoutImageLabel);moveFrom54to55Pushbutton->move(1500, 0);
	moveFrom54to55Pushbutton->setMinimumWidth(100);
	moveFrom54to55Pushbutton->setIcon(QIcon(":/arrowMoveRight.png"));
	moveFrom54to55Pushbutton->setIconSize(QSize(20,20));

	moveFrom54to55Label=new QLabel("",posLayoutImageLabel);moveFrom54to55Label->move(1540, 30);
	moveFrom54to55Label->setPixmap(QPixmap(":/arrowLeft.png"));

	moveFrom55to54Pushbutton=new QPushButton("",posLayoutImageLabel);moveFrom55to54Pushbutton->move(1500, 55);
	moveFrom55to54Pushbutton->setMinimumWidth(100);
	moveFrom55to54Pushbutton->setIcon(QIcon(":/arrowMoveLeft.png"));
	moveFrom55to54Pushbutton->setIconSize(QSize(20,20));
	connect(moveFrom55to54Pushbutton,SIGNAL(clicked()),this,SLOT(moveFrom55to54PushbuttonSlot()));
	connect(moveFrom55to54Pushbutton,SIGNAL(released()),this,SLOT(buttonReleasedSlot()));

	//leftDownLayout0
	stopLabel=new QLabel("");
	stopLabel->setAlignment(Qt::AlignHCenter);
	stopLabel->setPixmap(QPixmap(":/stopNull.png"));
	leftDownLayout0->addWidget(stopLabel);

	recallStopPushButton=new myQPushButton(tcpServer,"N22|3|23","N22|3|0","Stop recall",NULL);
	leftDownLayout0->addWidget(recallStopPushButton);

	okLabel=new QLabel("...");
	okLabel->setAlignment(Qt::AlignHCenter|Qt::AlignCenter);
	okLabel->setFont(QFont("Arial",12));
	okLabel->setMinimumHeight(30);
	leftDownLayout0->addWidget(okLabel);

	makeResetLabel=new QLabel("...");
	makeResetLabel->setAlignment(Qt::AlignHCenter|Qt::AlignCenter);
	makeResetLabel->setFont(QFont("Arial",12));
	makeResetLabel->setMinimumHeight(30);
	leftDownLayout0->addWidget(makeResetLabel);

	alarm_13_1Label=new QLabel("...");
	alarm_13_1Label->setAlignment(Qt::AlignHCenter|Qt::AlignCenter);
	alarm_13_1Label->setFont(QFont("Arial",12));
	alarm_13_1Label->setMinimumHeight(50);
	leftDownLayout0->addWidget(alarm_13_1Label);

	alarm_13_2Label=new QLabel("...");
	alarm_13_2Label->setAlignment(Qt::AlignHCenter|Qt::AlignCenter);
	alarm_13_2Label->setFont(QFont("Arial",11));
	alarm_13_2Label->setMinimumHeight(50);
	leftDownLayout0->addWidget(alarm_13_2Label);


	leftDownLayout0->addStretch(1);
	////////////////////////////////////////////////////////////
	//rightLayout
	QVBoxLayout *rightLayout0=new QVBoxLayout();topRightLayout->addLayout(rightLayout0);
	QVBoxLayout *rightLayout1=new QVBoxLayout();topRightLayout->addLayout(rightLayout1);
	QVBoxLayout *rightLayout2=new QVBoxLayout();topRightLayout->addLayout(rightLayout2);
	QVBoxLayout *rightLayout3=new QVBoxLayout();topRightLayout->addLayout(rightLayout3);
	//autoRadios
	QGroupBox *radioButtonsGroupBox=new QGroupBox(tr("PLC Mode"));rightLayout0->addWidget(radioButtonsGroupBox);
	QVBoxLayout *radioButtonsMainLayout=new QVBoxLayout();radioButtonsGroupBox->setLayout(radioButtonsMainLayout);
	QHBoxLayout *radioButtonsLayout=new QHBoxLayout();radioButtonsMainLayout->addLayout(radioButtonsLayout);
	autoRadioButton=new QRadioButton(tr("Auto"));radioButtonsLayout->addWidget(autoRadioButton);											connect(autoRadioButton,SIGNAL(released()),this,SLOT(buttonReleasedSlot()));connect(autoRadioButton,SIGNAL(clicked(bool)),this,SLOT(autoRadioButtonSlot(bool)));
	stepRadioButton=new QRadioButton(tr("Step"));radioButtonsLayout->addWidget(stepRadioButton);											connect(stepRadioButton,SIGNAL(released()),this,SLOT(buttonReleasedSlot()));connect(stepRadioButton,SIGNAL(clicked(bool)),this,SLOT(setpRadioButtonSlot(bool)));
	maintenanceRadioButton=new QRadioButton(tr("Maintenance"));radioButtonsLayout->addWidget(maintenanceRadioButton);	connect(maintenanceRadioButton,SIGNAL(released()),this,SLOT(buttonReleasedSlot()));connect(maintenanceRadioButton,SIGNAL(clicked(bool)),this,SLOT(maintenanceRadioButtonSlot(bool)));

	QHBoxLayout *makeStepLayout=new QHBoxLayout();radioButtonsMainLayout->addLayout(makeStepLayout);
	makeStepBtn=new myQPushButton(tcpServer,"N22|3|4","N22|3|0",tr("Make step"),NULL);makeStepBtn->setDisabled(true);
	makeStepLayout->addWidget(makeStepBtn);

	radioButtonsLayout->addStretch(1);
	//commands group
	QGroupBox *commandsGroupBox=new QGroupBox(tr("PLC commands"));rightLayout0->addWidget(commandsGroupBox);
	QVBoxLayout *commandsGroupBoxLayout=new QVBoxLayout();commandsGroupBox->setLayout(commandsGroupBoxLayout);
	command10PushButton=new myQPushButton(tcpServer,"N22|3|10","N22|3|0",tr("Move axis z up "														),NULL);commandsGroupBoxLayout->addWidget(command10PushButton);
	command11PushButton=new myQPushButton(tcpServer,"N22|3|11","N22|3|0",tr("Move axis z down"													),NULL);commandsGroupBoxLayout->addWidget(command11PushButton);
	command12PushButton=new myQPushButton(tcpServer,"N22|3|12","N22|3|0",tr("Move axis x to anodizing"									),NULL);commandsGroupBoxLayout->addWidget(command12PushButton);
	command13PushButton=new myQPushButton(tcpServer,"N22|3|13","N22|3|0",tr("Move axis x to pack 1"											),NULL);commandsGroupBoxLayout->addWidget(command13PushButton);
	command14PushButton=new myQPushButton(tcpServer,"N22|3|14","N22|3|0",tr("Move axis y to press"											),NULL);commandsGroupBoxLayout->addWidget(command14PushButton);
	command15PushButton=new myQPushButton(tcpServer,"N22|3|15","N22|3|0",tr("Move axis y to anodizing"									),NULL);commandsGroupBoxLayout->addWidget(command15PushButton);
	command16PushButton=new myQPushButton(tcpServer,"N22|3|16","N22|3|0",tr("Close pincers to get shipping basket"			),NULL);commandsGroupBoxLayout->addWidget(command16PushButton);
	command17PushButton=new myQPushButton(tcpServer,""        ,"N22|3|0",tr("Open pincers to release shipping basket"		),NULL);commandsGroupBoxLayout->addWidget(command17PushButton);
	command18PushButton=new myQPushButton(tcpServer,"N22|3|18","N22|3|0",tr("Lock shipping basket in upper position"		),NULL);commandsGroupBoxLayout->addWidget(command18PushButton);
	command19PushButton=new myQPushButton(tcpServer,"N22|3|19","N22|3|0",tr("Unlock shipping basket in upper position"	),NULL);commandsGroupBoxLayout->addWidget(command19PushButton);
	connect(command17PushButton,SIGNAL(clicked(bool)),this,SLOT(openToReleaseBasketSlot()));

	//reset group
	QGroupBox *resetGroupBox=new QGroupBox(tr("PLC reset"));rightLayout0->addWidget(resetGroupBox);
	QGridLayout *resetGroupBoxLayout=new QGridLayout();resetGroupBox->setLayout(resetGroupBoxLayout);
	command20PushButton=new myQPushButton(tcpServer,"N22|3|20","N22|3|0",tr("Reset Z axis "),NULL);resetGroupBoxLayout->addWidget(command20PushButton,0,0);
	command21PushButton=new myQPushButton(tcpServer,"N22|3|21","N22|3|0",tr("Reset X axis "),NULL);resetGroupBoxLayout->addWidget(command21PushButton,1,0);
	command22PushButton=new myQPushButton(tcpServer,"N22|3|22","N22|3|0",tr("Reset Y axis "),NULL);resetGroupBoxLayout->addWidget(command22PushButton,2,0);

	resetingZLabel=new QLabel(tr("   On Z reset position  "));resetingZLabel->setMinimumWidth(100);resetGroupBoxLayout->addWidget(resetingZLabel,0,1,1,1,Qt::AlignHCenter);
	resetingXLabel=new QLabel(tr("   On X reset position  "));resetingXLabel->setMinimumWidth(100);resetGroupBoxLayout->addWidget(resetingXLabel,1,1,1,1,Qt::AlignHCenter);
	resetingYLabel=new QLabel(tr("   On Y reset position  "));resetingYLabel->setMinimumWidth(100);resetGroupBoxLayout->addWidget(resetingYLabel,2,1,1,1,Qt::AlignHCenter);
	//state group
	QGroupBox *stateGroupBox=new QGroupBox(tr("PLC states"));rightLayout0->addWidget(stateGroupBox);
	QVBoxLayout *stateGroupBoxLayout=new QVBoxLayout();stateGroupBox->setLayout(stateGroupBoxLayout);

	stateLabel_14_10=new QLabel(tr("       Executing mission"								));stateLabel_14_10->setMinimumWidth(100);stateGroupBoxLayout->addWidget(stateLabel_14_10,Qt::AlignHCenter);stateLabel_14_10->setMinimumHeight(25);
	stateLabel_14_11=new QLabel(tr("       With basket in unload mission"		));stateLabel_14_11->setMinimumWidth(100);stateGroupBoxLayout->addWidget(stateLabel_14_11,Qt::AlignHCenter);stateLabel_14_11->setMinimumHeight(25);
	stateLabel_14_12=new QLabel(tr("       Mission finished, going up "	    ));stateLabel_14_12->setMinimumWidth(100);stateGroupBoxLayout->addWidget(stateLabel_14_12,Qt::AlignHCenter);stateLabel_14_12->setMinimumHeight(25);
	stateLabel_21_12=new QLabel(tr("       Load target has full basket"			));stateLabel_21_12->setMinimumWidth(100);stateGroupBoxLayout->addWidget(stateLabel_21_12,Qt::AlignHCenter);stateLabel_21_12->setMinimumHeight(25);
	stateLabel_21_14=new QLabel(tr("       Is moving a full basket"					));stateLabel_21_14->setMinimumWidth(100);stateGroupBoxLayout->addWidget(stateLabel_21_14,Qt::AlignHCenter);stateLabel_21_14->setMinimumHeight(25);
	stateLabel_21_15=new QLabel(tr("       Is moving an empty basket"				));stateLabel_21_15->setMinimumWidth(100);stateGroupBoxLayout->addWidget(stateLabel_21_15,Qt::AlignHCenter);stateLabel_21_15->setMinimumHeight(25);
	rightLayout0->addStretch(1);
	///////////////////////////////////////////////////////////////////
	//valve group
	QGroupBox *valveGroupBox=new QGroupBox(tr("Valves"));rightLayout1->addWidget(valveGroupBox);
	QVBoxLayout *valveGroupBoxLayout=new QVBoxLayout();valveGroupBox->setLayout(valveGroupBoxLayout);
	valve_14_13=new QLabel(tr("  Pincer crane lock load in upper position"		)); valve_14_13->setMinimumWidth(250);valveGroupBoxLayout->addWidget(valve_14_13,Qt::AlignHCenter);valve_14_13->setMinimumHeight(30);
	valve_14_14=new QLabel(tr("  Pincer crane unlock load in upper position"	)); valve_14_14->setMinimumWidth(250);valveGroupBoxLayout->addWidget(valve_14_14,Qt::AlignHCenter);valve_14_14->setMinimumHeight(30);
	valve_14_15=new QLabel(tr("  Pincers closed to get shipping Basket"				)); valve_14_15->setMinimumWidth(250);valveGroupBoxLayout->addWidget(valve_14_15,Qt::AlignHCenter);valve_14_15->setMinimumHeight(30);
	valve_15_00=new QLabel(tr("  Pincers opened to release shipping Basket"		));	valve_15_00->setMinimumWidth(250);valveGroupBoxLayout->addWidget(valve_15_00,Qt::AlignHCenter);valve_15_00->setMinimumHeight(30);

	//targets group

	QGroupBox *targetsGroupBox=new QGroupBox(tr("Targets"));rightLayout1->addWidget(targetsGroupBox);
	QVBoxLayout *targetsGroupBoxLayout=new QVBoxLayout();targetsGroupBox->setLayout(targetsGroupBoxLayout);

	loadTargetZLabel=new QLabel("...");targetsGroupBoxLayout->addWidget(loadTargetZLabel);
	loadTargetXLabel=new QLabel("...");targetsGroupBoxLayout->addWidget(loadTargetXLabel);
	loadTargetYLabel=new QLabel("...");targetsGroupBoxLayout->addWidget(loadTargetYLabel);

	unloadTargetZLabel=new QLabel("...");targetsGroupBoxLayout->addWidget(unloadTargetZLabel);
	unloadTargetXLabel=new QLabel("...");targetsGroupBoxLayout->addWidget(unloadTargetXLabel);
	unloadTargetYLabel=new QLabel("...");targetsGroupBoxLayout->addWidget(unloadTargetYLabel);

	stepTargetZLabel=new QLabel("...");targetsGroupBoxLayout->addWidget(stepTargetZLabel);
	stepTargetXLabel=new QLabel("...");targetsGroupBoxLayout->addWidget(stepTargetXLabel);
	stepTargetYLabel=new QLabel("...");targetsGroupBoxLayout->addWidget(stepTargetYLabel);

	//reals pos group
	QGroupBox *realGroupBox=new QGroupBox(tr("Position"));rightLayout1->addWidget(realGroupBox);
	QVBoxLayout *realGroupBoxLayout=new QVBoxLayout();realGroupBox->setLayout(realGroupBoxLayout);

	realPositionZLabel=new QLabel("...");realGroupBoxLayout->addWidget(realPositionZLabel);
	realPositionXLabel=new QLabel("...");realGroupBoxLayout->addWidget(realPositionXLabel);
	realPositionYLabel=new QLabel("...");realGroupBoxLayout->addWidget(realPositionYLabel);
	//velocity group
	QGroupBox *velocityGroupBox=new QGroupBox(tr("Velocity"));rightLayout1->addWidget(velocityGroupBox);
	QVBoxLayout *velocityGroupBoxLayout=new QVBoxLayout();velocityGroupBox->setLayout(velocityGroupBoxLayout);

	setSpeedZLabel=new QLabel("...");velocityGroupBoxLayout->addWidget(setSpeedZLabel);
	setSpeedXLabel=new QLabel("...");velocityGroupBoxLayout->addWidget(setSpeedXLabel);
	setSpeedYLabel=new QLabel("...");velocityGroupBoxLayout->addWidget(setSpeedYLabel);

	realSpeedZLabel=new QLabel("...");velocityGroupBoxLayout->addWidget(realSpeedZLabel);
	realSpeedXLabel=new QLabel("...");velocityGroupBoxLayout->addWidget(realSpeedXLabel);
	realSpeedYLabel=new QLabel("...");velocityGroupBoxLayout->addWidget(realSpeedYLabel);

	rightLayout1->addStretch(1);
	/////////////
	//sensors group
	QGroupBox *sensorsGroupBox=new QGroupBox(tr("Sensors"));rightLayout2->addWidget(sensorsGroupBox);
	QVBoxLayout *sensorsGroupBoxLayout=new QVBoxLayout();sensorsGroupBox->setLayout(sensorsGroupBoxLayout);
	sensor_13_14=new QLabel(tr("  Magnetic switch(x2) pincer crane lock\n  load in upper position pins out lock"											));		sensor_13_14->setMinimumWidth(250);sensorsGroupBoxLayout->addWidget(sensor_13_14,Qt::AlignHCenter);sensor_13_14->setMinimumHeight(50);
	sensor_13_15=new QLabel(tr("  Magnetic switch(x2) pincer crane lock\n  load in upper position pins in unlock"											));		sensor_13_15->setMinimumWidth(250);sensorsGroupBoxLayout->addWidget(sensor_13_15,Qt::AlignHCenter);sensor_13_15->setMinimumHeight(50);
	sensor_14_00=new QLabel(tr("  Magnetic switch(x2) pincer crane pincers\n  in close pos to get shipping basket"										));		sensor_14_00->setMinimumWidth(250);sensorsGroupBoxLayout->addWidget(sensor_14_00,Qt::AlignHCenter);sensor_14_00->setMinimumHeight(50);
	sensor_14_01=new QLabel(tr("  Magnetic switch(x2) pincer crane pincers\n  in open pos to release shipping basket"									));		sensor_14_01->setMinimumWidth(250);sensorsGroupBoxLayout->addWidget(sensor_14_01,Qt::AlignHCenter);sensor_14_01->setMinimumHeight(50);
	sensor_14_02=new QLabel(tr("  Photocell full shipping basket feed\n  chain shipping basket in anodizing position"									));		sensor_14_02->setMinimumWidth(250);sensorsGroupBoxLayout->addWidget(sensor_14_02,Qt::AlignHCenter);sensor_14_02->setMinimumHeight(50);
	sensor_14_03=new QLabel(tr("  Photocell full shipping basket feed\n  chain products in shipping basket \n  in anodizing position"	));		sensor_14_03->setMinimumWidth(250);sensorsGroupBoxLayout->addWidget(sensor_14_03,Qt::AlignHCenter);sensor_14_03->setMinimumHeight(50);
	sensor_14_04=new QLabel(tr("  Proximity switch full shipping basket\n  feed chain shipping basket\n  in pincer crane position"		));		sensor_14_04->setMinimumWidth(250);sensorsGroupBoxLayout->addWidget(sensor_14_04,Qt::AlignHCenter);sensor_14_04->setMinimumHeight(50);
	sensor_14_05=new QLabel(tr("  Proximity switch empty shipping basket\n  feed chain shipping baskets \n  in loading position"			));		sensor_14_05->setMinimumWidth(250);sensorsGroupBoxLayout->addWidget(sensor_14_05,Qt::AlignHCenter);sensor_14_05->setMinimumHeight(50);
	sensor_14_06=new QLabel(tr("  Proximity switch empty shipping basket\n  feed chain shipping baskets \n  in pincercrane position"	));		sensor_14_06->setMinimumWidth(250);sensorsGroupBoxLayout->addWidget(sensor_14_06,Qt::AlignHCenter);sensor_14_06->setMinimumHeight(50);

	//signals group
	QGroupBox *signalsGroupBox=new QGroupBox(tr("Signals"));rightLayout2->addWidget(signalsGroupBox);
	QVBoxLayout *signalsGroupBoxLayout=new QVBoxLayout();signalsGroupBox->setLayout(signalsGroupBoxLayout);
	signal_13_03=new QLabel(tr("  Signal from pack 1 (15) permit pincer\n  vertical movement on rollers 10x"		));signal_13_03->setMinimumWidth(250);signalsGroupBoxLayout->addWidget(signal_13_03,Qt::AlignHCenter);signal_13_03->setMinimumHeight(45);
	signal_13_04=new QLabel(tr("  Signal from pack 1 (15) permit pincer\n  vertical movement on belts 11x"			));signal_13_04->setMinimumWidth(250);signalsGroupBoxLayout->addWidget(signal_13_04,Qt::AlignHCenter);signal_13_04->setMinimumHeight(45);
	signal_13_05=new QLabel(tr("  Signal from pack 2 (13) permit pincer\n  vertical movement on rollers 20x"		));signal_13_05->setMinimumWidth(250);signalsGroupBoxLayout->addWidget(signal_13_05,Qt::AlignHCenter);signal_13_05->setMinimumHeight(45);
	signal_13_06=new QLabel(tr("  Signal from pack 2 (13) permit pincer\n  vertical movement on belts 21x"			));signal_13_06->setMinimumWidth(250);signalsGroupBoxLayout->addWidget(signal_13_06,Qt::AlignHCenter);signal_13_06->setMinimumHeight(45);
	signal_13_07=new QLabel(tr("  Signal to pack 1(15) permit\n  roller movements 10x"													));signal_13_07->setMinimumWidth(250);signalsGroupBoxLayout->addWidget(signal_13_07,Qt::AlignHCenter);signal_13_07->setMinimumHeight(45);
	signal_13_08=new QLabel(tr("  Signal to pack 1(15) permit\n  belt movements 11x"														));signal_13_08->setMinimumWidth(250);signalsGroupBoxLayout->addWidget(signal_13_08,Qt::AlignHCenter);signal_13_08->setMinimumHeight(45);
	signal_13_09=new QLabel(tr("  Signal to pack 2(13) permit\n  roller movements 20x"													));signal_13_09->setMinimumWidth(250);signalsGroupBoxLayout->addWidget(signal_13_09,Qt::AlignHCenter);signal_13_09->setMinimumHeight(45);
	signal_13_10=new QLabel(tr("  Signal to pack 2(13) permit\n  belt movements 21x"														));signal_13_10->setMinimumWidth(250);signalsGroupBoxLayout->addWidget(signal_13_10,Qt::AlignHCenter);signal_13_10->setMinimumHeight(45);

	rightLayout2->addStretch(1);

	//plc status group
	//alarms group
	QGroupBox *alarmsGroupBox=new QGroupBox(tr("Alarms"));rightLayout3->addWidget(alarmsGroupBox);
	QVBoxLayout *alarmsGroupBoxLayout=new QVBoxLayout();alarmsGroupBox->setLayout(alarmsGroupBoxLayout);

	alarmLabel_15_02=new QLabel(tr("  Overweight basket"												));alarmLabel_15_02->setMinimumWidth(175);alarmLabel_15_02->setMinimumHeight(30);alarmsGroupBoxLayout->addWidget(alarmLabel_15_02,Qt::AlignHCenter);alarmLabel_15_02->setMinimumHeight(25);
	alarmLabel_15_03=new QLabel(tr("  Axis Z overspeed"													));alarmLabel_15_03->setMinimumWidth(175);alarmLabel_15_03->setMinimumHeight(30);alarmsGroupBoxLayout->addWidget(alarmLabel_15_03,Qt::AlignHCenter);alarmLabel_15_03->setMinimumHeight(25);
	alarmLabel_15_04=new QLabel(tr("  Axis Z underspeed"												));alarmLabel_15_04->setMinimumWidth(175);alarmLabel_15_04->setMinimumHeight(30);alarmsGroupBoxLayout->addWidget(alarmLabel_15_04,Qt::AlignHCenter);alarmLabel_15_04->setMinimumHeight(25);
	alarmLabel_15_05=new QLabel(tr("  Axis X overspeed"													));alarmLabel_15_05->setMinimumWidth(175);alarmLabel_15_05->setMinimumHeight(30);alarmsGroupBoxLayout->addWidget(alarmLabel_15_05,Qt::AlignHCenter);alarmLabel_15_05->setMinimumHeight(25);
	alarmLabel_15_06=new QLabel(tr("  Axis X underspeed"												));alarmLabel_15_06->setMinimumWidth(175);alarmLabel_15_06->setMinimumHeight(30);alarmsGroupBoxLayout->addWidget(alarmLabel_15_06,Qt::AlignHCenter);alarmLabel_15_06->setMinimumHeight(25);
	alarmLabel_15_07=new QLabel(tr("  Axis Y overspeed"													));alarmLabel_15_07->setMinimumWidth(175);alarmLabel_15_07->setMinimumHeight(30);alarmsGroupBoxLayout->addWidget(alarmLabel_15_07,Qt::AlignHCenter);alarmLabel_15_07->setMinimumHeight(25);
	alarmLabel_15_08=new QLabel(tr("  Axis Y underspeed"												));alarmLabel_15_08->setMinimumWidth(175);alarmLabel_15_08->setMinimumHeight(30);alarmsGroupBoxLayout->addWidget(alarmLabel_15_08,Qt::AlignHCenter);alarmLabel_15_08->setMinimumHeight(25);
	alarmLabel_15_11=new QLabel(tr("  Motor empty shipping basket feed chain"		));alarmLabel_15_11->setMinimumWidth(175);alarmLabel_15_11->setMinimumHeight(30);alarmsGroupBoxLayout->addWidget(alarmLabel_15_11,Qt::AlignHCenter);alarmLabel_15_11->setMinimumHeight(25);
	alarmLabel_15_12=new QLabel(tr("  Motor full shipping basket feed chain"		));alarmLabel_15_12->setMinimumWidth(175);alarmLabel_15_12->setMinimumHeight(30);alarmsGroupBoxLayout->addWidget(alarmLabel_15_12,Qt::AlignHCenter);alarmLabel_15_12->setMinimumHeight(25);
	alarmLabel_15_13=new QLabel(tr("  Inverter U5"															));alarmLabel_15_13->setMinimumWidth(175);alarmLabel_15_13->setMinimumHeight(30);alarmsGroupBoxLayout->addWidget(alarmLabel_15_13,Qt::AlignHCenter);alarmLabel_15_13->setMinimumHeight(25);
	//plcStatusGroupBox
	QGroupBox *plcStatusGroupBox=new QGroupBox(tr("Plc status"));rightLayout3->addWidget(plcStatusGroupBox);
	QVBoxLayout *plcStatusGroupBoxLayout=new QVBoxLayout();plcStatusGroupBox->setLayout(plcStatusGroupBoxLayout);
	plcStatusLabel_22_00=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_00);plcStatusLabel_22_00->setMinimumWidth(250);plcStatusLabel_22_00->setMinimumHeight(35);
	plcStatusLabel_22_01=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_01);plcStatusLabel_22_01->setMinimumWidth(250);plcStatusLabel_22_01->setMinimumHeight(35);
	plcStatusLabel_22_02=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_02);plcStatusLabel_22_02->setMinimumWidth(250);plcStatusLabel_22_02->setMinimumHeight(35);
	plcStatusLabel_22_03=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_03);plcStatusLabel_22_03->setMinimumWidth(250);plcStatusLabel_22_03->setMinimumHeight(35);
	plcStatusLabel_22_04=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_04);plcStatusLabel_22_04->setMinimumWidth(250);plcStatusLabel_22_04->setMinimumHeight(35);
	plcStatusLabel_22_05=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_05);plcStatusLabel_22_05->setMinimumWidth(250);plcStatusLabel_22_05->setMinimumHeight(35);
	plcStatusLabel_22_06=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_06);plcStatusLabel_22_06->setMinimumWidth(250);plcStatusLabel_22_06->setMinimumHeight(35);
	plcStatusLabel_22_07=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_07);plcStatusLabel_22_07->setMinimumWidth(250);plcStatusLabel_22_07->setMinimumHeight(35);
	plcStatusLabel_22_08=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_08);plcStatusLabel_22_08->setMinimumWidth(250);plcStatusLabel_22_08->setMinimumHeight(35);
	plcStatusLabel_22_09=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_09);plcStatusLabel_22_09->setMinimumWidth(250);plcStatusLabel_22_09->setMinimumHeight(35);
	plcStatusLabel_22_10=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_10);plcStatusLabel_22_10->setMinimumWidth(250);plcStatusLabel_22_10->setMinimumHeight(35);
	plcStatusLabel_22_11=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_11);plcStatusLabel_22_11->setMinimumWidth(250);plcStatusLabel_22_11->setMinimumHeight(35);
	plcStatusLabel_22_12=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_12);plcStatusLabel_22_12->setMinimumWidth(250);plcStatusLabel_22_12->setMinimumHeight(35);
	plcStatusLabel_22_13=new QLabel(tr(""));plcStatusGroupBoxLayout->addWidget(plcStatusLabel_22_13);plcStatusLabel_22_13->setMinimumWidth(250);plcStatusLabel_22_13->setMinimumHeight(35);
	rightLayout3->addStretch(1);
	//
	mainSplitter->setSizes(QList<int>()<<1800<<secondSpliterPaneWidth);
	leftScrollArea->setWidget(leftScrollAreaWidget);
	rightScrollArea->setWidget(topRightWidget);
	//clearFromToPositions();
}
mainPositionClass *pincerClass::createPosition(QString title,int plcId
	,int ind0,int ind1,int ind2,int ind3,int ind4,int ind5,int ind6,int ind7,int ind8
	,int bit0,int bit1,int bit2,int bit3,int bit4,int bit5,int bit6,int bit7,int bit8
	,int com0,int com1,int com2,int com3,int com4,int com5,int com6,int com7,int com8
	,QWidget *p)
{
	QVector<int> indexVector;
	QVector<int> bitVector;
	QVector<int> commandVector;
	if ((ind0>=0)&&(bit0>=0)&&(com0>=0)){indexVector<<ind0;bitVector<<bit0;commandVector<<com0;}
	if ((ind1>=0)&&(bit1>=0)&&(com1>=0)){indexVector<<ind1;bitVector<<bit1;commandVector<<com1;}
	if ((ind2>=0)&&(bit2>=0)&&(com2>=0)){indexVector<<ind2;bitVector<<bit2;commandVector<<com2;}
	if ((ind3>=0)&&(bit3>=0)&&(com3>=0)){indexVector<<ind3;bitVector<<bit3;commandVector<<com3;}
	if ((ind4>=0)&&(bit4>=0)&&(com4>=0)){indexVector<<ind4;bitVector<<bit4;commandVector<<com4;}
	if ((ind5>=0)&&(bit5>=0)&&(com5>=0)){indexVector<<ind5;bitVector<<bit5;commandVector<<com5;}
	if ((ind6>=0)&&(bit6>=0)&&(com6>=0)){indexVector<<ind6;bitVector<<bit6;commandVector<<com6;}
	if ((ind7>=0)&&(bit7>=0)&&(com7>=0)){indexVector<<ind7;bitVector<<bit7;commandVector<<com7;}
	if ((ind8>=0)&&(bit8>=0)&&(com8>=0)){indexVector<<ind8;bitVector<<bit8;commandVector<<com8;}
	mainPositionClass *ans=new mainPositionClass(tcpServer,title,plcId,indexVector,bitVector,commandVector,p);
	connect(ans,SIGNAL(clickedSignal(positionClass*)),this,SLOT(onPositionClickedSlot(positionClass*)));
	allPositions<<ans;
	return ans;
}
void pincerClass::splitterMovedSlot(int p,int in)
{
	int w=mainSplitter->sizes()[1];
	if (secondSpliterPaneWidth!=w)
	{
		if (secondSpliterPaneWidth==0)
		{
			passwordClass *pass=new passwordClass(this);
			pass->exec();
			if (!pass->getPasswordIsCorrect())
			{
				secondSpliterPaneWidth=0;
				mainSplitter->setSizes(QList<int>()<<1800<<secondSpliterPaneWidth);
			}
			delete pass;
		}
		secondSpliterPaneWidth=w;
	}
}
void pincerClass::buttonReleasedSlot()
{
	tcpServer->sendString(QString("N22|3|0"));
}

void pincerClass::removeBasketBlinkingBtnTimerSlot()
{
	if (removeBasketBtn->text()==tr("Remove basket "))
	{
		removeBasketBtn->setText("Remove basket");
		removeBasketBtn->setPalette(QPalette(Qt::lightGray));
	}
	else
	{
		removeBasketBtn->setText("Remove basket ");
		removeBasketBtn->setPalette(QPalette(Qt::red));
	}
}
void pincerClass::moveFrom60to61PushbuttonSlot()
{
	tcpServer->sendString(QString("N22|3|26"));
}
void pincerClass::moveFrom54to55PushbuttonSlot()
{
	tcpServer->sendString(QString("N22|3|24"));
}
void pincerClass::moveFrom55to54PushbuttonSlot()
{
	if (mainPosition55->defineContents())
		tcpServer->sendString(QString("N22|3|25"));
}
void pincerClass::autoRadioButtonSlot(bool checked)
{
	tcpServer->sendString(QString("N22|3|2"));
	tcpServer->sendString(QString("N22|3|0"));
	if (plcModeEnum==AUTO_MODE)//let the plc do it
		autoRadioButton->setChecked(true);
	else if  (plcModeEnum==STEP_MODE)
		stepRadioButton->setChecked(true);
	else if  (plcModeEnum==MAINTENANCE_MODE)
		maintenanceRadioButton->setChecked(true);
}
void pincerClass::setpRadioButtonSlot(bool checked)
{
	tcpServer->sendString(QString("N22|3|3"));
	tcpServer->sendString(QString("N22|3|0"));
	if (plcModeEnum==AUTO_MODE)//let the plc do it
		autoRadioButton->setChecked(true);
	else if  (plcModeEnum==STEP_MODE)
		stepRadioButton->setChecked(true);
	else if  (plcModeEnum==MAINTENANCE_MODE)
		maintenanceRadioButton->setChecked(true);
}
void pincerClass::openToReleaseBasketSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowIcon(QIcon(":/logo_16x16.png"));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setText("You selected to open the pincers that hold the shiping basket on the air.\nMake sure that the shiping basket is safe.\nIf you proceed the basket WILL fall. ");
	msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	if (msgBox.exec()==QMessageBox::Yes)
	{
		QString passStr=QInputDialog::getText(this,tr("Password is neaded to proceed."),tr("Please input the password"),QLineEdit::Password,"");
		if(passStr=="notSafe")
		{
			tcpServer->sendString("N22|3|17");
		}
	}
}
void pincerClass::maintenanceRadioButtonSlot(bool checked)
{
	tcpServer->sendString(QString("N22|3|1"));
	tcpServer->sendString(QString("N22|3|0"));
	if (plcModeEnum==AUTO_MODE)//let the plc do it
		autoRadioButton->setChecked(true);
	else if  (plcModeEnum==STEP_MODE)
		stepRadioButton->setChecked(true);
	else if  (plcModeEnum==MAINTENANCE_MODE)
		maintenanceRadioButton->setChecked(true);
}
void pincerClass::plcStatusChanged(int index,int value)
{
	QVector<bool> bitsVector=deprecatedCalculateBits(value);
	if (index==10)
		fromPlcLabel->setText(QString().number(value));
	if (index==11)
		toPlcLabel->setText(QString().number(value));
	if (index==12)
	{
		N22_12=value;
		if (N22_12==0)fromToPlcStatus->setText(tr("Plc not in a mission"));
		if (N22_12==1)fromToPlcStatus->setText(tr("Executing mission"));
		if (N22_12==2)fromToPlcStatus->setText(tr("Mission finished"));
		if (N22_12==3)fromToPlcStatus->setText(tr("Mission Not finished"));
		if (N22_12==4)fromToPlcStatus->setText(tr("Mission Not finished, overweight basket"));
	}
	if ((index>=16)&&(index<=21))
	{
		for (int bit=0;bit<bitsVector.count();bit++)
		{
			if (bitsVector[bit]==true)
			{
				for (int j=0;j<allPositions.count();j++)
					allPositions[j]->setPlcStatusForNoEmptyFullToTrue(index,bit);
			}
		}
		if (mainPosition11->getNumberOfSBs()>0)
			mainPosition12->setDisabled(true);
		else
			mainPosition12->setDisabled(false);

		if (mainPosition12->getNumberOfSBs()>0)
			mainPosition11->setDisabled(true);
		else
			mainPosition11->setDisabled(false);
		//
		if ((mainPosition60->getNumberOfSBs()==0)||(mainPosition61->getNumberOfSBs()!=0))
			moveFrom60to61Pushbutton->setEnabled(false);
		else
			moveFrom60to61Pushbutton->setEnabled(true);
		if ( (mainPosition55->getNumberOfSBs()==0)||(mainPosition54->getNumberOfSBs()!=0))
			moveFrom55to54Pushbutton->setEnabled(false);
		else
			moveFrom55to54Pushbutton->setEnabled(true);
		if ( (mainPosition54->getNumberOfSBs()!=1)||(mainPosition55->getNumberOfSBs()!=0))
			moveFrom54to55Pushbutton->setEnabled(false);
		else
			moveFrom54to55Pushbutton->setEnabled(true);
	}
	if (index==21)
	{
		//moveFrom60to61Pushbutton
		if (bitsVector[8])
			moveFrom60to61Label->setPixmap(QPixmap(":/arrowUpSmall.png"));
		else if (bitsVector[9])
			moveFrom60to61Label->setPixmap(QPixmap(":/arrowDownSmall.png"));
		else
			moveFrom60to61Label->setPixmap(QPixmap(":/arrowNull.png"));
		//moveFrom54to55Label
		if (bitsVector[6])
			moveFrom54to55Label->setPixmap(QPixmap(":/arrowLeftC.png"));
		else if (bitsVector[7])
			moveFrom54to55Label->setPixmap(QPixmap(":/arrowRightC.png"));
		else
			moveFrom54to55Label->setPixmap(QPixmap(":/arrowNullH.png"));
		//
		if (bitsVector[5])
		{
			removeBasketBtn->setEnabled(true);
			removeBasketBlinkingBtnTimer.start(750);
		}
		else
		{
			removeBasketBtn->setText(tr("Remove basket"));
			removeBasketBtn->setStyleSheet("QPushButton{color:black,background-color:gray}");
			removeBasketBtn->setEnabled(false);
			removeBasketBlinkingBtnTimer.stop();
		}
		if (bitsVector[13])
		{
			clearFromToPositions();
		}
		//
		N22_21_10=bitsVector[10];
		if (bitsVector[10])fromLabel->setPalette(QPalette(Qt::green));else fromLabel->setPalette(QPalette(Qt::red));
		N22_21_11=bitsVector[11];
		if (bitsVector[11])toLabel->setPalette(QPalette(Qt::green));  else toLabel->setPalette(QPalette(Qt::red));
		///
		if (bitsVector[12]) stateLabel_21_12->setStyleSheet("QLabel{background-color:green}");else stateLabel_21_12->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[14]) stateLabel_21_14->setStyleSheet("QLabel{background-color:green}");else stateLabel_21_14->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[15]) stateLabel_21_15->setStyleSheet("QLabel{background-color:green}");else stateLabel_21_15->setStyleSheet("QLabel{background-color:gray}");
	}
	if (index==12)
	{
		if (value==1)
			clearBtn->setDisabled(true);
		else
		{
			if (fromPosition)
				clearBtn->setDisabled(false);
		}
	}
	if (index==22)
	{
		if (bitsVector[ 0])
		{
			plcStatusLabel_22_00->setText(tr("  1769-HSC/B BC_axisZ_axisX\n  module Ok"));
			plcStatusLabel_22_00->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_00->setText(tr("  1769-HSC/B BC_axisZ_axisX\n  module fault"));
			plcStatusLabel_22_00->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[ 1])
		{
			plcStatusLabel_22_01->setText(tr("  1769-HSC/B BC_axisY_PC_axisZ\n  module Ok"));
			plcStatusLabel_22_01->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_01->setText(tr("  1769-HSC/B BC_axisY_PC_axisZ\n  module fault"));
			plcStatusLabel_22_01->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[ 2])
		{
			plcStatusLabel_22_02->setText(tr("  1769-HSC/B PC_axisX_axisY\n  module Ok"));
			plcStatusLabel_22_02->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_02->setText(tr("  1769-HSC/B PC_axisX_axisY\n  module fault"));
			plcStatusLabel_22_02->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[ 3])
		{
			plcStatusLabel_22_03->setText(tr("  1769-IQ32T/A DIG_Input_Mod_400\n  module Ok"));
			plcStatusLabel_22_03->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_03->setText(tr("  1769-IQ32T/A DIG_Input_Mod_400\n  module fault"));
			plcStatusLabel_22_03->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[ 4])
		{
			plcStatusLabel_22_04->setText(tr("  1769-IQ32T/A DIG_Input_Mod_500\n  module Ok"));
			plcStatusLabel_22_04->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_04->setText(tr("   1769-IQ32T/A DIG_Input_Mod_500\n  module fault"));
			plcStatusLabel_22_04->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[ 5])
		{
			plcStatusLabel_22_05->setText(tr("  1769-IF4I/A ANL_Input_600\n  module Ok"));
			plcStatusLabel_22_05->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_05->setText(tr("  1769-IF4I/A ANL_Input_600\n  module fault"));
			plcStatusLabel_22_05->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[ 6])
		{
			plcStatusLabel_22_06->setText(tr("  1769-OF4/A ANL_Output_700\n  module Ok"));
			plcStatusLabel_22_06->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_06->setText(tr("  1769-OF4/A ANL_Output_700\n  module fault"));
			plcStatusLabel_22_06->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[ 7])
		{
			plcStatusLabel_22_07->setText(tr("  1769-OW16/A DIG_Output_800\n  module Ok"));
			plcStatusLabel_22_07->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_07->setText(tr("  1769-OW16/A DIG_Output_800\n  module fault"));
			plcStatusLabel_22_07->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[ 8])
		{
			plcStatusLabel_22_08->setText(tr("  1769-OW16/A DIG_Output_900\n  module Ok"));
			plcStatusLabel_22_08->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_08->setText(tr("  1769-OW16/A DIG_Output_900\n  module fault"));
			plcStatusLabel_22_08->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[ 9])
		{
			plcStatusLabel_22_09->setText(tr("  1769-OW16/A DIG_Output_1000\n  module Ok"));
			plcStatusLabel_22_09->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_09->setText(tr("  1769-OW16/A DIG_Output_1000\n  module fault"));
			plcStatusLabel_22_09->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[10])
		{
			plcStatusLabel_22_10->setText(tr("  1769-OW16/A DIG_Output_1100\n  module Ok"));
			plcStatusLabel_22_10->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_10->setText(tr("  1769-OW16/A DIG_Output_1100\n  module fault"));
			plcStatusLabel_22_10->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[11])
		{
			plcStatusLabel_22_11->setText(tr("  1769-OW16/A DIG_Output_1200\n  module Ok"));
			plcStatusLabel_22_11->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_11->setText(tr("  1769-OW16/A DIG_Output_1200\n  module fault"));
			plcStatusLabel_22_11->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (bitsVector[12])
		{
			plcStatusLabel_22_12->setText(tr("  Communication with pack lgx Ok"));
			plcStatusLabel_22_12->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_12->setText(tr("  Communication with pack lgx fault"));
			plcStatusLabel_22_12->setStyleSheet("QLabel{background-color:red}");
		}
		/////
		if (!bitsVector[13])
		{
			plcStatusLabel_22_13->setText(tr("  PLC Battery Ok"));
			plcStatusLabel_22_13->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			plcStatusLabel_22_13->setText(tr("  PLC Low Battery minor fault"));
			plcStatusLabel_22_13->setStyleSheet("QLabel{background-color:orange}");
		}
	}
	if (index==15)
	{

		if (bitsVector[0])valve_15_00->setStyleSheet("QLabel{background-color:green}");else valve_15_00->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[1])stopLabel->setPixmap(QPixmap(":/stopedBig.png"));else stopLabel->setPixmap(QPixmap(":/stopNull.png"));
		if (bitsVector[9])
		{
			moveFrom54to55Pushbutton->setIcon(QIcon(":/stopSmall.png"));
			moveFrom55to54Pushbutton->setIcon(QIcon(":/stopSmall.png"));
			moveFrom54to55Pushbutton->setIconSize(QSize(78,20));
			moveFrom55to54Pushbutton->setIconSize(QSize(78,20));
		}
		else
		{
			moveFrom54to55Pushbutton->setIcon(QIcon(":/arrowMoveRight.png"));
			moveFrom55to54Pushbutton->setIcon(QIcon(":/arrowMoveLeft.png"));
			moveFrom54to55Pushbutton->setIconSize(QSize(20,20));
			moveFrom55to54Pushbutton->setIconSize(QSize(20,20));
		}
		if (bitsVector[10])moveFrom60to61Pushbutton->setIcon(QIcon(":/deleteSmall.png"));else moveFrom60to61Pushbutton->setIcon(QIcon(":/arrowMoveUpSmall.png"));
		//alarms
		if (bitsVector[2])alarmLabel_15_02->setStyleSheet("QLabel{background-color:red}");else alarmLabel_15_02->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[3])alarmLabel_15_03->setStyleSheet("QLabel{background-color:red}");else alarmLabel_15_03->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[4])alarmLabel_15_04->setStyleSheet("QLabel{background-color:red}");else alarmLabel_15_04->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[5])alarmLabel_15_05->setStyleSheet("QLabel{background-color:red}");else alarmLabel_15_05->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[6])alarmLabel_15_06->setStyleSheet("QLabel{background-color:red}");else alarmLabel_15_06->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[7])alarmLabel_15_07->setStyleSheet("QLabel{background-color:red}");else alarmLabel_15_07->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[8])alarmLabel_15_08->setStyleSheet("QLabel{background-color:red}");else alarmLabel_15_08->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[11])alarmLabel_15_11->setStyleSheet("QLabel{background-color:red}");else alarmLabel_15_11->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[12])alarmLabel_15_12->setStyleSheet("QLabel{background-color:red}");else alarmLabel_15_12->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[13])alarmLabel_15_13->setStyleSheet("QLabel{background-color:red}");else alarmLabel_15_13->setStyleSheet("QLabel{background-color:green}");
	}
	if (index==13)
	{
		if (bitsVector[0])
		{
			okLabel->setText(tr("System Ok"));
			okLabel->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			okLabel->setText(tr("System Not Ok"));
			okLabel->setStyleSheet("QLabel{background-color:red}");
		}
		if (bitsVector[1])
		{
			alarm_13_1Label->setText(tr("Alarm of pincer crane\ninverter U3 or U4"));
			alarm_13_1Label->setStyleSheet("QLabel{background-color:red}");
		}
		else
		{
			alarm_13_1Label->setText(tr("Pincer crane\ninverter U3 and U4 Ok"));
			alarm_13_1Label->setStyleSheet("QLabel{background-color:green}");
		}
		if (bitsVector[2])
		{
			alarm_13_2Label->setText(tr("Alarm of pincer crane\nmotors(X,Y or Z axis)"));
			alarm_13_2Label->setStyleSheet("QLabel{background-color:red}");
		}
		else
		{
			alarm_13_2Label->setText(tr("Pincer crane motors\n(X,Y and Z axis) Ok"));
			alarm_13_2Label->setStyleSheet("QLabel{background-color:green}");
		}
		//
		if (bitsVector[11])resetingZLabel->setStyleSheet("QLabel{background-color:green}");else resetingZLabel->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[12])resetingXLabel->setStyleSheet("QLabel{background-color:green}");else resetingXLabel->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[13])resetingYLabel->setStyleSheet("QLabel{background-color:green}");else resetingYLabel->setStyleSheet("QLabel{background-color:gray}");
		//
		if (bitsVector[14])sensor_13_14->setStyleSheet("QLabel{background-color:green}");else sensor_13_14->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[15])sensor_13_15->setStyleSheet("QLabel{background-color:green}");else sensor_13_15->setStyleSheet("QLabel{background-color:gray}");
		//////signals
		if (bitsVector[ 3])signal_13_03->setStyleSheet("QLabel{background-color:green}");else signal_13_03->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[ 4])signal_13_04->setStyleSheet("QLabel{background-color:green}");else signal_13_04->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[ 5])signal_13_05->setStyleSheet("QLabel{background-color:green}");else signal_13_05->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[ 6])signal_13_06->setStyleSheet("QLabel{background-color:green}");else signal_13_06->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[ 7])signal_13_07->setStyleSheet("QLabel{background-color:green}");else signal_13_07->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[ 8])signal_13_08->setStyleSheet("QLabel{background-color:green}");else signal_13_08->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[ 9])signal_13_09->setStyleSheet("QLabel{background-color:green}");else signal_13_09->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[10])signal_13_10->setStyleSheet("QLabel{background-color:green}");else signal_13_10->setStyleSheet("QLabel{background-color:gray}");

	}
	if (index==14)
	{
		for (int j=0;j<allPositions.count();j++)
			allPositions[j]->setInMaintenanceMode(bitsVector[7]);
		if (bitsVector[0])sensor_14_00->setStyleSheet("QLabel{background-color:green}");else sensor_14_00->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[1])sensor_14_01->setStyleSheet("QLabel{background-color:green}");else sensor_14_01->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[2])sensor_14_02->setStyleSheet("QLabel{background-color:green}");else sensor_14_02->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[3])sensor_14_03->setStyleSheet("QLabel{background-color:green}");else sensor_14_03->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[4])sensor_14_04->setStyleSheet("QLabel{background-color:green}");else sensor_14_04->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[5])sensor_14_05->setStyleSheet("QLabel{background-color:green}");else sensor_14_05->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[6])sensor_14_06->setStyleSheet("QLabel{background-color:green}");else sensor_14_06->setStyleSheet("QLabel{background-color:gray}");
		////

		command10PushButton->setEnabled(false);
		command11PushButton->setEnabled(false);
		command12PushButton->setEnabled(false);
		command13PushButton->setEnabled(false);
		command14PushButton->setEnabled(false);
		command15PushButton->setEnabled(false);
		command16PushButton->setEnabled(false);
		command17PushButton->setEnabled(false);
		command18PushButton->setEnabled(false);
		command19PushButton->setEnabled(false);
		command20PushButton->setEnabled(false);
		command21PushButton->setEnabled(false);
		command22PushButton->setEnabled(false);

		plcModeEnum=NO_MODE;
		if (bitsVector[7]==true)
		{
			plcModeEnum=MAINTENANCE_MODE;
			maintenanceRadioButton->blockSignals(true);
			maintenanceRadioButton->setChecked(true);
			maintenanceRadioButton->blockSignals(false);
			command10PushButton->setEnabled(true);
			command11PushButton->setEnabled(true);
			command12PushButton->setEnabled(true);
			command13PushButton->setEnabled(true);
			command14PushButton->setEnabled(true);
			command15PushButton->setEnabled(true);
			command16PushButton->setEnabled(true);
			command17PushButton->setEnabled(true);
			command18PushButton->setEnabled(true);
			command19PushButton->setEnabled(true);
			command20PushButton->setEnabled(true);
			command21PushButton->setEnabled(true);
			command22PushButton->setEnabled(true);
		}
		N22_14_08=bitsVector[8];
		if (bitsVector[8]==true)
		{
			plcModeEnum=AUTO_MODE;
			autoRadioButton->blockSignals(true);
			autoRadioButton->setChecked(true);
			autoRadioButton->blockSignals(false);
		}
		N22_14_09=bitsVector[9];
		makeStepBtn->setEnabled(false);
		if (bitsVector[9]==true)
		{
			plcModeEnum=STEP_MODE;
			stepRadioButton->blockSignals(true);
			stepRadioButton->setChecked(true);
			stepRadioButton->blockSignals(false);
			makeStepBtn->setEnabled(true);
		}
		N22_14_10=bitsVector[10];
		if (bitsVector[10])stateLabel_14_10->setStyleSheet("QLabel{background-color:green}");else stateLabel_14_10->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[11])stateLabel_14_11->setStyleSheet("QLabel{background-color:green}");else stateLabel_14_11->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[12])stateLabel_14_12->setStyleSheet("QLabel{background-color:green}");else stateLabel_14_12->setStyleSheet("QLabel{background-color:gray}");

		if (bitsVector[13])valve_14_13->setStyleSheet("QLabel{background-color:green}");else valve_14_13->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[14])valve_14_14->setStyleSheet("QLabel{background-color:green}");else valve_14_14->setStyleSheet("QLabel{background-color:gray}");
		if (bitsVector[15])valve_14_15->setStyleSheet("QLabel{background-color:green}");else valve_14_15->setStyleSheet("QLabel{background-color:gray}");
	}
	if (index==15)
	{
		if (bitsVector[15])
		{
			makeResetLabel->setText(tr("Reset all axis"));
			makeResetLabel->setStyleSheet("QLabel{background-color:red}");
		}
		else
		{
			makeResetLabel->setText(tr("No reset needed"));
			makeResetLabel->setStyleSheet("QLabel{background-color:green}");
		}
	}
	//
	executeBtn->setEnabled(false);
	if ((N22_12==0)&&(N22_14_08||N22_14_09)&&(!N22_14_10)&&N22_21_10&&N22_21_11)
		executeBtn->setEnabled(true);
}
void pincerClass::plcFloatValuesChanged(int index,float value)
{
	if (index==0)loadTargetZLabel->setText(QString("Load target Z :%1mm").arg(value,8,'f',2));
	if (index==1)loadTargetXLabel->setText(QString("Load target X :%1mm").arg(value,8,'f',2));
	if (index==2)loadTargetYLabel->setText(QString("Load target Y :%1mm").arg(value,8,'f',2));

	if (index==3)unloadTargetZLabel->setText(QString("Unload target Z :%1mm").arg(value,8,'f',2));
	if (index==4)unloadTargetXLabel->setText(QString("Unload target X :%1mm").arg(value,8,'f',2));
	if (index==5)unloadTargetYLabel->setText(QString("Unload target Y :%1mm").arg(value,8,'f',2));

	if (index==6)stepTargetZLabel->setText(QString("Current step target Z :%1mm)").arg(value,8,'f',2));
	if (index==7)stepTargetXLabel->setText(QString("Current step target X :%1mm)").arg(value,8,'f',2));
	if (index==8)stepTargetYLabel->setText(QString("Current step target Y :%1mm)").arg(value,8,'f',2));

	if (index== 9)realPositionZLabel->setText(QString("Z Position :%1mm").arg(value,8,'f',2));
	if (index==10)realPositionXLabel->setText(QString("X Position :%1mm").arg(value,8,'f',2));
	if (index==11)realPositionYLabel->setText(QString("Y Position :%1mm").arg(value,8,'f',2));

	if (index==12)setSpeedZLabel->setText(QString("Z Set speed :%1m/min").arg(value,8,'f',2));
	if (index==13)setSpeedXLabel->setText(QString("X Set speed :%1m/min").arg(value,8,'f',2));
	if (index==14)setSpeedYLabel->setText(QString("Y Set speed :%1m/min").arg(value,8,'f',2));

	if (index==15)realSpeedZLabel->setText(QString("Z real speed :%1m/m").arg(value,8,'f',2));
	if (index==16)realSpeedXLabel->setText(QString("X real speed :%1m/m").arg(value,8,'f',2));
	if (index==17)realSpeedYLabel->setText(QString("Y real speed :%1m/m").arg(value,8,'f',2));
}
void pincerClass::onExecuteBtnClickedSlot()
{
	tcpServer->sendString(QString("N22|3|27"));
}
void pincerClass::clearFromToPositions()
{
	fromPosition=toPosition=NULL;
	fromLabel->setText("...");
	toLabel->setText("...");
	tcpServer->sendString(QString("N22|0|0"));
	tcpServer->sendString(QString("N22|1|0"));
	tcpServer->sendString(QString("N22|10|0"));
	tcpServer->sendString(QString("N22|11|0"));
	//executeBtn->setEnabled(false);
	//clearBtn->setEnabled(false);
	for (int i=0;i<allPositions.count();i++)
		allPositions[i]->setSelectedFromPosition(NULL);
}
void pincerClass::onClearBtnClickedSlot()
{
	clearFromToPositions();
}
void pincerClass::onPositionClickedSlot(positionClass* pos)
{
	if (!fromPosition)
	{
		fromPosition=pos;
		fromLabel->setText(QString().number(fromPosition->getPositionId()));
		clearBtn->setEnabled(true);
		for (int i=0;i<allPositions.count();i++)
			allPositions[i]->setSelectedFromPosition(fromPosition);
		tcpServer->sendString(QString("N22|0|%1").arg(fromPosition->getPositionId()));
	}
	else
	{
		toPosition=pos;
		toLabel->setText(QString().number(toPosition->getPositionId()));
		tcpServer->sendString(QString("N22|1|%1").arg(toPosition->getPositionId()));
		//clearBtn->setEnabled(true);
		//executeBtn->setEnabled(true);
	}
}

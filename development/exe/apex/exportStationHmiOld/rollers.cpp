#include "rollers.h"
rollersClass::rollersClass(tcpServerClass *tcpServer_,QWidget *p):QWidget(p)
{
	//this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px}  QCheckBox{font-size:%1px}").arg(12));
	tcpServer=tcpServer_;
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);	
	QSplitter *mainSplitter=new QSplitter(Qt::Vertical,this);mainLayout->addWidget(mainSplitter);
	/////////////////////////////////////////////////////////////////////////area
	QImage image(":/rollersLayout.png");
	QLabel *imageLabel = new QLabel;
	imageLabel->setMinimumWidth(1750);
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
	autoPushbutton=new myQPushButton(tcpServer,"","N100|30|0",tr(""),imageLabel);
	autoPushbutton->setGeometry(0,460,100,30);
	stopLabel_0_3=new QLabel("",imageLabel);
	stopLabel_0_3->setGeometry(105,460,100,30);
	myQPushButton *resetAlarmsPushButton=new myQPushButton(tcpServer,"N100|30|639","N100|30|0",tr("Reset Alarms"),imageLabel);
	resetAlarmsPushButton->setGeometry(210,460,100,30);
	
	int X,Y;
	X=1085;Y=45;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_UP   ,"N100|30|603","N100|30|0",tr("Roller way 5 up"          ), X+54,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_DOWN ,"N100|30|604","N100|30|0",tr("Roller way 5 down"           ), X   ,Y+10,imageLabel);

	X=73;Y=345;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT ,"N100|30|605","N100|30|0",tr("Roller way 1 forward"          ), X+54,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT  ,"N100|30|606","N100|30|0",tr("Roller way 1 backward"           ), X   ,Y+10,imageLabel);

	X=580;Y=270;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT ,"N100|30|607","N100|30|0",tr("Roller way 2 forward"          ), X+54,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT  ,"N100|30|608","N100|30|0",tr("Roller way 2 backward"           ), X   ,Y+10,imageLabel);

	X=580;Y=350;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT ,"N100|30|609","N100|30|0",tr("Roller way 3 forward"          ), X+54,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT  ,"N100|30|610","N100|30|0",tr("Roller way 3 backward"           ), X   ,Y+10,imageLabel);

	X=925;Y=45;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT ,"N100|30|611","N100|30|0",tr("Roller way 5 forward"          ), X+54,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT  ,"N100|30|612","N100|30|0",tr("Roller way 5 backward"           ), X   ,Y+10,imageLabel);

	X=1350;Y=45;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT ,"N100|30|642","N100|30|0",tr("Roller way 6 forward"          ), X+54,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT  ,"N100|30|641","N100|30|0",tr("Roller way 6 backward"           ), X   ,Y+10,imageLabel);

	X=1085;Y=200;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_UP   ,"N100|30|613","N100|30|0",tr("Roller way 5 forward"          ), X+54,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_DOWN ,"N100|30|614","N100|30|0",tr("Roller way 5 backward"           ), X   ,Y+10,imageLabel);
	X=460;Y=360;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT ,"N100|30|649","N100|30|0",tr("R1+R3 right"     ), X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT  ,"N100|30|650","N100|30|0",tr("R1+R3 left "     ), X   ,Y,imageLabel);
	X=910;Y=360;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT ,"N100|30|651","N100|30|0",tr("R3+R5 right"        ), X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT  ,"N100|30|652","N100|30|0",tr("R3+R5 left "       ), X   ,Y,imageLabel);
	X=910;Y=270;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT ,"N100|30|653","N100|30|0",tr("R2+R5 right"        ), X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT  ,"N100|30|654","N100|30|0",tr("R2+R5 left "       ), X   ,Y,imageLabel);
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
	QGroupBox *activeCommantsGroupBox=new QGroupBox(tr("Active commands"));btnsMainLayout0->addWidget(activeCommantsGroupBox);
	QVBoxLayout *activeCommantsGroupBoxLayout=new QVBoxLayout();activeCommantsGroupBox->setLayout(activeCommantsGroupBoxLayout);
	activeCommantsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N100|30|648","N100|30|0",tr("Reset"),NULL));
	signal_35_00=new QLabel(tr("R1-R3"							));signal_35_00->setMargin(4);	activeCommantsGroupBoxLayout->addWidget(signal_35_00,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_01=new QLabel(tr("R3-R5"							));signal_35_01->setMargin(4);	activeCommantsGroupBoxLayout->addWidget(signal_35_01,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_02=new QLabel(tr("R2-R5"							));signal_35_02->setMargin(4);	activeCommantsGroupBoxLayout->addWidget(signal_35_02,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_03=new QLabel(tr("R5-R6"							));signal_35_03->setMargin(4);	activeCommantsGroupBoxLayout->addWidget(signal_35_03,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_04=new QLabel(tr("R5 side to R3"			));signal_35_04->setMargin(4);	activeCommantsGroupBoxLayout->addWidget(signal_35_04,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_05=new QLabel(tr("R5 side to R6"			));signal_35_05->setMargin(4);	activeCommantsGroupBoxLayout->addWidget(signal_35_05,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_06=new QLabel(tr("R5 side to R2 left"	));signal_35_06->setMargin(4);	activeCommantsGroupBoxLayout->addWidget(signal_35_06,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_07=new QLabel(tr("R5 side to R2 righ"	));signal_35_07->setMargin(4);	activeCommantsGroupBoxLayout->addWidget(signal_35_07,Qt::AlignHCenter|Qt::AlignCenter);
	btnsMainLayout0->addStretch(1);
	//btnsMainLayout1
	QGroupBox *sideMovingSignalsGroupBox=new QGroupBox(tr("Side moving aligment"));btnsMainLayout1->addWidget(sideMovingSignalsGroupBox);
	QVBoxLayout *sideMovingSignalsGroupBoxLayout=new QVBoxLayout();sideMovingSignalsGroupBox->setLayout(sideMovingSignalsGroupBoxLayout);
	signal_35_08=new QLabel(tr("R1-R3."						));signal_35_08->setMargin(2);		sideMovingSignalsGroupBoxLayout->addWidget(signal_35_08,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_09=new QLabel(tr("R3-R5."						));signal_35_09->setMargin(2);		sideMovingSignalsGroupBoxLayout->addWidget(signal_35_09,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_10=new QLabel(tr("R2-R5."						));signal_35_10->setMargin(2);		sideMovingSignalsGroupBoxLayout->addWidget(signal_35_10,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_11=new QLabel(tr("R5-R6."						));signal_35_11->setMargin(2);		sideMovingSignalsGroupBoxLayout->addWidget(signal_35_11,Qt::AlignHCenter|Qt::AlignCenter);
	QGroupBox *sideMovingPositionSignalsGroupBox=new QGroupBox(tr("Side moving position"));btnsMainLayout1->addWidget(sideMovingPositionSignalsGroupBox);
	QVBoxLayout *sideMovingPositionSignalsGroupBoxLayout=new QVBoxLayout();sideMovingPositionSignalsGroupBox->setLayout(sideMovingPositionSignalsGroupBoxLayout);
	signal_35_14=new QLabel(tr("On R6."						));signal_35_14->setMargin(2);		sideMovingPositionSignalsGroupBoxLayout->addWidget(signal_35_14,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_13=new QLabel(tr("On R2."						));signal_35_13->setMargin(2);		sideMovingPositionSignalsGroupBoxLayout->addWidget(signal_35_13,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_12=new QLabel(tr("On R3."						));signal_35_12->setMargin(2);		sideMovingPositionSignalsGroupBoxLayout->addWidget(signal_35_12,Qt::AlignHCenter|Qt::AlignCenter);
	btnsMainLayout1->addStretch(1);
	//btnsMainLayout2
	QGroupBox *sideRollersGroupBox=new QGroupBox(tr("Side rollers"));btnsMainLayout2->addWidget(sideRollersGroupBox);
	QGridLayout *sideRollersGroupBoxLayout=new QGridLayout();sideRollersGroupBox->setLayout(sideRollersGroupBoxLayout);
	
	myQPushButton *r3LabelOpenPushButton=new myQPushButton(tcpServer,"N100|30|635","N100|30|0",tr("Open"),NULL);r3LabelOpenPushButton->setMinimumSize(QSize(75,28));sideRollersGroupBoxLayout->addWidget(r3LabelOpenPushButton,0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	QLabel *r3Label=new QLabel(tr("R3"));r3Label->setMargin(5);	sideRollersGroupBoxLayout->addWidget(r3Label,0,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *r3LabelClosePushButton=new myQPushButton(tcpServer,"N100|30|636","N100|30|0",tr("Close"),NULL);r3LabelClosePushButton->setMinimumSize(QSize(75,28));sideRollersGroupBoxLayout->addWidget(r3LabelClosePushButton,0,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);

	myQPushButton *r5LabelOpenPushButton=new myQPushButton(tcpServer,"N100|30|637","N100|30|0",tr("Open"),NULL);r3LabelOpenPushButton->setMinimumSize(QSize(75,28));sideRollersGroupBoxLayout->addWidget(r5LabelOpenPushButton,1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	QLabel *r5Label=new QLabel(tr("R5"));r3Label->setMargin(5);	sideRollersGroupBoxLayout->addWidget(r5Label,1,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *r5LabelClosePushButton=new myQPushButton(tcpServer,"N100|30|638","N100|30|0",tr("Close"),NULL);r5LabelClosePushButton->setMinimumSize(QSize(75,28));sideRollersGroupBoxLayout->addWidget(r5LabelClosePushButton,1,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	
	myQPushButton *r6LabelOpenPushButton=new myQPushButton(tcpServer,"N100|30|645","N100|30|0",tr("Open"),NULL);r6LabelOpenPushButton->setMinimumSize(QSize(75,28));sideRollersGroupBoxLayout->addWidget(r6LabelOpenPushButton,2,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	QLabel *r6Label=new QLabel(tr("R6"));r6Label->setMargin(5);	sideRollersGroupBoxLayout->addWidget(r6Label,2,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *r6LabelClosePushButton=new myQPushButton(tcpServer,"N100|30|646","N100|30|0",tr("Close"),NULL);r6LabelClosePushButton->setMinimumSize(QSize(75,28));sideRollersGroupBoxLayout->addWidget(r6LabelClosePushButton,2,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	btnsMainLayout2->addStretch(1);
	//btnsMainLayout3
	//packetsGroupBox
	QGroupBox *packetsGroupBox=new QGroupBox(tr("Packets"));btnsMainLayout3->addWidget(packetsGroupBox);
	QGridLayout *packetsGroupBoxLayout=new QGridLayout();packetsGroupBox->setLayout(packetsGroupBoxLayout);
	int r=0;
	signal_35_16=new QLabel(tr("On R1"));signal_35_16->setMargin(5);	packetsGroupBoxLayout->addWidget(signal_35_16,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_17=new QLabel(tr("On R2"));signal_35_17->setMargin(5);	packetsGroupBoxLayout->addWidget(signal_35_17,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_18=new QLabel(tr("On R3"));signal_35_18->setMargin(5);	packetsGroupBoxLayout->addWidget(signal_35_18,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_19=new QLabel(tr("On R5"));signal_35_19->setMargin(5);	packetsGroupBoxLayout->addWidget(signal_35_19,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_20=new QLabel(tr("On R6"));signal_35_20->setMargin(5);	packetsGroupBoxLayout->addWidget(signal_35_20,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	r=0;
	myQPushButton *signal_35_16_onPushButton=new myQPushButton(tcpServer,"N100|30|615","N100|30|0",tr("On"),NULL);signal_35_16_onPushButton->setMinimumSize(QSize(75,28));packetsGroupBoxLayout->addWidget(signal_35_16_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_17_onPushButton=new myQPushButton(tcpServer,"N100|30|617","N100|30|0",tr("On"),NULL);signal_35_17_onPushButton->setMinimumSize(QSize(75,28));packetsGroupBoxLayout->addWidget(signal_35_17_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_18_onPushButton=new myQPushButton(tcpServer,"N100|30|619","N100|30|0",tr("On"),NULL);signal_35_18_onPushButton->setMinimumSize(QSize(75,28));packetsGroupBoxLayout->addWidget(signal_35_18_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_19_onPushButton=new myQPushButton(tcpServer,"N100|30|621","N100|30|0",tr("On"),NULL);signal_35_19_onPushButton->setMinimumSize(QSize(75,28));packetsGroupBoxLayout->addWidget(signal_35_19_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_20_onPushButton=new myQPushButton(tcpServer,"N100|30|623","N100|30|0",tr("On"),NULL);signal_35_20_onPushButton->setMinimumSize(QSize(75,28));packetsGroupBoxLayout->addWidget(signal_35_20_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	r=0;
	myQPushButton *signal_35_16_offPushButton=new myQPushButton(tcpServer,"N100|30|616","N100|30|0",tr("Off"),NULL);signal_35_16_offPushButton->setMinimumSize(QSize(75,28));packetsGroupBoxLayout->addWidget(signal_35_16_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_17_offPushButton=new myQPushButton(tcpServer,"N100|30|618","N100|30|0",tr("Off"),NULL);signal_35_17_offPushButton->setMinimumSize(QSize(75,28));packetsGroupBoxLayout->addWidget(signal_35_17_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_18_offPushButton=new myQPushButton(tcpServer,"N100|30|620","N100|30|0",tr("Off"),NULL);signal_35_18_offPushButton->setMinimumSize(QSize(75,28));packetsGroupBoxLayout->addWidget(signal_35_18_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_19_offPushButton=new myQPushButton(tcpServer,"N100|30|622","N100|30|0",tr("Off"),NULL);signal_35_19_offPushButton->setMinimumSize(QSize(75,28));packetsGroupBoxLayout->addWidget(signal_35_19_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_20_offPushButton=new myQPushButton(tcpServer,"N100|30|624","N100|30|0",tr("Off"),NULL);signal_35_20_offPushButton->setMinimumSize(QSize(75,28));packetsGroupBoxLayout->addWidget(signal_35_20_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	//SBGroupBox
	QGroupBox *SBGroupBox=new QGroupBox(tr("Shiping baskets"));btnsMainLayout3->addWidget(SBGroupBox);
	QGridLayout *SBGroupBoxLayout=new QGridLayout();SBGroupBox->setLayout(SBGroupBoxLayout);
	r=0;
	signal_35_21=new QLabel(tr("On R1"));signal_35_21->setMargin(2);	SBGroupBoxLayout->addWidget(signal_35_21,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_22=new QLabel(tr("On R2"));signal_35_22->setMargin(2);	SBGroupBoxLayout->addWidget(signal_35_22,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_23=new QLabel(tr("On R3"));signal_35_23->setMargin(2);	SBGroupBoxLayout->addWidget(signal_35_23,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_24=new QLabel(tr("On R5"));signal_35_24->setMargin(2);	SBGroupBoxLayout->addWidget(signal_35_24,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_35_25=new QLabel(tr("On R6"));signal_35_25->setMargin(2);	SBGroupBoxLayout->addWidget(signal_35_25,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	r=0;
	myQPushButton *signal_35_21_onPushButton=new myQPushButton(tcpServer,"N100|30|625","N100|30|0",tr("On"),NULL);signal_35_21_onPushButton->setMinimumSize(QSize(75,28));SBGroupBoxLayout->addWidget(signal_35_21_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_22_onPushButton=new myQPushButton(tcpServer,"N100|30|627","N100|30|0",tr("On"),NULL);signal_35_22_onPushButton->setMinimumSize(QSize(75,28));SBGroupBoxLayout->addWidget(signal_35_22_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_23_onPushButton=new myQPushButton(tcpServer,"N100|30|629","N100|30|0",tr("On"),NULL);signal_35_23_onPushButton->setMinimumSize(QSize(75,28));SBGroupBoxLayout->addWidget(signal_35_23_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_24_onPushButton=new myQPushButton(tcpServer,"N100|30|631","N100|30|0",tr("On"),NULL);signal_35_24_onPushButton->setMinimumSize(QSize(75,28));SBGroupBoxLayout->addWidget(signal_35_24_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_25_onPushButton=new myQPushButton(tcpServer,"N100|30|633","N100|30|0",tr("On"),NULL);signal_35_25_onPushButton->setMinimumSize(QSize(75,28));SBGroupBoxLayout->addWidget(signal_35_25_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	r=0;
	myQPushButton *signal_35_21_offPushButton=new myQPushButton(tcpServer,"N100|30|626","N100|30|0",tr("Off"),NULL);signal_35_21_offPushButton->setMinimumSize(QSize(75,28));SBGroupBoxLayout->addWidget(signal_35_21_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_22_offPushButton=new myQPushButton(tcpServer,"N100|30|628","N100|30|0",tr("Off"),NULL);signal_35_22_offPushButton->setMinimumSize(QSize(75,28));SBGroupBoxLayout->addWidget(signal_35_22_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_23_offPushButton=new myQPushButton(tcpServer,"N100|30|630","N100|30|0",tr("Off"),NULL);signal_35_23_offPushButton->setMinimumSize(QSize(75,28));SBGroupBoxLayout->addWidget(signal_35_23_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_24_offPushButton=new myQPushButton(tcpServer,"N100|30|632","N100|30|0",tr("Off"),NULL);signal_35_24_offPushButton->setMinimumSize(QSize(75,28));SBGroupBoxLayout->addWidget(signal_35_24_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_35_25_offPushButton=new myQPushButton(tcpServer,"N100|30|634","N100|30|0",tr("Off"),NULL);signal_35_25_offPushButton->setMinimumSize(QSize(75,28));SBGroupBoxLayout->addWidget(signal_35_25_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	btnsMainLayout3->addStretch(1);
	//btnsMainLayout4
	btnsMainLayout4->addStretch(1);
	//btnsMainLayout5
	QGroupBox *warningSignalsGroupBox=new QGroupBox(tr("Warnings"));btnsMainLayout5->addWidget(warningSignalsGroupBox);
	QVBoxLayout *warningSignalsGroupBoxLayout=new QVBoxLayout();warningSignalsGroupBox->setLayout(warningSignalsGroupBoxLayout);
	signal_37_00=new QLabel(tr("Roller conveyor R1 not down."						));signal_37_00->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_00,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_01=new QLabel(tr("Side rollers conveyors R1-R3 not ok"		));signal_37_01->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_01,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_02=new QLabel(tr("Roller conveyor R5 not down"						));signal_37_02->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_02,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_03=new QLabel(tr("Side rollers conveyors R3-R5 not ok"		));signal_37_03->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_03,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_04=new QLabel(tr("R5 not on R3 position."									));signal_37_04->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_04,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_05=new QLabel(tr("Roller conveyor R5 not down."						));signal_37_05->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_05,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_06=new QLabel(tr("Side rollers conveyors R2-R5 not ok."		));signal_37_06->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_06,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_07=new QLabel(tr("R5 not on R2 position."									));signal_37_07->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_07,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_08=new QLabel(tr("Roller conveyor R5 not up."							));signal_37_08->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_08,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_09=new QLabel(tr("Side rollers conveyors R6-R5 not ok."		));signal_37_09->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_09,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_10=new QLabel(tr("R5 not on R6 position."									));signal_37_10->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_10,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_11=new QLabel(tr("Roller conveyor R5 not down."						));signal_37_11->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_11,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_12=new QLabel(tr("Roller conveyor R5 not down."						));signal_37_12->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_12,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_13=new QLabel(tr("Roller conveyor R5 not down."						));signal_37_13->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_13,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_14=new QLabel(tr("Roller conveyor R5 not down."						));signal_37_14->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_14,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_15=new QLabel(tr("R5 height measurer not up."							));signal_37_15->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_15,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_16=new QLabel(tr("R5 height measurer not up."							));signal_37_16->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_16,Qt::AlignHCenter|Qt::AlignCenter);
	signal_37_17=new QLabel(tr("R5 height measurer not up."							));signal_37_17->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_37_17,Qt::AlignHCenter|Qt::AlignCenter);
	warningSignalsGroupBox->setLayout(warningSignalsGroupBoxLayout);
	btnsMainLayout5->addStretch(1);
	//btnsMainLayout6
	QGroupBox *alarmsSignalsGroupBox=new QGroupBox(tr("Alarms"));btnsMainLayout6->addWidget(alarmsSignalsGroupBox);
	QVBoxLayout *alarmsSignalsGroupBoxLayout=new QVBoxLayout();
	signal_38_00=new QLabel(tr("Thermal overload\nQ5,Q51,Q52,Q53,Q54,Q55,Q69,U5." ));signal_38_00->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_38_00,Qt::AlignHCenter|Qt::AlignCenter);
	signal_38_01=new QLabel(tr("General stop cycle."															));signal_38_01->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_38_01,Qt::AlignHCenter|Qt::AlignCenter);
	signal_38_02=new QLabel(tr("Push button stop cycle."													));signal_38_02->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_38_02,Qt::AlignHCenter|Qt::AlignCenter);
	signal_38_03=new QLabel(tr("Thermal overload\nQ16,Q17,U16,U17."								));signal_38_03->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_38_03,Qt::AlignHCenter|Qt::AlignCenter);
	signal_38_04=new QLabel(tr("Thermal overload\nQ6,Q7,Q8,Q9,Q10,Q20.."					));signal_38_04->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_38_04,Qt::AlignHCenter|Qt::AlignCenter);
	alarmsSignalsGroupBox->setLayout(alarmsSignalsGroupBoxLayout);
	btnsMainLayout6->addStretch(1);
	//automated packet warehouse
	//QWidget *btnsAreaWidget=new QWidget();
	//QHBoxLayout *btnsAreaWidgetLayout=new QHBoxLayout();
	//btnsAreaWidgetLayout->setContentsMargins(0,0,0,0);
	//btnsAreaWidget->setLayout(btnsAreaWidgetLayout);
	QScrollArea *btnsArea=new QScrollArea();
	//btnsArea->setAlignment(Qt::AlignTop);
	//btnsAreaWidgetLayout->addWidget(btnsArea);
	btnsArea->setWidget(btnsWidget);
	mainSplitter->addWidget(btnsArea);
	///////////////////////////////////////////////////////////////////////////faults
	QList<int> sizes;sizes<<525<<475;
	mainSplitter->setSizes(sizes);
}
void rollersClass::plcMemoryChanged(int index,int value)
{
	QVector<bool> bitsVector=deprecatedCalculateBits(value);
	if (index==35)
	{
		if (bitsVector[26])
		{
			autoPushbutton->setText("Auto");
			autoPushbutton->setSendCommandToServerString("N100|30|602");
			hideButtons();
		}
		else
		{
			autoPushbutton->setText("Manual");
			autoPushbutton->setSendCommandToServerString("N100|30|601");
			showButtons();
		}
		if (bitsVector[15])
		{
			stopLabel_0_3->setText("  Active stop");
			stopLabel_0_3->setStyleSheet("QLabel{text-align:center;color:white; border: 4px solid red;border-radius: 4px;}");
		}
		else
		{
			stopLabel_0_3->setText("  No active stop");
			stopLabel_0_3->setStyleSheet("QLabel{text-align:center;color:gray}");
		}
		if (bitsVector[ 0])signal_35_00->setStyleSheet("QLabel{background-color:green}");else signal_35_00->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 1])signal_35_01->setStyleSheet("QLabel{background-color:green}");else signal_35_01->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 2])signal_35_02->setStyleSheet("QLabel{background-color:green}");else signal_35_02->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 3])signal_35_03->setStyleSheet("QLabel{background-color:green}");else signal_35_03->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 4])signal_35_04->setStyleSheet("QLabel{background-color:green}");else signal_35_04->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 5])signal_35_05->setStyleSheet("QLabel{background-color:green}");else signal_35_05->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 6])signal_35_06->setStyleSheet("QLabel{background-color:green}");else signal_35_06->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 7])signal_35_07->setStyleSheet("QLabel{background-color:green}");else signal_35_07->setStyleSheet("QLabel{background-color:lightgray}");

		if (bitsVector[ 8])signal_35_08->setStyleSheet("QLabel{background-color:green}");else signal_35_08->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 9])signal_35_09->setStyleSheet("QLabel{background-color:green}");else signal_35_09->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[10])signal_35_10->setStyleSheet("QLabel{background-color:green}");else signal_35_10->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[11])signal_35_11->setStyleSheet("QLabel{background-color:green}");else signal_35_11->setStyleSheet("QLabel{background-color:lightgray}");

		if (bitsVector[14])signal_35_14->setStyleSheet("QLabel{background-color:green}");else signal_35_14->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[13])signal_35_13->setStyleSheet("QLabel{background-color:green}");else signal_35_13->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[12])signal_35_12->setStyleSheet("QLabel{background-color:green}");else signal_35_12->setStyleSheet("QLabel{background-color:lightgray}");

		if (bitsVector[16])signal_35_16->setStyleSheet("QLabel{background-color:green}");else signal_35_16->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[17])signal_35_17->setStyleSheet("QLabel{background-color:green}");else signal_35_17->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[18])signal_35_18->setStyleSheet("QLabel{background-color:green}");else signal_35_18->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[19])signal_35_19->setStyleSheet("QLabel{background-color:green}");else signal_35_19->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[20])signal_35_20->setStyleSheet("QLabel{background-color:green}");else signal_35_20->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[21])signal_35_21->setStyleSheet("QLabel{background-color:green}");else signal_35_21->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[22])signal_35_22->setStyleSheet("QLabel{background-color:green}");else signal_35_22->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[23])signal_35_23->setStyleSheet("QLabel{background-color:green}");else signal_35_23->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[24])signal_35_24->setStyleSheet("QLabel{background-color:green}");else signal_35_24->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[25])signal_35_25->setStyleSheet("QLabel{background-color:green}");else signal_35_25->setStyleSheet("QLabel{background-color:lightgray}");
	
	}
	else if (index==37)
	{
		if (bitsVector[ 0])signal_37_00->setStyleSheet("QLabel{background-color:orange}");else signal_37_00->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 1])signal_37_01->setStyleSheet("QLabel{background-color:orange}");else signal_37_01->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 2])signal_37_02->setStyleSheet("QLabel{background-color:orange}");else signal_37_02->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 3])signal_37_03->setStyleSheet("QLabel{background-color:orange}");else signal_37_03->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 4])signal_37_04->setStyleSheet("QLabel{background-color:orange}");else signal_37_04->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 5])signal_37_05->setStyleSheet("QLabel{background-color:orange}");else signal_37_05->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 6])signal_37_06->setStyleSheet("QLabel{background-color:orange}");else signal_37_06->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 7])signal_37_07->setStyleSheet("QLabel{background-color:orange}");else signal_37_07->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 8])signal_37_08->setStyleSheet("QLabel{background-color:orange}");else signal_37_08->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 9])signal_37_09->setStyleSheet("QLabel{background-color:orange}");else signal_37_09->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[10])signal_37_10->setStyleSheet("QLabel{background-color:orange}");else signal_37_10->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[11])signal_37_11->setStyleSheet("QLabel{background-color:orange}");else signal_37_11->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[12])signal_37_12->setStyleSheet("QLabel{background-color:orange}");else signal_37_12->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[13])signal_37_13->setStyleSheet("QLabel{background-color:orange}");else signal_37_13->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[14])signal_37_14->setStyleSheet("QLabel{background-color:orange}");else signal_37_14->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[15])signal_37_15->setStyleSheet("QLabel{background-color:orange}");else signal_37_15->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[16])signal_37_16->setStyleSheet("QLabel{background-color:orange}");else signal_37_16->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[17])signal_37_17->setStyleSheet("QLabel{background-color:orange}");else signal_37_17->setStyleSheet("QLabel{background-color:lightgray}");
	}
	else if (index==38)
	{
		if (bitsVector[ 0])signal_38_00->setStyleSheet("QLabel{background-color:red}");else signal_38_00->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 1])signal_38_01->setStyleSheet("QLabel{background-color:red}");else signal_38_01->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 2])signal_38_02->setStyleSheet("QLabel{background-color:red}");else signal_38_02->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 3])signal_38_03->setStyleSheet("QLabel{background-color:red}");else signal_38_03->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 4])signal_38_04->setStyleSheet("QLabel{background-color:red}");else signal_38_04->setStyleSheet("QLabel{background-color:lightgray}");
	}
}
void rollersClass::hideButtons()
{
	for (int i=0;i<areaButtonsVector.count();i++)
		areaButtonsVector[i]->hide();
}
void rollersClass::showButtons()
{
	for (int i=0;i<areaButtonsVector.count();i++)
		areaButtonsVector[i]->show();
}
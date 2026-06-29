#include "straping.h"
strapingClass::strapingClass(tcpServerClass *tcpServer_,QWidget *p):QWidget(p)
{
	defineNewPacketDlg=NULL;
	//this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px}  QCheckBox{font-size:%1px}").arg(12));
	plcLength=0;
	noOfWoods=0;
	lastWeight=0.f;
	tcpServer=tcpServer_;
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);	
	QSplitter *mainSplitter=new QSplitter(Qt::Vertical,this);mainLayout->addWidget(mainSplitter);
	/////////////////////////////////////////////////////////////////////////area
	QImage image(":/strapingLayout.png");
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
	autoPushbutton=new myQPushButton(tcpServer,"","N162|2|0",tr(""),imageLabel);
	autoPushbutton->setGeometry(0,460,100,30);
	myQPushButton *resetAlarmsPushButton=new myQPushButton(tcpServer,"N160|23|422","N160|23|0",tr("Reset Alarms"),imageLabel);
	resetAlarmsPushButton->setGeometry(105,460,100,30);
	stopLabel_1_18=new QLabel("",imageLabel);
	stopLabel_1_18->setGeometry(210,460,100,30);
	
	int X,Y;
	X=875;Y=135;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT ,"N162|2|823","N162|2|0",tr("R6 right"          ), X+54,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT  ,"N162|2|824","N162|2|0",tr("R6 left"           ), X   ,Y+10,imageLabel);

	X=1490;Y=130;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT ,"N162|2|818","N162|2|0",tr("R8 right"          ), X+54,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT  ,"N162|2|819","N162|2|0",tr("R8 left"           ), X   ,Y+10,imageLabel);
	
	X=1200;Y=250;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT ,"N162|2|825","N162|2|0",tr("R6 & R8 right"          ), X+54,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT  ,"N162|2|826","N162|2|0",tr("R6 & R8 left"           ), X   ,Y+10,imageLabel);

	X=1100;Y=145;
	resetPacketQPushButton=new myQPushButton(tcpServer,"","",tr("Reset packet"),imageLabel);resetPacketQPushButton->setGeometry(X,Y,100,30);
	connect(resetPacketQPushButton,SIGNAL(clicked()),this,SLOT(resetPacketSlot()));

	closeR6RollersPushButton=new myQPushButton(tcpServer,"N162|2|841","N162|2|0",tr("Close side rollers"       ),imageLabel);closeR6RollersPushButton->setGeometry(720,130,110,20);
	openR6RollersPushButton=new myQPushButton(tcpServer,"N162|2|840","N162|2|0",tr("Open side rollers"          ),imageLabel);openR6RollersPushButton->setGeometry (720,170,110,20);
	//r5->r6
	r5Tor6Label=new QLabel(tr("             R5 to R6"),imageLabel);r5Tor6Label->setMargin(5);r5Tor6Label->setGeometry(570,110,125,20);
	myQPushButton *r52r6PushButtonOff=new myQPushButton(tcpServer,"N162|2|835","N162|2|0",tr("Off"),imageLabel);r52r6PushButtonOff->setGeometry(570,130,125,20);
	//
	readyForStrapping=new QLabel(tr("Ready for straping"),imageLabel);readyForStrapping->setMargin(5);readyForStrapping->setGeometry(1180,90,100,25);
	//r5
	QGroupBox *r5GroupBox=new QGroupBox(tr("R5"),imageLabel);
	QGridLayout *r5GroupBoxLayout=new QGridLayout();
	QPushButton *packetOnR5PushButtonOn=new QPushButton(tr("On"),imageLabel);r5GroupBoxLayout->addWidget(packetOnR5PushButtonOn,0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	packetOnR5Label=new QLabel(tr("packet"),imageLabel);packetOnR5Label->setMargin(5);r5GroupBoxLayout->addWidget(packetOnR5Label,0,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	QPushButton *packetOnR5PushButtonOff=new QPushButton(tr("Off"),imageLabel);r5GroupBoxLayout->addWidget(packetOnR5PushButtonOff,0,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	connect(packetOnR5PushButtonOn,SIGNAL(clicked()),this,SLOT(packetOnR5PushButtonOnSlot()));
	connect(packetOnR5PushButtonOff,SIGNAL(clicked()),this,SLOT(packetOnR5PushButtonOffSlot()));
	
	QPushButton *sbOnR5PushButtonOn=new QPushButton(tr("On"),imageLabel);r5GroupBoxLayout->addWidget(sbOnR5PushButtonOn,1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	sbOnR5Label=new QLabel(tr("S.B."),imageLabel);sbOnR5Label->setMargin(5);r5GroupBoxLayout->addWidget(sbOnR5Label,1,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	QPushButton *sbOnR5PushButtonOff=new QPushButton(tr("Off"),imageLabel);r5GroupBoxLayout->addWidget(sbOnR5PushButtonOff,1,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	r5GroupBox->setLayout(r5GroupBoxLayout);r5GroupBox->setGeometry(350,30,175,90);
	connect(sbOnR5PushButtonOn,SIGNAL(clicked()),this,SLOT(sbOnR5PushButtonOnSlot()));
	connect(sbOnR5PushButtonOff,SIGNAL(clicked()),this,SLOT(sbOnR5PushButtonOffSlot()));
	//////r6
	QGroupBox *r6GroupBox=new QGroupBox(tr("R6"),imageLabel);
	QGridLayout *r6GroupBoxLayout=new QGridLayout();
	QPushButton *packetOnR6PushButtonOn=new QPushButton(tr("On"),imageLabel);r6GroupBoxLayout->addWidget(packetOnR6PushButtonOn,0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	packetOnR6Label=new QLabel(tr("packet"),imageLabel);packetOnR6Label->setMargin(5);r6GroupBoxLayout->addWidget(packetOnR6Label,0,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	QPushButton *packetOnR6PushButtonOff=new QPushButton(tr("Off"),imageLabel);r6GroupBoxLayout->addWidget(packetOnR6PushButtonOff,0,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	connect(packetOnR6PushButtonOn,SIGNAL(clicked()),this,SLOT(packetOnR6PushButtonOnSlot()));
	connect(packetOnR6PushButtonOff,SIGNAL(clicked()),this,SLOT(packetOnR6PushButtonOffSlot()));

	QPushButton *sbOnR6PushButtonOn=new QPushButton(tr("On"),imageLabel);r6GroupBoxLayout->addWidget(sbOnR6PushButtonOn,1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	sbOnR6Label=new QLabel(tr("S.B."),imageLabel);sbOnR6Label->setMargin(5);r6GroupBoxLayout->addWidget(sbOnR6Label,1,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	QPushButton *sbOnR6PushButtonOff=new QPushButton(tr("Off"),imageLabel);r6GroupBoxLayout->addWidget(sbOnR6PushButtonOff,1,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	r6GroupBox->setLayout(r6GroupBoxLayout);r6GroupBox->setGeometry(850,30,175,90);
	connect(sbOnR6PushButtonOn,SIGNAL(clicked()),this,SLOT(sbOnR6PushButtonOnSlot()));
	connect(sbOnR6PushButtonOff,SIGNAL(clicked()),this,SLOT(sbOnR6PushButtonOffSlot()));
	//r8
	QGroupBox *r8GroupBox=new QGroupBox(tr("R8"),imageLabel);
	QGridLayout *r8GroupBoxLayout=new QGridLayout();r8GroupBox->setLayout(r8GroupBoxLayout);r8GroupBox->setGeometry(1450,30,175,75);
	QPushButton *packetOnR8PushButtonOn=new QPushButton(tr("On"),imageLabel);r8GroupBoxLayout->addWidget(packetOnR8PushButtonOn,0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	packetOnR8Label=new QLabel(tr("packet"),imageLabel);packetOnR8Label->setGeometry(X+50,Y,100,30);packetOnR8Label->setMargin(5);r8GroupBoxLayout->addWidget(packetOnR8Label,0,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	QPushButton *packetOnR8PushButtonOff=new QPushButton(tr("Off"),imageLabel);packetOnR8PushButtonOff->setGeometry(X+150,Y,50,30);r8GroupBoxLayout->addWidget(packetOnR8PushButtonOff,0,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	connect(packetOnR8PushButtonOn,SIGNAL(clicked()),this,SLOT(packetOnR8PushButtonOnSlot()));
	connect(packetOnR8PushButtonOff,SIGNAL(clicked()),this,SLOT(packetOnR8PushButtonOffSlot()));
	//newPacketInShipingBasketStationButton
	newPacketInShipingBasketStationButton=new QPushButton(tr("New S.B. ready"),imageLabel);newPacketInShipingBasketStationButton->setGeometry(850,250,100,50);
	connect(newPacketInShipingBasketStationButton,SIGNAL(clicked()),this,SLOT(newPacketInShipingBasketStationButtonSlot()));
	//newPacketInShipingBasketStationButton
	newPacketInPacketStationButton=new QPushButton(tr("New packet ready"),imageLabel);newPacketInPacketStationButton->setGeometry(1500,250,100,50);
	connect(newPacketInPacketStationButton,SIGNAL(clicked()),this,SLOT(newPacketInPacketStationButtonSlot()));
	//newPacketInImportStationButton
	newPacketInImportStationButton=new QPushButton(tr("New packet ready"),imageLabel);newPacketInImportStationButton->setGeometry(1700,250,100,50);
	connect(newPacketInImportStationButton,SIGNAL(clicked()),this,SLOT(newPacketInImportStationButtonSlot()));
	//newPacketInImportStationButton->hide();
	//newPacketInShipingBasketStationButton
	newSBInImportStationButton=new QPushButton(tr("New S.B. ready"),imageLabel);newSBInImportStationButton->setGeometry(1700,300,100,50);
	connect(newSBInImportStationButton,SIGNAL(clicked()),this,SLOT(newSBInImportStationButtonSlot()));
	//newSBInImportStationButton->hide();
	//
	ph1Label=new QLabel(tr(" PH1"),imageLabel);ph1Label->setGeometry(1300,155,25,20);
	ls2Label=new QLabel(tr(" LS2"),imageLabel);ls2Label->setGeometry(1392,155,25,20);
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
	QGroupBox *dimentionsGroupBox=new QGroupBox(tr("Packet dimentions"));btnsMainLayout0->addWidget(dimentionsGroupBox);
	QGridLayout *dimentionsGroupBoxLayout=new QGridLayout();dimentionsGroupBox->setLayout(dimentionsGroupBoxLayout);

	dimentionsGroupBoxLayout->addWidget(new QLabel(tr("Length :")),0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	lengthLabel=new QLabel(tr("............." ));lengthLabel->setMargin(5);
	dimentionsGroupBoxLayout->addWidget(lengthLabel,0,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	changeLengthPushButton=new QPushButton(tr("Change"));
	dimentionsGroupBoxLayout->addWidget(changeLengthPushButton,0,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	connect(changeLengthPushButton,SIGNAL(clicked()),this,SLOT(changeLengthPushButtonSlot()));


	dimentionsGroupBoxLayout->addWidget(new QLabel(tr("Width :")),1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	widthLabel=new QLabel(tr("........." ));widthLabel->setMargin(5);
	dimentionsGroupBoxLayout->addWidget(widthLabel,1,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	
	dimentionsGroupBoxLayout->addWidget(new QLabel(tr("Height :")),2,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	heightLabel=new QLabel(tr("........."));heightLabel->setMargin(5);
	dimentionsGroupBoxLayout->addWidget(heightLabel,2,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);

	r5ToR6PushButton=new myQPushButton(tcpServer,"N162|2|846","N162|2|0",tr("R5 to R6"),NULL);
	btnsMainLayout0->addWidget(r5ToR6PushButton);
	r5ToR62PacketsPushButton=new myQPushButton(tcpServer,"N162|2|847","N162|2|0",tr("R5 to R6 (2 packets)"),NULL);
	btnsMainLayout0->addWidget(r5ToR62PacketsPushButton);
	startStrapingPushButton=new myQPushButton(tcpServer,"N162|2|834","N162|2|0",tr("Start straping"),NULL);
	btnsMainLayout0->addWidget(startStrapingPushButton);
	takePacketQPushButton=new myQPushButton(tcpServer,"N160|23|429","N160|23|0",tr("Take packet"),NULL);
	btnsMainLayout0->addWidget(takePacketQPushButton);	
	checkWoodsPositionLabel=new QLabel(tr("Check woods position."));checkWoodsPositionLabel->setMargin(5);
	btnsMainLayout0->addWidget(checkWoodsPositionLabel);
	btnsMainLayout0->addStretch(1);
	//btnsMainLayout1
	QGroupBox *strapingGroupBox=new QGroupBox(tr("Strapping"));btnsMainLayout1->addWidget(strapingGroupBox);
	QVBoxLayout *strapingGroupBoxLayout=new QVBoxLayout();strapingGroupBox->setLayout(strapingGroupBoxLayout);
	strapingGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N162|2|801","N162|2|0",tr("Head up"),NULL),Qt::AlignHCenter|Qt::AlignCenter);
	strapingGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N162|2|802","N162|2|0",tr("Head down"),NULL),Qt::AlignHCenter|Qt::AlignCenter);
	strapingGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N162|2|803","N162|2|0",tr("Close wood"),NULL),Qt::AlignHCenter|Qt::AlignCenter);
	strapingGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N162|2|804","N162|2|0",tr("Open wood"),NULL),Qt::AlignHCenter|Qt::AlignCenter);
	strapingGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N162|2|805","N162|2|0",tr("Open small cover"),NULL),Qt::AlignHCenter|Qt::AlignCenter);
	strapingGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N162|2|806","N162|2|0",tr("Close small cover"),NULL),Qt::AlignHCenter|Qt::AlignCenter);
	strapingGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N162|2|807","N162|2|0",tr("Open big cover"),NULL),Qt::AlignHCenter|Qt::AlignCenter);
	strapingGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N162|2|808","N162|2|0",tr("Close big cover"),NULL),Qt::AlignHCenter|Qt::AlignCenter);

	strapingGroupBoxLayout->addWidget(new QLabel(tr("Woods offset")),Qt::AlignHCenter|Qt::AlignCenter);
	QHBoxLayout *woodsOffsetL=new QHBoxLayout();strapingGroupBoxLayout->addLayout(woodsOffsetL);

	woodsOffsetL->addWidget(woodOffsetLabel=new QLabel());
	woodOffsetSpinBox=new QSpinBox();woodOffsetSpinBox->setMinimum(-100);woodOffsetSpinBox->setMaximum(100);
	connect(woodOffsetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(woodOffsetSpinBoxValueChangedSlot(int)));
	woodsOffsetL->addWidget(woodOffsetSpinBox);



	btnsMainLayout1->addStretch(1);
	//btnsMainLayout2
	QGroupBox *tensionGroupBox=new QGroupBox(tr("Tension"));btnsMainLayout2->addWidget(tensionGroupBox);
	QGridLayout *tensionGroupBoxLayout=new QGridLayout();tensionGroupBox->setLayout(tensionGroupBoxLayout);
	dieTensionSpinBox=new QSpinBox();dieTensionSpinBox->setMinimum(1);dieTensionSpinBox->setMaximum(100000);
	connect(dieTensionSpinBox,SIGNAL(valueChanged(int)),this,SLOT(dieTensionSpinBoxSlot(int)));
	tensionGroupBoxLayout->addWidget(new QLabel(tr("Die Number")),0,0,Qt::AlignHCenter|Qt::AlignCenter);
	tensionGroupBoxLayout->addWidget(dieTensionSpinBox,0,1,Qt::AlignHCenter|Qt::AlignCenter);
	dieTensionSpinBox->setMinimumWidth(75);
	
	percendageTensionSpinBox=new QSpinBox();percendageTensionSpinBox->setMinimum(0);percendageTensionSpinBox->setMaximum(100);percendageTensionSpinBox->setSuffix("%");
	connect(percendageTensionSpinBox,SIGNAL(valueChanged(int)),this,SLOT(percendageTensionSpinBoxSlot(int)));
	tensionGroupBoxLayout->addWidget(new QLabel(tr("Tension")),1,0,Qt::AlignHCenter|Qt::AlignCenter);
	tensionGroupBoxLayout->addWidget(percendageTensionSpinBox,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	percendageTensionSpinBox->setMinimumWidth(75);
	////
	QGroupBox *woods4GroupBox=new QGroupBox(tr("4 woods packets"));btnsMainLayout2->addWidget(woods4GroupBox);
	QGridLayout *woods4GroupBoxLayout=new QGridLayout();woods4GroupBox->setLayout(woods4GroupBoxLayout);
	woods4GroupBoxLayout->addWidget(new QLabel(tr("Horizontal")),0,0,Qt::AlignRight|Qt::AlignVCenter);
	woods4HorizontalLength=new QLabel("...");woods4GroupBoxLayout->addWidget(woods4HorizontalLength,0,1,Qt::AlignLeft|Qt::AlignVCenter);
	woods4GroupBoxLayout->addWidget(new QLabel(tr("Vertical packet 1")),1,0,Qt::AlignRight|Qt::AlignVCenter);
	woods4VerticalLengthPacket1=new QLabel("...");woods4GroupBoxLayout->addWidget(woods4VerticalLengthPacket1,1,1,Qt::AlignLeft|Qt::AlignVCenter);
	woods4GroupBoxLayout->addWidget(new QLabel(tr("Vertical packet 2")),2,0,Qt::AlignRight|Qt::AlignVCenter);
	woods4VerticalLengthPacket2=new QLabel("...");woods4GroupBoxLayout->addWidget(woods4VerticalLengthPacket2,2,1,Qt::AlignLeft|Qt::AlignVCenter);

	QGroupBox *woods2GroupBox=new QGroupBox(tr("2 woods packets"));btnsMainLayout2->addWidget(woods2GroupBox);
	QGridLayout *woods2GroupBoxLayout=new QGridLayout();woods2GroupBox->setLayout(woods2GroupBoxLayout);
	woods2GroupBoxLayout->addWidget(new QLabel(tr("Horizontal")),0,0,Qt::AlignRight|Qt::AlignVCenter);
	woods2HorizontalLength=new QLabel("...");woods2GroupBoxLayout->addWidget(woods2HorizontalLength,0,1,Qt::AlignLeft|Qt::AlignVCenter);

	QGroupBox *numberOfWoodsGroupBox=new QGroupBox(tr("Number of woods"));btnsMainLayout2->addWidget(numberOfWoodsGroupBox);
	QVBoxLayout *numberOfWoodsGroupBoxLayout=new QVBoxLayout();numberOfWoodsGroupBox->setLayout(numberOfWoodsGroupBoxLayout);
	numberOfWoodsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N162|2|844","N162|2|0",tr("2-3-4-5-6"),NULL),Qt::AlignHCenter|Qt::AlignCenter);
	numberOfWoodsGroupBoxLayout->addWidget(noOfWoodsLabel=new QLabel(tr(". . . . . . . . . ")),Qt::AlignHCenter|Qt::AlignCenter);
	numberOfWoodsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N162|2|845","N162|2|0",tr("2-3-5-7"),NULL),Qt::AlignHCenter|Qt::AlignCenter);
	btnsMainLayout2->addStretch(1);
	//btnsMainLayout3
	btnsMainLayout3->addStretch(1);
	//btnsMainLayout4
	btnsMainLayout4->addStretch(1);
	//btnsMainLayout5
	//QGroupBox *warningSignalsGroupBox=new QGroupBox(tr("Warnings"));btnsMainLayout5->addWidget(warningSignalsGroupBox);
	//QVBoxLayout *warningSignalsGroupBoxLayout=new QVBoxLayout();warningSignalsGroupBox->setLayout(warningSignalsGroupBoxLayout);
	//warningSignalsGroupBox->setLayout(warningSignalsGroupBoxLayout);
	//btnsMainLayout5->addStretch(1);
	//btnsMainLayout6
	QGroupBox *alarmsSignalsGroupBox=new QGroupBox(tr("Alarms"));btnsMainLayout6->addWidget(alarmsSignalsGroupBox);
	QVBoxLayout *alarmsSignalsGroupBoxLayout=new QVBoxLayout();
	signal_00_00=new QLabel(tr("Strapping head not up"						));signal_00_00->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_00,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_01=new QLabel(tr("Strapping head not down"					));signal_00_01->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_01,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_02=new QLabel(tr("Wood not closed"									));signal_00_02->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_02,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_03=new QLabel(tr("Wood not opened"									));signal_00_03->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_03,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_04=new QLabel(tr("Wood not at home position"				));signal_00_04->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_04,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_05=new QLabel(tr("Wood not at strap position"				));signal_00_05->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_05,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_06=new QLabel(tr("Cover not opened"									));signal_00_06->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_06,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_07=new QLabel(tr("Cover not back position"					));signal_00_07->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_07,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_08=new QLabel(tr("Cover not front position"					));signal_00_08->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_08,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_09=new QLabel(tr("Thermal overload Q16,Q17,U16,U17"	));signal_00_09->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_09,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_10=new QLabel(tr("N162[0].10"												));signal_00_10->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_10,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_11=new QLabel(tr("N162[0].11"												));signal_00_11->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_11,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_12=new QLabel(tr("N162[0].12"												));signal_00_12->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_12,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_13=new QLabel(tr("N162[0].13"												));signal_00_13->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_13,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_14=new QLabel(tr("N162[0].14"												));signal_00_14->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_14,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_15=new QLabel(tr("N162[0].15"												));signal_00_15->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_15,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_16=new QLabel(tr("N162[0].16"												));signal_00_16->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_16,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_17=new QLabel(tr("N162[0].17"												));signal_00_17->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_17,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_18=new QLabel(tr("N162[0].18"												));signal_00_18->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_18,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_19=new QLabel(tr("N162[0].19"												));signal_00_19->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_00_19,Qt::AlignHCenter|Qt::AlignCenter);
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
	QList<int> sizes;sizes<<730<<600;
	mainSplitter->setSizes(sizes);
}
void strapingClass::dieTensionSpinBoxSlot(int dieNum)
{
	int dieInTable=GETONEATMOST(QString("select die from tension where die=%1").arg(dieNum),STRAPING_DB_CONNECTION).toInt();
	if (dieInTable==0)
		execQuery(QString("insert into tension  (die,tension) values(%1,%2)").arg(dieNum).arg(0),STRAPING_DB_CONNECTION);
	int tensionInTable=GETONEATMOST(QString("select tension from tension where die=%1").arg(dieNum),STRAPING_DB_CONNECTION).toInt();
	percendageTensionSpinBox->setValue(tensionInTable);
}
void strapingClass::percendageTensionSpinBoxSlot(int percentNum)
{
	execQuery(QString("update tension set tension=%1 where die=%2").arg(percentNum).arg(dieTensionSpinBox->value()),STRAPING_DB_CONNECTION);
}
void strapingClass::woodOffsetSpinBoxValueChangedSlot(int off)
{
	tcpServer->sendString(QString("N160|14|%1").arg(off));
}
void strapingClass::plcFloatMemoryChanged(int index,float value)
{
	if (index==3){lengthLabel->setText(QString("%1 mm").arg(int(value)));plcLength=value;}
	if (index==1)
	{
		widthLabel->setText(QString("%1 mm").arg(int(value)));
		woods4HorizontalLength->setText(QString("%1 mm").arg(int(value+10.)));
		woods2HorizontalLength->setText(QString("%1 mm").arg(int(value-15.)));
	}
	if (index==2)
	{
		heightLabel->setText(QString("%1 mm").arg(int(value)));
		woods4VerticalLengthPacket1->setText(QString("%1 mm").arg(int(value-12.)));
	}
	if (index==11)
	{
		woods4VerticalLengthPacket2->setText(QString("%1 mm").arg(int(value-12.)));
	}
	if (index==4)sbPosPacketLength=value;
	if (index==5)sbPosPacketWidth=value;
	if (index==6)sbPacketHeight=value;
	if (index==7)packetPosPacketLength=value;
	if (index==8)packetPosPacketWidth=value;
	if (index==9)packetPosPacketHeight=value;
	if (index==10)noOfWoods=value;
}
void strapingClass::plcMemoryN160Changed(int index,int value)
{
	QVector<bool> bitsVector=deprecatedCalculateBits(value);
	if (index==5)
	{
		if (bitsVector[10])
			takePacketQPushButton->setEnabled(true);
		else 
			takePacketQPushButton->setEnabled(false);
		if (bitsVector[18]){checkWoodsPositionLabel->setStyleSheet("QLabel{background-color:red}");}else {checkWoodsPositionLabel->setStyleSheet("QLabel{background-color:lightgray}");}
	}
	if (index==14)
		woodOffsetLabel->setText(tr("Actual:%1").arg(value));
}
void strapingClass::plcMemoryN162Changed(int index,int value)
{
	if ((index==2)&&(value==846))
		r5ToR6PushButton->setEnabled(false);
	QVector<bool> bitsVector=deprecatedCalculateBits(value);
	if (index==0)
	{
		if (bitsVector[ 0]){signal_00_00->setStyleSheet("QLabel{background-color:red}");signal_00_00->show();}else {signal_00_00->setStyleSheet("QLabel{background-color:lightgray}");signal_00_00->hide();}
		if (bitsVector[ 1]){signal_00_01->setStyleSheet("QLabel{background-color:red}");signal_00_01->show();}else {signal_00_01->setStyleSheet("QLabel{background-color:lightgray}");signal_00_01->hide();}
		if (bitsVector[ 2]){signal_00_02->setStyleSheet("QLabel{background-color:red}");signal_00_02->show();}else {signal_00_02->setStyleSheet("QLabel{background-color:lightgray}");signal_00_02->hide();}
		if (bitsVector[ 3]){signal_00_03->setStyleSheet("QLabel{background-color:red}");signal_00_03->show();}else {signal_00_03->setStyleSheet("QLabel{background-color:lightgray}");signal_00_03->hide();}
		if (bitsVector[ 4]){signal_00_04->setStyleSheet("QLabel{background-color:red}");signal_00_04->show();}else {signal_00_04->setStyleSheet("QLabel{background-color:lightgray}");signal_00_04->hide();}
		if (bitsVector[ 5]){signal_00_05->setStyleSheet("QLabel{background-color:red}");signal_00_05->show();}else {signal_00_05->setStyleSheet("QLabel{background-color:lightgray}");signal_00_05->hide();}
		if (bitsVector[ 6]){signal_00_06->setStyleSheet("QLabel{background-color:red}");signal_00_06->show();}else {signal_00_06->setStyleSheet("QLabel{background-color:lightgray}");signal_00_06->hide();}
		if (bitsVector[ 7]){signal_00_07->setStyleSheet("QLabel{background-color:red}");signal_00_07->show();}else {signal_00_07->setStyleSheet("QLabel{background-color:lightgray}");signal_00_07->hide();}
		if (bitsVector[ 8]){signal_00_08->setStyleSheet("QLabel{background-color:red}");signal_00_08->show();}else {signal_00_08->setStyleSheet("QLabel{background-color:lightgray}");signal_00_08->hide();}
		if (bitsVector[ 9]){signal_00_09->setStyleSheet("QLabel{background-color:red}");signal_00_09->show();}else {signal_00_09->setStyleSheet("QLabel{background-color:lightgray}");signal_00_09->hide();}
		if (bitsVector[10]){signal_00_10->setStyleSheet("QLabel{background-color:red}");signal_00_10->show();}else {signal_00_10->setStyleSheet("QLabel{background-color:lightgray}");signal_00_10->hide();}
		if (bitsVector[11]){signal_00_11->setStyleSheet("QLabel{background-color:red}");signal_00_11->show();}else {signal_00_11->setStyleSheet("QLabel{background-color:lightgray}");signal_00_11->hide();}
		if (bitsVector[12]){signal_00_12->setStyleSheet("QLabel{background-color:red}");signal_00_12->show();}else {signal_00_12->setStyleSheet("QLabel{background-color:lightgray}");signal_00_12->hide();}
		if (bitsVector[13]){signal_00_13->setStyleSheet("QLabel{background-color:red}");signal_00_13->show();}else {signal_00_13->setStyleSheet("QLabel{background-color:lightgray}");signal_00_13->hide();}
		if (bitsVector[14]){signal_00_14->setStyleSheet("QLabel{background-color:red}");signal_00_14->show();}else {signal_00_14->setStyleSheet("QLabel{background-color:lightgray}");signal_00_14->hide();}
		if (bitsVector[15]){signal_00_15->setStyleSheet("QLabel{background-color:red}");signal_00_15->show();}else {signal_00_15->setStyleSheet("QLabel{background-color:lightgray}");signal_00_15->hide();}
		if (bitsVector[16]){signal_00_16->setStyleSheet("QLabel{background-color:red}");signal_00_16->show();}else {signal_00_16->setStyleSheet("QLabel{background-color:lightgray}");signal_00_16->hide();}
		if (bitsVector[17]){signal_00_17->setStyleSheet("QLabel{background-color:red}");signal_00_17->show();}else {signal_00_17->setStyleSheet("QLabel{background-color:lightgray}");signal_00_17->hide();}
		if (bitsVector[18]){signal_00_18->setStyleSheet("QLabel{background-color:red}");signal_00_18->show();}else {signal_00_18->setStyleSheet("QLabel{background-color:lightgray}");signal_00_18->hide();}
		if (bitsVector[19]){signal_00_19->setStyleSheet("QLabel{background-color:red}");signal_00_19->show();}else {signal_00_19->setStyleSheet("QLabel{background-color:lightgray}");signal_00_19->hide();}
		
	}
	if (index==1)
	{
		if (bitsVector[10])
		{
			autoPushbutton->setText("Auto");
			autoPushbutton->setSendCommandToServerString("N162|2|815");
			hideButtons();
			openR6RollersPushButton->hide();
			closeR6RollersPushButton->hide();
		}
		else
		{
			autoPushbutton->setText("Manual");
			autoPushbutton->setSendCommandToServerString("N162|2|814");
			openR6RollersPushButton->show();
			closeR6RollersPushButton->show();
			showButtons();
		}
		if (bitsVector[18])
		{
			stopLabel_1_18->setText("  Active stop");
			stopLabel_1_18->setStyleSheet("QLabel{text-align:center;color:white; border: 4px solid red;border-radius: 4px;}");
		}
		else
		{
			stopLabel_1_18->setText("  No active stop");
			stopLabel_1_18->setStyleSheet("QLabel{text-align:center;color:gray}");
		}
		if (bitsVector[26])packetOnR5Label->setStyleSheet("QLabel{background-color:green}");else packetOnR5Label->setStyleSheet("");
		if (bitsVector[27])    sbOnR5Label->setStyleSheet("QLabel{background-color:green}");else     sbOnR5Label->setStyleSheet("");
		if (bitsVector[13])packetOnR6Label->setStyleSheet("QLabel{background-color:green}");else packetOnR6Label->setStyleSheet("");
		if (bitsVector[19])
		{
			sbOnR6Label->setStyleSheet("QLabel{background-color:green}");
			newPacketInShipingBasketStationButton->setEnabled(true);
		}
		else
		{
			sbOnR6Label->setStyleSheet("");
			newPacketInShipingBasketStationButton->setEnabled(false);
		}
		if (bitsVector[12])
		{
			packetOnR8Label->setStyleSheet("QLabel{background-color:green}");
			newPacketInPacketStationButton->setEnabled(true);
		}
		else
		{
			packetOnR8Label->setStyleSheet("");
			newPacketInPacketStationButton->setEnabled(false);
		}
		if (bitsVector[24])readyForStrapping->setStyleSheet("QLabel{background-color:green}");else readyForStrapping->setStyleSheet("");
		if (bitsVector[25])r5Tor6Label->setStyleSheet("QLabel{background-color:green}");else r5Tor6Label->setStyleSheet("");
		if (bitsVector[20])
			startStrapingPushButton->setEnabled(true);
		else 
		{
			if (startStrapingPushButton->isEnabled())
			{
				tcpServer->sendString(QString("N162|2|0"));
				startStrapingPushButton->setEnabled(false);
			}
		}
		if (bitsVector[22])
		{
			r5ToR6PushButton->setText("Package from R5 to R6");
			r5ToR6PushButton->setEnabled(true);
			r5ToR62PacketsPushButton->setEnabled(true);
		}
		else if (bitsVector[23])
		{
			r5ToR6PushButton->setText("S.B. from R5 to R6");
			r5ToR6PushButton->setEnabled(true);
			r5ToR62PacketsPushButton->setEnabled(true);
		}
		else
		{
			r5ToR6PushButton->setText("R5 to R6");
			r5ToR6PushButton->setEnabled(false);
			r5ToR62PacketsPushButton->setEnabled(false);
		}
		//
		if (bitsVector[29])
			noOfWoodsLabel->setText("2-3-4-5-6");
		else
			noOfWoodsLabel->setText("2-3-5-7");
		if (bitsVector[30]){ph1Label->setStyleSheet("QLabel{background-color:green}");}else {ph1Label->setStyleSheet("QLabel{background-color:lightgray}");}
		if (bitsVector[31]){ls2Label->setStyleSheet("QLabel{background-color:green}");}else {ls2Label->setStyleSheet("QLabel{background-color:lightgray}");}
	}
}

void strapingClass::changeLengthPushButtonSlot()
{
	bool ok;
	int value=QInputDialog::getInt(this,tr("Define the length of the packet"),tr("Define the length of the packet"),plcLength,MIN_PACKET_LENGTH,MAX_PACKET_LENGTH,1,&ok);
	if (ok)
	{
		tcpServer->sendString(QString("F163|0|%1").arg(value));
		tcpServer->sendString(QString("F163|0|0"));
	}
}
//r5
void strapingClass::packetOnR5PushButtonOnSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Change PLC memory"));
	msgBox.setText(tr("This will change a memory in PLC."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N162|2|836"));
	tcpServer->sendString(QString("N162|2|0"));
}
void strapingClass::sbOnR5PushButtonOnSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Change PLC memory"));
	msgBox.setText(tr("This will change a memory in PLC."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N162|2|838"));
	tcpServer->sendString(QString("N162|2|0"));
}
void strapingClass::packetOnR5PushButtonOffSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Change PLC memory"));
	msgBox.setText(tr("This will change a memory in PLC."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N162|2|837"));
	tcpServer->sendString(QString("N162|2|0"));
}
void strapingClass::sbOnR5PushButtonOffSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Change PLC memory"));
	msgBox.setText(tr("This will change a memory in PLC."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N162|2|839"));
	tcpServer->sendString(QString("N162|2|0"));
}
//r6
void strapingClass::packetOnR6PushButtonOnSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Change PLC memory"));
	msgBox.setText(tr("This will change a memory in PLC."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N162|2|832"));
	tcpServer->sendString(QString("N162|2|0"));
}
void strapingClass::sbOnR6PushButtonOnSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Change PLC memory"));
	msgBox.setText(tr("This will change a memory in PLC."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N162|2|830"));
	tcpServer->sendString(QString("N162|2|0"));
}
void strapingClass::packetOnR6PushButtonOffSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Change PLC memory"));
	msgBox.setText(tr("This will change a memory in PLC."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N162|2|833"));
	tcpServer->sendString(QString("N162|2|0"));
}
void strapingClass::sbOnR6PushButtonOffSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Change PLC memory"));
	msgBox.setText(tr("This will change a memory in PLC."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N162|2|831"));
	tcpServer->sendString(QString("N162|2|0"));
}
//r8
void strapingClass::packetOnR8PushButtonOnSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Change PLC memory"));
	msgBox.setText(tr("This will change a memory in PLC."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N162|2|816"));
	tcpServer->sendString(QString("N162|2|0"));
}
void strapingClass::packetOnR8PushButtonOffSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Change PLC memory"));
	msgBox.setText(tr("This will change a memory in PLC."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N162|2|817"));
	tcpServer->sendString(QString("N162|2|0"));
}
//
void strapingClass::resetPacketSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Change PLC memory"));
	msgBox.setText(tr("This will change a memory in PLC."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N162|2|829"));
	tcpServer->sendString(QString("N162|2|0"));
}
void strapingClass::hideButtons()
{
	for (int i=0;i<areaButtonsVector.count();i++)
		areaButtonsVector[i]->hide();
	resetPacketQPushButton->hide();
}
void strapingClass::showButtons()
{
	for (int i=0;i<areaButtonsVector.count();i++)
		areaButtonsVector[i]->show();
	resetPacketQPushButton->show();
}
defineNewPacketClass *strapingClass::getDefineNewPacketDlg()
{
	return defineNewPacketDlg;
}
void strapingClass::newPacketInShipingBasketStationButtonSlot()
{
	int packetOnPos=GETONE("select packetId from positions where posId=4001").toInt();
	if (packetOnPos!=0)
	{
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("Warning"));
		msgBox.setText(tr("There is already defined a packet in shiping basket position."));
		msgBox.setInformativeText(QString("The basket defined is:  %1").arg(packetOnPos));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
		return;
	}
	defineNewPacketDlg=new defineNewPacketClass(defineNewPacketClass::TYPE_SB_STATION,sbPosPacketLength,sbPosPacketWidth,sbPacketHeight,lastWeight,noOfWoods,this);
	defineNewPacketDlg->exec();
	delete defineNewPacketDlg;
	defineNewPacketDlg=NULL;
}
void strapingClass::newPacketInPacketStationButtonSlot()
{
	int packetOnPos=GETONE("select packetId from positions where posId=4002").toInt();
	if (packetOnPos!=0)
	{
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("Warning"));
		msgBox.setText(tr("There is already defined a packet in packet position."));
		msgBox.setInformativeText(QString("The basket defined is:  %1").arg(packetOnPos));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
		return;
	}
	defineNewPacketDlg=new defineNewPacketClass(defineNewPacketClass::TYPE_PACKET_STATION,packetPosPacketLength,packetPosPacketWidth,packetPosPacketHeight,lastWeight,noOfWoods,this);
	defineNewPacketDlg->exec();
	delete defineNewPacketDlg;
	defineNewPacketDlg=NULL;
}
void strapingClass::newPacketInImportStationButtonSlot()
{
	int packetOnPos=GETONE("select packetId from positions where posId=4005").toInt();
	if (packetOnPos!=0)
	{
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("Warning"));
		msgBox.setText(tr("There is already defined a packet in packet position."));
		msgBox.setInformativeText(QString("The basket defined is:  %1").arg(packetOnPos));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
		return;
	}
	defineNewPacketDlg=new defineNewPacketClass(defineNewPacketClass::TYPE_IMPORT_PACKET_STATION,0,0,0,0,0,this);
	defineNewPacketDlg->exec();
	delete defineNewPacketDlg;
	defineNewPacketDlg=NULL;
}
void strapingClass::newSBInImportStationButtonSlot()
{
	int packetOnPos=GETONE("select packetId from positions where posId=4005").toInt();
	if (packetOnPos!=0)
	{
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("Warning"));
		msgBox.setText(tr("There is already defined a packet in packet position."));
		msgBox.setInformativeText(QString("The basket defined is:  %1").arg(packetOnPos));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
		return;
	}
	defineNewPacketDlg=new defineNewPacketClass(defineNewPacketClass::TYPE_IMPORT_SB_STATION,0,0,0,0,0,this);
	defineNewPacketDlg->exec();
	delete defineNewPacketDlg;
	defineNewPacketDlg=NULL;
}

#include "conv.h"
conveyorsClass::conveyorsClass(tcpServerClass *tcpServer_,QWidget *p):QWidget(p)
{
	//this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px}  QCheckBox{font-size:%1px}").arg(12));
	tcpServer=tcpServer_;
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);	
	QSplitter *mainSplitter=new QSplitter(Qt::Vertical,this);mainLayout->addWidget(mainSplitter);
	cylinderUp=cylinderDown=false;
	/////////////////////////////////////////////////////////////////////////area
	QImage image(":/conveyorsLayout.png");
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
	autoPushbutton->setGeometry(0,320,100,30);
	stopLabel_0_3=new QLabel("",imageLabel);
	stopLabel_0_3->setGeometry(105,320,100,30);
	myQPushButton *resetAlarmsPushButton=new myQPushButton(tcpServer,"N100|30|50","N100|30|0",tr("Reset Alarms"),imageLabel);
	resetAlarmsPushButton->setGeometry(210,320,100,30);
	
	autoFeeding13Pushbutton=new myQPushButton(tcpServer,"","N100|30|0",tr(""),imageLabel);
	autoFeeding13Pushbutton->setGeometry(1540,2,80,20);
	autoFeeding15Pushbutton=new myQPushButton(tcpServer,"","N100|30|0",tr(""),imageLabel);
	autoFeeding15Pushbutton->setGeometry(1000,2,80,20);
	int X,Y;
	X=1530;Y=50;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_DOWN,"N100|30|11","N100|30|0",tr("Conveyor 13 forward"           ), X   ,Y+10,imageLabel);
	conveyorMovementVector<<new conveyorMovementClass( 0, 4,":/arrowDownC.png", 0, 5,":/arrowUpC.png" ,																						 X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_UP  ,"N100|30|12","N100|30|0",tr("Conveyor 13 backward"          ), X+54,Y+10,imageLabel);

	X=1000;Y=50;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_DOWN,"N100|30|15","N100|30|0",tr("Conveyor 15 forward"           ), X   ,Y+10,imageLabel);
	conveyorMovementVector<<new conveyorMovementClass( 0, 8,":/arrowDownC.png", 0, 9,":/arrowUpC.png" ,																						 X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_UP  ,"N100|30|16","N100|30|0",tr("Conveyor 15 backward"          ), X+54,Y+10,imageLabel);
	
	X=498;Y=50;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_UP			,"N100|30|22","N100|30|0",tr("Conveyor 18 forward"           ), X   ,Y+10,imageLabel);
	conveyorMovementVector<<new conveyorMovementClass( 0,14,":/arrowDownC.png", 0,15,":/arrowUpC.png" ,																						 X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_DOWN  ,"N100|30|21","N100|30|0",tr("Conveyor 18 backward"          ), X+54,Y+10,imageLabel);

	X=1300;Y=140;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT,"N100|30|13","N100|30|0",tr("Conveyor 14 forward"           ), X   ,Y+10,imageLabel);
	conveyorMovementVector<<new conveyorMovementClass( 0, 6,":/arrowLeftC.png", 0, 7,":/arrowRightC.png" ,																						 X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT  ,"N100|30|14","N100|30|0",tr("Conveyor 14 backward"          ), X+54,Y+10,imageLabel);

	X=1525;Y=140;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_UP,"N100|30|27","N100|30|0",tr("Conveyor 14 upward"           ), X   ,Y+10,imageLabel);
	//conveyorMovementVector<<new conveyorMovementClass( 2, 5,":/arrowUpC.png", 2, 6,":/arrowDownC.png" ,																						 X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_DOWN  ,"N100|30|28","N100|30|0",tr("Conveyor 14 downward"          ), X+54,Y+10,imageLabel);

	X=650;Y=140;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT,"N100|30|19","N100|30|0",tr("Conveyor 17 forward"           ), X   ,Y+10,imageLabel);
	conveyorMovementVector<<new conveyorMovementClass( 0,12,":/arrowLeftC.png", 0,13,":/arrowRightC.png" ,																						 X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT  ,"N100|30|20","N100|30|0",tr("Conveyor 17 backward"          ), X+54,Y+10,imageLabel);

	X=850;Y=140;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_UP,"N100|30|31","N100|30|0",tr("Conveyor 16 upward"           ), X   ,Y+10,imageLabel);
	//conveyorMovementVector<<new conveyorMovementClass( 2,16,":/arrowUpC.png", 2,17,":/arrowDownC.png" ,																						 X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_DOWN  ,"N100|30|32","N100|30|0",tr("Conveyor 16 downward"          ), X+54,Y+10,imageLabel);

	X=650;Y=230;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT,"N100|30|17","N100|30|0",tr("Conveyor 16 forward"           ), X   ,Y+10,imageLabel);
	conveyorMovementVector<<new conveyorMovementClass( 0,10,":/arrowLeftC.png", 0,11,":/arrowRightC.png" ,																						 X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT  ,"N100|30|18","N100|30|0",tr("Conveyor 16 backward"          ), X+54,Y+10,imageLabel);

	X=850;Y=230;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_UP,"N100|30|29","N100|30|0",tr("Conveyor 16 upward"           ), X   ,Y+10,imageLabel);
	//conveyorMovementVector<<new conveyorMovementClass( 2,10,":/arrowUpC.png", 2,11,":/arrowDownC.png" ,																						 X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_DOWN  ,"N100|30|30","N100|30|0",tr("Conveyor 16 downward"          ), X+54,Y+10,imageLabel);

	X=100;Y=140;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT,"N100|30|23","N100|30|0",tr("Conveyor 28 forward"           ), X   ,Y+10,imageLabel);
	conveyorMovementVector<<new conveyorMovementClass( 0,24,":/arrowLeftC.png", 0,25,":/arrowRightC.png" ,																						 X+30,Y,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT  ,"N100|30|24","N100|30|0",tr("Conveyor 28 backward"          ), X+54,Y+10,imageLabel);

	X=280;Y=140;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT ,"N100|30|77","N100|30|0",tr("Conveyor 28 and 17 forward"						), X   ,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT,"N100|30|78","N100|30|0",tr("Conveyor 28 and 17 backward"          ), X+30,Y+10,imageLabel);
	X=1147;Y=140;
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_LEFT ,"N100|30|75","N100|30|0",tr("Conveyor 14 and 17 forward"						), X   ,Y+10,imageLabel);
	areaButtonsVector<<new moveConveyorClass(tcpServer,moveConveyorClass::TYPE_RIGHT,"N100|30|76","N100|30|0",tr("Conveyor 14 and 17 backward"          ), X+30,Y+10,imageLabel);
	//
	myQPushButton *command51myQPushButton=new myQPushButton(tcpServer,"N100|30|51","N100|30|0",tr("On"),			imageLabel);command51myQPushButton->setGeometry(300,50,30,20);
	command51_52Label=new QLabel(tr("  Basket is for crane"),imageLabel);command51_52Label->setGeometry(330,50,100,20);
	myQPushButton *command52myQPushButton=new myQPushButton(tcpServer,"N100|30|52","N100|30|0",tr("Off"),	imageLabel);command52myQPushButton->setGeometry(430,50,30,20);

	///////////////////
	proximitySwitchVector<<new proximitySwitchClass(2, 1,1650, 70,"PS 701", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2, 2,1660,210,"PS 702", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2, 3,1690,180,"PS 703", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2, 4, 145,185,"PS 704", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2, 5,1215,145,"PS 705 up", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2, 6,1215,175,"PS 706 down", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2, 7,1140, 85,"PS 707", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2, 8,1130,310,"PS 708", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2, 9,1160,280,"PS 709", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2,10, 750,240,"PS 710 up", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2,11, 750,280,"PS 711 down", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2,12, 440,280,"PS 712", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2,13, 360,170,"PS 713", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2,14, 420,180,"PS 714", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2,15, 550,180,"PS 715", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2,16, 470, 20,"PS 716", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2,17, 510,110,"PS 717", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2,18, 750,140,"PS 718 up", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2,19, 750,180,"PS 719 down", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(2,28, 200,180,"PS 728", imageLabel);
	//plcMemoryVector
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",10,1540, 90,"13", tr("Conveyor 13 basket number"),imageLabel);
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",11,1620,140,"14", tr("Conveyor 14 basket number"),imageLabel);
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",12, 520,150,"20", tr("Conveyor 20 basket number"),imageLabel);
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",13,1020, 80,"15", tr("Conveyor 15 basket number"),imageLabel);
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",14, 990,250,"16", tr("Conveyor 16 basket number"),imageLabel);
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",15, 520,250,"19", tr("Conveyor 19 basket number"),imageLabel);
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",16,  30,125,"28", tr("Conveyor 28 basket number"),imageLabel);
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",17, 520, 80,"18", tr("Conveyor 18 basket number"),imageLabel);
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",18,1470, 20,"13^",tr("Conveyor 13 basket number delivered from basket crane"),imageLabel);
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",19, 950, 20,"15^",tr("Conveyor 15 basket number delivered from basket crane"),imageLabel);
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",20, 470, 20,"18^",tr("Conveyor 18 basket number delivered from basket crane"),imageLabel);
	plcMemoryVector<<new expStationPlcMemoryClass(tcpServer,"N100",21, 775,145,"17",tr("Conveyor 17 basket number"),imageLabel);
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
	QGroupBox *autoCycleGroupBox=new QGroupBox(tr("Auto cycles"));btnsMainLayout0->addWidget(autoCycleGroupBox);
	QGridLayout *autoCycleGroupBoxLayout=new QGridLayout();autoCycleGroupBox->setLayout(autoCycleGroupBoxLayout);
	int r=0;
	signal_01_00=new QLabel(tr("  14 to 18"));signal_01_00->setMinimumSize(QSize(75,28));	autoCycleGroupBoxLayout->addWidget(signal_01_00,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_01_01=new QLabel(tr("  13 to 14"));signal_01_01->setMinimumSize(QSize(75,28));	autoCycleGroupBoxLayout->addWidget(signal_01_01,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_01_02=new QLabel(tr("  14 to 28"));signal_01_02->setMinimumSize(QSize(75,28));	autoCycleGroupBoxLayout->addWidget(signal_01_02,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_01_03=new QLabel(tr("  28 to 14"));signal_01_03->setMinimumSize(QSize(75,28));	autoCycleGroupBoxLayout->addWidget(signal_01_03,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_01_04=new QLabel(tr("  15 to 16"));signal_01_04->setMinimumSize(QSize(75,28));	autoCycleGroupBoxLayout->addWidget(signal_01_04,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_01_05=new QLabel(tr("  16 to 18"));signal_01_05->setMinimumSize(QSize(75,28));	autoCycleGroupBoxLayout->addWidget(signal_01_05,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_01_06=new QLabel(tr("  16 to 28"));signal_01_06->setMinimumSize(QSize(75,28));	autoCycleGroupBoxLayout->addWidget(signal_01_06,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_01_07=new QLabel(tr("  28 to 16"));signal_01_07->setMinimumSize(QSize(75,28));	autoCycleGroupBoxLayout->addWidget(signal_01_07,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_01_08=new QLabel(tr("  18 to 28"));signal_01_08->setMinimumSize(QSize(75,28));	autoCycleGroupBoxLayout->addWidget(signal_01_08,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	signal_01_09=new QLabel(tr("  28 to 18"));signal_01_09->setMinimumSize(QSize(75,28));	autoCycleGroupBoxLayout->addWidget(signal_01_09,r++,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	r=0;
	myQPushButton *signal_01_00_onPushButton=new myQPushButton(tcpServer,"N100|30|53","N100|30|0",tr("On"),NULL);signal_01_00_onPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_00_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_01_onPushButton=new myQPushButton(tcpServer,"N100|30|55","N100|30|0",tr("On"),NULL);signal_01_01_onPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_01_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_02_onPushButton=new myQPushButton(tcpServer,"N100|30|57","N100|30|0",tr("On"),NULL);signal_01_02_onPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_02_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_03_onPushButton=new myQPushButton(tcpServer,"N100|30|59","N100|30|0",tr("On"),NULL);signal_01_03_onPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_03_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_04_onPushButton=new myQPushButton(tcpServer,"N100|30|61","N100|30|0",tr("On"),NULL);signal_01_04_onPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_04_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_05_onPushButton=new myQPushButton(tcpServer,"N100|30|63","N100|30|0",tr("On"),NULL);signal_01_05_onPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_05_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_06_onPushButton=new myQPushButton(tcpServer,"N100|30|65","N100|30|0",tr("On"),NULL);signal_01_06_onPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_06_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_07_onPushButton=new myQPushButton(tcpServer,"N100|30|67","N100|30|0",tr("On"),NULL);signal_01_07_onPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_07_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_08_onPushButton=new myQPushButton(tcpServer,"N100|30|69","N100|30|0",tr("On"),NULL);signal_01_08_onPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_08_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_09_onPushButton=new myQPushButton(tcpServer,"N100|30|71","N100|30|0",tr("On"),NULL);signal_01_09_onPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_09_onPushButton,r++,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	r=0;
	myQPushButton *signal_01_00_offPushButton=new myQPushButton(tcpServer,"N100|30|54","N100|30|0",tr("Off"),NULL);signal_01_00_offPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_00_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_01_offPushButton=new myQPushButton(tcpServer,"N100|30|56","N100|30|0",tr("Off"),NULL);signal_01_01_offPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_01_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_02_offPushButton=new myQPushButton(tcpServer,"N100|30|58","N100|30|0",tr("Off"),NULL);signal_01_02_offPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_02_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_03_offPushButton=new myQPushButton(tcpServer,"N100|30|60","N100|30|0",tr("Off"),NULL);signal_01_03_offPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_03_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_04_offPushButton=new myQPushButton(tcpServer,"N100|30|62","N100|30|0",tr("Off"),NULL);signal_01_04_offPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_04_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_05_offPushButton=new myQPushButton(tcpServer,"N100|30|64","N100|30|0",tr("Off"),NULL);signal_01_05_offPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_05_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_06_offPushButton=new myQPushButton(tcpServer,"N100|30|66","N100|30|0",tr("Off"),NULL);signal_01_06_offPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_06_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_07_offPushButton=new myQPushButton(tcpServer,"N100|30|68","N100|30|0",tr("Off"),NULL);signal_01_07_offPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_07_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_08_offPushButton=new myQPushButton(tcpServer,"N100|30|70","N100|30|0",tr("Off"),NULL);signal_01_08_offPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_08_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *signal_01_09_offPushButton=new myQPushButton(tcpServer,"N100|30|72","N100|30|0",tr("Off"),NULL);signal_01_09_offPushButton->setMinimumSize(QSize(75,28));autoCycleGroupBoxLayout->addWidget(signal_01_09_offPushButton,r++,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	btnsMainLayout0->addStretch(1);
	//btnsMainLayout1
	//signals group
	QGroupBox *commandsGroupBox=new QGroupBox(tr("Commands"));btnsMainLayout1->addWidget(commandsGroupBox);
	QVBoxLayout *commandsGroupBoxLayout=new QVBoxLayout();commandsGroupBox->setLayout(commandsGroupBoxLayout);
	myQPushButton *command03myQPushButton=new myQPushButton(tcpServer,"N100|30|3","N100|30|0",tr("Return basket from 14"		),NULL);commandsGroupBoxLayout->addWidget(command03myQPushButton,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *command04myQPushButton=new myQPushButton(tcpServer,"N100|30|4","N100|30|0",tr("Send basket from 14 out"	),NULL);commandsGroupBoxLayout->addWidget(command04myQPushButton,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *command05myQPushButton=new myQPushButton(tcpServer,"N100|30|5","N100|30|0",tr("Return basket from 16"		),NULL);commandsGroupBoxLayout->addWidget(command05myQPushButton,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *command06myQPushButton=new myQPushButton(tcpServer,"N100|30|6","N100|30|0",tr("Send basket from 16 out"	),NULL);commandsGroupBoxLayout->addWidget(command06myQPushButton,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *command07myQPushButton=new myQPushButton(tcpServer,"N100|30|7","N100|30|0",tr("Get basket from 28 to 16"	),NULL);commandsGroupBoxLayout->addWidget(command07myQPushButton,Qt::AlignHCenter|Qt::AlignCenter);
	myQPushButton *command08myQPushButton=new myQPushButton(tcpServer,"N100|30|8","N100|30|0",tr("Get basket from 28 to 14"	),NULL);commandsGroupBoxLayout->addWidget(command08myQPushButton,Qt::AlignHCenter|Qt::AlignCenter);
	btnsMainLayout1->addStretch(1);
	//btnsMainLayout2
	QGroupBox *importStationGroupBox=new QGroupBox(tr("Import station"));btnsMainLayout2->addWidget(importStationGroupBox);
	QVBoxLayout *importStationGroupBoxLayout=new QVBoxLayout();importStationGroupBox->setLayout(importStationGroupBoxLayout);
	myQPushButton *command49myQPushButton=new myQPushButton(tcpServer,"N100|30|49","N100|30|0",tr("Return basket from 28"			),				NULL);importStationGroupBoxLayout->addWidget(command49myQPushButton,Qt::AlignHCenter|Qt::AlignCenter);
	
	QGroupBox *cylinderGroupBox=new QGroupBox(tr("Cylinder"));btnsMainLayout2->addWidget(cylinderGroupBox);
	QVBoxLayout *cylinderGroupBoxLayout=new QVBoxLayout();cylinderGroupBox->setLayout(cylinderGroupBoxLayout);
	cylinderUpButton=new myQPushButton(tcpServer,"N100|30|81","N100|30|0",tr("Up"			),				NULL);cylinderGroupBoxLayout->addWidget(cylinderUpButton,Qt::AlignHCenter|Qt::AlignCenter);
	cylinderLabel=new QLabel(tr("  "));cylinderLabel->setMinimumSize(QSize(220,28));		cylinderGroupBoxLayout->addWidget(cylinderLabel,Qt::AlignHCenter|Qt::AlignCenter);
	cylinderDownButton=new myQPushButton(tcpServer,"N100|30|80","N100|30|0",tr("down"			),				NULL);cylinderGroupBoxLayout->addWidget(cylinderDownButton,Qt::AlignHCenter|Qt::AlignCenter);
	btnsMainLayout2->addStretch(1);
	//btnsMainLayout3
	//signals group
	QGroupBox *receivedSignalsGroupBox=new QGroupBox(tr("Receiving signals"));btnsMainLayout3->addWidget(receivedSignalsGroupBox);
	QVBoxLayout *receivedSignalsGroupBoxLayout=new QVBoxLayout();receivedSignalsGroupBox->setLayout(receivedSignalsGroupBoxLayout);
	signal_02_21=new QLabel(tr("    Permission to move conveyor 13	"								));signal_02_21->setMinimumSize(QSize(220,28));		receivedSignalsGroupBoxLayout->addWidget(signal_02_21,Qt::AlignHCenter|Qt::AlignCenter);
	signal_02_22=new QLabel(tr("    Basket delivered at conveyor 13	"								));signal_02_22->setMinimumSize(QSize(220,28));		receivedSignalsGroupBoxLayout->addWidget(signal_02_22,Qt::AlignHCenter|Qt::AlignCenter);
	signal_02_23=new QLabel(tr("    Permission to move conveyor 15	"								));signal_02_23->setMinimumSize(QSize(220,28));		receivedSignalsGroupBoxLayout->addWidget(signal_02_23,Qt::AlignHCenter|Qt::AlignCenter);
	signal_02_24=new QLabel(tr("    Basket delivered at conveyor 15	"								));signal_02_24->setMinimumSize(QSize(220,28));		receivedSignalsGroupBoxLayout->addWidget(signal_02_24,Qt::AlignHCenter|Qt::AlignCenter);
	signal_02_25=new QLabel(tr("    Permission to move conveyor 18	"								));signal_02_25->setMinimumSize(QSize(220,28));		receivedSignalsGroupBoxLayout->addWidget(signal_02_25,Qt::AlignHCenter|Qt::AlignCenter);
	signal_02_26=new QLabel(tr("    Basket delivered at conveyor 18	"								));signal_02_26->setMinimumSize(QSize(220,28));		receivedSignalsGroupBoxLayout->addWidget(signal_02_26,Qt::AlignHCenter|Qt::AlignCenter);
	signal_02_27=new QLabel(tr("    Basket removed from conveyor 18	"								));signal_02_27->setMinimumSize(QSize(220,28));		receivedSignalsGroupBoxLayout->addWidget(signal_02_27,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_28=new QLabel(tr("    Conveyor 18 locked from basket crane"					  ));signal_00_28->setMinimumSize(QSize(220,28));		receivedSignalsGroupBoxLayout->addWidget(signal_00_28,Qt::AlignHCenter|Qt::AlignCenter);
	btnsMainLayout3->addStretch(1);
	//btnsMainLayout4
	QGroupBox *transmittedSignalsGroupBox=new QGroupBox(tr("Transmitting signals"));btnsMainLayout4->addWidget(transmittedSignalsGroupBox);
	QVBoxLayout *transmittedSignalsGroupBoxLayout=new QVBoxLayout();transmittedSignalsGroupBox->setLayout(transmittedSignalsGroupBoxLayout);
	signal_00_16=new QLabel(tr("    Basket demand at 13	"								));signal_00_16->setMinimumSize(QSize(220,28));		transmittedSignalsGroupBoxLayout->addWidget(signal_00_16,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_17=new QLabel(tr("    Permission for basket to 13	"				));signal_00_17->setMinimumSize(QSize(220,28));		transmittedSignalsGroupBoxLayout->addWidget(signal_00_17,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_18=new QLabel(tr("    Basket demand at 15	"								));signal_00_18->setMinimumSize(QSize(220,28));		transmittedSignalsGroupBoxLayout->addWidget(signal_00_18,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_19=new QLabel(tr("    Permission for basket to 15	"				));signal_00_19->setMinimumSize(QSize(220,28));		transmittedSignalsGroupBoxLayout->addWidget(signal_00_19,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_20=new QLabel(tr("    Basket demand at 18	"								));signal_00_20->setMinimumSize(QSize(220,28));		transmittedSignalsGroupBoxLayout->addWidget(signal_00_20,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_21=new QLabel(tr("    Permission for basket to 18	"				));signal_00_21->setMinimumSize(QSize(220,28));		transmittedSignalsGroupBoxLayout->addWidget(signal_00_21,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_22=new QLabel(tr("    Basket in 18	"											));signal_00_22->setMinimumSize(QSize(220,28));		transmittedSignalsGroupBoxLayout->addWidget(signal_00_22,Qt::AlignHCenter|Qt::AlignCenter);
	signal_00_23=new QLabel(tr("    Position 18 is locked	"							));signal_00_23->setMinimumSize(QSize(220,28));		transmittedSignalsGroupBoxLayout->addWidget(signal_00_23,Qt::AlignHCenter|Qt::AlignCenter);
	btnsMainLayout4->addStretch(1);
	//btnsMainLayout5
	QGroupBox *warningSignalsGroupBox=new QGroupBox(tr("Warnings"));btnsMainLayout5->addWidget(warningSignalsGroupBox);
	QVBoxLayout *warningSignalsGroupBoxLayout=new QVBoxLayout();
	signal_05_00=new QLabel(tr("Conveyor C14 not down .PS4 not activated."						));signal_05_00->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_00,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_01=new QLabel(tr("Conveyor C14 not up.PS3 not activated."								));signal_05_01->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_01,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_02=new QLabel(tr("Conveyor C14 not up.PS3 not activated."								));signal_05_02->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_02,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_03=new QLabel(tr("Conveyor C14 not up.PS3 not activated."								));signal_05_03->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_03,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_04=new QLabel(tr("Conveyor C17 not up.PS7 not activated."								));signal_05_04->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_04,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_05=new QLabel(tr("Conveyor C14 not up.PS3 not activated."								));signal_05_05->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_05,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_06=new QLabel(tr("Conveyor C17 not up.PS7 not activated."								));signal_05_06->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_06,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_07=new QLabel(tr("Conveyor C14 not up.PS3 not activated."								));signal_05_07->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_07,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_08=new QLabel(tr("Conveyor C17 not up.PS7 not activated."								));signal_05_08->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_08,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_09=new QLabel(tr("Conveyor C17 not down.PS8 not activated."							));signal_05_09->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_09,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_10=new QLabel(tr("Not permission for position 18."											));signal_05_10->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_10,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_11=new QLabel(tr("Conveyor C17 not down.PS8 not activated."							));signal_05_11->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_11,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_12=new QLabel(tr("Not permission for position 18."											));signal_05_12->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_12,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_13=new QLabel(tr("Conveyor C17 not down.PS8 not activated."							));signal_05_13->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_13,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_14=new QLabel(tr("Conveyor C16 not down.PS9 not activated."							));signal_05_14->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_14,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_15=new QLabel(tr("Conveyor C17 not down.PS8 not activated."							));signal_05_15->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_15,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_16=new QLabel(tr("Conveyor C16 not down.PS9 not activated."							));signal_05_16->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_16,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_17=new QLabel(tr("No permission from basket crane C13."									));signal_05_17->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_17,Qt::AlignHCenter|Qt::AlignCenter);
	signal_05_18=new QLabel(tr("No permission from basket crane C15."									));signal_05_18->setMargin(2);		warningSignalsGroupBoxLayout->addWidget(signal_05_18,Qt::AlignHCenter|Qt::AlignCenter);
	warningSignalsGroupBox->setLayout(warningSignalsGroupBoxLayout);
	btnsMainLayout5->addStretch(1);
	//btnsMainLayout6
	QGroupBox *alarmsSignalsGroupBox=new QGroupBox(tr("Alarms"));btnsMainLayout6->addWidget(alarmsSignalsGroupBox);
	QVBoxLayout *alarmsSignalsGroupBoxLayout=new QVBoxLayout();
	signal_08_00=new QLabel(tr("Basket on conveyor C13."																				));signal_08_00->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_00,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_01=new QLabel(tr("Basket on conveyor C14."																				));signal_08_01->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_01,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_02=new QLabel(tr("Basket on conveyor C28."																				));signal_08_02->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_02,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_03=new QLabel(tr("Basket on conveyor C15."																				));signal_08_03->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_03,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_04=new QLabel(tr("Basket on conveyor C16."																				));signal_08_04->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_04,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_05=new QLabel(tr("Basket on conveyor C19."																				));signal_08_05->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_05,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_06=new QLabel(tr("Basket on conveyor C20."																				));signal_08_06->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_06,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_07=new QLabel(tr("Basket on conveyor C18."																				));signal_08_07->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_07,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_08=new QLabel(tr("Conveyor C14 not on position up/down."													));signal_08_08->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_08,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_09=new QLabel(tr("Conveyor C16 not on position up/down."													));signal_08_09->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_09,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_10=new QLabel(tr("Conveyor C17 not on position up/down."													));signal_08_10->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_10,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_11=new QLabel(tr("Thermal overload Q1,Q11,Q12,\nQ13,Q14,Q2,Q21,Q22,Q23,U1,U2."));signal_08_11->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_11,Qt::AlignHCenter|Qt::AlignCenter);
	signal_08_12=new QLabel(tr("Stop cycle chain conveyors."																		));signal_08_12->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_08_12,Qt::AlignHCenter|Qt::AlignCenter);
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
	QList<int> sizes;sizes<<500<<800;
	mainSplitter->setSizes(sizes);
}
void conveyorsClass::plcMemoryChanged(int index,int value)
{
	QVector<bool> bitsVector=deprecatedCalculateBits(value);
	if (index==0)
	{
		for (int i=0;i<conveyorMovementVector.count();i++)
		{
			for (int j=0;j<bitsVector.count();j++)
			{
				conveyorMovementVector[i]->plcValueChanged(index,j,bitsVector[j]);
			}
		}
		if (bitsVector[0])
		{
			autoFeeding13Pushbutton->setText("Auto feed");
			autoFeeding13Pushbutton->setSendCommandToServerString("N100|30|2");
		}
		else
		{
			autoFeeding13Pushbutton->setText("Manual feed");
			autoFeeding13Pushbutton->setSendCommandToServerString("N100|30|1");
		}
		if (bitsVector[1])
		{
			autoFeeding15Pushbutton->setText("Auto feed");
			autoFeeding15Pushbutton->setSendCommandToServerString("N100|30|10");
		}
		else
		{
			autoFeeding15Pushbutton->setText("Manual feed");
			autoFeeding15Pushbutton->setSendCommandToServerString("N100|30|9");
		}
		if (bitsVector[2])
		{
			autoPushbutton->setText("Auto");
			autoPushbutton->setSendCommandToServerString("N100|30|26");
			hideButtons();
		}
		else
		{
			autoPushbutton->setText("Manual");
			autoPushbutton->setSendCommandToServerString("N100|30|25");
			showButtons();
		}
		if (bitsVector[3])
		{
			stopLabel_0_3->setText("  Active stop");
			stopLabel_0_3->setStyleSheet("QLabel{text-align:center;color:white; border: 4px solid red;border-radius: 4px;}");
		}
		else
		{
			stopLabel_0_3->setText("  No active stop");
			stopLabel_0_3->setStyleSheet("QLabel{text-align:center;color:gray}");
		}
		if (bitsVector[16])signal_00_16->setStyleSheet("QLabel{background-color:green}");else signal_00_16->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[17])signal_00_17->setStyleSheet("QLabel{background-color:green}");else signal_00_17->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[18])signal_00_18->setStyleSheet("QLabel{background-color:green}");else signal_00_18->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[19])signal_00_19->setStyleSheet("QLabel{background-color:green}");else signal_00_19->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[20])signal_00_20->setStyleSheet("QLabel{background-color:green}");else signal_00_20->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[21])signal_00_21->setStyleSheet("QLabel{background-color:green}");else signal_00_21->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[22])signal_00_22->setStyleSheet("QLabel{background-color:green}");else signal_00_22->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[23])signal_00_23->setStyleSheet("QLabel{background-color:green}");else signal_00_23->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[28])signal_00_28->setStyleSheet("QLabel{background-color:green}");else signal_00_28->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[30])command51_52Label->setStyleSheet("QLabel{background-color:green}");else command51_52Label->setStyleSheet("");
		if (bitsVector[31])
		{
			cylinderDown=true;
			cylinderLabel->setText(tr("  Cylinder is down."));
			cylinderLabel->setStyleSheet("QLabel{background-color:lightgray}");
		}
		else
		{
			if (!cylinderUp)
			{
				cylinderLabel->setText(tr("  Cylinder is midway."));
				cylinderLabel->setStyleSheet("QLabel{background-color:red}");
			}
			cylinderDown=false;
		}
	}
	else if (index==1)
	{
		if (bitsVector[ 0])signal_01_00->setStyleSheet("QLabel{background-color:green}");else signal_01_00->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 1])signal_01_01->setStyleSheet("QLabel{background-color:green}");else signal_01_01->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 2])signal_01_02->setStyleSheet("QLabel{background-color:green}");else signal_01_02->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 3])signal_01_03->setStyleSheet("QLabel{background-color:green}");else signal_01_03->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 4])signal_01_04->setStyleSheet("QLabel{background-color:green}");else signal_01_04->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 5])signal_01_05->setStyleSheet("QLabel{background-color:green}");else signal_01_05->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 6])signal_01_06->setStyleSheet("QLabel{background-color:green}");else signal_01_06->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 7])signal_01_07->setStyleSheet("QLabel{background-color:green}");else signal_01_07->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 8])signal_01_08->setStyleSheet("QLabel{background-color:green}");else signal_01_08->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 9])signal_01_09->setStyleSheet("QLabel{background-color:green}");else signal_01_09->setStyleSheet("QLabel{background-color:lightgray}");
	}
	else if (index==2)
	{
		for (int i=0;i<conveyorMovementVector.count();i++)
		{
			for (int j=0;j<bitsVector.count();j++)
			{
				conveyorMovementVector[i]->plcValueChanged(index,j,bitsVector[j]);
			}
		}
		for (int i=0;i<proximitySwitchVector.count();i++)
		{
			for (int j=0;j<bitsVector.count();j++)
			{
				proximitySwitchVector[i]->plcValueChanged(index,j,bitsVector[j]);
			}
		}
		if (bitsVector[21])signal_02_21->setStyleSheet("QLabel{background-color:green}");else signal_02_21->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[22])signal_02_22->setStyleSheet("QLabel{background-color:green}");else signal_02_22->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[23])signal_02_23->setStyleSheet("QLabel{background-color:green}");else signal_02_23->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[24])signal_02_24->setStyleSheet("QLabel{background-color:green}");else signal_02_24->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[25])signal_02_25->setStyleSheet("QLabel{background-color:green}");else signal_02_25->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[26])signal_02_26->setStyleSheet("QLabel{background-color:green}");else signal_02_26->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[27])signal_02_27->setStyleSheet("QLabel{background-color:green}");else signal_02_27->setStyleSheet("QLabel{background-color:lightgray}");
	}
	else if (index==3)
	{
		if (bitsVector[0])
		{
			cylinderUp=true;
			cylinderLabel->setText(tr("  Cylinder is up."));
			cylinderLabel->setStyleSheet("QLabel{background-color:lightgray}");
		}
		else
		{
			cylinderUp=false;
			if (!cylinderDown)
			{
				cylinderLabel->setText(tr("  Cylinder is midway."));
				cylinderLabel->setStyleSheet("QLabel{background-color:red}");
			}
		}
	}
	else if (index==5)
	{
		if (bitsVector[ 0])signal_05_00->setStyleSheet("QLabel{background-color:orange}");else signal_05_00->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 1])signal_05_01->setStyleSheet("QLabel{background-color:orange}");else signal_05_01->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 2])signal_05_02->setStyleSheet("QLabel{background-color:orange}");else signal_05_02->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 3])signal_05_03->setStyleSheet("QLabel{background-color:orange}");else signal_05_03->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 4])signal_05_04->setStyleSheet("QLabel{background-color:orange}");else signal_05_04->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 5])signal_05_05->setStyleSheet("QLabel{background-color:orange}");else signal_05_05->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 6])signal_05_06->setStyleSheet("QLabel{background-color:orange}");else signal_05_06->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 7])signal_05_07->setStyleSheet("QLabel{background-color:orange}");else signal_05_07->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 8])signal_05_08->setStyleSheet("QLabel{background-color:orange}");else signal_05_08->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[ 9])signal_05_09->setStyleSheet("QLabel{background-color:orange}");else signal_05_09->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[10])signal_05_10->setStyleSheet("QLabel{background-color:orange}");else signal_05_10->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[11])signal_05_11->setStyleSheet("QLabel{background-color:orange}");else signal_05_11->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[12])signal_05_12->setStyleSheet("QLabel{background-color:orange}");else signal_05_12->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[13])signal_05_13->setStyleSheet("QLabel{background-color:orange}");else signal_05_13->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[14])signal_05_14->setStyleSheet("QLabel{background-color:orange}");else signal_05_14->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[15])signal_05_15->setStyleSheet("QLabel{background-color:orange}");else signal_05_15->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[16])signal_05_16->setStyleSheet("QLabel{background-color:orange}");else signal_05_16->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[17])signal_05_17->setStyleSheet("QLabel{background-color:orange}");else signal_05_17->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[18])signal_05_18->setStyleSheet("QLabel{background-color:orange}");else signal_05_18->setStyleSheet("QLabel{background-color:lightgray}");
	}
	else if (index==8)
	{
		if (bitsVector[ 0])signal_08_00->setStyleSheet("QLabel{background-color:red}");else signal_08_00->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[ 1])signal_08_01->setStyleSheet("QLabel{background-color:red}");else signal_08_01->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[ 2])signal_08_02->setStyleSheet("QLabel{background-color:red}");else signal_08_02->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[ 3])signal_08_03->setStyleSheet("QLabel{background-color:red}");else signal_08_03->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[ 4])signal_08_04->setStyleSheet("QLabel{background-color:red}");else signal_08_04->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[ 5])signal_08_05->setStyleSheet("QLabel{background-color:red}");else signal_08_05->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[ 6])signal_08_06->setStyleSheet("QLabel{background-color:red}");else signal_08_06->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[ 7])signal_08_07->setStyleSheet("QLabel{background-color:red}");else signal_08_07->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[ 8])signal_08_08->setStyleSheet("QLabel{background-color:red}");else signal_08_08->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[ 9])signal_08_09->setStyleSheet("QLabel{background-color:red}");else signal_08_09->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[10])signal_08_10->setStyleSheet("QLabel{background-color:red}");else signal_08_10->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[11])signal_08_11->setStyleSheet("QLabel{background-color:red}");else signal_08_11->setStyleSheet("QLabel{background-color:green}");
		if (bitsVector[12])signal_08_12->setStyleSheet("QLabel{background-color:red}");else signal_08_12->setStyleSheet("QLabel{background-color:green}");
	}
	else if ((index>=10)&&(index<=21))
	{
		for (int i=0;i<plcMemoryVector.count();i++)
		{
			if (plcMemoryVector[i]->plcValueChanged(index,value))
				break;
		}
	}
}
void conveyorsClass::hideButtons()
{
	cylinderUpButton->setEnabled(false);
	cylinderDownButton->setEnabled(false);
	for (int i=0;i<areaButtonsVector.count();i++)
		areaButtonsVector[i]->hide();
}
void conveyorsClass::showButtons()
{
	cylinderUpButton->setEnabled(true);
	cylinderDownButton->setEnabled(true);
	for (int i=0;i<areaButtonsVector.count();i++)
		areaButtonsVector[i]->show();
}
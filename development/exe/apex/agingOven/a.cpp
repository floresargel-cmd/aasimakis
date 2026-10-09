#include "a.h"//
//george2018.11.20
areaClass::areaClass(tcpServerClass *oldOvenTcpServer_,tcpServerClass *newOvenTcpServer_,QWidget *p):QScrollArea(p)
{
	setAlignment(Qt::AlignCenter);
	setGeometry(0,0,1733,462);
	oldOvenTcpServer=oldOvenTcpServer_;
	newOvenTcpServer=newOvenTcpServer_;
	setBackgroundRole(QPalette::Shadow);
	QWidget *mainWidget=new QWidget(p);
	mainWidget->setMinimumWidth(1733);mainWidget->setMinimumHeight(462);
	setWidget(mainWidget);
	//
	QHBoxLayout *mainLayout=new QHBoxLayout();mainWidget->setLayout(mainLayout);
	QLabel *imageLabel = new QLabel;
	mainLayout->addWidget(imageLabel);
	QImage image(":/ovenLayout.png");
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setAlignment(Qt::AlignCenter);

	//thirtyFeetVector
	thirtyFeetVector<< new thirtyFeetClass(oldOvenTcpServer,33,34,23,16,1600,123,imageLabel);
	thirtyFeetVector<< new thirtyFeetClass(oldOvenTcpServer,35,36,23,17,1600,183,imageLabel);
	thirtyFeetVector<< new thirtyFeetClass(oldOvenTcpServer,37,38,23,18, 510,218,imageLabel);
	//moveConveyorVector
	//conveyorMovmentVector

	QPushButton *createExcelReportPushButton=new QPushButton(QIcon(":/excel.png"),tr(""),imageLabel);
	connect(createExcelReportPushButton,SIGNAL(clicked()),this,SLOT(createOvenLoadExcelReportSlot()));
	createExcelReportPushButton->setIconSize(QSize(28,28));
	createExcelReportPushButton->setGeometry(995,15,32,32);

	ovenTimeLeftLabel=new QLabel("",imageLabel);
	ovenTimeLeftLabel->setGeometry(790,20,150,20);

	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT,  4,tr("Oven load conveyor backwards"           ),  50,105 ,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT, 3,tr("Oven load conveyor forward"             ) , 430,105,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT,  6,tr("Oven conveyor backwards"                ),  630,105,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT, 5,tr("Oven conveyor forward"                  ) ,1010,105,imageLabel);

	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT, 45,tr("Oven load conveyosr and oven conveyors backwards"           ) , 500,100,imageLabel,40,40);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,44,tr("Oven load conveyosr and oven conveyors forward"             ) , 540,100,imageLabel,40,40);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT, 47,tr("Oven oven conveyors and unload conveyors backwards"         ) ,1070,100,imageLabel,40,40);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,46,tr("Oven oven conveyors and unload conveyors backwards"         ) ,1110,100,imageLabel,40,40);

	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,10,tr("Unload conveyor 1 backwards"            ) ,1180, 75,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT, 9,tr("Unload conveyor 1 forward"			         ) ,1335, 75,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,12,tr("Unload conveyor 2 backwards"            ) ,1180,135,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,11,tr("Unload conveyor 2 forward"			         ) ,1335,135,imageLabel);

	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,14,tr("Cross transfer conveyor 1 backwards"    ) ,1460,130,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,13,tr("Cross transfer conveyor 1 forward"			 ) ,1640,130,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,16,tr("Cross transfer conveyor 2 backwards"    ) ,1460,190,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,15,tr("Cross transfer conveyor 2 forward"			 ) ,1640,190,imageLabel);

	///////////////////////////
	//Code added by Vahid
	///////////////////////////
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_LEFT, 61, tr("Outlet conveyor backwards"), 1610, 350, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_RIGHT, 60, tr("Outlet conveyor forward"), 1640, 350, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_LEFT, 63, tr("Outlet conveyor + Cross transfer conveyor 1 backwards"), 1610, 380, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_RIGHT, 62, tr("Outlet conveyor + Cross transfer conveyor 1 forwards"), 1640, 380, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_LEFT, 67, tr("Outlet conveyor + Cross transfer conveyor 2 backwards"), 1610, 410, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_RIGHT, 66, tr("Outlet conveyor + Cross transfer conveyor 2 forwards"), 1640, 410, imageLabel);

	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_LEFT, 69, tr("Agin Oven 2 Inlet conveyors backwards"), 100, 350, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_RIGHT, 68, tr("Agin Oven 2 Inlet conveyors forwards"), 130, 350, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_LEFT, 71, tr("Agin Oven 2 Middle conveyors backwards"), 200, 350, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_RIGHT, 70, tr("Agin Oven 2 Middle conveyors forwards"), 230, 350, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_LEFT, 73, tr("Agin Oven 2 Outlet conveyors backwards"), 300, 350, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_RIGHT, 72, tr("Agin Oven 2 Outlet conveyors forwards"), 330, 350, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_LEFT, 75, tr("Aging Oven 2 Inlet  + Middle conveyors 2 backwards"), 150, 310, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_RIGHT, 74, tr("Aging Oven 2 Inlet  + Middle conveyors 2 forwards"), 180, 310, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_LEFT, 77, tr("Aging Oven 2 Outlet  + Middle conveyors 2 backwards"), 250, 310, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_RIGHT, 76, tr("Aging Oven 2 Outlet  + Middle conveyors 2 forwards"), 280, 310, imageLabel);

	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_LEFT, 79, tr("conveyor 930 backwards"), 400, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_RIGHT, 78, tr("conveyor 930 forward"), 430, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_LEFT, 81, tr("conveyor 940 backwards"), 400, 330, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_RIGHT, 80, tr("conveyor 940 forward"), 430, 330, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_LEFT, 83, tr("conveyor 950 backwards"), 400, 290, imageLabel);
	moveConveyorVector << new moveConveyorClass(oldOvenTcpServer, moveConveyorClass::TYPE_RIGHT, 82, tr("conveyor 950 forward"), 430, 290, imageLabel);
	///////////////////////////


	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_UP   ,29,tr("Cross transfer conveyor to oven"				 ) ,1550,250,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_DOWN ,31,tr("Cross transfer conveyor to return 1"  	 ) ,1550,277,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_DOWN ,30,tr("Cross transfer conveyor to return 2"  	 ) ,1550,304,imageLabel);

	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,17,tr("Returning conveyor 1 forward"           ) ,1180,225,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,18,tr("Returning conveyor 1 backwards"			   ) ,1340,225,imageLabel);

	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,19,tr("Returning conveyor 2 forward"           ) , 910,225,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,20,tr("Returning conveyor 2 backwards"		     ) ,1060,225,imageLabel);

	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,21,tr("Returning conveyor 3 forward"           ) , 650,225,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,22,tr("Returning conveyor 3 backwards"		     ) , 810,225,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,23,tr("Returning conveyor 4 forward"           ) , 380,225,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,24,tr("Returning conveyor 4 backwards"		     ) , 550,225,imageLabel);

	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,43,tr("Returning conveyor 1+2 forward"         ) ,1100,300,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,40,tr("Returning conveyor 1+2 backwards"			 ) ,1160,300,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,42,tr("Returning conveyor 2+3 forward"         ) , 830,300,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,39,tr("Returning conveyor 2+3 backwards"			 ) , 890,300,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,41,tr("Returning conveyor 4+3 forward"         ) , 560,300,imageLabel);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,38,tr("Returning conveyor 4+3 backwards"			 ) , 620,300,imageLabel);

	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,49,tr("UC1+CT1 forward"        ) ,1410,280,imageLabel,40,40);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,50,tr("UC1+CT1 backwards"			 ) ,1450,280,imageLabel,40,40);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,51,tr("UC1+CT1 forward"        ) ,1410,320,imageLabel,40,40);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,52,tr("UC1+CT1 backwards"			 ) ,1450,320,imageLabel,40,40);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,53,tr("UC1+CT1 forward"        ) ,1410,360,imageLabel,40,40);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,54,tr("UC1+CT1 backwards"			 ) ,1450,360,imageLabel,40,40);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_LEFT ,55,tr("UC1+CT1 forward"        ) ,1410,400,imageLabel,40,40);
	moveConveyorVector<<new moveConveyorClass(oldOvenTcpServer,moveConveyorClass::TYPE_RIGHT,56,tr("UC1+CT1 backwards"			 ) ,1450,400,imageLabel,40,40);
	///
	QGroupBox *crossTransferGroupBox=new QGroupBox(tr("Cross transfer"),imageLabel);
	QVBoxLayout *crossTransferGroupBoxLayout=new QVBoxLayout();
	crossTransferGroupBox->setLayout(crossTransferGroupBoxLayout);
	crossTransferGroupBox->setGeometry(1507,0,120,80);

	crossTransferPosLabel=new QLabel("...");crossTransferGroupBoxLayout->addWidget(crossTransferPosLabel,Qt::AlignHCenter|Qt::AlignCenter);
	QPushButton *resetCrossTransferPos=new QPushButton(tr("Reset position"));crossTransferGroupBoxLayout->addWidget(resetCrossTransferPos,Qt::AlignHCenter|Qt::AlignCenter);
	connect(resetCrossTransferPos,SIGNAL(clicked()),this,SLOT(resetCrossTransferPosSlot()));

	//conveyorMovmentVector
	conveyorMovmentVector<<new conveyorMovmentClass(oldOvenTcpServer,33, 0,":/arrowRight.png",33, 1,":/arrowLeft.png" , 230, 95, imageLabel);//Output:Run fwd load conveyor+Output:Run rev load conveyor
	conveyorMovmentVector<<new conveyorMovmentClass(oldOvenTcpServer,33, 2,":/arrowRight.png",33, 3,":/arrowLeft.png" , 790, 95, imageLabel);//Output:Run fwd oven conveyor+Output:Run rev oven conveyor
	conveyorMovmentVector<<new conveyorMovmentClass(oldOvenTcpServer,33, 4,":/arrowRight.png",33, 5,":/arrowLeft.png" ,1260, 75, imageLabel);//Output:Run fwd unload conveyor 1+Output:Run rev unload conveyor 1
	conveyorMovmentVector<<new conveyorMovmentClass(oldOvenTcpServer,33, 6,":/arrowRight.png",33, 7,":/arrowLeft.png" ,1260,135, imageLabel);//Output:Run fwd unload conveyor 2+Output:Run rev unload conveyor 2
	conveyorMovmentVector<<new conveyorMovmentClass(oldOvenTcpServer,33, 8,":/arrowRight.png",33, 9,":/arrowLeft.png" ,1560,135, imageLabel);//Output:Run fwd cross trans conveyor 1+Output:Run rev cross trans conveyor 1
	conveyorMovmentVector<<new conveyorMovmentClass(oldOvenTcpServer,33,10,":/arrowRight.png",33,11,":/arrowLeft.png" ,1560,185, imageLabel);//Output:Run fwd cross trans conveyor 2+Output:Run rev cross trans conveyor 2
	conveyorMovmentVector<<new conveyorMovmentClass(oldOvenTcpServer,33,12,":/arrowLeft.png", 33,13,":/arrowRight.png",1250,225, imageLabel);//Output:Run fwd return conveyor 1+Output:Run rev return conveyor 1
	conveyorMovmentVector<<new conveyorMovmentClass(oldOvenTcpServer,33,14,":/arrowLeft.png", 33,15,":/arrowRight.png", 980,225, imageLabel);//Output:Run fwd return conveyor 2+Output:Run rev return conveyor 2
	conveyorMovmentVector<<new conveyorMovmentClass(oldOvenTcpServer,34, 0,":/arrowLeft.png", 34, 1,":/arrowRight.png", 735,225, imageLabel);//Output:Run fwd return conveyor 3+Output:Run rev return conveyor 3
	conveyorMovmentVector<<new conveyorMovmentClass(oldOvenTcpServer,34, 2,":/arrowLeft.png", 34, 3,":/arrowRight.png", 450,225, imageLabel);//Output:Run fwd return conveyor 4+Output:Run rev return conveyor 4
	conveyorMovmentVector<<new conveyorMovmentClass(oldOvenTcpServer,34, 4,":/arrowDown.png", 34, 5,":/arrowUp.png"   ,1560, 50, imageLabel);//side moving
	//proximitySwitchVector
	proximitySwitchVector<<new proximitySwitchClass(oldOvenTcpServer,32,11,1630, 60,"PS1", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(oldOvenTcpServer,32,10,1630, 80,"PS2", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(oldOvenTcpServer,32,12,1630,220,"PS3", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(oldOvenTcpServer,32,13,1630,250,"PS4", imageLabel);
	proximitySwitchVector<<new proximitySwitchClass(oldOvenTcpServer,32,14,1630,270,"PS5", imageLabel);

	///////////////////////photoSensorsVector
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,30, 0,photoSensorClass::TYPE_DOUBLE,  15, 50,"PH1 ", imageLabel);
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,30, 1,photoSensorClass::TYPE_DOUBLE, 218, 50,"PH2 ", imageLabel);
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,30, 2,photoSensorClass::TYPE_DOUBLE, 495, 50,"PH3 ", imageLabel);

	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,30, 5,photoSensorClass::TYPE_DOUBLE, 580, 50,"PH4 ", imageLabel);
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,30, 6,photoSensorClass::TYPE_DOUBLE, 880, 50,"PH5 ", imageLabel);
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,30, 7,photoSensorClass::TYPE_DOUBLE,1050, 50,"PH6 ", imageLabel);

	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,30, 9,photoSensorClass::TYPE_SINGLE,1160, 53,"PH7 ", imageLabel);
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,30,10,photoSensorClass::TYPE_SINGLE,1370, 53,"PH8 ", imageLabel);
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,30,12,photoSensorClass::TYPE_SINGLE,1165,113,"PH9 ", imageLabel);
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,30,13,photoSensorClass::TYPE_SINGLE,1380,113,"PH10", imageLabel);

	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,31, 1,photoSensorClass::TYPE_SINGLE,1670,103,"PH12", imageLabel);
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,31, 4,photoSensorClass::TYPE_SINGLE,1680,163,"PH14", imageLabel);

	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,31, 7,photoSensorClass::TYPE_SINGLE,1155,203,"PH16", imageLabel);
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,31, 6,photoSensorClass::TYPE_SINGLE,1380,203,"PH15", imageLabel);

	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,32, 3,photoSensorClass::TYPE_SINGLE, 970,203,"PH18", imageLabel);
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,31,10,photoSensorClass::TYPE_SINGLE, 885,203,"PH19", imageLabel);

	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,31,13,photoSensorClass::TYPE_SINGLE, 620,203,"PH21", imageLabel);
	photoSensorsVector<<new photoSensorClass(oldOvenTcpServer,32, 1,photoSensorClass::TYPE_SINGLE, 355,203,"PH24", imageLabel);
	///////////////////////////plcMemoryVector
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer, 1, 85,70 ,tr("24:"    ),tr("oven entrance north west 24 feet"),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer, 9, 85,130,tr("24:"    ),tr("oven entrance south west 24 feet"),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer, 3,375,70 ,tr("24:"    ),tr("oven entrance north east 24 feet"),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,11,375,130,tr("24:"    ),tr("oven entrance south east 24 feet"),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer, 2,220,70 ,tr("30:"    ),tr("oven entrance north 30 feet"     ),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,10,220,130,tr("30:"    ),tr("oven entrance south 30 feet"     ),imageLabel);

	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer, 4,635,70 ,tr("24:"    ),tr("oven north west 24 feet"         ),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,12,635,130,tr("24:"    ),tr("oven south west 24 feet"         ),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer, 6,925,70 ,tr("24:"    ),tr("oven north east 24 feet"         ),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,14,925,130,tr("24:"    ),tr("oven south east 24 feet"         ),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer, 5,770,70 ,tr("30:"    ),tr("oven north 30 feet"              ),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,13,770,130,tr("30:"    ),tr("oven south 30 feet"              ),imageLabel);

	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer, 7,1235,60 ,tr("24:"   ),tr("oven exit north west 24 feet"    ),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,15,1235,120,tr("24:"   ),tr("oven exit south west 24 feet"    ),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer, 8,1520,120,tr("30:"   ),tr("oven exit north east 24/30 feet" ),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,16,1520,175,tr("30:"   ),tr("oven exit south east 24/30 feet" ),imageLabel);

	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,17,1225,210,tr("24:"   ),tr("oven returning 1 24 feet"				),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,18,1090,210,tr("30:"   ),tr("oven returning 1 30 feet"				),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,19, 950,210,tr("24:"   ),tr("oven returning 2 24 feet"				),imageLabel);

	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,20, 710,210,tr("24:"   ),tr("oven returning 3 24 feet"				),imageLabel);
	plcMemoryVector<<new plcMemoryClass(oldOvenTcpServer,22, 430,210,tr("30:"   ),tr("oven returning 4 30 feet"				),imageLabel);

#ifdef newOvenActive
	int newovenYOffset=300;
	int dy=50;
	plcMemoryVector<<new plcMemoryClass(newOvenTcpServer, 1,450,newovenYOffset,     tr("1:"),tr("new oven entrance 1"),imageLabel);
	plcMemoryVector<<new plcMemoryClass(newOvenTcpServer, 2,450,newovenYOffset+dy,  tr("2:"),tr("new oven entrance 2"),imageLabel);
	plcMemoryVector<<new plcMemoryClass(newOvenTcpServer, 3,450,newovenYOffset+2*dy,tr("3:"),tr("new oven entrance 3"),imageLabel);

	plcMemoryVector<<new plcMemoryClass(newOvenTcpServer, 4,450+300,newovenYOffset,     tr("4:"),tr("new oven entrance 4"),imageLabel);
	plcMemoryVector<<new plcMemoryClass(newOvenTcpServer, 5,450+300,newovenYOffset+dy,  tr("5:"),tr("new oven entrance 5"),imageLabel);
	plcMemoryVector<<new plcMemoryClass(newOvenTcpServer, 6,450+300,newovenYOffset+2*dy,tr("6:"),tr("new oven entrance 6"),imageLabel);

	plcMemoryVector<<new plcMemoryClass(newOvenTcpServer, 7,450+800,newovenYOffset,     tr("7:"),tr("new oven entrance 7"),imageLabel);
	plcMemoryVector<<new plcMemoryClass(newOvenTcpServer, 8,450+800,newovenYOffset+dy,  tr("8:"),tr("new oven entrance 8"),imageLabel);
	plcMemoryVector<<new plcMemoryClass(newOvenTcpServer, 9,450+800,newovenYOffset+2*dy,tr("9:"),tr("new oven entrance 9"),imageLabel);
#endif
}
void areaClass::resetCrossTransferPosSlot()
{
	//QMessageBox msgBox(this);
	//msgBox.setWindowTitle(tr("Question"));
	//msgBox.setText(tr("This will reset the cross transfer conveyor."));
	//msgBox.setInformativeText("Do you want to proceed?");
	//msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	//msgBox.setDefaultButton(QMessageBox::Yes);
	//if (msgBox.exec()==QMessageBox::No)
	//	return;
	QString passStr=QInputDialog::getText(this,tr("Password neaded for this operation"),tr("Please input the password"),QLineEdit::Password,"");
	if(checkStrongPass(passStr))
	{
		oldOvenTcpServer->sendString(QString("N11|0|48"));
		//tcpServer->sendString(QString("N11|0|0"));
	}
	else
	{
		if (!passStr.isEmpty())
			QMessageBox::warning(0,tr("Warning"),tr("The password that you typed is incorrect"));
	}
}
void areaClass::setCrossTransferPos(int p)
{
	crossTransferPosLabel->setText(QString("Pos: %1mm").arg(p));
}
//
void areaClass::plcStatusChanged(int index,int value)
{
	QVector<bool> bitsVector=deprecatedCalculateBits(value);
	for (int i=0;i<photoSensorsVector.count();i++)
	{
		for (int bit=0;bit<bitsVector.count();bit++)
		{
			photoSensorsVector[i]->plcValueChanged(index,bit,bitsVector[bit]);
		}
	}
	if (index==32)
	{
		for (int i=0;i<proximitySwitchVector.count();i++)
		{
			for (int bit=0;bit<bitsVector.count();bit++)
			{
				proximitySwitchVector[i]->plcValueChanged(index,bit,bitsVector[bit]);
			}
		}
	}
	if (index==23)
	{
		if (bitsVector[0]==true)//auto mode
			hideButtons();
		else
			showButtons();
	}
	if ((index==33)||(index==34))
	{
		for (int i=0;i<conveyorMovmentVector.count();i++)
		{
			for (int bit=0;bit<bitsVector.count();bit++)
			{
				conveyorMovmentVector[i]->plcValueChanged(index,bit,bitsVector[bit]);
			}
		}
	}
	if (index==23)
	{
		thirtyFeetVector[0]->plcValueChanged(index,16,bitsVector[16]);
		thirtyFeetVector[1]->plcValueChanged(index,17,bitsVector[17]);
		thirtyFeetVector[2]->plcValueChanged(index,18,bitsVector[18]);
	}
	if (index==35)
	{
		int time=value;
		int hours=time/(60);time-=hours*(60);
		int minutes=time;
		QString txt;
		if (hours>0)
			txt+=QString("%1 hours ").arg(hours);
		if (minutes>0)
			txt+=QString("%1 minutes ").arg(minutes);
		if (value>0)
			txt+="left";
		ovenTimeLeftLabel->setText(txt);
	}
	if ((index==36)&&(value==1))
		saveOvenLoadDataToDatabase(1);
}
void areaClass::plcMemoryChanged(QString ip,QString table,int index,int value)
{
	for (int i=0;i<plcMemoryVector.count();i++)
	{
		if (plcMemoryVector[i]->plcValueChanged(ip,index,value))
			break;
	}
	if (ip==newOvenIp)
	{
		if (table=="N12")
		{
			if ((index==36)&&(value==1))
				saveOvenLoadDataToDatabase(2);
		}
	}
}
void areaClass::saveOvenLoadBasketDataToDatabase(int ovenId,int position,int positionIndex)
{
	int bId=GETFIRST(QString("select basketTableId from positions where posNumber=%1 and posIndex=%2").arg(position).arg(positionIndex)).toInt();
	if (bId>0)
	{
		QVector<QVariant> profilesArray=execQuery(QString("select dienum from basket_details where basketnum=%1 and Pc<>0").arg(bId),APEX_DB_CONNECTION);
		QVector<QVariant> sequenceArray=execQuery(QString("select sequence from basket_details where basketnum=%1 and Pc<>0").arg(bId),APEX_DB_CONNECTION);
		QVector<QVariant> SONumArray=execQuery(QString("select SONum from basket_details where basketnum=%1 and Pc<>0").arg(bId),APEX_DB_CONNECTION);
		QVector<QVariant> SOItemNumArray=execQuery(QString("select SOItemNum from basket_details where basketnum=%1 and Pc<>0").arg(bId),APEX_DB_CONNECTION);
		QVector<QVariant> LotArray=execQuery(QString("select Lot from basket_details where basketnum=%1 and Pc<>0").arg(bId),APEX_DB_CONNECTION);
		QVector<QVariant> PcArray=execQuery(QString("select Pc from basket_details where basketnum=%1 and Pc<>0").arg(bId),APEX_DB_CONNECTION);
		QVector<QVariant> PcLenArray=execQuery(QString("select PcLen from basket_details where basketnum=%1 and Pc<>0").arg(bId),APEX_DB_CONNECTION);
		QVector<QVariant> WtArray=execQuery(QString("select Wt from basket_details where basketnum=%1 and Pc<>0").arg(bId),APEX_DB_CONNECTION);
		QVector<QVariant> PcPulledArray=execQuery(QString("select PcPulled from basket_details where basketnum=%1 and Pc<>0").arg(bId),APEX_DB_CONNECTION);
		QVector<QVariant> temperArray=execQuery(QString("select temper from basket_details where basketnum=%1 and Pc<>0").arg(bId),APEX_DB_CONNECTION);
		for (int i=0;i<profilesArray.count();i++)
		{
			QString queryStr="insert into ovenLoadsContents (ovenId,basketId,profile,sequence,SONum,SOItemNum,Lot,pc,PcLen,Wt,PcPulled,temper,position) values(";
			queryStr+=QString("%1,").arg(ovenId);
			queryStr+=QString("%1,").arg(bId);
			queryStr+=QString("%1,").arg(profilesArray[i].toInt());
			queryStr+=QString("%1,").arg(sequenceArray[i].toInt());
			queryStr+=QString("'%1',").arg(SONumArray[i].toString());
			queryStr+=QString("'%1',").arg(SOItemNumArray[i].toString());
			queryStr+=QString("'%1',").arg(LotArray[i].toString());
			queryStr+=QString("%1,").arg(PcArray[i].toInt());
			queryStr+=QString("%1,").arg(PcLenArray[i].toInt());
			queryStr+=QString("%1,").arg(WtArray[i].toReal());
			queryStr+=QString("%1,").arg(PcPulledArray[i].toInt());
			queryStr+=QString("'%1',").arg(temperArray[i].toString());
			queryStr+=QString("%1").arg(position);
			queryStr+=QString(")");
			execQuery(queryStr,MAIN_DB_CONNECTION);
		}
	}
}
void areaClass::saveOvenLoadDataToDatabase(int ovenNumber)
{
	int agingId=GETFIRST("select top 1 ovenId from ovenLoads order by id desc").toInt();
	agingId++;
	execQuery(QString("insert into ovenLoads (ovenId,date,ovenNumber) values (%1,'%2',%3)").arg(agingId).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(ovenNumber),MAIN_DB_CONNECTION);
	if (ovenNumber==1)
	{
		for (int i=600;i<=650;i+=10)
		{
			for (int j=1;j<=4;j++)
				saveOvenLoadBasketDataToDatabase(agingId,i,j);
		}
		oldOvenTcpServer->sendString(QString("N11|36|0"));
		oldOvenTcpServer->sendString(QString("N11|37|%1").arg(agingId+1));
	}
	if (ovenNumber==2)
	{
		for (int i=1010;i<=1030;i+=10)
		{
			for (int j=1;j<=4;j++)
				saveOvenLoadBasketDataToDatabase(agingId,i,j);
		}
		newOvenTcpServer->sendString(QString("N12|36|0"));
		newOvenTcpServer->sendString(QString("N12|37|%1").arg(agingId+1));
	}
	createOvenExcelReport(agingId,false);
}
void areaClass::createOvenExcelReport(int ovenId,bool show)
{
	QString fName=QString("aging#%1.").arg(ovenId)+QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString("_hh.mm.ss")+".csv";
#ifdef inMyLaptop
	QString excelFName=fName;
#else
	//QString excelFName=QString("Z:\\Aging Reports\\%1").arg(fName);
	QString excelFName=QString("C:\\AgingOvenReports\\%1").arg(fName);
#endif
	QFile file(excelFName);
  if (!file.open(QIODevice::Append|QIODevice::Text))
	{
		file.setFileName(QString(".\\reports\\%1").arg(fName));
		if (!file.open(QIODevice::Append|QIODevice::Text))
			return;
	}
	QTextStream out(&file);
	out<<QString("Oven id:%1\n").arg(ovenId);
	out<<tr("basket,")<<tr("position,")<<tr("die number,")<<tr("sequence in the basket,")<<tr("SONum,")<<tr("SOItemNum,")<<tr("Lot,")<<tr("number of pieces,")<<tr("length,")<<tr("Weight,")<<tr("number of pieces pulled,")<<tr("Temper,")<<"\n";
	QVector<QVariant> basketsArray=execQuery(QString("select basketId from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	QVector<QVariant> profileArray=execQuery(QString("select profile from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	QVector<QVariant> sequenceArray=execQuery(QString("select sequence from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	QVector<QVariant> SONumArray=execQuery(QString("select SONum from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	QVector<QVariant> SOItemNumArray=execQuery(QString("select SOItemNum from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	QVector<QVariant> LotArray=execQuery(QString("select Lot from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	QVector<QVariant> pcArray=execQuery(QString("select pc from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	QVector<QVariant> PcLenArray=execQuery(QString("select PcLen from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	QVector<QVariant> WtArray=execQuery(QString("select Wt from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	QVector<QVariant> PcPulledArray=execQuery(QString("select PcPulled from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	QVector<QVariant> temperArray=execQuery(QString("select temper from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	QVector<QVariant> positionArray=execQuery(QString("select position from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId),MAIN_DB_CONNECTION);
	for (int i=0;i<basketsArray.count();i++)
	{
		(out)<<basketsArray[i].toInt()<<",";
		(out)<<positionArray[i].toInt()<<",";
		(out)<<profileArray[i].toInt()<<",";
		(out)<<sequenceArray[i].toInt()<<",";
		(out)<<SONumArray[i].toString().trimmed()<<",";
		(out)<<SOItemNumArray[i].toString().trimmed()<<",";
		(out)<<LotArray[i].toString().trimmed()<<",";
		(out)<<pcArray[i].toInt()<<",";
		(out)<<PcLenArray[i].toInt()<<",";
		(out)<<WtArray[i].toInt()<<",";
		(out)<<PcPulledArray[i].toInt()<<",";
		(out)<<temperArray[i].toString().trimmed();
		(out)<<"\n";
	}
	file.close();
	if (show)
		ShellExecute(0,"open",excelFName.toLatin1(), NULL, NULL, SW_SHOWNORMAL);
}
void areaClass::createOvenLoadExcelReportSlot()
{
	QDialog *dlg=new QDialog(this);
	dlg->setWindowTitle(tr("Create an Excel report"));
	QVBoxLayout *mainLayout=new QVBoxLayout();dlg->setLayout(mainLayout);
	QHBoxLayout *listLayout=new QHBoxLayout();mainLayout->addLayout(listLayout);
	QHBoxLayout *btnsLayout=new QHBoxLayout();mainLayout->addLayout(btnsLayout);
	QListWidget *ovenList=new QListWidget();listLayout->addWidget(ovenList);
	QPushButton *cancelPushButton=new QPushButton(tr("Cancel"));btnsLayout->addWidget(cancelPushButton);
	connect(cancelPushButton,SIGNAL(clicked()),dlg,SLOT(reject()));
	QPushButton *okPushButton=new QPushButton(tr("Ok"));btnsLayout->addWidget(okPushButton);
	connect(okPushButton,SIGNAL(clicked()),dlg,SLOT(accept()));
	//
	ovenList->setSelectionMode(QAbstractItemView::SingleSelection);
	QVector<QVariant> agingIdArray=apexExecQuery("select ovenId from ovenLoads order by id desc");
	QVector<QVariant> agingDatesArray=apexExecQuery("select date from ovenLoads order by id desc");
	for (int i=0;i<agingIdArray.count();i++)
	{
		QListWidgetItem *item=new QListWidgetItem(QString("%1 %2").arg(agingIdArray[i].toInt()).arg(agingDatesArray[i].toString()));
		item->setData(Qt::UserRole,agingIdArray[i]);
		ovenList->addItem(item);
	}
	if (dlg->exec()==QDialog::Accepted)
	{
		QListWidgetItem * current=ovenList->currentItem();
		if (current)
			createOvenExcelReport(current->data(Qt::UserRole).toInt(),true);
	}
}
void areaClass::hideButtons()
{
	for (int i=0;i<moveConveyorVector.count();i++)
		moveConveyorVector[i]->hide();
}
void areaClass::showButtons()
{
	for (int i=0;i<moveConveyorVector.count();i++)
		moveConveyorVector[i]->show();
}

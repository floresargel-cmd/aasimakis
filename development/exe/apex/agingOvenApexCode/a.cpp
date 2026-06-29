#include "a.h"

//george2018.11.20
areaClass::areaClass(tcpServerClass *oven1TCPServer_,tcpServerClass *oven2TCPServer_,QWidget *p):QScrollArea(p)
{
	setAlignment(Qt::AlignCenter);
	setGeometry(0,0,1733,462);

	oven1TCPServer = oven1TCPServer_;
	oven2TCPServer = oven2TCPServer_;

	setBackgroundRole(QPalette::Shadow);
	QWidget *mainWidget = new QWidget(p);
	mainWidget->setMinimumWidth(1733); mainWidget->setMinimumHeight(462);
	setWidget(mainWidget);

	//
	QHBoxLayout *mainLayout=new QHBoxLayout();mainWidget->setLayout(mainLayout);
	QLabel *imageLabel = new QLabel;
	mainLayout->addWidget(imageLabel);
	QImage image(":/ovenLayout.png");
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setAlignment(Qt::AlignCenter);

	//thirtyFeetVector
	thirtyFeetVector << new thirtyFeetClass(oven1TCPServer, 33, 34, 23, 16, 1600, 123, imageLabel);
	thirtyFeetVector << new thirtyFeetClass(oven1TCPServer, 35, 36, 23, 17, 1600, 183, imageLabel);
	thirtyFeetVector << new thirtyFeetClass(oven1TCPServer, 37, 38, 23, 18,  510, 218, imageLabel);

	QPushButton *createExcelReportPushButton=new QPushButton(QIcon(":/excel.png"),tr(""),imageLabel);
	connect(createExcelReportPushButton,SIGNAL(clicked()),this,SLOT(createOvenLoadExcelReportSlot()));
	createExcelReportPushButton->setIconSize(QSize(28,28));
	createExcelReportPushButton->setGeometry(995,15,32,32);

	ovenTimeLeftLabel = new QLabel("", imageLabel);
	ovenTimeLeftLabel->setGeometry(790, 20, 150, 20);

	oven2TimeLeftLabel = new QLabel("", imageLabel);
	oven2TimeLeftLabel->setGeometry(810, 20, 150, 20);

	//moveConveyorVector
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 4, tr("Oven load conveyors backwards"),   50, 105, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer,moveConveyorClass::TYPE_RIGHT, 3, tr("Oven load conveyors forward"  ),  430, 105, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer,moveConveyorClass::TYPE_LEFT,  6, tr("Oven conveyors backwards"     ),  630, 105, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer,moveConveyorClass::TYPE_RIGHT, 5, tr("Oven conveyors forward"       ), 1010, 105, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer,moveConveyorClass::TYPE_LEFT,  45, tr("Oven load conveyors and oven conveyors backwards"  ),  500, 100, imageLabel, 40, 40);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer,moveConveyorClass::TYPE_RIGHT, 44, tr("Oven load conveyors and oven conveyors forward"    ),  540, 100, imageLabel, 40, 40);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer,moveConveyorClass::TYPE_LEFT,  47, tr("Oven oven conveyors and unload conveyors backwards"), 1070, 100, imageLabel, 40, 40);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer,moveConveyorClass::TYPE_RIGHT, 46, tr("Oven oven conveyors and unload conveyors backwards"), 1110, 100, imageLabel, 40, 40);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT,  10, tr("Unload conveyor 1 backwards"),  1180,  75, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT,  9, tr("Unload conveyor 1 forward"	), 1335,  75, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT,  12, tr("Unload conveyor 2 backwards"),  1180, 135, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 11, tr("Unload conveyor 2 forward"	), 1335, 135, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 13, tr("Cross transfer conveyor 1 forward"), 1640, 130, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT,  14, tr("Cross transfer conveyor 1 backwards"), 1460, 130, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 15, tr("Cross transfer conveyor 2 forward"), 1640, 190, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT,  16, tr("Cross transfer conveyor 2 backwards"), 1460, 190, imageLabel);

	///////////////////////////
	//Code added by Vahid
	///////////////////////////
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 68, tr("Aging Oven 2 Inlet conveyors forward"), 360, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 69, tr("Aging Oven 2 Inlet conveyors backward"), 330, 370, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 169, tr("Aging Oven 2 Middle conveyors forward"), 715, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 168, tr("Aging Oven 2 Middle conveyors backward"), 685, 370, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 199, tr("Aging Oven 2 Outlet conveyors forward"), 960, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 198, tr("Aging Oven 2 Outlet conveyors backward"), 930, 370, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 167, tr("Aging Oven 2 Inlet + Middle conveyors forward"), 650, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 166, tr("Aging Oven 2 Inlet + Middle conveyors backward"), 620, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 197, tr("Aging Oven 2 Middle + Outlet conveyors forward"), 895, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 196, tr("Aging Oven 2 Middle + Outlet conveyors backward"), 865, 370, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 82, tr("Oven 2 inlet 1 forward"), 425, 330, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 83, tr("Oven 2 inlet 1 backward"), 395, 330, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 80, tr("Oven 2 inlet 2 forward"), 425, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 81, tr("Oven 2 inlet 2 backward"), 395, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 78, tr("Oven 2 inlet 3 forward"), 425, 410, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 79, tr("Oven 2 inlet 3 backward"), 395, 410, imageLabel);

/*  // middle conveyors do not mo
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 140, tr("Oven 2 middle 4 forward"), 720, 330, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 141, tr("Oven 2 middle 4 backward"), 690, 330, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 150, tr("Oven 2 middle 5 forward"), 720, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 151, tr("Oven 2 middle 5 backward"), 690, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 160, tr("Oven 2 middle 6 forward"), 720, 410, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 161, tr("Oven 2 middle 6 backward"), 690, 410, imageLabel);
*/

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 170, tr("Oven 2 outlet 7 forward"), 1025, 330, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 171, tr("Oven 2 outlet 7 backward"), 995, 330, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 180, tr("Oven 2 outlet 8 forward"), 1025, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 181, tr("Oven 2 outlet 8 backward"),  995, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 190, tr("Oven 2 outlet 9 forward"), 1025, 410, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 191, tr("Oven 2 outlet 9 backward"), 995, 410, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 0, tr("Oven 2 outlet 7 + CT1 forward"), 1215, 330, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 0, tr("Oven 2 outlet 7 + CT1 backward"), 1185, 330, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 0, tr("Oven 2 outlet 9 + CT2 forward"), 1215, 410, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 0, tr("Oven 2 outlet 9  + CT2 backward"), 1185, 410, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 0, tr("Oven 2 outlet 8 + CT2 forward"), 1215, 370, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 0, tr("Oven 2 outlet 8 + CT2 backward"), 1185, 370, imageLabel);
	/*
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 0, tr("Outlet conveyor + Cross transfer conveyor 2 backward"), 1600, 380, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 0, tr("Outlet conveyor + Cross transfer conveyor 2 forward"), 1630, 380, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 0, tr("Outlet conveyor + Cross transfer conveyor 1 backward"), 1600, 410, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 0, tr("Outlet conveyor + Cross transfer conveyor 1 forward"), 1630, 410, imageLabel);
*/
/*
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 0, tr("Outlet conveyors backward"), 1600, 350, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 0, tr("Outlet conveyors forward"), 1630, 350, imageLabel);
*/
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 90, tr("Move full baskets to export"),  1670, 350, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT,  91, tr("Move empty baskets to import"), 1670, 380, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_PAUSE, 89, tr("Pause basket export / import"), 1670, 410, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_UP, 29, tr("Cross transfer conveyor to oven"), 1550, 250, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_DOWN, 31, tr("Cross transfer conveyor to return 1"), 1550, 277, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_DOWN, 30, tr("Cross transfer conveyor to return 2"), 1550, 304, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 17, tr("Returning conveyor 1 forward"), 1180, 225, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 18, tr("Returning conveyor 1 backwards"), 1340, 225, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 19, tr("Returning conveyor 2 forward"), 910, 225, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 20, tr("Returning conveyor 2 backwards"), 1060, 225, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 21, tr("Returning conveyor 3 forward"), 650, 225, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 22, tr("Returning conveyor 3 backwards"), 810, 225, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 23, tr("Returning conveyor 4 forward"), 380, 225, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 24, tr("Returning conveyor 4 backwards"), 550, 225, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 43, tr("Returning conveyor 1+2 forward"), 1100, 295, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 40, tr("Returning conveyor 1+2 backwards"), 1160, 295, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 42, tr("Returning conveyor 2+3 forward"), 830, 295, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 39, tr("Returning conveyor 2+3 backwards"), 890, 295, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 41, tr("Returning conveyor 4+3 forward"), 560, 295, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 38, tr("Returning conveyor 4+3 backwards"), 620, 295, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 49, tr("UC1+CT1 forward"), 1420, 315, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 50, tr("UC1+CT1 backwards"), 1450, 315, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 51, tr("UC2+CT2 forward"), 1420, 345, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 52, tr("UC2+CT2 backwards"), 1450, 345, imageLabel);

	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 53, tr("RT1+CT1 forward"), 1420, 380, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 54, tr("RT1+CT1 backwards"), 1450, 380, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_LEFT, 55, tr("RT1+CT2 forward"), 1420, 410, imageLabel);
	moveConveyorVector << new moveConveyorClass(oven1TCPServer, moveConveyorClass::TYPE_RIGHT, 56, tr("RT1+CT2 backwards"), 1450, 410, imageLabel);

	// cross transfer display
	QGroupBox *crossTransferGroupBox=new QGroupBox(tr("Cross transfer"),imageLabel);
	QVBoxLayout *crossTransferGroupBoxLayout=new QVBoxLayout();
	crossTransferGroupBox->setLayout(crossTransferGroupBoxLayout);
	crossTransferGroupBox->setGeometry(1507,0,120,80);

	crossTransferPosLabel = new QLabel("...");
	crossTransferGroupBoxLayout->addWidget(crossTransferPosLabel, Qt::AlignHCenter | Qt::AlignCenter);

	QPushButton *resetCrossTransferPos = new QPushButton(tr("Reset position"));
	crossTransferGroupBoxLayout->addWidget(resetCrossTransferPos, Qt::AlignHCenter | Qt::AlignCenter);
	connect(resetCrossTransferPos, SIGNAL(clicked()), this, SLOT(resetCrossTransferPosSlot()));

	//conveyorMovmentVector (display conveyor movement)
	conveyorMovmentVector << new conveyorMovmentClass(oven1TCPServer,33, 0, ":/arrowRight.png",33, 1, ":/arrowLeft.png",  230, 95, imageLabel); //Output:Run fwd load conveyor+Output:Run rev load conveyor
	conveyorMovmentVector << new conveyorMovmentClass(oven1TCPServer,33, 2, ":/arrowRight.png",33, 3, ":/arrowLeft.png",  790, 95, imageLabel); //Output:Run fwd oven conveyor+Output:Run rev oven conveyor
	conveyorMovmentVector << new conveyorMovmentClass(oven1TCPServer,33, 4, ":/arrowRight.png",33, 5, ":/arrowLeft.png", 1260, 75, imageLabel); //Output:Run fwd unload conveyor 1+Output:Run rev unload conveyor 1
	conveyorMovmentVector << new conveyorMovmentClass(oven1TCPServer,33, 6, ":/arrowRight.png",33, 7, ":/arrowLeft.png", 1260,135, imageLabel); //Output:Run fwd unload conveyor 2+Output:Run rev unload conveyor 2
	conveyorMovmentVector << new conveyorMovmentClass(oven1TCPServer,33, 8, ":/arrowRight.png",33, 9, ":/arrowLeft.png", 1560,135, imageLabel); //Output:Run fwd cross trans conveyor 1+Output:Run rev cross trans conveyor 1
	conveyorMovmentVector << new conveyorMovmentClass(oven1TCPServer,33,10, ":/arrowRight.png",33,11, ":/arrowLeft.png", 1560,185, imageLabel); //Output:Run fwd cross trans conveyor 2+Output:Run rev cross trans conveyor 2
	conveyorMovmentVector << new conveyorMovmentClass(oven1TCPServer,33,12, ":/arrowLeft.png", 33,13, ":/arrowRight.png",1250,225, imageLabel); //Output:Run fwd return conveyor 1+Output:Run rev return conveyor 1
	conveyorMovmentVector << new conveyorMovmentClass(oven1TCPServer,33,14, ":/arrowLeft.png", 33,15, ":/arrowRight.png", 980,225, imageLabel); //Output:Run fwd return conveyor 2+Output:Run rev return conveyor 2

	conveyorMovmentVector << new conveyorMovmentClass(oven1TCPServer,34, 0, ":/arrowLeft.png", 34, 1, ":/arrowRight.png", 735,225, imageLabel); //Output:Run fwd return conveyor 3+Output:Run rev return conveyor 3
	conveyorMovmentVector << new conveyorMovmentClass(oven1TCPServer,34, 2, ":/arrowLeft.png", 34, 3, ":/arrowRight.png", 450,225, imageLabel); //Output:Run fwd return conveyor 4+Output:Run rev return conveyor 4
	conveyorMovmentVector << new conveyorMovmentClass(oven1TCPServer,34, 4, ":/arrowDown.png", 34, 5, ":/arrowUp.png"   ,1560, 50, imageLabel); //side moving

	// proximitySwitchVector
	proximitySwitchVector << new proximitySwitchClass(oven1TCPServer, 32, 11, 1630, 60, "PS1", imageLabel);
	proximitySwitchVector << new proximitySwitchClass(oven1TCPServer, 32, 10, 1630, 80, "PS2", imageLabel);
	proximitySwitchVector << new proximitySwitchClass(oven1TCPServer, 32, 12, 1630, 220, "PS3", imageLabel);
	proximitySwitchVector << new proximitySwitchClass(oven1TCPServer, 32, 13, 1630, 250, "PS4", imageLabel);
	proximitySwitchVector << new proximitySwitchClass(oven1TCPServer, 32, 14, 1630, 270, "PS5", imageLabel);

	// photoSensorsVector
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 30, 0, photoSensorClass::TYPE_DOUBLE, 15, 50, "PH1 ", imageLabel);
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 30, 1, photoSensorClass::TYPE_DOUBLE, 218, 50, "PH2 ", imageLabel);
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 30, 2, photoSensorClass::TYPE_DOUBLE, 495, 50, "PH3 ", imageLabel);

	photoSensorsVector << new photoSensorClass(oven1TCPServer, 30, 5, photoSensorClass::TYPE_DOUBLE, 580, 50, "PH4 ", imageLabel);
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 30, 6, photoSensorClass::TYPE_DOUBLE, 880, 50, "PH5 ", imageLabel);
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 30, 7, photoSensorClass::TYPE_DOUBLE, 1050, 50, "PH6 ", imageLabel);

	photoSensorsVector << new photoSensorClass(oven1TCPServer, 30, 9, photoSensorClass::TYPE_SINGLE, 1160, 53, "PH7 ", imageLabel);
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 30, 10, photoSensorClass::TYPE_SINGLE, 1370, 53, "PH8 ", imageLabel);
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 30, 12, photoSensorClass::TYPE_SINGLE, 1165, 113, "PH9 ", imageLabel);
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 30, 13, photoSensorClass::TYPE_SINGLE, 1380, 113, "PH10", imageLabel);

	photoSensorsVector << new photoSensorClass(oven1TCPServer, 31, 1, photoSensorClass::TYPE_SINGLE, 1670, 103, "PH12", imageLabel);
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 31, 4, photoSensorClass::TYPE_SINGLE, 1680, 163, "PH14", imageLabel);

	photoSensorsVector << new photoSensorClass(oven1TCPServer, 31, 7, photoSensorClass::TYPE_SINGLE, 1155, 203, "PH16", imageLabel);
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 31, 6, photoSensorClass::TYPE_SINGLE, 1380, 203, "PH15", imageLabel);

	photoSensorsVector << new photoSensorClass(oven1TCPServer, 32, 3, photoSensorClass::TYPE_SINGLE, 970, 203, "PH18", imageLabel);
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 31, 10, photoSensorClass::TYPE_SINGLE, 885, 203, "PH19", imageLabel);

	photoSensorsVector << new photoSensorClass(oven1TCPServer, 31, 13, photoSensorClass::TYPE_SINGLE, 620, 203, "PH21", imageLabel);
	photoSensorsVector << new photoSensorClass(oven1TCPServer, 32, 1, photoSensorClass::TYPE_SINGLE, 355, 203, "PH24", imageLabel);

	// plcMemoryVector (display values at memory locations)
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 1, 85, 70, tr("24:"), tr("oven entrance north west 24 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 9, 85, 130, tr("24:"), tr("oven entrance south west 24 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 3, 375, 70, tr("24:"), tr("oven entrance north east 24 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 11, 375, 130, tr("24:"), tr("oven entrance south east 24 feet"), imageLabel);

	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 2, 220, 70, tr("30:"), tr("oven entrance north 30 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 10, 220, 130, tr("30:"), tr("oven entrance south 30 feet"), imageLabel);

	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 4, 635, 70, tr("24:"), tr("oven north west 24 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 12, 635, 130, tr("24:"), tr("oven south west 24 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 6, 925, 70, tr("24:"), tr("oven north east 24 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 14, 925, 130, tr("24:"), tr("oven south east 24 feet"), imageLabel);

	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 5, 770, 70, tr("30:"), tr("oven north 30 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 13, 770, 130, tr("30:"), tr("oven south 30 feet"), imageLabel);

	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 7, 1235, 60, tr("24:"), tr("oven exit north west 24 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 15, 1235, 120, tr("24:"), tr("oven exit south west 24 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 8, 1520, 120, tr("30:"), tr("oven exit north east 24/30 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 16, 1520, 175, tr("30:"), tr("oven exit south east 24/30 feet"), imageLabel);

	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 17, 1225, 210, tr("24:"), tr("oven returning 1 24 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 18, 1090, 210, tr("30:"), tr("oven returning 1 30 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 19, 950, 210, tr("24:"), tr("oven returning 2 24 feet"), imageLabel);

	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 20, 710, 210, tr("24:"), tr("oven returning 3 24 feet"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 22, 430, 210, tr("30:"), tr("oven returning 4 30 feet"), imageLabel);

	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 39, 5, 410, tr("P2O:"), tr("Stacker 2 Out"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 47, 85, 410, tr("INP2:"), tr("At Stacker 2"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 38, 165, 410, tr("P2I:"), tr("Stacker 2 In"), imageLabel);

	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 27, 1635, 305, tr("Pkg I/O:"), tr("Packaging in / out"), imageLabel);

#ifdef Oven2Active
	int oven2YOffset = 325;
	int oven2XOffset = 450;
	int dy = 40;
	int dx = 300;

	// we have to use the old oven plc as the crane connot see the new oven plc.
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 44, oven2XOffset, oven2YOffset,		   tr("1:"), tr("new oven entrance 1"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 45, oven2XOffset, oven2YOffset + dy,	   tr("2:"), tr("new oven entrance 2"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven1TCPServer, 46, oven2XOffset, oven2YOffset + 2 * dy, tr("3:"), tr("new oven entrance 3"), imageLabel);

	// plcMemoryVector<<new plcMemoryClass(oven2TCPServer, 1,450,newovenYOffset,     tr("1:"),tr("new oven entrance 1"),imageLabel);
	// plcMemoryVector<<new plcMemoryClass(oven2TCPServer, 2,450,newovenYOffset+dy,  tr("2:"),tr("new oven entrance 2"),imageLabel);
	// plcMemoryVector<<new plcMemoryClass(oven2TCPServer, 3,450,newovenYOffset+2*dy,tr("3:"),tr("new oven entrance 3"),imageLabel);

	plcMemoryVector << new plcMemoryClass(oven2TCPServer, 4, oven2XOffset + dx, oven2YOffset,		   tr("4:"), tr("new oven inside 1"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven2TCPServer, 5, oven2XOffset + dx, oven2YOffset + dy,	   tr("5:"), tr("new oven inside 2"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven2TCPServer, 6, oven2XOffset + dx, oven2YOffset + 2 * dy, tr("6:"), tr("new oven inside 3"), imageLabel);

	plcMemoryVector << new plcMemoryClass(oven2TCPServer, 7, oven2XOffset + 2 * dx, oven2YOffset,		   tr("7:"), tr("new oven exit 1"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven2TCPServer, 8, oven2XOffset + 2 * dx, oven2YOffset + dy,	   tr("8:"), tr("new oven exit 2"), imageLabel);
	plcMemoryVector << new plcMemoryClass(oven2TCPServer, 9, oven2XOffset + 2 * dx, oven2YOffset + 2 * dy, tr("9:"), tr("new oven exit 3"), imageLabel);
#endif

	//oven2TimeLeftLabel->setText(QString("oven 1 time"));
	//oven2TimeLeftLabel->setText(QString("oven 2 time"));

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
	QString passStr = QInputDialog::getText(this, tr("Password neaded for this operation"), tr("Please input the password"), QLineEdit::Password, "");
	if (checkStrongPass(passStr))
	{
		oven1TCPServer->sendString(QString("N11|0|48"));
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

void areaClass::plcStatusChanged(int index, int value)
{
	QVector<bool> bitsVector = deprecatedCalculateBits(value);

	for (int i  =0; i < photoSensorsVector.count(); i++)
	{
		for (int bit = 0; bit < bitsVector.count(); bit++)
		{
			photoSensorsVector[i]->plcValueChanged(index, bit, bitsVector[bit]);
		}
	}

	if (index == 32)
	{
		for (int i = 0; i < proximitySwitchVector.count(); i++)
		{
			for (int bit = 0; bit < bitsVector.count(); bit++)
			{
				proximitySwitchVector[i]->plcValueChanged(index, bit, bitsVector[bit]);
			}
		}
	}

	if (index == 23)
	{
		if (bitsVector[0] == true) //auto mode
			hideButtons();
		else
			showButtons();
	}

	if ((index == 33) || (index == 34))
	{
		for (int i=0; i < conveyorMovmentVector.count(); i++)
		{
			for (int bit = 0; bit < bitsVector.count(); bit++)
			{
				conveyorMovmentVector[i]->plcValueChanged(index, bit, bitsVector[bit]);
			}
		}
	}

	if (index == 23)
	{
		thirtyFeetVector[0]->plcValueChanged(index, 16, bitsVector[16]);
		thirtyFeetVector[1]->plcValueChanged(index, 17, bitsVector[17]);
		thirtyFeetVector[2]->plcValueChanged(index, 18, bitsVector[18]);
	}

	if (index == 35)
	{
		//int time = value;
		//int hours = time / (60);
		//time -= hours * (60);

		//int minutes = time;

		QString txt;
		txt = getTimeString(value);
		//if (hours > 0)
		//	txt += QString("%1 hours ").arg(hours);
		//if (minutes > 0)
		//	txt += QString("%1 minutes ").arg(minutes);
		//if (value > 0)
		//	txt += "left";
		ovenTimeLeftLabel->setText(txt);
	}

	if ((index == 36) && (value==1))
		saveOvenLoadDataToDatabase(1);
}

QString areaClass::getTimeString(int timeValue) 
{
	int time = timeValue;
	int hours = time / (60);
	time -= hours * (60);

	int minutes = time;

	QString txt;
	if (hours > 0)
		txt += QString("%1 hours ").arg(hours);
	if (minutes > 0)
		txt += QString("%1 minutes ").arg(minutes);
	if (timeValue > 0)
		txt += "left";

	return txt;
}

void areaClass::plcMemoryChanged(QString ip, QString table, int index, int value)
{
	for (int i = 0; i < plcMemoryVector.count(); i++)
	{
		// int myIndex = 
		if (plcMemoryVector[i]->plcValueChanged(ip, index, value))
			break;
	}

	if (ip == newOvenIp)
	{
		if (table == "N12")
		{
			if (index == 35)
				oven2TimeLeftLabel->setText(getTimeString(value));

			if ((index == 36) && (value == 1))
				saveOvenLoadDataToDatabase(2);
		}
	}
}

void areaClass::saveOvenLoadBasketDataToDatabase(int ovenId, int position, int positionIndex)
{
	int bId = GETFIRST(QString("select basketTableId from positions where posNumber = %1 and posIndex = %2").arg(position).arg(positionIndex)).toInt();
	if (bId > 0)
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
		oven1TCPServer->sendString(QString("N11|36|0"));
		oven1TCPServer->sendString(QString("N11|37|%1").arg(agingId+1));
	}
	if (ovenNumber==2)
	{
		for (int i=1010;i<=1030;i+=10)
		{
			for (int j=1;j<=4;j++)
				saveOvenLoadBasketDataToDatabase(agingId,i,j);
		}
		oven2TCPServer->sendString(QString("N12|36|0"));
		oven2TCPServer->sendString(QString("N12|37|%1").arg(agingId+1));
	}
	createOvenExcelReport(agingId,false);
}
void areaClass::createOvenExcelReport(int ovenId,bool show)
{
	QString fName = QString("aging#%1.").arg(ovenId) + QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString("_hh.mm.ss") + ".csv";
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
		if (!file.open(QIODevice::Append | QIODevice::Text))
			return;
	}
	QTextStream out(&file);

	out << QString("Oven id:%1\n").arg(ovenId);
	out<<tr("basket,")<<tr("position,")<<tr("die number,")<<tr("sequence in the basket,")<<tr("SONum,")<<tr("SOItemNum,")<<tr("Lot,")<<tr("number of pieces,")<<tr("length,")<<tr("Weight,")<<tr("number of pieces pulled,")<<tr("Temper,")<<"\n";

	QVector<QVariant> basketsArray = execQuery(QString("select basketId from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);
	QVector<QVariant> profileArray = execQuery(QString("select profile from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);
	QVector<QVariant> sequenceArray = execQuery(QString("select sequence from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);
	QVector<QVariant> SONumArray = execQuery(QString("select SONum from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);
	QVector<QVariant> SOItemNumArray = execQuery(QString("select SOItemNum from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);
	QVector<QVariant> LotArray = execQuery(QString("select Lot from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);
	QVector<QVariant> pcArray = execQuery(QString("select pc from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);
	QVector<QVariant> PcLenArray = execQuery(QString("select PcLen from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);
	QVector<QVariant> WtArray = execQuery(QString("select Wt from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);
	QVector<QVariant> PcPulledArray = execQuery(QString("select PcPulled from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);
	QVector<QVariant> temperArray = execQuery(QString("select temper from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);
	QVector<QVariant> positionArray = execQuery(QString("select position from ovenLoadsContents where ovenId=%1 order by basketId").arg(ovenId), MAIN_DB_CONNECTION);

	for (int i = 0; i < basketsArray.count(); i++)
	{
		(out) << basketsArray[i].toInt() << ",";
		(out) << positionArray[i].toInt() << ",";
		(out) << profileArray[i].toInt() << ",";
		(out) << sequenceArray[i].toInt() << ",";
		(out) << SONumArray[i].toString().trimmed() << ",";
		(out) << SOItemNumArray[i].toString().trimmed() << ",";
		(out) << LotArray[i].toString().trimmed() << ",";
		(out) << pcArray[i].toInt() << ",";
		(out) << PcLenArray[i].toInt() << ",";
		(out) << WtArray[i].toInt() << ",";
		(out) << PcPulledArray[i].toInt() << ",";
		(out) << temperArray[i].toString().trimmed();
		(out) << "\n";
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
	for (int i = 0; i < moveConveyorVector.count(); i++)
		moveConveyorVector[i]->hide();
}

void areaClass::showButtons()
{
	for (int i = 0; i < moveConveyorVector.count(); i++)
		moveConveyorVector[i]->show();
}

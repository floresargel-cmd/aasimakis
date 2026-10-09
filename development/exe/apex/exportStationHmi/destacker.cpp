#include "destacker.h"
destackerClass::destackerClass(tcpServerClass *tcpServer_,QWidget *p):QWidget(p)
{
	//this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px}  QCheckBox{font-size:%1px}").arg(12));
	tcpServer=tcpServer_;
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	QSplitter *mainSplitter=new QSplitter(Qt::Vertical,this);mainLayout->addWidget(mainSplitter);
	/////////////////////////////////////////////////////////////////////////area
	QImage image(":/destackerLayout.png");
	QLabel *imageLabel = new QLabel;
	imageLabel->setMinimumWidth(1750);
	imageLabel->setMinimumHeight(702);
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
	//autoPushbutton=new myQPushButton(tcpServer,"","N100|30|0",tr(""),imageLabel);
	//autoPushbutton->setGeometry(0,460,100,30);
	//stopLabel_0_3=new QLabel("",imageLabel);
	//stopLabel_0_3->setGeometry(105,460,100,30);
	//myQPushButton *resetAlarmsPushButton=new myQPushButton(tcpServer,"N100|30|639","N100|30|0",tr("Reset Alarms"),imageLabel);
	//resetAlarmsPushButton->setGeometry(210,460,100,30);

	int X,Y;
	X=1500;Y=400;//1
  signal_25_08=new 	QLabel("Photo sensors activated",imageLabel);																								signal_25_08  ->setGeometry(X,Y+20,150,20);
	signal_25_08->setStyleSheet(QString("QLabel{font-size:16;color:red}"));

	X=1600;Y=553;//1
	myQPushButton *signal_25_00PD=new myQPushButton(tcpServer,"N100|30|200","N100|30|0",tr("Disable"),imageLabel);signal_25_00PD->setGeometry(X,Y   ,75,20);
	signal_25_00=new QLabel("",imageLabel);																																				signal_25_00  ->setGeometry(X,Y+20,75,20);
	myQPushButton *signal_25_00PE=new myQPushButton(tcpServer,"N100|30|201","N100|30|0",tr("Enable"),imageLabel); signal_25_00PE->setGeometry(X,Y+40,75,20);
	signal_25_15=new QLabel("",imageLabel);																																				signal_25_15  ->setGeometry(X,Y+80,75,20);

	X=1355;Y=553;//2
	myQPushButton *signal_25_01PD=new myQPushButton(tcpServer,"N100|30|202","N100|30|0",tr("Disable"),imageLabel);signal_25_01PD->setGeometry(X,Y   ,75,20);
	signal_25_01=new QLabel("",imageLabel);																																				signal_25_01  ->setGeometry(X,Y+20,75,20);
	myQPushButton *signal_25_01PE=new myQPushButton(tcpServer,"N100|30|203","N100|30|0",tr("Enable"),imageLabel); signal_25_01PE->setGeometry(X,Y+40,75,20);
	signal_25_16=new QLabel("",imageLabel);	    																																	signal_25_16  ->setGeometry(X,Y+80,75,20);

	X=1140;Y=553;//3
	myQPushButton *signal_25_02PD=new myQPushButton(tcpServer,"N100|30|204","N100|30|0",tr("Disable"),imageLabel);signal_25_02PD->setGeometry(X,Y   ,75,20);
	signal_25_02=new QLabel("",imageLabel);																																				signal_25_02  ->setGeometry(X,Y+20,75,20);
	myQPushButton *signal_25_02PE=new myQPushButton(tcpServer,"N100|30|205","N100|30|0",tr("Enable"),imageLabel); signal_25_02PE->setGeometry(X,Y+40,75,20);
	signal_25_17=new QLabel("",imageLabel);	  																																		signal_25_17  ->setGeometry(X,Y+80,75,20);

	X=960;Y=553;//4
	myQPushButton *signal_25_03PD=new myQPushButton(tcpServer,"N100|30|206","N100|30|0",tr("Disable"),imageLabel);signal_25_03PD->setGeometry(X,Y   ,75,20);
	signal_25_03=new QLabel("",imageLabel);																																				signal_25_03  ->setGeometry(X,Y+20,75,20);
	myQPushButton *signal_25_03PE=new myQPushButton(tcpServer,"N100|30|207","N100|30|0",tr("Enable"),imageLabel); signal_25_03PE->setGeometry(X,Y+40,75,20);
	signal_25_18=new QLabel("",imageLabel);																																				signal_25_18  ->setGeometry(X,Y+80,75,20);

	X=780;Y=553;//5
	myQPushButton *signal_25_04PD=new myQPushButton(tcpServer,"N100|30|208","N100|30|0",tr("Disable"),imageLabel);signal_25_04PD->setGeometry(X,Y   ,75,20);
	signal_25_04=new QLabel("",imageLabel);																																				signal_25_04  ->setGeometry(X,Y+20,75,20);
	myQPushButton *signal_25_04PE=new myQPushButton(tcpServer,"N100|30|209","N100|30|0",tr("Enable"),imageLabel); signal_25_04PE->setGeometry(X,Y+40,75,20);
	signal_25_19=new QLabel("",imageLabel);																																				signal_25_19  ->setGeometry(X,Y+80,75,20);

	X=580;Y=553;//6
	myQPushButton *signal_25_05PD=new myQPushButton(tcpServer,"N100|30|210","N100|30|0",tr("Disable"),imageLabel);signal_25_05PD->setGeometry(X,Y   ,75,20);
	signal_25_05=new QLabel("",imageLabel);																																				signal_25_05  ->setGeometry(X,Y+20,75,20);
	myQPushButton *signal_25_05PE=new myQPushButton(tcpServer,"N100|30|211","N100|30|0",tr("Enable"),imageLabel); signal_25_05PE->setGeometry(X,Y+40,75,20);
	signal_25_20=new QLabel("",imageLabel);																																				signal_25_20  ->setGeometry(X,Y+80,75,20);

	X=380;Y=553;//7
	myQPushButton *signal_25_06PD=new myQPushButton(tcpServer,"N100|30|212","N100|30|0",tr("Disable"),imageLabel);signal_25_06PD->setGeometry(X,Y   ,75,20);
	signal_25_06=new QLabel("",imageLabel);																																				signal_25_06  ->setGeometry(X,Y+20,75,20);
	myQPushButton *signal_25_06PE=new myQPushButton(tcpServer,"N100|30|213","N100|30|0",tr("Enable"),imageLabel); signal_25_06PE->setGeometry(X,Y+40,75,20);
	signal_25_21=new QLabel("",imageLabel);																																				signal_25_21  ->setGeometry(X,Y+80,75,20);

	X=130;Y=553;//8
	myQPushButton *signal_25_07PD=new myQPushButton(tcpServer,"N100|30|214","N100|30|0",tr("Disable"),imageLabel);signal_25_07PD->setGeometry(X,Y   ,75,20);
	signal_25_07=new QLabel("",imageLabel);																																				signal_25_07  ->setGeometry(X,Y+20,75,20);
	myQPushButton *signal_25_07PE=new myQPushButton(tcpServer,"N100|30|215","N100|30|0",tr("Enable"),imageLabel); signal_25_07PE->setGeometry(X,Y+40,75,20);
	signal_25_22=new QLabel("",imageLabel);																																				signal_25_22  ->setGeometry(X,Y+80,75,20);
	//
	mainSplitter->addWidget(area);
	/////////////////////////////////////////////////////////////////////////buttons
	QWidget *btnsWidget=new QWidget();
	QHBoxLayout *btnsLayout=new QHBoxLayout();btnsWidget->setLayout(btnsLayout);
	QVBoxLayout *btnsMainLayout0=new QVBoxLayout();btnsLayout->addLayout(btnsMainLayout0);
	QVBoxLayout *btnsMainLayout1=new QVBoxLayout();btnsLayout->addLayout(btnsMainLayout1);
	QVBoxLayout *btnsMainLayout2=new QVBoxLayout();btnsLayout->addLayout(btnsMainLayout2);
	QVBoxLayout *btnsMainLayout3=new QVBoxLayout();btnsLayout->addLayout(btnsMainLayout3);
	//btnsMainLayout0
	QGroupBox *spacersGroupBox=new QGroupBox(tr("Spacers cycle"));btnsMainLayout0->addWidget(spacersGroupBox);
	QGridLayout *spacersGroupBoxLayout=new QGridLayout();spacersGroupBox->setLayout(spacersGroupBoxLayout);

	spacersGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N100|30|220","N100|30|0",tr("On"),NULL),0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	spacersGroupBoxLayout->addWidget(signal_28_23=new QLabel(tr("Long belts")),0,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);signal_28_23->setMargin(5);
	spacersGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N100|30|221","N100|30|0",tr("Off"),NULL),0,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);

	spacersGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N100|30|222","N100|30|0",tr("On"),NULL),1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	spacersGroupBoxLayout->addWidget(signal_28_24=new QLabel(tr("Short belts")),1,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);signal_28_24->setMargin(5);
	spacersGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N100|30|223","N100|30|0",tr("Off"),NULL),1,2,1,1,Qt::AlignHCenter|Qt::AlignCenter);

	btnsMainLayout0->addStretch(1);
	//btnsMainLayout1
	QGroupBox *autoGroupBox=new QGroupBox(tr("Auto cycle"));btnsMainLayout1->addWidget(autoGroupBox);
	QGridLayout *autoGroupBoxLayout=new QGridLayout();autoGroupBox->setLayout(autoGroupBoxLayout);

	autoGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N100|30|225","N100|30|0",tr("On"),NULL),0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	autoGroupBoxLayout->addWidget(signal_25_25=new QLabel(tr("On hooks")),0,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);signal_25_25->setMargin(5);

	autoGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N100|30|226","N100|30|0",tr("On"),NULL),1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	autoGroupBoxLayout->addWidget(signal_25_26=new QLabel(tr("Out of hooks")),1,1,1,1,Qt::AlignHCenter|Qt::AlignCenter);signal_25_26->setMargin(5);
	btnsMainLayout1->addStretch(1);
	//btnsMainLayout2
	QGroupBox *alarmsSignalsGroupBox=new QGroupBox(tr("Alarms"));btnsMainLayout2->addWidget(alarmsSignalsGroupBox);
	QVBoxLayout *alarmsSignalsGroupBoxLayout=new QVBoxLayout();
	signal_28_00=new QLabel(tr("Vertical  motion underspeed." ))	;signal_28_00->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_28_00,Qt::AlignHCenter|Qt::AlignCenter);
	signal_28_01=new QLabel(tr("Horizontal motion underspeed." ))	;signal_28_01->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_28_01,Qt::AlignHCenter|Qt::AlignCenter);
	signal_28_02=new QLabel(tr("Pneumatic valve feedback." ))			;signal_28_02->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_28_02,Qt::AlignHCenter|Qt::AlignCenter);
	signal_28_03=new QLabel(tr("Thermal overload Q3,U3." ))				;signal_28_03->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_28_03,Qt::AlignHCenter|Qt::AlignCenter);
	signal_28_04=new QLabel(tr("Thermal overload." ))							;signal_28_04->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_28_04,Qt::AlignHCenter|Qt::AlignCenter);

	signal_28_06=new QLabel(tr("Vertical  motion  overspeed." ))	;signal_28_06->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_28_06,Qt::AlignHCenter|Qt::AlignCenter);
	signal_28_07=new QLabel(tr("Horizontal motion overspeed." ))	;signal_28_07->setMargin(5);		alarmsSignalsGroupBoxLayout->addWidget(signal_28_07,Qt::AlignHCenter|Qt::AlignCenter);
	alarmsSignalsGroupBox->setLayout(alarmsSignalsGroupBoxLayout);
	btnsMainLayout2->addStretch(1);
	//
	//btnsMainLayout3
	spacers=new spacersClass(tcpServer,this);
	btnsMainLayout3->addWidget(spacers);
	//btnsMainLayout3->addStretch(1);
	//
	QScrollArea *btnsArea=new QScrollArea();
	btnsArea->setWidget(btnsWidget);
	mainSplitter->addWidget(btnsArea);

	QList<int> sizes;sizes<<800<<300;
	mainSplitter->setSizes(sizes);
}
void destackerClass::plcMemoryChanged(int index,int value)
{
	if (index==25)
	{
		if (HAVECOMMONBIT(value,BITS[ 0]))signal_25_00->setText("Disabled");else signal_25_00->setText("Enabled");
		if (HAVECOMMONBIT(value,BITS[ 1]))signal_25_01->setText("Disabled");else signal_25_01->setText("Enabled");
		if (HAVECOMMONBIT(value,BITS[ 2]))signal_25_02->setText("Disabled");else signal_25_02->setText("Enabled");
		if (HAVECOMMONBIT(value,BITS[ 3]))signal_25_03->setText("Disabled");else signal_25_03->setText("Enabled");
		if (HAVECOMMONBIT(value,BITS[ 4]))signal_25_04->setText("Disabled");else signal_25_04->setText("Enabled");
		if (HAVECOMMONBIT(value,BITS[ 5]))signal_25_05->setText("Disabled");else signal_25_05->setText("Enabled");
		if (HAVECOMMONBIT(value,BITS[ 6]))signal_25_06->setText("Disabled");else signal_25_06->setText("Enabled");
		if (HAVECOMMONBIT(value,BITS[ 7]))signal_25_07->setText("Disabled");else signal_25_07->setText("Enabled");
		if (HAVECOMMONBIT(value,BITS[ 8]))
			signal_25_08->show();
		else
			signal_25_08->hide();

		if (HAVECOMMONBIT(value,BITS[15]))signal_25_15->setText("Grabing");else signal_25_15->setText("Not grabing");
		if (HAVECOMMONBIT(value,BITS[16]))signal_25_16->setText("Grabing");else signal_25_16->setText("Not grabing");
		if (HAVECOMMONBIT(value,BITS[17]))signal_25_17->setText("Grabing");else signal_25_17->setText("Not grabing");
		if (HAVECOMMONBIT(value,BITS[18]))signal_25_18->setText("Grabing");else signal_25_18->setText("Not grabing");
		if (HAVECOMMONBIT(value,BITS[19]))signal_25_19->setText("Grabing");else signal_25_19->setText("Not grabing");
		if (HAVECOMMONBIT(value,BITS[20]))signal_25_20->setText("Grabing");else signal_25_20->setText("Not grabing");
		if (HAVECOMMONBIT(value,BITS[21]))signal_25_21->setText("Grabing");else signal_25_21->setText("Not grabing");
		if (HAVECOMMONBIT(value,BITS[22]))signal_25_22->setText("Grabing");else signal_25_22->setText("Not grabing");

		if (HAVECOMMONBIT(value,BITS[23]))signal_28_23->setStyleSheet("QLabel{background-color:green}");else signal_28_23->setStyleSheet("QLabel{background-color:lightgray}");
		if (HAVECOMMONBIT(value,BITS[24]))signal_28_24->setStyleSheet("QLabel{background-color:green}");else signal_28_24->setStyleSheet("QLabel{background-color:lightgray}");

		if (HAVECOMMONBIT(value,BITS[25]))signal_25_26->setStyleSheet("QLabel{background-color:green}");else signal_25_25->setStyleSheet("QLabel{background-color:lightgray}");
		if (HAVECOMMONBIT(value,BITS[26]))signal_25_26->setStyleSheet("QLabel{background-color:green}");else signal_25_26->setStyleSheet("QLabel{background-color:lightgray}");
		spacers->setAutoStatus(HAVECOMMONBIT(value,BITS[28]));
		spacers->setOkToSendData(HAVECOMMONBIT(value,BITS[29]));
	}
	else if (index==28)
	{
		if (HAVECOMMONBIT(value,BITS[ 0]))signal_28_00->setStyleSheet("QLabel{background-color:red}");else signal_28_00->setStyleSheet("QLabel{background-color:lightgray}");
		if (HAVECOMMONBIT(value,BITS[ 1]))signal_28_01->setStyleSheet("QLabel{background-color:red}");else signal_28_01->setStyleSheet("QLabel{background-color:lightgray}");
		if (HAVECOMMONBIT(value,BITS[ 2]))signal_28_02->setStyleSheet("QLabel{background-color:red}");else signal_28_02->setStyleSheet("QLabel{background-color:lightgray}");
		if (HAVECOMMONBIT(value,BITS[ 3]))signal_28_03->setStyleSheet("QLabel{background-color:red}");else signal_28_03->setStyleSheet("QLabel{background-color:lightgray}");
		if (HAVECOMMONBIT(value,BITS[ 4]))signal_28_04->setStyleSheet("QLabel{background-color:red}");else signal_28_04->setStyleSheet("QLabel{background-color:lightgray}");

		if (HAVECOMMONBIT(value,BITS[ 6]))signal_28_06->setStyleSheet("QLabel{background-color:red}");else signal_28_06->setStyleSheet("QLabel{background-color:lightgray}");
		if (HAVECOMMONBIT(value,BITS[ 7]))signal_28_07->setStyleSheet("QLabel{background-color:red}");else signal_28_07->setStyleSheet("QLabel{background-color:lightgray}");
	}
}
void destackerClass::destackerBasketChanged(int value)
{
	spacers->setBasket(value);
}
void destackerClass::spacersMemoryChanged(int index,int value)//N163
{
	spacers->spacerMemoryChanged(index,value);
}

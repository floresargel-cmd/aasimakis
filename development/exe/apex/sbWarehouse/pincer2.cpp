#include "pch.h"
#include "pincer2.h"
pincer2Class::pincer2Class(allPossClass *allPos_,tuxipClass *tuxip_,QWidget *p):QWidget(p)
{
	allPos=allPos_;
	tuxip=tuxip_;
	pcStatus=plcStatus=plcFrom=plcTo=plcCraneBasket=0;
	readyForNewMission=false;
	plcX=plcZ=0.f;
	//
	//
	QVBoxLayout *topLayout=new QVBoxLayout();topLayout->setMargin(0);setLayout(topLayout);
	QScrollArea *topScrollArea=new QScrollArea();
	topLayout->addWidget(topScrollArea);
	topScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	QWidget *topScrollAreaWidget=new QWidget();topScrollArea->setWidget(topScrollAreaWidget);
	topScrollArea->setWidgetResizable(true);
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);topScrollAreaWidget->setLayout(mainLayout);
	QSplitter *mainSplitter=new QSplitter(Qt::Vertical,this);mainLayout->addWidget(mainSplitter);
	mainSplitter->setStyleSheet("QSplitter::handle:vertical{height:3px;background-color:#777777;} QSplitterHandle:hover{} QSplitter::handle:vertical:hover{height:3px;background-color:#339966;}");
	QWidget *mainSplitterUpW=new QWidget();mainSplitter->addWidget(mainSplitterUpW);
	QWidget *mainSplitterDownW=new QWidget();mainSplitter->addWidget(mainSplitterDownW);
	QHBoxLayout *upLayout=new QHBoxLayout();upLayout->setMargin(0);mainSplitterUpW->setLayout(upLayout);
	QHBoxLayout *downLayout=new QHBoxLayout();downLayout->setMargin(0);mainSplitterDownW->setLayout(downLayout);
	mainSplitter->setSizes(QList<int>()<<750<<200);
	QSplitter *upSplitter=new QSplitter(Qt::Horizontal,mainSplitter);upLayout->addWidget(upSplitter);
	upSplitter->setStyleSheet("QSplitter::handle:horizontal{height:3px;background-color:#777777;} QSplitterHandle:hover{} QSplitter::handle:horizontal:hover{height:3px;background-color:#339966;}");
	QWidget *upSplitterW0=new QWidget();upSplitter->addWidget(upSplitterW0);QVBoxLayout *upSplitterW0L=new QVBoxLayout();upSplitterW0->setLayout(upSplitterW0L);
	QWidget *upSplitterW1=new QWidget();upSplitter->addWidget(upSplitterW1);QVBoxLayout *upSplitterW1L=new QVBoxLayout();upSplitterW1->setLayout(upSplitterW1L);upSplitterW1L->setMargin(2);
	QWidget *upSplitterW2=new QWidget();upSplitter->addWidget(upSplitterW2);QVBoxLayout *upSplitterW2L=new QVBoxLayout();upSplitterW2->setLayout(upSplitterW2L);
	upSplitter->setSizes(QList<int>()<<300<<800<<300);
	//upSplitterW0L
	QGroupBox *upDownGroupBox=new QGroupBox(tr("UP-down"));upSplitterW0L->addWidget(upDownGroupBox);
	QVBoxLayout *upDownGroupBoxL=new QVBoxLayout();upDownGroupBox->setLayout(upDownGroupBoxL);
	QGridLayout *upDownGroupBox1L=new QGridLayout();upDownGroupBoxL->addLayout(upDownGroupBox1L);
	QGridLayout *upDownGroupBox2L=new QGridLayout();upDownGroupBoxL->addLayout(upDownGroupBox2L);
	int row=0;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Up"),":/upArrow.png",tuxip,"PC2_PCcomDint",5,3,0,"","PC2_PCcomDint",6,23,"","","","","",buttonStyle,buttonStyleActive,tr("Up"),this);upDownGroupBox1L->addWidget(pushButtonsVector.last(),row,0);
	bitVector<<new bitClass("PC2_PCcomDint",6,23,bitClass::TYPE_CHANGE_COLOR,tr("moving up"),tr("..."),styleLabelG,styleLabelGr,"","","","",this);upDownGroupBox1L->addWidget(bitVector.last(),row,1);
	bitVector.last()->setMinimumWidth(200);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Down"),":/downArrow.png",tuxip,"PC2_PCcomDint",5,4,0,"","PC2_PCcomDint",6,24,"","","","","",buttonStyle,buttonStyleActive,tr("Down"),this);upDownGroupBox1L->addWidget(pushButtonsVector.last(),row,0);
	bitVector<<new bitClass("PC2_PCcomDint",6,24,bitClass::TYPE_CHANGE_COLOR,tr("moving down"),tr("..."),styleLabelG,styleLabelGr,"","bit move down","","",this);upDownGroupBox1L->addWidget(bitVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset"),":/reset.png",tuxip,"PC2_PCcomDint",5,18,0,"","PC2_PCcomDint",6,15,"","","","","",buttonStyle,buttonStyleActive,tr("Reset"),this);upDownGroupBox1L->addWidget(pushButtonsVector.last(),row,0);
	bitVector<<new bitClass("PC2_PCcomDint",6,15,bitClass::TYPE_CHANGE_COLOR,tr("active reset"),tr("..."),styleLabelG,styleLabelGr,"","","","",this);upDownGroupBox1L->addWidget(bitVector.last(),row,1);
	row=0;
	bitVector<<new bitClass("PC2_PCcomDint",6,2,bitClass::TYPE_CHANGE_COLOR,tr("Reset proximity switch on"),tr("Reset proximity switch off"),styleLabelG,styleLabelGr,"","","","",this);upDownGroupBox2L->addWidget(bitVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,tuxip->getIp(),"PC2_PCcomReal",3,0,1782.,tr("Target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,0,"",this);upDownGroupBox2L->addWidget(floatPlcMemoryVector.last(),row,0);
	bitVector<<new bitClass("PC2_PCcomDint",6,17,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);upDownGroupBox2L->addWidget(bitVector.last(),row,1);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,tuxip->getIp(),"PC2_PCcomReal",1,0,1782.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);upDownGroupBox2L->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,tuxip->getIp(),"PC2_PCcomReal",7,0,100.,tr("Hz"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,0,"",this);upDownGroupBox2L->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	progressVector<<new progressClass(tuxip,tuxip->getIp(),"PC2_PCcomReal",5,tr("Velocity"),tr("m/min"),-20.,20.,5.,200,200,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","value","","vertical velocity",180,2,this);upDownGroupBox2L->addWidget(progressVector.last(),row,0,1,2,Qt::AlignHCenter|Qt::AlignTop);
	verticalVelocityPlot=new plotClass(tuxip->getIp(),"PC2_PCcomReal",5,tr("Vertical velocity"),this);
	progressVector.last()->setPlotClass(verticalVelocityPlot);
	upDownGroupBox2L->setRowStretch(row,1);
	QGroupBox *fwdBwdGroupBox=new QGroupBox(tr("Fwd-bwd"));upSplitterW0L->addWidget(fwdBwdGroupBox);
	QGridLayout *fwdBwdGroupBoxL=new QGridLayout();fwdBwdGroupBox->setLayout(fwdBwdGroupBoxL);
	row=0;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Fwd"),":/leftArrow.png",tuxip,"PC2_PCcomDint",5,1,0,"","PC2_PCcomDint",6,25,"","","","","",buttonStyle,buttonStyleActive,tr("Fwd"),this);fwdBwdGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	bitVector<<new bitClass("PC2_PCcomDint",6,25,bitClass::TYPE_CHANGE_COLOR,tr("moving fwd"),tr("..."),styleLabelG,styleLabelGr,"","bit move fwd","","",this);fwdBwdGroupBoxL->addWidget(bitVector.last(),row,1);
	bitVector.last()->setMinimumWidth(200);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Bwd"),":/rightArrow.png",tuxip,"PC2_PCcomDint",5,2,0,"","PC2_PCcomDint",6,26,"","","","","",buttonStyle,buttonStyleActive,tr("bwd"),this);fwdBwdGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	bitVector<<new bitClass("PC2_PCcomDint",6,26,bitClass::TYPE_CHANGE_COLOR,tr("moving bwd"),tr("..."),styleLabelG,styleLabelGr,"","bit move bwd","","",this);fwdBwdGroupBoxL->addWidget(bitVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset"),":/reset.png",tuxip,"PC2_PCcomDint",5,17,0,"","PC2_PCcomDint",6,14,"","","","","",buttonStyle,buttonStyleActive,tr("Reset"),this);fwdBwdGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	bitVector<<new bitClass("PC2_PCcomDint",6,14,bitClass::TYPE_CHANGE_COLOR,tr("active reset"),tr("..."),styleLabelG,styleLabelGr,"","","","",this);fwdBwdGroupBoxL->addWidget(bitVector.last(),row,1);
	row++;
	bitVector<<new bitClass("PC2_PCcomDint",6,1,bitClass::TYPE_CHANGE_COLOR,tr("Reset proximity switch on"),tr("Reset proximity switch off"),styleLabelG,styleLabelGr,"","","","",this);fwdBwdGroupBoxL->addWidget(bitVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,tuxip->getIp(),"PC2_PCcomReal",2,0,100000.,tr("Target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,0,"",this);fwdBwdGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,0);
	bitVector<<new bitClass("PC2_PCcomDint",6,16,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);fwdBwdGroupBoxL->addWidget(bitVector.last(),row,1);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,tuxip->getIp(),"PC2_PCcomReal",0,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);fwdBwdGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,tuxip->getIp(),"PC2_PCcomReal",6,0,100.,tr("Hz"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,0,"",this);fwdBwdGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	row++;
	progressVector<<new progressClass(tuxip,tuxip->getIp(),"PC2_PCcomReal",4,tr("Velocity"),tr("m/min"),-100.,100.,25.,200,200,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","value","","vertical velocity",180,2,this);fwdBwdGroupBoxL->addWidget(progressVector.last(),row,0,1,2,Qt::AlignHCenter|Qt::AlignTop);
	fwdBwdGroupBoxL->setRowStretch(row,1);
	//layout
	QScrollArea *craneScrollArea=new QScrollArea();craneScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);upSplitterW1L->addWidget(craneScrollArea);
	craneScrollArea->setWidgetResizable(true);
	QWidget *craneScrollAreaWidget=new QWidget();
	QVBoxLayout *craneScrollAreaWidgetLayout=new QVBoxLayout();craneScrollAreaWidgetLayout->setMargin(0);craneScrollAreaWidget->setLayout(craneScrollAreaWidgetLayout);
	craneScrollArea->setWidget(craneScrollAreaWidget);
	layoutGraphicsView=new myQGraphicsViewClass(true,this);
	layoutGraphicsView->scale(0.025,0.025);
	QGraphicsScene *layoutScene=new QGraphicsScene(this);
	craneScrollAreaWidgetLayout->addWidget(layoutGraphicsView);
	layoutGraphicsView->setScene(layoutScene);

	gItemClass *layoutGraphicsItem=new gItemClass(":/dxfs/pincer2/layoutSide.dxf",QPen(QColor(200,200,200)),this,Qt::NoBrush,Qt::NoBrush);layoutScene->addItem(layoutGraphicsItem);
	craneXItem=new gItemClass(":/dxfs/pincer2/pincerX.dxf",QPen(QColor(100,100,200)),this,Qt::NoBrush,Qt::NoBrush);layoutScene->addItem(craneXItem);
	craneZItem=new gItemClass(":/dxfs/pincer2/pincerZ.dxf",QPen(QColor(200,100,100)),this,Qt::NoBrush,Qt::NoBrush);layoutScene->addItem(craneZItem);

	allPincer2Positions=new allPincer2PosClass(allPos,tuxip,this,layoutScene,this);

	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr("Zoom extents"),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));


	maintenanceBtn=new QPushButton(QIcon(":/gearGray.png"),tr("Maintenance"),layoutGraphicsView);
	maintenanceBtn->move(125,5);
	maintenanceBtn->setIconSize(QSize(16,16));
	connect(maintenanceBtn,SIGNAL(clicked()),this,SLOT(maintenanceBtnClickedSlot()));
	//Hooks
	QGroupBox *hooksGroupBox=new QGroupBox(tr("Forks"));upSplitterW1L->addWidget(hooksGroupBox);
	QVBoxLayout *hooksGroupBoxL=new QVBoxLayout() ;hooksGroupBox->setLayout (hooksGroupBoxL);
	QHBoxLayout *hooksGroupBox1L=new QHBoxLayout();hooksGroupBoxL->addLayout(hooksGroupBox1L);
	QHBoxLayout *hooksGroupBox2L=new QHBoxLayout();hooksGroupBoxL->addLayout(hooksGroupBox2L);

	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Forks open"),":/oppenArrows.png",tuxip,"PC2_PCcomDint",5,6,0,"","PC2_PCcomDint",6,21,"","","","","",buttonStyle,buttonStyleActive,tr("Forks open"),this);hooksGroupBox1L->addWidget(pushButtonsVector.last());
	bitVector<<new bitClass("PC2_PCcomDint",6,5,bitClass::TYPE_CHANGE_COLOR,tr("Magnetic switch, forks open"),tr("Magnetic switch, forks not open"),styleLabelG,styleLabelGr,"","","","",this);hooksGroupBox1L->addWidget(bitVector.last());
	bitVector.last()->setMinimumWidth(250);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Forks close"),":/closeArrows.png",tuxip,"PC2_PCcomDint",5,5,0,"","PC2_PCcomDint",6,10,"","","","","",buttonStyle,buttonStyleActive,tr("Forks close"),this);hooksGroupBox1L->addWidget(pushButtonsVector.last());
	bitVector<<new bitClass("PC2_PCcomDint",6,6,bitClass::TYPE_CHANGE_COLOR,tr("Magnetic switch, forks closed"),tr("Magnetic switch, forks not closed"),styleLabelG,styleLabelGr,"","","","",this);hooksGroupBox1L->addWidget(bitVector.last());
	bitVector.last()->setMinimumWidth(250);

	bitVector<<new bitClass("PC2_PCcomDint",6,12,bitClass::TYPE_CHANGE_COLOR,tr("SB detection photocell left"),tr("SB detection photocell left"),styleLabelG,styleLabelGr,"","","","",this);hooksGroupBox2L->addWidget(bitVector.last());
	bitVector<<new bitClass("PC2_PCcomDint",6,8,bitClass::TYPE_CHANGE_COLOR,tr("Magnetic switch, pin locked"),tr("Magnetic switch, pin not locked"),styleLabelG,styleLabelGr,"","","","",this);hooksGroupBox2L->addWidget(bitVector.last());
	bitVector<<new bitClass("PC2_PCcomDint",6,7,bitClass::TYPE_CHANGE_COLOR,tr("Magnetic switch, pin unlocked"),tr("Magnetic switch, pin not unlocked"),styleLabelG,styleLabelGr,"","","","",this);hooksGroupBox2L->addWidget(bitVector.last());
	bitVector<<new bitClass("PC2_PCcomDint",6,13,bitClass::TYPE_CHANGE_COLOR,tr("SB detection photocell right"),tr("SB detection photocell right"),styleLabelG,styleLabelGr,"","","","",this);hooksGroupBox2L->addWidget(bitVector.last());


	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto missions"),"",tuxip,"PC2_PCcomDint",5,28,0,"","PC2_PCcomDint",21,0,"","","","","",buttonStyle,buttonStyleActive,tr("Manual missions"),this);upSplitterW2L->addWidget(pushButtonsVector.last());
	QGroupBox *missionGroupBox=new QGroupBox(tr("Mission"));upSplitterW2L->addWidget(missionGroupBox);
	QGridLayout *missionGroupBoxL=new QGridLayout();missionGroupBoxL->setMargin(2);missionGroupBox->setLayout(missionGroupBoxL);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,tuxip->getIp(),"PC2_PCcomDint",0,1,164,tr("From:"),"","value","","from pos",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_POPUP,"",this);missionGroupBoxL->addWidget(intPlcMemoryVector.last(),0,0);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,tuxip->getIp(),"PC2_PCcomDint",1,10,164,tr("To:"),"","value","","to pos",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_POPUP,"",this);missionGroupBoxL->addWidget(intPlcMemoryVector.last(),0,1);

	bitVector<<new bitClass("PC2_PCcomDint",6,20,bitClass::TYPE_CHANGE_COLOR,tr("Ok for new mission"),tr(""),styleLabelG,styleLabelGr,"","","","",this);missionGroupBoxL->addWidget(bitVector.last(),3,0,1,2);
	bitVector<<new bitClass("PC2_PCcomDint",6,31,bitClass::TYPE_CHANGE_COLOR,tr("On mission"),tr("Not on mission"),styleLabelG,styleLabelGr,"","","","",this);missionGroupBoxL->addWidget(bitVector.last(),3,2,1,2);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Continue"),"",tuxip,"PC2_PCcomDint",5,13,0,"","PC2_PCcomDint",7,4,"","","","","",buttonStyle,"",tr("Continue"),this);missionGroupBoxL->addWidget(pushButtonsVector.last(),4,0,1,4);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Abort"),"",tuxip,"PC2_PCcomDint",5,14,0,"",   "PC2_PCcomDint",7,3,"","","","","",buttonStyle,"",tr("Abort"),this);missionGroupBoxL->addWidget(pushButtonsVector.last(),5,0,1,2);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("End"),"",tuxip,"PC2_PCcomDint",5,12,0,"",     "PC2_PCcomDint",7,2,"","","","","",buttonStyle,"",tr("End"),this);missionGroupBoxL->addWidget(pushButtonsVector.last(),5,2,1,2);
	QGroupBox *modeGroupBox=new QGroupBox(tr("Mode"));upSplitterW2L->addWidget(modeGroupBox);
	QHBoxLayout *modeGroupBoxL=new QHBoxLayout();modeGroupBoxL->setMargin(2);modeGroupBox->setLayout(modeGroupBoxL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",tuxip,"PC2_PCcomDint",5,10,0,"","PC2_PCcomDint",6,18,"bit auto","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",tuxip,"PC2_PCcomDint",5,11,0,"","PC2_PCcomDint",6,19,"bit manual","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	missionStepsVector=new bitLogVectorClass("PC2_PCcomDint","pincer2AutoMissionSteps.txt",bitClass::TYPE_SHOW_HIDE,styleLabelG,styleLabelGr,"","","","",false,bitLogHisoryClass::typeMySql,this);
	missionStepsVector->setMaximumHeight(100);
	upSplitterW2L->addWidget(missionStepsVector);
	horizontalPosVector=new bitLogVectorClass("PC2_PCcomDint","pincer2AutoHorizontalPositions.txt",bitClass::TYPE_SHOW_HIDE,styleLabelB,styleLabelGr,"","","","",false,bitLogHisoryClass::typeMySql,this);
	horizontalPosVector->setMaximumHeight(100);
	upSplitterW2L->addWidget(horizontalPosVector);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset alarms"),"",tuxip,"PC2_PCcomDint",5,9,0,"","PC2_PCcomDint",6,29,"","","","","",buttonStyleGreen,buttonStyleRed,tr("No alarms"),this);upSplitterW2L->addWidget(pushButtonsVector.last());
	QHBoxLayout *stopL=new QHBoxLayout();upSplitterW2L->addLayout(stopL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Active stop"),"",tuxip,"PC2_PCcomDint",5,20,0,"","PC2_PCcomDint",6,3,"","","","","",buttonStyleGreen,buttonStyleRed,tr("No active stop"),this);stopL->addWidget(pushButtonsVector.last());
	//	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Stop recall"),"",tuxip,"PC2_PCcomDint",5,15,0,"","PC2_PCcomDint",0,0,"","","","","",buttonStyle,"",tr("Stop recall"),this);stopL->addWidget(pushButtonsVector.last());
	//bitVector<<new bitClass("PC2_PCcomDint",6,9,bitClass::TYPE_CHANGE_COLOR,tr("Active stop"),tr("No active stop"),styleLabelR,styleLabelGr,"","","","",this);stopL->addWidget(bitVector.last());
	//bitVector.last()->setMinimumWidth(150);
	//QHBoxLayout *alarmsL=new QHBoxLayout();upSplitterW2L->addLayout(alarmsL);
	//bitVector<<new bitClass("PC2_PCcomDint",6,29,bitClass::TYPE_CHANGE_COLOR,tr("Alarm"),tr("No alarm"),styleLabelR,styleLabelG,"","","","",this);alarmsL->addWidget(bitVector.last());
	////bitVector<<new bitClass("PC2_PCcomDint",6,30,bitClass::TYPE_CHANGE_COLOR,tr("Warning"),tr("No warning"),styleLabelR,styleLabelG,"","","","",this);alarmsL->addWidget(bitVector.last());
	upSplitterW2L->addStretch(1);

	bitErrorsVector=new bitLogVectorClass("PC2_PCcomDint","pincer2Alarms.txt",bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,"","Errors","","",false,bitLogHisoryClass::typeMySql,this);
	bitWarningsVector=new bitLogVectorClass("PC2_PCcomDint","pincer2PlcWarnings.txt",bitClass::TYPE_SHOW_HIDE,styleLabelO,styleLabelGr,"","Warnings","","",false,bitLogHisoryClass::typeMySql,this);
	downLayout->addWidget(bitWarningsVector);
	downLayout->addWidget(bitErrorsVector);

	myPos=allPincer2Positions->getPosWithId(pincer2Id);

	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);

	QTimer *checkForNewMissionsTimer=new QTimer(this);
	connect(checkForNewMissionsTimer,SIGNAL(timeout()),this,SLOT(checkForNewMissionsTimerSlot()));
	checkForNewMissionsTimer->start(1000);
}
pincer2Class::~pincer2Class()
{
}
void pincer2Class::resetGraphicsViewZoom()
{
	layoutGraphicsView->scene()->setSceneRect(layoutGraphicsView->scene()->itemsBoundingRect());
	layoutGraphicsView->fitInView(layoutGraphicsView->sceneRect(),Qt::KeepAspectRatio);
}
void pincer2Class::stopSlot()
{
	tuxip->writeInteger("PC2_PCcomDint",5,20);
	tuxip->writeInteger("PC2_PCcomDint",5,0);
}
void pincer2Class::maintenanceBtnClickedSlot()
{
	allPincer2Positions->setUserMaintenanseMode(!allPincer2Positions->getUserMaintenanseMode());
	maintenanceBtn->setIcon(allPincer2Positions->getUserMaintenanseMode()?QIcon(":/gearBlue.png"):QIcon(":/gearGray.png"));
}
void pincer2Class::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==tuxip->getIp())
	{
		allPincer2Positions->dataInPlcChanged(ip,table,index,value);
		if ((table=="PC2_PCcomDint")&&(index==6))
		{
			bool v=HAVECOMMONBIT(value.toInt(),BITS[20]);
			if (v!=readyForNewMission)
			{
				readyForNewMission=v;
				allPincer2Positions->setPlcOnMission(!readyForNewMission);
			}
		}
		missionStepsVector->updateValue(table,index,value.toInt());
		horizontalPosVector->updateValue(table,index,value.toInt());
		bitErrorsVector->updateValue(table,index,value.toInt());
		bitWarningsVector->updateValue(table,index,value.toInt());
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<intPlcMemoryVector.count();i++)
			intPlcMemoryVector[i]->updateValue(ip,table,index,value.toInt());
		for (int i=0;i<floatPlcMemoryVector.count();i++)
			floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toFloat());
		for (int i=0;i<progressVector.count();i++)
			progressVector[i]->updateValue(ip,table,index,value.toFloat());
		for (int i=0;i<bitVector.count();i++)
			bitVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<activeIconsVector.count();i++)
			activeIconsVector[i]->updateValue(table,index,value.toInt());
		verticalVelocityPlot->dataInPlcChanged(ip,table,index,value.toFloat());
		if (table=="PC2_PCcomDint")
		{
			if (index==0)
				plcFrom=value.toInt();
			if (index==1)
				plcTo=value.toInt();
			if (index==2)
				pcStatus=value.toInt();
			if (index==3)
				plcStatus=value.toInt();
			if (index==21)
				plcCraneBasket=value.toInt();
		}
		if (table=="PC2_PCcomReal")
		{
			if ((index==0)||(index==1))
			{
				if (index==0)
					plcX=value.toFloat();
				if (index==1)
					plcZ=value.toFloat();
				craneXItem->setPos(-plcX,0.f);
				craneZItem->setPos(-plcX,-plcZ);
				float yOffset=180.;
				myPos->setPos(-plcX,yOffset-plcZ);
			}
		}
	}
}
void pincer2Class::checkForNewMissionsTimerSlot()
{
	//if ( (!importsCheckBtn->isChecked())&&(!exportsCheckBtn->isChecked()) )
	//	return;
	if (plcStatus!=0)
		return;
}
void pincer2Class::newMission(pincerPositionClass *from,pincerPositionClass *to)
{
	if (!getIsIdle())
		return;
	tuxip->writeInteger("PC2_PCcomDint",0,from->getPlcIndex());
	tuxip->writeInteger("PC2_PCcomDint",1,to->getPlcIndex());
	tuxip->writeInteger("PC2_PCcomDint",5,21);
}
bool pincer2Class::getIsIdle()
{
	return (plcStatus==0)&&(plcFrom==0)&&(plcTo==0)&&(plcCraneBasket==0);
}
void pincer2Class::importsCheckBtnStateChangedSlot(int state)
{
	checkForNewMissionsTimerSlot();
}
void pincer2Class::exportsCheckBtnStateChangedSlot(int state)
{
	checkForNewMissionsTimerSlot();
}

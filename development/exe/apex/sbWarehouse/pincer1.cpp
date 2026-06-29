#include "pch.h"
#include "pincer1.h"
pincer1Class::pincer1Class(allPossClass *allPos_,tuxipClass *tuxip_,QWidget *p):QWidget(p)
{
	allPos=allPos_;
	tuxip=tuxip_;
	pcStatus=plcStatus=plcFrom=plcTo=plcCraneBasket=0;
	readyForNewMission=false;
	plcX=plcY=0.f;
	//
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);
	setLayout(mainLayout);
	QSplitter *centralSplitter=new QSplitter(Qt::Vertical);mainLayout->addWidget(centralSplitter);

	QScrollArea *mainScrollArea=new QScrollArea();mainScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	mainScrollArea->setWidgetResizable(true);
	QWidget *mainScrollAreaWidget=new QWidget();
	QHBoxLayout *mainScrollAreaLayout=new QHBoxLayout();mainScrollAreaLayout->setMargin(2);mainScrollAreaWidget->setLayout(mainScrollAreaLayout);
	mainScrollArea->setWidget(mainScrollAreaWidget);
	centralSplitter->addWidget(mainScrollArea);
	QVBoxLayout *layout0=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout0);layout0->setMargin(2);
	QVBoxLayout *layout1=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout1);
	QVBoxLayout *layout2=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout2);
	QWidget *warningsErrorsWidget=new QWidget();
	QHBoxLayout *warningsErrorsLayout=new QHBoxLayout();warningsErrorsWidget->setLayout(warningsErrorsLayout);
	bitErrorsVector=new bitLogVectorClass("pc1ToPCI","pincer1Alarms.txt",bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelG,"","Errors","c2plcMessages","",false,bitLogHisoryClass::typeMySql,this);
	bitWarningsVector=new bitLogVectorClass("pc1ToPCI","pincer1Warnings.txt",bitClass::TYPE_SHOW_HIDE,styleLabelO,styleLabelG,"","Warnings","c2plcMessages","",false,bitLogHisoryClass::typeMySql,this);
	warningsErrorsLayout->addWidget(bitWarningsVector);
	warningsErrorsLayout->addWidget(bitErrorsVector);
	centralSplitter->addWidget(warningsErrorsWidget);

	fillAxisGroup(layout0,tr("Horizontal X"),0);
	fillAxisGroup(layout0,tr("Horizontal Y"),6);
	fillAxisGroup(layout0,tr("Vartical"),12);
	layout0->addStretch(1);
	//layout1
	QScrollArea *craneScrollArea=new QScrollArea();craneScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);layout1->addWidget(craneScrollArea);
	craneScrollArea->setWidgetResizable(true);
	QWidget *craneScrollAreaWidget=new QWidget();
	QVBoxLayout *craneScrollAreaWidgetLayout=new QVBoxLayout();craneScrollAreaWidgetLayout->setMargin(0);craneScrollAreaWidget->setLayout(craneScrollAreaWidgetLayout);
	craneScrollArea->setWidget(craneScrollAreaWidget);
	layoutGraphicsView=new myQGraphicsViewClass(true,this);
	layoutGraphicsView->scale(0.025,0.025);
	QGraphicsScene *layoutScene=new QGraphicsScene(this);
	craneScrollAreaWidgetLayout->addWidget(layoutGraphicsView);
	layoutGraphicsView->setScene(layoutScene);
	gItemClass *layoutGraphicsItem=new gItemClass(":/dxfs/pincer1/layoutTop.dxf",QPen(QColor(200,200,200)),this,Qt::NoBrush,Qt::NoBrush);layoutScene->addItem(layoutGraphicsItem);
	craneXItem=new gItemClass(":/dxfs/pincer1/pincerX.dxf",QPen(QColor(100,100,200)),this,Qt::NoBrush,Qt::NoBrush);layoutScene->addItem(craneXItem);
	craneYItem=new gItemClass(":/dxfs/pincer1/pincerY.dxf",QPen(QColor(200,100,100)),this,Qt::NoBrush,Qt::NoBrush);layoutScene->addItem(craneYItem);

	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr("Zoom extents"),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));

	maintenanceBtn=new QPushButton(QIcon(":/gearGray.png"),tr("Maintenance"),layoutGraphicsView);
	maintenanceBtn->move(125,5);
	maintenanceBtn->setIconSize(QSize(16,16));
	connect(maintenanceBtn,SIGNAL(clicked()),this,SLOT(maintenanceBtnClickedSlot()));


	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto missions"),"",tuxip,"pc1ToPCI",22,28,0,"","pc1ToPCI",24,31,"","","","","",buttonStyle,buttonStyleActive,tr("Manual missions"),this);layout2->addWidget(pushButtonsVector.last());

	QGroupBox *missionGroupBox=new QGroupBox(tr("Mission"));layout2->addWidget(missionGroupBox);
	QGridLayout *missionGroupBoxL=new QGridLayout();missionGroupBoxL->setMargin(2);missionGroupBox->setLayout(missionGroupBoxL);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,tuxip->getIp(),"pc1ToPCI",0,2,60,tr("Next From:"),"","value","","from pos",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_POPUP,"",this);missionGroupBoxL->addWidget(intPlcMemoryVector.last(),0,0);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,tuxip->getIp(),"pc1ToPCI",1,2,60,tr("Next To:"),"","value","","to pos",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_POPUP,"",this);missionGroupBoxL->addWidget(intPlcMemoryVector.last(),0,1);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,tuxip->getIp(),"pc1ToPCI",10,2,60,tr("Active from:"),"","value","","from pos",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_POPUP,"",this);missionGroupBoxL->addWidget(intPlcMemoryVector.last(),1,0);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,tuxip->getIp(),"pc1ToPCI",11,2,60,tr("Active To:"),"","value","","to pos",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_POPUP,"",this);missionGroupBoxL->addWidget(intPlcMemoryVector.last(),1,1);

	bitVector<<new bitClass("pc1ToPCI",21,1,bitClass::TYPE_CHANGE_COLOR,tr("Ok for new mission"),tr(""),styleLabelG,styleLabelGr,"","","","",this);missionGroupBoxL->addWidget(bitVector.last(),3,0,1,2);
	bitVector<<new bitClass("pc1ToPCI",21,2,bitClass::TYPE_CHANGE_COLOR,tr("On mission"),tr("Not on mission"),styleLabelG,styleLabelGr,"","","","",this);missionGroupBoxL->addWidget(bitVector.last(),3,2,1,2);
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Continue"),"",tuxip,"pc1ToPCI",5,13,0,"","pc1ToPCI",0,0,"","","","","",buttonStyle,"",tr("Continue"),this);missionGroupBoxL->addWidget(pushButtonsVector.last(),4,0,1,4);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Abort"),"",tuxip,"pc1ToPCI",22,26,0,"","pc1ToPCI",21,15,"","","","","",buttonStyle,"",tr("Abort"),this);missionGroupBoxL->addWidget(pushButtonsVector.last(),5,0,1,2);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("End"),"",tuxip,"pc1ToPCI",22,24,0,"","pc1ToPCI",21,17,"","","","","",buttonStyle,"",tr("End"),this);missionGroupBoxL->addWidget(pushButtonsVector.last(),5,2,1,2);
	QGroupBox *modeGroupBox=new QGroupBox(tr("Mode"));layout2->addWidget(modeGroupBox);
	QHBoxLayout *modeGroupBoxL=new QHBoxLayout();modeGroupBoxL->setMargin(2);modeGroupBox->setLayout(modeGroupBoxL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",tuxip,"pc1ToPCI",22,21,0,"","pc1ToPCI",24,0,"bit auto","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",tuxip,"pc1ToPCI",22,22,0,"","pc1ToPCI",24,1,"bit manual","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	missionStepsVector=new bitLogVectorClass("pc1ToPCI","pincer1AutoMissionSteps.txt",bitClass::TYPE_SHOW_HIDE,styleLabelG,styleLabelGr,"","","","",false,bitLogHisoryClass::typeMySql,this);
	missionStepsVector->setMaximumHeight(100);
	layout2->addWidget(missionStepsVector);
	horizontalPosVector=new bitLogVectorClass("pc1ToPCI","pincer1Positions.txt",bitClass::TYPE_SHOW_HIDE,styleLabelB,styleLabelGr,"","","","",false,bitLogHisoryClass::typeMySql,this);
	horizontalPosVector->setMaximumHeight(100);
	layout2->addWidget(horizontalPosVector);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Alarms"),"",tuxip,"pc1ToPCI",22,23,0,"","pc1ToPCI",24,2,"","","","","",buttonStyleActive,buttonStyleRed,tr("Reset Alarms"),this);layout2->addWidget(pushButtonsVector.last());
	QHBoxLayout *stopL=new QHBoxLayout();layout2->addLayout(stopL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Stop"),"",tuxip,"pc1ToPCI",22,226,0,"","pc1ToPCI",23,27,"","","","","",buttonStyleActive,buttonStyleRed,tr("Stop"),this);stopL->addWidget(pushButtonsVector.last());
	//	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Stop recall"),"",tuxip,"pc1ToPCI",5,15,0,"","pc1ToPCI",0,0,"","","","","",buttonStyle,"",tr("Stop recall"),this);stopL->addWidget(pushButtonsVector.last());
	//bitVector<<new bitClass("pc1ToPCI",6,9,bitClass::TYPE_CHANGE_COLOR,tr("Active stop"),tr("No active stop"),styleLabelR,styleLabelGr,"","","","",this);stopL->addWidget(bitVector.last());
	bitVector.last()->setMinimumWidth(150);
	//QHBoxLayout *alarmsL=new QHBoxLayout();layout2->addLayout(alarmsL);
	//bitVector<<new bitClass("pc1ToPCI",24,9,bitClass::TYPE_CHANGE_COLOR,tr("Alarm"),tr("No alarm"),styleLabelR,styleLabelG,"","","","",this);alarmsL->addWidget(bitVector.last());
//	bitVector<<new bitClass("pc1ToPCI",6,30,bitClass::TYPE_CHANGE_COLOR,tr("Warning"),tr("No warning"),styleLabelR,styleLabelG,"","","","",this);alarmsL->addWidget(bitVector.last());

	QGroupBox *safetyPinsGroupBox=new QGroupBox(tr("Safety pin"));layout2->addWidget(safetyPinsGroupBox);
	QGridLayout *safetyPinsGroupBoxL=new QGridLayout();safetyPinsGroupBox->setLayout(safetyPinsGroupBoxL);
	int row=0;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Lock"),":/lock.png",tuxip,"pc1ToPCI",22,12,0,"","pc1ToPCI",23,5,"","","","","",buttonStyle,buttonStyleActive,tr("Lock"),this);safetyPinsGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Unlock"),":/unLock.png",tuxip,"pc1ToPCI",22,11,0,"","pc1ToPCI",23,4,"","","","","",buttonStyle,buttonStyleActive,tr("Unlock"),this);safetyPinsGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	bitVector<<new bitClass("pc1ToPCI",23,6,bitClass::TYPE_CHANGE_COLOR,tr("Locked"),tr("Locked"),styleLabelG,styleLabelGr,"","","","",this);safetyPinsGroupBoxL->addWidget(bitVector.last(),row,0);
	bitVector<<new bitClass("pc1ToPCI",23,7,bitClass::TYPE_CHANGE_COLOR,tr("Unlocked"),tr("Unlocked"),styleLabelG,styleLabelGr,"","","","",this);safetyPinsGroupBoxL->addWidget(bitVector.last(),row,1);
	//Basket pins
	row=0;
	QGroupBox *basketPinsGroupBox=new QGroupBox(tr("Forks"));layout2->addWidget(basketPinsGroupBox);
	QGridLayout *basketPinsGroupBoxL=new QGridLayout();basketPinsGroupBox->setLayout(basketPinsGroupBoxL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Forks open"), ":/oppenArrows.png",tuxip,"pc1ToPCI",22,5,0,"","pc1ToPCI",23,9,"","","","","",buttonStyle,buttonStyleActive,tr("Forks open"),this);basketPinsGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Forks close"),":/closeArrows.png",tuxip,"pc1ToPCI",22,6,0,"","pc1ToPCI",23,8,"","","","","",buttonStyle,buttonStyleActive,tr("Forks close"),this);basketPinsGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	bitVector<<new bitClass("pc1ToPCI",23,10,bitClass::TYPE_CHANGE_COLOR,tr("Magnetic switch, forks open"),tr("Magnetic switch, forks not open"),styleLabelG,styleLabelGr,"","","","",this);basketPinsGroupBoxL->addWidget(bitVector.last(),row,0);
	bitVector<<new bitClass("pc1ToPCI",23,18,bitClass::TYPE_CHANGE_COLOR,tr("Magnetic switch, forks closed"),tr("Magnetic switch, forks not closed"),styleLabelG,styleLabelGr,"","","","",this);basketPinsGroupBoxL->addWidget(bitVector.last(),row,1);


	layout2->addStretch(1);
	//
	centralSplitter->setSizes(QList<int>()<<900<<100);
//	settings=new hmiSettingsClass(tuxip,this);
	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);

	allPincer1Positions=new allPincer1PosClass(allPos,tuxip,this,layoutScene,this);
	myPos=allPincer1Positions->getPosWithId(pincer1Id);
}
pincer1Class::~pincer1Class()
{
}
void pincer1Class::fillAxisGroup(QVBoxLayout *layout,QString title,int index)
{
	QGroupBox *groupBox=new QGroupBox(title);layout->addWidget(groupBox);
	QGridLayout *groupBoxL=new QGridLayout();groupBox->setLayout(groupBoxL);groupBoxL->setMargin(0);
	int row=0;
	progressVector<<new progressClass(tuxip,tuxip->getIp(),"pc1ToPCF",index+2,tr("Velocity"),tr("m/min"),-120.,120.,20.,175,175,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","","","",180,1,this);groupBoxL->addWidget(progressVector.last(),row,0,1,2,Qt::AlignHCenter|Qt::AlignTop);
	plotsVector<<new plotClass(tuxip->getIp(),"pc1ToPCF",index+3,tr("Hz"),this);
	plotsVector.last()->addNewGraph(tuxip->getIp(),"pc1ToPCF",index+0,tr("Position[mm]"),1,0);
	plotsVector.last()->addNewGraph(tuxip->getIp(),"pc1ToPCF",index+2,tr("Velocity[m/min]"),2,0);
	plotsVector.last()->addNewGraph(tuxip->getIp(),"pc1ToPCF",index+4,tr("Current[Amps]"),3,0);
	progressVector.last()->setPlotClass(plotsVector.last());

	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,tuxip->getIp(),"pc1ToPCF",index+1,0,100000.,tr("Target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);groupBoxL->addWidget(floatPlcMemoryVector.last(),row,0);
	if (index==0)
	{
		bitVector<<new bitClass("pc1ToPCI",23,22,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,1);
	}
	if (index==6)
	{
		bitVector<<new bitClass("pc1ToPCI",23,23,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,1);
	}
	if (index==12)
	{
		bitVector<<new bitClass("pc1ToPCI",23,25,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,1);
	}
	bitVector.last()->setMinimumWidth(150);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,tuxip->getIp(),"pc1ToPCF",index+0,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);groupBoxL->addWidget(floatPlcMemoryVector.last(),row,0);
	if (index==0)
	{
		bitVector<<new bitClass("pc1ToPCI",23,1,bitClass::TYPE_CHANGE_COLOR,tr("Proximity on"),tr("Proximity off"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,1);
	}
	if (index==6)
	{
		bitVector<<new bitClass("pc1ToPCI",23,15,bitClass::TYPE_CHANGE_COLOR,tr("Proximity on"),tr("Proximity off"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,1);
	}
	if (index==12)
	{
		bitVector<<new bitClass("pc1ToPCI",23,0,bitClass::TYPE_CHANGE_COLOR,tr("Proximity on"),tr("Proximity off"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,1);
	}
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,tuxip->getIp(),"pc1ToPCF",index+3,0,100.,tr("Hz"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);groupBoxL->addWidget(floatPlcMemoryVector.last(),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,tuxip->getIp(),"pc1ToPCF",index+4,0,100.,tr("Amps"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);groupBoxL->addWidget(floatPlcMemoryVector.last(),row,1);
	row++;
	if (index==0)
	{
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Bwd"),":/arrowLeft.png",tuxip,"pc1ToPCI",22,2,0,"","pc1ToPCI",24,4,"","","","","",buttonStyle,buttonStyleActive,tr("Bwd"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,0);
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Fwd"),":/arrowRight.png",tuxip,"pc1ToPCI",22,1,0,"","pc1ToPCI",24,3,"","","","","",buttonStyle,buttonStyleActive,tr("Fwd"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,1);
		row++;
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset BWD"),":/arrowUp.png",tuxip,"pc1ToPCI",22,101,0,"","pc1ToPCI",23,14,"","","","","",buttonStyle,buttonStyleActive,tr("Reset BWD"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,0,1,2);
	}
	if (index==6)
	{
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Fwd"),":/arrowDown.png",tuxip,"pc1ToPCI",22,3,0,"","pc1ToPCI",24,5,"","","","","",buttonStyle,buttonStyleActive,tr("Fwd"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,0);
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Bwd"),":/arrowUp.png",tuxip,"pc1ToPCI",22,4,0,"","pc1ToPCI",24,6,"","","","","",buttonStyle,buttonStyleActive,tr("Bwd"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,1);
		row++;
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset BWD"),":/arrowUp.png",tuxip,"pc1ToPCI",22,103,0,"","pc1ToPCI",23,13,"","","","","",buttonStyle,buttonStyleActive,tr("Reset BWD"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,0,1,2);
	}
	if (index==12)
	{
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Up"),":/arrowUp.png",tuxip,"pc1ToPCI",22,8,0,"","pc1ToPCI",24,10,"","","","","",buttonStyle,buttonStyleActive,tr("Up"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,0);
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Down"),":/arrowDown.png",tuxip,"pc1ToPCI",22,7,0,"","pc1ToPCI",24,9,"","","","","",buttonStyle,buttonStyleActive,tr("Down"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,1);
		row++;
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset up"),":/arrowUp.png",tuxip,"pc1ToPCI",22,102,0,"","pc1ToPCI",23,12,"","","","","",buttonStyle,buttonStyleActive,tr("Reset up"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,0,1,2);
	}
}
void pincer1Class::resetGraphicsViewZoom()
{
	layoutGraphicsView->scene()->setSceneRect(layoutGraphicsView->scene()->itemsBoundingRect());
	layoutGraphicsView->fitInView(layoutGraphicsView->sceneRect(),Qt::KeepAspectRatio);
}
void pincer1Class::maintenanceBtnClickedSlot()
{
	allPincer1Positions->setUserMaintenanseMode(!allPincer1Positions->getUserMaintenanseMode());
	maintenanceBtn->setIcon(allPincer1Positions->getUserMaintenanseMode()?QIcon(":/gearBlue.png"):QIcon(":/gearGray.png"));
}
void pincer1Class::stopSlot()
{
	tuxip->writeInteger("pc1ToPCI",22,226);
	tuxip->writeInteger("pc1ToPCI",22,0);
}
void pincer1Class::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==tuxip->getIp())
	{
		allPincer1Positions->dataInPlcChanged(ip,table,index,value);
		if ((table=="pc1ToPCI")&&(index==21))
		{
			bool v=HAVECOMMONBIT(value.toInt(),BITS[1]);
			if (v!=readyForNewMission)
			{
				readyForNewMission=v;
				allPincer1Positions->setPlcOnMission(!readyForNewMission);
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
		if("pc1ToPCI"==table)
		{
			if ((index==21)||(index==23)||(index==24))
			{
				for (int i=0;i<bitVector.count();i++)
					bitVector[i]->updateValue(table,index,value.toInt());
				for (int i=0;i<pushButtonsVector.count();i++)
					pushButtonsVector[i]->updateValue(table,index,value.toInt());
			}
			for (int i=0;i<intPlcMemoryVector.count();i++)
				intPlcMemoryVector[i]->updateValue(ip,table,index,value.toInt());
			bitErrorsVector->updateValue(table,index,value.toInt());
			bitWarningsVector->updateValue(table,index,value.toInt());
			//horizontalPositionsLogVector->updateValue(table,index,value.toInt());
		}
		if("pc1ToPCF"==table)
		{
			for (int i=0;i<progressVector.count();i++)
				progressVector[i]->updateValue(ip,table,index,value.toFloat());
			for (int i=0;i<floatPlcMemoryVector.count();i++)
				floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toFloat());
			for (int i=0;i<plotsVector.count();i++)
				plotsVector[i]->dataInPlcChanged(ip,table,index,value.toFloat());
			//settings->dataInPlcChanged(ip,table,index,value);
		}
		if (table=="pc1ToPCI")
		{
			if (index==0)
				plcFrom=value.toInt();
			if (index==1)
				plcTo=value.toInt();
			if (index==2)
				pcStatus=value.toInt();
			if (index==3)
				plcStatus=value.toInt();
		}
		if (table=="posSb")
		{
			if (index==30)
				plcCraneBasket=value.toInt();
		}
		if (table=="pc1ToPCF")
		{
			if ((index==0)||(index==6))
			{
				if (index==0)
					plcX=value.toFloat();
				if (index==6)
					plcY=value.toFloat();
				craneXItem->setPos(0.,-plcX);
				craneYItem->setPos(-plcY,-plcX);
				myPos->setPos(-plcY,-plcX-680.);
			}
		}
	}
}
void pincer1Class::newMission(pincerPositionClass *from,pincerPositionClass *to)
{
	if (!getIsIdle())
		return;
	tuxip->writeInteger("pc1ToPCI",0,from->getPlcIndex());
	tuxip->writeInteger("pc1ToPCI",1,to->getPlcIndex());
	tuxip->writeInteger("pc1ToPCI",5,21);
}
bool pincer1Class::getIsIdle()
{
	return (plcStatus==0)&&(plcFrom==0)&&(plcTo==0)&&(plcCraneBasket==0);
}

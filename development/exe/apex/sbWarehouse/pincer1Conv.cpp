#include "pch.h"
#include "pincer1Conv.h"
pincer1ConvPositionsClass::pincer1ConvPositionsClass(tuxipClass *tuxip_,allPossClass *allPos_,QWidget *p):QWidget(p)
{
	tuxip=tuxip_;
	allPos=allPos_;
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);setLayout(mainLayout);

	QSplitter *mainSplitter=new QSplitter(Qt::Horizontal,this);mainLayout->addWidget(mainSplitter);
	mainSplitter->setStyleSheet("QSplitter::handle:Horizontal{height:3px;background-color:#777777;} QSplitterHandle:hover{} QSplitter::handle:Horizontal:hover{height:3px;background-color:#339966;}");
	mainLayout->addWidget(mainSplitter);

	QScrollArea *leftScrollArea=new QScrollArea();leftScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);mainSplitter->addWidget(leftScrollArea);
	leftScrollArea->setWidgetResizable(true);
	QWidget *leftScrollAreaWidget=new QWidget();leftScrollArea->setWidget(leftScrollAreaWidget);
	QVBoxLayout *leftScrollAreaWidgetL=new QVBoxLayout();leftScrollAreaWidgetL->setMargin(0);leftScrollAreaWidget->setLayout(leftScrollAreaWidgetL);
	//mainLayout->addStretch(1);
	splash->showMessage(tr("Creating positions..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QScrollArea *layoutCscrollArea=new QScrollArea();layoutCscrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);mainSplitter->addWidget(layoutCscrollArea);
	splash->showMessage(tr("Creating mode gui..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));

	QGroupBox *modeGroupBox=new QGroupBox(tr("Mode"));leftScrollAreaWidgetL->addWidget(modeGroupBox);
	QVBoxLayout *modeGroupBoxL=new QVBoxLayout();modeGroupBoxL->setMargin(2);modeGroupBox->setLayout(modeGroupBoxL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",tuxip,"rPcCom",9,1,0,"","rConveyorState",1,0,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",tuxip,"rPcCom",9,2,0,"","rConveyorState",1,1,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("No alarms"),"",tuxip,"rPcCom",9,3,0,"","rConveyorState",1,6,"","","","","",buttonStyleRed,buttonStyleActive,tr("Reset alarms"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Move cassette from 2 to 4"),"",tuxip,"rPcCom",9,24,0,"","rPcCom",0,2,"","","","","",buttonStyle,buttonStyleActive,tr("Move cassette from 2 to 4"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Move cassette from 4 to 5"),"",tuxip,"rPcCom",9,45,0,"","rPcCom",0,3,"","","","","",buttonStyle,buttonStyleActive,tr("Move cassette from 4 to 5"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	bitLogVectors<<new bitLogVectorClass("rPcCom",QString("pincer1ConCommands.txt"),bitClass::TYPE_SHOW_HIDE,styleLabelG,styleLabelGr,"","Commands","","",true,bitLogHisoryClass::typeMySql,this);leftScrollAreaWidgetL->addWidget(bitLogVectors.last());
	bitLogVectors<<new bitLogVectorClass("rPcCom",QString("pincer1ConAlarms.txt"),bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,"","Errors","","",true,bitLogHisoryClass::typeMySql,this);leftScrollAreaWidgetL->addWidget(bitLogVectors.last());
	//bitVector<<new bitClass("PcCom",hmiStatusIndex,3,bitClass::TYPE_CHANGE_COLOR,tr("Active stop"),tr("No active stop"),styleLabelR,styleLabelG,"","","","",this);modeGroupBoxL->addWidget(bitVector.last());
	//bitErrorsVector->setBitLogHistoryClassDatabaseEnum(bitLogHisoryClass::typeMySql);
	//bitWarningsVector->setBitLogHistoryClassDatabaseEnum(bitLogHisoryClass::typeMySql);
	mainSplitter->setSizes(QList<int>()<<500<<1300);

	layoutCscrollArea->setWidgetResizable(true);
	QWidget *scrollAreaWidget=new QWidget();
	QVBoxLayout *scrollAreaWidgetLayout=new QVBoxLayout();scrollAreaWidgetLayout->setMargin(0);scrollAreaWidget->setLayout(scrollAreaWidgetLayout);
	layoutGraphicsView=new myQGraphicsViewClass(true,this);scrollAreaWidgetLayout->addWidget(layoutGraphicsView);
	layoutGraphicsView->scale(0.03,0.03);
	layoutScene=new QGraphicsScene(this);
	layoutCscrollArea->setWidget(scrollAreaWidget);
	layoutGraphicsView->setScene(layoutScene);//setScene
	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr(""),scrollAreaWidget);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	//	return;
	//zoomExtentsBtn->setMaximumWidth(250);
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));
	//
	gItemClass *layoutGraphicsItem=new gItemClass(":dxfs/pincer1/conLayout.dxf",QPen(QColor(200,200,200),0),NULL,Qt::NoBrush,Qt::NoBrush);

	layoutGraphicsItem->setZValue(-10.);
	layoutScene->addItem(layoutGraphicsItem);
	gItemClass *posTopMouseOver=new gItemClass(":dxfs/posConvMouseOver.dxf",QPen(QColor(200,200,250)),NULL,QBrush(QColor(100,110,120,150)),Qt::NoBrush);
	gItemClass *conveyor=new gItemClass(":dxfs/posConveyorTop.dxf",QPen(QColor(225,200,200),0.0),NULL,Qt::NoBrush,Qt::NoBrush);
	gItemClass *cassette=new gItemClass(":dxfs/posConvCassette.dxf",QPen(QColor(100,200,100)),NULL,Qt::NoBrush,Qt::NoBrush);
	gItemClass *sb=new gItemClass(":dxfs/posConvSbBlue.dxf",QPen(QColor(100,100,200)),NULL,Qt::NoBrush,Qt::NoBrush);
	gItemClass *profile=new gItemClass(":dxfs/posConvProfiles.dxf",QPen(QColor(100,100,200)),this,Qt::NoBrush,QBrush(QColor(225,255,255,150)));

	positionsVector<<new convPositionClass(allPos->getPosWithId(conveyor__1_id),layoutScene,0.,0.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(conveyor__2_id),layoutScene,-1350.,0.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(conveyor__4_id),layoutScene,-1350.,-8000.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(conveyor__5_id),layoutScene,-1350.,-18000.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(conveyor__6_id),layoutScene,0.,-18000.,posTopMouseOver,conveyor,cassette,sb,profile,this);

	positionsVector<<new convPositionClass(allPos->getPosWithId(pincer1_49_id),layoutScene,-3500.,0.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(pincer1_50_id),layoutScene,-3500.,-6500,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(pincer1_52_id),layoutScene,-3500.,-15000,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(pincer1_53_id),layoutScene,-3500.,-22000,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(pincer1_54_id),layoutScene,-3500.,-26000,posTopMouseOver,conveyor,cassette,sb,profile,this);


	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,303,"rConveyorState",-1,-1,+250,layoutScene,tr("Conveyor 2->1"),buttonClass::typeNormal);btnsVector.last()->myRotate(-650.f,2000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,302,"rConveyorState",-1,-1,-250,layoutScene,tr("Conveyor 1->2"),buttonClass::typeNormal);btnsVector.last()->myRotate(-650.f,2000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,236,"",-1,-1,+250,layoutScene,tr("Conveyor 4->2"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-1400.f,-3800.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,237,"",-1,-1,-250,layoutScene,tr("Conveyor 2->4"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-1400.f,-3800.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,436,"",-1,-1,+250,layoutScene,tr("Conveyor 5->4"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-1400.f,-13000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,437,"",-1,-1,-250,layoutScene,tr("Conveyor 4->5"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-1400.f,-13000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,702,"",-1,-1,+250,layoutScene,tr("Conveyor 5->6"),buttonClass::typeNormal);btnsVector.last()->myRotate(-650.f,-19800.f,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,703,"",-1,-1,-250,layoutScene,tr("Conveyor 6->5"),buttonClass::typeNormal);btnsVector.last()->myRotate(-650.f,-19800.f,+0.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,5326,"rConveyorState",53,30,+250,layoutScene,tr("Side Moving East"),buttonClass::typeNormal);btnsVector.last()->myRotate(-2000.f,-22000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,5327,"rConveyorState",53,31,-250,layoutScene,tr("Side Moving West"),buttonClass::typeNormal);btnsVector.last()->myRotate(-2000.f,-22000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,4936,"",-1,-1,+250,layoutScene,tr("Conveyor 50->49"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-3500.f,-3300.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,4937,"",-1,-1,-250,layoutScene,tr("Conveyor 49->50"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-3500.f,-3300.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,5036,"",-1,-1,+250,layoutScene,tr("Conveyor 52->50"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-3500.f,-11000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,5037,"",-1,-1,-250,layoutScene,tr("Conveyor 50->52"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-3500.f,-11000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,5236,"",-1,-1,+250,layoutScene,tr("Conveyor 53->52"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-3500.f,-19000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,5237,"",-1,-1,-250,layoutScene,tr("Conveyor 52->53"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-3500.f,-19000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,126,"rConveyorState",1,14,+250,layoutScene,tr("Conveyor 1 up"),buttonClass::typeNormal);	btnsVector.last()->myRotate(+1000.f,0.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,127,"rConveyorState",1,15,-250,layoutScene,tr("Conveyor 1 down"),buttonClass::typeNormal);btnsVector.last()->myRotate(+1000.f,0.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,226,"rConveyorState",2,14,+250,layoutScene,tr("Conveyor 2 up"),buttonClass::typeNormal);  btnsVector.last()->myRotate(-2000.f,0.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,227,"rConveyorState",2,15,-250,layoutScene,tr("Conveyor 2 down"),buttonClass::typeNormal);btnsVector.last()->myRotate(-2000.f,0.f,+45.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,526,"rConveyorState",5,14,+250,layoutScene,tr("Conveyor 5 up"),	buttonClass::typeNormal);	btnsVector.last()->myRotate(-2000.f,-17800.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,527,"rConveyorState",5,15,-250,layoutScene,tr("Conveyor 5 down"),buttonClass::typeNormal);btnsVector.last()->myRotate(-2000.f,-17800.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,626,"rConveyorState",6,14,+250,layoutScene,tr("Conveyor 6 up"),	buttonClass::typeNormal);	btnsVector.last()->myRotate(+1000.f,-17800.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,627,"rConveyorState",6,15,-250,layoutScene,tr("Conveyor 6 down"),buttonClass::typeNormal);btnsVector.last()->myRotate(+1000.f,-17800.f,+45.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,5026,"rConveyorState",50,15,+250,layoutScene,tr("Conveyor 50 left" ),buttonClass::typeNormal);btnsVector.last()->myRotate(-3500.,-8500,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"rPcCom",9,5027,"rConveyorState",50,14,-250,layoutScene,tr("Conveyor 50 right"),buttonClass::typeNormal);btnsVector.last()->myRotate(-3500.,-8500,+0.f);btnsVector.last()->setMaintenanceMode(true);

	getPosWithId(conveyor__1_id)->addButton(tuxip->getIp(),"rPcCom",9,102,"rConveyorState",1,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(conveyor__1_id)->addButton(tuxip->getIp(),"rPcCom",9,103,"rConveyorState",1,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(conveyor__1_id)->addProximitySwitch(4,-1500.);

	getPosWithId(conveyor__2_id)->addButton(tuxip->getIp(),"rPcCom",9,202,"rConveyorState",2,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(conveyor__2_id)->addButton(tuxip->getIp(),"rPcCom",9,203,"rConveyorState",2,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(conveyor__2_id)->addProximitySwitch(4,-1500.);

	getPosWithId(conveyor__4_id)->addButton(tuxip->getIp(),"rPcCom",9,402,"rConveyorState",4,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(conveyor__4_id)->addButton(tuxip->getIp(),"rPcCom",9,403,"rConveyorState",4,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(conveyor__4_id)->addProximitySwitch(5,+500.);

	getPosWithId(conveyor__5_id)->addButton(tuxip->getIp(),"rPcCom",9,502,"rConveyorState",5,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(conveyor__5_id)->addButton(tuxip->getIp(),"rPcCom",9,503,"rConveyorState",5,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(conveyor__5_id)->addProximitySwitch(5,+1500.);

	getPosWithId(conveyor__6_id)->addButton(tuxip->getIp(),"rPcCom",9,602,"rConveyorState",6,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(conveyor__6_id)->addButton(tuxip->getIp(),"rPcCom",9,603,"rConveyorState",6,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(conveyor__6_id)->addProximitySwitch(5,+1500.);

	getPosWithId(pincer1_49_id)->addButton(tuxip->getIp(),"rPcCom",9,4902,"rConveyorState",49,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(pincer1_49_id)->addButton(tuxip->getIp(),"rPcCom",9,4903,"rConveyorState",49,17,+1000.,tr("BWD"),buttonClass::typeNormal);

	getPosWithId(pincer1_50_id)->addButton(tuxip->getIp(),"rPcCom",9,5002,"rConveyorState",50,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(pincer1_50_id)->addButton(tuxip->getIp(),"rPcCom",9,5003,"rConveyorState",50,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(pincer1_50_id)->addProximitySwitch(5,+1500.);

	getPosWithId(pincer1_52_id)->addButton(tuxip->getIp(),"rPcCom",9,5202,"rConveyorState",52,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(pincer1_52_id)->addButton(tuxip->getIp(),"rPcCom",9,5203,"rConveyorState",52,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(pincer1_52_id)->addProximitySwitch(5,+1500.);
	getPosWithId(pincer1_52_id)->addProximitySwitch(4,-1500.);

	getPosWithId(pincer1_53_id)->addButton(tuxip->getIp(),"rPcCom",9,5302,"rConveyorState",53,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(pincer1_53_id)->addButton(tuxip->getIp(),"rPcCom",9,5303,"rConveyorState",53,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(pincer1_53_id)->addProximitySwitch(5,+1500.);
//	getPosWithId(pincer1_53_id)->addProximitySwitch(4,-1500.);

	getPosWithId(pincer1_54_id)->addButton(tuxip->getIp(),"rPcCom",9,5402,"rConveyorState",54,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(pincer1_54_id)->addButton(tuxip->getIp(),"rPcCom",9,5403,"rConveyorState",54,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(pincer1_54_id)->addProximitySwitch(5,+1500.);

	getPosWithId(conveyor__1_id)->myRotate(0.,0.,-90.);
	getPosWithId(conveyor__2_id)->myRotate(0.,0.,-90.);
	getPosWithId(conveyor__4_id)->myRotate(0.,0.,-90.);
	getPosWithId(conveyor__5_id)->myRotate(0.,0.,-90.);
	getPosWithId(conveyor__6_id)->myRotate(0.,0.,-90.);
	getPosWithId(pincer1_49_id)->myRotate(0.,0.,-90.);
	getPosWithId(pincer1_50_id)->myRotate(0.,0.,-90.);
	getPosWithId(pincer1_52_id)->myRotate(0.,0.,-90.);
	getPosWithId(pincer1_53_id)->myRotate(0.,0.,-90.);
	getPosWithId(pincer1_54_id)->myRotate(0.,0.,-90.);
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimerSlot()));
	mainTimer->start(1000);
}
pincer1ConvPositionsClass::~pincer1ConvPositionsClass()
{
}
void pincer1ConvPositionsClass::resetGraphicsViewZoom()
{
	layoutScene->setSceneRect(layoutScene->itemsBoundingRect());
	layoutGraphicsView->fitInView(layoutGraphicsView->sceneRect(),Qt::KeepAspectRatio);
}
convPositionClass* pincer1ConvPositionsClass::getPosWithId(int id)
{
	convPositionClass *ans=NULL;
	for (int i=0;i<positionsVector.count();i++)
	{
		if (positionsVector[i]->getGlobalId()==id)
		{
			ans=positionsVector[i];
			break;
		}
	}
	return ans;
}
void pincer1ConvPositionsClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==tuxip->getIp())
	{
		for (int i=0;i<positionsVector.count();i++)
			positionsVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<movesVector.count();i++)
			movesVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<btnsVector.count();i++)
			btnsVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<proxVector.count();i++)
			proxVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<bitLogVectors.count();i++)
			bitLogVectors[i]->updateValue(table,index,value.toInt());
		//if ( (table=="rConveyorState")&&(index==53) )
		//{
		//	bool isLeft=getValueOfBit(value.toInt(),20);
		//	bool isRight=getValueOfBit(value.toInt(),21);
		//	if (isLeft)
		//		getPosWithId(pincer1_53_id)->myRotate(-22000,-3500.,-90);
		//	if (isRight)
		//		getPosWithId(pincer1_53_id)->myRotate(-22000,-3500.+2150,-90);
		//}
	}
}
//int angle=0.;
void pincer1ConvPositionsClass::onMainTimerSlot()
{
}

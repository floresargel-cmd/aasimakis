#include "pch.h"
#include "pincer2Conv.h"
pincer2ConvPositionsClass::pincer2ConvPositionsClass(tuxipClass *tuxip_,allPossClass *allPos_,QWidget *p):QWidget(p)
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

	QGroupBox *westGroupBox=new QGroupBox(tr("West 8-13"));leftScrollAreaWidgetL->addWidget(westGroupBox);
	QVBoxLayout *westGroupBoxL=new QVBoxLayout();westGroupBoxL->setMargin(2);westGroupBox->setLayout(westGroupBoxL);
	{
	QGroupBox *modeGroupBox=new QGroupBox(tr("Mode"));westGroupBoxL->addWidget(modeGroupBox);
	QVBoxLayout *modeGroupBoxL=new QVBoxLayout();modeGroupBoxL->setMargin(2);modeGroupBox->setLayout(modeGroupBoxL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",tuxip,"pcCom",9,1,0,"","rConveyorState",8,0,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",tuxip,"pcCom",9,2,0,"","rConveyorState",8,1,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("No alarms"),"",tuxip,"pcCom",9,3,0,"","rConveyorState",8,6,"","","","","",buttonStyleRed,buttonStyleActive,tr("Reset alarms"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	bitLogVectors<<new bitLogVectorClass("pcCom",QString("pincer2ConCommandsTriple.txt"),bitClass::TYPE_SHOW_HIDE,styleLabelG,styleLabelGr,"","Commands","","",true,bitLogHisoryClass::typeMySql,this);leftScrollAreaWidgetL->addWidget(bitLogVectors.last());
	bitLogVectors<<new bitLogVectorClass("pcCom",QString("pincer2ConAlarmsTriple.txt"),bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,"","Errors","","",true,bitLogHisoryClass::typeMySql,this);leftScrollAreaWidgetL->addWidget(bitLogVectors.last());
	}
	QGroupBox *eastGroupBox=new QGroupBox(tr("East 15-18"));leftScrollAreaWidgetL->addWidget(eastGroupBox);
	QVBoxLayout *eastGroupBoxL=new QVBoxLayout();eastGroupBoxL->setMargin(2);eastGroupBox->setLayout(eastGroupBoxL);
	{
	QGroupBox *modeGroupBox=new QGroupBox(tr("Mode"));eastGroupBoxL->addWidget(modeGroupBox);
	QVBoxLayout *modeGroupBoxL=new QVBoxLayout();modeGroupBoxL->setMargin(2);modeGroupBox->setLayout(modeGroupBoxL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",tuxip,"pcCom",10,11,0,"","rConveyorState",15,0,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",tuxip,"pcCom",10,12,0,"","rConveyorState",15,1,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("No alarms"),"",tuxip,"pcCom",10,13,0,"","rConveyorState",15,6,"","","","","",buttonStyleRed,buttonStyleActive,tr("Reset alarms"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Take SB in"),"",tuxip,"pcCom",10,21,0,"","rConveyorState",21,4,"","","","","",buttonStyle,buttonStyleActive,tr("Take SB in"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Take SB out"),"",tuxip,"pcCom",10,20,0,"","rConveyorState",20,22,"","","","","",buttonStyle,buttonStyleActive,tr("Take SB out"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	bitLogVectors<<new bitLogVectorClass("pcCom",QString("pincer2ConCommandsDouble.txt"),bitClass::TYPE_SHOW_HIDE,styleLabelG,styleLabelGr,"","Commands","","",true,bitLogHisoryClass::typeMySql,this);leftScrollAreaWidgetL->addWidget(bitLogVectors.last());
	bitLogVectors<<new bitLogVectorClass("pcCom",QString("pincer2ConAlarmsDouble.txt"),bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,"","Errors","","",true,bitLogHisoryClass::typeMySql,this);leftScrollAreaWidgetL->addWidget(bitLogVectors.last());
	}

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
	gItemClass *layoutGraphicsItem=new gItemClass(":dxfs/pincer2/conLayout.dxf",QPen(QColor(200,200,200),0),NULL,Qt::NoBrush,Qt::NoBrush);

	layoutGraphicsItem->setZValue(-10.);
	layoutScene->addItem(layoutGraphicsItem);
	gItemClass *posTopMouseOver=new gItemClass(":dxfs/posConvMouseOver.dxf",QPen(QColor(200,200,250)),NULL,QBrush(QColor(100,110,120,150)),Qt::NoBrush);
	gItemClass *conveyor=new gItemClass(":dxfs/posConveyorTop.dxf",QPen(QColor(225,200,200),0.0),NULL,Qt::NoBrush,Qt::NoBrush);
	gItemClass *cassette=new gItemClass(":dxfs/posConvCassette.dxf",QPen(QColor(100,200,100)),NULL,Qt::NoBrush,Qt::NoBrush);
	gItemClass *sb=new gItemClass(":dxfs/posConvSbBlue.dxf",QPen(QColor(100,100,200)),NULL,Qt::NoBrush,Qt::NoBrush);
	gItemClass *profile=new gItemClass(":dxfs/posConvProfiles.dxf",QPen(QColor(100,100,200)),this,Qt::NoBrush,QBrush(QColor(225,255,255,150)));

	positionsVector<<new convPositionClass(allPos->getPosWithId(cassetteDoubleExit_18_id),layoutScene,0.,0.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(cassetteDoubleExit_17_id),layoutScene,+800.,0.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(pincer2_22_id),layoutScene,+2600.,0.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(pincer2_21_id),layoutScene,+2600.,-5000.,posTopMouseOver,conveyor,cassette,sb,profile,this);

	positionsVector<<new convPositionClass(allPos->getPosWithId(cassetteTripleExit_13_id),layoutScene,-7500.					,0.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(cassetteTripleExit_10_id),layoutScene,-8320.					,0.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(cassetteTripleExit__9_id),layoutScene,-9140.					,0.,posTopMouseOver,conveyor,cassette,sb,profile,this);

	positionsVector<<new convPositionClass(allPos->getPosWithId(cassetteDoubleExit_16_id),layoutScene,+800.						,7000.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(cassetteDoubleExit_15_id),layoutScene,0.							,7000.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(cassetteTripleExit_12_id),layoutScene,-7500.					,7000.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(cassetteTripleExit_11_id),layoutScene,-8320.					,7000.,posTopMouseOver,conveyor,cassette,sb,profile,this);
	positionsVector<<new convPositionClass(allPos->getPosWithId(cassetteTripleExit__8_id),layoutScene,-9140.					,7000.,posTopMouseOver,conveyor,cassette,sb,profile,this);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,2136,"rConveyorState",-1,-1,+250,layoutScene,tr("Conveyor 21->22"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(2600.,-2500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,2137,"rConveyorState",-1,-1,-250,layoutScene,tr("Conveyor 22->21"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(2600.,-2500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1902,"rConveyorState",19,16,+250,layoutScene,tr("Conveyor 18->17"),buttonClass::typeNormal);btnsVector.last()->myRotate(400.,-2000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1903,"rConveyorState",19,17,-250,layoutScene,tr("Conveyor 17->18"),buttonClass::typeNormal);btnsVector.last()->myRotate(400.,-2000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1402,"rConveyorState",14,16,+250,layoutScene,tr("Conveyor 13->9"),buttonClass::typeNormal);btnsVector.last()->myRotate(-8320.,-2000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1403,"rConveyorState",14,17,-250,layoutScene,tr("Conveyor 9->13"),buttonClass::typeNormal);btnsVector.last()->myRotate(-8320.,-2000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1636,"rConveyorState",-1,-1,+250,layoutScene,tr("Conveyor 17->16"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(800.,3500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1637,"rConveyorState",-1,-1,-250,layoutScene,tr("Conveyor 16->17"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(800.,3500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1536,"rConveyorState",-1,-1,+250,layoutScene,tr("Conveyor 18->15"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(0.,3500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1537,"rConveyorState",-1,-1,-250,layoutScene,tr("Conveyor 15->18"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(0.,3500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1236,"rConveyorState",-1,-1,+250,layoutScene,tr("Conveyor 13->12"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-7500.,3500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1237,"rConveyorState",-1,-1,-250,layoutScene,tr("Conveyor 12->13"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-7500.,3500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1036,"rConveyorState",-1,-1,+250,layoutScene,tr("Conveyor 11->10"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-8320.,3500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1037,"rConveyorState",-1,-1,-250,layoutScene,tr("Conveyor 10->11"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-8320.,3500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9, 836,"rConveyorState",-1,-1,+250,layoutScene,tr("Conveyor 9->8"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-9140.,3500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9, 837,"rConveyorState",-1,-1,-250,layoutScene,tr("Conveyor 8->9"),buttonClass::typeConveyorDouble);btnsVector.last()->myRotate(-9140.,3500.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,2003,"rConveyorState",20,17,+250,layoutScene,tr("Conveyor 20 right"),buttonClass::typeNormal);btnsVector.last()->myRotate(-1500.,0.f,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,2002,"rConveyorState",20,16,-250,layoutScene,tr("Conveyor 20 left"), buttonClass::typeNormal);btnsVector.last()->myRotate(-1500.,0.f,+0.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1826,"rConveyorState",18,14,+250,layoutScene,tr("Conveyor 18 up"),buttonClass::typeNormal);	 btnsVector.last()->myRotate(			0.f,-3300.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1827,"rConveyorState",18,15,-250,layoutScene,tr("Conveyor 18 down"),buttonClass::typeNormal);btnsVector.last()->myRotate(			0.f,-3300.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1726,"rConveyorState",17,14,+250,layoutScene,tr("Conveyor 17 up"),buttonClass::typeNormal);	 btnsVector.last()->myRotate(		800.f,-3300.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1727,"rConveyorState",17,15,-250,layoutScene,tr("Conveyor 17 down"),buttonClass::typeNormal);btnsVector.last()->myRotate(		800.f,-3300.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1326,"rConveyorState",13,14,+250,layoutScene,tr("Conveyor 13 up"),buttonClass::typeNormal);	 btnsVector.last()->myRotate(	-7500.f,-3300.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1327,"rConveyorState",13,15,-250,layoutScene,tr("Conveyor 13 down"),buttonClass::typeNormal);btnsVector.last()->myRotate(	-7500.f,-3300.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1026,"rConveyorState",10,14,+250,layoutScene,tr("Conveyor 10 up"),buttonClass::typeNormal);	 btnsVector.last()->myRotate(	-8320.f,-3300.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1027,"rConveyorState",10,15,-250,layoutScene,tr("Conveyor 10 down"),buttonClass::typeNormal);btnsVector.last()->myRotate(	-8320.f,-3300.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,926 ,"rConveyorState", 9,14,+250,layoutScene,tr("Conveyor 9 up"),buttonClass::typeNormal);	 btnsVector.last()->myRotate(	-9140.f,-3300.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,927 ,"rConveyorState", 9,15,-250,layoutScene,tr("Conveyor 9 down"),buttonClass::typeNormal); btnsVector.last()->myRotate(	-9140.f,-3300.f,+45.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1632 ,"rConveyorState",16,14,+250,layoutScene,tr("Stop up"),buttonClass::typeNormal);	 btnsVector.last()->myRotate(		800.f,9500.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",10,1633 ,"rConveyorState",16,15,-250,layoutScene,tr("Stop down"),buttonClass::typeNormal); btnsVector.last()->myRotate(	800.f,9500.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1132,"rConveyorState",11,14,+250,layoutScene,tr("Stop up"),buttonClass::typeNormal);	 btnsVector.last()->myRotate(	-8320.f,9500.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,tuxip->getIp(),"pcCom",9,1133,"rConveyorState",11,15,-250,layoutScene,tr("Stop down"),buttonClass::typeNormal); btnsVector.last()->myRotate(-8320.f,9500.f,+45.f);btnsVector.last()->setMaintenanceMode(true);

	getPosWithId(cassetteDoubleExit_18_id)->addButton(tuxip->getIp(),"pcCom",10,1802,"rConveyorState",18,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(cassetteDoubleExit_18_id)->addButton(tuxip->getIp(),"pcCom",10,1803,"rConveyorState",18,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(cassetteDoubleExit_18_id)->addProximitySwitch(5,+1500.);

	getPosWithId(cassetteDoubleExit_17_id)->addButton(tuxip->getIp(),"pcCom",10,1702,"rConveyorState",17,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(cassetteDoubleExit_17_id)->addButton(tuxip->getIp(),"pcCom",10,1703,"rConveyorState",17,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(cassetteDoubleExit_17_id)->addProximitySwitch(5,+1500.);

	getPosWithId(pincer2_22_id)->addButton(tuxip->getIp(),"pcCom",10,2202,"rConveyorState",22,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(pincer2_22_id)->addButton(tuxip->getIp(),"pcCom",10,2203,"rConveyorState",22,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(pincer2_22_id)->addProximitySwitch(4,-1500.,"Photosensor",QPen(QColor(255,100,100),100));
	getPosWithId(pincer2_22_id)->addProximitySwitch(5,+1500.,"Photosensor",QPen(QColor(255,100,100),100));

	getPosWithId(pincer2_21_id)->addButton(tuxip->getIp(),"pcCom",10,2102,"rConveyorState",21,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(pincer2_21_id)->addButton(tuxip->getIp(),"pcCom",10,2103,"rConveyorState",21,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(pincer2_21_id)->addProximitySwitch(4,-1500.,"Photosensor",QPen(QColor(255,100,100),100));

	getPosWithId(cassetteTripleExit_13_id)->addButton(tuxip->getIp(),"pcCom",9,1302,"rConveyorState",13,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit_13_id)->addButton(tuxip->getIp(),"pcCom",9,1303,"rConveyorState",13,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit_13_id)->addProximitySwitch(5,+1750.);
	getPosWithId(cassetteTripleExit_13_id)->addProximitySwitch(10,0.,"Photosensor",QPen(QColor(255,100,100),100));

	getPosWithId(cassetteTripleExit_10_id)->addButton(tuxip->getIp(),"pcCom",9,1002,"rConveyorState",10,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit_10_id)->addButton(tuxip->getIp(),"pcCom",9,1003,"rConveyorState",10,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit_10_id)->addProximitySwitch(4,+1500.);
	getPosWithId(cassetteTripleExit_10_id)->addProximitySwitch(5,+1750.);

	getPosWithId(cassetteTripleExit__9_id)->addButton(tuxip->getIp(),"pcCom",9, 902,"rConveyorState",9,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit__9_id)->addButton(tuxip->getIp(),"pcCom",9, 903,"rConveyorState",9,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit__9_id)->addProximitySwitch(5,+1750.);
	getPosWithId(cassetteTripleExit__9_id)->addProximitySwitch(10,0.,"Photosensor",QPen(QColor(255,100,100),100));

	getPosWithId(cassetteDoubleExit_16_id)->addButton(tuxip->getIp(),"pcCom",10,1602,"rConveyorState",16,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(cassetteDoubleExit_16_id)->addButton(tuxip->getIp(),"pcCom",10,1603,"rConveyorState",16,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(cassetteDoubleExit_16_id)->addProximitySwitch(4,-1500.);

	getPosWithId(cassetteDoubleExit_15_id)->addButton(tuxip->getIp(),"pcCom",10,1502,"rConveyorState",15,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(cassetteDoubleExit_15_id)->addButton(tuxip->getIp(),"pcCom",10,1503,"rConveyorState",15,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(cassetteDoubleExit_15_id)->addProximitySwitch(5,+1500.);

	getPosWithId(cassetteTripleExit_12_id)->addButton(tuxip->getIp(),"pcCom",9,1202,"rConveyorState",12,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit_12_id)->addButton(tuxip->getIp(),"pcCom",9,1203,"rConveyorState",12,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit_12_id)->addProximitySwitch(5,+1750.);

	getPosWithId(cassetteTripleExit_11_id)->addButton(tuxip->getIp(),"pcCom",9,1102,"rConveyorState",11,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit_11_id)->addButton(tuxip->getIp(),"pcCom",9,1103,"rConveyorState",11,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit_11_id)->addProximitySwitch(5,-1500.);

	getPosWithId(cassetteTripleExit__8_id)->addButton(tuxip->getIp(),"pcCom",9, 802,"rConveyorState",8,16,-1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit__8_id)->addButton(tuxip->getIp(),"pcCom",9, 803,"rConveyorState",8,17,+1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(cassetteTripleExit__8_id)->addProximitySwitch(5,+1750.);



	getPosWithId(cassetteDoubleExit_18_id)->myRotate(0.,0.,-90.);
	getPosWithId(cassetteDoubleExit_17_id)->myRotate(0.,0.,-90.);
	getPosWithId(pincer2_22_id)->myRotate(0.,0.,-90.);
	getPosWithId(pincer2_21_id)->myRotate(0.,0.,-90.);
	getPosWithId(cassetteTripleExit_13_id)->myRotate(0.,0.,-90.);
	getPosWithId(cassetteTripleExit_10_id)->myRotate(0.,0.,-90.);
	getPosWithId(cassetteTripleExit__9_id)->myRotate(0.,0.,-90.);

	getPosWithId(cassetteDoubleExit_16_id)->myRotate(0.,0.,-90.);
	getPosWithId(cassetteDoubleExit_15_id)->myRotate(0.,0.,-90.);
	getPosWithId(cassetteTripleExit_12_id)->myRotate(0.,0.,-90.);
	getPosWithId(cassetteTripleExit_11_id)->myRotate(0.,0.,-90.);
	getPosWithId(cassetteTripleExit__8_id)->myRotate(0.,0.,-90.);


	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimerSlot()));
	mainTimer->start(1000);
}
pincer2ConvPositionsClass::~pincer2ConvPositionsClass()
{
}
void pincer2ConvPositionsClass::resetGraphicsViewZoom()
{
	layoutScene->setSceneRect(layoutScene->itemsBoundingRect());
	layoutGraphicsView->fitInView(layoutGraphicsView->sceneRect(),Qt::KeepAspectRatio);
}
convPositionClass* pincer2ConvPositionsClass::getPosWithId(int id)
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
void pincer2ConvPositionsClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
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
		if ( (table=="bConveyorState")&&(index==5) )
		{
			bool isHorizontal=false;
			bool isVertical=false;
			if (isHorizontal)
			{
			}
			else if (isVertical)
			{
			}
			else
			{
				//getPosWithId(rotatingSouthPlcId)->myRotate(0.f,+900.f,-45.);
				//getPosWithId(rotatingNorthPlcId)->myRotate(0.f,-900.f,-45.);
			}
		}
	}
}
//int angle=0.;
void pincer2ConvPositionsClass::onMainTimerSlot()
{

}

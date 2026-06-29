#include "pch.h"
#include "allConvPos.h"
extern QSplashScreen *splash;
allConvPositionsClass::allConvPositionsClass(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,conveyorsGroupTypeEnum conveyorsGroupType_,QWidget *p):QWidget(p)
{
	splash->showMessage(tr("Creating Conveyors Widget..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	craneTuxip=craneTuxip_;
	forkTuxip=forkTuxip_;
	conveyorsGroupType=conveyorsGroupType_;
	inMaintenanceMode=false;
	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);
	QSplitter *mainSplitter=new QSplitter(Qt::Horizontal,this);mainLayout->addWidget(mainSplitter);
	mainSplitter->setStyleSheet("QSplitter::handle:Horizontal{height:3px;background-color:#777777;} QSplitterHandle:hover{} QSplitter::handle:Horizontal:hover{height:3px;background-color:#339966;}");
	mainLayout->addWidget(mainSplitter);
	QWidget *leftWidget=new QWidget();mainSplitter->addWidget(leftWidget);
	QVBoxLayout *leftWidgetL=new QVBoxLayout();leftWidget->setLayout(leftWidgetL);
	QWidget *rightWidget=new QWidget();mainSplitter->addWidget(rightWidget);
	QVBoxLayout *rightWidgetL=new QVBoxLayout();rightWidget->setLayout(rightWidgetL);

	QGroupBox *modeGroupBox=new QGroupBox(tr("Mode"));leftWidgetL->addWidget(modeGroupBox);
	QVBoxLayout *modeGroupBoxL=new QVBoxLayout();modeGroupBoxL->setMargin(2);modeGroupBox->setLayout(modeGroupBoxL);
	if (conveyorsGroupType==conveyorsGroupTypeEnum::eastGroup)
	{
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",craneTuxip,"pcCom",9,1,0,"","ConveyorState",30,0,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",craneTuxip,"pcCom",9,2,0,"","ConveyorState",30,1,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("No alarms"),"",craneTuxip,"pcCom",9,3,0,"","ConveyorState",30,6,"","","","","",buttonStyleRed,buttonStyleActive,tr("Reset alarms"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
		commandsVector=new bitLogVectorClass("pcCom",QString("conveyorCommands%1.txt").arg(9),bitClass::TYPE_SHOW_HIDE,styleLabelG,styleLabelGr,"","Commands","","",true,bitLogHisoryClass::typeMsSql,this);leftWidgetL->addWidget(commandsVector);
		bitErrorsVector=new bitLogVectorClass("pcCom",QString("conveyorAlarms%1.txt").arg(9),bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,"","Errors","","",true,bitLogHisoryClass::typeMsSql,this);leftWidgetL->addWidget(bitErrorsVector);
		bitWarningsVector=new bitLogVectorClass("pcCom",QString("conveyorWarnings%1.txt").arg(9),bitClass::TYPE_SHOW_HIDE,styleLabelO,styleLabelGr,"","Warnings","","",true,bitLogHisoryClass::typeMsSql,this);leftWidgetL->addWidget(bitWarningsVector);
	}	
	if (conveyorsGroupType==conveyorsGroupTypeEnum::packingGroup)
	{
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",craneTuxip,"pcCom",29,1,0,"","ConveyorState",40,0,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",craneTuxip,"pcCom",29,2,0,"","ConveyorState",40,1,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("No alarms"),"",craneTuxip,"pcCom",29,3,0,"","ConveyorState",40,8,"","","","","",buttonStyleRed,buttonStyleActive,tr("Reset alarms"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
		commandsVector=new bitLogVectorClass("pcCom",QString("conveyorCommands%1.txt").arg(29),bitClass::TYPE_SHOW_HIDE,styleLabelG,styleLabelGr,"","Commands","","",true,bitLogHisoryClass::typeMsSql,this);leftWidgetL->addWidget(commandsVector);
		bitErrorsVector=new bitLogVectorClass("pcCom",QString("conveyorAlarms%1.txt").arg(29),bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,"","Errors","","",true,bitLogHisoryClass::typeMsSql,this);leftWidgetL->addWidget(bitErrorsVector);
		bitWarningsVector=new bitLogVectorClass("pcCom",QString("conveyorWarnings%1.txt").arg(29),bitClass::TYPE_SHOW_HIDE,styleLabelO,styleLabelGr,"","Warnings","","",true,bitLogHisoryClass::typeMsSql,this);leftWidgetL->addWidget(bitWarningsVector);
	}	
	QScrollArea *scrollArea=new QScrollArea();scrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);rightWidgetL->addWidget(scrollArea);
	scrollArea->setWidgetResizable(true);
	QWidget *scrollAreaWidget=new QWidget();
	QVBoxLayout *scrollAreaWidgetLayout=new QVBoxLayout();scrollAreaWidgetLayout->setMargin(0);scrollAreaWidget->setLayout(scrollAreaWidgetLayout);
	layoutGraphicsView=new myQGraphicsViewClass(true,this);scrollAreaWidgetLayout->addWidget(layoutGraphicsView);
	layoutGraphicsView->scale(0.015,0.015);
	layoutScene=new QGraphicsScene(this);
	scrollArea->setWidget(scrollAreaWidget);
	layoutGraphicsView->setScene(layoutScene);//setScene
	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr(""),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));
	//
	float xyM=1.;
	dxfClass *layoutGraphicsItem=new dxfClass(":dxfs/conv/cLayout.dxf",QPen(QColor(200,200,200),0),false,false,QColor(),this);
	layoutGraphicsItem->setZValue(-10.);
	layoutScene->addItem(layoutGraphicsItem);
	cWBasket=new dxfClass(":dxfs/conv/c1WithBasket.dxf",QPen(QColor(100,100,175),0.0),false,false,QColor(),this);
	c=new dxfClass(":dxfs/conv/c1.dxf",QPen(QColor(150,125,125 ),0.0),false,false,QColor(),this);
	cLong=new dxfClass(":dxfs/conv/c1Long.dxf",QPen(QColor(150,125,125 ),0.0),false,false,QColor(),this);
	cDouble=new dxfClass(":dxfs/conv/c1Double.dxf",QPen(QColor(150,125,125 ),0.0),false,false,QColor(),this);
	cWFull=new dxfClass(":dxfs/conv/c1WithFullBasket.dxf",QPen(QColor(100,150,100),0.0),false,false,QColor(),this);
	cMOver=new dxfClass(":dxfs/conv/c1MouseOver.dxf",QPen(QColor(200,200,255),0.0),true,true,QColor(225,225,255));
	triGraphicsItem=new dxfClass(":dxfs/conv/tri.dxf",QPen(QColor(200,100,100),0.0),true,true,QColor(200,100,100),this);
	dxfClass *proxOnItem=new dxfClass(":dxfs/conv/proximitySwitchOn.dxf",QPen(QColor(75,150,75),25),false,false,QColor(),this);
	dxfClass *proxOffItem=new dxfClass(":dxfs/conv/proximitySwitchOff.dxf",QPen(QColor(150,150,150),10),false,false,QColor(),this);
	//conveyorsGroupBlueBasket
	if (conveyorsGroupType==conveyorsGroupTypeEnum::eastGroup)
	{
		positionsVector<<new convPositionClass(craneTuxip,nullptr,layoutScene,34,tr("Export 1"),packetCraneIp,"ConveyorBasket",34,cMOver,c,cWBasket,cWFull,2000.f,7000.f,this);
		positionsVector<<new convPositionClass(craneTuxip,nullptr,layoutScene,33,tr("Imports "),packetCraneIp,"ConveyorBasket",33,cMOver,c,cWBasket,cWFull,1000.f,7000.f,this);
		positionsVector<<new convPositionClass(craneTuxip,nullptr,layoutScene,30,tr("Export 2"),packetCraneIp,"ConveyorBasket",30,cMOver,c,cWBasket,cWFull,0.000f,7000.f,this);

		positionsVector<<new convPositionClass(craneTuxip,nullptr,layoutScene,35,tr("Export back 1"),packetCraneIp,"ConveyorBasket",35,cMOver,c,cWBasket,cWFull,2000.f,0.f,this);
		positionsVector<<new convPositionClass(craneTuxip,nullptr,layoutScene,32,tr("Imports back" ),packetCraneIp,"ConveyorBasket",32,cMOver,c,cWBasket,cWFull,1000.f,0.f,this);
		positionsVector<<new convPositionClass(craneTuxip,nullptr,layoutScene,31,tr("Export back 2"),packetCraneIp,"ConveyorBasket",31,cMOver,c,cWBasket,cWFull,0.000f,0.f,this);

		getPosWithId(30)->addButton(packetCraneIp,"pcCom",9,3002,"ConveyorState",30,16,-1500.f,"Moves conveyor FWD",buttonClass::typeConveyorDouble);
		getPosWithId(30)->addButton(packetCraneIp,"pcCom",9,3003,"ConveyorState",30,17,+1500.f,"Moves conveyor BWD",buttonClass::typeConveyorDouble);

		getPosWithId(31)->addButton(packetCraneIp,"pcCom",9,3102,"ConveyorState",31,16,-1500.f,"Moves conveyor FWD",buttonClass::typeConveyorDouble);
		getPosWithId(31)->addButton(packetCraneIp,"pcCom",9,3103,"ConveyorState",31,17,+1500.f,"Moves conveyor BWD",buttonClass::typeConveyorDouble);

		getPosWithId(32)->addButton(packetCraneIp,"pcCom",9,3202,"ConveyorState",32,16,-1500.f,"Moves conveyor FWD",buttonClass::typeConveyorDouble);
		getPosWithId(32)->addButton(packetCraneIp,"pcCom",9,3203,"ConveyorState",32,17,+1500.f,"Moves conveyor BWD",buttonClass::typeConveyorDouble);
	
		getPosWithId(33)->addButton(packetCraneIp,"pcCom",9,3302,"ConveyorState",33,16,-1500.f,"Moves conveyor FWD",buttonClass::typeConveyorDouble);
		getPosWithId(33)->addButton(packetCraneIp,"pcCom",9,3303,"ConveyorState",33,17,+1500.f,"Moves conveyor BWD",buttonClass::typeConveyorDouble);

		getPosWithId(34)->addButton(packetCraneIp,"pcCom",9,3402,"ConveyorState",34,16,-1500.f,"Moves conveyor FWD",buttonClass::typeConveyorDouble);
		getPosWithId(34)->addButton(packetCraneIp,"pcCom",9,3403,"ConveyorState",34,17,+1500.f,"Moves conveyor BWD",buttonClass::typeConveyorDouble);

		getPosWithId(35)->addButton(packetCraneIp,"pcCom",9,3502,"ConveyorState",35,16,-1500.f,"Moves conveyor FWD",buttonClass::typeConveyorDouble);
		getPosWithId(35)->addButton(packetCraneIp,"pcCom",9,3503,"ConveyorState",35,17,+1500.f,"Moves conveyor BWD",buttonClass::typeConveyorDouble);


		getPosWithId(30)->addProximitySwitch(packetCraneIp,"ConveyorState",30,4,"I200",-2500.f);
		getPosWithId(31)->addProximitySwitch(packetCraneIp,"ConveyorState",31,5,"I201",-2000.f);
		getPosWithId(31)->addProximitySwitch(packetCraneIp,"ConveyorState",31,4,"I202",-2500.f);
		getPosWithId(34)->addProximitySwitch(packetCraneIp,"ConveyorState",34,4,"I301",-2500.f);
		getPosWithId(35)->addProximitySwitch(packetCraneIp,"ConveyorState",35,5,"I205",-2000.f);
		getPosWithId(35)->addProximitySwitch(packetCraneIp,"ConveyorState",35,4,"I206",-2500.f);

		getPosWithId(32)->addProximitySwitch(packetCraneIp,"ConveyorState",32,4,"I203",-3000.f);//pros 31
		getPosWithId(32)->addProximitySwitch(packetCraneIp,"ConveyorState",32,5,"I204",-3100.f);//pros 35

		getPosWithId(33)->addProximitySwitch(packetCraneIp,"ConveyorState",33,4,"I207",+2000.f);
		getPosWithId(33)->addProximitySwitch(packetCraneIp,"ConveyorState",33,5,"I300",+2500.f);

		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",9,3332,"ConveyorState",33,14,+100,layoutScene,tr("Moves stop up")  ,buttonClass::typeNormal);				btnsVector.last()->myRotate(1000.f,10750.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",9,3333,"ConveyorState",33,15,-100,layoutScene,tr("Moves stop down"),buttonClass::typeNormal);				btnsVector.last()->myRotate(1000.f,10750.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",9,3126,"ConveyorState",31,14,+100,layoutScene,tr("Moves 31 up")  ,buttonClass::typeNormal);				btnsVector.last()->myRotate(0.f,-3500.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",9,3127,"ConveyorState",31,15,-100,layoutScene,tr("Moves 31 down"),buttonClass::typeNormal);				btnsVector.last()->myRotate(0.f,-3500.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",9,3226,"ConveyorState",32,14,+100,layoutScene,tr("Moves 32 up")  ,buttonClass::typeNormal);				btnsVector.last()->myRotate(1000.f,-3500.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",9,3227,"ConveyorState",32,15,-100,layoutScene,tr("Moves 32 down"),buttonClass::typeNormal);				btnsVector.last()->myRotate(1000.f,-3500.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",9,3526,"ConveyorState",35,14,+100,layoutScene,tr("Moves 35 up")  ,buttonClass::typeNormal);				btnsVector.last()->myRotate(2000.f,-3500.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",9,3527,"ConveyorState",35,15,-100,layoutScene,tr("Moves 35 down"),buttonClass::typeNormal);				btnsVector.last()->myRotate(2000.f,-3500.f,0.f);
	}
	if (conveyorsGroupType==conveyorsGroupTypeEnum::packingGroup)
	{
		positionsVector<<new convPositionClass(craneTuxip,nullptr,layoutScene,3040,tr("Export"),packetCraneIp,"ConveyorBasket",40,cMOver,c,cWBasket,cWFull,8000.f,0.f,this);
		positionsVector<<new convPositionClass(craneTuxip,nullptr,layoutScene,3041,tr("Export Out"),packetCraneIp,"ConveyorBasket",41,cMOver,c,cWBasket,cWFull,0.f,0.f,this);
		positionsVector<<new convPositionClass(craneTuxip,nullptr,layoutScene,3042,tr("Imports Out"),packetCraneIp,"ConveyorBasket",42,cMOver,c,cWBasket,cWFull,0.f,1000.f,this);
		positionsVector<<new convPositionClass(craneTuxip,nullptr,layoutScene,3043,tr("Imports In"),packetCraneIp,"ConveyorBasket",43,cMOver,c,cWBasket,cWFull,8000.f,1000.f,this);
		positionsVector<<new convPositionClass(craneTuxip,nullptr,layoutScene,3044,tr("Empty In"),packetCraneIp,"ConveyorBasket",44,cMOver,c,cWBasket,cWFull,8000.f,2000.f,this);
		positionsVector<<new convPositionClass(craneTuxip,forkTuxip,layoutScene,3045,tr("Empty Out"),packetCraneIp,"ConveyorBasket",45,cMOver,c,cWBasket,cWFull,0.f,2000.f,this);

		getPosWithId(3040)->addButton(packetCraneIp,"pcCom",29,4002,"ConveyorState",40,16,-1500.f,"Moves conveyor FWD",buttonClass::typeNormal);
		getPosWithId(3040)->addButton(packetCraneIp,"pcCom",29,4003,"ConveyorState",40,17,+1500.f,"Moves conveyor BWD",buttonClass::typeNormal);
		getPosWithId(3040)->addProximitySwitch(packetCraneIp,"ConveyorState",40,5,"I107 Proximity",+2500.f,700,tr("Basket Stop"));

		getPosWithId(3041)->addButton(packetCraneIp,"pcCom",29,4102,"ConveyorState",41,16,-1500.f,"Moves conveyor FWD",buttonClass::typeNormal);
		getPosWithId(3041)->addButton(packetCraneIp,"pcCom",29,4103,"ConveyorState",41,17,+1500.f,"Moves conveyor BWD",buttonClass::typeNormal);
		getPosWithId(3041)->addProximitySwitch(packetCraneIp,"ConveyorState",41,5,"I202 Photocell",2500.f,+700,tr("Basket Stop"));
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",41,4,QString("I307 Proximity"),0,layoutScene,100,tr("Basket Existance"));proxVector.last()->moveBy(0.f,-400.f);
	
		getPosWithId(3042)->addButton(packetCraneIp,"pcCom",29,4202,"ConveyorState",42,16,-1500.f,"Moves conveyor FWD",buttonClass::typeNormal);
		getPosWithId(3042)->addButton(packetCraneIp,"pcCom",29,4203,"ConveyorState",42,17,+1500.f,"Moves conveyor BWD",buttonClass::typeNormal);
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",42,4,QString("I206 Proximity"),0,layoutScene,100,tr("Basket Existance"));proxVector.last()->moveBy(-1500.f,700.f);
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",42,5,QString("I207 Proximity"),0,layoutScene,100,tr("Basket Existance"));proxVector.last()->moveBy(-1500.f,1300.f);
		
		getPosWithId(3043)->addButton(packetCraneIp,"pcCom",29,4302,"ConveyorState",43,16,-1500.f,"Moves conveyor FWD",buttonClass::typeNormal);
		getPosWithId(3043)->addButton(packetCraneIp,"pcCom",29,4303,"ConveyorState",43,17,+1500.f,"Moves conveyor BWD",buttonClass::typeNormal);
		getPosWithId(3043)->addProximitySwitch(packetCraneIp,"ConveyorState",43,4,"I103 Proximity",-2500.f,+700,tr("Basket Stop"));

		getPosWithId(3044)->addButton(packetCraneIp,"pcCom",29,4402,"ConveyorState",44,16,-1500.f,"Moves conveyor FWD",buttonClass::typeNormal);
		getPosWithId(3044)->addButton(packetCraneIp,"pcCom",29,4403,"ConveyorState",44,17,+1500.f,"Moves conveyor BWD",buttonClass::typeNormal);
		getPosWithId(3044)->addProximitySwitch(packetCraneIp,"ConveyorState",44,4,"I102 Proximity",+2500.f,700,tr("Basket Stop"));
		
		getPosWithId(3045)->addButton(packetCraneIp,"pcCom",29,4502,"ConveyorState",45,16,-1500.f,"Moves conveyor FWD",buttonClass::typeNormal);
		getPosWithId(3045)->addButton(packetCraneIp,"pcCom",29,4503,"ConveyorState",45,17,+1500.f,"Moves conveyor BWD",buttonClass::typeNormal);
		getPosWithId(3045)->addProximitySwitch(packetCraneIp,"ConveyorState",45,5,"I302 Photocell",2500.f,+700,tr("Basket Stop"));
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",45,4,QString("I306 Proximity"),0,layoutScene,100,tr("Basket Existance"));proxVector.last()->moveBy(0.f,2350.f);
//stop
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",29,4332,"ConveyorState",43,30,+100,layoutScene,tr("Moves stop up")  ,buttonClass::typeNormal);				btnsVector.last()->myRotate(11300.f,1000.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",29,4333,"ConveyorState",43,31,-100,layoutScene,tr("Moves stop down"),buttonClass::typeNormal);				btnsVector.last()->myRotate(11300.f,1000.f,0.f);
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",43,14,QString("I104 Magnetic Switch"),0,layoutScene,200,tr("Stop Is Up"));proxVector.last()->moveBy(11300.f,1500.f);
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",43,15,QString("I105 Magnetic Switch"),0,layoutScene,200,tr("Stop Is Down"));proxVector.last()->moveBy(11300.f,500.f);
		//41 up/down
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",29,4126,"ConveyorState",41,30,+100,layoutScene,tr("Moves Conveyor up")  ,buttonClass::typeNormal);btnsVector.last()->myRotate(-4000.f,0.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",29,4127,"ConveyorState",41,31,-100,layoutScene,tr("Moves Conveyor down"),buttonClass::typeNormal);btnsVector.last()->myRotate(-4000.f,0.f,0.f);
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",41,14,QString("I200 Magnetic Switch"),0,layoutScene,200,tr("Conveyor Is Up"));proxVector.last()->moveBy(-4000.f,400.f);
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",41,15,QString("I201 Magnetic Switch"),0,layoutScene,200,tr("Conveyor Is Down"));proxVector.last()->moveBy(-4000.f,-400.f);
		//42 up/down
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",29,4226,"ConveyorState",42,30,+100,layoutScene,tr("Moves Conveyor up")  ,buttonClass::typeNormal);btnsVector.last()->myRotate(-4000.f,1000.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",29,4227,"ConveyorState",42,31,-100,layoutScene,tr("Moves Conveyor down"),buttonClass::typeNormal);btnsVector.last()->myRotate(-4000.f,1000.f,0.f);
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",42,14,QString("I204 Magnetic Switch"),0,layoutScene,200,tr("Conveyor Is Up"));proxVector.last()->moveBy(-4000.f,1400.f);
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",42,15,QString("I205 Magnetic Switch"),0,layoutScene,200,tr("Conveyor Is Down"));proxVector.last()->moveBy(-4000.f,600.f);
		//45 up/down
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",29,4526,"ConveyorState",45,30,+100,layoutScene,tr("Moves Conveyor up")  ,buttonClass::typeNormal);btnsVector.last()->myRotate(-4000.f,2000.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",29,4527,"ConveyorState",45,31,-100,layoutScene,tr("Moves Conveyor down"),buttonClass::typeNormal);btnsVector.last()->myRotate(-4000.f,2000.f,0.f);
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",45,14,QString("I300 Magnetic Switch"),0,layoutScene,200,tr("Conveyor Is Up"));proxVector.last()->moveBy(-4000.f,2400.f);
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",45,15,QString("I301 Magnetic Switch"),0,layoutScene,200,tr("Conveyor Is Down"));proxVector.last()->moveBy(-4000.f,1600.f);
		//Side movement
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",29,4602,"ConveyorState",46,16,+100,layoutScene,tr("Moves Conveyor North"),buttonClass::typeNormal);btnsVector.last()->myRotate(-3200.f,1000.f,0.f);
		btnsVector<<new buttonClass(craneTuxip,packetCraneIp,"pcCom",29,4603,"ConveyorState",46,17,-100,layoutScene,tr("Moves Conveyor South"),buttonClass::typeNormal);btnsVector.last()->myRotate(-3200.f,1000.f,0.f);
		//photocells
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",42,29,QString("I106 Magnetic Switch"),0,layoutScene, 1000,tr("Length Check"),true);proxVector.last()->moveBy(-3500.f,1000.f);proxVector.last()->setPens(QPen(QColor(150,200,150),50),QPen(QColor(255,100,100),50));
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",42,28,QString("I303 Magnetic Switch"),0,layoutScene, 1000,tr("Length Check"),true);proxVector.last()->moveBy(+3500.f,1000.f);proxVector.last()->setPens(QPen(QColor(150,200,150),50),QPen(QColor(255,100,100),50));
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",42,25,QString("I304 Magnetic Switch"),0,layoutScene, 250,tr("Width Check"));proxVector.last()->moveBy(4000.f,1500.f);proxVector.last()->setPens(QPen(QColor(150,200,150),50),QPen(QColor(255,100,100),50));
		proxVector<<new gProximitySwitchClass(packetCraneIp,"ConveyorState",42,24,QString("I305 Magnetic Switch"),0,layoutScene, 250,tr("Width Check"));proxVector.last()->moveBy(4000.f, 500.f);proxVector.last()->setPens(QPen(QColor(150,200,150),50),QPen(QColor(255,100,100),50));

		getPosWithId(3040)->myRotate(0,0,90);
		getPosWithId(3041)->myRotate(0,0,90);
		getPosWithId(3042)->myRotate(0,0,90);
		getPosWithId(3043)->myRotate(0,0,90);
		getPosWithId(3044)->myRotate(0,0,90);
		getPosWithId(3045)->myRotate(0,0,90);

	}	mainSplitter->setSizes(QList<int>()<<600<<1300);
}
allConvPositionsClass::~allConvPositionsClass()
{
}
void allConvPositionsClass::resetGraphicsViewZoom()
{
	layoutScene->setSceneRect(layoutScene->itemsBoundingRect());
	layoutGraphicsView->fitInView(layoutGraphicsView->sceneRect(),Qt::KeepAspectRatio);
}
convPositionClass* allConvPositionsClass::getPosWithId(int id)
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
QString allConvPositionsClass::getPosDescriptionWithBasket(int b,int ignorePlcId)
{
	QString ans;
	if (ignorePlcId>0)
	{
		convPositionClass *pos=getPosWithId(ignorePlcId);
		if (pos&&(pos->getBasket()==b))
			return ans;
	}
	for (int i=0;i<positionsVector.count();i++)
	{
		if (positionsVector[i]->getBasket()==b)
		{
			ans=tr("Basket on %1 (%2)").arg(positionsVector[i]->getDescription()).arg(positionsVector[i]->getGlobalId());
			return ans;
		}
	}
	int dbPos=getFirst(QString("select posId from positions where cassette=%1").arg(b),MAIN_DB_CONNECTION).toInt();
	if (dbPos!=0)
	{
		ans=tr("Basket on %1").arg(dbPos);
		return ans;
	}
	return ans;
}
void allConvPositionsClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==packetCraneIp)
	{
		for (int i=0;i<positionsVector.count();i++)
			positionsVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<movesVector.count();i++)
			movesVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<btnsVector.count();i++)
			btnsVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<proxVector.count();i++)
			proxVector[i]->dataInPlcChanged(ip,table,index,value);
		if (conveyorsGroupType==eastGroup)
		{
			if ((table=="ConveyorState")&&(index==30))
			{
				bool inMaintenanseMode=getValueOfBit(value.toInt(),1);
				for (int i=0;i<positionsVector.count();i++)
					positionsVector[i]->setMaintenanceMode(inMaintenanseMode);
				for (int i=0;i<btnsVector.count();i++)
					btnsVector[i]->setMaintenanceMode(inMaintenanseMode);
			}
		}
		if (conveyorsGroupType==packingGroup)
		{
			if ((table=="ConveyorState")&&(index==40))
			{
				bool inMaintenanseMode=getValueOfBit(value.toInt(),1);
				inMaintenanseMode=true;
				for (int i=0;i<positionsVector.count();i++)
					positionsVector[i]->setMaintenanceMode(inMaintenanseMode);
				for (int i=0;i<btnsVector.count();i++)
					btnsVector[i]->setMaintenanceMode(inMaintenanseMode);
			}
		}
		for (int i=0;i<bitVector.count();i++)
			bitVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		commandsVector->updateValue(table,index,value.toInt());
		bitErrorsVector->updateValue(table,index,value.toInt());
		bitWarningsVector->updateValue(table,index,value.toInt());
	}
}
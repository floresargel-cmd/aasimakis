#include "pch.h"
#include "conAllPos.h"
allConvPositionsClass::allConvPositionsClass(tuxipClass *tuxip_,QWidget *p):QWidget(p)
{
	tuxip=tuxip_;
	QVBoxLayout *mainLayout=new QVBoxLayout();
	mainLayout->setMargin(0);
	setLayout(mainLayout);

	QScrollArea *scrollArea=new QScrollArea();
	scrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	mainLayout->addWidget(scrollArea);
	scrollArea->setWidgetResizable(true);
	QWidget *scrollAreaWidget=new QWidget();
	QVBoxLayout *scrollAreaWidgetLayout=new QVBoxLayout();
	scrollAreaWidgetLayout->setMargin(0);
	scrollAreaWidget->setLayout(scrollAreaWidgetLayout);
	layoutGraphicsView=new myQGraphicsViewClass(true,this);
	scrollAreaWidgetLayout->addWidget(layoutGraphicsView);
	layoutGraphicsView->scale(0.03,0.03);
	layoutScene=new QGraphicsScene(this);
	scrollArea->setWidget(scrollAreaWidget);
	layoutGraphicsView->setScene(layoutScene);//setScene
	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr(""),this);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));
	gItemClass *layoutGraphicsItem=new gItemClass(":dxfs/conLayout.dxf",QPen(QColor(200,200,200),0),false,false,Qt::color0,this);
	layoutGraphicsItem->setZValue(-10.);
	layoutScene->addItem(layoutGraphicsItem);
	gItemClass *posTopMouseOver=new gItemClass(":dxfs/conPosTopMouseOver.dxf",QPen(QColor(200,200,250)),true,true,QColor(150,225,150,150),this);
	gItemClass *conveyor=new gItemClass(":dxfs/conPosTop.dxf",QPen(QColor(225,200,200),0.0),false,false,Qt::color0,this);
	gItemClass *basketTop=new gItemClass(":dxfs/conBasketTop.dxf",QPen(QColor(100,200,100)),false,false,Qt::color0,this);
	gItemClass *profileTop=new gItemClass(":dxfs/conProfileTop.dxf",Qt::NoPen,false,false,Qt::color0,this);
	gItemClass *proxOnItem=new gItemClass(":dxfs/proximitySwitchOn.dxf",QPen(QColor(75,150,75),25),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *proxOffItem=new gItemClass(":dxfs/proximitySwitchOff.dxf",QPen(QColor(150,150,150),10),this,Qt::NoBrush,Qt::NoBrush);
	QVector<QVector<QVariant>> dataVV;

  positionsVector<<new convPositionClass(tuxip,layoutScene,crane2FromToPlcIdNorth,"From Crane 2 North",crane2Ip,"bConveyorBasket","bConveyorState","bConveyorData",2,posTopMouseOver,conveyor,basketTop,profileTop,0.,8170.,this);
	getPosWithId(crane2FromToPlcIdNorth)->addProximitySwitch(4,+2500.);
	getPosWithId(crane2FromToPlcIdNorth)->addProximitySwitch(5,-2000.);
	getPosWithId(crane2FromToPlcIdNorth)->addButton(crane2Ip,"PcCom",9,202,"bConveyorState",2,16,+1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(crane2FromToPlcIdNorth)->addButton(crane2Ip,"PcCom",9,203,"bConveyorState",2,17,-1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(crane2FromToPlcIdNorth)->myRotate(0.,0.,90.);

	positionsVector<<new convPositionClass(tuxip,layoutScene,crane2FromToPlcId,"From Crane 2 South",crane2Ip,"bConveyorBasket","bConveyorState","bConveyorData",1,posTopMouseOver,conveyor,basketTop,profileTop,0.f,0.f,this);
	getPosWithId(crane2FromToPlcId)->addProximitySwitch(4,+2500.);
	getPosWithId(crane2FromToPlcId)->addProximitySwitch(5,-2000.);
	getPosWithId(crane2FromToPlcId)->addButton(crane2Ip,"PcCom",9,102,"bConveyorState",1,16,+1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(crane2FromToPlcId)->addButton(crane2Ip,"PcCom",9,103,"bConveyorState",1,17,-1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(crane2FromToPlcId)->myRotate(0.,0.,90.);

	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,136,"",-1,-1,250,layoutScene,tr("Conveyors 1+2 north"),buttonClass::typeNormal);btnsVector.last()->myRotate(1725.f,4085.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,137,"",-1,-1,-250,layoutScene,tr("Conveyor 1+2 south"),buttonClass::typeNormal);btnsVector.last()->myRotate(1725.f,4085.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimerSlot()));
	mainTimer->start(1000);
}
void allConvPositionsClass::resetGraphicsViewZoom()
{
	layoutGraphicsView->resetView();
	layoutGraphicsView->fitInView(10000.f,-3000.f,13000.f,32000.f,Qt::KeepAspectRatio);
}
convPositionClass* allConvPositionsClass::getPosWithId(int globalId)
{
	for (int i=0;i<positionsVector.count();i++)
	{
	  if (positionsVector[i]->getGlobalId()==globalId)
		  return positionsVector[i];
	}
	return nullptr;
}
convPositionClass* allConvPositionsClass::getPosWithPlcIndex(int plcId)
{
	for (int i=0;i<positionsVector.count();i++)
	{
	  if (positionsVector[i]->getPlcIndex()==plcId)
		  return positionsVector[i];
	}
	return nullptr;
}
void allConvPositionsClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane2Ip)
	{
		for (int i=0;i<positionsVector.count();i++)
			positionsVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<movesVector.count();i++)
			movesVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<btnsVector.count();i++)
			btnsVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<proxVector.count();i++)
			proxVector[i]->dataInPlcChanged(ip,table,index,value);
		if ( (table=="bConveyorState")&&(index==14) )
		{
			bool m=getValueOfBit(value.toInt(),1);
			m=true;
			for (int i=0;i<positionsVector.count();i++)
				positionsVector[i]->setMaintenanceMode(m);
			for (int i=0;i<btnsVector.count();i++)
				btnsVector[i]->setMaintenanceMode(m);
		}
	}
}
void allConvPositionsClass::onMainTimerSlot()
{
}

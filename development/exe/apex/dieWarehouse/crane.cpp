#include "pch.h"
#include "crane.h"
extern QSplashScreen *splash;
craneClass::craneClass(tuxipServerClass *tuxipServer,tuxipClass *tuxipConnection,allPossClass *allPos_,QGraphicsScene *layoutScene_,bool useTranslation,QWidget *p):QWidget(p)
{
	splash->showMessage(tr("Initializing crane..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxip=tuxipConnection;
	allPos=allPos_;
	setVisible(false);
	layoutScene=layoutScene_;

	dxfGroupOfGraphicsItemClass *cellGraphicsItem=new dxfGroupOfGraphicsItemClass(":dxfs/cell.dxf",QPen(QColor(200,200,250)),false,false,QColor(),this);

	dxfGroupOfGraphicsItemClass *bigMouseOver=new dxfGroupOfGraphicsItemClass(":dxfs/bigMouseOver.dxf",QPen(QColor(255,100,100)),true,true,QColor(255,100,100,150),this);
	dxfGroupOfGraphicsItemClass *bigEmptyBasket=new dxfGroupOfGraphicsItemClass(":dxfs/bigEmptyBasket.dxf",QPen(QColor(100,200,100)),false,false,QColor(),this);
	dxfGroupOfGraphicsItemClass *bigFullBasket=new dxfGroupOfGraphicsItemClass(":dxfs/bigFullBasket.dxf",QPen(QColor(100,100,200)),false,false,QColor(),this);
	dxfGroupOfGraphicsItemClass *bigLocked=new dxfGroupOfGraphicsItemClass(":dxfs/bigLocked.dxf",QPen(QColor(255,100,100)),false,false,QColor(),this);
	bigMouseOver->setZValue(-100);

	int basketNum=getOne(QString("select basket from pos where plcId=%1").arg(cranePlcId),db).toInt();
	conveyor=new craneConveyorClass(tuxip,allPos,cranePlcId,basketNum,"crane" ,layoutScene,bigMouseOver,cellGraphicsItem,bigFullBasket,bigEmptyBasket,bigLocked,p);

	conveyor->setYOffset(0.f,gYPlussOffset,gYMinusOffset);
	conveyor->setPlcMissionTable(dieIp,"bRobotToPcI",0,1,2,10,11,12,13,18,19,30,31,32,33,34,35,8);
	conveyor->loadFromDatabase();

	QColor craneColor=QColor(150,150,175);
	topViewItem=new dxfGroupOfGraphicsItemClass(":dxfs/craneTopView.dxf",QPen(QColor(200,225,200)),false,false,QColor(),this);  layoutScene->addItem(topViewItem);topViewItem->setZValue(-1);
	sideViewUpItem=new dxfGroupOfGraphicsItemClass(":dxfs/sideView.dxf",QPen(craneColor),false,false,QColor(),this);  layoutScene->addItem(sideViewUpItem);sideViewUpItem->setZValue(-1);
//	sideViewDownItem=new dxfGroupOfGraphicsItemClass(":dxfs/sideView.dxf",QPen(craneColor),false,false,QColor(),this);  layoutScene->addItem(sideViewDownItem);sideViewDownItem->setZValue(-1);
	carriageUpItem=new dxfGroupOfGraphicsItemClass(":dxfs/carriage.dxf",QPen(craneColor),false,false,QColor(),this);   layoutScene->addItem(carriageUpItem);carriageUpItem->setZValue(-1);
//	carriageDownItem=new dxfGroupOfGraphicsItemClass(":dxfs/carriage.dxf",QPen(craneColor),false,false,QColor(),this); layoutScene->addItem(carriageDownItem);carriageDownItem->setZValue(-1);
	hooksItem=new dxfGroupOfGraphicsItemClass(":dxfs/hooks.dxf",QPen(QColor(150,200,150)),false,false,QColor(),this);layoutScene->addItem(hooksItem);hooksItem->setZValue(-1);
	craneWidget=new craneWidgetClass(tuxipServer,tuxip,conveyor->getCraneConveyorWidget(),useTranslation,this);
	craneWidget->setCraneConveyorClass(conveyor);
	//
	workStation=NULL;
	//
	missionsTimer=new QTimer(this);
	connect(missionsTimer,SIGNAL(timeout()),this,SLOT(missionsTimerSlot()));
	craneWidget->addMessageToGui(QString("..."),QString("Program started"),infoStr);
	autoMode=0;
}
craneClass::~craneClass()
{
}
void craneClass::addToAutoMode(int mode)
{
	autoMode=autoMode|mode;
	missionsTimerSlot();
}
void craneClass::removeFromAutoMode(int mode)
{
	autoMode=autoMode^mode;
}
QWidget* craneClass::getCraneWidget()
{
	return craneWidget;
}
craneConveyorClass *craneClass::getCraneConveyor()
{
	return conveyor;
}
void craneClass::setStations(workStationClass *workStation_)
{
	workStation=workStation_;
	missionsTimer->start(2500);
}
void craneClass::loadFromDatabase()
{
	conveyor->loadFromDatabase();
}
void craneClass::sendManualMission(posClass *from,posClass *to)
{
	if (conveyor->getIsIdle()&&conveyor->isReadyForNewMission())
		conveyor->missionStart(from,to);
}
void craneClass::checkForNextMissionInDatabase()
{
	conveyor->checkForNextMissionInDatabase();
}
void craneClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((ip==dieIp)&&(table=="bRobotToPcF"))
	{
		if (index==0)
		{
			plcX=value.toFloat();
		}
		if (index==1)
			plcZ=value.toFloat();
		if (index==4)
			plcYA=value.toFloat();
		if (index==5)
			plcYB=value.toFloat();
		if ((index==0)||(index==1)||(index==2)||(index==3)||(index==4)||(index==5))
		{
			conveyor->setPlcPos(plcX,plcYA,plcZ);

			topViewItem->setPos(plcX,0);
			sideViewUpItem->setPos(plcX,gYPlussOffset);
//			sideViewDownItem->setPos(plcX,gYMinusOffset);
			carriageUpItem->setPos(plcX,plcZ+gYPlussOffset);
//			carriageDownItem->setPos(plcX,plcZ+gYMinusOffset);
			hooksItem->setPos(plcX,1656.5-plcYA);
		}
	}
	if ((ip==dieIp)&&(table=="bRobotToPcI"))
	{
		conveyor->dataInPlcChanged(ip,table,index,value);
		if ((index==27))
		{
			hooksItem->setPen(HAVECOMMONBIT(value.toInt(),BITS[25])?QPen(QColor(150,255,150)):QPen(QColor(255,100,100)));
		}
	}
	craneWidget->dataInPlcChanged(ip,table,index,value);
}
void craneClass::missionsTimerSlot()
{
	if (autoMode&autoExports)
	{
		int basketToE=workStation->getNextBasketToComeOut();
		int basketFromE=workStation->getBasketToReturn();
		if ((basketToE!=0)&&(isBasketActive(basketToE)))
			basketToE=0;
		posClass *cellToEPos=NULL;
		if ((basketToE)&&(workStation->getCanGetBasket()))
			cellToEPos=allPos->getCellWithBasket(basketToE,true);
		if ((basketToE)&&(conveyor->isReadyForNewMission())&&cellToEPos&&workStation->getCanGetBasket())
		{
			craneWidget->addMessageToGui(QString("%1 to %2 ").arg(cellToEPos->getDescription()).arg(workStation->getDescription()),QString("Starting mission: export %1 to %2").arg(cellToEPos->getDescription()).arg(workStation->getDescription()),infoStr);
			conveyor->missionStart(cellToEPos,workStation);
		}
		//return
		if ((basketFromE!=0)&&(isBasketActive(basketFromE)))
			basketFromE=0;
		posClass *cellFromEPos=NULL;
		if (basketFromE)
			cellFromEPos=allPos->getPosWithId(basketFromE);
		if ((cellFromEPos)&&(conveyor->isReadyForNewMission())&&cellFromEPos&&(!cellFromEPos->getIsLocked()))
		{
			craneWidget->addMessageToGui(QString("%1 to %2 ").arg(workStation->getDescription()).arg(cellFromEPos->getDescription()),QString("Starting mission: export %1 to %2").arg(workStation->getDescription()).arg(cellFromEPos->getDescription()),infoStr);
			conveyor->missionStart(workStation,cellFromEPos);
		}
	}
}

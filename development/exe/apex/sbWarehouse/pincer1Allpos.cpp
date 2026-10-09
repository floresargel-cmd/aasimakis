#include "pch.h"
#include "pincer1AllPos.h"
gItemClass			*externEmptyBasketGraphicsItem								;
gItemClass			*externFullBasketGraphicsItem									;
gItemClass			*externBasketPlcFromWithBasketGraphicsItem		;
gItemClass			*externBasketPlcFromWithOutBasketGraphicsItem	;
gItemClass			*externBasketPlcToWithBasketGraphicsItem			;
gItemClass			*externBasketPlcToWithOutBasketGraphicsItem		;
gItemClass			*externBasketUndefinedGraphicsItem						;
gItemClass			*externBasketUserSelectedFromPosGraphicsItem	;
gItemClass			*externNoBasketGraphicsItem										;
gItemClass			*externPosGraphicsItem												;
allPincer1PosClass::allPincer1PosClass(allPossClass *allPos_,tuxipClass *tuxip_,pincer1Class *pincer1_,QGraphicsScene *layoutScene,QWidget *p):QWidget(p)
{
	tuxip=tuxip_;
	pincer1=pincer1_;
	allPos=allPos_;
	missionLogFromPos=missionLogToPos=0;
	userMaintenanseMode=false;

	gItemClass *profilesItem=new gItemClass(":dxfs/posProfiles.dxf",QPen(QColor(100,100,200)),this,Qt::NoBrush,QBrush(QColor(225,255,255,150)));
	gItemClass *activeFromItem=new gItemClass(":dxfs/posFrom.dxf",QPen(QColor(200,100,100)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *activeToItem=new gItemClass(":dxfs/posTo.dxf",QPen(QColor(200,100,100)),this,Qt::NoBrush,Qt::NoBrush);


	externEmptyBasketGraphicsItem									=new gItemClass(":/dxfs/posSbBlue.dxf",							QPen(QColor(100,100,100)),this);

	externFullBasketGraphicsItem									=new gItemClass(externEmptyBasketGraphicsItem,0.,0.);externFullBasketGraphicsItem->addPath(profilesItem->getPath());
	externBasketPlcFromWithBasketGraphicsItem			=new gItemClass(externEmptyBasketGraphicsItem,0.,0.);externBasketPlcFromWithBasketGraphicsItem->addPath(activeFromItem->getPath());
	externBasketPlcFromWithOutBasketGraphicsItem	=new gItemClass(activeFromItem,0.,0.);

	externBasketPlcToWithBasketGraphicsItem				=new gItemClass(externEmptyBasketGraphicsItem,0.,0.);externBasketPlcToWithBasketGraphicsItem->addPath(activeToItem->getPath());
	externBasketPlcToWithOutBasketGraphicsItem		=new gItemClass(activeToItem,0.,0.);

	externBasketUndefinedGraphicsItem							=new gItemClass(":/dxfs/posMouseOver.dxf",						QPen(QColor(200,200,200)),this);
	externBasketUserSelectedFromPosGraphicsItem		=new gItemClass(externBasketPlcFromWithBasketGraphicsItem,0.,0.);
	externNoBasketGraphicsItem										=new gItemClass(":/dxfs/posSbConveyor.dxf",						QPen(QColor(100,100,150)),this/*,QBrush(QColor(225,225,225,150)),QBrush(QColor(225,225,225,150))*/);
	externPosGraphicsItem													=new gItemClass(":/dxfs/posMouseOver.dxf",						QPen(QColor(200,200,200)),this,QBrush(QColor(225,225,225,150)));
	userSelectedFromPosition=NULL;
	plcActiveFromPos=plcActiveToPos=-1;
	float sbYStep=482.f;

	allV<<(pincer2Pos=new pincerPositionClass(allPos->getPosWithId(pincer1Id),layoutScene,0.,0.,this));

	colsVector<<new pincerColumnsClass(tuxipConnection,tr("Conveyor"),layoutScene,QPointF(-290.,-1920.),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(conveyor__2_id),layoutScene,290.,-1920.,this));

	colsVector<<new pincerColumnsClass(tuxipConnection,tr("Buffer"),layoutScene,QPointF(-1520.,-400.),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_41_id),layoutScene,+1520.,-400.,this));
	//colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_42_id),layoutScene,+1140.,-300.+sbYStep,this));
	//colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_43_id),layoutScene,+1140.,-300.+2*sbYStep,this));
	//colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_44_id),layoutScene,+1140.,-300.+3*sbYStep,this));

	colsVector<<new pincerColumnsClass(tuxipConnection,tr("Buffer"),layoutScene,QPointF(-3220.,-400.),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_45_id),layoutScene,+3220.,-400.,this));
	//colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_46_id),layoutScene,+2005.,-300.+sbYStep,this));
	//colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_47_id),layoutScene,+2005.,-300.+2*sbYStep,this));
	//colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_48_id),layoutScene,+2005.,-300.+3*sbYStep,this));

	colsVector<<new pincerColumnsClass(tuxipConnection,tr("Stacker Buffer"),layoutScene,QPointF(-2370.,-400.),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_49_id),layoutScene,+2370.,-400.,this));

	colsVector<<new pincerColumnsClass(tuxipConnection,tr("Stacket"),layoutScene,QPointF(-2370,-7035),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_50_id),layoutScene,+2370,-7035,this));

	colsVector<<new pincerColumnsClass(tuxipConnection,tr("Scrap"),layoutScene,QPointF(-1520.,-7035),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_51_id),layoutScene,+1520.,-7035,this));

	colsVector<<new pincerColumnsClass(tuxipConnection,tr("Conveyor"),layoutScene,QPointF(-290.,-9950.),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(conveyor__4_id),layoutScene,290.,-9950.,this));

	colsVector<<new pincerColumnsClass(tuxipConnection,tr("To warehouse"),layoutScene,QPointF(-2350.,-17960.),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer1_52_id),layoutScene,+2350.,-17960.,this));

	for (int i=0;i<colsVector.count();i++)
		allV<<colsVector[i]->getAllPos();
	for (int i=0;i<allV.count();i++)
		connect(allV[i],SIGNAL(clickedSignal(int)),this,SLOT(positionClickedSlot(int)));
	userSelectedPosTimer=new QTimer(this);
	connect(userSelectedPosTimer,SIGNAL(timeout()),this,SLOT(userSelectedPosTimerSlot()));
	userSelectedPosTimer->setInterval(10000);

	QTimer *updateFromParentPosTimer=new QTimer(this);
	connect(updateFromParentPosTimer,SIGNAL(timeout()),this,SLOT(updateFromParentPosTimerTimeoutSlot()));
	updateFromParentPosTimer->start(2000);
}
allPincer1PosClass::~allPincer1PosClass()
{
	//delete externEmptyBasketGraphicsItem;
	//delete externFullBasketGraphicsItem;
	//delete externBasketPlcFromWithBasketGraphicsItem;
	//delete externBasketPlcFromWithOutBasketGraphicsItem;
	//delete externBasketPlcToWithBasketGraphicsItem;
	//delete externBasketPlcToWithOutBasketGraphicsItem;
	//delete externBasketUndefinedGraphicsItem;
	//delete externBasketUserSelectedFromPosGraphicsItem;
	//delete externNoBasketGraphicsItem;
	//delete externPosGraphicsItem;
}
//disablePositions/ hide + disable
void allPincer1PosClass::setPlcOnMission(bool m)
{
	for(int i=0;i<colsVector.count();i++)
	{
		if (m)
		{
			colsVector[i]->addMode(pincerPositionClass::PLC_ON_MISSION);
			colsVector[i]->removeMode(pincerPositionClass::ENABLED_FOR_USER_TO_SELECTE_FROM);
		}
		else
		{
			colsVector[i]->removeMode(pincerPositionClass::PLC_ON_MISSION);
			colsVector[i]->addMode(pincerPositionClass::ENABLED_FOR_USER_TO_SELECTE_FROM);
		}
	}
}
pincerPositionClass* allPincer1PosClass::getPincer2Pos()
{
	return pincer2Pos;
}
void allPincer1PosClass::setUserMaintenanseMode(bool m)
{
	userMaintenanseMode=m;
	for(int i=0;i<colsVector.count();i++)
		colsVector[i]->setUserMaintenanseMode(userMaintenanseMode);
	pincer2Pos->setUserMaintenanseMode(userMaintenanseMode);
}
bool allPincer1PosClass::getUserMaintenanseMode()
{
	return userMaintenanseMode;
}
void allPincer1PosClass::addMode(int m)
{
	for(int i=0;i<colsVector.count();i++)
		colsVector[i]->addMode(m);
}
void allPincer1PosClass::removeMode(int m)
{
	for(int i=0;i<colsVector.count();i++)
		colsVector[i]->removeMode(m);
}
void allPincer1PosClass::setUserSelectedFromPos(int p)
{
	if (userSelectedFromPosition)
		userSelectedFromPosition->removeMode(pincerPositionClass::USER_SELECTED);
	userSelectedFromPosition=getPosWithId(p);
	if (userSelectedFromPosition)
		userSelectedFromPosition->addMode(pincerPositionClass::USER_SELECTED);
}
void allPincer1PosClass::positionClickedSlot(int posId)
{
	//userSelectedFromPosition=NULL;
	if (!pincer1->getIsIdle())
		return;
	if (userSelectedFromPosition==NULL)//from selected
	{
		setUserSelectedFromPos(posId);
		removeMode(pincerPositionClass::ENABLED_FOR_USER_TO_SELECTE_FROM);
		addMode(pincerPositionClass::SHOWN_FOR_USER_TO_SELECTE_TO);
		userSelectedPosTimer->start();
	}
	else//to selected
	{
		if (userSelectedFromPosition->getPosId()==posId) //if the same was selected -> clear
		{
			setUserSelectedFromPos(0);
			removeMode(pincerPositionClass::SHOWN_FOR_USER_TO_SELECTE_TO);
			addMode(pincerPositionClass::ENABLED_FOR_USER_TO_SELECTE_FROM);
		}
		else//send new mission
		{
			pincerPositionClass *userSelectedToPosition=getPosWithId(posId);
			if (userSelectedToPosition)
			{
				pincer1->newMission(userSelectedFromPosition,userSelectedToPosition);
				setUserSelectedFromPos(0);
				removeMode(pincerPositionClass::SHOWN_FOR_USER_TO_SELECTE_TO);
				addMode(pincerPositionClass::ENABLED_FOR_USER_TO_SELECTE_FROM);
			}
		}
	}
	//emit positionClickedSignal(posId);
}
pincerPositionClass *allPincer1PosClass::getPosWithId(int p)
{
	for(int i=0;i<allV.count();i++)
	{
		if (allV[i]->getPosId()==p)
			return allV[i];
	}
	return NULL;
}
void allPincer1PosClass::updateFromParentPosTimerTimeoutSlot()
{
	for(int i=0;i<allV.count();i++)
		allV[i]->updateVisual();
}
void allPincer1PosClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (table=="CR2_PCcomDint")
	{
		if (index==16)//active plc from position
		{
			if (plcActiveFromPos!=value)
			{
				pincerPositionClass *oldPos=getPosWithId(plcActiveFromPos);
				if (oldPos)oldPos->removeMode(pincerPositionClass::PLC_FROM);
				pincerPositionClass *newPos=getPosWithId(value.toInt());
				if (newPos)newPos->addMode(pincerPositionClass::PLC_FROM);
				plcActiveFromPos=value.toInt();
			}
		}
		if (index==17)//active plc to position
		{
			if (plcActiveToPos!=value.toInt())
			{
				pincerPositionClass *oldPos=getPosWithId(plcActiveToPos);
				if (oldPos)oldPos->removeMode(pincerPositionClass::PLC_TO);
				pincerPositionClass *newPos=getPosWithId(value.toInt());
				if (newPos)newPos->addMode(pincerPositionClass::PLC_TO);
				plcActiveToPos=value.toInt();
			}
		}
	}
}
void allPincer1PosClass::userSelectedPosTimerSlot()
{
	if (userSelectedFromPosition)
		positionClickedSlot(userSelectedFromPosition->getPosId());
	userSelectedPosTimer->stop();
}

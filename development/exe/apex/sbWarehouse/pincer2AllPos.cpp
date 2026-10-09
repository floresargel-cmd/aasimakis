#include "pch.h"
#include "pincer2AllPos.h"
#ifdef eastStation
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
#endif
allPincer2PosClass::allPincer2PosClass(allPossClass *allPos_,tuxipClass *tuxip_,pincer2Class *pincer2_,QGraphicsScene *layoutScene,QWidget *p):QWidget(p)
{
	tuxip=tuxip_;
	pincer2=pincer2_;
	allPos=allPos_;
	missionLogFromPos=missionLogToPos=0;
	userMaintenanseMode=false;

	gItemClass *profilesItem=new gItemClass(":dxfs/posProfiles.dxf",QPen(QColor(100,100,200)),this,Qt::NoBrush,QBrush(QColor(225,255,255,150)));
	gItemClass *activeFromItem=new gItemClass(":dxfs/posFrom.dxf",QPen(QColor(200,100,100)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *activeToItem=new gItemClass(":dxfs/posTo.dxf",QPen(QColor(200,100,100)),this,Qt::NoBrush,Qt::NoBrush);


#ifdef eastStation
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
#endif
	userSelectedFromPosition=NULL;
	plcActiveFromPos=plcActiveToPos=-1;
	float sbYStep=482.f;

	allV<<(pincer2Pos=new pincerPositionClass(allPos->getPosWithId(pincer2Id),layoutScene,0.,2200.,this));

	colsVector<<new pincerColumnsClass(tuxipConnection,tr("Imports"),layoutScene,QPointF(0.,300.),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer2_22_id),layoutScene,360.,300.,this));
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer2_27_id),layoutScene,360.,300.+sbYStep,this));
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer2_28_id),layoutScene,360.,300.+2*sbYStep,this));
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer2_29_id),layoutScene,360.,300.+3*sbYStep,this));

	colsVector<<new pincerColumnsClass(tuxipConnection,tr("To warehouse"),layoutScene,QPointF(-1560.,697.),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(cassetteDoubleExit_17_id),layoutScene,1560.,697.,this));
	colsVector<<new pincerColumnsClass(tuxipConnection,tr("From warehouse"),layoutScene,QPointF(-2380.,500.),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(cassetteDoubleExit_18_id),layoutScene,2380.,697.,this));
	colsVector<<new pincerColumnsClass(tuxipConnection,tr("Exports"),layoutScene,QPointF(-3717.,311.),this);
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer2_20_id),layoutScene,3717.,311.,this));
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer2_31_id),layoutScene,3717.,311.+sbYStep,this));
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer2_32_id),layoutScene,3717.,311.+2*sbYStep,this));
	colsVector.last()->addPositionClass(new pincerPositionClass(allPos->getPosWithId(pincer2_33_id),layoutScene,3717.,311.+3*sbYStep,this));

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
allPincer2PosClass::~allPincer2PosClass()
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
void allPincer2PosClass::setPlcOnMission(bool m)
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
pincerPositionClass* allPincer2PosClass::getPincer2Pos()
{
	return pincer2Pos;
}
void allPincer2PosClass::setUserMaintenanseMode(bool m)
{
	userMaintenanseMode=m;
	for(int i=0;i<colsVector.count();i++)
		colsVector[i]->setUserMaintenanseMode(userMaintenanseMode);
}
bool allPincer2PosClass::getUserMaintenanseMode()
{
	return userMaintenanseMode;
}
void allPincer2PosClass::addMode(int m)
{
	for(int i=0;i<colsVector.count();i++)
		colsVector[i]->addMode(m);
}
void allPincer2PosClass::removeMode(int m)
{
	for(int i=0;i<colsVector.count();i++)
		colsVector[i]->removeMode(m);
}
void allPincer2PosClass::setUserSelectedFromPos(int p)
{
	if (userSelectedFromPosition)
		userSelectedFromPosition->removeMode(pincerPositionClass::USER_SELECTED);
	userSelectedFromPosition=getPosWithId(p);
	if (userSelectedFromPosition)
		userSelectedFromPosition->addMode(pincerPositionClass::USER_SELECTED);
}
void allPincer2PosClass::positionClickedSlot(int posId)
{
	//userSelectedFromPosition=NULL;
	if (!pincer2->getIsIdle())
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
				pincer2->newMission(userSelectedFromPosition,userSelectedToPosition);
				setUserSelectedFromPos(0);
				removeMode(pincerPositionClass::SHOWN_FOR_USER_TO_SELECTE_TO);
				addMode(pincerPositionClass::ENABLED_FOR_USER_TO_SELECTE_FROM);
			}
		}
	}
	//emit positionClickedSignal(posId);
}
pincerPositionClass *allPincer2PosClass::getPosWithId(int p)
{
	for(int i=0;i<allV.count();i++)
	{
		if (allV[i]->getPosId()==p)
			return allV[i];
	}
	return NULL;
}
void allPincer2PosClass::updateFromParentPosTimerTimeoutSlot()
{
	for(int i=0;i<allV.count();i++)
		allV[i]->updateVisual();
}
void allPincer2PosClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (table=="CR2_PCcomDint")
	{
		//if (index==7)
		//{
		//	QVector<bool> bits=deprecatedCalculateBits(value);
		//	for(int i=0;i<colsVector.count();i++)
		//	{
		//		for (int j=26;j<31;j++)
		//			colsVector[i]->setPlcHasBasket(j,bits[j]);
		//		for (int j=0;j<5;j++)
		//			colsVector[i]->setBasketPosIsUnLockedFromOven(j,bits[j]);
		//	}
		//	if (userSelectedFromPosition&&(userSelectedFromPosition->getHasMode(pincerPositionClass::LOCKED_FROM_OVEN_PLC)))
		//		positionClickedSlot(userSelectedFromPosition->getPosId());
		//}
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
void allPincer2PosClass::userSelectedPosTimerSlot()
{
	if (userSelectedFromPosition)
		positionClickedSlot(userSelectedFromPosition->getPosId());
	userSelectedPosTimer->stop();
}

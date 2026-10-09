#include "pch.h"
#include "crane.h"
extern QSplashScreen *splash;
int noOfBasketsToSendToPackingDestackersG=2;
craneClass::craneClass(tuxipServerClass *tuxipServer,tuxipClass *tuxipConnection,allPossClass *allPos_,QGraphicsScene *layoutScene_,QWidget *p):QWidget(p)
{
	splash->showMessage(tr("Initializing crane..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxip=tuxipConnection;
	allPos=allPos_;
	setVisible(false);
	layoutScene=layoutScene_;

	gItemClass *posItem=new gItemClass(":dxfs/basketRect.dxf",QPen(QColor(200,200,250)),false,false,QColor(),this);
	gItemClass *mouseOverItem=new gItemClass(":dxfs/basketRect.dxf",QPen(QColor(255,100,100)),true,true,QColor(255,100,100,150),this);
	gItemClass *basketItem=new gItemClass(":dxfs/basket.dxf",QPen(QColor(100,200,100)),false,false,QColor(),this);
	gItemClass *profilesItem=new gItemClass(":dxfs/profiles.dxf",QPen(QColor(150,150,100)),false,false,QColor(),this);profilesItem->setFillBrush(QBrush(QColor(150,150,100,50)));
	gItemClass *lockedItem=new gItemClass(":dxfs/basketLocked.dxf",QPen(QColor(255,100,100)),false,false,QColor(),this);
	gItemClass *basketRectFrom=new gItemClass(":dxfs/basketRectFrom.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);
	gItemClass *basketRectTo=new gItemClass(":dxfs/basketRectTo.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);

	int basketNum=getOne(QString("select basketTableId from positions where posNumber=%1").arg(crane2PlcId),bDb).toInt();
	carriage=new carriageClass(tuxip,allPos,"Crane 2",layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,p);

	carriage->setPlcMissionTable(crane2Ip,"toPcI",0,1,2,10,11,12,13,18);
	carriage->loadFromDatabase();

	QColor craneColor=QColor(150,150,175);
	// Draw the beam and carriage above baskets without blocking position clicks.
	craneXItem=new gItemClass(":dxfs/craneX.dxf",QPen(QColor(200,225,200)),false,false,QColor(),this);  layoutScene->addItem(craneXItem);craneXItem->setZValue(20);craneXItem->setAcceptedMouseButtons(Qt::NoButton);
	craneYItem=new gItemClass(":dxfs/craneY.dxf",QPen(QColor(150,150,175)),false,false,QColor(),this);  layoutScene->addItem(craneYItem);craneYItem->setZValue(21);craneYItem->setAcceptedMouseButtons(Qt::NoButton);
	craneWidget=new craneWidgetClass(tuxipServer,tuxip,carriage->getCarriageWidget(),this);
	craneWidget->setCarriageClass(carriage);
	//
	columnsWithBasketsForExportV=allPos->getColumnsWithBasketsForExport();
	missionsTimer=new QTimer(this);
	connect(missionsTimer,SIGNAL(timeout()),this,SLOT(missionsTimerSlot()));
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
carriageClass *craneClass::getCarriage()
{
	return carriage;
}
void craneClass::setStations()
{
	toRotatingNorth=allPos->getPosWithPosIndex(toRotatingNorthPlcId,1);
	toRotatingSouth=allPos->getPosWithPosIndex(toRotatingSouthPlcId,1);
	rotatingNorth=allPos->getPosWithPosIndex(rotatingNorthPlcId,1);
	destackerBuffer=allPos->getPosWithPosIndex(stackerWaitingPlcId,1);
	destacker=allPos->getPosWithPosIndex(stackerWorkingingPlcId,1);
	exit1=allPos->getPosWithPosIndex(exit1PlcId,1);
	exit2=allPos->getPosWithPosIndex(exit2PlcId,1);
	missionsTimer->start(2500);
}
void craneClass::loadFromDatabase()
{
	carriage->loadFromDatabase();
}
bool craneClass::canGetNewMission()
{
	return carriage->getIsIdle()&&carriage->isReadyForNewMission();
}
void craneClass::sendManualMission(posClass *from,posClass *to)
{
	if (carriage->getCanStartNewMission(from,to))
		carriage->missionStart(from,to);
}
void craneClass::checkForNextMissionInDatabase()
{
	carriage->checkForNextMissionInDatabase();
}
void craneClass::setDisplayMode(int m)
{
	carriage->setDisplayMode(m);
}
void craneClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((ip==crane2Ip)&&(table=="toPcF"))
	{
		if (index==0)
			plcX=value.toFloat();
		if (index==1)
			plcZ=value.toFloat();
		if (index==6)
			plcY=value.toFloat();
		if ((index==0)||(index==1)||(index==6))
		{
			craneXItem->setPos(plcX,0.);
			craneYItem->setPos(plcX,plcY);
			carriage->setPlcPos(plcX,plcY,plcZ);
		}
	}
	if ((ip==crane2Ip)&&(table=="toPcI"))
	{
		carriage->dataInPlcChanged(ip,table,index,value);
	}
	craneWidget->dataInPlcChanged(ip,table,index,value);
}
posClass *checkPosToBeFrom(posClass *pos)
{
	if (pos)
	{
		if ((!pos->getIsFreeToMove())||isCrane2PosActiveG(pos->getPositionNumber()))
			pos=NULL;
	}
	return pos;
}
void craneClass::missionsTimerSlot()
{
	if (!carriage->getIsIdle())
		return;
	allPos->updateExportEnums();
	bool canSendToDestackerToSb=( (!toRotatingNorth->getHasBasket())&&(!rotatingNorth->getHasBasket())&&(!destackerBuffer->getHasBasket()) );
	bool canSendSouth=(!toRotatingNorth->getHasBasketCommingIn())&&(!rotatingNorth->getHasBasketCommingIn())&&(!exit1->getHasBasketCommingIn())&&(!exit2->getHasBasketCommingIn());
	if (autoMode!=0)
	{
		posClass *forDestackerPos=NULL;
		if (canSendToDestackerToSb)
		{
			if (autoMode&autoExportsToDestaker)
				forDestackerPos=checkPosToBeFrom(allPos->getPosWithBasketToMoveInAuto(getFirst(QString("select basket from %1 order by priority asc").arg(crane2ExportsDestackerForSb),bDb).toInt())); 
		}
		posClass *forAnodizingPos=NULL;
		posClass *forPackingDestacker1Pos=NULL;
		posClass *forPackingDestacker2Pos=NULL;
		if (canSendSouth)
		{
			if (autoMode&autoExportsToAnodizing)
				forAnodizingPos=checkPosToBeFrom(allPos->getPosWithBasketToMoveInAuto(getFirst(QString("select basket from %1 order by priority asc").arg(crane2ExportsAnodizing),bDb).toInt())); 
			if (autoMode&autoExportsToPackingDestaker1)
			{
				forPackingDestacker1Pos=checkPosToBeFrom(allPos->getPosWithBasketToMoveInAuto(getFirst(QString("select basket from %1 order by priority asc").arg(crane2ExportsPackingDestacker1Position),bDb).toInt()));
				int numberOfBasketsForDestacker=getFirst(QString("select (select count(tId) from %1)+(select count(pid) from positions where basketTableId<>0 and posNumber=%2) as totalCount")
					.arg(crane3ExportsDestackerPositions1).arg(destackerWorkingPos1PlcId),bDb).toInt();
				if (numberOfBasketsForDestacker>noOfBasketsToSendToPackingDestackersG)
					forPackingDestacker1Pos=nullptr;
			}
			if (autoMode&autoExportsToPackingDestaker2)
			{
				forPackingDestacker2Pos=checkPosToBeFrom(allPos->getPosWithBasketToMoveInAuto(getFirst(QString("select basket from %1 order by priority asc").arg(crane2ExportsPackingDestacker2Position),bDb).toInt())); 
				int numberOfBasketsForDestacker=getFirst(QString("select (select count(tId) from %1)+(select count(pid) from positions where basketTableId<>0 and posNumber=%2) as totalCount")
					.arg(crane3ExportsDestackerPositions2).arg(destackerWorkingPos2PlcId),bDb).toInt();
				if (numberOfBasketsForDestacker>noOfBasketsToSendToPackingDestackersG)
					forPackingDestacker2Pos=nullptr;
			}
		}
		//imports
		posClass *returningPos=allPos->getFreeReturningPosition();
		posClass *fromImportNorthPos=NULL;
		posClass *fromImportSouthPos=NULL;
		if (returningPos)
		{
			if (autoMode&autoImportsFromNorth)
			{
				if (toRotatingNorth->getHasBasket()&&(toRotatingNorth->getDestination()==destinationStruct::plcValueEnum::inFromOut)&&(!isCrane2PosActiveG(toRotatingNorth->getPositionNumber())))
					fromImportNorthPos=toRotatingNorth;
			}
			if (autoMode&autoImportsFromSouth)
			{
				if (toRotatingSouth->getHasBasket()&&(toRotatingSouth->getDestination()==destinationStruct::plcValueEnum::inFromDestacker)&&(!isCrane2PosActiveG(toRotatingSouth->getPositionNumber())))
					fromImportSouthPos=toRotatingSouth;
			}
		}
		if (carriage->isReadyForNewMission())
		{
			if (forDestackerPos)
				carriage->missionStart(forDestackerPos,toRotatingNorth);
			else if (forAnodizingPos)
				carriage->missionStart(forAnodizingPos,toRotatingNorth);
			else if (forPackingDestacker1Pos)
				carriage->missionStart(forPackingDestacker1Pos,toRotatingNorth);
			else if (forPackingDestacker2Pos)
				carriage->missionStart(forPackingDestacker2Pos,toRotatingNorth);
			else if (fromImportNorthPos&&returningPos)
				carriage->missionStart(fromImportNorthPos,returningPos);
			else if (fromImportSouthPos&&returningPos)
				carriage->missionStart(fromImportSouthPos,returningPos);
			else
			{
				int basketToDestackerForSb=getFirst(QString("select top 1 basket from %1 order by priority").arg(crane2ExportsDestackerForSb),bDb).toInt();
				int basketToAnodizing=getFirst(QString("select top 1 basket from %1 order by priority").arg(crane2ExportsAnodizing),bDb).toInt();
				int basketToPackingDestacker1Pos=getFirst(QString("select top 1 basket from %1 order by priority").arg(crane2ExportsPackingDestacker1Position),bDb).toInt();
				int basketToPackingDestacker2Pos=getFirst(QString("select top 1 basket from %1 order by priority").arg(crane2ExportsPackingDestacker2Position),bDb).toInt();
				if (canSendToDestackerToSb&&uncoverBasketForExport(basketToDestackerForSb))
					return;
				if (canSendSouth&&uncoverBasketForExport(basketToAnodizing))
					return;
				if (canSendSouth&&uncoverBasketForExport(basketToPackingDestacker1Pos))
					return;
				if (canSendSouth&&uncoverBasketForExport(basketToPackingDestacker2Pos))
					return;
			}
		}
		if (autoMode&autoExportsUseEpics)
		{
			addExportsFromEpicsData();
		}
	}
}
craneContentsDataStruct getBottomProfileContentsDataOfBasket(int basket)
{
	craneContentsDataStruct ans;
	QVector<QVector<QVariant>> bContVV=execTableQuery(QString("%1 and RackDetail.RackId=%2 order by StopTime desc").arg(selectContentsFromEpicsStrG).arg(basket),eDb);
	if (bContVV.count())
		ans=craneContentsDataStruct(bContVV.last());
	return ans;
}
void craneClass::addBasketForPackingDestacker(posClass *pos,int destackerPos,QString crane2ExportsTable,QString crane3ExportsTable)
{
	if (pos->getExportEnun()!=exportCrane2EnumClass::none)
		return;
	QVector<QVariant> basketsGoingToDestackerV=execQuery(QString(""
	"select basket,priority,1 as source from %1 "
	"union all "
	"select basket, priority, 2 as source from %2 "
	"union all "
	"select baskettableid,3 as priority,3 as source from positions where baskettableid<>0 and posnumber=%3 "
	"order by source, priority ")
		.arg(crane2ExportsTable).arg(crane3ExportsTable).arg(destackerPos),bDb);
	if (basketsGoingToDestackerV.count()<=noOfBasketsToSendToPackingDestackersG)
	{
		if (basketsGoingToDestackerV.count()==0)
			allPos->addBasketToExports(pos->getBasketNumber(),crane2ExportsTable);
		else
		{
			craneContentsDataStruct lastBasketData=getBottomProfileContentsDataOfBasket(basketsGoingToDestackerV.first().toInt());
			craneContentsDataStruct basketData=pos->getTopContentsDataStruct();
			if (basketData.hasSameSoSoLine(lastBasketData))
				allPos->addBasketToExports(pos->getBasketNumber(),crane2ExportsTable);
		}
	}
}
void craneClass::addBasketForPackingSb(posClass *pos)
{
	if (pos->getExportEnun()!=exportCrane2EnumClass::none)
		return;

	QVector<QVariant> basketsGoingToDestackerV;
	if (destacker->getHasBasket())
		basketsGoingToDestackerV<<destacker->getBasketNumber();
	if (destackerBuffer->getHasBasket())
		basketsGoingToDestackerV<<destackerBuffer->getBasketNumber();
	if (rotatingNorth->getHasBasket()&&(rotatingNorth->getDestination()==destinationStruct::destackerForSb))
		basketsGoingToDestackerV<<rotatingNorth->getBasketNumber();
	if (toRotatingNorth->getHasBasket()&&(toRotatingNorth->getDestination()==destinationStruct::destackerForSb))
		basketsGoingToDestackerV<<toRotatingNorth->getBasketNumber();
	basketsGoingToDestackerV<<execQuery(QString("select basket from %1 order by priority").arg(crane2ExportsDestackerForSb),bDb);
	if (basketsGoingToDestackerV.count()<=2)
	{
		if (basketsGoingToDestackerV.count()==0)
			allPos->addBasketToExports(pos->getBasketNumber(),crane2ExportsDestackerForSb);
		else
		{
			craneContentsDataStruct lastBasketData=getBottomProfileContentsDataOfBasket(basketsGoingToDestackerV.first().toInt());
			craneContentsDataStruct basketData=pos->getTopContentsDataStruct();
			if (basketData.hasSameSoSoLine(lastBasketData))
				allPos->addBasketToExports(pos->getBasketNumber(),crane2ExportsDestackerForSb);
		}
	}
}
void craneClass::addExportsFromEpicsData()
{
	QVector<posClass*> allAvailable;
	for (int i=0;i<columnsWithBasketsForExportV.count();i++)
	{
		posClass *topPos=columnsWithBasketsForExportV[i].getTopPositionWithBasket();
		if (topPos)
			allAvailable<<topPos;
	}
	for (int i=0;i<allAvailable.count();i++)
	{
		if (allAvailable[i]->getHoneyCombDestinationEnum()==honeyCombDestinationEnumClass::anodizing)
			allPos->addBasketToExports(allAvailable[i]->getBasketNumber(),crane2ExportsAnodizing);
		if (allAvailable[i]->getHoneyCombDestinationEnum()==honeyCombDestinationEnumClass::HCB)
			addBasketForPackingSb(allAvailable[i]);
		if (allAvailable[i]->getHoneyCombDestinationEnum()==honeyCombDestinationEnumClass::HCA)
		{
			addBasketForPackingDestacker(allAvailable[i],destackerWorkingPos1PlcId,crane2ExportsPackingDestacker1Position,crane3ExportsDestackerPositions1);
			addBasketForPackingDestacker(allAvailable[i],destackerWorkingPos2PlcId,crane2ExportsPackingDestacker2Position,crane3ExportsDestackerPositions2);
		}
	}
}
posClass *craneClass::getFreePosForBasketInPos(posClass *p)
{
	QVector<posClass*> posToCheckV;
	if ( (p->getPositionNumber()==670)||(p->getPositionNumber()==690)||(p->getPositionNumber()==1040)||(p->getPositionNumber()==1050)||(p->getPositionNumber()==1060)  )
		posToCheckV<<allPos->getPosWithPosIndex(1080,1)<<allPos->getPosWithPosIndex(1090,1)<<allPos->getPosWithPosIndex(1080,2)<<allPos->getPosWithPosIndex(1090,2)<<allPos->getPosWithPosIndex(1080,3)<<allPos->getPosWithPosIndex(1090,3);
	if (p->getPositionNumber()==1080)
		posToCheckV<<allPos->getPosWithPosIndex(1090,1)<<allPos->getPosWithPosIndex(1090,2)<<allPos->getPosWithPosIndex(1090,3);
	if (p->getPositionNumber()==1090)
		posToCheckV<<allPos->getPosWithPosIndex(1080,1)<<allPos->getPosWithPosIndex(1080,2)<<allPos->getPosWithPosIndex(1080,3);
	for (int i=0;i<posToCheckV.count();i++)
	{
		if (posToCheckV[i]->getCanGetBasketInPos(p)&&(posToCheckV[i]->getExportEnun()==exportCrane2EnumClass::none))
			return posToCheckV[i];
	}
	return NULL;
}
bool craneClass::uncoverBasketForExport(int basket)
{
	posClass *basketPos=allPos->getPosWithBasket(basket);
	if (!basketPos)
		return false;
	QVector<posClass*> preventMovePos=basketPos->getPosThatPreventYouToMove();
	if (preventMovePos.count()==0)
		return false;
	posClass *fromPos=preventMovePos.last();
	posClass *toPos=getFreePosForBasketInPos(fromPos);
	if (fromPos&&toPos)
		return carriage->missionStart(fromPos,toPos);
	return false;
}
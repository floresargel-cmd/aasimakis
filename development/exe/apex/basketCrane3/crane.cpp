#include "pch.h"
#include "crane.h"
extern QSplashScreen *splash;
craneClass::craneClass(tuxipServerClass *tuxipServer,tuxipClass *tuxipConnection,allPosClass *allPos_,QGraphicsScene *layoutScene_,QWidget *p):QWidget(p)
{
	splash->showMessage(tr("Initializing crane..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxip=tuxipConnection;
	allPos=allPos_;
	setVisible(false);
	layoutScene=layoutScene_;

	gItemClass *posItem					=new gItemClass(":dxfs/basketRect.dxf",			QPen(QColor(200,200,250)),false,false,QColor(),this);
	gItemClass *mouseOverItem		=new gItemClass(":dxfs/basketRect.dxf",			QPen(QColor(255,100,100)), true, true,QColor(255,100,100,150),this);
	gItemClass *basketItem			=new gItemClass(":dxfs/basket.dxf",					QPen(QColor(100,200,100)),false,false,QColor(),this);
	gItemClass *profilesItem		=new gItemClass(":dxfs/profiles.dxf",				QPen(QColor(150,150,100)),false,false,QColor(),this);profilesItem->setFillBrush(QBrush(QColor(150,150,100,50)));
	gItemClass *lockedItem			=new gItemClass(":dxfs/basketLocked.dxf",		QPen(QColor(255,100,100)),false,false,QColor(),this);
	gItemClass *basketRectFrom	=new gItemClass(":dxfs/basketRectFrom.dxf",	QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);
	gItemClass *basketRectTo		=new gItemClass(":dxfs/basketRectTo.dxf",		QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);

	carriage=new carriageClass(tuxip,allPos,"Crane 3",layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,p);
	carriage->setPlcMissionTable(crane3Ip,"CR3_PCcomDint",30,31,32,0,1,2,3,18);
	carriage->loadFromDatabase();

	QColor craneColor=QColor(150,150,175);
	craneXItem=new gItemClass(":dxfs/craneX.dxf",QPen(QColor(200,225,200)),false,false,QColor(),this);  
	layoutScene->addItem(craneXItem);
	craneXItem->setZValue(-1);
	craneZItem=new gItemClass(":dxfs/craneZ.dxf",QPen(craneColor),false,false,QColor(),this);  
	layoutScene->addItem(craneZItem);
	craneZItem->setZValue(-1);
	craneWidget=new craneWidgetClass(tuxipServer,tuxip,carriage->getCarriageWidget(),this);
	craneWidget->setCarriageClass(carriage);

	destackerXItem=new gItemClass(":dxfs/destackerX.dxf",QPen(QColor(100,225,200)),false,false,QColor(),this);
	layoutScene->addItem(destackerXItem);
	destackerXItem->setZValue(-1);

	destackerZItem=new gItemClass(":dxfs/destackerZ.dxf",QPen(QColor(100,225,200)),false,false,QColor(),this);
	layoutScene->addItem(destackerZItem);
	destackerZItem->setZValue(-1);
	
	missionsTimer=new QTimer(this);
	connect(missionsTimer,SIGNAL(timeout()),this,SLOT(missionsTimerTimeoutSlot()));
	autoMode=autoModesEnum::none;
}
void craneClass::addToAutoMode(autoModesEnum mode)
{
	autoMode=static_cast<autoModesEnum>(autoMode|mode);
	missionsTimerTimeoutSlot();
}
void craneClass::removeFromAutoMode(autoModesEnum mode)
{
	autoMode=static_cast<autoModesEnum>(autoMode^mode);
}
QWidget* craneClass::getCraneWidget() const
{
	return craneWidget;
}
carriageClass *craneClass::getCarriage() const
{
	return carriage;
}
void craneClass::setStations()
{
	toFromCrane2Pos=allPos->getPosWithPosIndex(crane2FromToPlcId,1);
	destackerWorking1Pos=allPos->getPosWithPosIndex(destackerWorkingPos1PlcId,1);
	destackerWorking2Pos=allPos->getPosWithPosIndex(destackerWorkingPos2PlcId,1);
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
	if ((ip==crane3Ip)&&(table=="CR3_PCcomReal"))
	{
		if (index==0)
			cranePlcX=value.toFloat();
		if (index==12)
			cranePlcZ=value.toFloat();
		if ((index==0)||(index==12))
		{
			craneXItem->setPos(cranePlcX,0.);
			craneZItem->setPos(cranePlcX,-cranePlcZ);
			carriage->setPlcPos(cranePlcX,-cranePlcZ);
		}
		if (index==20)
			destackerPlcX=value.toFloat();
		if (index==22)
			destackerPlcZ=value.toFloat();
		if ((index==20)||(index==22))
		{
			destackerXItem->setPos(-destackerPlcX,0.);
			destackerZItem->setPos(-destackerPlcX,-destackerPlcZ);
		}
	}
	if ((ip==crane3Ip)&&(table=="CR3_PCcomDint"))
	{
		carriage->dataInPlcChanged(ip,table,index,value);
	}
	craneWidget->dataInPlcChanged(ip,table,index,value);
}
void craneClass::missionsTimerTimeoutSlot()
{
	if (!canGetNewMission())
		return;
	allPos->updateExportEnums();
	if (autoMode!=autoModesEnum::none)
	{ 
		bool missionSend=false;
		QVector<posClass*> topPosWithBasketV=allPos->getTopPositionsWithBasket();
		if ((!missionSend)&&(autoMode&autoModesEnum::toDestackerWorking1Pos))
		{
			if (!destackerWorking1Pos->getHasBasket())
			{
				int selectedBasket=getFirst(QString("select top 1 basket from %1 order by priority asc").arg(crane3ExportsDestackerPositions1),bDb).toInt();
				QVector<QVariant> nextBasketsV=execQuery(QString("select basket from %1 order by priority asc").arg(crane3ExportsDestackerPositions1),bDb);
				int nextBasket=0;
				for (int i=0;i<nextBasketsV.count();i++)
				{
					for (int j=0;(nextBasket==0)&&(j<topPosWithBasketV.count());j++)
					{
						if (topPosWithBasketV[j]->getHasSameContentsWithPos(allPos->getPosWithBasket(nextBasketsV[i].toInt())))
							nextBasket=nextBasketsV[i].toInt();
					}
				}
				if (nextBasket!=0)
					selectedBasket=nextBasket;
				if (getFirst(QString("select count(pid) from positions where basketTableId=%1").arg(selectedBasket),bDb).toInt()==0)
					execQuery(QString("delete from %1 where basket=%2").arg(crane3ExportsDestackerPositions1).arg(selectedBasket),bDb);
				missionSend=carriage->missionStart(allPos->getPosWithBasketToMoveInAuto(selectedBasket),destackerWorking1Pos);
			}
		}
		if ((!missionSend)&&(autoMode&autoModesEnum::toDestackerWorking2Pos))
		{
			if (!destackerWorking2Pos->getHasBasket())
			{
				int selectedBasket=getFirst(QString("select top 1 basket from %1 order by priority asc").arg(crane3ExportsDestackerPositions2),bDb).toInt();
				QVector<QVariant> nextBasketsV=execQuery(QString("select basket from %1 order by priority asc").arg(crane3ExportsDestackerPositions2),bDb);
				int nextBasket=0;
				for (int i=0;i<nextBasketsV.count();i++)
				{
					for (int j=0;(nextBasket==0)&&(j<topPosWithBasketV.count());j++)
					{
						if (topPosWithBasketV[j]->getHasSameContentsWithPos(allPos->getPosWithBasket(nextBasketsV[i].toInt())))
							nextBasket=nextBasketsV[i].toInt();
					}
				}
				if (nextBasket!=0)
					selectedBasket=nextBasket;
				if (getFirst(QString("select count(pid) from positions where basketTableId=%1").arg(selectedBasket),bDb).toInt()==0)
					execQuery(QString("delete from %1 where basket=%2").arg(crane3ExportsDestackerPositions2).arg(selectedBasket),bDb);
				missionSend=carriage->missionStart(allPos->getPosWithBasketToMoveInAuto(selectedBasket),destackerWorking2Pos);
			}
		}
		if ((!missionSend)&&(autoMode&autoModesEnum::toDestackerWorking1Pos))
		{
				int basketToDestacker=getFirst(QString("select top 1 basket from %1 order by priority").arg(crane3ExportsDestackerPositions1),bDb).toInt();
				if ((!destackerWorking1Pos->getHasBasket())&&uncoverBasketForExport(basketToDestacker))
					return;
		}
		if ((!missionSend)&&(autoMode&autoModesEnum::toDestackerWorking2Pos))
		{
				int basketToDestacker=getFirst(QString("select top 1 basket from %1 order by priority").arg(crane3ExportsDestackerPositions2),bDb).toInt();
				if ((!destackerWorking2Pos->getHasBasket())&&uncoverBasketForExport(basketToDestacker))
					return;
		}
		if ((!missionSend)&&(autoMode&autoModesEnum::toFromCrane2))
		{
			//to crane 2
			int selectedBasket=getFirst(QString("select top 1 basket from %1 order by priority asc").arg(crane3ExportsToCrane2),bDb).toInt();;
			QVector<QVariant> nextBasketsV=execQuery(QString("select basket from %1 order by priority asc").arg(crane3ExportsToCrane2),bDb);
			int nextBasket=0;
			for (int i=0;i<nextBasketsV.count();i++)
			{
				for (int j=0;(nextBasket)&&(j<topPosWithBasketV.count());j++)
				{
					if (topPosWithBasketV[j]->getBasketNumber()==nextBasketsV[i].toInt())
						nextBasket=nextBasketsV[i].toInt();
				}
			}
			if (nextBasket!=0)
				selectedBasket=nextBasket;
			if (getFirst(QString("select count(pid) from positions where basketTableId=%1").arg(selectedBasket),bDb).toInt()==0)
				execQuery(QString("delete from %1 where basket=%2").arg(crane3ExportsToCrane2).arg(selectedBasket),bDb);
			posClass *nextPos=allPos->getPosWithBasketToMoveInAuto(selectedBasket);
			if (nextPos)
			{
				if ((!missionSend)&&(!toFromCrane2Pos->getHasBasket()))
					missionSend=carriage->missionStart(nextPos,toFromCrane2Pos);
				else if ((nextPos==destackerWorking1Pos)||(nextPos==destackerWorking2Pos))
					missionSend=carriage->missionStart(nextPos,allPos->getPosInBufferToGetBasketInPos(nextPos));
			}
			//from crane 2
			if ((!missionSend)&&toFromCrane2Pos->getHasBasket()&&(toFromCrane2Pos->getExportEnun()==exportCrane3EnumClass::packingDestacker1))
				missionSend=carriage->missionStart(toFromCrane2Pos,allPos->getPosInBufferToGetBasketInPos(toFromCrane2Pos));
			if ((!missionSend)&&toFromCrane2Pos->getHasBasket()&&(toFromCrane2Pos->getExportEnun()==exportCrane3EnumClass::packingDestacker2))
				missionSend=carriage->missionStart(toFromCrane2Pos,allPos->getPosInBufferToGetBasketInPos(toFromCrane2Pos));
		}
		if ((!missionSend)&&(autoMode&autoModesEnum::returnEmpty)&&(!toFromCrane2Pos->getHasBasket()))
		{
			posClass *emptyBasketPos=allPos->getPosWithEmptyBasketOnTop();
			if (emptyBasketPos&&(getFirst(QString("select count(tId) from %1").arg(crane3ExportsToCrane2),bDb).toInt()==0))
				execQuery(QString("insert into %1 (basket,priority) select %2,isnull((select max(priority) from %1),0)+1 where not exists (select 1 from %1 where basket=%2)").arg(crane3ExportsToCrane2).arg(emptyBasketPos->getBasketNumber()),bDb);
		}
	}
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
	posClass *toPos=allPos->getPosInBufferToGetBasketInPos(fromPos);
	if (fromPos&&toPos)
		return carriage->missionStart(fromPos,toPos);
	return false;
}
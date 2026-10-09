#include "pch.h"
#include "allPos.h"
extern QSplashScreen *splash;
////////////////////////////////////////////////////columnClass///////////////////////////////////////////////////////
columnClass::columnClass()
{
};
columnClass::columnClass(int posNumber_,QVector<posClass*> posV_)
{
	posNumber=posNumber_;
	posV=posV_;
};
posClass *columnClass::getTopPositionWithBasket()
{
	for (int i=(posV.count()-1);i>=0;i--)
	{
		if (posV[i]->getHasBasket())
			return posV[i];
	}
	return nullptr;
}
allPosClass::allPosClass(tuxipClass *crane2Tux_,tuxipClass *crane3Tux_,QGraphicsScene *layoutScene,QWidget *p):QWidget(p)
{
	crane2Tux=crane2Tux_;
	crane3Tux=crane3Tux_;
	selectMode=selectForMission;
	setVisible(false);  //// why false?
	gItemClass *posItem=new gItemClass(":dxfs/basketRect.dxf",QPen(QColor(240,240,240)),false,false,QColor(),this);
	gItemClass *mouseOverItem=new gItemClass(":dxfs/basketRect.dxf",QPen(QColor(255,100,100)),true,true,QColor(255,100,100,150),this);
	gItemClass *basketItem=new gItemClass(":dxfs/basket.dxf",QPen(QColor(100,200,100)),false,false,QColor(),this);
	gItemClass *profilesItem=new gItemClass(":dxfs/profiles.dxf",QPen(QColor(150,150,100)),false,false,QColor(),this);profilesItem->setFillBrush(QBrush(QColor(150,150,100,50)));
	gItemClass *lockedItem=new gItemClass(":dxfs/basketLocked.dxf",QPen(QColor(255,100,100)),false,false,QColor(),this);
	gItemClass *basketRectFrom=new gItemClass(":dxfs/basketRectFrom.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);
	gItemClass *basketRectTo=new gItemClass(":dxfs/basketRectTo.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);
	manualSelectedFromPos=nullptr;
	crane=nullptr;
	splash->showMessage(tr("Loading position data..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	posV<<new posClass(this,crane2FromToPlcId,1,0.f,99.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(crane2Tux,"bConveyorBasket",1);
	posV<<new posClass(this,destackerWorkingPos1PlcId,1,11355.f,0.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(crane3Tux,"CR3_PCcomDint",26);
	posV<<new posClass(this,destackerWorkingPos2PlcId,1,13610.f,0.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(crane3Tux,"CR3_PCcomDint",27);

	posV<<new posClass(this,bufferApos,1,5260.f,1080.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferApos,2,5260.f,1080.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferApos,3,5260.f,1080.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	bufferV<<columnClass(bufferApos,QVector<posClass*>()<<getPosWithPosIndex(bufferApos,1,false)<<getPosWithPosIndex(bufferApos,2,false)<<getPosWithPosIndex(bufferApos,3,false));
	setUpDownPositionsOfColumn(bufferApos);
	posV<<new posClass(this,bufferBpos,1,6900.f,1080.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferBpos,2,6900.f,1080.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferBpos,3,6900.f,1080.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	bufferV<<columnClass(bufferBpos,QVector<posClass*>()<<getPosWithPosIndex(bufferBpos,1,false)<<getPosWithPosIndex(bufferBpos,2,false)<<getPosWithPosIndex(bufferBpos,3,false));
	setUpDownPositionsOfColumn(bufferBpos);
	posV<<new posClass(this,bufferCpos,1,8885.f,102.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferCpos,2,8885.f,102.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferCpos,3,8885.f,102.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
  posV<<new posClass(this,bufferCpos,4,8885.f,102.f,0.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	bufferV<<columnClass(bufferCpos,QVector<posClass*>()<<getPosWithPosIndex(bufferCpos,1,false)<<getPosWithPosIndex(bufferCpos,2,false)<<getPosWithPosIndex(bufferCpos,3,false)<<getPosWithPosIndex(bufferCpos,4,false));
	setUpDownPositionsOfColumn(bufferCpos);
	for (int i=0;i<posV.count();i++)
	{
		connect(posV[i],SIGNAL(posMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
		connect(posV[i],SIGNAL(userChangedBasketSignal(posClass*)),this,SLOT(userChangedBasketSlot(posClass*)));
	}

	QList <QGraphicsView*> gv=layoutScene->views();
	if (gv.count()!=0)
		connect(gv[0],SIGNAL(keyPressedSignal(QKeyEvent*)),this,SLOT(graphicsViewKeyPressedSlot(QKeyEvent*)));

	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer->start(25000);

	loadFromDatabase();
	setDisplayMode(posClass::displayToSelectFrom);
	exportListChangedSlot();
}
void allPosClass::mainTimerSlot()
{
	loadFromDatabase();
}
void allPosClass::loadFromDatabase()
{
	QVector<QVector<QVariant>> allPosVV=execTableQuery(QString("select posNumber,posIndex,description,basketTableId,locked from positions"),bDb);
	QVector<QVector<QVariant>> allContentsVV=execTableQuery(QString("%1 order by StopTime desc").arg(selectContentsFromEpicsStrG),eDb);
	for (int i=0;i<posV.count();i++)
		posV[i]->loadFromAllData(allPosVV,allContentsVV);
}
void allPosClass::setDisplayMode(int m)
{
	if (crane)
		crane->setDisplayMode(m);
	if ((m==posClass::displayToModify)&&(!crane->canGetNewMission()))
	{
		for (int i=0;i<posV.count();i++)
			posV[i]->setDisplayMode(posClass::displayNormal);
		return;
	}
	for (int i=0;i<posV.count();i++)
		posV[i]->setDisplayMode(m);
}
void allPosClass::clearIsFromPos()
{
	for (int i=0;i<posV.count();i++)
		posV[i]->setIsFrom(false);
}
void allPosClass::clearIsToPos()
{
	for (int i=0;i<posV.count();i++)
		posV[i]->setIsTo(false);
}

void allPosClass::setUpDownPositionsOfColumn(int column)
{
	QVector<posClass*> pV;
	posClass* p1=getPosWithPosIndex(column,1,false);
	posClass* p2=getPosWithPosIndex(column,2,false);
	posClass* p3=getPosWithPosIndex(column,3,false);
	posClass* p4=getPosWithPosIndex(column,4,false);
	p1->setAboveBelowPos(p2,nullptr);
	p2->setAboveBelowPos(p3,p1);
	p3->setAboveBelowPos(p4,p2);
	if (p4)
		p4->setAboveBelowPos(nullptr,p3);
}
void allPosClass::setUpObstacleOfColumn(int column,QVector<posClass*> v)
{
	posClass* p1=getPosWithPosIndex(column,1,false);
	posClass* p2=getPosWithPosIndex(column,2,false);
	posClass* p3=getPosWithPosIndex(column,3,false);
	posClass* p4=getPosWithPosIndex(column,4,false);
	p1->setObstaclePos(v);
	p2->setObstaclePos(v);
	p3->setObstaclePos(v);
	p4->setObstaclePos(v);
}
void allPosClass::setCraneClass(craneClass *c)
{
	crane=c;
}
posClass* allPosClass::getPosWithPosIndex(int pos,int index,bool unLockedOnly)
{
	for (int i=0;i<posV.count();i++)
	{
		if ( (posV[i]->getPositionNumber()==pos)&&(posV[i]->getPositionIndex()==index)  )
		{
			if (!unLockedOnly)
				return posV[i];
			else if (!posV[i]->getIsLocked())
				return posV[i];
			break;
		}
	}
	return nullptr;
}
posClass* allPosClass::getPosWithPlcIndex(int plcIndex,bool unLockedOnly)
{
	for (int i=0;i<posV.count();i++)
	{
		if (posV[i]->getIdForPlc()==plcIndex)
		{
			if (!unLockedOnly)
				return posV[i];
			else if (!posV[i]->getIsLocked())
				return posV[i];
			break;
		}
	}
	return nullptr;
}
posClass* allPosClass::getPosWithBasket(int b,bool unLockedOnly)
{
	posClass* ans=nullptr;
	for (int i=0;i<posV.count();i++)
	{
		if (posV[i]->getBasketNumber()==b)
		{
			if (!unLockedOnly)
				ans=posV[i];
			else if (!posV[i]->getIsLocked())
				ans=posV[i];
			break;
		}
	}
	return ans;
}
posClass* allPosClass::getPosInBufferToGetBasketInPos(posClass *pos)
{
	QVector<posClass*> allAvailable;
	for (int i=0;i<bufferV.count();i++)
	{
		if (bufferV[i].posNumber!=pos->getPositionNumber())
		{
			for (int j=0;j<bufferV[i].posV.count();j++)
			{
				if (bufferV[i].posV[j]->getCanGetBasketInPos(pos))
					allAvailable<<bufferV[i].posV[j];
			}
		}
	}
	qsort(allAvailable.data(),allAvailable.size(),sizeof(posClass*),[](const void *a,const void *b)->int{const posClass *pa=*(const posClass**)a,*pb=*(const posClass**)b;return pa->getPositionIndex()>pb->getPositionIndex();});
	for (int i=0;i<allAvailable.count();i++)
	{
		posClass *belowPos=allAvailable[i]->getBelowPos();
		if (belowPos&&belowPos->getTopContentsDataStruct().hasSameSoSoLine(pos->getBottomContentsDataStruct()))
			return allAvailable[i];
	}
	return allAvailable.first();
}
posClass* allPosClass::getPosWithBasketToMoveInAuto(int baskNum)
{
	if (baskNum==0)
		return nullptr;
	for (int i=0;i<posV.count();i++)
	{
		if (
			   (posV[i]->getPositionNumber()==crane2FromToPlcId)||
			   (posV[i]->getPositionNumber()==destackerWorkingPos1PlcId)||
			   (posV[i]->getPositionNumber()==destackerWorkingPos2PlcId)||
			   (posV[i]->getPositionNumber()==bufferApos)||
			   (posV[i]->getPositionNumber()==bufferBpos)||
			   (posV[i]->getPositionNumber()==bufferCpos)
			 )
		{
			if (posV[i]->getBasketNumber()==baskNum)
			{
				if (!posV[i]->getIsLocked())
					return posV[i];
			}
		}
	}
	return nullptr;
}
posClass* allPosClass::getPosWithEmptyBasketOnTop()
{
	for (int i=0;i<bufferV.count();i++)
	{
		posClass *pos=bufferV[i].getTopPositionWithBasket();
		if (pos&&pos->getHasEmptyBasket())
			return pos;
	}
	return nullptr;
}
QVector<posClass*> allPosClass::getTopPositionsWithBasket()
{
	QVector<posClass*> ans;
	for (int i=0;i<bufferV.count();i++)
	{
		posClass *pos=bufferV[i].getTopPositionWithBasket();
		if (pos)
			ans<<pos;
	}
	return ans;
}
int allPosClass::moveBasket(posClass *from,posClass *to)
{
	if ((!from)||(!to))
	{
		log(QString("moveBasket from==%1 to==%2").arg(from->getPosIndexDisplayString()).arg(to->getPosIndexDisplayString()),errorStr);
		uExit(errorStr,QString("moveBasket from==%1 to==%2").arg(from->getPosIndexDisplayString()).arg(to->getPosIndexDisplayString()));
	}
	int fromBasket=from->getBasketNumber();
	int toBasket=to->getBasketNumber();
	if ((fromBasket==0)||(toBasket!=0))
	{
		log(QString("moveBasket from==%1 to==%2, fromBasket:%3 toBasket:%4").arg(from->getPosIndexDisplayString()).arg(to->getPosIndexDisplayString()).arg(fromBasket).arg(toBasket),errorStr);
		uExit(errorStr,QString("moveBasket from==%1 to==%2, fromBasket:%3 toBasket:%4").arg(from->getPosIndexDisplayString()).arg(to->getPosIndexDisplayString()).arg(fromBasket).arg(toBasket));
	}
	logV(QString("moveBasket b=%1 f=%2 t=%3 ").arg(fromBasket).arg(from->getPosIndexDisplayString()).arg(to->getPosIndexDisplayString()));
	to->setBasketId(fromBasket,false,"move basket",from->getPositionNumber(),from->getPositionIndex());
	from->setBasketId(0);
	execQuery(QString("insert into c3logPos (pos,basket,info,time) values(%1,%2,'from %3','%4')").arg(to->getPosIndexDisplayString()).arg(fromBasket).arg(from->getPosIndexDisplayString()).arg(timeForLog),bDb);
	return fromBasket;
}
void allPosClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ( ((ip==crane3Ip)&&(table=="CR3_PCcomDint"))||((ip==crane2Ip)&&(table=="bConveyorBasket")) )
	{
		for (int i=0;i<posV.count();i++)
			posV[i]->dataInPlcChanged(ip,table,index,value);
	}
	if ((ip==crane3Ip)&&(table=="CR3_PCcomDint"))
	{
		if (index==0)//from
		{
			int from=value.toInt();
			if (from)
			{
				int col=floor(from/10)*10;
				int index=from-col;
				posClass *pos=getPosWithPosIndex(col,index);
				if (pos)
					pos->setIsFrom(true);
			}
			else
				clearIsFromPos();
		}
		if (index==1)//to
		{
			int to=value.toInt();
			if (to)
			{
				int col=floor(to/10)*10;
				int index=to-col;
				posClass *pos=getPosWithPosIndex(col,index);
				if (pos)
					pos->setIsTo(true);
			}
			else
				clearIsToPos();
		}
	}
}
void allPosClass::graphicsViewKeyPressedSlot(QKeyEvent *ev)
{
	if (ev->key()==Qt::Key_Escape)
	{
		if (manualSelectedFromPos)
		{
			manualSelectedFromPos->setIsFrom(false);
			manualSelectedFromPos=nullptr;
			setDisplayMode(posClass::displayToSelectFrom);
		}
	}
}
void allPosClass::setSelectMode(int m)
{
	selectMode=m;
	if (selectMode==selectForMission)
		setDisplayMode(posClass::displayToSelectFrom);
	else if (selectMode==selectToModify)
		setDisplayMode(posClass::displayToModify);
}
void allPosClass::posMouseReleasedSlot(posClass *p,QGraphicsSceneMouseEvent *ev)
{
	if (selectMode==selectForMission)
	{
		if (!crane->canGetNewMission())
			return;
		if (p->getIsLocked())
			return;
		if (!manualSelectedFromPos)
		{
			if (p->getBasketNumber()>0)
			{
				manualSelectedFromPos=p;
				manualSelectedFromPos->setIsFrom(true);
				setDisplayMode(posClass::displayToSelectTo);
			}
		}
		else
		{
			if ((manualSelectedFromPos==p)||(p->getBasketNumber()>0))
			{
				manualSelectedFromPos->setIsFrom(false);
				manualSelectedFromPos=nullptr;
				return;
			}
			crane->sendManualMission(manualSelectedFromPos,p);
			manualSelectedFromPos->setIsFrom(false);
			manualSelectedFromPos=nullptr;
			setDisplayMode(posClass::displayToSelectFrom);
		}
	}
}
void allPosClass::returnDestacker1PosBasket()
{
	int basket=getPosWithPosIndex(destackerWorkingPos1PlcId,1)->getBasketNumber();
	if (basket)
	{
		if (getFirst(QString("select count(tId) from %1 where basket=%2").arg(crane3ExportsToCrane2).arg(basket),bDb).toInt()==0)
			execQuery(QString("insert into %1 (basket,priority) select %2,isnull((select max(priority) from %1),0)+1 where not exists (select 1 from %1 where basket=%2)").arg(crane3ExportsToCrane2).arg(basket),bDb);
	}
}
void allPosClass::returnDestacker2PosBasket()
{
	int basket=getPosWithPosIndex(destackerWorkingPos2PlcId,1)->getBasketNumber();
	if (basket)
	{
		if (getFirst(QString("select count(tId) from %1 where basket=%2").arg(crane3ExportsToCrane2).arg(basket),bDb).toInt()==0)
			execQuery(QString("insert into %1 (basket,priority) select %2,isnull((select max(priority) from %1),0)+1 where not exists (select 1 from %1 where basket=%2)").arg(crane3ExportsToCrane2).arg(basket),bDb);
	}
}
void allPosClass::removeBasketFromExportTable(posClass *pos)
{
	if (!pos)
		return;
	int basket=pos->getBasketNumber();
	if (!basket)
		return;
	if (pos->getPositionNumber()==exit2PlcId)
	{
		execQuery(QString("delete from %1 where basket=%2").arg(crane3ExportsToCrane2).arg(basket),bDb);
		crane2Tux->writeInteger("bConveyorData",1,int(destinationStruct::inFromOut));
	}
	if (pos->getPositionNumber()==destackerWorkingPos1PlcId)
		execQuery(QString("delete from %1 where basket=%2").arg(crane3ExportsDestackerPositions1).arg(basket),bDb);
	if (pos->getPositionNumber()==destackerWorkingPos2PlcId)
		execQuery(QString("delete from %1 where basket=%2").arg(crane3ExportsDestackerPositions2).arg(basket),bDb);
	pos->setExportEnun(exportCrane3EnumClass::none);
	emit exportListChangedSignal();
}
void allPosClass::updateExportEnums()
{
	for (int i=0;i<posV.count();i++)
		posV[i]->setExportEnun(exportCrane3EnumClass::none);
	QVector<QVector<QVariant>> exportBasketsVV=execTableQuery(QString("select basket,%1 from %2 union select basket,%3 from %4 union select basket,%5 from %6")
		.arg(int(exportCrane3EnumClass::crane2)).arg(crane3ExportsToCrane2)
		.arg(int(exportCrane3EnumClass::packingDestacker1)).arg(crane3ExportsDestackerPositions1)
		.arg(int(exportCrane3EnumClass::packingDestacker2)).arg(crane3ExportsDestackerPositions2),bDb);
	for (int i=0;i<exportBasketsVV.count();i++)
	{
		int basket=exportBasketsVV[i][0].toInt();
		exportCrane3EnumClass exEnum=exportCrane3EnumClass(exportBasketsVV[i][1].toInt());
		posClass *pos=getPosWithBasket(basket,false);
		if (pos)
			pos->setExportEnun(exEnum);
	}
}
void allPosClass::exportListChangedSlot()
{
	updateExportEnums();
}
void allPosClass::userChangedBasketSlot(posClass *pos)
{
	if (selectMode==selectToModify)
	{
		setDisplayMode(posClass::displayToModify);
	}
}

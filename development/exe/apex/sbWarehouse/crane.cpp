#include "pch.h"
#include "crane.h"
craneClass::craneClass(tuxipClass *tux_,tuxipClass *tuxAutoCrane_,allPossClass *allPos_,QGraphicsScene *layoutScene_,bool useTranslation,QWidget *p):QWidget(p)
{
	splash->showMessage(tr("Initializing crane..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tux=tux_;
	tuxAutoCrane=tuxAutoCrane_;
	allPos=allPos_;
	setVisible(false);
	layoutScene=layoutScene_;
	autoMode=autoModeEnum::none;
	gItemClass *craneCarriageItem=new gItemClass(":dxfs/posConveyor.dxf",QPen(QColor(250,200,200)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *mouseOverItem=new gItemClass(":dxfs/posMouseOver.dxf",QPen(QColor(255,100,100)),this,QBrush(QColor(225,225,255,150)),Qt::NoBrush);
	gItemClass *cassetteItem=new gItemClass(":dxfs/posCassette.dxf",QPen(QColor(100,200,100)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *sbBlueItem=new gItemClass(":dxfs/posSbBlue.dxf",QPen(QColor(100,100,200)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *profilesItem=new gItemClass(":dxfs/posProfiles.dxf",QPen(QColor(100,100,200)),this,Qt::NoBrush,QBrush(QColor(225,255,255,150)));
	gItemClass *lockedGraphicsItem=new gItemClass(":dxfs/posLocked.dxf",QPen(QColor(255,100,100)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *activeFromItem=new gItemClass(":dxfs/posFrom.dxf",QPen(QColor(200,100,100)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *activeToItem=new gItemClass(":dxfs/posTo.dxf",QPen(QColor(200,100,100)),this,Qt::NoBrush,Qt::NoBrush);
	mouseOverItem->setZValue(-100);

	int cassetteNum=getOne(QString("select cassette from pos where posId=%1").arg(craneId),mDb).toInt();
	splash->showMessage(tr("Creating cariage..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	conveyor=new craneConveyorClass(tux,allPos,craneId,cassetteNum,"Sb crane",mouseOverItem,craneCarriageItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,p);
	conveyor->setYOffset(0.f,layoutYMinusOffset,layoutYPlusOffset);
	conveyor->setPlcMissionTable(sbWarehouseIp,"bRobotToPcI",0,1,2,10,11,12,13,18,30,31,32,33,34,35,8);
	splash->showMessage(tr("Loading cariage..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	conveyor->loadFromDatabase();

	splash->showMessage(tr("Initializing crane graphs..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QColor craneColor=QColor(175,75,75);
	topViewItem=new gItemClass(":dxfs/craneTopView.dxf",QPen(QColor(200,225,200)),this,Qt::NoBrush,Qt::NoBrush); layoutScene->addItem(topViewItem);topViewItem->setZValue(-1);
	sideViewUpItem=new gItemClass(":dxfs/craneSideView.dxf",QPen(craneColor),this,Qt::NoBrush,Qt::NoBrush); layoutScene->addItem(sideViewUpItem);sideViewUpItem->setZValue(-1);
	sideViewDownItem=new gItemClass(":dxfs/craneSideView.dxf",QPen(craneColor),this,Qt::NoBrush,Qt::NoBrush); layoutScene->addItem(sideViewDownItem);sideViewDownItem->setZValue(-1);
	carriageUpItem=new gItemClass(":dxfs/craneCarriage.dxf",QPen(craneColor),this,Qt::NoBrush,Qt::NoBrush); layoutScene->addItem(carriageUpItem);carriageUpItem->setZValue(-1);
	carriageDownItem=new gItemClass(":dxfs/craneCarriage.dxf",QPen(craneColor),this,Qt::NoBrush,Qt::NoBrush); layoutScene->addItem(carriageDownItem);carriageDownItem->setZValue(-1);
	drawerItem=new gItemClass(":dxfs/craneHooks.dxf",QPen(QColor(150,200,150)),this,Qt::NoBrush,Qt::NoBrush); layoutScene->addItem(drawerItem);drawerItem->setZValue(-1);
	//
	craneWidget=new craneWidgetClass(tux,conveyor->getCraneConveyorWidget(),useTranslation,this);
	craneWidget->setCraneConveyorClass(conveyor);
	//
	missionsTimer=new QTimer(this);
	connect(missionsTimer,SIGNAL(timeout()),this,SLOT(missionsTimerSlot()));
	craneWidget->addMessageToGui(QString("..."),QString("Program started"),infoStr);
	isPlcConnected=true;
}
craneClass::~craneClass()
{
	delete craneWidget;
}
void craneClass::setIsPlcConnected(bool v)
{
	isPlcConnected=v;
	conveyor->setIsPlcConnected(isPlcConnected);
}
void craneClass::addToAutoMode(int mode)
{
	autoMode=autoModeEnum(autoMode|mode);
	missionsTimerSlot();
}
void craneClass::removeFromAutoMode(int mode)
{
	autoMode=autoModeEnum(autoMode^mode);
}
QWidget* craneClass::getCraneWidget()
{
	return craneWidget;
}
craneConveyorClass *craneClass::getCraneConveyor()
{
	return conveyor;
}
void craneClass::startMissions()
{
	missionsTimer->start(500);
}
void craneClass::loadFromDatabase()
{
	conveyor->loadFromDatabase();
}
void craneClass::sendManualMission(posClass *from,posClass *to)
{
	if (!isPlcConnected)
		return;
	if (conveyor->getIsIdle()&&conveyor->isReadyForNewMission())
		conveyor->missionStart(from,to);
}
void craneClass::checkForNextMissionInDatabase()
{
	if (!isPlcConnected)
		return;
	conveyor->checkForNextMissionInDatabase();
}
void craneClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((ip==sbWarehouseIp)&&(table=="bRobotToPcF"))
	{
		if (index==0)
			plcX=value.toFloat();
		if (index==6)
			plcZ=value.toFloat();
		if (index==12)
			plcY=value.toFloat();
		if ((index==0)||(index==6)||(index==12))
		{
			conveyor->setPlcPos(plcX,plcY,plcZ);

			topViewItem->setPos(plcX,0);
			sideViewUpItem->setPos(plcX,layoutYMinusOffset);
			sideViewDownItem->setPos(plcX,layoutYPlusOffset);
			carriageUpItem->setPos(plcX,plcZ+layoutYMinusOffset);
			carriageDownItem->setPos(plcX,plcZ+layoutYPlusOffset);
			drawerItem->setPos(plcX,+(plcY-3588.));
		}
	}
	if ((ip==sbWarehouseIp)&&(table=="bRobotToPcI"))
	{
		conveyor->dataInPlcChanged(ip,table,index,value);
		if ((index==27))
		{
			drawerItem->setPen(getValueOfBit(value.toInt(),25)?QPen(QColor(150,255,150)):QPen(QColor(255,100,100)));
		}
	}
	craneWidget->dataInPlcChanged(ip,table,index,value);
}
void craneClass::startMission(posClass *from,posClass *to)
{
	if ((from)&&(to))
	{
		if (conveyor->getCanStartNewMission(from,to))
		{
			craneWidget->addMessageToGui(QString("A: %1 to %2 single").arg(from->getDescription()).arg(to->getDescription()),QString("Starting new single mission: A: %1 to %2").arg(from->getDescription()).arg(to->getDescription()),infoStr);
			conveyor->missionStart(from,to);
		}
	}
}
bool craneClass::getHasNextMission()
{
	return conveyor->getHasNextMission();
}
bool getIsPosActive(int pos)
{
	return getFirst(QString("select count(tId) from missions where fromPos=%1 or toPos=%1").arg(pos),mDb).toInt()!=0;
}
bool craneClass::getImportsFromWest()
{
	if (autoMode&autoWestImports)
	{
		if ((!getIsPosActive(conveyor__6_id))&&(allPos->getPosWithId(conveyor__6_id)->getHasCassette()))
		{
			conveyor->missionStart(allPos->getPosWithId(conveyor__6_id),allPos->getPosWithId(allPos->getPosWithId(conveyor__6_id)->getCassetteNumber()));
			return true;
		}
	}
	return false;
}
bool craneClass::sendEmptySbToWest()
{
	if (!(autoMode&autoWestExportsEmptySb))
		return false;
	if (allPos->getPosWithId(conveyor__1_id)->getHasCassette())
		return false;
	int maxNoOfEmptySbInWest=3;
	int noOfEmptySBInWest=0;
	if (allPos->getPosWithId(conveyor__1_id)->getHasEmptySb())
		noOfEmptySBInWest++;
	if (allPos->getPosWithId(conveyor__2_id)->getHasEmptySb())
		noOfEmptySBInWest++;
	if (allPos->getPosWithId(pincer1_41_id)->getHasEmptySb())
		noOfEmptySBInWest++;
	if (allPos->getPosWithId(pincer1_49_id)->getHasEmptySb())
		noOfEmptySBInWest++;
	if (allPos->getPosWithId(pincer1_45_id)->getHasEmptySb())
		noOfEmptySBInWest++;
	if (allPos->getPosWithId(pincer1_50_id)->getHasTicket())
		noOfEmptySBInWest--;
	if (allPos->getPosWithId(pincer1Id)->getHasEmptySb())
		noOfEmptySBInWest++;
	//robot
	int toPos=getFirst(QString("select toPos from missions where conveyorPos='%1'").arg(activeStr),mDb).toInt();
	bool isGoingToStacker=(toPos==conveyor__1_id);
	if (isGoingToStacker)
	{
		if (conveyor->getHasEmptySb())
			noOfEmptySBInWest++;
	}
	if (noOfEmptySBInWest<maxNoOfEmptySbInWest)
	{
		int cassetteForDestackerArea=getFirst(QString("select cassette from pos %1 and ticket=0 and sbNumber<>0 order by colNo,rowNo limit 1 ").arg(whereIdleCassette),mDb).toInt();
		if ((cassetteForDestackerArea!=0)&&(!getIsPosActive(cassetteForDestackerArea)))
		{
			posClass *posForDouble=allPos->getPosWithId(cassetteForDestackerArea,true,false);
			if (posForDouble)
			{
				if (conveyor->getCanStartNewMission(posForDouble,allPos->getPosWithId(conveyor__1_id)))
				{
					conveyor->missionStart(posForDouble,allPos->getPosWithId(conveyor__1_id));
					return true;
				}
			}
		}
	}
	return false;
}
bool craneClass::sendExportsToWest()
{
	if (!(autoMode&autoWestExportsFullSb))
		return false;
	if (allPos->getPosWithId(conveyor__1_id)->getHasCassette())
		return false;
	if (allPos->getPosWithId(pincer1Id)->getHasSb())
		return false;
	if (allPos->getPosWithId(pincer1_52_id)->getHasSb())
		return false;
	if (allPos->getPosWithId(pincer1_54_id)->getHasSb())
		return false;
	int cassetteForDestackerArea=getFirst(QString("select cassette from %1 order by priority limit 1").arg(exportsFrontTable),mDb).toInt();
	posClass *posForStackerArea=allPos->getPosWithId(cassetteForDestackerArea,true,false);
	if (!posForStackerArea)
	{
		execQuery(QString("delete from %1 where cassette=%2").arg(exportsFrontTable).arg(cassetteForDestackerArea),mDb);
		return false;
	}
	if ( (cassetteForDestackerArea!=0)&&(!getIsPosActive(conveyor__1_id))&&(!getIsPosActive(cassetteForDestackerArea)) )
	{
		if (!allPos->getPosWithId(conveyor__1_id)->getHasCassette())
		{
			conveyor->missionStart(posForStackerArea,allPos->getPosWithId(conveyor__1_id));
			return true;
		}
	}
	return false;
}
bool craneClass::sendEmptyCassettesToWest()
{
	if (!(autoMode&autoWestExportsEmptyCassettes))
		return false;
	if (allPos->getPosWithId(conveyor__1_id)->getHasCassette())
		return false;
	if (allPos->getPosWithId(conveyor__2_id)->getHasCassette())
		return false;
	if (allPos->getPosWithId(conveyor__4_id)->getHasCassette()&&(tuxAutoCrane->readIntegerValue("pc1ToPCI",11)!=4))
		return false;
	int cassetteForDestackerArea=getFirst(QString("select cassette from pos %1 and sbNumber=0 and cassette<>0 order by colNo,rowNo limit 1").arg(whereIdleCassette),mDb).toInt();
	posClass *posForStackerArea=allPos->getPosWithId(cassetteForDestackerArea,true,false);
	if (!posForStackerArea)
		return false;
	if ( (cassetteForDestackerArea!=0)&&(!getIsPosActive(conveyor__1_id))&&(!getIsPosActive(cassetteForDestackerArea)) )
	{
		if (!allPos->getPosWithId(conveyor__1_id)->getHasCassette())
		{
			conveyor->missionStart(posForStackerArea,allPos->getPosWithId(conveyor__1_id));
			return true;
		}
	}
	return false;
}
bool craneClass::getImportsFromTriple()
{
	if (!(autoMode&tripleImports))
		return false;
	if ((!getIsPosActive(cassetteTripleExit_11_id))&&(allPos->getPosWithId(cassetteTripleExit_11_id)->getHasCassette()))
	{
		conveyor->missionStart(allPos->getPosWithId(cassetteTripleExit_11_id),allPos->getPosWithId(allPos->getPosWithId(cassetteTripleExit_11_id)->getCassetteNumber()));
		return true;
	}
	return false;
}
bool craneClass::sendExportsToTripleWest()
{
	if (!(autoMode&tripleExportsWest))
		return false;
	if (allPos->getPosWithId(cassetteTripleExit__8_id)->getHasCassette())
		return false;
	int cassette=getFirst(QString("select cassette from %1 order by priority limit 1").arg(exportsTriple1Table),mDb).toInt();
	posClass *pos=allPos->getPosWithId(cassette,true,false);
	if (!pos)
	{
		execQuery(QString("delete from %1 where cassette=%2").arg(exportsTriple1Table).arg(cassette),mDb);
		return false;
	}
	if ( (cassette!=0)&&(!getIsPosActive(cassetteTripleExit__8_id))&&(!getIsPosActive(cassette)) )
	{
		if (!allPos->getPosWithId(conveyor__1_id)->getHasCassette())
		{
			conveyor->missionStart(pos,allPos->getPosWithId(cassetteTripleExit__8_id));
			return true;
		}
	}
	return false;
}
bool craneClass::sendExportsToTripleEast()
{
	if (!(autoMode&tripleExportsEast))
		return false;
	if (allPos->getPosWithId(cassetteTripleExit_12_id)->getHasCassette())
		return false;
	int cassette=getFirst(QString("select cassette from %1 order by priority limit 1").arg(exportsTriple2Table),mDb).toInt();
	posClass *pos=allPos->getPosWithId(cassette,true,false);
	if (!pos)
	{
		execQuery(QString("delete from %1 where cassette=%2").arg(exportsTriple2Table).arg(cassette),mDb);
		return false;
	}
	if ( (cassette!=0)&&(!getIsPosActive(cassetteTripleExit_12_id))&&(!getIsPosActive(cassette)) )
	{
		if (!allPos->getPosWithId(cassetteTripleExit_12_id)->getHasCassette())
		{
			conveyor->missionStart(pos,allPos->getPosWithId(cassetteTripleExit_12_id));
			return true;
		}
	}
	return false;
}
bool craneClass::getImportsFromDouble()
{
	if (!(autoMode&doubleExportImports))
		return false;
	if ((!getIsPosActive(cassetteDoubleExit_16_id))&&(allPos->getPosWithId(cassetteDoubleExit_16_id)->getHasCassette()))
	{
		conveyor->missionStart(allPos->getPosWithId(cassetteDoubleExit_16_id),allPos->getPosWithId(allPos->getPosWithId(cassetteDoubleExit_16_id)->getCassetteNumber()));
		return true;
	}
	return false;
}
bool craneClass::sendExportsToDouble()
{
	if (!(autoMode&doubleExportFullSB))
		return false;
	if (allPos->getPosWithId(cassetteDoubleExit_15_id)->getHasCassette())
		return false;
	int cassette=getFirst(QString("select cassette from %1 order by priority limit 1").arg(exportsDoubleTable),mDb).toInt();
	posClass *pos=allPos->getPosWithId(cassette,true,false);
	if (!pos)
	{
		execQuery(QString("delete from %1 where cassette=%2").arg(exportsDoubleTable).arg(cassette),mDb);
		return false;
	}
	if ( (cassette!=0)&&(!getIsPosActive(cassetteDoubleExit_15_id))&&(!getIsPosActive(cassette)) )
	{
		if (!allPos->getPosWithId(cassetteDoubleExit_15_id)->getHasCassette())
		{
			conveyor->missionStart(pos,allPos->getPosWithId(cassetteDoubleExit_15_id));
			return true;
		}
	}
	return false;
}
bool craneClass::sendEmptyToDouble()
{
	if (!(autoMode&doubleExportEmptyCassettes))
		return false;
	if (allPos->getPosWithId(cassetteDoubleExit_15_id)->getHasCassette())
		return false;
	if (allPos->getPosWithId(cassetteDoubleExit_18_id)->getHasCassette())
		return false;
	if (allPos->getPosWithId(cassetteDoubleExit_17_id)->getHasCassette())
		return false;
	int cassette=getFirst(QString("select cassette from pos %1 and sbNumber=0 and cassette<>0 order by colNo,rowNo limit 1").arg(whereIdleCassette),mDb).toInt();
	posClass *pos=allPos->getPosWithId(cassette,true,false);
	if (!cassette)
		return false;
	if ( (cassette!=0)&&(!getIsPosActive(cassetteDoubleExit_15_id))&&(!getIsPosActive(cassette)) )
	{
		if (!allPos->getPosWithId(cassetteDoubleExit_15_id)->getHasCassette())
		{
			conveyor->missionStart(pos,allPos->getPosWithId(cassetteDoubleExit_15_id));
			return true;
		}
	}
	return false;
}
void craneClass::missionsTimerSlot()
{
	if (!isPlcConnected)
		return;
	if (autoMode==none)
		return;
	if (getHasNextMission())
		return;
	if (!conveyor->isMissionAboutToEnd())
		return;
	if (!conveyor->isReadyForNewMission())
		return;
	//take returns if in the area
	int toPos=getFirst(QString("select toPos from missions where conveyorPos='%1'").arg(activeStr),mDb).toInt();
	bool isGoingToStacker=(toPos==conveyor__1_id);
	bool isGoingToDouble=(toPos==cassetteDoubleExit_15_id);
	bool isGoingToTriple=( (toPos==cassetteTripleExit__8_id)||(toPos==cassetteTripleExit_12_id) );
	if (isGoingToStacker)
	{
		if (getImportsFromWest())return;
		if (sendExportsToWest())return;
		if (sendEmptySbToWest())return;
		if (sendEmptyCassettesToWest())return;
		
		if (sendExportsToTripleWest())return;
		if (sendExportsToTripleEast())return;
		if (getImportsFromTriple())return;

		if (sendExportsToDouble())return;
		if (sendEmptyToDouble())return;
		if (getImportsFromDouble())return;
	}
	else if (isGoingToDouble)
	{
		if (getImportsFromDouble())return;
		if (getImportsFromTriple())return;
		if (getImportsFromWest())return;

		if (sendExportsToWest())return;
		if (sendEmptySbToWest())return;
		if (sendEmptyCassettesToWest())return;
	
		if (sendExportsToTripleWest())return;
		if (sendExportsToTripleEast())return;
	
		if (sendExportsToDouble())return;
		if (sendEmptyToDouble())return;
	}
	else if (isGoingToTriple)
	{
		if (getImportsFromTriple())return;

		if (sendExportsToWest())return;
		if (sendEmptySbToWest())return;
		if (sendEmptyCassettesToWest())return;
		if (getImportsFromWest())return;

		if (sendExportsToTripleWest())return;
		if (sendExportsToTripleEast())return;
		
		if (sendExportsToDouble())return;
		if (sendEmptyToDouble())return;
		if (getImportsFromDouble())return;
	}
	else
	{
		if (getImportsFromWest())return;
		if (sendExportsToWest())return;
		if (sendEmptySbToWest())return;
		if (sendEmptyCassettesToWest())return;
		
		if (getImportsFromTriple())return;
		if (sendExportsToTripleWest())return;
		if (sendExportsToTripleEast())return;

		if (sendExportsToDouble())return;
		if (sendEmptyToDouble())return;
		if (getImportsFromDouble())return;
	}
}

	
#include "pch.h"
#include "carriage.h"
carriageClass::carriageClass(tuxipClass *tuxipConnection,allPosClass *allPos_,QString description,QGraphicsScene *scene,gItemClass *posItem_,gItemClass *mouseOverItem_,gItemClass *basketItem_,gItemClass *profilesItem_,gItemClass *lockedItem_,gItemClass *fromItem_,gItemClass *toItem_,QWidget *p):QWidget(p)
{
	splash->showMessage(tr("Initializing carriage..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	pos=new posClass(allPos_,crane3PlcId,0,0.,0.,0.,4200,scene,posItem_,mouseOverItem_,basketItem_,profilesItem_,lockedItem_,fromItem_,toItem_,true,p);
	pos->loadFromDatabase();
	pos->setDisplayMode(posClass::displayNormal);
	allPos=allPos_;
	tuxip=tuxipConnection;
	carriageWidget=new carriageWidgetClass(this);
	nextFromPos=nextToPos=nullptr;
	activeFromPos=activeToPos=nullptr;
	activeMissionStatus=plcStatusIdle;
	activeCraneStep=plcStepIdle;
	carriageWidget->addMessageToGui(QString("..."),QString("Program started"),infoStr);

	QTimer *checkStatusTimer=new QTimer(this);
	connect(checkStatusTimer,SIGNAL(timeout()),this,SLOT(checkStatusTimerSlot()));
	checkStatusTimer->start(1000);
#ifdef emulateCrane3
	emulateTimerMsec=1000;
	emulateTimer=new QTimer(this);
	connect(emulateTimer,SIGNAL(timeout()),this,SLOT(emulateTimerSlot()));
	emulateTimer->start(emulateTimerMsec);

	emulateReturnBasketFromPosition1Timer=new QTimer(this);
	connect(emulateReturnBasketFromPosition1Timer,SIGNAL(timeout()),this,SLOT(emulateReturnBasketFromPosition1TimerSlot()));
	emulateReturnBasketFromPosition1Timer->setSingleShot(true);
	emulateReturnBasketFromPosition2Timer=new QTimer(this);
	connect(emulateReturnBasketFromPosition2Timer,SIGNAL(timeout()),this,SLOT(emulateReturnBasketFromPosition2TimerSlot()));
	emulateReturnBasketFromPosition2Timer->setSingleShot(true);
#endif
}
QString carriageClass::getCraneStepText(int s)
{
	return carriageWidget->getCraneStepText(s);
}
QString carriageClass::getCraneStepDescription(int s)
{
	return carriageWidget->getCraneStepDescription(s);
}
QWidget *carriageClass::getCarriageWidget() const
{
	return carriageWidget;
}
void carriageClass::setPlcPos(float x,float z)
{
	pos->setPlcPos(x,z);
}
void carriageClass::setDisplayMode(int m)
{
	if (m==posClass::displayToModify)
		pos->setDisplayMode(m);
	else
		pos->setDisplayMode(posClass::displayNormal);
}
void carriageClass::setNextFromPos(posClass *p)
{
	if ((nextFromPos)&&(p))
	{
		carriageWidget->addMessageToGui(QString("internal error"),QString("setNextFromPos, p!=null while nextFromPos!=null"),errorStr);
		return;
	}
	nextFromPos=p;
	if (nextFromPos)
		carriageWidget->setNextFrom(nextFromPos->getDescription());
	else
	{
		carriageWidget->setNextFrom("");
		execQuery(QString("update c3missions set fromPosNumber=0,fromPosIndex=0 where status='%1'").arg(nextStr),bDb);
	}
}
void carriageClass::setNextToPos(posClass *p)
{
	if ((nextToPos)&&(p))
	{
		carriageWidget->addMessageToGui(QString("internal error"),QString("setNextToPos, p!=null while nextToPos!=null"),errorStr);
		return;
	}
	nextToPos=p;
	if (nextToPos)
		carriageWidget->setNextTo(nextToPos->getDescription());
	else
	{
		carriageWidget->setNextTo("");
		execQuery(QString("update c3missions set toPosNumber=0,toPosIndex=0 where status='%1'").arg(nextStr),bDb);
	}
}
void carriageClass::setActiveFromPos(posClass *p)
{
	if ((activeFromPos)&&(p))
	{
		carriageWidget->addMessageToGui(QString("internal error"),QString("setActiveFromPos, p!=null while activeFromPos!=null"),errorStr);
		return;
	}
	activeFromPos=p;
	if (activeFromPos)
		carriageWidget->setActiveFrom(activeFromPos->getDescription());
	else
		carriageWidget->setActiveFrom("");
}
void carriageClass::setActiveToPos(posClass *p)
{
	if ((activeToPos)&&(p))
	{
		carriageWidget->addMessageToGui(QString("internal error"),QString("setActiveToPos, p!=null while activeToPos!=null"),errorStr);
		return;
	}
	activeToPos=p;
	if (activeToPos)
		carriageWidget->setActiveTo(activeToPos->getDescription());
	else
		carriageWidget->setActiveTo("");
}
bool carriageClass::moveNextToActive()
{
	if ((nextFromPos)&&(nextToPos)&&(!activeFromPos)&&(!activeToPos))
	{
		carriageWidget->addMessageToGui("Making next mission active","Moving data from of next mission to active mission",infoStr);
		execQuery(QString("update c3missions set fromPosNumber=%1,fromPosIndex=%2,toPosNumber=%3,toPosIndex=%4 where status='%5'").arg(nextFromPos->getPositionNumber()).arg(nextFromPos->getPositionIndex()).arg(nextToPos->getPositionNumber()).arg(nextToPos->getPositionIndex()).arg(activeStr),bDb);
		execQuery(QString("update c3missions set fromPosNumber=0,fromPosIndex=0,toPosNumber=0,toPosIndex=0 where status='%1'").arg(nextStr),bDb);
		setActiveFromPos(nextFromPos);
		setActiveToPos(nextToPos);
		setNextFromPos(NULL);
		setNextToPos(NULL);
		return true;
	}
	else if ((nextFromPos)&&(nextToPos)&&(activeFromPos)&&(activeToPos))
	{
		if ( 
			(nextFromPos->getPositionNumber()==activeFromPos->getPositionNumber())&&(nextFromPos->getPositionIndex()==activeFromPos->getPositionIndex())  
			&&  
			(nextToPos->getPositionNumber()==activeToPos->getPositionNumber())&&(nextToPos->getPositionIndex()==activeToPos->getPositionIndex())
			)
			return true;
		else
			carriageWidget->addMessageToGui("Tried to move next to active","Internal error, active pos is not empty and next not equal to active",errorStr);
		return true;
	}
	//else if ((!nextFromPos)&&(!nextToPos)&&(activeFromPos)&&(activeToPos))
	//{
	//	carriageWidget->addMessageToGui("Move next to active","Move next mission to active, mission allready moved",infoStr);
	//	return true;
	//}
	return false;
}
void carriageClass::setPlcMissionTable(QString plcIp_,QString plcMissionTable_,int plcNextMissionFromIndex_,int plcNextMissionToIndex_,int plcNextMissionStatusIndex_,int plcCurrentMissionFromIndex_,int plcCurrentMissionToIndex_,int plcCurrentMissionPcStatusIndex_,int plcCurrentMissionPLcStatusIndex_,int plcCurrentCranePlcStepIndex_)
{
	plcIp=plcIp_;
	plcMissionTable=plcMissionTable_;
	plcNextMissionFromIndex=plcNextMissionFromIndex_;
	plcNextMissionToIndex=plcNextMissionToIndex_;
	plcNextMissionStatusIndex=plcNextMissionStatusIndex_;
	plcCurrentMissionFromIndex=plcCurrentMissionFromIndex_;
	plcCurrentMissionToIndex=plcCurrentMissionToIndex_;
	plcCurrentMissionPcStatusIndex=plcCurrentMissionPcStatusIndex_;
	plcCurrentMissionPLcStatusIndex=plcCurrentMissionPLcStatusIndex_;
	plcCurrentCranePlcStepIndex=plcCurrentCranePlcStepIndex_;
}
void carriageClass::checkForNextMissionInDatabase()
{
	carriageWidget->addMessageToGui(QString("Load from database"),QString("Loading data from database"),infoStr);
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select fromPosNumber,fromPosIndex,toPosNumber,toPosIndex from c3missions where status='%1'").arg(nextStr),bDb);
	if (dataVV.count()==1)
	{
		int fromPosNumber=dataVV[0][0].toInt();
		int fromPosIndex=dataVV[0][1].toInt();
		int toPosNumber=dataVV[0][2].toInt();
		int toPosIndex=dataVV[0][3].toInt();
		if ((fromPosNumber!=0)&&(fromPosIndex!=0)&&(toPosNumber!=0)&&(toPosIndex!=0))
		{
			posClass *f=allPos->getPosWithPosIndex(fromPosNumber,fromPosIndex);
			posClass *t=allPos->getPosWithPosIndex(toPosNumber,toPosIndex);
			carriageWidget->addMessageToGui(QString("Found next mission"),QString("from %1 to %2").arg(f->getDescription()).arg(t->getDescription()),infoStr);
			missionStart(f,t);
		}
		if ((fromPosNumber==0)&&(fromPosIndex==0)&&(toPosNumber==0)&&(toPosIndex==0))
			carriageWidget->addMessageToGui(QString("No next missions"),QString("No next missions found in database"),infoStr);
	}
}
void carriageClass::loadFromDatabase()
{
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select fromPosNumber,fromPosIndex,toPosNumber,toPosIndex from c3missions where status='%1'").arg(activeStr),bDb);
	if (dataVV.count()==1)
	{
		int fromPosNumber=dataVV[0][0].toInt();
		int fromPosIndex=dataVV[0][1].toInt();
		int toPosNumber=dataVV[0][2].toInt();
		int toPosIndex=dataVV[0][3].toInt();
		if ((fromPosNumber!=0)&&(fromPosIndex!=0)&&(toPosNumber!=0)&&(toPosIndex!=0))
		{
			QMessageBox::critical(this,"Mission",tr("Found mission from %1 to %2.").arg(fromPosNumber+fromPosIndex).arg(toPosNumber+toPosIndex));
			if (QMessageBox::question(this,tr("Found mission"),tr("If the programm does not start select yes to clear the mission. You have to correct the baskets numbers in the programm"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes)
			{
				clearMissionInDb("user");
				return;
			}			
			posClass *f=allPos->getPosWithPosIndex(fromPosNumber,fromPosIndex);
			posClass *t=allPos->getPosWithPosIndex(toPosNumber,toPosIndex);
			setActiveFromPos(f);
			setActiveToPos(t);
			carriageWidget->addMessageToGui(QString("Found active mission"),QString("from %1 to %2").arg(f->getDescription()).arg(t->getDescription()),infoStr);
		}
		else
			carriageWidget->addMessageToGui(QString("No missions"),QString("No active missions found in database"),infoStr);
	}
}
bool carriageClass::getIsIdle() const
{
	return currentMissionPLcStatus==0;
}
bool carriageClass::isReadyForNewMission()
{
	return (nextMissionFrom==0)&&(nextMissionTo==0)&&(nextMissionStatus==plcStatusIdle)&&(nextFromPos==NULL)&&(nextToPos==NULL)&&(!pos->getHasBasket());
}
bool carriageClass::getCanStartNewMission(posClass* f,posClass* t)
{
	if (!isReadyForNewMission())
		return false;
	if (tuxip->getHasError())
		return false;
	if ((!f)||(!t))
		return false;
	if (!f->getHasBasket())
		return false;
	if (t->getHasBasket())
		return false;
	if (!f->getIsPlcReadyToSendBasket())
		return false;
	if (!t->getIsPlcReadyToGetBasket())
		return false;
	if (f->getIsLocked())
	{
		carriageWidget->addMessageToGui(QString("Can not start new mission from:%1").arg(f->getPosIndexDisplayString()),"Position is locked",infoStr);
		log(QString("Can not start new mission from:%1, position is locked").arg(f->getPosIndexDisplayString()),errorStr);
		return false;
	}
	if (t->getIsLocked())
	{
		carriageWidget->addMessageToGui(QString("Can not start new mission to:%1").arg(t->getPosIndexDisplayString()),"Position is locked",infoStr);
		log(QString("Can not start new mission to:%1, position is locked").arg(t->getPosIndexDisplayString()),errorStr);
		return false;
	}
	if (!t->getCanGetBasketInPos(f))
		return false;
	if (!f->getIsFreeToMove())
		return false;
	if (!t->getIsFreeToMove())
		return false;
	if (isPosActiveG(f->getPositionNumber()))
		return false;
	if (isPosActiveG(t->getPositionNumber()))
		return false;
	if (!isReadyForNewMission())
	{
		carriageWidget->addMessageToGui(QString("Can not start new mission"),"It is not ready to start a new mission",infoStr);
		log(QString("startNewMission f=%1 t=%2 s=%3 ").arg(f->getPosIndexDisplayString()).arg(t->getPosIndexDisplayString()).arg(currentMissionPLcStatus),errorStr);
		return false;
	}
	if ((!canPlcSendBasketToCrane2)&&(t->getPositionIndex()==exit2PlcId))
		return false;
	QVector<QVector<QVariant>> nDataVV=execTableQuery(QString("select fromPosNumber,fromPosIndex,toPosNumber,toPosIndex from c3missions where status='%1'").arg(nextStr),bDb);
	int nextFromPosNumber= nDataVV[0][0].toInt();
	int nextFromPosIndex= nDataVV[0][1].toInt();
	int nextToPosNumber= nDataVV[0][2].toInt();
	int nextToPosIndex= nDataVV[0][3].toInt();
	if (  
		(nextFromPosNumber==f->getPositionNumber())&&(nextFromPosIndex=f->getPositionIndex())
		&&
		(nextToPosNumber==t->getPositionNumber())&&(nextToPosIndex=t->getPositionIndex())
		)
	{
		log(QString("missionStart f:%1 t:%2 allready next").arg(f->getPosIndexDisplayString()).arg(t->getPosIndexDisplayString()),infoStr);
		return false;
	}
	QVector<QVector<QVariant>> aDataVV=execTableQuery(QString("select fromPosNumber,fromPosIndex,toPosNumber,toPosIndex from c3missions where status='%1'").arg(activeStr),bDb);
	int activeFromPosNumber= aDataVV[0][0].toInt();
	int activeFromPosIndex= aDataVV[0][1].toInt();
	int activeToPosNumber= aDataVV[0][2].toInt();
	int activeToPosIndex= aDataVV[0][3].toInt();
	if (  
		(activeFromPosNumber==f->getPositionNumber())&&(activeFromPosIndex=f->getPositionIndex())
		&&
		(activeToPosNumber==t->getPositionNumber())&&(activeToPosIndex=t->getPositionIndex())
		)
	{
		log(QString("missionStart f:%1 t:%2 allready active").arg(f->getPosIndexDisplayString()).arg(t->getPosIndexDisplayString()),infoStr);
		return false;
	}
	return true;
}
bool carriageClass::missionStart(posClass* f,posClass*t)
{	
	if (f)f->loadFromDatabase();
	if (t)t->loadFromDatabase();
	if (!getCanStartNewMission(f,t))
		return false;
	setNextFromPos(f);
	setNextToPos(t);
	logV(QString("startNewMission f=%1 t=%2").arg(nextFromPos->getPosIndexDisplayString()).arg(nextToPos->getPosIndexDisplayString()));
	execQuery(QString("update c3missions set fromPosNumber=%1,fromPosIndex=%2,toPosNumber=%3,toPosIndex=%4 where status='%5'").arg(nextFromPos->getPositionNumber()).arg(nextFromPos->getPositionIndex()).arg(nextToPos->getPositionNumber()).arg(nextToPos->getPositionIndex()).arg(nextStr),bDb);
	carriageWidget->addMessageToGui(QString("New mission %1 to %2").arg(f->getDescription()).arg(t->getDescription()),"Starting new mission",infoStr);
	if ((!nextFromPos)||(!nextToPos))
	{
		log(QString("missionStart ((!fromPos)||(!toPos))"),errorStr);
		return false;
	}
	if (nextFromPos->getBasketNumber()==0)
	{
		log(QString("missionStart fromPos->getBasketNumber()==0 from:%1").arg(nextFromPos->getPosIndexDisplayString()),errorStr);
		return false;
	}
	carriageWidget->addMessageToGui(QString("%1 to %2 ").arg(f->getDescription()).arg(t->getDescription()),QString("Starting mission:%1 to %2").arg(f->getDescription()).arg(t->getDescription()),infoStr);
	return missionSendToPlc();
}
void carriageClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (inMyLatopG)
		return;
	if ((plcIp==ip)&&(table=="CR3_PCcomDint")&&(23))
		canPlcSendBasketToCrane2=getValueOfBit(value.toInt(),5);
	if ((plcIp==ip)&&(plcMissionTable==table))
	{
		if (index==plcNextMissionFromIndex)nextMissionFrom=value.toInt();
		if (index==plcNextMissionToIndex)nextMissionTo=value.toInt();
		if (index==plcNextMissionStatusIndex)nextMissionStatus=value.toInt();
		if (index==plcCurrentMissionFromIndex)currentMissionFrom=value.toInt();
		if (index==plcCurrentMissionToIndex)currentMissionTo=value.toInt();
		if (index==plcCurrentMissionPcStatusIndex)currentMissionPcStatus=value.toInt();
		if (index==plcCurrentMissionPLcStatusIndex){plcStatusChanged(value.toInt());}
		if (index==plcCurrentCranePlcStepIndex){carriageWidget->setActiveCraneStep(value.toInt());}
	}
}
void carriageClass::checkStatusTimerSlot()
{
	plcStatusChanged(currentMissionPLcStatus);
}
void carriageClass::emulateTimerSlot()
{
#ifdef emulateCrane3
	nextMissionFrom=tuxip->readIntegerValue(plcMissionTable,plcNextMissionFromIndex);
	nextMissionTo=tuxip->readIntegerValue(plcMissionTable,plcNextMissionToIndex);
	nextMissionStatus=tuxip->readIntegerValue(plcMissionTable,plcNextMissionStatusIndex);
	currentMissionFrom=tuxip->readIntegerValue(plcMissionTable,plcCurrentMissionFromIndex);
	currentMissionTo=tuxip->readIntegerValue(plcMissionTable,plcCurrentMissionToIndex);
	currentMissionPcStatus=tuxip->readIntegerValue(plcMissionTable,plcCurrentMissionPcStatusIndex);
	currentMissionPLcStatus=tuxip->readIntegerValue(plcMissionTable,plcCurrentMissionPLcStatusIndex);
	if (nextMissionFrom&&nextMissionTo)
	{
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionFromIndex,nextMissionFrom);
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionToIndex,nextMissionTo);
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionPLcStatusIndex,plcStatusStart);
		tuxip->writeInteger(plcMissionTable,plcNextMissionFromIndex,0);
		tuxip->writeInteger(plcMissionTable,plcNextMissionToIndex,0);
		tuxip->writeInteger(plcMissionTable,plcNextMissionStatusIndex,plcStatusIdle);
		tuxip->writeFloat("CR3_PCcomReal",0,allPos->getPosWithPlcIndex(nextMissionFrom)->getPlcX());
		emulateTimer->start(emulateTimerMsec);
		return;
	}
	if ( (currentMissionPLcStatus==plcStatusStart)&&(currentMissionPcStatus==plcStatusStart))
	{
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionPLcStatusIndex,plcStatusBascketOnConv);
		if (currentMissionTo)
			tuxip->writeFloat("CR3_PCcomReal",0,allPos->getPosWithPlcIndex(currentMissionTo)->getPlcX());
		emulateTimer->start(emulateTimerMsec);
		return;
	}
	if ( (currentMissionPLcStatus==plcStatusBascketOnConv)&&(currentMissionPcStatus==plcStatusBascketOnConv))
	{
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionPLcStatusIndex,plcStatusFinished);
		emulateTimer->start(emulateTimerMsec);
		return;
	}
	if ( (currentMissionPLcStatus==plcStatusFinished)&&(currentMissionPcStatus==plcStatusFinished))
	{
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionFromIndex,0);
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionToIndex,0);
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionPLcStatusIndex,plcStatusIdle);
		emulateTimer->start(emulateTimerMsec);
		return;
	}
	//conveyor
	posClass *fromCranePos=allPos->getPosWithPosIndex(exit2PlcId,1);
	fromCranePos->loadFromDatabase();
	allPos->updateExportEnums();
	if ((fromCranePos->getExportEnun()!=exportCrane3EnumClass::packingDestacker1)&&(fromCranePos->getExportEnun()!=exportCrane3EnumClass::packingDestacker2))
		fromCranePos->setBasketId(0);
	int basketToRotatingNorth=getFirst(QString("select basketTableId from positions where posNumber=%1 and posIndex=%3").arg(toRotatingNorthPlcId).arg(1),bDb).toInt();
	int basketRotatingNorth=getFirst(QString("select basketTableId from positions where posNumber=%1 and posIndex=%3").arg(rotatingNorthPlcId).arg(1),bDb).toInt();
	int basketExit1=getFirst(QString("select basketTableId from positions where posNumber=%1 and posIndex=%3").arg(exit1PlcId).arg(1),bDb).toInt();
	int basketExit2=getFirst(QString("select basketTableId from positions where posNumber=%1 and posIndex=%3").arg(exit2PlcId).arg(1),bDb).toInt();
	if ( (basketToRotatingNorth==0)&&(basketRotatingNorth==0)&&(basketExit1==0)&&(basketExit2==0) )
		tuxip->writeInteger("CR3_PCcomDint",23,-1);
	else
		tuxip->writeInteger("CR3_PCcomDint",23,0);
	//destacker
	int basketOnDestackerPos1=getFirst(QString("select basketTableId from positions where posNumber=%1 and posIndex=%3").arg(destackerWorkingPos1PlcId).arg(1),bDb).toInt();
	if (basketOnDestackerPos1&&(!emulateReturnBasketFromPosition1Timer->isActive()))
		emulateReturnBasketFromPosition1Timer->start(5*emulateTimerMsec);
	int basketOnDestackerPos2=getFirst(QString("select basketTableId from positions where posNumber=%1 and posIndex=%3").arg(destackerWorkingPos2PlcId).arg(1),bDb).toInt();
	if (basketOnDestackerPos2&&(!emulateReturnBasketFromPosition2Timer->isActive()))
		emulateReturnBasketFromPosition2Timer->start(5*emulateTimerMsec);
#endif
}
void carriageClass::emulateReturnBasketFromPosition1TimerSlot()
{
#ifdef emulateCrane3
	int basket=getFirst(QString("select basketTableId from positions where posNumber=%1 and posIndex=%3").arg(destackerWorkingPos1PlcId).arg(1),bDb).toInt();
	execQuery(QString("delete from RackDetail where RackId=%1").arg(basket),eDb);
	allPos->returnDestacker1PosBasket();
	allPos->getPosWithPosIndex(destackerWorkingPos1PlcId,1)->loadFromDatabase();
#endif
}
void carriageClass::emulateReturnBasketFromPosition2TimerSlot()
{
#ifdef emulateCrane3
	int basket=getFirst(QString("select basketTableId from positions where posNumber=%1 and posIndex=%3").arg(destackerWorkingPos2PlcId).arg(1),bDb).toInt();
	execQuery(QString("delete from RackDetail where RackId=%1").arg(basket),eDb);
	allPos->returnDestacker2PosBasket();
	allPos->getPosWithPosIndex(destackerWorkingPos2PlcId,1)->loadFromDatabase();
#endif
}
void carriageClass::plcStatusChanged(int s)
{
	currentMissionPLcStatus=s;
	if (currentMissionPLcStatus==plcStatusIdle)
	{
		if (activeFromPos!=NULL)
			log(QString("carriageClass::setStatus plcStatus:%1 and activeFromPos!=NULL").arg(currentMissionPLcStatus),errorStr);
		if (activeToPos!=NULL)
			log(QString("carriageClass::setStatus plcStatus:%1 and activeToPos!=NULL").arg(currentMissionPLcStatus),errorStr);
		if ((activeFromPos!=NULL)||(activeToPos!=NULL))
		{
			carriageWidget->addMessageToGui(QString("Plc status 0"),"Pc has active mission and plc is with status 0",errorStr);
			log(QString("plcStatus==0 and pc in a mission"),errorStr);
		}
	}
	else if (currentMissionPLcStatus==plcStatusStart)
	{
		if (moveNextToActive())
		{
#ifdef logMissionsTimes
		execQuery(QString("insert into crane3MissionsTimeLog (fromPos,toPos,startSec,endSec,time) values(%1,%2,%3,0,'%4')").arg(activeFromPos->getIdForPlc()).arg(activeToPos->getIdForPlc()).arg(QDateTime::currentDateTime().toTime_t()).arg(timeForLog),bDb);
#endif
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusStart);
		}
	}
	else if (currentMissionPLcStatus==plcStatusBascketOnConv)
		missionBasketOnConveyor();
	else if (currentMissionPLcStatus==plcStatusFinished)
		missionFinished();
	else if (currentMissionPLcStatus==plcStatusAborted)
		missionAborted();
}

bool carriageClass::missionSendToPlc(bool force)
{
	if ((!nextFromPos)||(!nextToPos))
	{
		log(QString("sendMissionToPlc ((!fromPos)||(!toPos))"),errorStr);
		return false;
	}
	carriageWidget->addMessageToGui(QString("Send to plc %1 to %2").arg(nextFromPos->getPosIndexDisplayString()).arg(nextToPos->getPosIndexDisplayString()),QString("Sending data to plc"),infoStr);
	tuxip->writeInteger(plcMissionTable,plcNextMissionFromIndex,nextFromPos->getIdForPlc());
	tuxip->writeInteger(plcMissionTable,plcNextMissionToIndex,nextToPos->getIdForPlc());
	tuxip->writeInteger(plcMissionTable,plcNextMissionStatusIndex,plcStatusStart);
	return true;
}
int carriageClass::moveBasket(posClass *from,posClass *to)
{
	if ((from==NULL)||(to==NULL))
	{
		log(QString("moveBasket ((from==NULL)||(to==NULL))"),errorStr);
		uExit(errorStr,QString("moveBasket ((from==NULL)||(to==NULL))"));
	}
	carriageWidget->addMessageToGui(QString("Move basket from %1 to %2").arg(from->getIdForPlc()).arg(to->getIdForPlc()),QString("Moving basket"),infoStr);
	int basket=allPos->moveBasket(from,to);
	if (from==pos)
		pos->loadFromDatabase();
	else
		from->loadFromDatabase();
	if (to==pos)
		pos->loadFromDatabase();
	else
		to->loadFromDatabase();
	return basket;
}
void carriageClass::clearNextMission()
{
	setNextFromPos(0);
	setNextToPos(0);
	tuxip->writeInteger("CR3_PCcomDint",30,0);
	tuxip->writeInteger("CR3_PCcomDint",31,0);
	tuxip->writeInteger("CR3_PCcomDint",32,0);
}
void carriageClass::clearMissionInDb(QString user)
{
	carriageWidget->addMessageToGui(QString("Clear mission"),QString("Mission has ended from %1, clearing data from database").arg(user.length()>0?user:"plc"),infoStr);
	execQuery(QString("update c3missions set fromPosNumber=0,fromPosIndex=0,toPosNumber=0,toPosIndex=0 where status='%1'").arg(nextStr),bDb);
	execQuery(QString("update c3missions set fromPosNumber=0,fromPosIndex=0,toPosNumber=0,toPosIndex=0 where status='%1'").arg(activeStr),bDb);

	setActiveFromPos(NULL);
	setActiveToPos(NULL);
	tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusFinished);
}
void carriageClass::missionBasketOnConveyor()
{
	if (activeFromPos)
	{
		if ((activeFromPos->getBasketNumber()!=0) && (pos->getBasketNumber()==0))
		{
			bool isToPosEmpty=activeToPos->getBasketNumber()==0;
			if (isToPosEmpty)
			{
				int b=moveBasket(activeFromPos,pos);
				tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusBascketOnConv);
			}
		}
		else
		{
		}
	}
	else
		carriageWidget->addMessageToGui(QString("Basket on conveyor"),QString("Plc moved basket on conveyor and pc has no active mission."),errorStr);
}
void carriageClass::missionFinished()
{
	carriageWidget->addMessageToGui(QString("Plc: mission finished"),QString("Mission has ended from plc"),infoStr);
	if (activeToPos&&(activeToPos->getBasketNumber()==0) && (pos->getBasketNumber()!=0))
	{
#ifdef logMissionsTimes 
		execQuery(QString("update crane3MissionsTimeLog set endSec=%1 where fromPos=%2 and toPos=%3 and endSec=0").arg(QDateTime::currentDateTime().toTime_t()).arg(activeFromPos->getIdForPlc()).arg(activeToPos->getIdForPlc()),bDb);
#endif
		pos->loadFromDatabase();
		activeToPos->loadFromDatabase();
		int basket=moveBasket(pos,activeToPos);
		if (basket)
		{
			allPos->removeBasketFromExportTable(activeToPos);
	#ifdef emulateCrane3
			if (activeToPos->getPositionNumber()==exit2PlcId)
			{
				activeToPos->setBasketId(0);
			}
	#endif
			emit exportListChangedSignal();
			clearMissionInDb();
			tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusFinished);
		}
		logV(QString("missionFinished "));
	}
	else if (activeFromPos&&activeToPos)
	{
		if (activeToPos&&(activeToPos->getBasketNumber()!=0))
			carriageWidget->addMessageToGui(QString("Basket move"),QString("Plc moved basket to destination and pc has allready a basket there."),errorStr);
		if (pos->getBasketNumber()==0)
			carriageWidget->addMessageToGui(QString("Basket move"),QString("Plc moved basket to destination and pc has no basket on hooks."),errorStr);
	}
}
void carriageClass::missionAborted()
{
	carriageWidget->addMessageToGui(QString("Mission was aborted"),QString("Mission has aborted by plc"),infoStr);
	int movedBasket=0;
	if (activeToPos&&activeToPos->getBasketNumber()!=0)
		movedBasket=moveBasket(activeToPos,activeFromPos);
	if ((pos->getBasketNumber()!=0)&&(activeFromPos))
		movedBasket=moveBasket(pos,activeFromPos);
	if (movedBasket==0)
	{
		clearMissionInDb();
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusAborted);
		logV(QString("missionAborted"));
	}
	else
		carriageWidget->addMessageToGui(QString("Mission aborted"),QString("Mission was aborted from plc and there is no basket on conveyor or the source position."),errorStr);
}




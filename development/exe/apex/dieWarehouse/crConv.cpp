#include "pch.h"
#include "crConv.h"
extern QSplashScreen *splash;
craneConveyorClass::craneConveyorClass(tuxipClass *tuxipConnection,allPossClass *allPos_,int plcId,int basketNum,QString description,QGraphicsScene *scene,dxfGroupOfGraphicsItemClass *mouseOverBasketGraphicsItem,dxfGroupOfGraphicsItemClass *cellItem,dxfGroupOfGraphicsItemClass *fullItem,dxfGroupOfGraphicsItemClass *emptyItem,dxfGroupOfGraphicsItemClass *lockedItem,QWidget *p):
triplePosClass(plcId,basketNum,description,scene,mouseOverBasketGraphicsItem,cellItem,fullItem,emptyItem,lockedItem,p)
{
	splash->showMessage(tr("Initializing conveyor..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	allPos=allPos_;
	tuxip=tuxipConnection;
	dbActiveName=activeStr;
	dbNextName=nextStr;
	craneConveyorWidget=new craneConveyorWidgetClass(this);
	nextFromPos=nextToPos=NULL;
	activeFromPos=activeToPos=NULL;
	nextFromXCorr=nextFromYCorr=nextFromZCorr=nextToXCorr=nextToYCorr=nextToZCorr=0;
	activeMissionStatus=plcStatusIdle;
	activeCraneStep=plcCraneStepIdle;
	activeHooksStep=plcHooksStepIdle;
	craneConveyorWidget->addMessageToGui(QString("..."),QString("Program started"),infoStr);
	basketOnConveyorTimer=new QTimer(this);
}
craneConveyorClass::~craneConveyorClass()
{
}
void craneConveyorClass::setExcludedPositions(QVector<int> excludedPosV_)
{
	excludedPosV=excludedPosV_;
}
QString craneConveyorClass::getCraneStepText(int s)
{
	return craneConveyorWidget->getCraneStepText(s);
}
QString craneConveyorClass::getCraneStepDescription(int s)
{
	return craneConveyorWidget->getCraneStepDescription(s);
}
QString craneConveyorClass::getHooksStepText(int s)
{
	return craneConveyorWidget->getHooksStepText(s);
}
QString craneConveyorClass::getHooksStepDescription(int s)
{
	return craneConveyorWidget->getHooksStepDescription(s);
}
QWidget *craneConveyorClass::getCraneConveyorWidget()
{
	return craneConveyorWidget;
}
void craneConveyorClass::setNextFromPos(posClass *p)
{
	if ((nextFromPos)&&(p))
	{
		craneConveyorWidget->addMessageToGui(QString("internal error"),QString("setNextFromPos, p!=null while nextFromPos!=null"),errorStr);
		return;
	}
	nextFromPos=p;
	if (nextFromPos)
		craneConveyorWidget->setNextFrom(nextFromPos->getDescription());
	else
	{
		craneConveyorWidget->setNextFrom("");
		execQuery(QString("update missions set fromPos=0 where status='%1'").arg(dbNextName),db);
	}
}
void craneConveyorClass::setNextToPos(posClass *p)
{
	if ((nextToPos)&&(p))
	{
		craneConveyorWidget->addMessageToGui(QString("internal error"),QString("setNextToPos, p!=null while nextToPos!=null"),errorStr);
		return;
	}
	nextToPos=p;
	if (nextToPos)
		craneConveyorWidget->setNextTo(nextToPos->getDescription());
	else
	{
		craneConveyorWidget->setNextTo("");
		execQuery(QString("update missions set toPos=0 where status='%1'").arg(dbNextName),db);
	}
}
void craneConveyorClass::setActiveFromPos(posClass *p)
{
	if ((activeFromPos)&&(p))
	{
		craneConveyorWidget->addMessageToGui(QString("internal error"),QString("setActiveFromPos, p!=null while activeFromPos!=null"),errorStr);
		return;
	}
	activeFromPos=p;
	if (activeFromPos)
		craneConveyorWidget->setActiveFrom(activeFromPos->getDescription());
	else
		craneConveyorWidget->setActiveFrom("");
}
void craneConveyorClass::setActiveToPos(posClass *p)
{
	if ((activeToPos)&&(p))
	{
		craneConveyorWidget->addMessageToGui(QString("internal error"),QString("setActiveToPos, p!=null while activeToPos!=null"),errorStr);
		return;
	}
	activeToPos=p;
	if (activeToPos)
		craneConveyorWidget->setActiveTo(activeToPos->getDescription());
	else
		craneConveyorWidget->setActiveTo("");
}
bool craneConveyorClass::moveNextToActive()
{
	if ((nextFromPos)&&(nextToPos)&&(!activeFromPos)&&(!activeToPos))
	{
		craneConveyorWidget->addMessageToGui("Making next mission active","Moving data from of next mission to active mission",infoStr);
		execQuery(QString("update missions set fromPos=%1,toPos=%2 where status='%3'").arg(nextFromPos->getPlcId()).arg(nextToPos->getPlcId()).arg(dbActiveName),db);
		setActiveFromPos(nextFromPos);
		setActiveToPos(nextToPos);
		setNextFromPos(NULL);
		setNextToPos(NULL);
		return true;
	}
	else if ((nextFromPos)&&(nextToPos)&&(activeFromPos)&&(activeToPos))
	{
		if ( (nextFromPos->getPlcId()==activeFromPos->getPlcId())  &&  (nextToPos->getPlcId()==activeToPos->getPlcId())  )
			return true;
		else
			craneConveyorWidget->addMessageToGui("Tried to move next to active","Internal error, active pos is not empty and next not equal to active",errorStr);
		return true;
	}
	else if ((!nextFromPos)&&(!nextToPos)&&(activeFromPos)&&(activeToPos))
	{
		craneConveyorWidget->addMessageToGui("Move next to active","Move next mission to active, mission allready moved",infoStr);
		return true;
	}
	return false;
}
void craneConveyorClass::setPlcMissionTable(QString plcIp_,QString plcMissionTable_,int plcNextMissionFromIndex_,int plcNextMissionToIndex_,int plcNextMissionStatusIndex_,int plcCurrentMissionFromIndex_,int plcCurrentMissionToIndex_,int plcCurrentMissionPcStatusIndex_,int plcCurrentMissionPLcStatusIndex_,int plcCurrentCranePlcStepIndex_,int plcCurrentHooksPlcStepIndex_,int plcNextFromXcorrIndex_,int plcNextFromYcorrIndex_,int plcNextFromZcorrIndex_,int plcNextToXcorrIndex_,int plcNextToYcorrIndex_,int plcNextToZcorrIndex_,int plcNextMissionBasketIndex_)
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
	plcCurrentHooksPlcStepIndex=plcCurrentHooksPlcStepIndex_;
	plcNextFromXcorrIndex=plcNextFromXcorrIndex_;
	plcNextFromYcorrIndex=plcNextFromYcorrIndex_;
	plcNextFromZcorrIndex=plcNextFromZcorrIndex_;
	plcNextToXcorrIndex=plcNextToXcorrIndex_;
	plcNextToYcorrIndex=plcNextToYcorrIndex_;
	plcNextToZcorrIndex=plcNextToZcorrIndex_;
	plcNextMissionBasketIndex=plcNextMissionBasketIndex_;
}
void craneConveyorClass::checkForNextMissionInDatabase()
{
	craneConveyorWidget->addMessageToGui(QString("Load from database"),QString("Loading data from database"),infoStr);
	int fromNext=getFirst(QString("select fromPos from missions where status='%1'").arg(dbNextName),db).toInt();
	int toNext=getFirst(QString("select toPos from missions where status='%1'").arg(dbNextName),db).toInt();
	if ((fromNext!=0)&&(toNext!=0))
	{
		posClass *f=allPos->getPosWithId(fromNext);
		posClass *t=allPos->getPosWithId(toNext);
		craneConveyorWidget->addMessageToGui(QString("Found next mission"),QString("from %1 to %2").arg(f->getDescription()).arg(t->getDescription()),infoStr);
		missionStart(f,t);
	}
	if ((fromNext==0)&&(toNext==0))
		craneConveyorWidget->addMessageToGui(QString("No next missions"),QString("No next missions found in database"),infoStr);
#ifdef plcEmulate
	//if (activeFromPos&&activeToPos)
	//{
	//	tuxip->writeInteger(plcMissionTable,plcCurrentMissionFromIndex,activeFromPos->getPlcId());
	//	tuxip->writeInteger(plcMissionTable,plcCurrentMissionToIndex,activeToPos->getPlcId());
	//}
#endif
}
void craneConveyorClass::loadFromDatabase()
{
	int fromActive=getFirst(QString("select fromPos from missions where status='%1'").arg(dbActiveName),db).toInt();
	int toActive=getFirst(QString("select toPos from missions where status='%1'").arg(dbActiveName),db).toInt();
	if ((fromActive!=0)&&(toActive!=0))
	{
		posClass *f=allPos->getPosWithId(fromActive);
		posClass *t=allPos->getPosWithId(toActive);
		setActiveFromPos(f);
		setActiveToPos(t);
		craneConveyorWidget->addMessageToGui(QString("Found active mission"),QString("from %1 to %2").arg(f->getDescription()).arg(t->getDescription()),infoStr);
	}
	if ((fromActive==0)&&(toActive==0))
		craneConveyorWidget->addMessageToGui(QString("No missions"),QString("No active missions found in database"),infoStr);
}
bool craneConveyorClass::getIsIdle()
{
	return currentMissionPLcStatus==0;
}
bool craneConveyorClass::isReadyForNewMission()
{
	return (nextMissionFrom==0)&&(nextMissionTo==0)&&(nextMissionStatus==plcStatusIdle)&&(nextFromPos==NULL)&&(nextToPos==NULL)/*&&(basketNum==0)*/;
}
bool craneConveyorClass::getCanStartNewMission(posClass* f,posClass* t)
{
	if (!isReadyForNewMission())
		return false;
	if (tuxip->getHasError())
		return false;
	if ((!f)||(!t))
		return false;
	for (int i=0;i<excludedPosV.count();i++)
	{
		if ((excludedPosV[i]==f->getPlcId())||(excludedPosV[i]==t->getPlcId()))
		{
			return false;
		}
	}
	if (f->getIsLocked())
	{
		craneConveyorWidget->addMessageToGui(QString("Can not start new mission from:%1").arg(f->getPlcId()),"Position is locked",infoStr);
		log(QString("Can not start new mission from:%1, position is locked").arg(f->getPlcId()),errorStr);
		return false;
	}
	if (t->getIsLocked())
	{
		craneConveyorWidget->addMessageToGui(QString("Can not start new mission to:%1").arg(t->getPlcId()),"Position is locked",infoStr);
		log(QString("Can not start new mission to:%1, position is locked").arg(t->getPlcId()),errorStr);
		return false;
	}
	if (!isReadyForNewMission())
	{
		craneConveyorWidget->addMessageToGui(QString("Can not start new mission"),"It is not ready to start a new mission",infoStr);
		log(QString("startNewMission f=%1 t=%2 s=%3 ").arg(currentMissionFrom).arg(currentMissionTo).arg(currentMissionPLcStatus),errorStr);
		return false;
	}
	if (!isReadyForNewMission())
	{
		craneConveyorWidget->addMessageToGui(QString("Can not start new mission"),"It is not ready to start a new mission",infoStr);
		log(QString("startNewMission f=%1 t=%2 s=%3").arg(currentMissionFrom).arg(currentMissionTo).arg(currentMissionPLcStatus),errorStr);
		return false;
	}
	if (!t->getCanTakeBasketInPos(f->getPlcId()))
		return false;
	int dbNextFrom=getFirst(QString("select fromPos from missions where status='%1'").arg(nextStr),db).toInt();
	int dbNextTo=getFirst(QString("select fromPos from missions where status='%1'").arg(nextStr),db).toInt();
	if ((dbNextFrom==f->getPlcId())&&(dbNextTo==t->getPlcId()))
	{
		log(QString("missionStart f:%1 t:%2 allready next").arg(f->getPlcId()).arg(t->getPlcId()),infoStr);
		return false;
	}
	int dbActiveFrom=getFirst(QString("select fromPos from missions where status='%1'").arg(activeStr),db).toInt();
	int dbActiveTo=getFirst(QString("select fromPos from missions where status='%1'").arg(activeStr),db).toInt();
	if ((dbActiveFrom==f->getPlcId())&&(dbActiveTo==t->getPlcId()))
	{
		log(QString("missionStart f:%1 t:%2 allready next").arg(f->getPlcId()).arg(t->getPlcId()),infoStr);
		return false;
	}
	return true;
}
void craneConveyorClass::missionStart(posClass* f,posClass*t)
{
	if (!getCanStartNewMission(f,t))
		return;
	setNextFromPos(f);
	setNextToPos(t);
	logV(QString("startNewMission f=%1 t=%2").arg(nextFromPos->getPlcId()).arg(nextToPos->getPlcId()));
	execQuery(QString("update missions set fromPos=%1,toPos=%2 where status='%3'").arg(nextFromPos->getPlcId()).arg(nextToPos->getPlcId()).arg(dbNextName),db);
	craneConveyorWidget->addMessageToGui(QString("New mission %1 to %2").arg(f->getDescription()).arg(t->getDescription()),"Starting new mission",infoStr);
	if ((!nextFromPos)||(!nextToPos))
	{
		log(QString("missionStart ((!fromPos)||(!toPos))"),errorStr);
		return;
	}
	if (nextFromPos->getBasketNumber()==0)
	{
		log(QString("missionStart fromPos->getBasketNumber()==0 from:%1").arg(nextFromPos->getPlcId()),errorStr);
		return;
	}
	missionSendToPlc();
}
void craneConveyorClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((plcIp==ip)&&(plcMissionTable==table))
	{
		if (index==plcNextMissionFromIndex)nextMissionFrom=value.toInt();
		if (index==plcNextMissionToIndex)nextMissionTo=value.toInt();
		if (index==plcNextMissionStatusIndex)nextMissionStatus=value.toInt();
		if (index==plcCurrentMissionFromIndex)currentMissionFrom=value.toInt();
		if (index==plcCurrentMissionToIndex)currentMissionTo=value.toInt();
		if (index==plcCurrentMissionPcStatusIndex)currentMissionPcStatus=value.toInt();
		if (index==plcCurrentMissionPLcStatusIndex){plcStatusChanged(value.toInt());}
		if (index==plcCurrentCranePlcStepIndex){craneConveyorWidget->setActiveCraneStep(value.toInt());}
		if (index==plcCurrentHooksPlcStepIndex){craneConveyorWidget->setActiveHooksStep(value.toInt());}
	}
}
void craneConveyorClass::plcStatusChanged(int s)
{
	if (currentMissionPLcStatus==s)
		return;
	currentMissionPLcStatus=s;
	if (currentMissionPLcStatus==plcStatusIdle)
	{
		if (activeFromPos!=NULL)
			log(QString("craneConveyorClass::setStatus plcStatus:%1 and activeFromPos!=NULL").arg(currentMissionPLcStatus),errorStr);
		if (activeToPos!=NULL)
			log(QString("craneConveyorClass::setStatus plcStatus:%1 and activeToPos!=NULL").arg(currentMissionPLcStatus),errorStr);
		if ((activeFromPos!=NULL)||(activeToPos!=NULL))
		{
			craneConveyorWidget->addMessageToGui(QString("Plc status 0"),"Pc has active mission and plc is with status 0",errorStr);
			log(QString("plcStatus==0 and pc in a mission"),errorStr);
		}
	}
	else if (currentMissionPLcStatus==plcStatusStart)
	{

		if (moveNextToActive())
		{
#ifdef logMissionsTimes
		execQuery(QString("insert into missionsTimeLog (fromPos,toPos,startSec,endSec,time) values(%1,%2,%3,0,'%4')").arg(activeFromPos->getPlcId()).arg(activeToPos->getPlcId()).arg(QDateTime::currentDateTime().toTime_t()).arg(timeForLog),db);
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

void craneConveyorClass::missionSendToPlc(bool force)
{
	if ((!nextFromPos)||(!nextToPos))
	{
		log(QString("sendMissionToPlc ((!fromPos)||(!toPos))"),errorStr);
		return;
	}
	craneConveyorWidget->addMessageToGui(QString("Send to plc %1 to %2").arg(nextFromPos->getPlcId()).arg(nextToPos->getPlcId()),QString("Sending data to plc"),infoStr);
	if (nextMissionFrom!=nextFromPos->getPlcId())
	{
		tuxip->writeInteger(plcMissionTable,plcNextMissionFromIndex,nextFromPos->getPlcId());
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg(plcMissionTable).arg(plcNextMissionFromIndex).arg(nextFromPos->getPlcId()));
	}
	if (nextMissionTo!=nextToPos->getPlcId())
	{
		tuxip->writeInteger(plcMissionTable,plcNextMissionToIndex,nextToPos->getPlcId());
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg(plcMissionTable).arg(plcNextMissionToIndex).arg(nextToPos->getPlcId()));
	}
	//basket number
	tuxip->writeInteger(plcMissionTable,plcNextMissionBasketIndex,nextFromPos->getBasketNumber());
	//corrections
	if (ufNEqual(nextFromXCorr,nextFromPos->getPlcXCorrection()))
	{
		tuxip->writeFloat("bRobotToPcF",plcNextFromXcorrIndex,nextFromPos->getPlcXCorrection());
		logV(QString("missionSendToToPlc.writeFloat %1[%2]=%3").arg("bRobotToPcF").arg(plcNextFromXcorrIndex).arg(nextFromPos->getPlcXCorrection()));
	}
	if (ufNEqual(nextFromYCorr,nextFromPos->getPlcYCorrection()))
	{
		tuxip->writeFloat("bRobotToPcF",plcNextFromYcorrIndex,nextFromPos->getPlcYCorrection());
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg("bRobotToPcF").arg(plcNextFromYcorrIndex).arg(nextFromPos->getPlcYCorrection()));
	}
	if (ufNEqual(nextFromZCorr,nextFromPos->getPlcZCorrection()))
	{
		tuxip->writeFloat("bRobotToPcF",plcNextFromZcorrIndex,nextFromPos->getPlcZCorrection());
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg("bRobotToPcF").arg(plcNextFromZcorrIndex).arg(nextFromPos->getPlcZCorrection()));
	}

	if (ufNEqual(nextToXCorr,nextToPos->getPlcXCorrection()))
	{
		tuxip->writeFloat("bRobotToPcF",plcNextToXcorrIndex,nextToPos->getPlcXCorrection());
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg("bRobotToPcF").arg(plcNextToXcorrIndex).arg(nextToPos->getPlcXCorrection()));
	}
	if (ufNEqual(nextToYCorr,nextToPos->getPlcYCorrection()))
	{
		tuxip->writeFloat("bRobotToPcF",plcNextToYcorrIndex,nextToPos->getPlcYCorrection());
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg("bRobotToPcF").arg(plcNextToYcorrIndex).arg(nextToPos->getPlcYCorrection()));
	}
	if (ufNEqual(nextToZCorr,nextToPos->getPlcZCorrection()))
	{
		tuxip->writeFloat("bRobotToPcF",plcNextToZcorrIndex,nextToPos->getPlcZCorrection());
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg("bRobotToPcF").arg(plcNextToZcorrIndex).arg(nextToPos->getPlcZCorrection()));
	}
	//status allways last to send
	if (nextMissionStatus!=plcStatusStart)
	{
		tuxip->writeInteger(plcMissionTable,plcNextMissionStatusIndex,plcStatusStart);
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg(plcMissionTable).arg(plcNextMissionStatusIndex).arg(plcStatusStart));
	}
}
int craneConveyorClass::moveBasket(posClass *from,posClass *to)
{
	if ((from==NULL)||(to==NULL))
	{
		log(QString("moveBasket ((from==NULL)||(to==NULL))"),errorStr);
		uExit(errorStr,QString("moveBasket ((from==NULL)||(to==NULL))"));
	}
	craneConveyorWidget->addMessageToGui(QString("Move basket from %1 to %2").arg(from->getPlcId()).arg(to->getPlcId()),QString("Moving basket"),infoStr);
	int basket=staticMoveBasket(from->getPlcId(),to->getPlcId());
	if (from==this)
		triplePosClass::loadFromDatabase();
	else
		from->loadFromDatabase();
	if (to==this)
		triplePosClass::loadFromDatabase();
	else
		to->loadFromDatabase();
	return basket;
}
void craneConveyorClass::clearMissionInDb(QString user)
{
	craneConveyorWidget->addMessageToGui(QString("Clear mission"),QString("Mission has ended from %1, clearing data from database").arg(user.length()>0?user:"plc"),infoStr);
	execQuery(QString("update missions set fromPos=0,toPos=0 where status='%1'").arg(dbActiveName),db);
	setActiveFromPos(NULL);
	setActiveToPos(NULL);
	tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusFinished);
}
void craneConveyorClass::missionBasketOnConveyor()
{
	if (activeFromPos)
	{
		if ((activeFromPos->getBasketNumber()!=0) && (this->getBasketNumber()==0))
		{
			moveBasket(activeFromPos,this);
			bool isToPosEmpty=activeToPos->getBasketNumber()==0;
			if (isToPosEmpty)
			{
				basketOnConveyorTimer->stop();//to pos got empty, stop timer
				tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusBascketOnConv);
			}
		}
		else
		{
			if (activeFromPos->getBasketNumber()==0)
				craneConveyorWidget->addMessageToGui(QString("Basket on conveyor"),QString("Plc moved basket on conveyor and pc has no basket in source position."),errorStr);
			if (this->getBasketNumber()!=0)
				craneConveyorWidget->addMessageToGui(QString("Basket on conveyor"),QString("Plc moved basket on conveyor and pc allready has a basket on conveyor."),errorStr);
		}
	}
	else
		craneConveyorWidget->addMessageToGui(QString("Basket on conveyor"),QString("Plc moved basket on conveyor and pc has no active mission."),errorStr);
}
void craneConveyorClass::missionFinished()
{
	craneConveyorWidget->addMessageToGui(QString("Plc: mission finished"),QString("Mission has ended from plc"),infoStr);
	if (activeToPos&&(activeToPos->getBasketNumber()==0) && (this->getBasketNumber()!=0))
	{
#ifdef logMissionsTimes
		execQuery(QString("update missionsTimeLog set endSec=%1 where fromPos=%2 and toPos=%3 and endSec=0").arg(QDateTime::currentDateTime().toTime_t()).arg(activeFromPos->getPlcId()).arg(activeToPos->getPlcId()),db);
#endif
		int b=moveBasket(this,activeToPos);
		clearMissionInDb();
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusFinished);
		logV(QString("missionFinished "));
	}
	else
	{
		if (activeToPos&&(activeToPos->getBasketNumber()!=0))
			craneConveyorWidget->addMessageToGui(QString("Basket on conveyor"),QString("Plc moved basket to destination and pc has allready a basket there."),errorStr);
		if (this->getBasketNumber()==0)
			craneConveyorWidget->addMessageToGui(QString("Basket on conveyor"),QString("Plc moved basket to destination and pc has no basket on conveyor."),errorStr);
	}
}
void craneConveyorClass::missionAborted()
{
	craneConveyorWidget->addMessageToGui(QString("Mission was aborted"),QString("Mission has aborted by plc"),infoStr);
	int movedBasket=0;
	if (activeToPos&&activeToPos->getBasketNumber()!=0)
		movedBasket=moveBasket(activeToPos,activeFromPos);
	if ((this->getBasketNumber()!=0)&&(activeFromPos))
		movedBasket=moveBasket(this,activeFromPos);
	if (movedBasket==0)
	{
		clearMissionInDb();
		tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusAborted);
		logV(QString("missionAborted"));
	}
	else
		craneConveyorWidget->addMessageToGui(QString("Mission aborted"),QString("Mission was aborted from plc and there is no basket on conveyor or the source position."),errorStr);
}

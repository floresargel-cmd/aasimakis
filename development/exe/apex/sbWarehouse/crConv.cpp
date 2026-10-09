#include "pch.h"
#include "crConv.h"
craneConveyorClass::craneConveyorClass(tuxipClass *tux_,allPossClass *allPos_,int posId,int cassetteNum,QString description
	,gItemClass *mouseOverItem,gItemClass *cellItem,gItemClass *cassetteItem,gItemClass *sbBlueItem,gItemClass *profilesItem,gItemClass *lockedItem,gItemClass *activeFromItem,gItemClass *activeToItem
	,QWidget *p):
triplePosClass(posId,cassetteNum,description,mouseOverItem,cellItem,cassetteItem,sbBlueItem,profilesItem,lockedItem,activeFromItem,activeToItem,p)
{
	splash->showMessage(tr("Initializing conveyor"),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	allPos=allPos_;
	tux=tux_;
	isPlcConnected=true;
	dbActiveName=activeStr;
	dbNextName=nextStr;
	craneConveyorWidget=new craneConveyorWidgetClass("conv",this);
	nextFromPos=nextToPos=NULL;
	activeFromPos=activeToPos=NULL;
	nextFromXCorr=nextFromYCorr=nextFromZCorr=nextToXCorr=nextToYCorr=nextToZCorr=0;
	activeMissionStatus=plcStatusIdle;
	activeMissionStep=plcStepIdle;
	craneConveyorWidget->addMessageToGui(QString("..."),QString("Program started"),infoStr);
	missionTimer=new QTimer(this);
	connect(missionTimer,SIGNAL(timeout()),this,SLOT(missionTimerSlot()));
}
craneConveyorClass::~craneConveyorClass()
{
}
void craneConveyorClass::setExcludedPositions(QVector<int> excludedPosV_)
{
	excludedPosV=excludedPosV_;
}
QString craneConveyorClass::getStepText(int s)
{
	return craneConveyorWidget->getStepText(s);
}
QString craneConveyorClass::getStepDescription(int s)
{
	return craneConveyorWidget->getStepDescription(s);
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
		execQuery(QString("update missions set fromPos=0 where conveyorPos='%1'").arg(dbNextName),mDb);
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
		execQuery(QString("update missions set toPos=0 where conveyorPos='%1'").arg(dbNextName),mDb);
	}
}
bool craneConveyorClass::getHasNextMission()
{
	return nextFromPos||nextToPos;
}
void craneConveyorClass::setActiveFromPos(posClass *p)
{
	if ((activeFromPos)&&(p))
	{
		craneConveyorWidget->addMessageToGui(QString("internal error"),QString("setActiveFromPos, p!=null while activeFromPos!=null"),errorStr);
		return;
	}
	if (activeFromPos)
		activeFromPos->setIsActiveFrom(false);
	activeFromPos=p;
	if (activeFromPos)
		activeFromPos->setIsActiveFrom(true);
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
	if (activeToPos)
		activeToPos->setIsActiveTo(false);
	activeToPos=p;
	if (activeToPos)
		activeToPos->setIsActiveTo(true);
	if (activeToPos)
		craneConveyorWidget->setActiveTo(activeToPos->getDescription());
	else
		craneConveyorWidget->setActiveTo("");
}
bool craneConveyorClass::moveNextToActive()
{
	if (getIsPlcConnected())
		return false;
	if ((nextFromPos)&&(nextToPos)&&(!activeFromPos)&&(!activeToPos))
	{
		craneConveyorWidget->addMessageToGui("Making next mission active","Moving data from of next mission to active mission",infoStr);
		execQuery(QString("update missions set fromPos=%1,toPos=%2 where conveyorPos='%3'").arg(nextFromPos->getPosId()).arg(nextToPos->getPosId()).arg(dbActiveName),mDb);
		setActiveFromPos(nextFromPos);
		setActiveToPos(nextToPos);
		setNextFromPos(NULL);
		setNextToPos(NULL);
		return true;
	}
	else if ((nextFromPos)&&(nextToPos)&&(activeFromPos)&&(activeToPos))
	{
		if ( (nextFromPos->getPosId()==activeFromPos->getPosId())  &&  (nextToPos->getPosId()==activeToPos->getPosId())  )
			return true;
		else
			craneConveyorWidget->addMessageToGui("Tried to move next to active","Internal error, active positions is not empty and next not equal to active",errorStr);
		return true;
	}
	else if ((!nextFromPos)&&(!nextToPos)&&(activeFromPos)&&(activeToPos))
	{
		craneConveyorWidget->addMessageToGui("Move next to active","Move next mission to active, mission allready moved",infoStr);
		return true;
	}
	return false;
}
void craneConveyorClass::setPlcMissionTable(QString plcIp_,QString plcMissionTable_,int plcNextMissionFromIndex_,int plcNextMissionToIndex_,int plcNextMissionStatusIndex_,int plcCurrentMissionFromIndex_,int plcCurrentMissionToIndex_,int plcCurrentMissionPcStatusIndex_,int plcCurrentMissionPLcStatusIndex_,int plcCurrentMissionPlcStepIndex_,int plcNextFromXcorrIndex_,int plcNextFromYcorrIndex_,int plcNextFromZcorrIndex_,int plcNextToXcorrIndex_,int plcNextToYcorrIndex_,int plcNextToZcorrIndex_,int plcNextMissionCassetteIndex_)
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
	plcCurrentMissionPlcStepIndex=plcCurrentMissionPlcStepIndex_;
	plcNextFromXcorrIndex=plcNextFromXcorrIndex_;
	plcNextFromYcorrIndex=plcNextFromYcorrIndex_;
	plcNextFromZcorrIndex=plcNextFromZcorrIndex_;
	plcNextToXcorrIndex=plcNextToXcorrIndex_;
	plcNextToYcorrIndex=plcNextToYcorrIndex_;
	plcNextToZcorrIndex=plcNextToZcorrIndex_;
	plcNextMissionCassetteIndex=plcNextMissionCassetteIndex_;
}
void craneConveyorClass::checkForNextMissionInDatabase()
{
	craneConveyorWidget->addMessageToGui(QString("Load from database"),QString("Loading data from database"),infoStr);
	int fromNext=getFirst(QString("select fromPos from missions where conveyorPos='%1'").arg(dbNextName),mDb).toInt();
	int toNext=getFirst(QString("select toPos from missions where conveyorPos='%1'").arg(dbNextName),mDb).toInt();
	if ((fromNext!=0)&&(toNext!=0))
	{
		posClass *f=allPos->getPosWithId(fromNext);
		posClass *t=allPos->getPosWithId(toNext);
		craneConveyorWidget->addMessageToGui(QString("Found next mission"),QString("from %1 to %2").arg(f->getDescription()).arg(t->getDescription()),infoStr);
		missionStart(f,t);
	}
	if ((fromNext==0)&&(toNext==0))
		craneConveyorWidget->addMessageToGui(QString("No next missions"),QString("No next missions found in database"),infoStr);
}
void craneConveyorClass::loadFromDatabase()
{
	int fromNext=getFirst(QString("select fromPos from missions where conveyorPos='%1'").arg(dbNextName),mDb).toInt();
	int toNext=getFirst(QString("select toPos from missions where conveyorPos='%1'").arg(dbNextName),mDb).toInt();
	if ((fromNext!=0)&&(toNext!=0))
	{
		posClass *f=allPos->getPosWithId(fromNext);
		posClass *t=allPos->getPosWithId(toNext);
		setNextFromPos(f);
		setNextToPos(t);
		craneConveyorWidget->addMessageToGui(QString("Found next mission"),QString("from %1 to %2").arg(f->getDescription()).arg(t->getDescription()),infoStr);
	}
	int fromActive=getFirst(QString("select fromPos from missions where conveyorPos='%1'").arg(dbActiveName),mDb).toInt();
	int toActive=getFirst(QString("select toPos from missions where conveyorPos='%1'").arg(dbActiveName),mDb).toInt();
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
	return (!getIsPlcConnected())&&(nextMissionFrom==0)&&(nextMissionTo==0)&&(nextMissionStatus==plcStatusIdle)&&(nextFromPos==NULL)&&(nextToPos==NULL)/*&&(cassetteNum==0)*/;
}
bool craneConveyorClass::getCanStartNewMission(posClass* f,posClass* t)
{
	if (getIsPlcConnected())
		return false;
	if (!isReadyForNewMission())
		return false;
	if ((!f)||(!t))
		return false;
	for (int i=0;i<excludedPosV.count();i++)
	{
		if ((excludedPosV[i]==f->getPosId())||(excludedPosV[i]==t->getPosId()))
		{
			return false;
		}
	}
	if (f->getIsLocked())
	{
		craneConveyorWidget->addMessageToGui(QString("Can not start new mission from:%1").arg(f->getPosId()),"Position is locked",errorStr);
		log(QString("Can not start new mission from:%1, position is locked").arg(f->getPosId()),errorStr);
		return false;
	}
	if (t->getIsLocked())
	{
		craneConveyorWidget->addMessageToGui(QString("Can not start new mission to:%1").arg(t->getPosId()),"Position is locked",errorStr);
		log(QString("Can not start new mission to:%1, position is locked").arg(t->getPosId()),errorStr);
		return false;
	}
	if (!f->getHasCassette())
	{
		craneConveyorWidget->addMessageToGui(QString("Can not start new mission from:%1").arg(f->getPosId()),"Position has no cassette",errorStr);
		log(QString("Can not start new mission from:%1, position is has no cassette").arg(f->getPosId()),errorStr);
		return false;
	}
	if (t->getHasCassette())
	{
		craneConveyorWidget->addMessageToGui(QString("Can not start new mission to:%1").arg(t->getPosId()),"Position has cassette",errorStr);
		log(QString("Can not start new mission to:%1, position has cassette").arg(t->getPosId()),errorStr);
		return false;
	}
	if (!isReadyForNewMission())
	{
		craneConveyorWidget->addMessageToGui(QString("Can not start new mission"),"It is not ready to start a new mission",errorStr);
		log(QString("startNewMission f=%1 t=%2 s=%3").arg(currentMissionFrom).arg(currentMissionTo).arg(currentMissionPLcStatus),errorStr);
		return false;
	}
	QString activConv=getFirst(QString("select conveyorPos from missions where fromPos=%1 or toPos=%1 or fromPos=%2 or toPos=%2  ").arg(f->getPosId()).arg(t->getPosId()),mDb).toString().trimmed();
	if ((activConv!="0")&&(activConv!=dbNextName))
		return false;
	return true;
}
void craneConveyorClass::missionStart(posClass* f,posClass*t)
{
	if (!getCanStartNewMission(f,t))
		return;
	setNextFromPos(f);
	setNextToPos(t);
	logV(QString("startNewMission f=%1 t=%2").arg(nextFromPos->getPosId()).arg(nextToPos->getPosId()),infoStr);
	execQuery(QString("update missions set fromPos=%1,toPos=%2 where conveyorPos='%3'").arg(nextFromPos->getPosId()).arg(nextToPos->getPosId()).arg(dbNextName),mDb);
	craneConveyorWidget->addMessageToGui(QString("New mission %1 to %2").arg(f->getDescription()).arg(t->getDescription()),"Starting new mission",infoStr);
	if ((!nextFromPos)||(!nextToPos))
	{
		log(QString("missionStart ((!fromPos)||(!toPos))"),errorStr);
		return;
	}
	if (nextFromPos->getCassetteNumber()==0)
	{
		log(QString("missionStart fromPos->getCassetteNumber()==0 from:%1").arg(nextFromPos->getPosId()),errorStr);
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
		if (index==plcCurrentMissionPlcStepIndex){activeMissionStep=value.toInt();craneConveyorWidget->setActiveStep(activeMissionStep);}
	}
}
void craneConveyorClass::plcStatusChanged(int s)
{
	if (getIsPlcConnected())
		return;
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
		missionTimer->start(3000);
		if (moveNextToActive())
		{
			if (logMissionsTimes)
				execQuery(QString("insert into missionsTimeLog (fromPos,toPos,startSec,endSec,time) values(%1,%2,%3,0,'%4')").arg(activeFromPos->getPosId()).arg(activeToPos->getPosId()).arg(QDateTime::currentDateTime().toTime_t()).arg(timeForLog),mDb);
			tux->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusStart);
		}
	}
	else if (currentMissionPLcStatus==plcStatusBascketOnConv)
	{
		missionTimer->start(3000);
		missionCassetteOnConveyor();
	}
	else if (  (currentMissionPLcStatus==plcStatusFinished)||(currentMissionPLcStatus==plcStatusUserFinished)   )
	{
		missionTimer->start(1000);
		missionFinished();
	}
	else if (currentMissionPLcStatus==plcStatusAborted)
		missionAborted();
}

void craneConveyorClass::missionSendToPlc(bool force)
{
	if (getIsPlcConnected())
		return;
	if ((!nextFromPos)||(!nextToPos))
	{
		log(QString("sendMissionToPlc ((!fromPos)||(!toPos))"),errorStr);
		return;
	}
	craneConveyorWidget->addMessageToGui(QString("Send to plc %1 to %2").arg(nextFromPos->getPosId()).arg(nextToPos->getPosId()),QString("Sending data to plc"),infoStr);
	if (nextMissionFrom!=nextFromPos->getPosId())
	{
		tux->writeInteger(plcMissionTable,plcNextMissionFromIndex,nextFromPos->getPosId());
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg(plcMissionTable).arg(plcNextMissionFromIndex).arg(nextFromPos->getPosId()),infoStr);
	}
	if (nextMissionTo!=nextToPos->getPosId())
	{
		tux->writeInteger(plcMissionTable,plcNextMissionToIndex,nextToPos->getPosId());
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg(plcMissionTable).arg(plcNextMissionToIndex).arg(nextToPos->getPosId()),infoStr);
	}
	//cassette number
	tux->writeInteger(plcMissionTable,plcNextMissionCassetteIndex,nextFromPos->getCassetteNumber());
	tux->writeInteger(plcMissionTable,plcNextMissionStatusIndex,1);
	logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg(plcMissionTable).arg(plcNextMissionStatusIndex).arg(1),infoStr);
}
int craneConveyorClass::moveCassette(posClass *from,posClass *to)
{
	if (getIsPlcConnected())
		return 0;
	if ((from==NULL)||(to==NULL))
	{
		log(QString("moveCassette ((from==NULL)||(to==NULL))"),errorStr);
		uExit(errorStr,QString("moveCassette ((from==NULL)||(to==NULL))"));
	}
	craneConveyorWidget->addMessageToGui(QString("Move cassette from %1 to %2").arg(from->getPosId()).arg(to->getPosId()),QString("Moving cassette"),infoStr);
	int cassette=0;
	cassette=movePosDataGlobal(from->getPosId(),to->getPosId(),mDb);
	if (from==this)
		triplePosClass::loadFromDatabase();
	else
		from->loadFromDatabase();
	if (to==this)
		triplePosClass::loadFromDatabase();
	else
		to->loadFromDatabase();
	from->sendCassetteSbTicketToPlc();
	to->sendCassetteSbTicketToPlc();
	return cassette;
}
void craneConveyorClass::clearMissionInDb(QString user)
{
	if (getIsPlcConnected())
		return;
	craneConveyorWidget->addMessageToGui(QString("Clear mission"),QString("Mission has ended from %1, clearing data from database").arg(user.length()>0?user:"plc"),infoStr);
	execQuery(QString("update missions set fromPos=0,toPos=0 where conveyorPos='%1'").arg(dbActiveName),mDb);
	setActiveFromPos(NULL);
	setActiveToPos(NULL);
	tux->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusFinished);
}
void craneConveyorClass::missionTimerSlot()
{
	if (getIsPlcConnected())
		return;
	if (currentMissionPLcStatus==plcStatusStart)
	{
		if (moveNextToActive())
			tux->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusStart);
	}
	else if (currentMissionPLcStatus==plcStatusBascketOnConv)
	{
		craneConveyorWidget->addMessageToGui(QString("Plc: mission cassette on conveyor"),QString("Plc: mission cassette on conveyor from plc"),infoStr);
		missionCassetteOnConveyor();
	}
	else if ((currentMissionPLcStatus==plcStatusFinished)||(currentMissionPLcStatus==plcStatusUserFinished))
	{
		craneConveyorWidget->addMessageToGui(QString("Plc: mission finished"),QString("Mission has ended from plc"),infoStr);
		missionFinished();
	}
	else if (currentMissionPLcStatus==plcStatusAborted)
		missionAborted();
}
void craneConveyorClass::missionCassetteOnConveyor()
{
	if (getIsPlcConnected())
		return;
	if ((activeFromPos)&&(activeToPos))
	{
		if (activeFromPos->getHasCassette()&&(!this->getHasCassette()))
		{
			if (moveCassette(activeFromPos,this)!=0)
				craneConveyorWidget->addMessageToGui(QString("Cassette on conveyor"),QString("Plc moved cassette on conveyor, cassette moved in database."),infoStr);
			else
				craneConveyorWidget->addMessageToGui(QString("Cassette on conveyor"),QString("Plc moved cassette on conveyor, could not move cassette in database."),errorStr);
		}
		if ((this->getHasCassette())&&(activeToPos->getHasCassette()))
			craneConveyorWidget->addMessageToGui(QString("Cassette on conveyor"),QString("Plc moved cassette on conveyor and destination is full."),infoStr);
		else if ((this->getHasCassette())&&((!activeToPos->getHasCassette())))
		{
			craneConveyorWidget->addMessageToGui(QString("Cassette on conveyor"),QString("Plc moved cassette on conveyor (pc status:2)."),infoStr);
			tux->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusBascketOnConv);
			logV(QString("missionCassetteOnConveyor"),infoStr);
			missionTimer->stop();
		}
	}
	else
		craneConveyorWidget->addMessageToGui(QString("Cassette on conveyor"),QString("Plc moved cassette on conveyor and pc has no active mission."),errorStr);
}
void craneConveyorClass::missionFinished()
{
	if (getIsPlcConnected())
		return;
	if (activeToPos&&(activeToPos->getCassetteNumber()==0) && (this->getCassetteNumber()!=0))
	{
		if (logMissionsTimes)
			execQuery(QString("update missionsTimeLog set endSec=%1 where fromPos=%2 and toPos=%3 and endSec=0").arg(QDateTime::currentDateTime().toTime_t()).arg(activeFromPos->getPosId()).arg(activeToPos->getPosId()),mDb);
		int movedCassette=moveCassette(this,activeToPos);
		int activeToPosId=activeToPos->getPosId();
		int activeFromPosId=activeFromPos->getPosId();
		posClass *toPos=activeToPos;
		if (movedCassette!=0)
		{
			clearMissionInDb();
			if (currentMissionPLcStatus==plcStatusFinished)
			{
				logV(QString("missionFinished normal"),infoStr);
				tux->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusFinished);
			}
			if (currentMissionPLcStatus==plcStatusUserFinished)
			{
				logV(QString("missionFinished by user"),infoStr);
				tux->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusUserFinished);
			}
			missionTimer->stop();
		}
	}
	else
	{
		if (activeToPos&&(activeToPos->getCassetteNumber()!=0))
			craneConveyorWidget->addMessageToGui(QString("Cassette on conveyor"),QString("Plc moved cassette to destination and pc has allready a cassette there."),errorStr);
		if (this->getCassetteNumber()==0)
			craneConveyorWidget->addMessageToGui(QString("Cassette on conveyor"),QString("Plc moved cassette to destination and pc has no cassette on conveyor."),errorStr);
	}
}
void craneConveyorClass::missionAborted()
{
	if (getIsPlcConnected())
		return;
	craneConveyorWidget->addMessageToGui(QString("Mission was aborted"),QString("Mission was aborted by plc"),infoStr);
	bool canAbort=false;
	if (activeFromPos&&(this->getHasCassette())&&(!activeFromPos->getHasCassette()))
	{
		int movedCassette=movedCassette=moveCassette(this,activeFromPos);
		canAbort=(movedCassette!=0);
	}
	else if (activeFromPos&&activeToPos&&(!this->getHasCassette())&&(activeFromPos->getHasCassette()))
	{
		canAbort=true;
	}
	else if ((!activeFromPos)&&(!activeToPos))
		canAbort=true;
	if (canAbort)
	{
		clearMissionInDb();
		tux->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusAborted);
		logV(QString("missionAborted"),infoStr);
	}
	else
		craneConveyorWidget->addMessageToGui(QString("Mission aborted"),QString("Mission was aborted from plc and there is no cassette on conveyor or the source position."),errorStr);
}
bool craneConveyorClass::getIsPlcConnected()
{
	return isPlcConnected&&tux->getHasError();
}
void craneConveyorClass::setIsPlcConnected(bool v)
{
	isPlcConnected=v;
}

bool craneConveyorClass::isMissionAboutToEnd()
{
	return ((activeMissionStep>=plcStepGoToYUnLoad)||(activeMissionStep==plcStepIdle)||(activeMissionStep==plcStepFinished))
		||
		(activeToPos&&(activeToPos->getPosId()==conveyor__1_id)
			&&((activeMissionStep==plcStepGoToXZUnLoad)||(activeMissionStep==plcStepGoToYUnLoad)||(activeMissionStep==plcStepHooksDown)||(activeMissionStep==plcStepHooksToCenterNoCassette))
			);
}

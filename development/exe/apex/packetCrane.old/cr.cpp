#include "cr.h"
//extern float plcSetXVelocityForOpenGl;
//extern float plcSetYVelocityForOpenGl;
//extern float plcSetZVelocityForOpenGl;
craneClass::craneClass(QWidget *p):QWidget(p)
{
	cranePlcTcpServer=NULL;
	plcXInMeters=plcYInMeters=plcZInMeters=0.0;
	lastPlcXInMeters=lastPlcYInMeters=lastPlcZInMeters=0.0;
	openGlX=openGlY=openGlZ=0.0f;
	plcXVelocity=plcYVelocity=plcZVelocity=0.0f;
	hasActiveMission=false;
	isCarryingCassette=false;
	plcCassetteOnCrane=0;
	fromPos=toPos=NULL;
	horizontalOpenGl=new openGLPartClass(QString("craneBase"),CRANE_BASE_TEXTURE);
	verticalOpenGl=new openGLPartClass(QString("craneCarrier"),CRANE_CARRIER_TEXTURE);
	connect(&animateTimer, SIGNAL(timeout()), this, SLOT(animateTimerSlot()));
	//
	position24=new conveyorPositionClass(POS_ID_WESTCONV,-2.0f,-7.4f,3.2f,this);	connect(position24,SIGNAL(openGlChangedSignal()),this,SLOT(openGlObjectChangedSlot()));
	position25=new conveyorPositionClass(POS_ID_EASTCONV,-2.0f,+7.4f,3.2f,this);  connect(position25,SIGNAL(openGlChangedSignal()),this,SLOT(openGlObjectChangedSlot()));
	positionCrane=new cellPositionClass(POS_ID_CRANE,0,0,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,STORAGE_TYPE_NULL,IS_NOT_LOCKED,0,0,p);connect(positionCrane,SIGNAL(openGlChangedSignal()),this,SLOT(openGlObjectChangedSlot()));
	positionCrane->setOpenGlX(0.0f);positionCrane->setOpenGlY(0.0f);positionCrane->setOpenGlZ(0.0f);
	//connect(&pendingMessagesToPlcTimer, SIGNAL(timeout()), this, SLOT(pendingMessagesToPlcTimerSlot()));
	//
	plcXTime.start();
	plcYTime.start();
	plcZTime.start();
}
craneClass::~craneClass()
{
	delete horizontalOpenGl;
	delete verticalOpenGl;
	delete position24;
	delete positionCrane;
}
conveyorPositionClass *craneClass::getConveyorPosition24()
{
	return position24;
}
conveyorPositionClass *craneClass::getConveyorPosition25()
{
	return position25;
}
cellPositionClass *craneClass::getCranePosition()
{
	return positionCrane;
}

void craneClass::draw()
{
	horizontalOpenGl->draw();
	verticalOpenGl->draw();
	position24->draw();
	position25->draw();
	positionCrane->draw();
}
void craneClass::drawForPicking()
{
	position24->drawForPicking();
	position25->drawForPicking();
	positionCrane->drawForPicking();
}
void craneClass::mouseOver(QColor pickedColor)
{
	position24->mouseOver(pickedColor);
	position25->mouseOver(pickedColor);
	positionCrane->mouseOver(pickedColor);
}
void craneClass::mouseClicked(QColor pickedColor)
{
	position24->mouseClicked(pickedColor);
	position25->mouseClicked(pickedColor);
	positionCrane->mouseClicked(pickedColor);
}
void craneClass::openGlObjectChangedSlot()
{
	emit openGlChangedSignal();
}
//
static float avarage(QVector<float> v)
{
	float ans=0;
	for(int i=0;i<v.count();i++)
		ans+=v[i];
	return ans/v.count();
}
////
static float stepToTarget(float pos,float target,float step)
{
	if (fabs(pos-target)>FLT_EPSILON)
	{
		if (pos<target)
			return ((pos+step)>target)?target:(pos+step);
		else
			return ((pos-step)<target)?target:(pos-step);
	}
	else
		return target;
}
void craneClass::setPlcX(float x)
{
	plcXInMeters=x;
	int dt=plcXTime.elapsed();
	if ((dt>0)&&(dt<2000))
	{
		plcXVelocityArray.append(1.0f*fabs(openGlX-x)/float(dt));
		plcXVelocity=avarage(plcXVelocityArray);
		plcXTime.restart();
		if ((plcXVelocity>0)&&(!animateTimer.isActive()))
			animateTimer.start(20);
	}
	else
	{
		openGlX=plcXInMeters;
		horizontalOpenGl->setOpenGlXpos(openGlX);
		verticalOpenGl->setOpenGlXpos(openGlX);
		positionCrane->setOpenGlX(openGlX);
		plcXTime.restart();
	}
}
void craneClass::setPlcY(float y)
{
	plcYInMeters=y;
}
void craneClass::setPlcZ(float z)
{
	plcZInMeters=z;
	int dt=plcZTime.elapsed();
	if ((dt>0)&&(dt<2000))
	{
		plcZVelocityArray.append(1.0f*fabs(openGlZ-z)/float(dt));
		plcZVelocity=avarage(plcZVelocityArray);
		if ((plcZVelocity>0)&&(!animateTimer.isActive()))
			animateTimer.start(20);
	}
	else
	{
		openGlZ=plcZInMeters;
		verticalOpenGl->setOpenGlZpos(openGlZ);
		positionCrane->setOpenGlZ(openGlZ+1.f);
		plcZTime.restart();
	}
}
float	craneClass::getPlcX()
{
	return plcXInMeters*1000.;
}
float	craneClass::getPlcY()
{
	return plcYInMeters*1000.;
}
float	craneClass::getPlcZ()
{
	return -plcZInMeters*1000.;
}

void craneClass::animateTimerSlot()
{
	float dt=animateTime.elapsed();
	if (plcXVelocity>FLT_EPSILON)
		openGlX=stepToTarget(openGlX,plcXInMeters,plcXVelocity*dt);
	if (plcYVelocity>FLT_EPSILON)
		openGlY=stepToTarget(openGlY,plcYInMeters,plcYVelocity*dt);
	if (plcZVelocity>FLT_EPSILON)
		openGlZ=stepToTarget(openGlZ,plcZInMeters,plcZVelocity*dt);
	animateTime.restart();

	horizontalOpenGl->setOpenGlXpos(openGlX);
	verticalOpenGl->setOpenGlXpos(openGlX);
	verticalOpenGl->setOpenGlZpos(openGlZ);
	positionCrane->setOpenGlX(openGlX+0.5f);
	positionCrane->setOpenGlY(openGlY);
	positionCrane->setOpenGlZ(openGlZ+0.5f);
	if ( (fabs(openGlX-plcXInMeters)<FLT_EPSILON)&&(fabs(openGlY-plcYInMeters)<FLT_EPSILON)&&(fabs(openGlZ-plcZInMeters)<FLT_EPSILON))
	{
		animateTimer.stop();
	}
	emit openGlChangedSignal();
}
void craneClass::setAllPositionsClass(allPosisionsClass *a)
{
	allPosisions=a;
}

void craneClass::setPlcClass(plcClass *plc)
{
	cranePlcTcpServer=plc->getPacketCraneTcpServer();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cellPositionClass *craneClass::getThePositionThatTheActiveCassetteIs()
{
	if (fromPos->hasCassette())
		return fromPos;
	if (positionCrane->hasCassette())
		return positionCrane;
return NULL;
}
bool craneClass::isIdle()
{
	if (fromPos||toPos||hasActiveMission||(plcStatusEnum!=PLC_STATUS_IDLE))
		return false;
	return true;
}
void craneClass::newMission(int fromId,int toId,bool resume,bool manualExportOfCassette)
{
	vLog("info","craneClass::newMission()",QString("fromId:%1,toId:%2,resume:%3,manualExportOfCassette:%4").arg(fromId).arg(toId).arg(resume).arg(manualExportOfCassette));
	newMission(allPosisions->getCellPosWithPosId(fromId),allPosisions->getCellPosWithPosId(toId),resume,manualExportOfCassette);
}
void craneClass::loadFromDatabase()
{
	int fromPosId=GETONE("select value from state where name='craneFromPos             '").toInt();
	int toPosId=GETONE("select value from state where name='craneToPos               '").toInt();
	vLog("info","craneClass::loadFromDatabase()",QString("fromPosId:%1,toPosId:%2").arg(fromPosId).arg(toPosId));
	fromPos=allPosisions->getCellPosWithPosId(fromPosId);
	toPos=allPosisions->getCellPosWithPosId(toPosId);
	if (fromPos&&toPos)
		hasActiveMission=true;
	position24->loadFromDatabase();
	position25->loadFromDatabase();
	positionCrane->loadFromDatabase();
}
void craneClass::newMission(cellPositionClass *from,cellPositionClass *to,bool resume,bool manualExportOfCassette)
{
	if ((!from)||(!to))
		return;
	vLog("info","craneClass::newMission()",QString("fromId:%1,toId:%2,resume:%3,manualExportOfCassette:%4").arg(from->getPosId()).arg(to->getPosId()).arg(resume).arg(manualExportOfCassette));
	if (  (fromPos||toPos||hasActiveMission||(plcStatusEnum!=PLC_STATUS_IDLE))  &&  (!resume)  )
	{
		int f=0;if(fromPos)f=fromPos->getPosId();
		int t=0;if(toPos)t=toPos->getPosId();
		vLog("error","craneClass::newMission()",tr("Crane is already with a mission from:%1 to:%2 hasActiveMission:%3 plcStatusEnum:%4").arg(f).arg(t).arg(hasActiveMission?"true":"false").arg(plcStatusEnum));
		return;
		EXIT(tr("Internal error"),tr("Crane is already with a mission from:%1 to:%2 hasActiveMission:%3 plcStatusEnum:%4").arg(f).arg(t).arg(hasActiveMission?"true":"false").arg(plcStatusEnum));
	}
	fromPos=from;
	toPos=to;
	hasActiveMission=true;
	if (!resume)
	{
		apexExecQuery(QString("update state set value=%1 where name='craneFromPos             '").arg(fromPos->getPosId()));
		apexExecQuery(QString("update state set value=%1 where name='craneToPos               '").arg(toPos->getPosId()));
		apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('crane mission','start',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	}
	else
		apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('crane mission','resume',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	if (!manualExportOfCassette&&(toPos->getPosId()==POS_ID_WESTCONV)&&(getThePositionThatTheActiveCassetteIs()->hasPacket()))position24->setActivePacketExport(true);
	if (!manualExportOfCassette&&(toPos->getPosId()==POS_ID_EASTCONV)&&(getThePositionThatTheActiveCassetteIs()->hasPacket()))position25->setActivePacketExport(true);

	vLog("info","craneClass::newMission()",QString("sendString N150[0]=%1").arg(fromPos->getPositionIdForCranePlc()));
	cranePlcTcpServer->sendString(QString("N150|0|%1").arg(fromPos->getPositionIdForCranePlc()));
	vLog("info","craneClass::newMission()",QString("sendString N150[1]=%1").arg(toPos->getPositionIdForCranePlc()));
	cranePlcTcpServer->sendString(QString("N150|1|%1").arg(toPos->getPositionIdForCranePlc()));
	if (plcStatusEnum==PLC_STATUS_IDLE)
	{
		vLog("info","craneClass::newMission()",QString("plcStatusEnum==PLC_STATUS_IDLE sendString N150[3]=0"));
		cranePlcTcpServer->sendString(QString("N150|3|0"));
	}
}
void craneClass::plcStatusChanged(int status)
{
	plcStatusEnum=status;
	//if ((plcStatusEnum!=PLC_STATUS_EXECUTING)&&(plcStatusEnum!=PLC_STATUS_FINISHED_OK)&&(hasActiveMission))
	//	EXIT(tr("Error"),tr("Crane pls is with status %1 and pc has not an active mission. Please correct this and restart the software.").arg(plcStatusEnum));
	vLog("info","craneClass::plcStatusChanged()",QString("plc status:%1").arg(plcStatusEnum));
	if (plcStatusEnum==PLC_STATUS_IDLE)
	{
		if (hasActiveMission&&fromPos&&toPos)
			newMission(fromPos,toPos,true,false);
	}
	else if (plcStatusEnum==PLC_STATUS_EXECUTING)
	{
		vLog("info","craneClass::plcStatusChanged()",QString("sendString N150[3]=1"));
		cranePlcTcpServer->sendString(QString("N150|3|1"));
	}
	else if (plcStatusEnum==PLC_STATUS_FINISHED_OK)
	{
		if (toPos)vLog("info","craneClass::plcStatusChanged()",QString("toPos:%1").arg(toPos->getPosId()));
		if (fromPos)vLog("info","craneClass::plcStatusChanged()",QString("fromPos:%1").arg(fromPos->getPosId()));
		vLog("info","craneClass::plcStatusChanged()",QString("hasActiveMission:%1").arg(hasActiveMission?"true":"false"));
		if ((toPos)&&(fromPos)&&hasActiveMission)
		{
			if (fromPos->getPosId()==POS_ID_WESTCONV){position24->setActivePacketImport(false);position24->setActiveCassetteImport(false);}
			if (fromPos->getPosId()==POS_ID_EASTCONV){position25->setActivePacketImport(false);position25->setActiveCassetteImport(false);}
			transferCassette(getThePositionThatTheActiveCassetteIs(),toPos);
			//exports table update
			if ((toPos->getPosId()==POS_ID_EASTCONV)||(toPos->getPosId()==POS_ID_WESTCONV))
				apexExecQuery(QString("update exports set status='1' where contents='%1' and type='%2'").arg(toPos->getPacketId()).arg((toPos->getIsOnSB()?PACKET_WITH_SB:PACKET_WITHOUT_SB)));
			//check that db is ok
			int fromPosId=GETONE("select value from state where name='craneFromPos             '").toInt();
			int toPosId=GETONE("select value from state where name='craneToPos               '").toInt();
			int fromPosCassette=GETONE(QString("select cassette from positions where posId=%1").arg(fromPosId)).toInt();
			int toPosCassette=GETONE(QString("select cassette from positions where posId=%1").arg(toPosId)).toInt();
			if ((fromPosCassette==0)&&(toPosCassette!=0))
			{
				vLog("info","craneClass::plcStatusChanged()","(fromPosCassette==0)&&(toPosCassette!=0)");
				apexExecQuery(QString("update state set value='0' where name='craneFromPos             ' or name='craneToPos               '"));
				apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('crane mission','ended',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
				toPos=fromPos=NULL;
				hasActiveMission=false;
			}
		}
		vLog("info","craneClass::plcStatusChanged()",QString("sendString N150[3]=2"));
		cranePlcTcpServer->sendString(QString("N150|3|2"));
	}
}

//void craneClass::resumeMission(int fromId,int toId)
//{
//	return resumeMission(allPosisions->getCellPosWithPosId(fromId),allPosisions->getCellPosWithPosId(toId));
//}
//void craneClass::resumeMission(cellPositionClass *from,cellPositionClass *to)
//{
//	if (fromPos||toPos||hasActiveMission)
//		EXIT(tr("Internal error"),tr("Crane is already with a mission"));
//	fromPos=from;
//	toPos=to;
//	hasActiveMission=true;
//	if ((toPos->getPosId()==POS_ID_WESTCONV)&&(fromPos->hasPacket()))position24->setActivePacketExport(true);
//	if ((toPos->getPosId()==POS_ID_EASTCONV)&&(fromPos->hasPacket()))position25->setActivePacketExport(true);
//	apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('crane mission','resume',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
//}

void craneClass::returnNoNeededCassette()
{
	if (isIdle())
	{
		vLog("info","craneClass::returnNoNeededCassette:","");
		if (position24->hasIdleCassette())
			newMission(position24->getPosId(),position24->getCassette(),false,false);
		else if (position25->hasIdleCassette())
			newMission(position25->getPosId(),position25->getCassette(),false,false);
	}
}
void craneClass::takeCassetteOutForPacketIn(packetPositionClass *p)
{
	if (isIdle())
	{
		vLog("info","craneClass::takeCassetteOutForPacketIn:",QString("pos:%1,packet:%2").arg(p->getPosId()).arg(p->getPacketId()));
		if (!position24->hasCassette())
			newMission(allPosisions->getEmptyCassetteForPacketIn(p),position24,false,false);
		else if (!position25->hasCassette())
			newMission(allPosisions->getEmptyCassetteForPacketIn(p),position25,false,false);
		else
			returnNoNeededCassette();
	}
}

void craneClass::plcCassetteOnCraneChanged(int c)
{
	vLog("info","craneClass::plcCassetteOnCraneChanged",QString().number(c));
	plcCassetteOnCrane=c;
	if (plcCassetteOnCrane&&toPos&&fromPos)
	{
		vLog("info","plcCassetteOnCrane&&toPos&&fromPos","true");
		if (plcCassetteOnCrane==fromPos->getCassette())
		{
			vLog("info","plcCassetteOnCrane==fromPos->getCassette()","true");
			if (fromPos->getPosId()==POS_ID_WESTCONV)
			{
				vLog("info","fromPos->getPosId()==POS_ID_WESTCONV","true");
				position24->setActivePacketImport(false);
				position24->setActiveCassetteImport(false);
			}
			if (fromPos->getPosId()==POS_ID_EASTCONV)
			{
				vLog("info","fromPos->getPosId()==POS_ID_EASTCONV","true");
				position25->setActivePacketImport(false);
				position25->setActiveCassetteImport(false);
			}
			transferCassette(fromPos,positionCrane);
		}
	}
}
void craneClass::plcCraneIsCommingOutWithCassette()
{
	vLog("info","craneClass::plcCraneIsCommingOutWithCassette()","");
	if (toPos&&(toPos->getPosId()==POS_ID_WESTCONV))
	{
		vLog("info","craneClass::plcCraneIsCommingOutWithCassette()","toPos&&(toPos->getPosId()==POS_ID_WESTCONV");
		if ((!position25->isActive())&&(position25->hasCassette()))
		{
			vLog("info","craneClass::plcCraneIsCommingOutWithCassette()","(!position25->isActive())&&(position25->hasCassette())");
			position25->setActiveCassetteImport(true);//lockForConcurrentPass
			vLog("info","craneClass::plcCraneIsCommingOutWithCassette()",QString("sendString N150[18]:%1").arg(position25->getCassette()));
			cranePlcTcpServer->sendString(QString("N150|18|%1").arg(position25->getCassette()));
		}
	}
	if (toPos&&(toPos->getPosId()==POS_ID_EASTCONV))
	{
		vLog("info","craneClass::plcCraneIsCommingOutWithCassette()","toPos&&(toPos->getPosId()==POS_ID_EASTCONV)");
		if ((!position24->isActive())&&(position24->hasCassette()))
		{
			vLog("info","craneClass::plcCraneIsCommingOutWithCassette()","(!position24->isActive())&&(position24->hasCassette())");
			position24->setActiveCassetteImport(true);//lockForConcurrentPass
			vLog("info","craneClass::plcCraneIsCommingOutWithCassette()",QString("sendString N150[18]:%1").arg(position24->getCassette()));
			cranePlcTcpServer->sendString(QString("N150|17|%1").arg(position24->getCassette()));
		}
	}
}
void craneClass::transferCassette(cellPositionClass *fPos,cellPositionClass *tPos)
{
	if (fPos)vLog("info","craneClass::transferCassette()",QString("fPos:%1").arg(fPos->getPosId()));
	if (tPos)vLog("info","craneClass::transferCassette()",QString("tPos:%1").arg(tPos->getPosId()));
	if ((!fPos)||(!tPos))
	{
		vLog("error","craneClass::transferCassette()","transferCassette fPos==NUL or tPos==NULL");
		EXIT("Internal error","transferCassette fPos==NUL or tPos==NULL");
	}
	int cass=fPos->getCassette();
	int packet=fPos->getPacketId();
	vLog("info","craneClass::transferCassette()",QString("%1->%2").arg(fPos->getPosId()).arg(tPos->getPosId()));
	vLog("info","craneClass::transferCassette()",QString("fPos:%1").arg(fPos->getPosId()));
	vLog("info","craneClass::transferCassette()",QString("fPos packet:%1").arg(fPos->getPacketId()));
	vLog("info","craneClass::transferCassette()",QString("tPos:%1").arg(tPos->getPosId()));
	vLog("info","craneClass::transferCassette()",QString("tPos packet:%1").arg(tPos->getPacketId()));
	if ((cass==0)||(tPos->getPacketId()!=0))
	{
		vLog("error","craneClass::transferCassette()","transferCassette fPos==NUL or tPos==NULL");
		EXIT("Internal error","transferCassette fPos->getCassette()==0 or tPos->getPacketId()!=0");
	}
	fPos->setCassette(0);
	tPos->setCassette(cass);
	tPos->setPacketId(packet);
}

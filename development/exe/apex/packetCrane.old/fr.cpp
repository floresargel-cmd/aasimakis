#include "fr.h"
forkClass::forkClass(QWidget *p):QWidget(p)
{
	forkPlcTcpServer=NULL;
	plcXInMeters=plcYInMeters=plcZInMeters=0.0;
	lastPlcXInMeters=lastPlcYInMeters=lastPlcZInMeters=0.0;
	openGlX=openGlY=openGlZ=0.0f;
	plcXVelocity=plcYVelocity=plcZVelocity=0.0f;
	hasActiveMission=false;fromPos=toPos=NULL;
	isCarryingPacket=false;
	position24=position25=NULL;
	plcStatusEnum=-1;
	fromPos=toPos=NULL;
	longStrockOpenGl=new openGLPartClass(QString("forkLongStrock"),CRANE_BASE_TEXTURE,-1.5f,-70.53f,7.040f);
	shortStrockOpenGl=new openGLPartClass(QString("forkTransversStrock"),CRANE_CARRIER_TEXTURE,-3.0f,-70.53f,7.040f);
	verticalOpenGl=new openGLPartClass(QString("forkVerticalStrock"),CRANE_CARRIER_TEXTURE,-2.5f,-70.53f,7.040f);
	connect(&animateTimer, SIGNAL(timeout()), this, SLOT(animateTimerSlot()));
	//connect(&pendingMessagesToPlcTimer, SIGNAL(timeout()), this, SLOT(pendingMessagesToPlcTimerSlot()));
	//
	positionSb=new newPacketPositionClass(POS_ID_NEWSB,-1.2f,-70.0f,4.0f,this);		connect(positionSb,SIGNAL(openGlChangedSignal()),this,SLOT(openGlObjectChangedSlot()));
	positionPa=new newPacketPositionClass(POS_ID_NEWPACKET,-1.2f,-60.0f,4.0f,this);		connect(positionPa,SIGNAL(openGlChangedSignal()),this,SLOT(openGlObjectChangedSlot()));
	positionIm=new newPacketPositionClass(POS_ID_SB_PACKET_IMPORT,-1.2f,-51.0f,4.0f,this);		connect(positionIm,SIGNAL(openGlChangedSignal()),this,SLOT(openGlObjectChangedSlot()));
	positionForkLift=new forkPositionClass(this);													connect(positionForkLift,SIGNAL(openGlChangedSignal()),this,SLOT(openGlObjectChangedSlot()));

	//	
	plcXTime.start();
	plcYTime.start();
	plcZTime.start();
}
forkClass::~forkClass()
{
	delete longStrockOpenGl;
	delete shortStrockOpenGl;
	delete verticalOpenGl;
	delete positionSb;
	delete positionPa;
	delete positionIm;
	delete positionForkLift;
}
newPacketPositionClass* forkClass::getPositionSb()
{
	return positionSb;
}
newPacketPositionClass* forkClass::getPositionPa()
{
	return positionPa;
}
newPacketPositionClass* forkClass::getPositionIm()
{
	return positionIm;
}
void forkClass::setPosition24(conveyorPositionClass *p)
{
	position24=p;
}
void forkClass::setPosition25(conveyorPositionClass *p)
{
	position25=p;
}
forkPositionClass* forkClass::getForkPosition()
{
	return positionForkLift;
}

void forkClass::draw()
{
	longStrockOpenGl->draw();
	shortStrockOpenGl->draw();
	verticalOpenGl->draw();
	positionSb->draw();
	positionPa->draw();
	positionIm->draw();
	positionForkLift->draw();
}
void forkClass::drawForPicking()
{
	positionSb->drawForPicking();
	positionPa->drawForPicking();
	positionIm->drawForPicking();
	positionForkLift->drawForPicking();
}
void forkClass::mouseOver(QColor pickedColor)
{
	positionSb->mouseOver(pickedColor);
	positionPa->mouseOver(pickedColor);
	positionIm->mouseOver(pickedColor);
	positionForkLift->mouseOver(pickedColor);
}
void forkClass::mouseClicked(QColor pickedColor)
{
	positionSb->mouseClicked(pickedColor);
	positionPa->mouseClicked(pickedColor);
	positionIm->mouseClicked(pickedColor);
	positionForkLift->mouseClicked(pickedColor);
}
void forkClass::openGlObjectChangedSlot()
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
void forkClass::setPlcX(float x)
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
		shortStrockOpenGl->setOpenGlXpos(openGlX);
		verticalOpenGl->setOpenGlXpos(openGlX);
		positionForkLift->setOpenGlX(openGlX);
		plcXTime.restart();
	}
}
void forkClass::setPlcY(float y)
{
	plcYInMeters=y;
	int dt=plcYTime.elapsed();
	if ((dt>0)&&(dt<2000))
	{
		plcYVelocityArray.append(1.0f*fabs(openGlY-y)/float(dt));
		plcYVelocity=avarage(plcYVelocityArray);
		plcYTime.restart();
		if ((plcYVelocity>0)&&(!animateTimer.isActive()))
			animateTimer.start(20);
	}
	else
	{
		openGlY=plcYInMeters;
		longStrockOpenGl->setOpenGlYpos(openGlY);
		shortStrockOpenGl->setOpenGlYpos(openGlY);
		verticalOpenGl->setOpenGlYpos(openGlY);
		positionForkLift->setOpenGlY(openGlY);
		plcYTime.restart();
	}
}
void forkClass::setPlcZ(float z)
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
		positionForkLift->setOpenGlZ(openGlZ);
		plcZTime.restart();
	}
}
float	forkClass::getPlcX()
{
	return plcXInMeters*1000.;
}
float	forkClass::getPlcY()
{
	return plcYInMeters*1000.;
}
float	forkClass::getPlcZ()
{
	return -plcZInMeters*1000.;
}	
void forkClass::animateTimerSlot()
{
	float dt=animateTime.elapsed();
	if (plcXVelocity>FLT_EPSILON)
		openGlX=stepToTarget(openGlX,plcXInMeters,plcXVelocity*dt);
	if (plcYVelocity>FLT_EPSILON)
		openGlY=stepToTarget(openGlY,plcYInMeters,plcYVelocity*dt);
	if (plcZVelocity>FLT_EPSILON)
		openGlZ=stepToTarget(openGlZ,plcZInMeters,plcZVelocity*dt);
	animateTime.restart();

	shortStrockOpenGl->setOpenGlXpos(openGlX);
	verticalOpenGl->setOpenGlXpos(openGlX);

	longStrockOpenGl->setOpenGlYpos(openGlY);
	shortStrockOpenGl->setOpenGlYpos(openGlY);
	verticalOpenGl->setOpenGlYpos(openGlY);

	verticalOpenGl->setOpenGlZpos(openGlZ);

	positionForkLift->setOpenGlX(openGlX);
	positionForkLift->setOpenGlY(openGlY+0.8f);
	positionForkLift->setOpenGlZ(openGlZ);

	if ( (fabs(openGlX-plcXInMeters)<FLT_EPSILON)&&(fabs(openGlY-plcYInMeters)<FLT_EPSILON)&&(fabs(openGlZ-plcZInMeters)<FLT_EPSILON))
	{
		animateTimer.stop();
	}
	emit openGlChangedSignal();
}

void forkClass::setPlcClass(plcClass *plc)
{
	forkPlcTcpServer=plc->getForkLiftTcpServer();
}
void forkClass::setPositionSbExit(packetPositionClass *p)
{
	positionSbExit=p;
}
void forkClass::setPositionPacketExit(packetPositionClass *p)
{
	positionPacketExit=p;
}
void forkClass::setPositionStoreExit(packetPositionClass *p)
{
	positionStoreExit=p;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool forkClass::isIdle()
{
	if (fromPos||toPos||hasActiveMission||(plcStatusEnum!=PLC_STATUS_IDLE))
		return false;
	return true;
}
bool forkClass::isPosActiveTo(int posId)
{
	if (toPos&&toPos->getPosId()==posId)
		return true;
	return false;
}
packetPositionClass *forkClass::getThePositionThatTheActivePacketIs()
{
	if (fromPos->hasPacket())
		return fromPos;
	if (positionForkLift->hasPacket())
		return positionForkLift;
return NULL;
}
void forkClass::loadFromDatabase()
{
	int fromPosId=GETONE("select value from state where name='forkFromPos'").toInt();
	int toPosId=GETONE("select value from state where name='forkToPos'").toInt();
	if (fromPosId==POS_ID_NEWSB)fromPos=positionSb;
	else if (fromPosId==POS_ID_NEWPACKET)fromPos=positionPa;
	else if (fromPosId==POS_ID_SB_PACKET_IMPORT)fromPos=positionIm;
	else if (fromPosId==POS_ID_WESTCONV)fromPos=position24;
	else if (fromPosId==POS_ID_EASTCONV)fromPos=position25;
	if (fromPos)
	{
		if (toPosId==POS_ID_SBEXIT)toPos=positionSbExit;
		else if (toPosId==POS_ID_PACKETEXIT)toPos=positionPacketExit;
		else if (toPosId==POS_ID_WESTCONV)toPos=position24;
		else if (toPosId==POS_ID_EASTCONV)toPos=position25;
		else if (toPosId==POS_ID_STOREEXIT)toPos=positionStoreExit;
	}
	if (fromPos&&toPos)
		hasActiveMission=true;
	positionSb->loadFromDatabase();
	positionPa->loadFromDatabase();
	positionIm->loadFromDatabase();
	positionForkLift->loadFromDatabase();
}
void forkClass::newMission(int f,int t)
{
	if (fromPos||toPos)
		return;
	packetPositionClass *fPos=NULL;
	switch (f)
	{
		case 6:
			fPos=positionSb;break;
		case 8:
			fPos=positionPa;break;
		case 9:
			fPos=positionIm;break;
		case 24:
			fPos=position24;break;
		case 25:
			fPos=position25;break;
		default:
			break;
	}
	packetPositionClass *tPos=NULL;
	switch (t)
	{
		case 6:
			tPos=positionSb;break;
		case 8:
			tPos=positionPa;break;
		case 21:
			tPos=positionSbExit;break;
		case 22:
			tPos=positionPacketExit;break;
		case 23:
			tPos=positionStoreExit;break;
		case 24:
			tPos=position24;break;
		case 25:
			tPos=position25;break;
		default:
			break;
	}
	if (fPos&&tPos)
		newMission(fPos,tPos,false);
}
void forkClass::newMission(packetPositionClass *from,packetPositionClass *to,bool resume)
{
	if (  (fromPos||toPos||hasActiveMission||(plcStatusEnum!=PLC_STATUS_IDLE))  &&  (!resume)  )
	{
		int f=0;if(fromPos)f=fromPos->getPosId();
		int t=0;if(toPos)t=toPos->getPosId();
		vLog("info","forkClass::newMission()",QString("Fork is already with a mission from:%1 to:%2 hasActiveMission:%3 plcStatusEnum:%4").arg(f).arg(t).arg(hasActiveMission?"true":"false").arg(plcStatusEnum));
		return;
		EXIT(tr("Internal error"),tr("Fork is already with a mission from:%1 to:%2 hasActiveMission:%3 plcStatusEnum:%4").arg(f).arg(t).arg(hasActiveMission?"true":"false").arg(plcStatusEnum));
	}
	fromPos=from;
	toPos=to;
	hasActiveMission=true;
	if (!resume)
	{
		apexExecQuery(QString("update state set value=%1 where name='forkFromPos              '").arg(fromPos->getPosId()));
		apexExecQuery(QString("update state set value=%1 where name='forkToPos                '").arg(toPos->getPosId()));
		apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('fork mission','start',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	}
	else
		apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('fork mission','resume',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	if (toPos->getPosId()==POS_ID_WESTCONV)position24->setActivePacketImport(true);
	if (toPos->getPosId()==POS_ID_EASTCONV)position25->setActivePacketImport(true);
	vLog("info","forkClass::newMission()",QString("sendString N160[0]=%1").arg(fromPos->getPositionIdForForkPlc()));
	forkPlcTcpServer->sendString(QString("N160|0|%1").arg(fromPos->getPositionIdForForkPlc()));
	vLog("info","forkClass::newMission()",QString("sendString N160[1]=%1").arg(toPos->getPositionIdForForkPlc()));
	forkPlcTcpServer->sendString(QString("N160|1|%1").arg(toPos->getPositionIdForForkPlc()));
	//packetPlcType
	int packetPlcType=0;
	if (fromPos->hasPacket())
		packetPlcType=fromPos->getPacketTypeForPlc();
	else
	{
		int craneToPos=GETONE("select value from state where name='craneToPos'").toInt();
		if (craneToPos==fromPos->getPosId())
		{
			int craneFromPosId=GETONE("select value from state where name='craneFromPos             '").toInt();
			int packetId=GETONEATMOST(QString("select packetId from positions where cassette=%1").arg(craneFromPosId)).toInt();
			bool isOnSb=GETONE(QString("select sb from packets where packetId=%1").arg(packetId)).toInt()==PACKET_WITHOUT_SB?false:true;
			if (isOnSb)
				packetPlcType=9;
			else
			{
				int length=GETONE(QString("select length from packets where packetId=%1").arg(packetId)).toInt();
				if (length>2743)
					packetPlcType=GETONE(QString("select woods from packets where packetId=%1").arg(packetId)).toInt();
				else
					packetPlcType=1;	
			}
		}
	}
	vLog("info","forkClass::newMission()",QString("sendString N160[4]=%1").arg(packetPlcType));
	forkPlcTcpServer->sendString(QString("N160|4|%1").arg(packetPlcType));
	if (plcStatusEnum==PLC_STATUS_IDLE)
	{
		vLog("info","forkClass::newMission()",QString("sendString N160[3]=0"));
		forkPlcTcpServer->sendString(QString("N160|3|0"));
	}
}
void forkClass::plcStatusChanged(int status)
{
	plcStatusEnum=status;
	vLog("info","forkClass::plcStatusChanged()",QString("status:%1").arg(plcStatusEnum));
	//if ((plcStatusEnum!=PLC_STATUS_EXECUTING)&&(plcStatusEnum!=PLC_STATUS_WITH_THE_PACKET)&&(plcStatusEnum!=PLC_STATUS_FINISHED_OK)&&(plcStatusEnum!=PLC_STATUS_FINISHED_ABORD)&&(hasActiveMission))
	//	EXIT(tr("Error"),tr("Fork pls is with status %1 and pc has not an active mission. Please correct this and restart the software.").arg(plcStatusEnum));
	if (plcStatusEnum==PLC_STATUS_IDLE)
	{
		vLog("info","forkClass::plcStatusChanged()","plcStatusEnum==PLC_STATUS_IDLE");
		if (hasActiveMission&&fromPos&&toPos)
			newMission(fromPos,toPos,true);
	}
	else if (plcStatusEnum==PLC_STATUS_EXECUTING)
	{
		vLog("info","forkClass::plcStatusChanged()",QString("plcStatusEnum==PLC_STATUS_EXECUTING:sendString N160[3]=1"));
		forkPlcTcpServer->sendString(QString("N160|3|1"));
	}
	else if (plcStatusEnum==PLC_STATUS_WITH_THE_PACKET)
	{
		vLog("info","forkClass::plcStatusChanged()",QString("plcStatusEnum==PLC_STATUS_WITH_THE_PACKET"));
		if (fromPos&&toPos&&fromPos->hasPacket()&&(!positionForkLift->hasPacket()))
		{
			transferPacket(fromPos,positionForkLift);
			if (fromPos->getPosId()==POS_ID_WESTCONV)position24->setActivePacketExport(false);
			if (fromPos->getPosId()==POS_ID_EASTCONV)position25->setActivePacketExport(false);
			vLog("info","forkClass::plcStatusChanged()",QString("sendString N160[3]=2"));
			forkPlcTcpServer->sendString(QString("N160|3|2"));
		}
		else
		{
			vLog("warning","forkClass::newMission()",QString("!  (fromPos&&toPos&&fromPos->hasPacket()&&(!positionForkLift->hasPacket()))"));
			if (!fromPos)vLog("warning","forkClass::plcStatusChanged()",QString("fromPos==NULL"));
			if (!toPos)vLog("warning","forkClass::plcStatusChanged()",QString("toPos==NULL"));
			if (positionForkLift->hasPacket())vLog("warning","forkClass::plcStatusChanged()",QString("positionForkLift packet:%1").arg(positionForkLift->getPacketId()));
			double lala=0.;
		}
	}
	else if (plcStatusEnum==PLC_STATUS_FINISHED_OK)
	{
		vLog("info","forkClass::plcStatusChanged()",QString("plcStatusEnum==PLC_STATUS_FINISHED_OK"));
		if (fromPos&&toPos&&   (fromPos->hasPacket()||positionForkLift->hasPacket())   &&   (!toPos->hasPacket())  )
		{
			bool missedPlcStatus2=false;
			//make the transfer
			if (positionForkLift->hasPacket())
				transferPacket(positionForkLift,toPos);
			else//the plc did not go to intermidiate status, went straight to mission finished status
			{
				missedPlcStatus2=true;
				transferPacket(fromPos,toPos);
				if (fromPos->getPosId()==POS_ID_WESTCONV)position24->setActivePacketExport(false);
				if (fromPos->getPosId()==POS_ID_EASTCONV)position25->setActivePacketExport(false);
			}
			//clear conveyors status
			if (toPos->getPosId()==POS_ID_WESTCONV)  {position24->setActivePacketImport(false);position24->setActiveCassetteImport(true);}
			if (toPos->getPosId()==POS_ID_EASTCONV)  {position25->setActivePacketImport(false);position25->setActiveCassetteImport(true);}
			//check that db is ok
			int toPosId=GETONE("select value from state where name='forkToPos                '").toInt();	
			int toPosPacket=GETONE(QString("select packetId from positions where posId=%1").arg(toPosId)).toInt();

			int fromPacket=0;
			if (!missedPlcStatus2)
				fromPacket=GETONE(QString("select packetId from positions where posId=%1").arg(POS_ID_FORK)).toInt();
			else
				fromPacket=GETONE(QString("select packetId from positions where posId=%1").arg(fromPos->getPosId())).toInt();
			vLog("info","forkClass::plcStatusChanged()",QString("fromPacket=%1 toPosPacket:%2").arg(fromPacket).arg(toPosPacket));
			if ((fromPacket==0)&&(toPosPacket!=0))//if the packets were transfered clear the state table and notify plc (close my mission and plc's mission)
			{
				apexExecQuery(QString("update state set value='0' where name='forkFromPos              ' or name='forkToPos                '"));
				apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('fork mission','ended',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
				toPos=fromPos=NULL;
				hasActiveMission=false;
				vLog("info","forkClass::plcStatusChanged()",QString("sendString N160[3]=3"));
				forkPlcTcpServer->sendString(QString("N160|3|3"));
			}
			else
			{
				vLog("error","forkClass::newMission()",QString("!  (fromPos&&toPos&&(fromPos->hasPacket()||positionForkLift->hasPacket())&&(!toPos->hasPacket()))"));
				if (!fromPos)vLog("error","forkClass::plcStatusChanged()",QString("fromPos==NULL"));
				if (!toPos)vLog("error","forkClass::plcStatusChanged()",QString("toPos==NULL"));
				if (!(fromPos->hasPacket()||positionForkLift->hasPacket()))vLog("error","forkClass::plcStatusChanged()",QString("! (fromPos->hasPacket()||positionForkLift->hasPacket())"));
				if (toPos->hasPacket())vLog("error","forkClass::plcStatusChanged()",QString("toPos packetId:").arg(toPos->getPacketId()));
				EXIT(tr("Internal error"),tr("plcStatusEnum==PLC_STATUS_FINISHED_OK and packets not transfered!"));
			}
		}
		//if (fromPos&&toPos&&positionForkLift->hasPacket()&&(!toPos->hasPacket()))
		//{
		//	transferPacket(positionForkLift,toPos);
		//	if (toPos->getPosId()==POS_ID_WESTCONV)  {position24->setActivePacketImport(false);position24->setActiveCassetteImport(true);}
		//	if (toPos->getPosId()==POS_ID_EASTCONV)  {position25->setActivePacketImport(false);position25->setActiveCassetteImport(true);}
		//	//check that db is ok
		//	int toPosId=GETONE("select value from state where name='forkToPos                '").toInt();	
		//	int forkPacket=GETONE(QString("select packetId from positions where posId=%1").arg(POS_ID_FORK)).toInt();
		//	int toPosPacket=GETONE(QString("select packetId from positions where posId=%1").arg(toPosId)).toInt();
		//	if ((forkPacket==0)&&(toPosPacket!=0))
		//	{
		//		apexExecQuery(QString("update state set value='0' where name='forkFromPos              ' or name='forkToPos                '"));
		//		apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('fork mission','ended',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		//		toPos=fromPos=NULL;
		//		hasActiveMission=false;
		//		forkPlcTcpServer->sendString(QString("N160|3|3"));
		//	}
		//}
	}
	else if (plcStatusEnum==PLC_STATUS_FINISHED_ABORD)
	{
		vLog("info","forkClass::plcStatusChanged()",QString("plcStatusEnum==PLC_STATUS_FINISHED_ABORD"));
		if ((toPos)&&(fromPos))
		{
			vLog("info","forkClass::plcStatusChanged()",QString("(toPos)&&(fromPos)"));
			if (fromPos->getPosId()==POS_ID_WESTCONV)position24->setActivePacketExport(false);
			if (fromPos->getPosId()==POS_ID_EASTCONV)position25->setActivePacketExport(false);
			if (toPos->getPosId()==POS_ID_WESTCONV)  position24->setActivePacketImport(false);
			if (toPos->getPosId()==POS_ID_EASTCONV)  position25->setActivePacketImport(false);
			if ((fromPos->getPosId()==POS_ID_WESTCONV)||(fromPos->getPosId()==POS_ID_EASTCONV))
			{
				vLog("info","forkClass::plcStatusChanged()",QString("(fromPos->getPosId()==POS_ID_WESTCONV)||(fromPos->getPosId()==POS_ID_EASTCONV)"));
				if (positionForkLift->hasPacket())
				{
					vLog("info","forkClass::plcStatusChanged()",QString("positionForkLift->hasPacket()"));
					fromPos->setPacketId(positionForkLift->getPacketId());
				}
				positionForkLift->setPacketId(0);
				toPos->setPacketId(0);
			}
			else
			{
				fromPos->setPacketId(0);
				toPos->setPacketId(0);
				positionForkLift->setPacketId(0);
			}
			apexExecQuery(QString("update state set value='0' where name='forkFromPos              ' or name='forkToPos                '"));
			apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('fork mission','aborded',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
			toPos=fromPos=NULL;
			hasActiveMission=false;
		}
		vLog("info","forkClass::plcStatusChanged()",QString("sendString N160[3]=3"));
		forkPlcTcpServer->sendString(QString("N160|3|3"));
	}
}

//void forkClass::resumeMission(packetPositionClass *from,packetPositionClass *to)
//{
//	if (fromPos||toPos||hasActiveMission)
//		EXIT(tr("Internal error"),tr("Fork is already with a mission"));
//	fromPos=from;
//	toPos=to;
//	hasActiveMission=true;
//	if (toPos->getPosId()==POS_ID_WESTCONV){if (!position24->isActiveForPacketImport()) position24->setActivePacketImport(true);}
//	if (toPos->getPosId()==POS_ID_EASTCONV){if (!position25->isActiveForPacketImport()) position25->setActivePacketImport(true);}
//	apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('fork mission','resume',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
//}

void forkClass::transferPacket(packetPositionClass *fPos,packetPositionClass *tPos)
{
	if ((!fPos)||(!tPos))
	{
		vLog("error","forkClass::transferPacket()",QString("(!fPos)||(!tPos)"));
		if (fPos)
			vLog("error","forkClass::transferPacket()",QString("fPos:").arg(fPos->getPosId()));
		else
			vLog("error","forkClass::transferPacket()",QString("fPos:NULL"));
		if (tPos)
			vLog("error","forkClass::transferPacket()",QString("tPos:").arg(tPos->getPosId()));
		else
			vLog("error","forkClass::transferPacket()",QString("tPos:NULL"));
		EXIT("Internal error","transferPacket fPos==NUL or tPos==NULL");
	}
	int pack=fPos->getPacketId();
	tPos->setPacketId(pack);
	fPos->setPacketId(0);
}

#include "pch.h"
#include "fr.h"
forkClass::forkClass(QGraphicsScene *scene,QWidget *p):QWidget(p)
{
	QWidget::hide();
	forkPlcTcpServer=NULL;
	plcXInMeters=plcYInMeters=plcZInMeters=0.0;
	hasActiveMission=false;fromPos=toPos=NULL;
	isCarryingPacket=false;
	position24=position25=NULL;
	plcStatusEnum=-1;
	fromPos=toPos=NULL;
	//
	dxfClass *mouseOverItem=new dxfClass(":dxfs/posMouseOver.dxf",QPen(QColor(200,200,200)),true,false,QColor(100,100,100,150),this);
	dxfClass *lockedItem=new dxfClass(":dxfs/posLocked.dxf",QPen(QColor(200,50,50)),false,false,QColor(),this);
	dxfClass *sbItem=new dxfClass(":dxfs/sb.dxf",QPen(QColor(50,50,150)),false,false,QColor(),this);

#ifdef keepPacketCraneOldFunctionality
	positionSb=new newPacketPositionClass(POS_ID_NEWSB,tr("New SB"),scene,mouseOverItem,lockedItem,sbItem,this);positionSb->setGXY(-3025.,-71653.);
	positionPa=new newPacketPositionClass(POS_ID_NEWPACKET,tr("New packet"),scene,mouseOverItem,lockedItem,sbItem,this);positionPa->setGXY(-3025.,-61442.);
#endif
	positionIm=new newPacketPositionClass(POS_ID_SB_PACKET_IMPORT,tr("New SB-packet"),scene,mouseOverItem,lockedItem,sbItem,this);positionIm->setGXY(+520.,-52955.);
	positionForkLift=new forkPositionClass(scene,mouseOverItem,lockedItem,sbItem,this);

#ifdef keepPacketCraneOldFunctionality
	connect(positionSb,SIGNAL(packetDataChangedSignal(int)),this,SLOT(onPacketDataChangedSlot(int)));
	connect(positionPa,SIGNAL(packetDataChangedSignal(int)),this,SLOT(onPacketDataChangedSlot(int)));
#endif
	connect(positionIm,SIGNAL(packetDataChangedSignal(int)),this,SLOT(onPacketDataChangedSlot(int)));
	QTimer *misionTimer=new QTimer(this);
	connect(misionTimer,SIGNAL(timeout()),this,SLOT(onMisionTimer()));
	misionTimer->start(5000);
}
forkClass::~forkClass()
{
}
void forkClass::onPacketDataChangedSlot(int cell)
{
#ifdef keepPacketCraneOldFunctionality
	positionSb->loadFromDatabase();
	positionPa->loadFromDatabase();
#endif
	positionIm->loadFromDatabase();
	positionForkLift->loadFromDatabase();
}
#ifdef keepPacketCraneOldFunctionality
newPacketPositionClass* forkClass::getPositionSb()
{
	return positionSb;
}
newPacketPositionClass* forkClass::getPositionPa()
{
	return positionPa;
}
#endif
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
void forkClass::setPlcX(float x)
{
	plcXInMeters=x;
	positionForkLift->setGXY(-2025.+1000.f*plcXInMeters,-30000.+1000.*plcYInMeters);
}
void forkClass::setPlcY(float y)
{
	plcYInMeters=y;
	positionForkLift->setGXY(-2025.+1000.f*plcXInMeters,-30000.+1000.*plcYInMeters);
}
void forkClass::setPlcZ(float z)
{
	plcZInMeters=z;
	positionForkLift->setGXY(-2025.+1000.f*plcXInMeters,-30000.+1000.*plcYInMeters);
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

void forkClass::setPlcClass(plcClass *plc)
{
	forkPlcTcpServer=plc->getForkLiftTcpServer();
}
#ifdef keepPacketCraneOldFunctionality
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
#endif
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
	if (false)
	{}
#ifdef keepPacketCraneOldFunctionality
	else if (fromPosId==POS_ID_NEWSB)fromPos=positionSb;
	else if (fromPosId==POS_ID_NEWPACKET)fromPos=positionPa;
#endif
	else if (fromPosId==POS_ID_SB_PACKET_IMPORT)fromPos=positionIm;
	else if (fromPosId==POS_ID_WESTCONV)fromPos=position24;
	else if (fromPosId==POS_ID_EASTCONV)fromPos=position25;
	if (fromPos)
	{
		if (false){}
#ifdef keepPacketCraneOldFunctionality
		else if (toPosId==POS_ID_SBEXIT)toPos=positionSbExit;
		else if (toPosId==POS_ID_PACKETEXIT)toPos=positionPacketExit;
		else if (toPosId==POS_ID_STOREEXIT)toPos=positionStoreExit;
#endif
		else if (toPosId==POS_ID_WESTCONV)toPos=position24;
		else if (toPosId==POS_ID_EASTCONV)toPos=position25;
	}
	if (fromPos&&toPos)
		hasActiveMission=true;
#ifdef keepPacketCraneOldFunctionality
	positionSb->loadFromDatabase();
	positionPa->loadFromDatabase();
#endif
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
#ifdef keepPacketCraneOldFunctionality
		case 6:
			fPos=positionSb;break;
		case 8:
			fPos=positionPa;break;
#endif
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
#ifdef keepPacketCraneOldFunctionality
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
#endif
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
	//packetPlcType
	int packetPlcType=0;
	int packetLength=0;
	if (fromPos->hasPacket())
	{
		packetPlcType=fromPos->getPacketTypeForPlc();
		packetLength=fromPos->getPacketLength();
	}
	else
	{
		int craneToPos=GETONE("select value from state where name='craneToPos'").toInt();
		if (craneToPos==fromPos->getPosId())
		{
			int craneFromPosId=GETONE("select value from state where name='craneFromPos'").toInt();
			int packetId=GETONEATMOST(QString("select packetId from positions where cassette=%1").arg(craneFromPosId)).toInt();
			bool isOnSb=GETONE(QString("select sb from packets where packetId=%1").arg(packetId)).toInt()==PACKET_WITHOUT_SB?false:true;
			if (isOnSb)
				packetPlcType=9;
			else
			{
				packetLength=GETONE(QString("select length from packets where packetId=%1").arg(packetId)).toInt();
				if (packetLength>2743)
					packetPlcType=GETONE(QString("select woods from packets where packetId=%1").arg(packetId)).toInt();
				else
					packetPlcType=1;	
			}
		}
	}
	int lenghtType=0;
	if ((packetPlcType==2)||(packetPlcType==4))
		lenghtType=(packetLength<smallLengthForEvenWoods?1:2);
	else
		lenghtType=(packetLength<smallLength?1:2);
	vLog("info","forkClass::newMission()",QString("sendString N160[4]=%1").arg(packetPlcType));
	forkPlcTcpServer->sendString(QString("N160|4|%1").arg(packetPlcType));
	vLog("info","forkClass::newMission()",QString("sendString N160[9]=%1").arg(lenghtType));
	forkPlcTcpServer->sendString(QString("N160|9|%1").arg(lenghtType));
	vLog("info","forkClass::newMission()",QString("sendString N160[0]=%1").arg(fromPos->getPositionIdForForkPlc()));
	forkPlcTcpServer->sendString(QString("N160|0|%1").arg(fromPos->getPositionIdForForkPlc()));
	vLog("info","forkClass::newMission()",QString("sendString N160[1]=%1").arg(toPos->getPositionIdForForkPlc()));
	forkPlcTcpServer->sendString(QString("N160|1|%1").arg(toPos->getPositionIdForForkPlc()));
	if (plcStatusEnum==PLC_STATUS_IDLE)
	{
		vLog("info","forkClass::newMission()",QString("sendString N160[3]=0"));
		forkPlcTcpServer->sendString(QString("N160|3|0"));
	}
}
void forkClass::onMisionTimer()
{
	plcStatusChanged(plcStatusEnum);
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
		else
		{
			if ((!fromPos)&&(!toPos))
				forkPlcTcpServer->sendString(QString("N160|3|3"));
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
bool forkClass::isInWareHouseArea()
{
	return plcYInMeters>59.;
}
bool forkClass::isGoingToWarehouse()
{
	if (toPos)
		return ((toPos->getPosId()==POS_ID_WESTCONV)||(toPos->getPosId()==POS_ID_EASTCONV));
	return false;
}
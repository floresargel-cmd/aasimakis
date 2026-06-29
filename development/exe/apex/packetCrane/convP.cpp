#include "pch.h"
#include "convP.h"
conveyorPositionClass::conveyorPositionClass(int posId,QString name,QGraphicsScene *scene,dxfClass *cellItem,dxfClass *mouseOverItem,dxfClass *lockedItem,dxfClass *cassetteItem,dxfClass *sbItem,dxfClass *fromItem,dxfClass *toItem,QWidget *p)
	:cellPositionClass(posId,name,0,0,0,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,STORAGE_TYPE_NULL,IS_NOT_LOCKED,0,0,0,0,0,scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,p)
{
	activePacketExport=activePacketImport=activeCassetteImport=false;
	loadFromDatabase();
}
conveyorPositionClass::~conveyorPositionClass()
{
}

bool conveyorPositionClass::isActive()
{
	return activePacketExport||activePacketImport||activeCassetteImport;
}
bool conveyorPositionClass::isActiveForPacketExport()
{
	return activePacketExport;
}
bool conveyorPositionClass::isActiveForPacketImport()
{
	return activePacketImport;
}
bool conveyorPositionClass::isActiveForCassetteImport()
{
	return activeCassetteImport;
}
bool conveyorPositionClass::hasIdleCassette()
{
	if ((cassette==0)||(isActive()))
		return false;
	return true;
}

void conveyorPositionClass::setActivePacketExport(bool a)
{
	if ((activePacketImport||activeCassetteImport)&&(a))
	{
		vLog("error","conveyorPositionClass::setActivePacketExport()","Attempt to set activite for conveyor. Conveyor is allready active");
		uExit(tr("Internal error"),tr("Attempt to set activite for conveyor. Conveyor is allready active"));
	}
	activePacketExport=a;
	if (packetPositionClass::getPosId()==POS_ID_WESTCONV)
	{
		vLog("info","conveyorPositionClass::setActivePacketExport()",QString("set status of POS_ID_WESTCONV:%1").arg(activePacketExport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='westConveyorActivePacketExport'").arg(activePacketExport?"1":"0"));
	}
	if (packetPositionClass::getPosId()==POS_ID_EASTCONV)
	{
		vLog("info","conveyorPositionClass::setActivePacketExport()",QString("set status of POS_ID_EASTCONV:%1").arg(activePacketExport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='eastConveyorActivePacketExport                    '").arg(activePacketExport?"1":"0"));
	}
	apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('setAPExport','%1',%2,%3,'%4',%5)").arg(a).arg(packetPositionClass::getPosId()).arg(packetPositionClass::getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
}
void conveyorPositionClass::setActivePacketImport(bool a)
{
	if ((activePacketExport||activeCassetteImport)&&(a))
	{
		vLog("error","conveyorPositionClass::setActivePacketImport()","Attempt to set activite for conveyor. Conveyor is allready active");
		uExit(tr("Internal error"),tr("Attempt to set activite for conveyor. Conveyor is allready active"));
	}
	activePacketImport=a;
	if (packetPositionClass::getPosId()==POS_ID_WESTCONV)
	{
		vLog("info","conveyorPositionClass::setActivePacketImport()",QString("set status of POS_ID_WESTCONV:%1").arg(activePacketImport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='westConveyorActivePacketImport                    '").arg(activePacketImport?"1":"0"));
	}
	if (packetPositionClass::getPosId()==POS_ID_EASTCONV)
	{
		vLog("info","conveyorPositionClass::setActivePacketImport()",QString("set status of POS_ID_EASTCONV:%1").arg(activePacketImport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='eastConveyorActivePacketImport                    '").arg(activePacketImport?"1":"0"));
	}
	apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('setAPImport','%1',%2,%3,'%4',%5)").arg(a).arg(packetPositionClass::getPosId()).arg(packetPositionClass::getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
}
void conveyorPositionClass::setActiveCassetteImport(bool a)
{
	if ((activePacketImport||activePacketExport)&&(a))
	{
		vLog("error","conveyorPositionClass::setActiveCassetteImport()","Attempt to set activite for conveyor. Conveyor is allready active");
		uExit(tr("Internal error"),tr("Attempt to set activite for conveyor. Conveyor is allready active"));
	}
	activeCassetteImport=a;
	if (packetPositionClass::getPosId()==POS_ID_WESTCONV)
	{
		vLog("info","conveyorPositionClass::setActiveCassetteImport()",QString("set status of POS_ID_WESTCONV:%1").arg(activeCassetteImport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='westConveyorActiveCassetteImport                  '").arg(activeCassetteImport?"1":"0"));
	}
	if (packetPositionClass::getPosId()==POS_ID_EASTCONV)
	{
		vLog("info","conveyorPositionClass::setActiveCassetteImport()",QString("set status of POS_ID_EASTCONV:%1").arg(activeCassetteImport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='eastConveyorActiveCassetteImport                  '").arg(activeCassetteImport?"1":"0"));
	}
	apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('setACImport','%1',%2,%3,'%4',%5)").arg(a).arg(packetPositionClass::getPosId()).arg(packetPositionClass::getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
}

void conveyorPositionClass::saveToDatabase()
{
	packetPositionClass::saveToDatabase();
	apexExecQuery(QString("update position set cassette=%1 where posId=%2").arg(cassette).arg(packetPositionClass::getPosId()));
}
void conveyorPositionClass::loadFromDatabase()
{
	cellPositionClass::loadFromDatabase();
	cassette=GETONE(QString("select cassette from positions where posId=%1").arg(packetPositionClass::getPosId())).toInt();
	if (packetPositionClass::getPosId()==POS_ID_WESTCONV)
	{
		activePacketExport=GETONE(QString("select value from state where name='westConveyorActivePacketExport'")).toString().trimmed()=="1"?true:false;
		activePacketImport=GETONE(QString("select value from state where name='westConveyorActivePacketImport'")).toString().trimmed()=="1"?true:false;
		activeCassetteImport=GETONE(QString("select value from state where name='westConveyorActiveCassetteImport'")).toString().trimmed()=="1"?true:false;
	}
	else if (packetPositionClass::getPosId()==POS_ID_EASTCONV)
	{
		activePacketExport=GETONE(QString("select value from state where name='eastConveyorActivePacketExport'")).toString().trimmed()=="1"?true:false;
		activePacketImport=GETONE(QString("select value from state where name='eastConveyorActivePacketImport'")).toString().trimmed()=="1"?true:false;
		activeCassetteImport=GETONE(QString("select value from state where name='eastConveyorActiveCassetteImport'")).toString().trimmed()=="1"?true:false;
	}
	vLog("info","conveyorPositionClass::loadFromDatabase()",QString("id:%1,activePacketExport:%2,activePacketImport:%3,activeCassetteImport:%4").arg(packetPositionClass::getPosId()).arg(activePacketExport).arg(activePacketImport).arg(activeCassetteImport));
}
//void conveyorPositionClass::loadContentsFromDatabase()
//{
//	packetPositionClass::loadContentsFromDatabase();
//}
bool conveyorPositionClass::canTakePacketThatIsOnPos(packetPositionClass *p)
{
	if (cellPositionClass::cassette!=0)
	{
		if (hasPacket())
			return false;
		if (activeCassetteImport)
			return false;
		int craneActiveFrom=GETONEATMOST(QString("select value from state where name='craneFromPos'")).toInt();
		if (craneActiveFrom==packetPositionClass::getPosId())
			return false;
		//
		return canCellTakePacketInPosG(cassette,p->getPacketId());
	}
	else //cassette=0
	{
		int fromPos=0;
		int craneToPos=GETONE(QString("select value from state where name='craneToPos'")).toInt();
		if (craneToPos==packetPositionClass::getPosId())
			fromPos=GETONEATMOST(QString("select value from state where name='craneFromPos'")).toInt();
		if (activeCassetteImport||activePacketExport||activePacketImport||(fromPos==0))
			return false;
		else//has activeExport
		{
			if (fromPos<POS_ID_CRANE)
				return canCellTakePacketInPosG(fromPos,p->getPacketId());
			else
			{
				fromPos=getFirst(QString("select cassette from positions where (posId=%1 or posId=%2)and cassette<>0").arg(POS_ID_CRANE).arg(fromPos),MAIN_DB_CONNECTION).toInt();
				if (fromPos!=0)
					return canCellTakePacketInPosG(fromPos,p->getPacketId());
			}
			return true;/////an empty cassete is coming out that takes the incoming packet
		}
		return false;//never here
	}
	return false;//never here
}


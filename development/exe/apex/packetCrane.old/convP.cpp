#include "convP.h"
conveyorPositionClass::conveyorPositionClass(int posId,GLfloat x,GLfloat y,GLfloat z,QWidget *p):cellPositionClass(posId,0,0,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,STORAGE_TYPE_NULL,IS_NOT_LOCKED,0,0,p)
{
	activePacketExport=activePacketImport=activeCassetteImport=false;
	setOpenGlX(x);
	setOpenGlY(y);
	setOpenGlZ(z);
	loadFromDatabase();
}
conveyorPositionClass::~conveyorPositionClass()
{
}
void conveyorPositionClass::mouseClicked(QColor pickedColor)
{
	if (pickedColor==pickingColor)
	{
	}
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
		EXIT(tr("Internal error"),tr("Attempt to set activite for conveyor. Conveyor is allready active"));
	}
	activePacketExport=a;
	if (posId==POS_ID_WESTCONV)
	{
		vLog("info","conveyorPositionClass::setActivePacketExport()",QString("set status of POS_ID_WESTCONV:%1").arg(activePacketExport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='westConveyorActivePacketExport                    '").arg(activePacketExport?"1":"0"));
	}
	if (posId==POS_ID_EASTCONV)
	{
		vLog("info","conveyorPositionClass::setActivePacketExport()",QString("set status of POS_ID_EASTCONV:%1").arg(activePacketExport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='eastConveyorActivePacketExport                    '").arg(activePacketExport?"1":"0"));
	}
	apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('setAPExport','%1',%2,%3,'%4',%5)").arg(a).arg(posId).arg(posId).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
}
void conveyorPositionClass::setActivePacketImport(bool a)
{
	if ((activePacketExport||activeCassetteImport)&&(a))
	{
		vLog("error","conveyorPositionClass::setActivePacketImport()","Attempt to set activite for conveyor. Conveyor is allready active");
		EXIT(tr("Internal error"),tr("Attempt to set activite for conveyor. Conveyor is allready active"));
	}
	activePacketImport=a;
	if (posId==POS_ID_WESTCONV)
	{
		vLog("info","conveyorPositionClass::setActivePacketImport()",QString("set status of POS_ID_WESTCONV:%1").arg(activePacketImport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='westConveyorActivePacketImport                    '").arg(activePacketImport?"1":"0"));
	}
	if (posId==POS_ID_EASTCONV)
	{
		vLog("info","conveyorPositionClass::setActivePacketImport()",QString("set status of POS_ID_EASTCONV:%1").arg(activePacketImport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='eastConveyorActivePacketImport                    '").arg(activePacketImport?"1":"0"));
	}
	apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('setAPImport','%1',%2,%3,'%4',%5)").arg(a).arg(posId).arg(posId).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
}
void conveyorPositionClass::setActiveCassetteImport(bool a)
{
	if ((activePacketImport||activePacketExport)&&(a))
	{
		vLog("error","conveyorPositionClass::setActiveCassetteImport()","Attempt to set activite for conveyor. Conveyor is allready active");
		EXIT(tr("Internal error"),tr("Attempt to set activite for conveyor. Conveyor is allready active"));
	}
	activeCassetteImport=a;
	if (posId==POS_ID_WESTCONV)
	{
		vLog("info","conveyorPositionClass::setActiveCassetteImport()",QString("set status of POS_ID_WESTCONV:%1").arg(activeCassetteImport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='westConveyorActiveCassetteImport                  '").arg(activeCassetteImport?"1":"0"));
	}
	if (posId==POS_ID_EASTCONV)
	{
		vLog("info","conveyorPositionClass::setActiveCassetteImport()",QString("set status of POS_ID_EASTCONV:%1").arg(activeCassetteImport?"true":"false"));
		apexExecQuery(QString("update state set value=%1 where name='eastConveyorActiveCassetteImport                  '").arg(activeCassetteImport?"1":"0"));
	}
	apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('setACImport','%1',%2,%3,'%4',%5)").arg(a).arg(posId).arg(posId).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
}

void conveyorPositionClass::saveToDatabase()
{
	packetPositionClass::saveToDatabase();
	apexExecQuery(QString("update position set cassette=%1 where posId=%2").arg(cassette).arg(posId));
}
void conveyorPositionClass::loadFromDatabase()
{
	cellPositionClass::loadFromDatabase();
	cassette=GETONE(QString("select cassette from positions where posId=%1").arg(posId)).toInt();
	if (posId==POS_ID_WESTCONV)
	{
		activePacketExport=GETONE(QString("select value from state where name='westConveyorActivePacketExport'")).toString().trimmed()=="1"?true:false;
		activePacketImport=GETONE(QString("select value from state where name='westConveyorActivePacketImport'")).toString().trimmed()=="1"?true:false;
		activeCassetteImport=GETONE(QString("select value from state where name='westConveyorActiveCassetteImport'")).toString().trimmed()=="1"?true:false;
	}
	else if (posId==POS_ID_EASTCONV)
	{
		activePacketExport=GETONE(QString("select value from state where name='eastConveyorActivePacketExport'")).toString().trimmed()=="1"?true:false;
		activePacketImport=GETONE(QString("select value from state where name='eastConveyorActivePacketImport'")).toString().trimmed()=="1"?true:false;
		activeCassetteImport=GETONE(QString("select value from state where name='eastConveyorActiveCassetteImport'")).toString().trimmed()=="1"?true:false;
	}
	vLog("info","conveyorPositionClass::loadFromDatabase()",QString("id:%1,activePacketExport:%2,activePacketImport:%3,activeCassetteImport:%4").arg(posId).arg(activePacketExport).arg(activePacketImport).arg(activeCassetteImport));
}
void conveyorPositionClass::loadContentsFromDatabase()
{
	packetPositionClass::loadContentsFromDatabase();
}
bool conveyorPositionClass::canTakePacketThatIsOnPos(packetPositionClass *p)
{
	if (cellPositionClass::cassette!=0)
	{
		if (hasPacket())
			return false;
		if (activeCassetteImport)
			return false;
		int craneActiveFrom=GETONEATMOST(QString("select value from state where name='craneFromPos                                      '")).toInt();
		if (craneActiveFrom==posId)
			return false;
		//
		int cassetteAccepts=GETONE(QString("select acceptableContents from positions where posId=%1").arg(cassette)).toInt();
		if ( (p->getPacketPlacingType()==LEFT_PLACED_PACKET) && (cassetteAccepts==CENTER_ONLY_PACKET_PLACING_CASSETTE) &&  (!p->getIsOnSB())  )
			return false;
		//
		int storagingTypeOfCassette=GETONE(QString("select storagingType from positions where posId=%1").arg(cassette)).toInt();
		if (p->getStoragingTypeOfPacket()!=storagingTypeOfCassette)
				return false;
		return true;/////empty cassete that takes the incoming packet
	}
	else //cassette=0
	{
		int cassetteThatIsComingOut=0;
		int craneToPos=GETONE(QString("select value from state where name='craneToPos                                        '")).toInt();
		if (craneToPos==posId)
			cassetteThatIsComingOut=GETONEATMOST(QString("select value from state where name='craneFromPos                                      '")).toInt();
		if (activeCassetteImport||activePacketExport||activePacketImport||(cassetteThatIsComingOut==0))
			return false;
		else//has activeExport
		{
			//
			int packetThatIsCommingOut=GETONEATMOST(QString("select packetId from positions where (posId=%1 or posId=%2) and packetId<>0").arg(POS_ID_CRANE).arg(cassetteThatIsComingOut)).toInt();
			if (packetThatIsCommingOut!=0)
				return false;
			//
			int cassetteThatIsCommingOutAccepts=GETONE(QString("select acceptableContents from positions where posId=%1").arg(cassetteThatIsComingOut)).toInt();
			if (p->getPacketPlacingType()>cassetteThatIsCommingOutAccepts)
				return false;
			//
			int storagingTypeOfCassetteThatIsComingOut=GETONE(QString("select storagingType from positions where posId=%2").arg(cassetteThatIsComingOut)).toInt();
			if (p->getStoragingTypeOfPacket()!=storagingTypeOfCassetteThatIsComingOut)
				return false;
			//
			return true;/////an empty cassete is coming out that takes the incoming packet
		}
		return false;//never here
	}
	return false;//never here
}


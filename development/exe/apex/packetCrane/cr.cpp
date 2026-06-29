#include "pch.h"
#include "cr.h"
craneClass::craneClass(QGraphicsScene *scene,QWidget *p):QWidget(p)
{
	QWidget::hide();
	cranePlcTcpServer=NULL;
	plcXInMeters=plcYInMeters=plcZInMeters=0.0;
	hasActiveMission=false;
	isCarryingCassette=false;
	plcCassetteOnCrane=0;
	dxfClass *cellItem=new dxfClass(":dxfs/conveyor.dxf",QPen(QColor(250,150,150)),false,false,QColor(),this);
	dxfClass *mouseOverItem=new dxfClass(":dxfs/posMouseOver.dxf",QPen(QColor(150,50,50)),true,true,QColor(255,100,100,150),this);
	dxfClass *lockedItem=new dxfClass(":dxfs/posLocked.dxf",QPen(QColor(200,50,50)),false,false,QColor(),this);
	dxfClass *cassetteItem=new dxfClass(":dxfs/cassette.dxf",QPen(QColor(150,50,50)),false,false,QColor(),this);
	dxfClass *sbItem=new dxfClass(":dxfs/sb.dxf",QPen(QColor(150,50,50)),false,false,QColor(),this);
	dxfClass *fromItem=new dxfClass(":dxfs/from.dxf",QPen(QColor(50,50,150)),false,false,QColor(),this);
	dxfClass *toItem=new dxfClass(":dxfs/to.dxf",QPen(QColor(50,150,50)),false,false,QColor(),this);
	//
	position24=new conveyorPositionClass(POS_ID_WESTCONV,tr("West conveyor"),scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this);position24->setPlcPos(0.,0.,-10762.f);
	position25=new conveyorPositionClass(POS_ID_EASTCONV,tr("East conveyor"),scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this);position25->setPlcPos(0.,0.,8247.f);
	positionCrane=new cellPositionClass(POS_ID_CRANE,tr("Crane"),0,0,0,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,STORAGE_TYPE_NULL,IS_NOT_LOCKED,0,0,0,0,bigLength,scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this);
	positionCraneUp=new cellPositionClass(POS_ID_CRANE,tr("Crane"),0,0,0,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,STORAGE_TYPE_NULL,IS_NOT_LOCKED,0,0,0,0,bigLength,scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this);
	positionCraneDown=new cellPositionClass(POS_ID_CRANE,tr("Crane"),0,0,0,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,STORAGE_TYPE_NULL,IS_NOT_LOCKED,0,0,0,0,bigLength,scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this);

	stationExit1=new conveyorPositionClass(stationExport1PlcId,tr("Exports 1"),scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this);stationExit1 ->setPlcPos(33191.,0.f,2720.+55.f+165.f+4930.f);
	stationImport=new conveyorPositionClass(stationImportPlcId ,tr("Imports") ,scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this);stationImport->setPlcPos(34012.,0.f,2720.+55.f+165.f+4930.f);
	stationExit2=new conveyorPositionClass(stationExport2PlcId,tr("Exports 2"),scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this);stationExit2 ->setPlcPos(34833.,0.f,2720.+55.f+165.f+4930.f);
	stationBackExit1=new conveyorPositionClass(stationBackExport1PlcId,tr("Exports 1 back"),scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this);stationBackExit1 ->setPlcPos(33191.,0.f,2720.+55.f+165.f+4930.f+7000.f);
	stationBackImport=new conveyorPositionClass(stationBackImportPlcId ,tr("Imports back") ,scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this);stationBackImport->setPlcPos(34012.,0.f,2720.+55.f+165.f+4930.f+7000.f);
	stationBackExit2=new conveyorPositionClass(stationBackExport2PlcId,tr("Exports 2 back"),scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this);stationBackExit2 ->setPlcPos(34833.,0.f,2720.+55.f+165.f+4930.f+7000.f);

	packingExit1			=new conveyorPositionClass(packingExport1PlcId,tr("Packing Exports 1"),scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this)					;packingExit1			->setPlcPos(37296.,0.f,-11400.f);
	packingImport			=new conveyorPositionClass(packingImportPlcId ,tr("Packing Imports") ,scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this)						;packingImport		->setPlcPos(38117.,0.f,-11400.f);
	packingExit2			=new conveyorPositionClass(packingExport2PlcId,tr("Packing Exports 2"),scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this)					;packingExit2			->setPlcPos(38938.,0.f,-11400.f);
	packingBackExit1	=new conveyorPositionClass(packingBackExport1PlcId,tr("Packing Exports 1 back"),scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this)	;packingBackExit1 ->setPlcPos(37296.,0.f,-11400.f-7000.f);
	packingBackImport	=new conveyorPositionClass(packingBackImportPlcId ,tr("Packing Imports back") ,scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this)	;packingBackImport->setPlcPos(38117.,0.f,-11400.f-7000.f);
	packingBackExit2	=new conveyorPositionClass(packingBackExport2PlcId,tr("Packing Exports 2 back"),scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,this)	;packingBackExit2 ->setPlcPos(38938.,0.f,-11400.f-7000.f);


	positionCraneUp->setZValue(-1);
	positionCraneDown->setZValue(-1);

	stationExit1->setZValue(-1);
	stationImport->setZValue(-1);
	stationExit2->setZValue(-1);
	stationBackExit1->setZValue(-1);
	stationBackImport->setZValue(-1);
	stationBackExit2->setZValue(-1);

	packingExit1->setZValue(-1);
	packingImport->setZValue(-1);
	packingExit2->setZValue(-1);
	packingBackExit1->setZValue(-1);
	packingBackImport->setZValue(-1);
	packingBackExit2->setZValue(-1);

	connect(positionCrane,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	
	connect(positionCraneUp,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	connect(positionCraneDown,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	
	connect(stationExit1,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	connect(stationImport,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	connect(stationExit2,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	connect(stationBackExit1,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	connect(stationBackImport,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	connect(stationBackExit2,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));

	connect(packingExit1,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	connect(packingImport,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	connect(packingExit2,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	connect(packingBackExit1,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	connect(packingBackImport,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
	connect(packingBackExit2,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));

	QTimer *misionTimer=new QTimer(this);
	connect(misionTimer,SIGNAL(timeout()),this,SLOT(onMisionTimer()));
	misionTimer->start(5000);
	QTimer *transferStationCassettesTimer=new QTimer(this);
	connect(transferStationCassettesTimer,SIGNAL(timeout()),this,SLOT(transferStationCassettesTimerSlot()));
	transferStationCassettesTimer->start(5000);
}
craneClass::~craneClass()
{
}
void craneClass::onCellDataChangedSlot(int cell)
{
	positionCrane->loadFromDatabase();
	positionCraneUp->loadFromDatabase();
	positionCraneDown->loadFromDatabase();
}
void craneClass::setPlcData(tuxipClass *tuxip)
{
	stationExit1 ->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",34);
	stationImport->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",33);
	stationExit2 ->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",30);
	stationBackExit1 ->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",35);
	stationBackImport->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",32);
	stationBackExit2 ->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",31);

	packingExit1			->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",44);
	packingImport			->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",43);
	packingExit2			->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",40);
	packingBackExit1	->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",45);
	packingBackImport	->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",42);
	packingBackExit2	->setPlcData(tuxip,packetCraneIp,"ConveyorBasket",41);
}
bool craneClass::tryToMovePlcCassette(int plcCassette,int posId)
{
	int pcCassette=getFirst(QString("select cassette from positions where posId=%1").arg(posId),MAIN_DB_CONNECTION).toInt();
	if ( (plcCassette!=0) && (pcCassette==0) )
	{
		int dbPosOfCassette=getFirst(QString("select posId from positions where cassette=%1 and (posId>=%2 and posId<=%3)").arg(plcCassette).arg(stationExport2PlcId).arg(stationBackExport1PlcId),MAIN_DB_CONNECTION).toInt();
		if (dbPosOfCassette!=0)
		{
			transferCassette(dbPosOfCassette,posId,false);
			return true;
		}
	}
	return false;
}
void craneClass::transferStationCassettesTimerSlot()
{
	if (!cranePlcTcpServer)
		return;
	if (cranePlcTcpServer->getTuxipClass()->getHasError())
		return;
	QVector<int> vals=cranePlcTcpServer->getTuxipClass()->readIntegerValues("ConveyorBasket",30,6);
	if (tryToMovePlcCassette(vals[0],stationExport2PlcId))return;
	if (tryToMovePlcCassette(vals[1],stationBackExport2PlcId))return;
	if (tryToMovePlcCassette(vals[2],stationBackImportPlcId))return;
	if (getFirst(QString("select value from state where name='craneFromPos'"),MAIN_DB_CONNECTION).toInt()!=stationImportPlcId)
		if (tryToMovePlcCassette(vals[3],stationImportPlcId))return;
	if (tryToMovePlcCassette(vals[4],stationExport1PlcId))return;
	if (tryToMovePlcCassette(vals[5],stationBackExport1PlcId))return;
}
void craneClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((ip==packetCraneIp)&&(  (table=="ConveyorBasket")||(table=="ConveyorData")))
	{
		packingExit1->dataInPlcChanged(ip,table,index,value);
		packingImport->dataInPlcChanged(ip,table,index,value);
		packingExit2->dataInPlcChanged(ip,table,index,value);
		packingBackExit1->dataInPlcChanged(ip,table,index,value);//for basket
		packingBackImport->dataInPlcChanged(ip,table,index,value);
		packingBackExit2->dataInPlcChanged(ip,table,index,value);
	}
	if ((ip==crane3Ip)&&(table=="packetId"))
	{
		packingBackExit1->dataInPlcChanged(ip,table,index,value);//for packet
	}
}
conveyorPositionClass *craneClass::getConveyorPosition24()
{
	return position24;
}
conveyorPositionClass *craneClass::getConveyorPosition25()
{
	return position25;
}
conveyorPositionClass *craneClass::getConveyorStationImport()
{
	return stationImport;
}
cellPositionClass *craneClass::getCranePosition()
{
	return positionCrane;
}
conveyorPositionClass *craneClass::getStationExit1()
{
	return stationExit1;
}
conveyorPositionClass *craneClass::getStationImport()
{
	return stationImport;
}
conveyorPositionClass *craneClass::getStationExit2()
{
	return stationExit2;
}
conveyorPositionClass *craneClass::getStationBackExit1()
{
	return stationBackExit1;
}
conveyorPositionClass *craneClass::getStationBackImport()
{
	return stationBackImport;
}
conveyorPositionClass *craneClass::getStationBackExit2()
{
	return stationBackExit2;
}
//
conveyorPositionClass *craneClass::getPackingExit1()
{
	return packingExit1;
}
conveyorPositionClass *craneClass::getPackingImport()
{
	return packingImport;
}
conveyorPositionClass *craneClass::getPackingExit2()
{
	return packingExit2;
}
conveyorPositionClass *craneClass::getPackingBackExit1()
{
	return packingBackExit1;
}
conveyorPositionClass *craneClass::getPackingBackImport()
{
	return packingBackImport;
}
conveyorPositionClass *craneClass::getPackingBackExit2()
{
	return packingBackExit2;
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
	positionCrane->setPlcPos(1000.f*plcXInMeters,0.f,0.f);
	positionCraneUp->setPlcPos(1000.f*plcXInMeters,0.f,5095.f+1000.f*plcZInMeters);
	positionCraneDown->setPlcPos(1000.f*plcXInMeters,0.f,-13915+1000.f*plcZInMeters);
}
void craneClass::setPlcY(float y)
{
	plcYInMeters=y;
	positionCrane->setPlcPos(1000.f*plcXInMeters,0.f,0.f);
	positionCraneUp->setPlcPos(1000.f*plcXInMeters,0.f,5095.f+1000.f*plcZInMeters);
	positionCraneDown->setPlcPos(1000.f*plcXInMeters,0.f,-13915+1000.f*plcZInMeters);
}
void craneClass::setPlcZ(float z)
{
	plcZInMeters=z;
	positionCrane->setPlcPos(1000.f*plcXInMeters,0.f,0.f);
	positionCraneUp->setPlcPos(1000.f*plcXInMeters,0.f,5095.f+1000.f*plcZInMeters);
	positionCraneDown->setPlcPos(1000.f*plcXInMeters,0.f,-13915+1000.f*plcZInMeters);
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
	if (positionCrane->hasCassette())
		return positionCrane;
	if (fromPos->hasCassette())
		return fromPos;
return NULL;
}
bool craneClass::isIdle()
{
	if (fromPos||toPos||hasActiveMission||(plcStatusEnum!=PLC_STATUS_IDLE))
		return false;
	return true;
}
bool craneClass::newMission(int fromId,int toId,bool resume,bool manualExportOfCassette)
{
	vLog("info","craneClass::newMission()",QString("fromId:%1,toId:%2,resume:%3,manualExportOfCassette:%4").arg(fromId).arg(toId).arg(resume).arg(manualExportOfCassette));
	return newMission(allPosisions->getCellPosWithPosId(fromId),allPosisions->getCellPosWithPosId(toId),resume,manualExportOfCassette);
}
void craneClass::loadFromDatabase()
{
	int fromPosId=GETONE("select value from state where name='craneFromPos             '").toInt();
	int toPosId=GETONE("select value from state where name='craneToPos               '").toInt();
	vLog("info","craneClass::loadFromDatabase()",QString("fromPosId:%1,toPosId:%2").arg(fromPosId).arg(toPosId));
	//fromPos =allPosisions->getCellPosWithPosId(fromPosId);
	//toPos =allPosisions->getCellPosWithPosId(toPosId);
	setFromToPos(allPosisions->getCellPosWithPosId(fromPosId),allPosisions->getCellPosWithPosId(toPosId));
	if (fromPos&&toPos)
		hasActiveMission=true;
	position24->loadFromDatabase();
	position25->loadFromDatabase();
	positionCrane->loadFromDatabase();
	positionCraneUp->loadFromDatabase();
	positionCraneDown->loadFromDatabase();
}
bool craneClass::newMission(cellPositionClass *from,cellPositionClass *to,bool resume,bool manualExportOfCassette)
{
	if ((!from)||(!to))
		return false;
	if (from->getIsLocked()||to->getIsLocked())
		return false;
	if (!canCellTakePacketInPosG(to->getPosId(),from->getPacketId()))
		return false;
	vLog("info","craneClass::newMission()",QString("fromId:%1,toId:%2,resume:%3,manualExportOfCassette:%4").arg(from->getPosId()).arg(to->getPosId()).arg(resume).arg(manualExportOfCassette));
	if (  (fromPos||toPos||hasActiveMission||(plcStatusEnum!=PLC_STATUS_IDLE))  &&  (!resume)  )
	{
		int f=0;if(fromPos)f=fromPos->getPosId();
		int t=0;if(toPos)t=toPos->getPosId();
		vLog("error","craneClass::newMission()",tr("Crane is already with a mission from:%1 to:%2 hasActiveMission:%3 plcStatusEnum:%4").arg(f).arg(t).arg(hasActiveMission?"true":"false").arg(plcStatusEnum));
		return false;
		uExit(tr("Internal error"),tr("Crane is already with a mission from:%1 to:%2 hasActiveMission:%3 plcStatusEnum:%4").arg(f).arg(t).arg(hasActiveMission?"true":"false").arg(plcStatusEnum));
	}
	//fromPos =from;
	//toPos =to;
	setFromToPos(from,to);
	hasActiveMission=true;
	if (!resume)
	{
		apexExecQuery(QString("update state set value=%1 where name='craneFromPos'").arg(fromPos->getPosId()));
		apexExecQuery(QString("update state set value=%1 where name='craneToPos'").arg(toPos->getPosId()));
		apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('crane mission','start',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	}
	else
		apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('crane mission','resume',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	if (!manualExportOfCassette&&(toPos->getPosId()==POS_ID_WESTCONV)&&(getThePositionThatTheActiveCassetteIs()->hasPacket()))position24->setActivePacketExport(true);
	if (!manualExportOfCassette&&(toPos->getPosId()==POS_ID_EASTCONV)&&(getThePositionThatTheActiveCassetteIs()->hasPacket()))position25->setActivePacketExport(true);

	vLog("info","craneClass::newMission()",QString("sendString F151[20]=%1").arg(fromPos->getXCorr()));
	cranePlcTcpServer->sendString(QString("F151|20|%1").arg(fromPos->getXCorr()));
	vLog("info","craneClass::newMission()",QString("sendString F151[21]=%1").arg(fromPos->getZCorr()));
	cranePlcTcpServer->sendString(QString("F151|21|%1").arg(fromPos->getZCorr()));
	vLog("info","craneClass::newMission()",QString("sendString F151[22]=%1").arg(toPos->getXCorr()));
	cranePlcTcpServer->sendString(QString("F151|22|%1").arg(toPos->getXCorr()));
	vLog("info","craneClass::newMission()",QString("sendString F151[23]=%1").arg(toPos->getZCorr()));
	cranePlcTcpServer->sendString(QString("F151|23|%1").arg(toPos->getZCorr()));
	
	vLog("info","craneClass::newMission()",QString("sendString N150[0]=%1").arg(fromPos->getPositionIdForCranePlc()));
	cranePlcTcpServer->sendString(QString("N150|0|%1").arg(fromPos->getPositionIdForCranePlc()));
	vLog("info","craneClass::newMission()",QString("sendString N150[1]=%1").arg(toPos->getPositionIdForCranePlc()));
	cranePlcTcpServer->sendString(QString("N150|1|%1").arg(toPos->getPositionIdForCranePlc()));
	if (plcStatusEnum==PLC_STATUS_IDLE)
	{
		vLog("info","craneClass::newMission()",QString("plcStatusEnum==PLC_STATUS_IDLE sendString N150[3]=0"));
		cranePlcTcpServer->sendString(QString("N150|3|0"));
	}
	return true;
}
void craneClass::onMisionTimer()
{
	plcCassetteOnCraneChanged(plcCassetteOnCrane);
	plcStatusChanged(plcStatusEnum);
}
void craneClass::plcStatusChanged(int status)
{
	plcStatusEnum=status;
	//if ((plcStatusEnum!=PLC_STATUS_EXECUTING)&&(plcStatusEnum!=PLC_STATUS_FINISHED_OK)&&(hasActiveMission))
	//	uExit(tr("Error"),tr("Crane pls is with status %1 and pc has not an active mission. Please correct this and restart the software.").arg(plcStatusEnum));
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
			transferCassette(getThePositionThatTheActiveCassetteIs(),toPos,true);
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
				fromPos->loadFromDatabase();
				fromPos->updateGui();
#ifdef tempForMove
				if (toPos->getPosId()<MAX_CELL_POS_ID)
				{
					apexExecQuery(QString("update positions set cassette=%1 where posId=%1").arg(toPos->getPosId()));
				}
#endif
				toPos->loadFromDatabase();
//				toPos =fromPos =NULL;
				setFromToPos(nullptr,nullptr);
				hasActiveMission=false;
				vLog("info","craneClass::plcStatusChanged()",QString("sendString N150[3]=2"));
				cranePlcTcpServer->sendString(QString("N150|3|2"));
			}
		}
	}
	else if (plcStatusEnum==PLC_STATUS_ABORT)
	{
		if (fromPos&&toPos)
		{
			if (fromPos->getCassette()!=0)
			{
				apexExecQuery(QString("update state set value='0' where name='craneFromPos' or name='craneToPos'"));
				apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('crane mission','aborted',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
				//toPos =fromPos =NULL;
				setFromToPos(nullptr,nullptr);
				hasActiveMission=false;
				cranePlcTcpServer->sendString(QString("N150|3|4"));
			}
			else
			{
				cellPositionClass * cassettePos=getThePositionThatTheActiveCassetteIs();
				if ((fromPos->getCassette()==0)||(cassettePos->getCassette()!=0))
				{
					transferCassette(cassettePos,fromPos,true);
					apexExecQuery(QString("update state set value='0' where name='craneFromPos' or name='craneToPos'"));
					apexExecQuery(QString("insert into log (type,status,fromPosId,toPosId,date,sec) values('crane mission','aborted',%1,%2,'%3',%4)").arg(fromPos->getPosId()).arg(toPos->getPosId()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
					//toPos =fromPos =NULL;
					setFromToPos(nullptr,nullptr);
					hasActiveMission=false;
					cranePlcTcpServer->sendString(QString("N150|3|4"));
				}
			}
		}
	}
}

//void craneClass::resumeMission(int fromId,int toId)
//{
//	return resumeMission(allPosisions->getCellPosWithPosId(fromId),allPosisions->getCellPosWithPosId(toId));
//}
//void craneClass::resumeMission(cellPositionClass *from,cellPositionClass *to)
//{
//	if (fromPos||toPos||hasActiveMission)
//		uExit(tr("Internal error"),tr("Crane is already with a mission"));
//	fromPos =from;
//	toPos =to;
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

void craneClass::plcCassetteOnCraneChanged(int cass)
{
	plcCassetteOnCrane=cass;
	vLog("info","craneClass::plcCassetteOnCraneChanged",QString().number(cass));
	if (plcCassetteOnCrane!=positionCrane->getCassette())
	{
		if ((plcCassetteOnCrane!=0)&&toPos&&fromPos)
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
				transferCassette(fromPos,positionCrane,true);
				cranePlcTcpServer->sendString(QString("N150|16|%1").arg(plcCassetteOnCrane));
			}
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
void craneClass::transferCassette(int fromPos,int toPos,bool updatePlc)
{
	cellPositionClass *fPos=allPosisions->getCellPosWithPosId(fromPos);
	cellPositionClass *tPos=allPosisions->getCellPosWithPosId(toPos);
	if (fPos&&tPos)
		transferCassette(fPos,tPos,updatePlc);
}
void craneClass::setFromToPos(cellPositionClass *from,cellPositionClass *to)
{
	if (fromPos)
		fromPos->setIsFrom(false);
	if (toPos)
		toPos->setIsTo(false);
	fromPos=from;
	toPos=to;
	if (fromPos)
		fromPos->setIsFrom(true);
	if (toPos)
		toPos->setIsTo(true);
}
void craneClass::transferCassette(cellPositionClass *fPos,cellPositionClass *tPos,bool updatePlc)
{
	if (fPos)vLog("info","craneClass::transferCassette()",QString("fPos:%1").arg(fPos->getPosId()));
	if (tPos)vLog("info","craneClass::transferCassette()",QString("tPos:%1").arg(tPos->getPosId()));
	if ((!fPos)||(!tPos))
	{
		vLog("error","craneClass::transferCassette()","transferCassette fPos==NUL or tPos==NULL");
		uExit("Internal error","transferCassette fPos==NUL or tPos==NULL");
	}
	int fromId=fPos->getPosId();
	int fromCassette=fPos->getCassette();
	int fromPacket=fPos->getPacketId();

	int toId=tPos->getPosId();
	int toCassette=tPos->getCassette();
	int toPacket=tPos->getPacketId();
	vLog("info","craneClass::transferCassette()",QString("%1->%2").arg(fPos->getPosId()).arg(tPos->getPosId()));
	vLog("info","craneClass::transferCassette()",QString("fPos:%1").arg(fPos->getPosId()));
	vLog("info","craneClass::transferCassette()",QString("fPos packet:%1").arg(fPos->getPacketId()));
	vLog("info","craneClass::transferCassette()",QString("tPos:%1").arg(tPos->getPosId()));
	vLog("info","craneClass::transferCassette()",QString("tPos packet:%1").arg(tPos->getPacketId()));
	if ((fromCassette==0)||(toCassette!=0)||(toPacket!=0))
	{
		vLog("error","craneClass::transferCassette()","transferCassette fPos==NUL or tPos==NULL");
		uExit("Internal error",QString("transferCassette\nfromId=%1 fromCassette=%2 fromPacket=%3\ntoId=%4 toCassette=%5 toPacket=%6")
			.arg(fromId).arg(fromCassette).arg(fromPacket).arg(toId).arg(toCassette).arg(toPacket));
	}
	fPos->setCassette(0,updatePlc);
	tPos->setCassette(fromCassette,updatePlc);
	tPos->setPacketId(fromPacket);
	if (updatePlc)
	{
		fPos->updatePacketIdInPlc(cranePlcTcpServer->getTuxipClass());
		tPos->updatePacketIdInPlc(cranePlcTcpServer->getTuxipClass());
	}
}

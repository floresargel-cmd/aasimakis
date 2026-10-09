#include "pch.h"
#include "forkPacking.h"
forkPackingClass::forkPackingClass(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,allPosPackingForkClass *allPos_,QGraphicsScene *layoutScene_,QWidget *p):QWidget(p)
{
	splash->showMessage(tr("Initializing fork..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	craneTuxip=craneTuxip_;
	forkTuxip=forkTuxip_;
	allPos=allPos_;
	setVisible(false);
	layoutScene=layoutScene_;

	gItemClass *posItem=new gItemClass(":dxfs/packingFork/packetRect.dxf",QPen(QColor(200,200,250)),false,false,QColor(),this);
	gItemClass *mouseOverItem=new gItemClass(":dxfs/packingFork/packetRect.dxf",QPen(QColor(255,100,100)),true,true,QColor(255,100,100,150),this);
	gItemClass *packetItem=new gItemClass(":dxfs/packingFork/packet.dxf",QPen(QColor(150,150,100)),false,false,QColor(),this);packetItem->setFillBrush(QBrush(QColor(150,150,100,50)));
	gItemClass *posFrom=new gItemClass(":dxfs/packingFork/posFrom.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);
	gItemClass *posTo=new gItemClass(":dxfs/packingFork/posTo.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);
	myPos=new posPackingForkClass(0,nullptr,forkTuxip,forkPlcId,7000.f,-1500.f,0.,0.,tr("Fork Position"),layoutScene,posItem,mouseOverItem,packetItem,posFrom,posTo,this);
	QColor craneColor=QColor(150,150,175);
	forkXItem=new gItemClass(":dxfs/packingFork/forkX.dxf",QPen(QColor(200,225,200)),false,false,QColor(),this);
	layoutScene->addItem(forkXItem);
	forkXItem->setZValue(-1);
	forkYItem=new gItemClass(":dxfs/packingFork/forkY.dxf",QPen(QColor(150,150,175)),false,false,QColor(),this);
	layoutScene->addItem(forkYItem);
	forkYItem->setZValue(-1);
	inputOutputPos					=allPos->getPosWithPlcId(inputOutputPlcId);
	packetFormingStation1Pos=allPos->getPosWithPlcId(packetFormingStation1PlcId);
	packetFormingStation2Pos=allPos->getPosWithPlcId(packetFormingStation2PlcId);
	strappingOldInPos				=allPos->getPosWithPlcId(strappingOldInPlcId);
	strappingOldOutPos			=allPos->getPosWithPlcId(strappingOldOutPlcId);
	strappingNewInPos				=allPos->getPosWithPlcId(strappingNewInPlcId);
	strappingNewOutPos			=allPos->getPosWithPlcId(strappingNewOutPlcId);
	warehouseConveyorPos		=allPos->getPosWithPlcId(warehouseConveyorPlcId);

	clearMissionSendTimer=new QTimer(this);
	connect(clearMissionSendTimer,SIGNAL(timeout()),this,SLOT(clearMissionSendTimerTriggeredSlot()));

#ifdef packingForkExe
	QTimer *missionsTimer=new QTimer(this);
	connect(missionsTimer,SIGNAL(timeout()),this,SLOT(missionsTimerTriggeredSlot()));
	missionsTimer->start(3000);
#endif
}
void forkPackingClass::setAutoMissionsEnabled(bool e)
{
	autoMissionsEnabled=e;
	if (autoMissionsEnabled)
		missionsTimerTriggeredSlot();
}
void forkPackingClass::clearMissionSendTimerTriggeredSlot()
{
	sendedMissionsToPlcAndIsPending=false;
}
bool forkPackingClass::getIsIdle()
{
	if (forkTuxip->getHasError())
		return false;
	plcActiveFromPos=forkTuxip->readIntegerValue("fc1ToPcI",4);
	plcActiveToPos=forkTuxip->readIntegerValue("fc1ToPcI",5);
	plcOkForNewMission=getValueOfBit(forkTuxip->readIntegerValue("fc1ToPcI",12),1);
	bool isPlcIdle=((plcActiveFromPos==0)&&(plcActiveToPos==0)&&(plcOkForNewMission)&&(myPos->getPlcPacketTicket()==0));
	if (!isPlcIdle)
		sendedMissionsToPlcAndIsPending=false;
	return isPlcIdle&&(!sendedMissionsToPlcAndIsPending);
}
bool forkPackingClass::getIsAboutToBeIdle()
{
	if (getValueOfBit(plcStepBits,17))return true;
	if (getValueOfBit(plcStepBits,18))return true;
	if (getValueOfBit(plcStepBits,19))return true;
	return false;
}
bool forkPackingClass::getCanSendMission(posPackingForkClass *from,posPackingForkClass *to)
{
	if (!getIsIdle())return false;
	if(from->getPlcPacketTicket()<=0)return false;
	if(to->getPlcPacketTicket()>0)return false;
	if (to==warehouseConveyorPos)
	{
		int cassetteInPlc=craneTuxip->readIntegerValue("ConveyorBasket",packingBackExport1PlcId-POS_ID_CRANE);
		if (cassetteInPlc==0)
			return false;
		if (!from->getIsPacketDefined())
			return false;
		int cassette=getFirst(QString("select cassette from positions where posId=%1").arg(packingBackExport1PlcId),pDb).toInt();
		if (cassette!=cassetteInPlc)
			return false;
		if (cassette==0)
			return false;
		int packet=getFirst(QString("select packetId from positions where posId=%1").arg(packingBackExport1PlcId),pDb).toInt();
		if (packet!=0)
			return false;
		if (!canCellTakePacketInPosG(cassette,from->getPlcPacketTicket(),pDb))
			return false;
	}
	if ( (to==packetFormingStation1Pos)||(to==packetFormingStation2Pos)||(to==strappingOldInPos)||(to==strappingNewInPos) )
	{
		if (from->getPlcPacketWoods()!=0)
			return false;
	}
	if (from->getPlcPacketLength()==0)return false;
	if ((from==inputOutputPos)&&(from->getPlcPacketWoods()==0)&&((to!=strappingOldInPos)&&(to!=strappingNewInPos)))return false;
	if ((from==inputOutputPos)&&(from->getPlcPacketWoods()!=0)&&(to!=warehouseConveyorPos))return false;
	if ((from==packetFormingStation1Pos)&&(!((to==strappingOldInPos)||(to==strappingNewInPos)||(to==inputOutputPos))))return false;
	if ((from==packetFormingStation2Pos)&&(!((to==strappingOldInPos)||(to==strappingNewInPos)||(to==inputOutputPos))))return false;
	if ((from==strappingOldInPos)&&(!((to==strappingNewInPos)||(to==inputOutputPos))))return false;
	if ((from==strappingNewInPos)&&(!((to==strappingOldInPos)||(to==inputOutputPos))))return false;
	if ((from==strappingOldOutPos)&&(from->getPlcPacketWoods()==0))return false;
	if ((from==strappingOldOutPos)&&(from->getPlcPacketWoods()!=0)&&(!((to==warehouseConveyorPos)||(to==inputOutputPos))))return false;
	if ((from==strappingNewOutPos)&&(from->getPlcPacketWoods()==0))return false;
	if ((from==strappingNewOutPos)&&(from->getPlcPacketWoods()!=0)&&(!((to==warehouseConveyorPos)||(to==inputOutputPos))))return false;
	if (from==warehouseConveyorPos)return false;
	int plcStatusValue=forkTuxip->readIntegerValue("fc1ToPcI",12);
	from->checkPlcStatus(plcStatusValue);
	to->checkPlcStatus(plcStatusValue);
	if (!from->getIsPlcReadyToSendPacket())return false;
	if (!to->getIsPlcReadyToGetPacket())return false;
	return true;
}
void tempLogForkMissions(int from,int fromPacket,int packetNumberOfWoods,int width,int to)
{
	QString logFileName=QString("forkMissionsLog.txt");
	QFile file(logFileName);
	if (!file.open(QIODevice::Append|QIODevice::Text))
		exit(0);
	QTextStream out(&file);
	out<<QString("%1: from %2(%3,%4) to %5 width:%6\n").arg(timeForLog).arg(from).arg(fromPacket).arg(packetNumberOfWoods).arg(to).arg(width);
	file.close();
}

bool forkPackingClass::sendMission(posPackingForkClass *from,posPackingForkClass *to)
{
	if (!getCanSendMission(from,to))
		return false;

	forkTuxip->writeInteger("fc1ToPcI",0,from->getPlcId());
	forkTuxip->writeInteger("fc1ToPcI",1,to->getPlcId());
	int width=0;
	if (to==warehouseConveyorPos)
	{
		width=from->getDbPacketWidth();
		forkTuxip->writeInteger("fc1ToPcI",28,width);
	}
	tempLogForkMissions(from->getPlcId(),from->getPlcPacketTicket(),from->getPlcPacketWoods(),width,to->getPlcId());
	sendedMissionsToPlcAndIsPending=true;
	clearMissionSendTimer->start(3000);
	return true;
}
bool forkPackingClass::sendMission(int from,int to)
{
	return sendMission(allPos->getPosWithPlcId(from),allPos->getPosWithPlcId(to));
}
void forkPackingClass::missionsTimerTriggeredSlot()
{
#ifndef packingForkExe
	return;
#endif
	if (!autoMissionsEnabled)
		return;
	if (!getIsIdle())
		return;
	bool sendedMission=false;
	if (warehouseConveyorPos->getDbHasCassette()&&(warehouseConveyorPos->getDbPacketTicket()==0))
	{
		if ((!sendedMission)&&strappingOldOutPos->getIsPacketDefined()&&(canCellTakePacketInPosG(warehouseConveyorPos->getDbCassette(),strappingOldOutPos->getDbPacketTicket(),pDb)))
			sendedMission=sendMission(strappingOldOutPos,warehouseConveyorPos);
		if ((!sendedMission)&&strappingNewOutPos->getIsPacketDefined()&&(canCellTakePacketInPosG(warehouseConveyorPos->getDbCassette(),strappingNewOutPos->getDbPacketTicket(),pDb)))
			sendedMission=sendMission(strappingNewOutPos,warehouseConveyorPos);
		if ((!sendedMission)&&inputOutputPos->getIsPacketDefined()&&(canCellTakePacketInPosG(warehouseConveyorPos->getDbCassette(),inputOutputPos->getDbPacketTicket(),pDb)))
			sendedMission=sendMission(inputOutputPos,warehouseConveyorPos);
		if (!sendedMission)
		{
			bool strappingOldNeedsCassette=true;
			bool strappingNewNeedsCassette=true;
			bool inOutNeedsCassette=true;
			if ((!strappingOldOutPos->getIsPacketDefined())||(!canCellTakePacketInPosG(warehouseConveyorPos->getDbCassette(),strappingOldOutPos->getDbPacketTicket(),pDb)))
				strappingOldNeedsCassette=false;
			if ((!strappingNewOutPos->getIsPacketDefined())||(!canCellTakePacketInPosG(warehouseConveyorPos->getDbCassette(),strappingNewOutPos->getDbPacketTicket(),pDb)))
				strappingNewNeedsCassette=false;
			if ((!inputOutputPos->getIsPacketDefined())||(!canCellTakePacketInPosG(warehouseConveyorPos->getDbCassette(),inputOutputPos->getDbPacketTicket(),pDb)))
				inOutNeedsCassette=false;

			if ((!strappingOldNeedsCassette)&&(!strappingNewNeedsCassette)&&(!inOutNeedsCassette))
				emit returnEmptyCassetteFromNorthPackingConveyor();
		}
	}
	if ((!sendedMission)&&packetFormingStation1Pos->getHasTicket())
	{
		if ((!sendedMission)&&(packetFormingStation1Pos->getNeedsWrapping())&&(getCanSendMission(packetFormingStation1Pos,strappingNewInPos)))
			sendedMission=sendMission(packetFormingStation1Pos,strappingNewInPos);
		if ((!sendedMission)&&(!packetFormingStation1Pos->getNeedsWrapping())&&(getCanSendMission(packetFormingStation1Pos,strappingOldInPos)))
			sendedMission=sendMission(packetFormingStation1Pos,strappingOldInPos);
	}
	if ((!sendedMission)&&packetFormingStation2Pos->getHasTicket())
	{
		if ((!sendedMission)&&(packetFormingStation2Pos->getNeedsWrapping())&&(getCanSendMission(packetFormingStation2Pos,strappingNewInPos)))
			sendedMission=sendMission(packetFormingStation2Pos,strappingNewInPos);
		if ((!sendedMission)&&(!packetFormingStation2Pos->getNeedsWrapping())&&(getCanSendMission(packetFormingStation2Pos,strappingOldInPos)))
			sendedMission=sendMission(packetFormingStation2Pos,strappingOldInPos);
	}
	if ((!sendedMission)&&packetFormingStation1Pos->getHasTicket())
	{
		if ((!sendedMission)&&(!packetFormingStation1Pos->getNeedsWrapping())&&(getCanSendMission(packetFormingStation1Pos,strappingNewInPos)))
			sendedMission=sendMission(packetFormingStation1Pos,strappingNewInPos);
	}
	if ((!sendedMission)&&packetFormingStation2Pos->getHasTicket())
	{
		if ((!sendedMission)&&(!packetFormingStation2Pos->getNeedsWrapping())&&(getCanSendMission(packetFormingStation2Pos,strappingNewInPos)))
			sendedMission=sendMission(packetFormingStation2Pos,strappingNewInPos);
	}
}
void forkPackingClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane3Ip)
	{
		myPos->dataInPlcChanged(ip,table,index,value);
		if (table=="fc1ToPcF")
		{
			if (index==0)
				plcX=value.toFloat();
			if (index==11)
				plcY=value.toFloat();
			if (index==6)
				plcZ=value.toFloat();
			if (index==16)
				plcR=value.toFloat();
			if ((index==0)||(index==11)||(index==6)||(index==16))
			{
				forkXItem->setPos(0.,-plcX);
				forkYItem->setPos(-plcY,-plcX);
				forkYItem->setRotation(-plcR);
			}
		}
		if (table=="fc1ToPcI")
		{
			if (index==4)
				plcActiveFromPos=value.toInt();
			if (index==5)
				plcActiveToPos=value.toInt();
			if (index==12)
				plcOkForNewMission=getValueOfBit(value.toInt(),1);
			if (index==24)
				plcStepBits=value.toInt();
		}
	}
}

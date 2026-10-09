#include "pch.h"
#include "posPackingFork.h"
posPackingForkClass::posPackingForkClass(int globalId_,tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,int plcIndex_,float plcXPos_,float plcYPos_,float plcZPos_,float angleDegs,QString description_,QGraphicsScene *scene_,gItemClass *posItem_,gItemClass *mouseOverItem_,gItemClass *profilesItem_,gItemClass *fromItem_,gItemClass *toItem_,QWidget *p):QWidget(p)
{
	setVisible(false);
	globalId=globalId_;
	craneTuxip=craneTuxip_;
	forkTuxip=forkTuxip_;
	plcIndex=plcIndex_;
	plcXPos=plcXPos_;
	plcYPos=plcYPos_;
	plcZPos=plcZPos_;
	description=description_;
	rotationAngleDegs=angleDegs;
	scene=scene_;
	isFrom=isTo=false;
	isForPacketId=((plcIndex==inputOutputPlcId)||(plcIndex==strappingOldOutPlcId)||(plcIndex==strappingNewOutPlcId)||(plcIndex==warehouseConveyorPlcId));
	isForRackId=((plcIndex==packetFormingStation1PlcId)||(plcIndex==packetFormingStation2PlcId)||(plcIndex==strappingOldInPlcId)||(plcIndex==strappingNewInPlcId)||(plcIndex==inputOutputPlcId));
#ifndef packingStrapingExe
	fromItem=new gItemClass(fromItem_,0.,0.,angleDegs,this);
	scene->addItem(fromItem);
	toItem=new gItemClass(toItem_,0.,0.,angleDegs,this);
	scene->addItem(toItem);
	profilesItem=new gItemClass(profilesItem_,0.,0.,angleDegs,this);
	scene->addItem(profilesItem);
	mouseOverItem=new gItemClass(mouseOverItem_,0.,0.,angleDegs,this);
	scene->addItem(mouseOverItem);
	mouseOverItem->setCursor(Qt::ClosedHandCursor);
	posItem=new gItemClass(posItem_,0.,0.,angleDegs,this);
	scene->addItem(posItem);

	posItemText=new QGraphicsSimpleTextItem();
	scene->addItem(posItemText);
	posItemText->setZValue(10);

	QTransform txtTransformation;
	txtTransformation.rotate(90-rotationAngleDegs);
	txtTransformation.scale(30,-30);
	posItemText->setTransform(txtTransformation,false);

	setPlcPos(plcXPos,plcYPos,plcZPos);
	connect(mouseOverItem,SIGNAL(mousePressedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedSlot(QGraphicsSceneMouseEvent*)));
#endif
}
void posPackingForkClass::setPlcPos(float x,float y,float z)
{
	plcXPos=x;
	plcYPos=y;
	plcZPos=z;
	gXPos=-plcYPos;
	gYPos=-plcXPos;
	posItem->setPos(gXPos,gYPos);
	mouseOverItem->setPos(gXPos,gYPos);
	profilesItem->setPos(gXPos,gYPos);
	fromItem->setPos(gXPos,gYPos);
	toItem->setPos(gXPos,gYPos);
	if (rotationAngleDegs)
		posItemText->setPos(gXPos-3500,gYPos+200);
	else
		posItemText->setPos(gXPos-200,gYPos-3500);
	updateGui();
}
void posPackingForkClass::updateGui()
{
#ifndef packingStrapingExe
	QString toolTipTxt=QString("%1<br>Position:%2<br>").arg(description).arg(plcIndex);
	toolTipTxt+=QString("Plc Packet id:%1<br>").arg(plcPacketTicket);
	toolTipTxt+=QString("Plc Length:%1<br>").arg(plcPacketLength);
	toolTipTxt+=QString("Plc Woods:%1<br>").arg(plcPacketWoods);
	if (globalId)
	{
		toolTipTxt+=QString("<br>Db Packet id:%1<br>").arg(dbPacketTicket);
		toolTipTxt+=QString("Db Length id:%1<br>").arg(dbPacketLength);
		toolTipTxt+=QString("Db Woods id:%1<br>").arg(dbPacketWoods);
		toolTipTxt+=QString("Db Width id:%1<br>").arg(dbPacketWidth);
		toolTipTxt+=QString("Db Height id:%1<br>").arg(dbPacketHeight);
		toolTipTxt+=QString("Db Weight id:%1<br>").arg(dbPacketWeight);
	}
	posItem->setToolTip(toolTipTxt);
	QString txt;
	if (plcPacketTicket!=0)
		txt=tr("%1").arg(plcPacketTicket);
	else
		txt=tr("%1").arg(getDescription());
	posItemText->setText(txt);
	posItemText->setPen((plcPacketTicket!=0)?QPen(QColor(100,100,200)):QPen(QColor(240,240,240)));
	profilesItem->setVisible(plcPacketTicket);
	fromItem->setVisible(isFrom);
	toItem->setVisible(isTo);
	scene->update();
#endif
}
void posPackingForkClass::loadFromDatabase()
{
	if (globalId)
	{
		QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select (select packetId from positions where posId=%1),length,woods,width,height,weight from packets where packetId in (select packetId from positions where posId=%1)").arg(globalId),pDb);
		if (dataVV.count()==1)
		{
			dbPacketTicket=dataVV[0][0].toInt();
			dbPacketLength=dataVV[0][1].toInt();
			dbPacketWoods=dataVV[0][2].toInt();
			dbPacketWidth=dataVV[0][3].toInt();
			dbPacketHeight=dataVV[0][4].toInt();
			dbPacketWeight=dataVV[0][5].toInt();
		}
	}
}
void posPackingForkClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (crane3Ip==ip)
	{
		if ((table=="packetId")&&(index==plcIndex))
		{
			plcPacketTicket=value.toInt();
			if ((globalId==positionInputOutputPosId)||(globalId==packingOldStrappingExitPosId)||(globalId==packingNewStrappingExitPosId)||(globalId==packingBackExport1PlcId))
				execQuery(QString("update positions set packetId=%1 where posId=%2").arg(value.toInt()).arg(globalId),pDb);
			updateGui();
			loadFromDatabase();
		}
		if ((table=="packetLength")&&(index==plcIndex))
		{
			plcPacketLength=value.toInt();
			updateGui();
		}
		if ((table=="packetWoods")&&(index==plcIndex))
		{
			plcPacketWoods=value.toInt();
			updateGui();
		}
		if (table=="fc1ToPcI")
		{
			if (index==4)
				setIsFrom(plcIndex==value.toInt());
			if (index==5)
				setIsTo(plcIndex==value.toInt());
		}
		if ((table=="fc1ToPcI")&&(index==12))
		{
			checkPlcStatus(value.toInt());
		}
	}
}
void posPackingForkClass::checkPlcStatus(int value)
{
	if (plcIndex==3){isPlcReadyToGetPacket=getValueOfBit( value,11);isPlcReadyToSendPacket=getValueOfBit(value,11);}
	if (plcIndex==4){isPlcReadyToGetPacket=getValueOfBit( value, 7);isPlcReadyToSendPacket=getValueOfBit(value,13);}
	if (plcIndex==5){isPlcReadyToGetPacket=getValueOfBit( value, 8);isPlcReadyToSendPacket=getValueOfBit(value,14);}
	if (plcIndex==6){isPlcReadyToSendPacket=getValueOfBit(value, 9);}
	if (plcIndex==7){isPlcReadyToSendPacket=getValueOfBit(value,10);}
	if (plcIndex==8){isPlcReadyToGetPacket=getValueOfBit( value,12);}
}
bool posPackingForkClass::getHasTicket()
{
	return getPlcPacketTicket()!=0;
}
int posPackingForkClass::getPlcPacketTicket()
{
	return plcPacketTicket;
}
bool posPackingForkClass::getDbHasCassette()
{
	return getDbCassette()!=0;
}
int posPackingForkClass::getDbCassette()
{
	if (globalId==0)
		uExit("Internal Error","posPackingForkClass::getDbPacketTicket");
	return getFirst(QString("select cassette from positions where posId=%1").arg(globalId),pDb).toInt();
}
int posPackingForkClass::getDbPacketTicket()
{
	if (globalId==0)
		uExit("Internal Error","posPackingForkClass::getDbPacketTicket");
	return getFirst(QString("select packetId from positions where posId=%1").arg(globalId),pDb).toInt();
}
bool posPackingForkClass::getNeedsWrapping()
{
	if (isConnectedToEpics)
		return getFirst(QString("select dbo.ufn_getRequiresWrappingByRackID(%1) as NeedStretchWrapping").arg(plcPacketTicket),eDb).toInt()==1;
	else
		return 0;
}
int posPackingForkClass::getPlcPacketLength()
{
	return plcPacketLength;
}
int posPackingForkClass::getPlcPacketWoods()
{
	return plcPacketWoods;
}
int posPackingForkClass::getDbPacketWidth()
{
	return getFirst(QString("select width from packets where packetId=%1").arg(plcPacketTicket),pDb).toInt();
}
bool posPackingForkClass::getIsPacketDefined()
{
	if ((plcPacketTicket<minTicketIdId)||(plcPacketTicket>maxTicketIdId))
		return false;
	int dbTicketId=getDbPacketTicket();
	if (plcPacketTicket!=dbTicketId)
		return false;
	QVector<QVector<QVariant>> packetVV=execTableQuery(QString("select packetId,woods,length from packets where packetId=%1").arg(plcPacketTicket),pDb);
	if (packetVV.count()==0)
		return false;
	if (packetVV[0][1]==0)
		return false;
	if (packetVV[0][2]==0)
		return false;

	return true;
}
bool posPackingForkClass::getIsPlcReadyToGetPacket()
{
	return isPlcReadyToGetPacket;
}
bool posPackingForkClass::getIsPlcReadyToSendPacket()
{
	return isPlcReadyToSendPacket;
}
void posPackingForkClass::setPacketTicket(int p)
{
}
void posPackingForkClass::mousePressedSlot(QGraphicsSceneMouseEvent *ev)
{
	if ((ev->button()==Qt::LeftButton)||(ev->button()==Qt::RightButton))
	{
		if (ev->modifiers()==Qt::ControlModifier)
		{
			showPositionDlg();
			emit userChangedBasketSignal(this);
		}
		else
			emit posMouseReleasedSignal(this,ev);
	}
}
void posPackingForkClass::showPositionDlgSlot()
{
	showPositionDlg();
}
void posPackingForkClass::showPositionDlg()
{
	loadFromDatabase();
	posDlgPackingForkClass *posDlg=new posDlgPackingForkClass(getDescription(),getPlcPacketTicket(),isForPacketId,isForRackId,this);
	if (!isForPacketId)
		posDlg->setData(plcPacketTicket,plcPacketLength,plcPacketWoods,0,0,0);
	if (posDlg->myExec(false))
	{
		forkTuxip->writeInteger("packetId",plcIndex,posDlg->getPacketTicket());
		forkTuxip->writeInteger("packetLength",plcIndex,posDlg->getPacketLength());
		if (isForPacketId)
			forkTuxip->writeInteger("packetWoods",plcIndex,posDlg->getPacketWoods());
		if (globalId==packingBackExport1PlcId)
			craneTuxip->writeInteger("ConveyorData",45,posDlg->getPacketTicket());

	}
	delete posDlg;
}
void posPackingForkClass::setIsFrom(bool s)
{
	isFrom=s;
	updateGui();
}
bool posPackingForkClass::getIsFrom()
{
	return isFrom;
}
void posPackingForkClass::setIsTo(bool s)
{
	isTo=s;
	updateGui();
}
QString posPackingForkClass::getDescription() const
{
	return QString("%1 (%2)").arg(description).arg(plcIndex);
}
int posPackingForkClass::getPlcId() const
{
	return plcIndex;
}
int posPackingForkClass::getGlobalId() const
{
	return globalId;
}

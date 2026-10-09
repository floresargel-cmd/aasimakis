#include "pch.h"
#include "p.h"
extern tuxipClass *tuxIpCraneGlobalPtr;
packetPositionClass::packetPositionClass(int posId_,QString name_,int packetId_,bool isOnSb_,int isLocked_,int acceptableContents_,int acceptableLength_,QGraphicsScene *scene,dxfClass *mouseOverItem_
	,dxfClass *lockedItem_,dxfClass *sbItem_,QWidget *p):QWidget(p)
{
	QWidget::hide();
	posId=posId_;
	if (posId==1)
		double a=0.;
	name=name_;
	packetId=packetId_;
	isLocked=isLocked_;
	acceptableContents=acceptableContents_;
	acceptableLength=acceptableLength_;
	packetNoOfWoods=packetLength=packetWidth=packetHeight=packetDestination=0;
	storagingTypeOfPacket=STORAGE_TYPE_SHORT_TERM;
	isOnSb=isOnSb_;
	isSelected=false;
	hasWrongPacket=false;
	mouseOverItem=new dxfClass(mouseOverItem_,0.,0.,this);scene->addItem(mouseOverItem);mouseOverItem->setCursor(Qt::ClosedHandCursor);
	lockedItem=new dxfClass(lockedItem_,0.,0.,this);scene->addItem(lockedItem);lockedItem->setCursor(Qt::ClosedHandCursor);
	sbItem=new dxfClass(sbItem_,0.,0.,this);scene->addItem(sbItem);sbItem->setCursor(Qt::ClosedHandCursor);
	packetItem=new QGraphicsRectItem();
	packetItem->setRect(QRectF(-1,1,2,2));scene->addItem(packetItem);
	packetItem->setPen(QPen(QColor(50,150,50)));

	graphicsItemText=new QGraphicsSimpleTextItem();
	scene->addItem(graphicsItemText);
	graphicsItemText->setZValue(10);
	QTransform txtTransformation;
	txtTransformation.scale(15,-15);
	graphicsItemText->setTransform(txtTransformation,false);
	graphicsItemText->setPen(QPen(QColor(100,100,175)));
	connect(mouseOverItem,SIGNAL(mousePressedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedSlot(QGraphicsSceneMouseEvent*)));
	if (posId>MAX_CELL_POS_ID)
	{
		mouseOverItem->setZValue(10);
		lockedItem->setZValue(10);
		sbItem->setZValue(10);
		packetItem->setZValue(10);
	}
}
packetPositionClass::~packetPositionClass()
{
}
/////////////////////////////////////////////////////////////////////////////////////
void packetPositionClass::setZValue(int v)
{
	mouseOverItem->setZValue(v);
	lockedItem->setZValue(v);
	sbItem->setZValue(v);
	packetItem->setZValue(v);
}
extern QVector<int> globalPosToPush;
void packetPositionClass::mousePressedSlot(QGraphicsSceneMouseEvent *ev)
{
	if ((ev->modifiers()==Qt::ShiftModifier)&&(ev->button()==Qt::LeftButton))
	{
		apexExecQuery(QString("update positions set isLocked=%1 where posId=%2").arg(isLocked?IS_LOCKED:0).arg(posId));
		loadFromDatabase();
		return;
	}
	if ((ev->button()==Qt::LeftButton)&&(ev->modifiers()&Qt::ControlModifier)&&(ev->modifiers()&Qt::ShiftModifier))
	{
#ifdef tempForMove
		if (false)
		{
			if (acceptableContents==CENTER_ONLY_PACKET_PLACING_CASSETTE)
				apexExecQuery(QString("update positions set acceptableContents=%1,acceptableLength=%2 where posId=%3").arg(LEFT_AND_CENTER_PACKET_PLACING_CASSETTE).arg(bigLength).arg(posId));
			else
				apexExecQuery(QString("update positions set acceptableContents=%1,acceptableLength=%2 where posId=%3").arg(CENTER_ONLY_PACKET_PLACING_CASSETTE).arg(smallLength).arg(posId));
			emit packetDataChangedSignal(posId);
		}
		if (true)
		{
			if ((packetId==0)||(canCellTakePacketInPos(posId,packetId)))
			{
				if (globalPosToPush.contains(posId))
					globalPosToPush.removeAll(posId);
				else
					globalPosToPush<<posId;
			}
		}
#endif
	}
	else if ((ev->button()==Qt::LeftButton)&&(ev->modifiers()&Qt::ControlModifier))
		emit mouseReleasedSignal(this,ev);
	else if (ev->button()==Qt::RightButton)
		showDlg();
}
void packetPositionClass::showDlg()
{
#ifndef _DEBUG
	passwordClass *pass=new passwordClass(this);
	pass->exec();
	if (pass->getPasswordIsCorrect())
#endif
	{
		if (
			(!((posId>=packingExport2PlcId)&&(posId<=packingBackExport1PlcId)))
			&&
			(!((posId>=packingOldStrappingExitPosId)&&(posId<=positionInputOutputPosId)))
			)
		{
			cellContentsClass *tCellContents=new cellContentsClass(posId,this);
			connect(tCellContents,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onPosDataChangedSlot(int)));
			connect(tCellContents,SIGNAL(cassetteChangedSignal(int)),this,SLOT(cassetteChangedSlot(int)));
			tCellContents->exec();
		}
	}
}
void packetPositionClass::onPosDataChangedSlot(int pos)
{
	emit packetDataChangedSignal(pos);
}
void packetPositionClass::cassetteChangedSlot(int cassette)
{
	emit cassetteChangedSignal(cassette);
}
int packetPositionClass::getNumberOfWoods()
{
	return packetNoOfWoods;
}
int packetPositionClass::getPosId()
{
	return posId;
}
bool packetPositionClass::getIsLocked()
{
	return isLocked;
}
bool packetPositionClass::getIsOnSB()
{
	return isOnSb;
}
void packetPositionClass::updateTxt()
{
	if (packetId!=0)
	{
		graphicsItemText->setVisible(true);
		graphicsItemText->setText(tr("%1").arg(packetId));
	}
	else
		graphicsItemText->setVisible(false);
}
void packetPositionClass::setGXY(float gXPos,float gYPos)
{
	graphicsItemText->setPos(gXPos-0.5*graphicsItemText->boundingRect().width()*15,gYPos+0.5*lockedItem->boundingRect().height());
	mouseOverItem->setPos(gXPos,gYPos);
	lockedItem->setPos(gXPos,gYPos);
	sbItem->setPos(gXPos,gYPos);
	packetItem->setPos(gXPos,gYPos+60.);
}
void packetPositionClass::setIsSelected(bool s)
{
	isSelected=s;
	updateGui(parentToolTip);
}
void packetPositionClass::updateGui(QString parentToolTip_)
{
	if (posId==packingBackExport1PlcId)
		double a=0.;
	parentToolTip=parentToolTip_;
	lockedItem->setVisible(isLocked||isSelected||hasWrongPacket);
	if (isSelected)
		lockedItem->setPen(QPen(QColor(0,150,0)));
	else if (hasWrongPacket)
		lockedItem->setPen(QPen(QColor(150,0,0),20));
	else
		lockedItem->setPen(QPen(QColor(150,0,0)));
	sbItem->setVisible(isOnSb&&(packetId!=0));
	packetItem->setVisible(packetId>0);
	if (packetId>0)
		packetItem->setRect(QRectF(-0.5*packetWidth,0.f,packetWidth,packetHeight));

	if (packetLength<smallLengthForEvenWoods)
	{
		packetItem->setPen(QPen(QColor(100,200,200),0,Qt::DashLine));
		sbItem->setPen(QPen(QColor(100,200,200),0,Qt::DashLine));
		graphicsItemText->setPen(QPen(QColor(100,200,200),0,Qt::DashLine));
	}
	else if (packetLength<smallLength)
	{
		if (getPacketPlacingType()==CENTER_PLACED_PACKET)
		{
			packetItem->setPen(QPen(QColor(100,200,200),0,Qt::DashLine));
			sbItem->setPen(QPen(QColor(100,200,200),0,Qt::DashLine));
			graphicsItemText->setPen(QPen(QColor(100,200,200),0,Qt::DashLine));
		}
		if (getPacketPlacingType()==LEFT_PLACED_PACKET)
		{
			packetItem->setPen(QPen(QColor(100,100,200),0,Qt::SolidLine));
			sbItem->setPen(QPen(QColor(100,100,200),0,Qt::SolidLine));
			graphicsItemText->setPen(QPen(QColor(100,100,200),0,Qt::SolidLine));
		}
	}
	else //bigLength
	{
		packetItem->setPen(QPen(QColor(100,200,100),0,Qt::SolidLine));
		sbItem->setPen(QPen(QColor(100,200,100),0,Qt::SolidLine));
		graphicsItemText->setPen(QPen(QColor(100,200,100),0,Qt::SolidLine));
	}
	QString toolStr=tr("Possition:%1(%2)").arg(posId).arg(name);
	toolStr+=tr("\nPacket:%1").arg(packetId);
	toolStr+=tr("(W:%1mm H:%2mm L:%3mm)").arg(packetWidth).arg(packetHeight).arg(packetLength);
	if (isOnSb)
		toolStr+=tr("On shipping basket");
	else
		toolStr+=tr("\nWoods:%1").arg(packetNoOfWoods);
	toolStr+=tr("\nLocked:%1").arg(isLocked?tr("yes"):tr("no"));
	toolStr+=tr("\nEven woods have max length:%1").arg(acceptableContents==CENTER_ONLY_PACKET_PLACING_CASSETTE?smallLengthForEvenWoods:acceptableLength);
	toolStr+=tr("\nAccepts packets up to %1mm length").arg(acceptableLength);
	toolStr+=parentToolTip;
	mouseOverItem->setToolTip(toolStr);
	updateTxt();
}
void packetPositionClass::setPacketId(int p,bool check)
{
	tempLogPackingStationG("packetPositionClass",posId,p);
	vLog("info","packetPositionClass::setPacketId()",QString("id:%1,packet:%2").arg(posId).arg(p));
	apexExecQuery(QString("insert into log (type,posId,newValue,oldValue,date,sec) values('define packet',%1,%2,%3,'%4',%5)").arg(posId).arg(p).arg(packetId).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	if (check&&(p!=0)&&(packetId!=0)&&(p!=packetId))
	{
		vLog("Error","packetPositionClass::setPacketId()",QString("Attempt to define packetId %1 in position %2. The position allready has packetId %3").arg(p).arg(posId).arg(packetId));
		uExit(tr("Error"),tr("Attempt to define packetId %1 in position %2. The position allready has packetId %3").arg(p).arg(posId).arg(packetId));
	}
	packetId=p;
	saveToDatabase();
	loadFromDatabase();
	updateGui(parentToolTip);
}
void packetPositionClass::updatePacketIdInPlc(tuxipClass *tuxip)
{
	if ((posId>=packingExport2PlcId)&&(posId<=packingBackExport1PlcId))
		tuxip->writeInteger("ConveyorData",posId-POS_ID_CRANE,packetId);
}
int packetPositionClass::getPacketTypeForPlc()
{
	if (isOnSb)
		return 9;
	else
	{
		if ((packetLength<2743)&&(posId!=POS_ID_SB_PACKET_IMPORT))
			return 1;
		else
			return packetNoOfWoods;
	}
}
int packetPositionClass::getPositionIdForForkPlc()
{
#ifdef keepPacketCraneOldFunctionality
	if (posId==POS_ID_NEWSB)return 6;
	if (posId==POS_ID_NEWPACKET)return 8;
#endif
	if (posId==POS_ID_SB_PACKET_IMPORT)return 9;
	if (posId==POS_ID_WESTCONV)return 24;
	if (posId==POS_ID_EASTCONV)return 25;
#ifdef keepPacketCraneOldFunctionality
	if (posId==POS_ID_SBEXIT)return 21;
	//if (posId==POS_ID_PACKETEXIT)return 22;
	if (posId==POS_ID_PACKETEXIT)return 9;
	if (posId==POS_ID_STOREEXIT)return 23;
#endif
	return 0;
}
int packetPositionClass::getPositionIdForCranePlc()
{
	return posId;
}
int packetPositionClass::getPacketId(bool loadFromDataBase)
{
	if (loadFromDataBase)
		loadFromDatabase();
	return packetId;
}
int packetPositionClass::getHasPacketReadyForWarehouse(bool loadFromDataBase)
{
	if (loadFromDataBase)
		loadFromDatabase();
	return (getPacketId()!=0)&&getIsPacketDefined()&&(packetNoOfWoods);
}
int packetPositionClass::getPacketDestination()
{
	return packetDestination;
}
int packetPositionClass::getStoragingTypeOfPacket()
{
	return storagingTypeOfPacket;
}
int packetPositionClass::getPacketLength()
{
	return packetLength;
}
int packetPositionClass::getPacketPlacingType()
{
	if ((packetNoOfWoods==2)||(packetNoOfWoods==4))
		return LEFT_PLACED_PACKET;
	if ((packetNoOfWoods==3)||(packetNoOfWoods==5))
		return CENTER_PLACED_PACKET;
	if (isOnSb)
		return CENTER_PLACED_PACKET;
	return -1;
}
bool packetPositionClass::hasPacket()
{
	return (packetId!=0);
}
bool packetPositionClass::getIsPacketDefined()
{
	if (packetId<100000)
		return false;
	return getFirst(QString("select count(tId) from packets where packetId=%1").arg(packetId),MAIN_DB_CONNECTION).toInt()==1;
}
void packetPositionClass::saveToDatabase()
{
	apexExecQuery(QString("update positions set packetId=%1 where posId=%2").arg(packetId).arg(posId));
}
//void packetPositionClass::loadContentsFromDatabase()
//{
//
//}
void packetPositionClass::loadFromDatabase()
{
//	packetId=GETONEATMOST(QString("select packetId from positions where posId=%1").arg(posId)).toInt();
	QVector<QVector<QVariant>> posVV=execTableQuery(QString("select packetId,acceptableContents,isLocked,acceptableLength from positions where posId=%1").arg(posId),MAIN_DB_CONNECTION);
	if (posVV.count()==1)
	{
		packetId=posVV[0][0].toInt();
		acceptableContents=posVV[0][1].toInt();
		isLocked=(posVV[0][2].toInt()==IS_LOCKED?true:false);
		acceptableLength=posVV[0][3].toInt();
	}
	if (packetId!=0)
	{
		QVector<QVector<QVariant>> dd=execTableQuery(QString("select woods,length,width,height,sb,destination,storagingType from packets where packetId=%1").arg(packetId),MAIN_DB_CONNECTION);
		if (dd.count()==1)
		{
			packetNoOfWoods=dd[0][0].toInt();
			packetLength=dd[0][1].toInt();
			packetWidth=dd[0][2].toInt();
			packetHeight=dd[0][3].toInt();
			isOnSb=(dd[0][4].toInt()==PACKET_WITH_SB);
			packetDestination=dd[0][5].toInt();
			storagingTypeOfPacket=dd[0][6].toInt();
		}
		else
		{
			if ((posId!=packingOldStrappingExitPosId)&&(posId!=packingNewStrappingExitPosId)&&(posId!=positionInputOutputPosId))//new packets
				uExit("Error",QString("packet %1 is not defined in packets table").arg(packetId));
		}
	}
	else
	{
		packetNoOfWoods=packetLength=packetWidth=packetHeight=packetDestination=storagingTypeOfPacket=storagingTypeOfPacket=0;
		isOnSb=false;
	}

	updateGui(parentToolTip);
}
/////////////////////////////////////////////////////////////////////////////////////////////
int packetPositionClass::getAcceptableContents()
{
	return acceptableContents;
}
int packetPositionClass::getAcceptableLength()
{
	return acceptableLength;
}

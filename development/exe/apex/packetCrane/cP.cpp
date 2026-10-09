#include "pch.h"
#include "cP.h"
double textScale=15.;
cellPositionClass::cellPositionClass(int posId,QString name,int packetId,bool isOnSb,int cassette_,int acceptableContents,int storagingTypeOfCassette_,int isLocked,int col_,int row_,int xCorr_,int zCorr_,int acceptableLength
	,QGraphicsScene *scene_,dxfClass *cellItem_,dxfClass *mouseOverItem_,dxfClass *lockedItem_,dxfClass *cassetteItem_,dxfClass *sbItem_,dxfClass *fromItem_,dxfClass *toItem_,QWidget *p):
	packetPositionClass(posId,name,packetId,isOnSb,isLocked,acceptableContents,acceptableLength,scene_,mouseOverItem_,lockedItem_,sbItem_,p)
{
	isPlcPos=false;
	cassette=cassette_;
	storagingTypeOfCassette=storagingTypeOfCassette_;
	row=row_;
	col=col_;
	xCorr=xCorr_;
	zCorr=zCorr_;
	scene=scene_;
	//
	cellItem=new dxfClass(cellItem_,0.,0.,this);scene->addItem(cellItem);
	cassetteItem=new dxfClass(cassetteItem_,0.,0.,this);scene->addItem(cassetteItem);cassetteItem->setCursor(Qt::ClosedHandCursor);
	if (fromItem_)
	{
		fromItem=new dxfClass(fromItem_,0.,0.,this);
		fromItem->setFill(true,QColor(0,0,200));
		scene->addItem(fromItem);
		fromItem->setVisible(false);
	}
	if (toItem_)
	{
		toItem=new dxfClass(toItem_,0.,0.,this);
		toItem->setFill(true,QColor(0,200,0));
		scene->addItem(toItem);
		toItem->setVisible(false);
	}
	//
	isCasseteOnCell=cassette!=0?true:false;
	xOffset=0.f;
	yOffset=0.f;
	plcXPos=0.f;
	plcYPos=0.f;
	plcZPos=0.f;
	if (posId<MAX_CELL_POS_ID)
	{
		yOffset=(posId>0)?(165.f+4930.f):(165.f-14080.f);
		plcXPos=1172+(col-1)*821;
		plcZPos=(row-1)*740;
	}
	else
	{
		cellItem->setZValue(10);
		cassetteItem->setZValue(10);
		if (fromItem)
			fromItem->setZValue(10);
		if (toItem)
			toItem->setZValue(10);
	}
	graphicsItemText=new QGraphicsSimpleTextItem();
	scene->addItem(graphicsItemText);
	graphicsItemText->setZValue(10);
	QTransform txtTransformation;
	txtTransformation.scale(textScale,-textScale);
	graphicsItemText->setTransform(txtTransformation,false);
	graphicsItemText->setText(QString("%1").arg(posId));
	graphicsItemText->setPen(QPen(QColor(175,175,175)));
	setPlcPos(plcXPos,plcYPos,plcZPos);
	connect(this,SIGNAL(packetDataChangedSignal(int)),this,SLOT(onPacketDataChangedSlot(int)));
//#ifndef _DEBUG
	///////////loadFromDatabase();
	packetPositionClass::hasWrongPacket=!canCellTakePacketInPosG(acceptableContents,acceptableLength,isLocked,packetNoOfWoods,packetLength,isOnSb);
//#endif
	setPens();
	updateGui();
}
cellPositionClass::~cellPositionClass()
{
}
void cellPositionClass::setPlcXCorrection(int c)
{
	execQuery(QString("update positions set xCorr=%1 where posId=%2").arg(c).arg(posId),MAIN_DB_CONNECTION);
	loadFromDatabase();
}
void cellPositionClass::setPlcZCorrection(int c)
{
	execQuery(QString("update positions set zCorr=%1 where posId=%2").arg(c).arg(posId),MAIN_DB_CONNECTION);
	loadFromDatabase();
}

void cellPositionClass::setPens()
{
	int cassetteAcceptableContents=acceptableContents;
	int cassetteAcceptableLength=acceptableLength;
	if (cassette!=0)
	{
		QVector<QVector<QVariant>> posVV=execTableQuery(QString("select acceptableContents,acceptableLength from positions where posId=%1").arg(cassette),MAIN_DB_CONNECTION);
		if (posVV.count()==1)
		{
			cassetteAcceptableContents=posVV[0][0].toInt();
			cassetteAcceptableLength=posVV[0][1].toInt();
		}
	}
	if (cassetteAcceptableContents==LEFT_AND_CENTER_PACKET_PLACING_CASSETTE)
	{
		if (cassetteAcceptableLength==bigLength)
		{
			cellItem->setPen(QPen(QColor(100,200,100),0,Qt::SolidLine));
			cassetteItem->setPen(QPen(QColor(100,200,100),0,Qt::SolidLine));
			graphicsItemText->setPen(QPen(QColor(100,200,100),0,Qt::SolidLine));
		}
		else
		{
			cellItem->setPen(QPen(QColor(100,100,200),0,Qt::SolidLine));
			cassetteItem->setPen(QPen(QColor(100,100,200),0,Qt::SolidLine));
			graphicsItemText->setPen(QPen(QColor(100,100,200),0,Qt::SolidLine));
		}
	}
	else
	{
		if (cassetteAcceptableLength==bigLength)
		{
			cellItem->setPen(QPen(QColor(100,200,100),0,Qt::DotLine));
			cassetteItem->setPen(QPen(QColor(100,200,100),0,Qt::DotLine));
			graphicsItemText->setPen(QPen(QColor(100,200,100),0,Qt::DotLine));
		}
		else
		{
			cellItem->setPen(QPen(QColor(100,200,200),0,Qt::SolidLine));
			cassetteItem->setPen(QPen(QColor(100,200,200),0,Qt::SolidLine));
			graphicsItemText->setPen(QPen(QColor(100,200,200),0,Qt::SolidLine));
		}
	}
}
void cellPositionClass::setZValue(int v)
{
	cellItem->setZValue(v);
	cellItem->setZValue(v);
	packetPositionClass::setZValue(v);
}
void cellPositionClass::onPacketDataChangedSlot(int cell)
{
	loadFromDatabase();
}
int cellPositionClass::getRow()
{
	return row;
}
int cellPositionClass::getColumn()
{
	return col;
}
void cellPositionClass::setPlcPos(float x,float y,float z)
{
	plcXPos=x;
	plcYPos=y;
	plcZPos=z;
	gXPos=plcXPos;
	gYPos=plcZPos+yOffset;
	cellItem->setPos(gXPos,gYPos);
	cassetteItem->setPos(gXPos,gYPos);
	if (fromItem)
		fromItem->setPos(gXPos,gYPos);
	if (toItem)
		toItem->setPos(gXPos,gYPos);
	graphicsItemText->setPos(gXPos-0.5*graphicsItemText->boundingRect().width()*textScale,gYPos+0.5*lockedItem->boundingRect().height());
	packetPositionClass::updateTxt();
	packetPositionClass::setGXY(gXPos,gYPos);
}
void cellPositionClass::saveToDatabase()
{
	packetPositionClass::saveToDatabase();
}
void cellPositionClass::loadFromDatabase()
{
	packetPositionClass::loadFromDatabase();
	QVector<QVector<QVariant>> dVV=execTableQuery(QString("select cassette,xCorr,zCorr from positions where posId=%1").arg(packetPositionClass::getPosId()),MAIN_DB_CONNECTION);
	if (dVV.count()==1)
	{
		cassette=dVV[0][0].toInt();
		xCorr=dVV[0][1].toInt();
		zCorr=dVV[0][2].toInt();
		isCasseteOnCell=cassette!=0?true:false;
		packetPositionClass::hasWrongPacket=!canCellTakePacketInPosG(cassette,packetPositionClass::getPacketId());
	}
	setPens();
	updateGui();
}
int cellPositionClass::getXCorr()
{
	return xCorr;
}
int cellPositionClass::getZCorr()
{
	return zCorr;
}
float cellPositionClass::getCorrectedPlcX()
{
	return plcXPos+xCorr;
}
float cellPositionClass::getCorrectedPlcZ()
{
	return plcZPos+zCorr;
}

void cellPositionClass::setIsFrom(bool is)
{
	fromItem->setVisible(is);
}
void cellPositionClass::setIsTo(bool is)
{
	toItem->setVisible(is);
}
void cellPositionClass::setCassette(int cas,bool updatePlc,bool isFromPlc)
{
	tempLogPackingStationG("cellPositionClass",posId,cas);
	apexExecQuery(QString("insert into log (type,posId,newValue,oldValue,date,sec) values('define cassette',%1,%2,%3,'%4',%5)").arg(packetPositionClass::getPosId()).arg(cas).arg(cassette).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	if (!isPlcPos)
	{
		if ((cassette!=0)&&(cas!=0)&&(cassette!=cas))
		{
			vLog("error","cellPositionClass::setCassette()",QString("Attempt to set cassette %1 on position %2. Position %2 allready has cassette %3").arg(cas).arg(packetPositionClass::getPosId()).arg(cassette));
			uExit("internal error",QString("Attempt to set cassette %1 on position %2. Position %2 allready has cassette %3").arg(cas).arg(packetPositionClass::getPosId()).arg(cassette));
		}
	}
	if (updatePlc&&isPlcPos)
		tuxip->writeInteger(plcTable,plcIndex,cas);
	apexExecQuery(QString("update positions set cassette=%1 where posId=%2").arg(cas).arg(packetPositionClass::getPosId()));
	vLog("info","cellPositionClass::setCassette()",QString("id:%1,cass:%2").arg(packetPositionClass::getPosId()).arg(cas));
	if (!isFromPlc)
	{
		if (cas!=0)
			packetPositionClass::setPacketId(getFirst(QString("select packetId from positions where cassette=%1").arg(cas),MAIN_DB_CONNECTION).toInt(),!isPlcPos);
		else
			packetPositionClass::setPacketId(0);
	}
	loadFromDatabase();
}
int cellPositionClass::getCassette()
{
	return cassette;
}
int cellPositionClass::getStoragingTypeOfCassette()
{
	return storagingTypeOfCassette;
}
bool cellPositionClass::hasCassette()
{
	return cassette!=0;
}
void cellPositionClass::updateGui()
{
	QString toolStr=tr("\nCassette:%1").arg(cassette);
	toolStr+=tr("\nCol:%1 Row:%2").arg(col).arg(row);
	toolStr+=tr("\nX:%1 Z:%2").arg(plcXPos).arg(plcZPos);
	toolStr+=tr("\nx corr:%1mm z corr:%2mm").arg(xCorr).arg(zCorr);
	packetPositionClass::updateGui(toolStr);
	cassetteItem->setVisible(cassette!=0);
	graphicsItemText->setText(QString("%1").arg(cassette));
	graphicsItemText->setVisible((cassette!=0)&&(packetPositionClass::packetId==0));
	//if (cassetteOutItem)
	//	cassetteOutItem->setVisible(cassette==0);
	scene->update();
}
void cellPositionClass::setPlcData(tuxipClass *tuxip_,QString plcIp_,QString plcTable_,int plcIndex_)
{
	isPlcPos=true;
	tuxip=tuxip_;
	plcIp=plcIp_;
	plcTable=plcTable_;
	plcIndex=plcIndex_;
}
void cellPositionClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((posId==packingExport2PlcId)||(posId==packingBackExport2PlcId)||(posId==packingBackImportPlcId)||(posId==packingImportPlcId)||(posId==packingExport1PlcId))
	{
		if (isPlcPos&&(plcIp==ip)&&(plcTable==table)&&(plcIndex==index))
			setCassette(value.toInt(),false,true);
		if (isPlcPos&&(ip==plcIp)&&(table=="ConveyorData")&&(plcIndex==index))
			setPacketId(value.toInt(),false);
	}
	if (posId==packingBackExport1PlcId)
	{
		if (isPlcPos&&(plcIp==ip)&&(plcTable==table)&&(plcIndex==index))
			setCassette(value.toInt(),false,true);
		if (isPlcPos&&(ip==crane3Ip)&&(table=="packetId")&&(index==8))
			setPacketId(value.toInt(),false);
	}
}
void cellPositionClass::cassetteChangedSlot(int cassette)
{
	setCassette(cassette,true);
}

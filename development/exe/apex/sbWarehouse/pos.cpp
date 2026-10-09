#include "pch.h"
#include "pos.h"
double posClass::textScale=15.;
#include "allPos.h"
posClass::posClass(int posId_,bool isSbPosition_,int cassetteNum_,QString description_,int column_,int row_,float plcXPos_,float plcYPos_,float plcZPos_,float xCorr_,float yCorr_,float zCorr_,bool isLocked_,int sbNumber_,int ticket_,float ticketWeight_,
	gItemClass *mouseOveItem_,gItemClass *posItem_,gItemClass *cassetteItem_,gItemClass *sbBlueItem_,gItemClass *profilesItem_,gItemClass *lockedItem_,gItemClass *activeFromItem_,gItemClass *activeToItem_,bool handleMousePressed,QWidget *p):QWidget(p)
{
	setVisible(false);
	tuxip=NULL;
	posId=posId_;
	isSbPosition=isSbPosition_;
	description=QString("%1 (%2)").arg(posId).arg(description_.trimmed());
	column=column_;
	row=row_;
	plcXPos=plcXPos_;
	plcYPos=plcYPos_;
	plcZPos=plcZPos_;
	plcXCorrection=xCorr_;
	plcYCorrection=yCorr_;
	plcZCorrection=zCorr_;
	cassetteNum=cassetteNum_;
	isLocked=isLocked_;
	isSelected=false;
	isActiveFrom=isActiveTo=false;
	sbNumber=sbNumber_;
	ticket=ticket_;
	ticketWeight=ticketWeight_;
	xOffset=0.f;
	yOffset=0.f;
	if (posId<craneId)//cells
	{
		if (posId<0)
			yOffset=layoutYMinusOffset;
		else
			yOffset=layoutYPlusOffset;
	}
	posItem=NULL;
	if (posItem_)
	{
		posItem=new gItemClass(posItem_,0.,0.);
		scene->addItem(posItem);
	}
	cassetteItem=NULL;
	if (!isSbPosition)
	{
		cassetteItem=new gItemClass(cassetteItem_,0.,0.);
		scene->addItem(cassetteItem);
	}
	sbBlueItem=new gItemClass(sbBlueItem_,0.,0.);scene->addItem(sbBlueItem);
	profilesItem=new gItemClass(profilesItem_,0.,0.);scene->addItem(profilesItem);
	lockedItem=new gItemClass(lockedItem_,0.,0.);scene->addItem(lockedItem);
	activeFromItem=new gItemClass(activeFromItem_,0.,0.);scene->addItem(activeFromItem);activeFromItem->setVisible(false);
	activeToItem=new gItemClass(activeToItem_,0.,0.);scene->addItem(activeToItem);activeToItem->setVisible(false);
	mouseOveItem=new gItemClass(mouseOveItem_,0.,0.);scene->addItem(mouseOveItem);
	posTollTipStr=tr("position:%1<br>plc id:%2<br>").arg(description).arg(posId);
	if (column!=0)
		posTollTipStr+=tr("column:%1,row:%2<br>x:%3mm,y:%4mm,z:%5mm<br>").arg(column).arg(row).arg(plcXPos).arg(plcYPos).arg(plcZPos);

	graphicsItemText=new QGraphicsSimpleTextItem();
	scene->addItem(graphicsItemText);
	graphicsItemText->setZValue(10);
	graphicsItemText->setTransform(txtTransformation,false);
	//graphicsItemText->setPen(QPen(QColor(75,50,0)));
	graphicsItemText->setBrush(QBrush(QColor(175,125,75)));

	setPlcPos(plcXPos,plcYPos,plcZPos);
	if (handleMousePressed)
		connect(mouseOveItem,SIGNAL(mousePressedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedSlot(QGraphicsSceneMouseEvent*)));
	//
	updateGui();
}
posClass::~posClass()
{
}
void posClass::setXOffset(float x)
{
	xOffset=x;
}
void posClass::setYOffset(float y)
{
	yOffset=y;
	setPlcPos(plcXPos,plcYPos,plcZPos);
}
void posClass::setPlcPos(float x,float y,float z)
{
	plcXPos=x;
	plcYPos=y;
	plcZPos=z;
	gXPos=plcXPos+xOffset+plcXCorrection;
	gYPos=plcZPos+yOffset+plcZCorrection;
	if (posItem)
		posItem->setPos(gXPos,gYPos);
	mouseOveItem->setPos(gXPos,gYPos);
	if (cassetteItem)
		cassetteItem->setPos(gXPos,gYPos);
	sbBlueItem->setPos(gXPos,gYPos);
	profilesItem->setPos(gXPos,gYPos);
	lockedItem->setPos(gXPos,gYPos);
	activeFromItem->setPos(gXPos,gYPos);
	activeToItem->setPos(gXPos,gYPos);
	updateTxt();
}
void posClass::setPlcAdress(tuxipClass *tuxip_,int plcIndex_)
{
	tuxip=tuxip_;
	if (plcIndex_>=0)
		plcIndex=plcIndex_;
	else
		plcIndex=(posId-craneId);
}
QPointF posClass::getGPos()
{
	return QPointF(gXPos,gYPos);
}
void posClass::updateTxt()
{
	if (ticket!=0)
		graphicsItemText->setText(tr("%1").arg(ticket));
	else if (sbNumber!=0)
		graphicsItemText->setText(tr("%1").arg(sbNumber));
	else if (cassetteNum!=0)
		graphicsItemText->setText(tr("%1").arg(cassetteNum));
	else
		graphicsItemText->setText(QString("%1").arg(posId>craneId?posId-craneId:posId));
	graphicsItemText->setPos(gXPos-0.5*graphicsItemText->boundingRect().width()*textScale,gYPos+graphicsItemText->boundingRect().height()*textScale);
}
void posClass::setCassetteSbTicket(int cassette_,int sbNumber_,int ticket_,QString logInfo,bool isPlcChange,bool isUserChange)
{
	if (tuxip&&(!isPlcChange))//only write to plc
		sendCassetteSbTicketToPlc(cassette_,sbNumber_,ticket_);
#ifndef eastStation
	else //write to db
	{
		cassetteNum=cassette_;
		sbNumber=sbNumber_;
		ticket=ticket_;
		setCassetteSbTicketInPosG(posId,isSbPosition?0:cassetteNum,sbNumber,ticket,logInfo,isPlcChange,false,isSbPosition,isUserChange,mDb);
		loadFromDatabase();
		emit posChangedSignal();
		if (posId==conveyor__1_id)
			execQuery(QString("delete from %1 where cassette=%2").arg(exportsFrontTable).arg(cassetteNum),mDb);
		if (posId==cassetteDoubleExit_15_id)
			execQuery(QString("delete from %1 where cassette=%2").arg(exportsDoubleTable).arg(cassetteNum),mDb);
		if (posId==cassetteTripleExit__8_id)
			execQuery(QString("delete from %1 where cassette=%2").arg(exportsTriple1Table).arg(cassetteNum),mDb);
		if (posId==cassetteTripleExit_12_id)
			execQuery(QString("delete from %1 where cassette=%2").arg(exportsTriple2Table).arg(cassetteNum),mDb);
	}
#endif
}
void posClass::sendCassetteSbTicketToPlc()
{
	sendCassetteSbTicketToPlc(cassetteNum,sbNumber,ticket);
}
void posClass::sendCassetteSbTicketToPlc(int cassette_,int sbNumber_,int ticket_)
{
	if ((tuxip)/*&&(tuxip->getHasError())*/)
	{
		tuxip->writeInteger("posCassette",plcIndex,cassette_);
		logV(QString("write cassette to plc %1[%2]:%3").arg("posCassette").arg(plcIndex).arg(cassetteNum),infoStr);

		tuxip->writeInteger("posSb",plcIndex,sbNumber_);
		logV(QString("write sbNumber to plc %1[%2]:%3").arg("posSb").arg(plcIndex).arg(sbNumber_),infoStr);

		tuxip->writeInteger("posTicket",plcIndex,ticket_);
		logV(QString("write ticket to plc %1[%2]:%3").arg("posTicket").arg(plcIndex).arg(ticket_),infoStr);
	}
}
void posClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
#ifndef eastStation
	if (!tuxip)
		return;
	if (index!=plcIndex)
		return;
	//if (tuxip->getIp()!=ip)
	//	return;
	if ((tuxip->getIp()==ip)&&((table=="posCassette")||(table=="posSb")||(table=="posTicket"))  )
	{
		if (index==40)
			double a=0.;
		int cass=tuxip->readIntegerValue("posCassette",plcIndex);
		int sb=tuxip->readIntegerValue("posSb",plcIndex);
		int tick=tuxip->readIntegerValue("posTicket",plcIndex);
		setCassetteSbTicket(cass,sb,tick,"plc change",true,false);
	}
#endif
}
void posClass::mousePressedSlot(QGraphicsSceneMouseEvent *ev)
{
	if ((ev->modifiers()==Qt::ShiftModifier)&&(ev->button()==Qt::LeftButton))
	{
		if (adminMode)
		{
			execQuery(QString("update pos set locked=%1 where posId=%2").arg(isLocked?0:1).arg(posId),mDb);
			loadFromDatabase();
		}
		return;
	}
	if ((ev->button()==Qt::LeftButton)||(ev->button()==Qt::RightButton))
	{
		if (ev->modifiers()&Qt::ControlModifier)
			emit posCtrlMouseReleasedSignal(this,ev);
		else
			showPositionDlg(false);
	}
}
void posClass::showPositionDlgSlotSlot()
{
	showPositionDlg(false);
}
void posClass::showPositionDlg(bool checkPass)
{
	posDlgClass *posDlg=new posDlgClass(posId,isSbPosition,isLocked,cassetteNum,sbNumber,ticket,description,column,row,plcXPos,plcYPos,plcZPos,this);
	if ( (posId==pincer1_50_id)||(posId==pincer1_52_id)||(posId==pincer1_53_id)||(posId==pincer1_54_id) )
		posDlg->setDestinationWarehouse(getValueOfBit(tuxip->readIntegerValue("rConveyorState",plcIndex),23));
	if (posDlg->myExec(checkPass))
	{
		int activeFromPos=getFirst(QString("select fromPos from missions where fromPos=%1").arg(posId),mDb).toInt();
		int activeToPos=getFirst(QString("select toPos from missions where toPos=%1").arg(posId),mDb).toInt();
		if ((activeFromPos!=posId)&&(activeToPos!=posId))
		{
			if (posId<craneId)
				execQuery(QString("update pos set locked=%1 where posId=%2").arg(posDlg->getIsLockedChecked()).arg(posId),mDb);
			setCassetteSbTicket(posDlg->getCassetteNum(),posDlg->getSbNumber(),posDlg->getTicket(),"user",false,true);
		}
		setPlcPos(plcXPos,plcYPos,plcZPos);
		emit posDlgClosedSignal();
		loadFromDatabase();
		if ( (posId==pincer1_50_id)||(posId==pincer1_52_id)||(posId==pincer1_53_id)||(posId==pincer1_54_id) )
		{
			tuxip->writeBit("rConveyorState",plcIndex,23,posDlg->getIsDestinationWarehouse());
			posDlg->logImportExportsTable();
		}
	}
	delete posDlg;
}
void posClass::setIsSelected(bool s)
{
	isSelected=s;
	updateGui();
}
bool posClass::getHasCassette()
{
	return (cassetteNum!=0);
}
bool posClass::getHasSb()
{
	return sbNumber!=0;
}
bool posClass::getHasEmptySb()
{
	return getHasSb()&&(!getHasTicket());
}
bool posClass::getHasTicket()
{
	return (ticket!=0);
}
void posClass::loadFromDatabase()
{
	int oldCassette=cassetteNum;
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select cassette,sbNumber,pos.ticket,locked,dx,dy,dz,ttt.weight from pos left join (select max(tId) as tId,ticket,weight from tickets group by ticket) as ttt on pos.ticket=ttt.ticket where posId=%1").arg(posId),mDb);
	if (dataVV.count()==1)
	{
		cassetteNum=dataVV[0][0].toInt();
		sbNumber=dataVV[0][1].toInt();
		ticket=dataVV[0][2].toInt();
		isLocked=dataVV[0][3].toInt();
		plcXCorrection=dataVV[0][4].toFloat();
		plcYCorrection=dataVV[0][5].toFloat();
		plcZCorrection=dataVV[0][6].toFloat();
		ticketWeight=dataVV[0][7].toFloat();
	}
	updateGui();
	if (oldCassette!=cassetteNum)
		emit posChangedSignal();
}

void posClass::updateGui()
{
	if (posId==-7)
		double a=0.;
	updateTxt();
	QString toolTipTxt=posTollTipStr;
	activeFromItem->setVisible(isActiveFrom);
	activeToItem->setVisible(isActiveTo);
	if (cassetteItem)
		cassetteItem->showGraphics(cassetteNum!=0);
	sbBlueItem->showGraphics(getHasSb());
	profilesItem->showGraphics(ticket!=0);
	lockedItem->showGraphics(getIsLocked());
	if (posItem)
		posItem->setPenWidth(isSelected?50.:0.);

	if (cassetteNum!=0)
		toolTipTxt+="<html><body bgcolor=\"#E6E6FA\"><font style='color:#333333'>"+tr("Cassette number:")+"</font><font style='color:#000077'>"+QString("%1").arg(cassetteNum)+"</font><br>";
	if (getHasSb())
		toolTipTxt+=QString(tr("Blue basket:%1<br>").arg(sbNumber));
	if (ticket!=0)
		toolTipTxt+=QString(tr("Ticket:%1 %2Kgr<br>").arg(ticket).arg(ticketWeight));
	toolTipTxt+=tr("X correction:%1 mm<br>").arg(getPlcXCorrection());
	toolTipTxt+=tr("Y correction:%1 mm<br>").arg(getPlcYCorrection());
	toolTipTxt+=tr("Z correction:%1 mm<br>").arg(getPlcZCorrection());
	toolTipTxt+=QString(QString("</body></html>"));
	mouseOveItem->setToolTip(toolTipTxt);
	updateTxt();
	scene->update();
}
//
int posClass::getPosId()
{
	return posId;
}
int posClass::getPlcIndex()
{
	return plcIndex;
}
int posClass::getColumn()
{
	return column;
}
int posClass::getColumnWithSign()
{
	return column*(posId/abs(posId));
}
int posClass::getRow()
{
	return row;
}
int posClass::getCassetteNumber()
{
	return cassetteNum;
}
int posClass::getTicket()
{
	return ticket;
}
bool posClass::getIsLocked()
{
	return isLocked;
}
tuxipClass* posClass::getTuxIp()
{
	return tuxip;
}
float posClass::getPlcX()
{
	return plcXPos;
}
float posClass::getPlcY()
{
	return plcYPos;
}
float posClass::getPlcZ()
{
	return plcZPos;
}
float posClass::getCorrectedPlcX()
{
	return plcXPos+plcXCorrection;
}
float posClass::getCorrectedPlcZ()
{
	return plcZPos+plcZCorrection;
}
void posClass::setPlcXCorrection(float dx)
{
	dx=max(-maxCorrection,dx);
	dx=min(+maxCorrection,dx);
	plcXCorrection=dx;
	execQuery(QString("update pos set dx=%1 where posId=%2").arg(plcXCorrection).arg(posId),mDb);
}
void posClass::setPlcYCorrection(float dy)
{
	dy=max(-maxCorrection,dy);
	dy=min(+maxCorrection,dy);
	plcYCorrection=dy;
	execQuery(QString("update pos set dy=%1 where posId=%2").arg(plcYCorrection).arg(posId),mDb);
}
void posClass::setPlcZCorrection(float dz)
{
	dz=max(-maxCorrection,dz);
	dz=min(+maxCorrection,dz);
	//if ((plcZPos+dz)>7920)
	//	dz=(7920-plcZPos);
	plcZCorrection=dz;
	execQuery(QString("update pos set dz=%1 where posId=%2").arg(plcZCorrection).arg(posId),mDb);
}
float posClass::getPlcXCorrection()
{
	return plcXCorrection;
}
float posClass::getPlcYCorrection()
{
	return plcYCorrection;
}
float posClass::getPlcZCorrection()
{
	return plcZCorrection;
}
QString posClass::getDescription()
{
	return description;
}
void posClass::setIsActiveFrom(bool a)
{
	isActiveFrom=a;
	updateGui();
}
void posClass::setIsActiveTo(bool a)
{
	isActiveTo=a;
	updateGui();
}
void posClass::setGraphicsZ(int z)
{
	mouseOveItem->setZValue(z);
	if (posItem)
		posItem->setZValue(z);
	if (cassetteItem)
		cassetteItem->setZValue(z);
	sbBlueItem->setZValue(z);
	profilesItem->setZValue(z);
	lockedItem->setZValue(z);
	activeFromItem->setZValue(z);
	activeToItem->setZValue(z);
}

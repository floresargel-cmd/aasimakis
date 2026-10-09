#include "pch.h"
#include "pos.h"
double textScale=5.;
#include "allPos.h"
extern 	allPossClass *globalAllPosPtr;
posClass::posClass(int type_,int plcId_,int basketNum_,QString description_,int column_,int row_,float plcXPos_,float plcYPos_,float plcZPos_,float xCorr_,float yCorr_,float zCorr_,QGraphicsScene *scene_,dxfGroupOfGraphicsItemClass *mouseOver,dxfGroupOfGraphicsItemClass *cell,dxfGroupOfGraphicsItemClass *fullBasket,dxfGroupOfGraphicsItemClass *emptyBasket,dxfGroupOfGraphicsItemClass *locked,QWidget *p):QWidget(p)
{
	tuxip=FALSE;
	setVisible(false);
	plcId=plcId_;
	description=description_.trimmed();
	column=column_;
	row=row_;
	plcXPos=plcXPos_;
	plcYPos=plcYPos_;
	plcZPos=plcZPos_;
	plcXCorrection=xCorr_;
	plcYCorrection=yCorr_;
	plcZCorrection=zCorr_;
	scene=scene_;
	type=type_;
	basketNum=basketNum_;
	isLocked=false;
	isSelected=false;
	totalWidth=totalWeight=0.;
	//if (basketNum)
	//	basketType=getFirst(QString("select type from baskets where basket=%1").arg(basketNum),db).toInt();
	//
	xOffset=0.f;
	yOffset=0.f;
	if ((plcId>0)&&(plcId<cranePlcId))
		yOffset=gYPlussOffset;
	else if ((plcId<0)&&(plcId<cranePlcId))
		yOffset=gYMinusOffset;
	else if (plcId==exitPlcId)
		yOffset=gYPlussOffset;
	cellItem=new dxfGroupOfGraphicsItemClass(cell,0.,0.,this);scene->addItem(cellItem);
	fullItem=new dxfGroupOfGraphicsItemClass(fullBasket,0.,0.,this);scene->addItem(fullItem);fullItem->setCursor(Qt::ClosedHandCursor);
	emptyItem=new dxfGroupOfGraphicsItemClass(emptyBasket,0.,0.,this);scene->addItem(emptyItem);emptyItem->setCursor(Qt::ClosedHandCursor);
	mouseOverItem=new dxfGroupOfGraphicsItemClass(mouseOver,0.,0.,this);scene->addItem(mouseOverItem);
	lockedItem=new dxfGroupOfGraphicsItemClass(locked,0.,0.,this);scene->addItem(lockedItem);
	posTollTipStr=QString("position:%1<br>plc id:%2<br>column:%3,row:%4<br>x:%5mm,y:%6mm,z:%7mm<br>").arg(description).arg(plcId).arg(column).arg(row).arg(plcXPos).arg(plcYPos).arg(plcZPos);
	pcItem=new QGraphicsRectItem();scene->addItem(pcItem);
	pcItem->setPen(QPen(Qt::NoPen));
	pcItem->setBrush(QBrush(QColor(200,100,100,150)));
	graphicsItemText=new QGraphicsSimpleTextItem();
	scene->addItem(graphicsItemText);
	graphicsItemText->setZValue(10);
	txtTransformation.scale(textScale,-textScale);
	graphicsItemText->setTransform(txtTransformation,false);

	setPlcPos(plcXPos,plcYPos,plcZPos);
	updateGui();
	connect(mouseOverItem,SIGNAL(mousePressedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedSlot(QGraphicsSceneMouseEvent*)));
	loadFromDatabase();
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
void posClass::setIsHighlited(bool h)
{
	if (h)
		mouseOverItem->hoverEnterEvent(NULL);
	else
		mouseOverItem->hoverLeaveEvent(NULL);
}
void posClass::setPlcPos(float x,float y,float z)
{
	plcXPos=x;
	plcYPos=y;
	plcZPos=z;
	gXPos=plcXPos+xOffset;
	gYPos=plcZPos+yOffset;
	cellItem->setPos(gXPos,gYPos);
	fullItem->setPos(gXPos,gYPos);
	emptyItem->setPos(gXPos,gYPos);
	lockedItem->setPos(gXPos,gYPos);
	mouseOverItem->setPos(gXPos,gYPos);
	updateTxt();
}
void posClass::updateTxt()
{
	if (basketNum!=0)
	{
		graphicsItemText->setText(tr("%1").arg(basketNum));
		graphicsItemText->setPos(gXPos-0.5*graphicsItemText->boundingRect().width()*textScale,gYPos+0.5*mouseOverItem->boundingRect().height());
		graphicsItemText->setVisible(true);
	}
	else
		graphicsItemText->setVisible(false);
}
void posClass::setBasketId(int b,bool user,QString logInfo)
{
	int oldBasket=basketNum;
	int posOfBasket=getFirst(QString("select plcId from pos where basket=%1").arg(b),db).toInt();
	if ((b!=0)&&(posOfBasket!=0)&&(posOfBasket!=plcId))
		uExit(errorStr,QString("trying to declare basket %1 in position %2, basket is allready declared int position %3").arg(b).arg(plcId).arg(posOfBasket));
	if ((basketNum!=0)&&(b!=0)&&(!user))
		uExit(errorStr,QString("trying to declare basket %1 in position %2, position %2 allready has basket %3 declared").arg(b).arg(plcId).arg(basketNum));
	execQuery(QString("update pos set basket=%1 where plcId=%2").arg(b).arg(plcId),db);
	execQuery(QString("insert into logPos (pos,basket,info,time) values(%1,%2,'%3','%4')").arg(plcId).arg(b).arg(logInfo).arg(timeForLog),db);
	if (oldBasket!=basketNum)
			emit basketChangedSignal(oldBasket,basketNum);
	loadFromDatabase();
	emit posChangedSignal();
}
void posClass::loadFromDatabase()
{
	int oldBasket=basketNum;
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select basket,locked,dx,dy,dz from pos where plcId=%1").arg(plcId),db);
	if (dataVV.count()==1)
	{
		basketNum=dataVV[0][0].toInt();
		isLocked=dataVV[0][1].toBool();
		plcXCorrection=dataVV[0][2].toFloat();
		plcYCorrection=dataVV[0][3].toFloat();
		plcZCorrection=dataVV[0][4].toFloat();
	}
	if (basketNum!=0)
	{
		totalWeight=calculateWeightOfBasket(basketNum);
		totalWidth=calculateWidthOfBasket(basketNum);
	}
	updateGui();
	if (oldBasket!=basketNum)
		emit basketChangedSignal(oldBasket,basketNum);
}
void posClass::mousePressedSlot(QGraphicsSceneMouseEvent *ev)
{
	if ((ev->modifiers()==Qt::ShiftModifier)&&(ev->button()==Qt::LeftButton))
	{
		execQuery(QString("update pos set locked=%1 where plcId=%2").arg(isLocked?0:1).arg(plcId),db);
		loadFromDatabase();
		return;
	}
	if ((ev->button()==Qt::LeftButton)||(ev->button()==Qt::RightButton))
	{
		if (ev->modifiers()==Qt::ControlModifier)
		{
			emit posCtrlMouseReleasedSignal(this,ev);
		}
		else if ((ev->modifiers()==Qt::ShiftModifier)&&(ev->button()==Qt::RightButton))
		{
			changeCassetteAndMoveContents();
		}
		else
		{
			showPositionDlgSlotSlot();
		}
	}
}
void posClass::changeCassetteAndMoveContents()
{
	if (true)
		return;
	if ((plcId!=basketNum)&&(basketNum!=0))
	{
		if (getFirst(QString("select tId from pos where basket=%1").arg(plcId),db).toInt()>0)
			return;
		if (QMessageBox::Yes==QMessageBox::question(this,tr("Change basket contents"),tr("Change basket and contents of position %1 to %2").arg(plcId).arg(basketNum),QMessageBox::Yes,QMessageBox::No))
		{
			execQuery(QString("update basketsContents set basket=%1 where basket=%2").arg(plcId).arg(basketNum),db);
			execQuery(QString("update pos set basket=%1 where plcId=%1").arg(plcId),db);
		}
		loadFromDatabase();
	}
}
void posClass::showPositionDlgSlotSlot()
{
	showPositionDlg(0);
}
void posClass::showPositionDlg(int activeTab,int type,bool checkPass)
{
	if (type<0)
		type=posDlgClass::basketNumberIsEditable|posDlgClass::showPositionData|posDlgClass::showBasketData;
	posDlgClass *posDlg=new posDlgClass(plcId,basketNum,description,column,row,plcXPos,plcYPos,plcZPos,type,activeTab,this);
	if (posDlg->myExec(checkPass))
	{
		int activeFromPos=getFirst(QString("select fromPos from missions where fromPos=%1").arg(plcId),db).toInt();
		int activeToPos=getFirst(QString("select toPos from missions where toPos=%1").arg(plcId),db).toInt();
		if ((activeFromPos!=plcId)&&(activeToPos!=plcId))
			setBasketId(posDlg->getBasketNum(),true,"user");
	}
	loadFromDatabase();
	delete posDlg;
}
void posClass::setIsSelected(bool s)
{
	isSelected=s;
	updateGui();
}
void posClass::updateGui()
{
	QString toolTipTxt=posTollTipStr;
	if (basketNum!=0)
	{
		toolTipTxt+="<html><body bgcolor=\"#E6E6FA\"><font style='color:#333333'>"+tr("Basket number:")+"</font><font style='color:#000077'>"+QString("%1").arg(basketNum)+"</font><br>";
		toolTipTxt+=tr("Weight:%1%(%2 libs out of %3 libs)<br>").arg(100*totalWeight/basketAvailableWeight,0,'f',1,' ').arg(toLibs(totalWeight),0,'f',1,' ').arg(toLibs(basketAvailableWeight),0,'f',0,' ');
		toolTipTxt+=tr("Width:%1%(%2 inches out of %3 inches)<br>").arg(100*totalWidth/basketAvailableWidth,0,'f',1,' ').arg(toInches(totalWidth),0,'f',1,' ').arg(toInches(basketAvailableWidth),0,'f',0,' ');
		pcItem->setVisible(true);
		pcItem->setRect(gXPos-100.,gYPos+15,(totalWeight/basketAvailableWeight)*200,50);
		QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select contentName,contentType from basketsContents where basket=%1").arg(basketNum),db);
		if (dataVV.count()>0)
		{
			fullItem->showGraphics(true);
			emptyItem->showGraphics(false);
		}
		else
		{
			fullItem->showGraphics(false);
			emptyItem->showGraphics(true);
		}
		toolTipTxt+=QString(QString("<table><tr><td>"));
		toolTipTxt+=tr("Name")+"</td><td>"+tr("Type")+"</td></tr>";
		for (int i=0;i<dataVV.count();i++)
				toolTipTxt+=QString("<tr><td>%1</td><td>%2</td></td></tr>").arg(dataVV[i][0].toString().trimmed()).arg(dataVV[i][1].toString().trimmed());
		toolTipTxt+=QString(QString("</table><br><br>"));
	}
	else
	{
		pcItem->setVisible(false);
		fullItem->showGraphics(false);
		emptyItem->showGraphics(false);
		toolTipTxt+=QString("<font style='color:#000077'>")+tr("No basket")+"</font><br>";
	}
	toolTipTxt+="<br>"+tr("X correction:%1").arg(getPlcXCorrection());
	toolTipTxt+="<br>"+tr("Y correction:%1").arg(getPlcYCorrection());
	toolTipTxt+="<br>"+tr("Z correction:%1").arg(getPlcZCorrection());
	if ((isLocked)||(isSelected))
	{
		lockedItem->showGraphics(true);
		if (isLocked)
			lockedItem->setPen(QPen(QColor(200,100,100)));
		if (isSelected)
			lockedItem->setPen(QPen(QColor(100,200,100)));
	}
	else
	{
		lockedItem->showGraphics(false);
	}
	toolTipTxt+=QString(QString("</body></html>"));
	mouseOverItem->setToolTip(toolTipTxt);
	updateTxt();
	scene->update();
}
//
int posClass::getPlcId()
{
	return plcId;
}
int posClass::getColumn()
{
	return column;
}
int posClass::getColumnWithSign()
{
	return column*(plcId/abs(plcId));
}
int posClass::getRow()
{
	return row;
}
int posClass::getBasketNumber()
{
	return basketNum;
}
bool posClass::getIsLocked()
{
	return isLocked;
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
void posClass::setPlcXCorrection(float dx)
{
	dx=max(-75.f,dx);
	dx=min(+75.f,dx);
	plcXCorrection=dx;
	execQuery(QString("update pos set dx=%1 where plcId=%2").arg(plcXCorrection).arg(plcId),db);
}
void posClass::setPlcYCorrection(float dy)
{
	dy=max(-75.f,dy);
	dy=min(+75.f,dy);
	plcYCorrection=dy;
	execQuery(QString("update pos set dy=%1 where plcId=%2").arg(plcYCorrection).arg(plcId),db);
}
void posClass::setPlcZCorrection(float dz)
{
	dz=max(-75.f,dz);
	dz=min(+75.f,dz);
	plcZCorrection=dz;
	execQuery(QString("update pos set dz=%1 where plcId=%2").arg(plcZCorrection).arg(plcId),db);
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
bool posClass::getCanTakeBasketInPos(int posId)
{
	int maxContentType=getFirst(QString("select max(contentTypes.cellType) from basketsContents join contentTypes on basketsContents.contentType=contentTypes.content where basket=%1").arg(posId),db).toInt();
	return maxContentType<=type;
}

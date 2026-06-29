#include "pch.h"
#include "gBtnItem.h"
gBtnItemClass::gBtnItemClass(tuxipClass *tuxip_,QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,QString plcActiveTable_,int plcActiveIndex_,int plcActiveBit_,QGraphicsScene *myScene,QString label_,float rotationAngle_,float offset_,float size,typeEnumClass typeEnum_)
{
	tuxip=tuxip_;
	plcCommandTable=plcCommandTable_;
	plcCommandIndex=plcCommandIndex_;
	plcCommandNumber=plcCommandNumber_;
	plcActiveTable=plcActiveTable_;
	plcActiveIndex=plcActiveIndex_;
	plcActiveBit=plcActiveBit_;
	offset=offset_;
	inActivePen=QPen(QColor(10,100,255));
	activePen=QPen(QColor(125,255,125));
	isActive=false;
	isMouseOver=false;
	inMaintenanceMode=false;
	label=label_;
	rotationAngle=rotationAngle_;
	typeEnum=typeEnum_;
	if (plcCommandNumber>0)
	{
		originalPath.moveTo(0.f,0.f);
		originalPath.lineTo(-size,+size);
		originalPath.lineTo(-size,-size);
		originalPath.closeSubpath();
		QTransform rotateformation;
		rotateformation.rotate(rotationAngle);
		originalPath=rotateformation.map(originalPath);
		QTransform translateformation;
		translateformation.translate(cos(rotationAngle*uPi/180.)*offset,sin(rotationAngle*uPi/180.)*offset);
		originalPath=translateformation.map(originalPath);
	}
	else
	{
		originalPath.moveTo(0.f,0.f);
		originalPath.addEllipse(QPointF(0.,0.),size*0.5,size*0.5);
	}
	if (plcCommandTable.length()>0)
	{
		setAcceptHoverEvents(true);
		setFlag(QGraphicsItem::ItemIsFocusable,true);
		setFlag(QGraphicsItem::ItemIsSelectable,true);
		QGraphicsItem::setCursor(Qt::ClosedHandCursor);
	}
	else
	{
		setAcceptHoverEvents(false);
		setFlag(QGraphicsItem::ItemIsFocusable,false);
		setFlag(QGraphicsItem::ItemIsSelectable,false);
		QGraphicsItem::setCursor(Qt::ArrowCursor);
	}
	currentPath=originalPath;
	setToolTip(tr("%1\nSends %2 to %3[%4]\nConnected to %5[%6].%7\nip: %8").arg(label).arg(plcCommandNumber).arg(plcCommandTable).arg(plcCommandIndex).arg(plcActiveTable).arg(plcActiveIndex).arg(plcActiveBit).arg(tuxip->getIp()));
	setZValue(1);
	myScene->addItem(this);
	setMaintenanceMode(true);
}
gBtnItemClass::~gBtnItemClass()
{
}
QRectF gBtnItemClass::boundingRect() const
{
 	return currentPath.boundingRect();
}
void gBtnItemClass::setPos(float x,float y)
{
	QTransform transformation;
	transformation.translate(x,y);
	currentPath=transformation.map(originalPath);

	//setTransform(transformation);
}
void gBtnItemClass::setMaintenanceMode(bool m)
{
	inMaintenanceMode=m;
	update();
}
void gBtnItemClass::setActvePen(QPen ap)
{
	activePen=ap;
}
void gBtnItemClass::setInactvePen(QPen ip)
{
	inActivePen=ip;
}
void gBtnItemClass::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	painter->drawRect(boundingRect());
	if ((plcCommandNumber>0))
	{
		if (isActive)
		{
			if (isMouseOver)
				painter->fillPath(currentPath,QBrush(QColor(255,150,150)));
			painter->setPen(activePen);
			painter->drawPath(currentPath);
		}
		else 
		{
			if (isMouseOver)
				painter->fillPath(currentPath,QBrush(QColor(150,150,255)));
			painter->setPen(inActivePen);
			painter->drawPath(currentPath);
		}
	}
	else
	{
		if (isActive)
			painter->fillPath(currentPath,QBrush(QColor(100,200,100)));
		painter->setPen(inActivePen);
		painter->drawPath(currentPath);
	}
}
void gBtnItemClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((tuxip->getIp()==ip)&&(plcActiveTable==table)&&(plcActiveIndex==index))
	{
		bool b=HAVECOMMONBIT(value.toInt(),BITS[plcActiveBit]);
		if (b!=isActive)
		{
			isActive=b;
			update();
		}
	}
}
void gBtnItemClass::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=true;
	QGraphicsItem::update();
	QGraphicsItem::hoverEnterEvent(ev);
}
void gBtnItemClass::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=false;
	QGraphicsItem::update();
	QGraphicsItem::hoverLeaveEvent(ev);
}
void gBtnItemClass::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
	if (plcCommandTable.length()>0)
		tuxip->writeInteger(plcCommandTable,plcCommandIndex,plcCommandNumber);
}
void gBtnItemClass::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
	if (plcCommandTable.length()>0)
		tuxip->writeInteger(plcCommandTable,plcCommandIndex,0);
}
#include "pch.h"
#include "psc.h"
gProximitySwitchClass::gProximitySwitchClass(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset_,QGraphicsScene *myScene,float width,QString description,bool rotate90)
{
	plcIpAdress=plcIpAdress_;
	plcTable=plcTable_;
	plcIndex=plcIndex_;
	plcBit=plcBit_;
	offset=offset_;
	isMouseOver=false;
	inActivePen=QPen(QColor(150,150,150),30,Qt::DotLine);
	activePen=QPen(QColor(100,255,100),50);
	isActive=false;
	if (!rotate90)
	{
		myPath.moveTo(-0.5*width,0.f);
		myPath.lineTo(+0.5*width,0.f);
	}
	else
	{
		myPath.moveTo(0.f,-0.5*width);
		myPath.lineTo(0.f,+0.5*width);
	}
	setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsFocusable,true);
	setFlag(QGraphicsItem::ItemIsSelectable,true);
	QGraphicsItem::setCursor(Qt::ClosedHandCursor);
	if (!rotate90)
		myBoundRect=QRectF(QPointF(-0.5*width,-50.f),QPointF(+0.5*width,50.f));
	else
		myBoundRect=QRectF(QPointF(-50.f,-0.5*width),QPointF(50.f,+0.5*width));
	setToolTip(tr("%1\nConnected to %2[%3].%4\nip: %5%6").arg(description).arg(plcTable).arg(plcIndex).arg(plcBit).arg(plcIpAdress).arg((txt.length()>0?"\n"+txt:"")));
	setZValue(1);
	myScene->addItem(this);
}
gProximitySwitchClass::~gProximitySwitchClass()
{
}
void gProximitySwitchClass::setTransform(QTransform tr)
{
	tr.translate(0.f,offset);
	QGraphicsItem::setTransform(tr);
}
void gProximitySwitchClass::setPens(QPen activePen_,QPen inActivePen_)
{
	activePen=activePen_;
	inActivePen=inActivePen_;
}
QRectF gProximitySwitchClass::boundingRect() const
{
 	return myBoundRect;
}
void gProximitySwitchClass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if (isMouseOver)
	{
		//painter->fillRect(myBoundRect,QBrush(QColor(240,225,225)));
		painter->setPen(QPen(QColor(150,150,200)));
		painter->drawRect(myBoundRect);
	}
	if (isActive)
	{
		painter->setPen(activePen);
		painter->drawPath(myPath);
	}
	else 
	{
		painter->setPen(inActivePen);
		painter->drawPath(myPath);
	}

}
void gProximitySwitchClass::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=true;
	QGraphicsItem::update();
	QGraphicsItem::hoverEnterEvent(ev);
}
void gProximitySwitchClass::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=false;
	QGraphicsItem::update();
	QGraphicsItem::hoverLeaveEvent(ev);
}
void gProximitySwitchClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((plcIpAdress==ip)&&(plcTable==table)&&(plcIndex==index))
	{
		bool b=HAVECOMMONBIT(value.toInt(),BITS[plcBit]);
		if (b!=isActive)
		{
			isActive=b;
			update();
		}
	}
}
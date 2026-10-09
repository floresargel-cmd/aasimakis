#include "pch.h"
#include "gProxItem.h"
gProximitySwitchClass::gProximitySwitchClass(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset_,QGraphicsScene *myScene,int typeEnum_,float x,float y,float width,QString title)
{
	plcIpAdress=plcIpAdress_;
	plcTable=plcTable_;
	plcIndex=plcIndex_;
	plcBit=plcBit_;
	offset=offset_;
	typeEnum=typeEnum_;
	isMouseOver=false;
	float size=250;
	inActivePen=QPen(QColor(200,150,150),size);
	activePen=QPen(QColor(100,255,100),size+25);
	isActive=false;
	if (typeEnum==typeHorizontal)
	{
		myPath.moveTo(x-width,y+offset);
		myPath.lineTo(x+width,y+offset);
		myBoundRect=QRectF(QPointF(x-width-size,y+offset-size),QPointF(x+width+size,y+offset+size));
	}
	else if (typeEnum==typeVertical)
	{
		myPath.moveTo(x+offset,y-width);
		myPath.lineTo(x+offset,y+width);
		myBoundRect=QRectF(QPointF(x+offset-size,y+width-size),QPointF(x+offset+size,y-width-size));
	}
	else if (typeEnum==typeRotated45)
	{
		myPath.moveTo(0.f,0.f);
		myPath.moveTo(0,+width);
		myPath.lineTo(0,-width);
		myPath.closeSubpath();
		QTransform transform;
		transform.translate(x,y);
		transform.translate(cos(45.f*uPi/180.)*offset,sin(45.f*uPi/180.)*offset);
		transform.rotate(45);
		myPath=transform.map(myPath);
		myBoundRect=myPath.boundingRect();
	}
	else if (typeEnum==typeLightCurtain45)
	{
		inActivePen=QPen(QColor(200,150,200),size);
		myPath.moveTo(0.f,0.f);
		myPath.moveTo(0,+700);
		myPath.lineTo(0,-700);
		myPath.closeSubpath();
		QTransform transform;
		transform.translate(x,y);
		transform.translate(cos(45.f*uPi/180.)*offset,sin(45.f*uPi/180.)*offset);
		transform.rotate(45);
		myPath=transform.map(myPath);
		myBoundRect=myPath.boundingRect();
	}
	setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsFocusable,true);
	setFlag(QGraphicsItem::ItemIsSelectable,true);
	QGraphicsItem::setCursor(Qt::ClosedHandCursor);
	setToolTip(tr("%1\nConnected to %2[%3].%4\nip: %5%6").arg(title).arg(plcTable).arg(plcIndex).arg(plcBit).arg(plcIpAdress).arg((txt.length()>0?"\n"+txt:"")));
	setZValue(10);
	myScene->addItem(this);
	this->setZValue(10.);
}
gProximitySwitchClass::~gProximitySwitchClass()
{
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
//void gProximitySwitchClass::setTransform(QTransform tr)
//{
//	tr.translate(offset,0.f);
//	QGraphicsItem::setTransform(tr);
//}
void gProximitySwitchClass::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if ( (isMouseOver)||(isActive))
		painter->setPen(activePen);
	else
		painter->setPen(inActivePen);
	painter->drawPath(myPath);
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

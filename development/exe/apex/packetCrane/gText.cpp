#include "pch.h"
#include "gText.h"
myGraphicsTextItem::myGraphicsTextItem(QString txt,QRectF r,QFont f,QColor col,QGraphicsScene *scene):QGraphicsItem()
{
	text=txt;
	font=f;
	pen=QPen(col,0);
	myBoundRect=r;
	textOp.setAlignment(Qt::AlignCenter);
	textOp.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	scene->addItem(this);
}
myGraphicsTextItem::~myGraphicsTextItem()
{
}
void myGraphicsTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(pen);
	//painter->drawRect(myBoundRect);
	painter->setFont(font);
	painter->drawText(boundingRect(),text,textOp);
	//QGraphicsItem::paint(painter,option,widget);
}
void myGraphicsTextItem::setColor(QColor c)
{
	pen.setColor(c);
	update();
}
void myGraphicsTextItem::setText(const QString &txt)
{
	text=txt;
	update();
}
void myGraphicsTextItem::setText(const QString &txt,QColor c)
{
	pen.setColor(c);
	setText(txt);
}
QRectF myGraphicsTextItem::boundingRect() const
{
 	return myBoundRect;
}
void myGraphicsTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
	QGraphicsItem::hoverEnterEvent(ev);
}
void myGraphicsTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
	QGraphicsItem::hoverLeaveEvent(ev);
}

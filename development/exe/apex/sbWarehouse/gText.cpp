#include "pch.h"
#include "gText.h"
myGraphicsTextItem::myGraphicsTextItem(QString txt,QRectF r,QFont f,QPen pen_,QGraphicsScene *scene):QGraphicsItem()
{
	text=txt;
	font=f;
	pen=pen_;
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
	painter->setFont(font);
	painter->drawText(boundingRect(),text,textOp);
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

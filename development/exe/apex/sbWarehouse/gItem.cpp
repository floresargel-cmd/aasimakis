#include "pch.h"
#include "gItem.h"
gItemClass::gItemClass():QGraphicsItem()
{
	myIsVisible=true;
	isMouseOver=false;
}
gItemClass::gItemClass(gItemClass *p,float dx,float dy):QGraphicsItem(),QObject(p->parent())
{
	pen=p->pen;
	fillBruch=p->fillBruch;
	mouseOverFillBruch=p->mouseOverFillBruch;
	rectOfAll=p->rectOfAll;
	pathOfAll=p->pathOfAll;
	boundingRectPath=p->boundingRectPath;
	QTransform transform=QTransform().translate(dx,dy);
	setTransform(transform);
	myIsVisible=true;
	isMouseOver=false;
	if (mouseOverFillBruch!=Qt::NoBrush)
	{
		setAcceptHoverEvents(true);
		setFlag(QGraphicsItem::ItemIsFocusable,true);
		setFlag(QGraphicsItem::ItemIsSelectable,true);
		QGraphicsItem::setCursor(Qt::ClosedHandCursor);
	}
}
gItemClass::gItemClass(QString fileName,QPen pen_,QObject* o,QBrush mouseOverFillBruch_,QBrush fillBruch_):QGraphicsItem(),QObject(o)
{
	pen=pen_;
	mouseOverFillBruch=mouseOverFillBruch_;
	fillBruch=fillBruch_;
	isMouseOver=false;
	myIsVisible=true;
	if (mouseOverFillBruch!=Qt::NoBrush)
	{
		setAcceptHoverEvents(true);
		setFlag(QGraphicsItem::ItemIsFocusable,true);
		setFlag(QGraphicsItem::ItemIsSelectable,true);
		QGraphicsItem::setCursor(Qt::ClosedHandCursor);
	}
	loadFromDxfFile(fileName);
}
gItemClass::~gItemClass()
{
}
void gItemClass::showGraphics(bool s)
{
	myIsVisible=s;
	QGraphicsItem::setVisible(s);
}
QPainterPath gItemClass::shape() const
{
	return boundingRectPath;
}
QRectF gItemClass::boundingRect() const
{
	return rectOfAll; 
}
QPainterPath gItemClass::getPath()
{
	return pathOfAll;
}
void gItemClass::addPath(QPainterPath p)
{
	pathOfAll.addPath(p);
	boundingRectPath.addRect(p.boundingRect());
}
void gItemClass::myRotate(float xCenter,float yCenter,float angle)
{
	QTransform  transformation;
	transformation.rotate(angle);
	transformation.translate(xCenter+QGraphicsItem::x()*qCos(-uPi*angle/180)-QGraphicsItem::y()*qSin(-uPi*angle/180),yCenter+QGraphicsItem::x()*qSin(-uPi*angle/180)+QGraphicsItem::y()*qCos(-uPi*angle/180));
	QGraphicsItem::setTransform(transformation);
}
void gItemClass::setPen(QPen p)
{
	prepareGeometryChange();
	pen=p;
}
void gItemClass::setFillBruch(QBrush b)
{
	fillBruch=b;
}
void gItemClass::setPenWidth(float w)
{
	pen.setWidthF(w);
}
void gItemClass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if (!myIsVisible)
		return;
	if ((mouseOverFillBruch!=Qt::NoBrush)&&(!isMouseOver))
		return;
	if ((isMouseOver)&&(mouseOverFillBruch!=Qt::NoBrush))
		painter->fillPath(pathOfAll,mouseOverFillBruch);
	if (fillBruch!=Qt::NoBrush)
		painter->fillPath(pathOfAll,QBrush(fillBruch));
	painter->setPen(pen);
	painter->drawPath(pathOfAll);
}
void gItemClass::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=true;
	emit hoverEnterSignal();
	QGraphicsItem::update();
	QGraphicsItem::hoverEnterEvent(ev);
}
void gItemClass::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=false;
	emit hoverLeaveSignal();
	QGraphicsItem::update();
	QGraphicsItem::hoverLeaveEvent(ev);
}
void gItemClass::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
	emit mousePressedSignal(ev);
	QGraphicsItem::mousePressEvent(ev);
}
void gItemClass::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
	emit mouseReleasedSignal(ev);
	QGraphicsItem::mouseReleaseEvent(ev);
}
void gItemClass::loadFromDxfFile(QString fileName)
{
	QFileInfo fInfo(fileName);
	if (!fInfo.exists())
		uExit(tr("error"),tr("Could not find %1 file").arg(fileName));
	dxfDataClass *dxfData=new dxfDataClass();//dxf reader
	DL_Dxf* dxf=new DL_Dxf();
	if (!dxf->qFileIn(fileName,dxfData))
	{
		delete dxf;
		delete dxfData;
		uExit(tr("error"),tr("Could read %1 file").arg(fileName));
	}
	dxfData->calculate();
	QPointF lastPoint;
	for (int i=0;i<dxfData->getNoOfLines();i++)
	{
		//if (!areSamePoints(dxfData->getLine(i).p1(),lastPoint))
		if (dxfData->getLine(i).p1()!=lastPoint)
			pathOfAll.moveTo(dxfData->getLine(i).p1());
		pathOfAll.lineTo(dxfData->getLine(i).p2());
		lastPoint=dxfData->getLine(i).p2();
		rectOfAll=rectOfAll.united(QRectF(dxfData->getLine(i).p1(),dxfData->getLine(i).p2()));
	}
	for (int i=0;i<dxfData->getNoOfStrings();i++)
	{
		QMatrix  mirrorMatrix;
		mirrorMatrix.scale(1,-1);
		QPainterPath txtPath;txtPath.addText(dxfData->getString(i).position,dxfData->getString(i).font,dxfData->getString(i));
		pathOfAll.addPath(mirrorMatrix.map(txtPath).translated(0,2*dxfData->getString(i).position.y()));
		//
	}
	boundingRectPath.addRect(rectOfAll);
	//delete dxf;
	//delete dxfData;
}

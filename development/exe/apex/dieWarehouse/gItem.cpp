#include "pch.h"
#include "gItem.h"
dxfGroupOfGraphicsItemClass::dxfGroupOfGraphicsItemClass():QGraphicsItem()
{
	myIsVisible=true;
	isMouseOver=false;
}
dxfGroupOfGraphicsItemClass::dxfGroupOfGraphicsItemClass(dxfGroupOfGraphicsItemClass *p,float dx,float dy,QObject* o):QGraphicsItem(),QObject(o)
{
	pen=p->pen;
	mouseOverFillColor=p->mouseOverFillColor;
	hideWhenMouseNotOver=p->hideWhenMouseNotOver;
	mouseOverFlag=p->mouseOverFlag;
	rectOfAll=p->rectOfAll;
	pathOfAll=p->pathOfAll;
	myIsVisible=p->myIsVisible;
	showGraphics(myIsVisible);
	boundingRectPath=p->boundingRectPath;
	QTransform transform=QTransform().translate(dx,dy);
	setTransform(transform);
	setMouseOverFlag(mouseOverFlag);
	isMouseOver=false;
}
dxfGroupOfGraphicsItemClass::dxfGroupOfGraphicsItemClass(QString fileName,QPen pen_,bool mouseOverFlag_,bool hideWhenMouseNotOver_,QColor mouseOverFillColor_,QObject* o):QGraphicsItem(),QObject(o)
{
	//isMouseOver=false;
	//xPos=0.0;
	//yPos=0.0;
	//color=color_;
	////
	//normalPen=QPen(color,0.0);
	//mouseOverPen=QPen(QColor(100,100,200),0.0);
	//mouseOverFlag=mouseOverFlag_;

	pen=pen_;
	mouseOverFillColor=mouseOverFillColor_;
	mouseOverFlag=mouseOverFlag_;
	hideWhenMouseNotOver=hideWhenMouseNotOver_;
	isMouseOver=false;
	myIsVisible=true;
	setMouseOverFlag(mouseOverFlag);
	loadFromDxfFile(fileName);
}
dxfGroupOfGraphicsItemClass::~dxfGroupOfGraphicsItemClass()
{
}
void dxfGroupOfGraphicsItemClass::showGraphics(bool s)
{
	myIsVisible=s;
	QGraphicsItem::setVisible(s);
}
void dxfGroupOfGraphicsItemClass::setMouseOverFlag(bool f)
{
	mouseOverFlag=f;
	setAcceptHoverEvents(mouseOverFlag);
	setFlag(QGraphicsItem::ItemIsFocusable,mouseOverFlag);
	setFlag(QGraphicsItem::ItemIsSelectable,mouseOverFlag);
	if (mouseOverFlag)
		QGraphicsItem::setCursor(Qt::ClosedHandCursor);
	else
		QGraphicsItem::setCursor(Qt::ArrowCursor);
}
QPainterPath dxfGroupOfGraphicsItemClass::shape() const
{
	return boundingRectPath;
	//return pathOfAll;
}
QRectF dxfGroupOfGraphicsItemClass::boundingRect() const
{
	return rectOfAll; 
}
void dxfGroupOfGraphicsItemClass::myRotate(float xCenter,float yCenter,float angle)
{
	QTransform  transformation;
	transformation.rotate(angle);
	transformation.translate(xCenter+QGraphicsItem::x()*qCos(-uPi*angle/180)-QGraphicsItem::y()*qSin(-uPi*angle/180),yCenter+QGraphicsItem::x()*qSin(-uPi*angle/180)+QGraphicsItem::y()*qCos(-uPi*angle/180));
	QGraphicsItem::setTransform(transformation);
}
void dxfGroupOfGraphicsItemClass::setPen(QPen p)
{
	pen=p;
	update();
}
void dxfGroupOfGraphicsItemClass::setMouseOverFillColor(QColor c)
{
	mouseOverFillColor=c;
	update();
}
void dxfGroupOfGraphicsItemClass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if (!myIsVisible)
		return;
	if ((mouseOverFlag)&&(hideWhenMouseNotOver)&&(!isMouseOver))
		return;
	if (isMouseOver)
	{
		//painter->fillRect(boundingRect(),QBrush(mouseOverFillColor));
		painter->fillPath(pathOfAll,QBrush(mouseOverFillColor));
		painter->drawPath(pathOfAll);
	}
	painter->setPen(pen);
	painter->drawPath(pathOfAll);
}
void dxfGroupOfGraphicsItemClass::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=true;
	emit hoverEnterSignal();
	QGraphicsItem::update();
	QGraphicsItem::hoverEnterEvent(ev);
}
void dxfGroupOfGraphicsItemClass::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=false;
	emit hoverLeaveSignal();
	QGraphicsItem::update();
	QGraphicsItem::hoverLeaveEvent(ev);
}
void dxfGroupOfGraphicsItemClass::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
	emit mousePressedSignal(ev);
	QGraphicsItem::mousePressEvent(ev);
}
void dxfGroupOfGraphicsItemClass::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
	emit mouseReleasedSignal(ev);
	QGraphicsItem::mouseReleaseEvent(ev);
}
void dxfGroupOfGraphicsItemClass::loadFromDxfFile(QString fileName)
{
	QFileInfo fInfo(fileName);
	if (!fInfo.exists())
		uExit(tr("error"),tr("Could not find %1 file").arg(fileName));
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	if (!fInfo.isNativePath())
#else
	if (!fInfo.isWritable())
#endif
	{
		QString d=QString("qrc");
		QDir::current().mkdir(d);
		QString f=d+"/"+fInfo.fileName();
		QFile::copy(fileName,f);
		fileName=f;
	}

	dxfDataClass *dxfData=new dxfDataClass();//dxf reader
	DL_Dxf* dxf = new DL_Dxf();
	if (!dxf->in(fileName.toStdString(),dxfData))
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
	delete dxf;
	delete dxfData;
}

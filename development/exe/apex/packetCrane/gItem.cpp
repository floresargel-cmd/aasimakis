#include "pch.h"
#include "gItem.h"
dxfClass::dxfClass():QGraphicsItem()
{
	myIsVisible=true;
	isMouseOver=false;
	fillFlag=false;
}
dxfClass::dxfClass(dxfClass *p,float dx,float dy,QObject* o):QGraphicsItem(),QObject(o)
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
	fillFlag=false;
}
dxfClass::dxfClass(QString fileName,QPen pen_,bool mouseOverFlag_,bool hideWhenMouseNotOver_,QColor mouseOverFillColor_,QObject* o):QGraphicsItem(),QObject(o)
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
	fillFlag=false;
	setMouseOverFlag(mouseOverFlag);
	loadFromDxfFile(fileName);
}
dxfClass::~dxfClass()
{
}
void dxfClass::showGraphics(bool s)
{
	myIsVisible=s;
	QGraphicsItem::setVisible(s);
}
void dxfClass::setMouseOverFlag(bool f)
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
QPainterPath dxfClass::shape() const
{
	return boundingRectPath;
	//return pathOfAll;
}
QRectF dxfClass::boundingRect() const
{
	return rectOfAll; 
}
void dxfClass::myRotate(float xCenter,float yCenter,float angle)
{
	QTransform  transformation;
	transformation.rotate(angle);
	transformation.translate(xCenter+QGraphicsItem::x()*qCos(-uPi*angle/180)-QGraphicsItem::y()*qSin(-uPi*angle/180),yCenter+QGraphicsItem::x()*qSin(-uPi*angle/180)+QGraphicsItem::y()*qCos(-uPi*angle/180));
	QGraphicsItem::setTransform(transformation);
}
void dxfClass::setPen(QPen p)
{
	pen=p;
	update();
}
void dxfClass::setMouseOverFillColor(QColor c)
{
	mouseOverFillColor=c;
	update();
}
void dxfClass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
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
	if (fillFlag)
	{
		painter->fillPath(pathOfAll,QBrush(fillColor));
		painter->drawPath(pathOfAll);
	}
	painter->setPen(pen);
	painter->drawPath(pathOfAll);
}
void dxfClass::setFill(bool f,QColor c)
{
	fillFlag=f;
	fillColor=c;
}
void dxfClass::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=true;
	emit hoverEnterSignal();
	QGraphicsItem::update();
	QGraphicsItem::hoverEnterEvent(ev);
}
void dxfClass::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=false;
	emit hoverLeaveSignal();
	QGraphicsItem::update();
	QGraphicsItem::hoverLeaveEvent(ev);
}
void dxfClass::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
	emit mousePressedSignal(ev);
	QGraphicsItem::mousePressEvent(ev);
}
void dxfClass::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
	emit mouseReleasedSignal(ev);
	QGraphicsItem::mouseReleaseEvent(ev);
}
void dxfClass::loadFromDxfFile(QString fileName)
{
	QFileInfo fInfo(fileName);
	if (!fInfo.exists())
		uExit(tr("error"),tr("Could not find %1 file").arg(fileName));
//#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
//	if (!fInfo.isNativePath())
//#else
//	if (!fInfo.isWritable())
//#endif
//	{
//		//QString d=QString("qrc");
//		//QDir::current().mkdir(d);
//		//QString f=d+"/"+fInfo.fileName();
//		//QFile::copy(fileName,f);
//		//fileName=f;
//	}
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
	delete dxf;
	delete dxfData;
}

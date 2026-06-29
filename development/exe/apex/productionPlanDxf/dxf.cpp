#include "dxf.h"
dxfGroupOfGraphicsItemClass::dxfGroupOfGraphicsItemClass():QGraphicsItem()
{
}
dxfGroupOfGraphicsItemClass::dxfGroupOfGraphicsItemClass(QString fileName_,double rotationAngle,bool stickToZeroY,QColor color_):QGraphicsItem()
{
	fileName=fileName_;
	color=color_;
	loadFromDxfFile(rotationAngle,stickToZeroY);
}
void dxfGroupOfGraphicsItemClass::setColor(QColor col)
{
	for (int i=0;i<items.count();i++)
		items[i]->color=col;
}
QPainterPath dxfGroupOfGraphicsItemClass::shape() const
{
	return pathOfAll;
}
QRectF dxfGroupOfGraphicsItemClass::boundingRect() const
{
	return rectOfAll; 
}
void dxfGroupOfGraphicsItemClass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	for (int i=0;i<items.count();i++)
	{
		items[i]->myPaint(painter);
	}
	//painter->drawLine(QLine(0.0,-1.0,0.0,1.0));
	//painter->drawLine(QLine(-1.0,0.0,1.0,0.0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dxfGroupOfGraphicsItemClass::clearItems()
{
	items.clear();
	rectOfAll=QRectF();
	pathOfAll=QPainterPath();
}
void dxfGroupOfGraphicsItemClass::appendItem(dxfGroupOfGraphicsItemClass *gitem)
{
	for (int i=0;i<gitem->noOfItems();i++)
		appendItem(gitem->getItem(i));
}

void dxfGroupOfGraphicsItemClass::appendItem(myGraphicsItem *item)
{
	items.append(item);
	pathOfAll.addPath(item->path);
	rectOfAll=rectOfAll.united(item->path.boundingRect());
}
void dxfGroupOfGraphicsItemClass::appendItems(QList<myGraphicsItem*> items)
{
	for (int i=0;i<items.count();i++)
		appendItem(items[i]);
}
void dxfGroupOfGraphicsItemClass::loadFromDxfFile(double rotationAngle,bool stickToZeroY)
{
	clearItems();
	if (!(QFile(fileName).exists()))
		EXIT("Error",QString("Could not locate dxf file %1").arg(fileName));
 	dxfDataClass *dxfData=new dxfDataClass();//dxf reader
	DL_Dxf* dxf = new DL_Dxf();
	std::string fname=fileName.toLatin1();
	bool ans=dxf->in(fname,dxfData);
	if (!ans)
	{
		delete dxf;delete dxfData;return;
	}
	dxfData->calculate();
	QList<groupOfLinesClass> groupOfLines=dxfData->getAllGroupsOfLines();
	for (int i=0;i<groupOfLines.count();i++)
	{
		groupOfLines[i].rotate(transformOriginPoint(),rotationAngle);
		//int colorIndex=groupOfLines[i].dxfColorIndex;
		//if ((colorIndex<0)||(colorIndex>255))
		//	colorIndex=1;
		//if (colorIndex==7)
		//	colorIndex=0;
		appendItem(new myGraphicsItem(groupOfLines[i],color));
	}
	delete dxf;delete dxfData;
	if (stickToZeroY)
		setTransform(QTransform().translate(0.0,boundingRect().height()-boundingRect().bottom()),false);
}
QPainterPath dxfGroupOfGraphicsItemClass::getPath()
{
	return pathOfAll;
}
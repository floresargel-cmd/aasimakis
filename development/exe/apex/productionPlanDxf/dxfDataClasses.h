#ifndef DXFDATACLASSES_H
#define DXFDATACLASSES_H
#include <QtGui>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QGraphicsScene>
////
#include "pdG.h"
#include "dl_dxf.h"
class graphicsItemClass//:public QGraphicsItem
{
protected:
	//int					color;
	//QRectF				boundingBox;
	//QPainterPath		path;
	QList<QLineF*>		lines;
	//
	//bool				mouseOverBoundingBox;
	//bool				isHighlighted;
public:
	graphicsItemClass(/*QGraphicsItem *parentGraphicsItem=NULL,int color_=0*/){}
	graphicsItemClass &operator=(const graphicsItemClass& p)
	{
		lines=p.lines;
		//color=p.color;
		//boundingBox=p.boundingBox;
		//path=p.path;
		return *this;
	}
	//virtual QRectF boundingRect() const{return boundingBox;};
	//virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	QList<QLineF*> getLines(){return lines;}
	//QPainterPath shape() const {return path;}
	//void setIsHighlighted(bool h){isHighlighted=h;}
	//bool getIsHighlighted(){return isHighlighted;}
	////
	//virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	//virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};
///////////////////////////////////////////////////point///////////////////////////////////////////////////
class pointGraphicsItemClass:public graphicsItemClass
{
public:
	pointGraphicsItemClass(/*QGraphicsItemGroup *parentGroup=NULL*/);
	pointGraphicsItemClass(double x,double y/*,int c,QGraphicsItemGroup *parentGroup=NULL*/);
};
///////////////////////////////////////////////////point///////////////////////////////////////////////////
///////////////////////////////////////////////////line////////////////////////////////////////////////////
class lineGraphicsItemClass:public graphicsItemClass
{
public:
	lineGraphicsItemClass(/*QGraphicsItemGroup *parentGroup=NULL*/);
	lineGraphicsItemClass(double x1,double y1,double x2,double y2/*,int c,QGraphicsItemGroup *parentGroup=NULL*/);
};
///////////////////////////////////////////////////ellipse/////////////////////////////////////////////////
class ellipseGraphicsItemClass:public graphicsItemClass
{
public:
	ellipseGraphicsItemClass(/*QGraphicsItemGroup *parentGroup=NULL*/);
	ellipseGraphicsItemClass(double cx,double cy,double mx,double my,double ratio,double angle1,double angle2/*,int c,QGraphicsItemGroup *parentGroup=NULL*/);
};
///////////////////////////////////////////////////ellipse/////////////////////////////////////////////////
#endif

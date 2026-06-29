#ifndef DXFDATACLASSES_H
#define DXFDATACLASSES_H
#include <QtGui>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QGraphicsScene>
////
#include "ppG.h"
#include "dl_dxf.h"
class graphicsItemClass
{
protected:
	QList<QLineF*>		lines;
public:
	graphicsItemClass();
	~graphicsItemClass();
	graphicsItemClass &operator=(const graphicsItemClass& p)
	{
		lines=p.lines;
		return *this;
	}
	QList<QLineF*> getLines(){return lines;}
};
///////////////////////////////////////////////////point///////////////////////////////////////////////////
class pointGraphicsItemClass:public graphicsItemClass
{
public:
	pointGraphicsItemClass();
	~pointGraphicsItemClass();
	pointGraphicsItemClass(double x,double y);
};
///////////////////////////////////////////////////point///////////////////////////////////////////////////
///////////////////////////////////////////////////line////////////////////////////////////////////////////
class lineGraphicsItemClass:public graphicsItemClass
{
public:
	lineGraphicsItemClass();
	~lineGraphicsItemClass();
	lineGraphicsItemClass(double x1,double y1,double x2,double y2);
};
///////////////////////////////////////////////////ellipse/////////////////////////////////////////////////
class ellipseGraphicsItemClass:public graphicsItemClass
{
public:
	ellipseGraphicsItemClass();
	~ellipseGraphicsItemClass();
	ellipseGraphicsItemClass(double cx,double cy,double mx,double my,double ratio,double angle1,double angle2);
};
///////////////////////////////////////////////////ellipse/////////////////////////////////////////////////
#endif

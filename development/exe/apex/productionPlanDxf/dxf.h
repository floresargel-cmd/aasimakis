#ifndef DXF_H
#define DXF_H

#include <QtGui>
#include <QMainWindow>
#include <QAction>
#include <QList.h>

#include "dxfData.h"
#include "dxfGV.h"
#include "pdG.h"
class myGraphicsItem//:public QGraphicsItem
{
public:
	QColor							color;
	groupOfLinesClass		group;
	QPainterPath				path;
	myGraphicsItem(groupOfLinesClass group_,QColor color_)
	{
		group=group_;
		path=group.calculatePath();
		color=color_;
		color=color_;
	}
	myGraphicsItem &operator=(const myGraphicsItem& p)
	{
		group=p.group;
		color=p.color;
		path=p.path;
		return *this;
	}
	void calculatePath()
	{
		path=group.calculatePath();
	}
	void myPaint(QPainter *painter)
	{
		QPen normalPen=QPen(color,0.0,Qt::SolidLine);
		painter->setPen(normalPen);
		painter->drawPath(path);
	}
	bool isContainedInPolygon(QPolygonF polygon)
	{
		return group.isContainedInPolygon(polygon);
	}
	QVector<QPointF*> getCentersInRect(QRectF r)
	{
		return group.getCentersInRect(r);
	}
	QVector<QPointF> getPointsInRect(QRectF r)
	{
		return group.getPointsInRect(r);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class dxfGroupOfGraphicsItemClass : public QObject,public QGraphicsItem
{
protected:
	QString									fileName;
	//
	QColor									color;
	QList<myGraphicsItem*>	items;
	QRectF									rectOfAll;
	QPainterPath						pathOfAll;
Q_OBJECT
public:
	dxfGroupOfGraphicsItemClass();
  dxfGroupOfGraphicsItemClass(QString fileName_,double rotationAngle=0.0,bool stickToZeroY=false,QColor color_=QColor(200,200,200));
	void loadFromDxfFile(double rotationAngle,bool stickToZeroY);
	//
	QPainterPath shape() const;
	QRectF boundingRect() const;
	QColor getColor(){return color;}
	int noOfItems(){return items.count();}
	myGraphicsItem* getItem(int i){return items[i];}
	void setColor(QColor col);
	void clearItems();
	void appendItem(dxfGroupOfGraphicsItemClass *gitem);
	void appendItems(QList<myGraphicsItem*> items);
	void appendItem(myGraphicsItem *item);
	QPainterPath getPath();
	//
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	//
signals:
  void changed(bool);
};
#endif

#ifndef G_ITEM_H
#define G_ITEM_H

#include "Gsw.h"
#include "gData.h"
class gItemClass : public QObject,public QGraphicsItem
{
protected:
	QPen												pen;
	QBrush											fillBruch;
	QBrush											mouseOverFillBruch;
	//	
	QRectF											rectOfAll;
	QPainterPath								pathOfAll;
	QPainterPath								boundingRectPath;
	//
	bool												isMouseOver;
	bool												myIsVisible;
	//
Q_OBJECT
public:
	gItemClass();
	gItemClass(gItemClass *p,float dx,float dy);
	gItemClass(QString fileName,QPen pen_,QObject* o=NULL,QBrush mouseOverFillBruch_=Qt::NoBrush,QBrush fillBruch_=Qt::NoBrush);
	~gItemClass();
	void myRotate(float xCenter,float yCenter,float angle);
	void loadFromDxfFile(QString fileName);
	//
	QPainterPath shape() const;
	QRectF boundingRect() const;
	//
	void setPen(QPen p);
	void setFillBruch(QBrush b);
	void setPenWidth(float w);
	void showGraphics(bool s);
	QPainterPath getPath();
	void addPath(QPainterPath p);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *ev);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *ev);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
signals:
	void hoverLeaveSignal();
	void hoverEnterSignal();
	void mousePressedSignal(QGraphicsSceneMouseEvent*);
	void mouseReleasedSignal(QGraphicsSceneMouseEvent*);
};											
#endif

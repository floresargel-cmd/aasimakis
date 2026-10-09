#ifndef G_ITEM_H
#define G_ITEM_H

#include "../G.h"
#include "gData.h"
class dxfClass:public QObject,public QGraphicsItem
{
protected:
	QPen												pen;
	QColor											mouseOverFillColor;
	bool												mouseOverFlag;
	bool												hideWhenMouseNotOver;
	//
	QRectF											rectOfAll;
	QPainterPath								pathOfAll;
	//
	QPainterPath								boundingRectPath;
	//
	bool												isMouseOver;
	bool												myIsVisible;
	bool												fillFlag;
	QColor											fillColor;
	//
Q_OBJECT
public:
	dxfClass();
	dxfClass(dxfClass *p,float dx,float dy,QObject* o);
	dxfClass(QString fileName,QPen pen_,bool mouseOverFlag_,bool hideWhenMouseNotOver_,QColor mouseOverFillColor_,QObject* o=NULL);
	~dxfClass();
	void myRotate(float xCenter,float yCenter,float angle);
	void loadFromDxfFile(QString fileName);
	void setPen(QPen p);
	void setMouseOverFillColor(QColor c);
	void setFill(bool f,QColor c);
	//
	QPainterPath shape() const;
	QRectF boundingRect() const;
	void setMouseOverFlag(bool f);
	//
	void showGraphics(bool s);
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

#ifndef PSC_H
#define PSC_H

#ifdef stationsExe
#include "Gc.h"
#endif
#include "serv.h"
#include "gItem.h"
#include "gText.h"
class gProximitySwitchClass :public QObject, public QGraphicsItem			
{
protected:
	QString																plcIpAdress;
	QString																plcTable;
	int																		plcIndex;
	int																		plcBit;
	bool																	isActive;
	bool																	isMouseOver;
	//
	QRectF																myBoundRect;
	float																	offset;
	QPainterPath													myPath;
	QPen																	activePen;
	QPen																	inActivePen;
Q_OBJECT
public:
	gProximitySwitchClass(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset_,QGraphicsScene *myScene,float width=700.f,QString description="Proximity Switch",bool rotate90=false);
	~gProximitySwitchClass();
	void setTransform(QTransform tr);
	void setPens(QPen activePen_,QPen inActivePen_);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	QRectF boundingRect() const;
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *ev);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev);
};											
#endif


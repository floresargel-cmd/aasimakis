#ifndef gProxItem_H
#define gProxItem_H

#include "Gf.h"
#include "serv.h"
#include "gItemP.h"
#include "gText.h"
class gProximitySwitchClass :public QObject,public QGraphicsItem
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
	int																		typeEnum;
public:
	enum//typeEnum
	{
		typeHorizontal=1,
		typeVertical=2,
		typeRotated45=3,
		typeLightCurtain45=4,
	};
Q_OBJECT
public:
	gProximitySwitchClass(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset_,QGraphicsScene *myScene,int typeEnum_,float x,float y,float width=450.f,QString title="Photocell");
	~gProximitySwitchClass();
	void setPens(QPen activePen_,QPen inActivePen_);
	void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
	QRectF boundingRect() const;
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *ev);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev);
};
#endif

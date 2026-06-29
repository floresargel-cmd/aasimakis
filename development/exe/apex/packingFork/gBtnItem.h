#ifndef gBtnItem_h
#define gBtnItem_h

#include "Gf.h"
#include "serv.h"
#include "gItemP.h"
#include "gText.h"
class gBtnItemClass :public QObject,public QGraphicsItem			
{
public:
	enum class typeEnumClass
	{
		normal=1,
	};
protected:
	tuxipClass														*tuxip;
	QString																plcCommandTable;
	int																		plcCommandIndex;
	int																		plcCommandNumber;
	QString																plcActiveTable;
	int																		plcActiveIndex;
	int																		plcActiveBit;
	QString																label;
	bool																	isActive;
	bool																	inMaintenanceMode;
	bool																	isMouseOver;
	float																	rotationAngle;
	//
	float																	offset;
	QPainterPath													originalPath;
	QPainterPath													currentPath;
	QPen																	activePen;
	QPen																	inActivePen;
	typeEnumClass													typeEnum=typeEnumClass::normal;
public:
Q_OBJECT
public:
	gBtnItemClass(tuxipClass *tuxip_,QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,QString plcActiveTable_,int plcActiveIndex_,int plcActiveBit_,QGraphicsScene *myScene,QString label_,float rotationAngle_=0,float offset_=0.,float size=350,typeEnumClass typeEnum_=typeEnumClass::normal);
	~gBtnItemClass();
	void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
	QRectF boundingRect() const;
	void setMaintenanceMode(bool m);
	void setActvePen(QPen ap);
	void setInactvePen(QPen ip);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setPos(float x,float y);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *ev);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *ev);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
};
#endif


#ifndef PROXITEM_H
#define PROXITEM_H

#include "Gsw.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
class proxItemClass: public QObject
{
	QGraphicsScene							*scene;
	gItemClass									*onItem;
	gItemClass									*offItem;
	QString											onString;
	QString											offString;
	QString											plcIp;
	QString											plcTable;
	int													plcIndex;
	int													plcBit;
	float												xPos;
	float												yPos;
	QTransform									txtTransformation;
	QGraphicsSimpleTextItem 		*graphicsItemText;
	Q_OBJECT;
public:
	proxItemClass(QGraphicsScene *scene_,gItemClass *onItem_,gItemClass *offItem_,QString onString_,QString offString_,QString plcIp_,QString plcTable_,int plcIndex_,int plcBit_,float xPos,float yPos,QString toolTip=QString(),QObject *o=NULL);
	~proxItemClass();
	void setPos(float x,float y);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
};
#endif

#ifndef PROXITEM_H
#define PROXITEM_H
#ifdef stationsExe
#include "Gc.h"
#endif
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "gItem.h"
class proxItemClass: public QObject		
{
	QGraphicsScene							*scene;
	dxfClass										*onItem;
	dxfClass										*offItem;
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
	proxItemClass(QGraphicsScene *scene_,dxfClass *onItem_,dxfClass *offItem_,QString onString_,QString offString_,QString plcIp_,QString plcTable_,int plcIndex_,int plcBit_,float xPos,float yPos,QString toolTip=QString(),QObject *o=NULL);
	~proxItemClass();
	void setPos(float x,float y);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
};											
#endif

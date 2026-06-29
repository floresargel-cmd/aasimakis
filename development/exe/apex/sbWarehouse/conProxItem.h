#ifndef conProxItem_H
#define conProxItem_H
class proxItemClass;

#include "../Gs.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
class conProxItemClass: public QObject		
{
	QGraphicsScene							*scene;
	gItemClass *onItem;
	gItemClass *offItem;
	QString											onString;
	QString											offString;
	int													plcIndex;
	int													plcBit;
	float												xPos;
	float												yPos;
	QTransform									txtTransformation;
	QGraphicsSimpleTextItem 		*graphicsItemText;
	QString											myIp;
	Q_OBJECT;
public:										
	conProxItemClass(QString myIp_,QGraphicsScene *scene_,gItemClass *onItem_,gItemClass *offItem_,QString onString_,QString offString_,int plcIndex_,int plcBit_,float xPos,float yPos,QString toolTip=QString(),QObject *o=NULL);
	~conProxItemClass();
	void setPos(float x,float y);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
};											
#endif

#ifndef PROXITEM_H
#define PROXITEM_H

#include "Gd.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "posDlg.h"
class proxItemClass: public QObject
{
	QGraphicsScene							*scene;
	dxfGroupOfGraphicsItemClass *onItem;
	dxfGroupOfGraphicsItemClass *offItem;
	QString											onString;
	QString											offString;
	int													plcIndex;
	int													plcBit;
	float												xPos;
	float												yPos;
	QTransform									txtTransformation;
	QGraphicsSimpleTextItem 		*graphicsItemText;
	Q_OBJECT;
public:
	proxItemClass(QGraphicsScene *scene_,dxfGroupOfGraphicsItemClass *onItem_,dxfGroupOfGraphicsItemClass *offItem_,QString onString_,QString offString_,int plcIndex_,int plcBit_,float xPos,float yPos,QString toolTip=QString(),QObject *o=NULL);
	~proxItemClass();
	void setPos(float x,float y);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
};
#endif

#ifndef TRIPLEPOS_H
#define TRIPLEPOS_H

#include "Gd.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
class triplePosClass : public posClass
{
	posClass										*secondPos;
//	posClass										*thirdPos;
	Q_OBJECT
public:
	triplePosClass(int plcId,int basketNum,QString description,QGraphicsScene *scene,dxfGroupOfGraphicsItemClass *mouseOver,dxfGroupOfGraphicsItemClass *cell,dxfGroupOfGraphicsItemClass *fullBasket,dxfGroupOfGraphicsItemClass *emptyBasket,dxfGroupOfGraphicsItemClass *locked,QWidget *p=NULL);
	~triplePosClass();
	void setPlcPos(float x,float y,float z);
	void setXOffset(float x0,float x1,float x2);
	void setYOffset(float y0,float y1,float y2);
	void loadFromDatabase();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:
	void mainPosChangedSlot();
	void secondPosChangedSlot();
	void thirdPosChangedSlot();
};
#endif

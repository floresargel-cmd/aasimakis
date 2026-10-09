#ifndef STE1_H
#define STE1_H

#include "Gd.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "plcBit.h"
class workStationClass : public posClass
{
	bool isReadyToReturn;
	Q_OBJECT
public:
	workStationClass(int type,int plcId,int basketNum,QString description,int column,int row,float plcXPos,float plcYPos,float plcZPos,float xCorr,float yCorr,float zCorr,QGraphicsScene *scene,dxfGroupOfGraphicsItemClass *mouseOver,dxfGroupOfGraphicsItemClass *cell,dxfGroupOfGraphicsItemClass *fullBasket,dxfGroupOfGraphicsItemClass *emptyBasket,dxfGroupOfGraphicsItemClass *locked,QWidget *p);
	~workStationClass();
	bool getCanGetBasket();
	int getNextBasketToComeOut();
	void setIsReadyToReturn(bool r);
	int getBasketToReturn();
signals:
	void basketChangedSignal(int b);
public slots:
	void loadFromDatabaseSlot();
	void basketChangedSlot(int oldBasket,int newBasket);
};
#endif

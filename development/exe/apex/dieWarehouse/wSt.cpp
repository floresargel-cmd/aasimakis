#include "pch.h"
#include "wSt.h"
workStationClass::workStationClass(int type,int plcId,int basketNum,QString description,int column,int row,float plcXPos,float plcYPos,float plcZPos,float xCorr,float yCorr,float zCorr,QGraphicsScene *scene,dxfGroupOfGraphicsItemClass *mouseOver,dxfGroupOfGraphicsItemClass *cell,dxfGroupOfGraphicsItemClass *fullBasket,dxfGroupOfGraphicsItemClass *emptyBasket,dxfGroupOfGraphicsItemClass *locked,QWidget *p):
posClass(type,plcId,basketNum,description,column,row,plcXPos,plcYPos,plcZPos,xCorr,yCorr,zCorr,scene,mouseOver,cell,fullBasket,emptyBasket,locked,p)
{
	isReadyToReturn=false;
	connect(this,SIGNAL(basketChangedSignal(int,int)),this,SLOT(basketChangedSlot(int,int)));
}
workStationClass::~workStationClass()
{
}
bool workStationClass::getCanGetBasket()
{
	loadFromDatabase();
	return (basketNum==0)&&(getFirst(QString("select toPos from missions where toPos=%1").arg(exitPlcId),db).toInt()!=exitPlcId);
}
void workStationClass::setIsReadyToReturn(bool r)
{
	isReadyToReturn=r;
}
int workStationClass::getBasketToReturn()
{
	if (isReadyToReturn)
		return basketNum;
	return 0;
}
void workStationClass::basketChangedSlot(int oldBasket,int newBasket)
{
	isReadyToReturn=false;
	if (basketNum!=0)
		execQuery(QString("delete from %1 where basket=%2").arg(exportsTable).arg(basketNum),db);
	emit basketChangedSignal(newBasket);
}
void workStationClass::loadFromDatabaseSlot()
{
	posClass::loadFromDatabase();
}
int workStationClass::getNextBasketToComeOut()
{
	return getFirst(QString("select top 1 basket from %1 order by priority").arg(exportsTable),db).toInt();
}

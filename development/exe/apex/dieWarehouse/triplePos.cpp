#include "pch.h"
#include "triplePos.h"
triplePosClass::triplePosClass(int plcId,int basketNum,QString description,QGraphicsScene *scene,dxfGroupOfGraphicsItemClass *mouseOver,dxfGroupOfGraphicsItemClass *cell,dxfGroupOfGraphicsItemClass *fullBasket,dxfGroupOfGraphicsItemClass *emptyBasket,dxfGroupOfGraphicsItemClass *locked,QWidget *p):
posClass(cellTypeBig,plcId,basketNum,description,0,0,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,scene,mouseOver,cell,fullBasket,emptyBasket,locked,p)
{
	setVisible(false);
	mouseOver->setMouseOverFlag(false);
	mouseOver->showGraphics(false);
	secondPos=new posClass(cellTypeBig,plcId,basketNum,description,0,0,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,scene,mouseOver,cell,fullBasket,emptyBasket,locked,p);
//	thirdPos=new posClass(cellTypeBig,plcId,basketNum,description,0,0,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,scene,mouseOver,cell,fullBasket,emptyBasket,locked,p);
	connect(this,SIGNAL(posChangedSignal()),this,SLOT(mainPosChangedSlot()));
}
triplePosClass::~triplePosClass()
{
}
void triplePosClass::setXOffset(float x0,float x1,float x2)
{
	posClass::setXOffset(x0);
	secondPos->setXOffset(x1);
//	thirdPos->setXOffset(x2);
}

void triplePosClass::setYOffset(float y0,float y1,float y2)
{
	posClass::setYOffset(y0);
	secondPos->setYOffset(y1);
//	thirdPos->setYOffset(y2);
}
void triplePosClass::setPlcPos(float x,float y,float z)
{
	posClass::setPlcPos(x,y,0.f);
	secondPos->setPlcPos(x,y,z);
//	thirdPos->setPlcPos(x,y,z);
}
void triplePosClass::mainPosChangedSlot()
{
	secondPos->loadFromDatabase();
//	thirdPos->loadFromDatabase();
}
void triplePosClass::secondPosChangedSlot()
{
	posClass::loadFromDatabase();
//	thirdPos->loadFromDatabase();
}
void triplePosClass::thirdPosChangedSlot()
{
	posClass::loadFromDatabase();
	secondPos->loadFromDatabase();
}
void triplePosClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==dieIp)
	{
	}
}
void triplePosClass::loadFromDatabase()
{
	posClass::loadFromDatabase();
	secondPos->loadFromDatabase();
//	thirdPos->loadFromDatabase();
}

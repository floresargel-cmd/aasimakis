#include "pch.h"
#include "triplePos.h"
triplePosClass::triplePosClass(int posId,int cassetteNum,QString description
	,gItemClass *mouseOverItem,gItemClass *cellItem,gItemClass *cassetteItem,gItemClass *sbBlueItem,gItemClass *profilesItem,gItemClass *lockedItem,gItemClass *activeFromItem,gItemClass *activeToItem
	,QWidget *p):
posClass(posId,false,cassetteNum,description,0,0,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,false,0,0,0.,mouseOverItem,cellItem,cassetteItem,sbBlueItem,profilesItem,lockedItem,activeFromItem,activeToItem,true,p)
{
	setVisible(false);
	secondPos=new posClass(posId,false,cassetteNum,description,0,0,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,false,0,0,0.,mouseOverItem,cellItem,cassetteItem,sbBlueItem,profilesItem,lockedItem,activeFromItem,activeToItem,true,p);
	secondPos->setGraphicsZ(-1);
	thirdPos=new posClass(posId,false,cassetteNum,description ,0,0,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,false,0,0,0.,mouseOverItem,cellItem,cassetteItem,sbBlueItem,profilesItem,lockedItem,activeFromItem,activeToItem,true,p);
	thirdPos->setGraphicsZ(-1);
	posClass::loadFromDatabase();
	connect(this,SIGNAL(posChangedSignal()),this,SLOT(mainPosChangedSlot()));
}
triplePosClass::~triplePosClass()
{
}
void triplePosClass::setXOffset(float x0,float x1,float x2)
{
	posClass::setXOffset(x0);
	secondPos->setXOffset(x1);
	thirdPos->setXOffset(x2);
}

void triplePosClass::setYOffset(float y0,float y1,float y2)
{
	posClass::setYOffset(y0);
	secondPos->setYOffset(y1);
	thirdPos->setYOffset(y2);
}
void triplePosClass::setPlcPos(float x,float y,float z)
{
	posClass::setPlcPos(x,y,0.f);
	secondPos->setPlcPos(x,y,z);
	thirdPos->setPlcPos(x,y,z);
}
void triplePosClass::mainPosChangedSlot()
{
	secondPos->loadFromDatabase();
	thirdPos->loadFromDatabase();
}
void triplePosClass::secondPosChangedSlot()
{
	posClass::loadFromDatabase();
	thirdPos->loadFromDatabase();
}
void triplePosClass::thirdPosChangedSlot()
{
	posClass::loadFromDatabase();
	secondPos->loadFromDatabase();
}
void triplePosClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==sbWarehouseIp)
	{
	}
}
void triplePosClass::loadFromDatabase()
{
	posClass::loadFromDatabase();
	secondPos->loadFromDatabase();
	thirdPos->loadFromDatabase();
}

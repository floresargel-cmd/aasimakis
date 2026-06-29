#include "pch.h"
#include "np.h"
newPacketPositionClass::newPacketPositionClass(int posId,QString name,QGraphicsScene *scene,dxfClass *mouseOverItem,dxfClass *lockedItem,dxfClass *sbItem,QWidget *p):
	packetPositionClass(posId,name,0,0,0,0,0,scene,mouseOverItem,lockedItem,sbItem,p)
{
	loadFromDatabase();
}
newPacketPositionClass::~newPacketPositionClass()
{
}
void newPacketPositionClass::saveToDatabase()
{
	packetPositionClass::saveToDatabase();
}
void newPacketPositionClass::loadFromDatabase()
{
	packetPositionClass::loadFromDatabase();
}
//void newPacketPositionClass::loadContentsFromDatabase()
//{
//	packetPositionClass::loadContentsFromDatabase();
//}

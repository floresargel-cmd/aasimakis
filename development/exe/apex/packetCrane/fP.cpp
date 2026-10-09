#include "pch.h"
#include "fP.h"
forkPositionClass::forkPositionClass(QGraphicsScene *scene,dxfClass *mouseOverItem,dxfClass *lockedItem,dxfClass *sbItem,QWidget *p):
	packetPositionClass(POS_ID_FORK,tr("Fork"),0,0,0,0,0,scene,mouseOverItem,lockedItem,sbItem,p)
{
	loadFromDatabase();
}
forkPositionClass::~forkPositionClass()
{
}
void forkPositionClass::saveToDatabase()
{
	packetPositionClass::saveToDatabase();
}
void forkPositionClass::loadFromDatabase()
{
	packetPositionClass::loadFromDatabase();
}
//void forkPositionClass::loadContentsFromDatabase()
//{
//	packetPositionClass::loadContentsFromDatabase();
//}

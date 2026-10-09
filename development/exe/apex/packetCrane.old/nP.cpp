#include "np.h"
newPacketPositionClass::newPacketPositionClass(int posId,GLfloat x,GLfloat y,GLfloat z,QWidget *p):packetPositionClass(posId,x,y,z,p)
{
	loadFromDatabase();
}
newPacketPositionClass::~newPacketPositionClass()
{
}
void newPacketPositionClass::mouseClicked(QColor pickedColor)
{
	if (pickedColor==pickingColor)
	{
	}
}
void newPacketPositionClass::saveToDatabase()
{
	packetPositionClass::saveToDatabase();
}
void newPacketPositionClass::loadFromDatabase()
{
	packetPositionClass::loadFromDatabase();
}
void newPacketPositionClass::loadContentsFromDatabase()
{
	packetPositionClass::loadContentsFromDatabase();
}

#include "fP.h"
forkPositionClass::forkPositionClass(QWidget *p):packetPositionClass(POS_ID_FORK,0.0f,0.0f,0.0f,p)
{
	loadFromDatabase();
	setOpenGlX(0.0f);
	setOpenGlY(0.0f);
	setOpenGlZ(0.0f);
}
forkPositionClass::~forkPositionClass()
{
}
void forkPositionClass::setOpenGlX(GLfloat v)
{
	packetPositionClass::setOpenGlX(v-2.5f);
}
void forkPositionClass::setOpenGlY(GLfloat v)
{
	packetPositionClass::setOpenGlY(v-71.2f);
}
void forkPositionClass::setOpenGlZ(GLfloat v)
{
	packetPositionClass::setOpenGlZ(v+6.3f);
}
void forkPositionClass::mouseClicked(QColor pickedColor)
{
	if (pickedColor==pickingColor)
	{
	}
}
void forkPositionClass::saveToDatabase()
{
	packetPositionClass::saveToDatabase();
}
void forkPositionClass::loadFromDatabase()
{
	packetPositionClass::loadFromDatabase();
}
void forkPositionClass::loadContentsFromDatabase()
{
	packetPositionClass::loadContentsFromDatabase();
}

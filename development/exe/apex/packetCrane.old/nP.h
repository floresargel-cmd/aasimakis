#ifndef NP_H
#define NP_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "pcG.h"
#include "../common/apexG.h"
#include "p.h"
#include "cellCont.h"
class newPacketPositionClass:public packetPositionClass
{
	Q_OBJECT
public:
	newPacketPositionClass(int posId,GLfloat x,GLfloat y,GLfloat z,QWidget *p);
	~newPacketPositionClass();
	//
	void mouseClicked(QColor pickedColor);
	//
	void saveToDatabase();
	void loadFromDatabase();
	void loadContentsFromDatabase();
};
#endif

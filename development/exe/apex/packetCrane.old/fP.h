#ifndef FP_H
#define FP_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "pcG.h"
#include "../common/apexG.h"
#include "p.h"
#include "cellCont.h"
class forkPositionClass:public packetPositionClass
{
	Q_OBJECT
public:
	forkPositionClass(QWidget *p);
	~forkPositionClass();
	//
	void mouseClicked(QColor pickedColor);
	void setOpenGlX(GLfloat v);
	void setOpenGlY(GLfloat v);
	void setOpenGlZ(GLfloat v);
	//
	void saveToDatabase();
	void loadFromDatabase();
	void loadContentsFromDatabase();
};
#endif

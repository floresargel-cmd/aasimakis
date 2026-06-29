#ifndef P_H
#define P_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "pcG.h"
#include "../common/apexG.h"
#include "op.h"

class packetPositionClass:public QWidget			
{
protected:
	int															posId;
	int															packetId;
	int															packetDestination;
	int															storagingTypeOfPacket;
	bool														isOnSb;
	//
	int															packetNoOfWoods;
	int															packetLength;
	int															packetWidth;
	int															packetHeight;
	//
	GLfloat													openGlX;
	GLfloat													openGlY;
	GLfloat													openGlZ;
	//
	bool														isMouseOver;
	QColor													pickingColor;
//
	Q_OBJECT
public:
	packetPositionClass(int posId_,GLfloat x,GLfloat y,GLfloat z,QWidget *p);
	~packetPositionClass();
	//
	void draw(QColor color=QColor(0,0,0),bool drawCassette=false);
	void drawForPicking();
	void mouseOver(QColor pickedColor);
	//
	void setOpenGlX(GLfloat v);
	void setOpenGlY(GLfloat v);
	void setOpenGlZ(GLfloat v);
	//
	void saveToDatabase();
	void loadFromDatabase();
	void loadContentsFromDatabase();
	int getPosId();
	int getPacketTypeForPlc();
	int getPositionIdForForkPlc();
	int getPositionIdForCranePlc();
	void setPacketId(int p,bool check=true);
	int getPacketId();
	int getPacketDestination();
	int getStoragingTypeOfPacket();
	int getPacketPlacingType();
	bool getIsOnSB();
	bool hasPacket();
	int getNumberOfWoods();
signals:
	void openGlChangedSignal();
};
#endif

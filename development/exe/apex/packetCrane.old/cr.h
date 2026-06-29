#ifndef CR_H
#define CR_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "GL/GLU.h"
class allPosisionsClass;
class craneClass;
class plcClass;
#include "pcG.h"
#include "plc.h"
#include "convP.h"
#include "cP.h"
#include "ap.h"
class craneClass:public QWidget			
{
	QTimer																animateTimer;
	cellPositionClass											*fromPos;
	cellPositionClass											*toPos;
	//
	conveyorPositionClass									*position24;//west conveyor
	conveyorPositionClass									*position25;//east conveyor
	cellPositionClass											*positionCrane;//crane
	allPosisionsClass											*allPosisions;
	Q_OBJECT
	openGLPartClass												*horizontalOpenGl;
	openGLPartClass												*verticalOpenGl;

	float																	lastPlcXInMeters;
	float																	lastPlcYInMeters;
	float																	lastPlcZInMeters;

	tcpServerClass												*cranePlcTcpServer;
	//QStringList														pendingMessagesToPlc;//this is used only in the start when there is a mission to be resumed and the connection with the plc is not ready yet
	//QTimer																pendingMessagesToPlcTimer;
	//////
	bool																	hasActiveMission;
	bool																	isCarryingCassette;
	//////
	float						plcXInMeters;
	float						plcYInMeters;
	float						plcZInMeters;
	QVector<float>	plcXVelocityArray;
	QVector<float>	plcYVelocityArray;
	QVector<float>	plcZVelocityArray;
	float						plcXVelocity;
	float						plcYVelocity;
	float						plcZVelocity;
	QTime						animateTime;
	QTime						plcXTime;
	QTime						plcYTime;
	QTime						plcZTime;
	float						openGlX;
	float						openGlY;
	float						openGlZ;
	//
	int							plcCassetteOnCrane;
	int							plcStatusEnum;
	enum{
		PLC_STATUS_IDLE=0,
		PLC_STATUS_EXECUTING,
		PLC_STATUS_FINISHED_OK,
	};
public:
	craneClass(QWidget *p);
	~craneClass();
	void setPlcClass(plcClass *plc);
	//
	void setPlcX(float x);
	void setPlcY(float y);
	void setPlcZ(float z);
	float	getPlcX();
	float	getPlcY();
	float	getPlcZ();	
	//
	void draw();
	void drawForPicking();
	void mouseOver(QColor pickedColor);
	void mouseClicked(QColor pickedColor);
	//
	void setAllPositionsClass(allPosisionsClass *a);
	//
	void loadFromDatabase();
	void transferCassette(cellPositionClass *fromPos,cellPositionClass *toPos);
	void newMission(int fromId,int toId,bool resume,bool manualExportOfCassette);
	void newMission(cellPositionClass *from,cellPositionClass *to,bool resume,bool manualExportOfCassette);
	//void resumeMission(cellPositionClass *from,cellPositionClass *to);
	//void resumeMission(int fromId,int toId);
	void takeCassetteOutForPacketIn(packetPositionClass *p);
	void returnNoNeededCassette();
	bool isIdle();
	//
	cellPositionClass *getThePositionThatTheActiveCassetteIs();
	//
	conveyorPositionClass *getConveyorPosition24();
	conveyorPositionClass *getConveyorPosition25();
	cellPositionClass   	*getCranePosition();
	//
	void plcStatusChanged(int status);
	void plcCassetteOnCraneChanged(int c);
	void plcCraneIsCommingOutWithCassette();
public slots:								
	void animateTimerSlot();						
	//void pendingMessagesToPlcTimerSlot();
	void openGlObjectChangedSlot();
	private:
signals:
	void openGlChangedSignal();
};						

#endif

#ifndef FR_H
#define FR_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "GL/GLU.h"
class allPosisionsClass;
class craneClass;

#include "pcG.h"
#include "../common/apexG.h"
#include "nP.h"
#include "fP.h"
#include "ap.h"
#include "convP.h"

class forkClass:public QWidget			
{
	QTimer																animateTimer;
	packetPositionClass										*fromPos;
	packetPositionClass										*toPos;
	//
	newPacketPositionClass								*positionSb;//position before strapping
	newPacketPositionClass								*positionPa;//position after strapping
	newPacketPositionClass								*positionIm;//import position 
	forkPositionClass											*positionForkLift;//forklift
	conveyorPositionClass									*position24;//west conveyor
	conveyorPositionClass									*position25;//east conveyor
	packetPositionClass										*positionSbExit;
	packetPositionClass										*positionPacketExit;
	packetPositionClass										*positionStoreExit;

	allPosisionsClass											*allPosisions;
	Q_OBJECT
	openGLPartClass												*longStrockOpenGl;
	openGLPartClass												*shortStrockOpenGl;
	openGLPartClass												*verticalOpenGl;

	float																	lastPlcXInMeters;
	float																	lastPlcYInMeters;
	float																	lastPlcZInMeters;

	tcpServerClass												*forkPlcTcpServer;
	//QStringList														pendingMessagesToPlc;//this is used only in the start when there is a mission to be resumed and the connection with the plc is not ready yet
	//QTimer																pendingMessagesToPlcTimer;
	//
	bool																	hasActiveMission;
	bool																	isCarryingPacket;
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
	int							plcStatusEnum;
	enum{
		PLC_STATUS_IDLE=0,
		PLC_STATUS_EXECUTING,
		PLC_STATUS_WITH_THE_PACKET,
		PLC_STATUS_FINISHED_OK,
		PLC_STATUS_FINISHED_ABORD,
	};

public:
	forkClass(QWidget *p);
	~forkClass();
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
	void loadFromDatabase();
	void transferPacket(packetPositionClass *fPos,packetPositionClass *tPos);
	void newMission(packetPositionClass *from,packetPositionClass *to,bool resume);
	void newMission(int f,int t);
	//void resumeMission(packetPositionClass *from,packetPositionClass *to);
	bool isIdle();
	//
	newPacketPositionClass* getPositionSb();
	newPacketPositionClass* getPositionPa();
	newPacketPositionClass* getPositionIm();
	forkPositionClass* getForkPosition();
	packetPositionClass *getThePositionThatTheActivePacketIs();
	void setPosition24(conveyorPositionClass *p);
	void setPosition25(conveyorPositionClass *p);
	void setPositionSbExit(packetPositionClass *p);
	void setPositionPacketExit(packetPositionClass *p);
	void setPositionStoreExit(packetPositionClass *p);

	bool isPosActiveTo(int posId);
	//
	void plcStatusChanged(int status);
public slots:								
	void animateTimerSlot();						
	//void pendingMessagesToPlcTimerSlot();
	void openGlObjectChangedSlot();
	private:
signals:
	void openGlChangedSignal();
};						

#endif

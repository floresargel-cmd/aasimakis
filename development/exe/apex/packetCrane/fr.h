#ifndef FR_H
#define FR_H

class allPosisionsClass;
class craneClass;

#include "pcG.h"
#include "../common/apexG.h"
#include "nP.h"
#include "fP.h"
#include "ap.h"
#include "convP.h"
#include "gView.h"
class forkClass:public QWidget			
{
	packetPositionClass										*fromPos;
	packetPositionClass										*toPos;
	//
#ifdef keepPacketCraneOldFunctionality
	newPacketPositionClass								*positionSb;//position before strapping
	newPacketPositionClass								*positionPa;//position after strapping
#endif
	newPacketPositionClass								*positionIm;//import position 
	forkPositionClass											*positionForkLift;//forklift
	conveyorPositionClass									*position24;//west conveyor
	conveyorPositionClass									*position25;//east conveyor
#ifdef keepPacketCraneOldFunctionality
	packetPositionClass										*positionSbExit;
	packetPositionClass										*positionPacketExit;
	packetPositionClass										*positionStoreExit;
#endif
	allPosisionsClass											*allPosisions;
	Q_OBJECT
	tcpServerClass												*forkPlcTcpServer;
	bool																	hasActiveMission;
	bool																	isCarryingPacket;
	//////
	float						plcXInMeters;
	float						plcYInMeters;
	float						plcZInMeters;
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
	forkClass(QGraphicsScene *scene,QWidget *p);
	~forkClass();
	void setPlcClass(plcClass *plc);
	//
	void loadFromDatabase();
	void transferPacket(packetPositionClass *fPos,packetPositionClass *tPos);
	void newMission(packetPositionClass *from,packetPositionClass *to,bool resume);
	void newMission(int f,int t);
	//void resumeMission(packetPositionClass *from,packetPositionClass *to);
	bool isIdle();
	//
	void setPlcX(float x);
	void setPlcY(float y);
	void setPlcZ(float z);
	float	getPlcX();
	float	getPlcY();
	float	getPlcZ();	
#ifdef keepPacketCraneOldFunctionality
	newPacketPositionClass* getPositionSb();
	newPacketPositionClass* getPositionPa();
#endif
	newPacketPositionClass* getPositionIm();
	forkPositionClass* getForkPosition();
	packetPositionClass *getThePositionThatTheActivePacketIs();
	void setPosition24(conveyorPositionClass *p);
	void setPosition25(conveyorPositionClass *p);
#ifdef keepPacketCraneOldFunctionality
	void setPositionSbExit(packetPositionClass *p);
	void setPositionPacketExit(packetPositionClass *p);
	void setPositionStoreExit(packetPositionClass *p);
#endif

	bool isPosActiveTo(int posId);
	//
	void plcStatusChanged(int status);
	bool isInWareHouseArea();
	bool isGoingToWarehouse();
public slots:
	void onMisionTimer();
	void onPacketDataChangedSlot(int cell);
};						

#endif

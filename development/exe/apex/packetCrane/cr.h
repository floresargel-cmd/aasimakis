#ifndef CR_H
#define CR_H

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
	cellPositionClass											*fromPos=nullptr;
	cellPositionClass											*toPos=nullptr;
	//
	conveyorPositionClass									*position24;//west conveyor
	conveyorPositionClass									*position25;//east conveyor
	cellPositionClass											*positionCrane;//crane
	cellPositionClass											*positionCraneUp;//crane
	cellPositionClass											*positionCraneDown;//crane
	
	conveyorPositionClass									*stationExit1;
	conveyorPositionClass									*stationImport;
	conveyorPositionClass									*stationExit2;
	conveyorPositionClass									*stationBackExit1;
	conveyorPositionClass									*stationBackImport;
	conveyorPositionClass									*stationBackExit2;

	conveyorPositionClass									*packingExit1;
	conveyorPositionClass									*packingImport;
	conveyorPositionClass									*packingExit2;
	conveyorPositionClass									*packingBackExit1;
	conveyorPositionClass									*packingBackImport;
	conveyorPositionClass									*packingBackExit2;

	allPosisionsClass											*allPosisions;

	Q_OBJECT

	tcpServerClass												*cranePlcTcpServer;
	//////
	bool																	hasActiveMission;
	bool																	isCarryingCassette;
	//////
	float																	plcXInMeters;
	float																	plcYInMeters;
	float																	plcZInMeters;
	//
	int																		plcCassetteOnCrane;
	int																		plcStatusEnum;
	enum{
		PLC_STATUS_IDLE=0,
		PLC_STATUS_EXECUTING=1,
		PLC_STATUS_FINISHED_OK=2,
		PLC_STATUS_ABORT=4,
	};
public:
	craneClass(QGraphicsScene *scene,QWidget *p);
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
	void setAllPositionsClass(allPosisionsClass *a);
	//
	void loadFromDatabase();
	void transferCassette(int fromPos,int toPos,bool updatePlc);
	void setFromToPos(cellPositionClass *from,cellPositionClass *to);
	void transferCassette(cellPositionClass *fromPos,cellPositionClass *toPos,bool updatePlc);
	bool newMission(int fromId,int toId,bool resume,bool manualExportOfCassette);
	bool newMission(cellPositionClass *from,cellPositionClass *to,bool resume,bool manualExportOfCassette);
	void takeCassetteOutForPacketIn(packetPositionClass *p);
	void returnNoNeededCassette();
	bool isIdle();
	cellPositionClass *getThePositionThatTheActiveCassetteIs();
	conveyorPositionClass *getConveyorPosition24();
	conveyorPositionClass *getConveyorPosition25();
	conveyorPositionClass *getConveyorStationImport();

	conveyorPositionClass *getStationExit1();
	conveyorPositionClass *getStationImport();
	conveyorPositionClass *getStationExit2();
	conveyorPositionClass *getStationBackExit1();
	conveyorPositionClass *getStationBackImport();
	conveyorPositionClass *getStationBackExit2();

	conveyorPositionClass *getPackingExit1();
	conveyorPositionClass *getPackingImport();
	conveyorPositionClass *getPackingExit2();
	conveyorPositionClass *getPackingBackExit1();
	conveyorPositionClass *getPackingBackImport();
	conveyorPositionClass *getPackingBackExit2();

	cellPositionClass   	*getCranePosition();
	//
	void plcStatusChanged(int status);
	void plcCassetteOnCraneChanged(int c);
	void plcCraneIsCommingOutWithCassette();
	void setPlcData(tuxipClass *tuxip);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	bool tryToMovePlcCassette(int plcCassette,int posId);
public slots:
	void onMisionTimer();
	void transferStationCassettesTimerSlot();
	void onCellDataChangedSlot(int cell);
};						

#endif

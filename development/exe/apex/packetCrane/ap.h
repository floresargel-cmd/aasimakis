#ifndef AP_H
#define AP_H

class allPosisionsClass;
class craneClass;

#include "pcG.h"
#include "../common/apexG.h"
#include "plc.h"
#include "convP.h"
class posCorrectionClass
{
public:
	int				col;
	int				row;
	float     x;
	float     z;
public:
	posCorrectionClass()
	{
		col=row=0;x=z=0.f;
	}
	posCorrectionClass(int col_,int row_,float x_,float z_)
	{
		col=col_;row=row_;x=x_;z=z_;
	}
};
class plcClass;
class allPosisionsClass:public QWidget
{
	tcpServerClass														*cranePlcTcpServer;
	QTimer																		mainTimer;
	QTimer																		sendMissionToPlcTimer;
	QVector<cellPositionClass*>								allPositionList;
	conveyorPositionClass											*position24;//west conveyor
	conveyorPositionClass											*position25;//east conveyor
	conveyorPositionClass											*positionStataionImport;//station import position
	cellPositionClass													*positionCrane;//crane

	conveyorPositionClass											*stationExit1;
	conveyorPositionClass											*stationImport;
	conveyorPositionClass											*stationExit2;
	conveyorPositionClass											*stationBackExit1;
	conveyorPositionClass											*stationBackImport;
	conveyorPositionClass											*stationBackExit2;

	conveyorPositionClass											*packingExit1=nullptr;
	conveyorPositionClass											*packingImport=nullptr;
	conveyorPositionClass											*packingExit2=nullptr;
	conveyorPositionClass											*packingBackExit1=nullptr;
	conveyorPositionClass											*packingBackImport=nullptr;
	conveyorPositionClass											*packingBackExit2=nullptr;

	int																				type1EmptyCassettes;
	int																				type2EmptyCassettes;
	int																				type3EmptyCassettes;
public:
	Q_OBJECT
public:
	allPosisionsClass(QGraphicsScene *scene,QWidget *p);
	~allPosisionsClass();
	void setPlcClass(plcClass *plc);
	cellPositionClass *getCellPosWithPosId(int id);
	cellPositionClass *getEmptyCassetteOnRow(int row,int packetType,int storagingType,QString cellSignFilter);
	cellPositionClass *getEmptyCassetteOnRow(int row,int packetType);//ignore storagingType
	cellPositionClass *getEmptyCassetteForPacketIn(packetPositionClass *packetPosition,bool secondTry=false,bool loadFromDatabase=false);
	cellPositionClass *getCellPosWithEmptyCassette();
	//
	void setConveyorsPositions(cellPositionClass *pcr,conveyorPositionClass *c24,conveyorPositionClass *c25,conveyorPositionClass *pstationImport
	,conveyorPositionClass *stationExit1_,conveyorPositionClass *stationImport_,conveyorPositionClass *stationExit2_
	,conveyorPositionClass *stationBackExit1_,conveyorPositionClass *stationBackImport_,conveyorPositionClass *stationBackExit2_
	,conveyorPositionClass *packingExit1_,conveyorPositionClass *packingImport_,conveyorPositionClass *packingExit2_
	,conveyorPositionClass *packingBackExit1_,conveyorPositionClass *packingBackImport_,conveyorPositionClass *packingBackExit2_
	);
	QVector<cellPositionClass*> getAllCells();
	int getType1EmptyCassettes();
	int getType2EmptyCassettes();
	int getType3EmptyCassettes();
	cellPositionClass* allPosisionsClass::getPosWithColumnRow(int col,int row);
//
	void calculateCorrections(int noOfRows,QVector<posCorrectionClass*> corV);
	QVector<QPointF> getMinusCellsPositions();
	QVector<QPointF> getPlusCellsPositions();
signals:
	void mousePressedOnPacketPosSignal(packetPositionClass *packetPos,QGraphicsSceneMouseEvent *ev);
public slots:
	void mainTimerSlot();
	void mousePressedOnPacketPosSlot(packetPositionClass *packetPos,QGraphicsSceneMouseEvent *ev);
};

#endif

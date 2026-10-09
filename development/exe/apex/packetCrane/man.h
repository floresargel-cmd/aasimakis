#ifndef MAN_H
#define MAN_H

#include "pcG.h"
#include "plc.h"
#include "../common/apexG.h"
#include "fr.h"
#include "cr.h"
#include "ap.h"
#include "../packingFork/forkPackingWdgt.h"
class managerClass:public QWidget
{
	QTimer										mainTimer;
	QTimer										updateFromDbTimer;

	plcClass									*plc;
	allPosisionsClass					*allPosisions;

	craneClass								*crane;
	cellPositionClass					*positionCrane;
	conveyorPositionClass			*position24;
	conveyorPositionClass			*position25;

	forkClass									*fork;
	forkPositionClass					*positionForkLift;//forklift
#ifdef keepPacketCraneOldFunctionality
	newPacketPositionClass		*positionSb;//position before strapping
	newPacketPositionClass		*positionPa;//position after strapping
	#endif
	newPacketPositionClass		*positionIm;//import position
#ifdef keepPacketCraneOldFunctionality
	packetPositionClass				*positionSbExit;
	packetPositionClass				*positionPacketExit;
	packetPositionClass				*positionStoreExit;
#endif
	forkLiftManualClass				*forkM;
	//packing area
	forkPackingWdgtClass			*forkPackingWdgt=nullptr;
	conveyorPositionClass			*stationExit1;
	conveyorPositionClass			*stationImport;
	conveyorPositionClass			*stationExit2;

	conveyorPositionClass			*packingExit1;
	conveyorPositionClass			*packingImport;
	conveyorPositionClass			*packingExit2;
	conveyorPositionClass			*packingBackExit1;

	packetPositionClass				*positionInputOutput;
	packetPositionClass				*positionOldStrappingExit;
	packetPositionClass				*positionNewStrappingExit;
	//packing fork
	bool											autoMissionsExports=false;
	bool											autoMissionsImports=false;
	bool											autoMissionsExportsStation1=false;
	bool											autoMissionsExportsStation2=false;
	bool											autoMissionsStationImports=false;
	bool											autoMissionsPacking=false;
public:
#ifdef keepPacketCraneOldFunctionality
	enum exportsDestinationEnum
	{
		exportsTo23=1,
		exportsTo22=2,
		exportsTo21=3,
	};
	exportsDestinationEnum		exportsDestination;
	#endif
Q_OBJECT
public:
	managerClass(QGraphicsScene *scene,allPosisionsClass *allPosisions_,craneClass *crane_,cellPositionClass *positionCrane_,conveyorPositionClass *position24_
	,conveyorPositionClass *position25_,forkClass *fork_,forkPositionClass *positionForkLift_
#ifdef keepPacketCraneOldFunctionality
	,newPacketPositionClass *positionSb_
#endif
	,conveyorPositionClass *stationExit1_,conveyorPositionClass *stationImport_,conveyorPositionClass *stationExit2_
	,conveyorPositionClass *packingExit1_,conveyorPositionClass *packingImport_,conveyorPositionClass *packingExit2_,conveyorPositionClass *packingBackExit1_
#ifdef keepPacketCraneOldFunctionality
	,newPacketPositionClass *positionPa_
#endif
	,newPacketPositionClass *positionIm_,forkPackingWdgtClass *forkPackingWdgt_,QWidget *p);
	~managerClass();
	void setPlcClass(plcClass *plc_);
	void startTimers();
	//
#ifdef keepPacketCraneOldFunctionality
	packetPositionClass *getPositionStoreExit();
	packetPositionClass *getPositionSbExit();
	packetPositionClass *getPositionPacketExit();
	void setExportsDestination(exportsDestinationEnum d);
#endif
	void enableAutoExportsMissions(bool enable);
	void enableAutoImportsMissions(bool enable);
	void enableAutoExportsStation1Missions(bool enable);
	void enableAutoExportsStation2Missions(bool enable);
	void enableAutoImportsStationMissions(bool enable);
	void enableAutoPackingMissions(bool enable);
	bool getAreAutoMissionsEnabled();
	//
public slots:
	void mainTimerSlot();
	void updateFromDbTimerSlot();
	void onPacketDataChangedSlot(int cell);
};
#endif

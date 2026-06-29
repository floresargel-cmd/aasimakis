#ifndef MAN_H
#define MAN_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "pcG.h"
#include "plc.h"
#include "../common/apexG.h"
#include "fr.h"
#include "cr.h"
#include "ap.h"
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
	packetPositionClass				*positionStoreExit;

	forkClass									*fork;
	forkPositionClass					*positionForkLift;//forklift
	newPacketPositionClass		*positionSb;//position before strapping
	newPacketPositionClass		*positionPa;//position after strapping
	newPacketPositionClass		*positionIm;//import position 
	packetPositionClass				*positionSbExit;
	packetPositionClass				*positionPacketExit;
	forkLiftManualClass				*forkM;
	bool											autoMissionsEnabled;
public:
	enum exportsDestinationEnum
	{
		exportsTo23=1,
		exportsTo22=2
	};
	exportsDestinationEnum		exportsDestination;
	Q_OBJECT
public:
	managerClass(allPosisionsClass *allPosisions_,craneClass *crane_,cellPositionClass *positionCrane_,conveyorPositionClass *position24_,conveyorPositionClass *position25_,forkClass *fork_,forkPositionClass *positionForkLift_,newPacketPositionClass *positionSb_,newPacketPositionClass *positionPa_,newPacketPositionClass *positionIm_,QWidget *p);
	~managerClass();
	void setPlcClass(plcClass *plc_);
	void startTimers();
	//
	packetPositionClass *getPositionStoreExit();
	packetPositionClass *getPositionSbExit();
	packetPositionClass *getPositionPacketExit();
	void setExportsDestination(exportsDestinationEnum d);
	void enableAutoMissions(bool enable);
	//
public slots:								
	void mainTimerSlot();						
	void updateFromDbTimerSlot();						
};
#endif

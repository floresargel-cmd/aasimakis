#ifndef aW_H
#define aW_H

#include "pcG.h"
#include "gView.h"
#include "../common/apexG.h"
#include "plc.h"
#include "ap.h"
#include "cr.h"
#include "fr.h"
#include "packingFork/forkPackingWdgt.h"
#include "man.h"
#include "manCasSel.h"
class allWidgetClass:public QWidget
{
	myQGraphicsViewClass				*layoutGraphicsView;
	plcClass										*plc=nullptr;
	//
	managerClass								*manager=nullptr;
	craneClass									*crane=nullptr;
	forkClass										*fork=nullptr;
	forkPackingWdgtClass				*forkPackingWdgt=nullptr;
	QDialog											*forkPackingWdgtDlg=nullptr;
	allPosisionsClass						*allPosisions=nullptr;
	packetPositionClass					*manualMissionFromPos=nullptr;
 Q_OBJECT
		public:
	allWidgetClass(QWidget *parent=0);
	~allWidgetClass();
	void setPlcClass(plcClass *plc_);
	void signalFromCranePlc(QString table,int index,QString value);
	void signalFromForkPlc(QString table,int index,QString value);
	void signalFromCrane3Plc(QString table,int index,QVariant value);
	craneClass* getCraneClass();
	forkClass* getForkClass();
	forkPackingWdgtClass* getForkPackingWdgtClass();
	allPosisionsClass* getAllPosClass();
	float getPacketCraneX();
	float getPacketCraneY();
	float getPacketCraneZ();
	float getForkCraneX();
	float getForkCraneY();
	float getForkCraneZ();
	//
	void startTimers();
#ifdef keepPacketCraneOldFunctionality
	void setExportsDestination(managerClass::exportsDestinationEnum e);
	void enableAutoExportsMissions(bool enable);
#endif
	void enableAutoImportsMissions(bool enable);
	void enableAutoExportsStation1Missions(bool enable);
	void enableAutoExportsStation2Missions(bool enable);
	void enableAutoImportsStationMissions(bool enable);
	void enableAutoWestStationMissions(bool enable);
	int getType1EmptyCassettes();
	int getType2EmptyCassettes();
	int getType3EmptyCassettes();
	//
	void showForkPackingWdgt();
	void newScanForInOutStation(int ticket);
protected:
signals:
	void clicked();
public slots:
	void resetGraphicsViewZoomSlot();
	void mousePressedOnPacketPosSlot(packetPositionClass *packetPos,QGraphicsSceneMouseEvent *ev);
};
#endif

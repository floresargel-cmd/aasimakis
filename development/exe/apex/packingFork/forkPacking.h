#ifndef forkPacking_h
#define forkPacking_h

class forkPackingClass;
#include "Gf.h"
#include "allPosPackingFork.h"

#include "gItemP.h"
#include "gView.h"
#include "hmiGuiWidgetsLib.h"
#include "posPackingFork.h"
#include "serv.h"

class forkPackingClass:public QWidget
{
	tuxipClass									*craneTuxip=nullptr;
	tuxipClass									*forkTuxip=nullptr;
	QGraphicsScene							*layoutScene;
	allPosPackingForkClass	 	  *allPos;
	gItemClass									*forkXItem;
	gItemClass									*forkYItem;

	posPackingForkClass					*myPos=nullptr;
	posPackingForkClass					*inputOutputPos=nullptr;
	posPackingForkClass					*packetFormingStation1Pos=nullptr;
	posPackingForkClass					*packetFormingStation2Pos=nullptr;
	posPackingForkClass					*strappingOldInPos=nullptr;
	posPackingForkClass					*strappingOldOutPos=nullptr;
	posPackingForkClass					*strappingNewInPos=nullptr;
	posPackingForkClass					*strappingNewOutPos=nullptr;
	posPackingForkClass					*warehouseConveyorPos=nullptr;

	QTimer											*clearMissionSendTimer=nullptr;

	bool												autoMissionsEnabled=false;
	bool												plcOkForNewMission=false;
	bool												sendedMissionsToPlcAndIsPending=false;
	int													plcActiveFromPos=0;
	int													plcActiveToPos=0;
	int													plcStepBits=0;

	float												plcX=0.;
	float												plcY=0.;
	float												plcZ=0.;
	float												plcR=0.;
	Q_OBJECT
public:
	forkPackingClass(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,allPosPackingForkClass *allPos_,QGraphicsScene *layoutScene_,QWidget *p=nullptr);
	void setAutoMissionsEnabled(bool e);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	bool getIsIdle();
	bool getIsAboutToBeIdle();
	bool getCanSendMission(posPackingForkClass *from,posPackingForkClass *to);
	bool sendMission(posPackingForkClass *from,posPackingForkClass *to);
	bool sendMission(int from,int to);
signals:
	void returnEmptyCassetteFromNorthPackingConveyor();
public slots:
	void missionsTimerTriggeredSlot();
	void clearMissionSendTimerTriggeredSlot();
};
#endif

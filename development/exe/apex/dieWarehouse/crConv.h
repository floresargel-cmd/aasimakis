#ifndef CRCONV_H
#define CRCONV_H

class craneConveyorClass;
#include "Gd.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "triplePos.h"
#include "plcBit.h"
#include "allPos.h"
#include "crConvW.h"
//class craneConveyorWidgetClass;
class craneConveyorClass : public triplePosClass				
{
	tuxipClass								*tuxip;
	QTimer										*basketOnConveyorTimer;
	allPossClass							*allPos;
	QVector<int>							excludedPosV;
	QString										plcIp;
	QString										plcMissionTable;
	int												plcNextMissionFromIndex;
	int												plcNextMissionToIndex;
	int												plcNextMissionStatusIndex;
	int												plcNextFromXcorrIndex;
	int												plcNextFromYcorrIndex;
	int												plcNextFromZcorrIndex;
	int												plcNextToXcorrIndex;
	int												plcNextToYcorrIndex;
	int												plcNextToZcorrIndex;
	int												plcCurrentMissionFromIndex;
	int												plcCurrentMissionToIndex;
	int												plcCurrentMissionPcStatusIndex;
	int												plcCurrentMissionPLcStatusIndex;
	int												plcCurrentCranePlcStepIndex;
	int												plcCurrentHooksPlcStepIndex;
	int												plcNextMissionBasketIndex;
	//
	int												nextMissionFrom;
	int												nextMissionTo;
	int												nextMissionStatus;
	int												nextFromXCorr;
	int												nextFromYCorr;
	int												nextFromZCorr;
	int												nextToXCorr;
	int												nextToYCorr;
	int												nextToZCorr;
	int												currentMissionFrom;
	int												currentMissionTo;
	int												currentMissionPcStatus;
	int												currentMissionPLcStatus;
	QString										dbNextName;
	QString										dbActiveName;
	//
	posClass									*nextFromPos;
	posClass									*nextToPos;
	posClass									*activeFromPos;
	posClass									*activeToPos;
	int												activeMissionStatus;
	int												activeCraneStep;
	int												activeHooksStep;
	craneConveyorWidgetClass	*craneConveyorWidget;
public:										
	enum//mission status
	{
		plcStatusIdle=0,
		plcStatusStart=1,
		plcStatusBascketOnConv=2,
		plcStatusFinished=3,
		plcStatusAborted=4,
		plcStatusStartDoubleMission=10,
	};
	enum//
	{
		plcCraneStepIdle=0,
		plcCraneStepStart=1,
		plcCraneStepGoToXZLoad=2,
		plcCraneStepGoToYLoad=3,
		plcCraneStepBasketToConveyor=31,
		plcCraneHooksUp=4,
		plcCraneStepHooksTakingBasketIn=5,
		plcCraneStepHooksTakingBasketInConveyor=51,
		plcCraneStepHooksTakingBasketInHooksDown=52,
		plcCraneStepHooksTakingBasketInHooksIn=53,
		plcCraneStepGoToXZUnLoad=6,
		plcCraneStepConveyorMovingBasketToCell=7,
		plcCraneStepConveyorMovingBasketOut=71,
	  plcCraneStepHooksDownAnchorsIn=8,
		plcCraneStepHooksAnchorsIn=9,
		plcCraneStepFinished=10,
	};
	enum//
	{
		plcHooksStepIdle=0,
		plcHooksStepStart=1,
		plcHooksStepGoToXZLoad=2,
		plcHooksStepGoToYLoad=3,
		plcHooksStepBasketToConveyor=31,
		plcHooksHooksUp=4,
		plcHooksStepHooksTakingBasketIn=5,
		plcHooksStepHooksTakingBasketInConveyor=51,
		plcHooksStepHooksTakingBasketInHooksDown=52,
		plcHooksStepHooksTakingBasketInHooksIn=53,
		plcHooksStepHooks55=55,
		plcHooksStepHooks56=56,
		plcHooksStepHooks57=57,
		plcHooksStepHooks58=58,
		plcHooksStepHooks59=59,
		plcHooksStepHooks60=60,
		plcHooksStepHooks61=61,
		plcHooksStepGoToXZUnLoad=6,
		plcHooksStepConveyorMovingBasketToCell=7,
		plcHooksStepConveyorMovingBasketOut=71,
	  plcHooksStepHooksDownAnchorsIn=8,
		plcHooksStepHooksAnchorsIn=9,
		plcHooksStepFinished=10,
	};	

	Q_OBJECT
public:										
	craneConveyorClass(tuxipClass *tuxipConnection,allPossClass *allPos_,int plcId,int basketNum,QString description,QGraphicsScene *scene,dxfGroupOfGraphicsItemClass *mouseOverBasketGraphicsItem,dxfGroupOfGraphicsItemClass *cellItem,dxfGroupOfGraphicsItemClass *fullItem,dxfGroupOfGraphicsItemClass *emptyItem,dxfGroupOfGraphicsItemClass *lockedItem,QWidget *p=NULL);
	~craneConveyorClass();
	QWidget* getCraneConveyorWidget();
	void setPlcMissionTable(QString plcIp_,QString plcMissionTable_,int plcNextMissionFromIndex_,int plcNextMissionToIndex_,int plcNextMissionStatusIndex_,int plcCurrentMissionFromIndex_,int plcCurrentMissionToIndex_,int plcCurrentMissionPcStatusIndex_,int plcCurrentMissionPLcStatusIndex_,int plcCurrentCranePlcStepIndex_,int plcCurrentHooksPlcStepIndex_,int plcNextFromXcorrIndex_,int plcNextFromYcorrIndex_,int plcNextFromZcorrIndex_,int plcNextToXcorrIndex_,int plcNextToYcorrIndex_,int plcNextToZcorrIndex_,int plcNextMissionBasketIndex_);
	void checkForNextMissionInDatabase();
	void loadFromDatabase();
	bool getIsIdle();
	void setNextFromPos(posClass *p);
	void setNextToPos(posClass *p);
	void setActiveFromPos(posClass *p);
	void setActiveToPos(posClass *p);
	bool isReadyForNewMission();
	bool getCanStartNewMission(posClass* f,posClass* t);
	void plcStatusChanged(int s);
	void missionStart(posClass* f,posClass*t);
	void missionSendToPlc(bool force=false);
	void missionBasketOnConveyor();
	void missionFinished();
	void missionAborted();
	int moveBasket(posClass *from,posClass *to);
	bool moveNextToActive();
	void clearMissionInDb(QString user=QString());
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setExcludedPositions(QVector<int> excludedPosV_);
	QString getCraneStepText(int s);
	QString getCraneStepDescription(int s);
	QString getHooksStepText(int s);
	QString getHooksStepDescription(int s);
};											
#endif

#ifndef CRCONV_H
#define CRCONV_H

class craneConveyorClass;
#include "Gsw.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "triplePos.h"
#include "allPos.h"
#include "crConvW.h"
//class craneConveyorWidgetClass;
class craneConveyorClass : public triplePosClass
{
	tuxipClass								*tux;
	QTimer										*missionTimer;
	allPossClass							*allPos;
	QVector<int>							excludedPosV;
	craneConveyorWidgetClass	*craneConveyorWidget;
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
	int												plcCurrentMissionPlcStepIndex;
	int												plcNextMissionCassetteIndex;
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
	int												activeMissionStep;
	bool											isPlcConnected;
public:
	enum//mission status
	{
		plcStatusIdle=0,
		plcStatusStart=1,
		plcStatusBascketOnConv=2,
		plcStatusFinished=3,
		plcStatusAborted=4,
		plcStatusUserFinished=5,
	};
	enum//mission status
	{
		plcStepIdle=0,
		plcStepStart=1,
		plcStepGoToXZLoad=2,
		plcStepGoToYLoad=3,
		plcStepBasketToConveyor=31,
		plcStepHooksUp=4,
		plcStepHooksToCenterWithCassette=5,
		plcStepGoToXZUnLoad=6,
		plcStepGoToYUnLoad=7,
		plcStepHooksDown=8,
		plcStepHooksToCenterNoCassette=9,
		plcStepFinished=10,
	};
	Q_OBJECT
public:
	craneConveyorClass(tuxipClass *tux_,allPossClass *allPos_,int posId,int cassetteNum,QString description
		,gItemClass *mouseOverItem,gItemClass *cellItem,gItemClass *cassetteItem,gItemClass *sbBlueItem,gItemClass *profilesItem,gItemClass *lockedItem,gItemClass *activeFromItem,gItemClass *activeToItem
		,QWidget *p=NULL);
	~craneConveyorClass();
	QWidget* getCraneConveyorWidget();
	void setPlcMissionTable(QString plcIp_,QString plcMissionTable_,int plcNextMissionFromIndex_,int plcNextMissionToIndex_,int plcNextMissionStatusIndex_,int plcCurrentMissionFromIndex_
		,int plcCurrentMissionToIndex_,int plcCurrentMissionPcStatusIndex_,int plcCurrentMissionPLcStatusIndex_,int plcCurrentMissionPlcStepIndex_,int plcNextFromXcorrIndex_
		,int plcNextFromYcorrIndex_,int plcNextFromZcorrIndex_,int plcNextToXcorrIndex_,int plcNextToYcorrIndex_,int plcNextToZcorrIndex_,int plcNextMissionCassetteIndex_);
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
	void missionCassetteOnConveyor();
	void missionFinished();
	void missionAborted();
	int moveCassette(posClass *from,posClass *to);
	bool moveNextToActive();
	void clearMissionInDb(QString user=QString());
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setExcludedPositions(QVector<int> excludedPosV_);
	QString getStepText(int s);
	QString getStepDescription(int s);
	bool getIsPlcConnected();
	void setIsPlcConnected(bool v);
	bool getHasNextMission();
	bool isMissionAboutToEnd();//to know when to look for new missions
public slots:
	void missionTimerSlot();
};
#endif

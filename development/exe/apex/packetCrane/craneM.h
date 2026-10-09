#ifndef CRANEM_H
#define CRANEM_H

class manualCassetteMovementClass;
#include "pcG.h"
#include "../common/apexG.h"
#include "../common/btns.h"
#include "ps.h"
#include "cm.h"
#include "convDlg.h"
#include "manCasSel.h"
#include "camera.h"
#include "hmiGuiWidgetsLib.h"
class packetCraneManualClass : public QWidget
{
	QTimer													alarmTimer;
	allPosisionsClass								*allPositions;
	manualCassetteMovementClass			*manualCassetteMovement;
	tcpServerClass									*tcpServer;
	conveyorsManualClass						*conveyorsManual;
	//mission
	QLabel                          *loadPosLabel;
	QLabel												  *unloadPosLabel;
	QLabel													*pcsStatusLabel;
	QLabel													*plcStatusLabel;
	//
	QListWidget                     *alarmsSignalListWidget;
	QPushButton											*showAlarmsHistoryBtn;
	int															alarmsIndex4Value;
	int															alarmsIndex5Value;
	QStringList											alarmsIndex4StringList;
	QStringList											alarmsIndex5StringList;

	QListWidget											*warningsSignalListWidget;
	QPushButton											*showWarningsHistoryBtn;
	int															warningsIndex0Value;
	int															warningsIndex1Value;
	int															warningsIndex8Value;
	QStringList											warningsIndex0StringList;
	QStringList											warningsIndex1StringList;
	QStringList											warningsIndex8StringList;
	//
	QVector<floatPlcMemoryClass*>				floatPlcMemoryVector;
	QVector<intPlcMemoryClass*>					intPlcMemoryVector;
	QVector<bitClass*>									bitVector;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;

	//
Q_OBJECT
	QVector<myQPushButton*>					areaButtonsVector;
public:
  packetCraneManualClass(tcpServerClass *tcpServer_,QWidget *p);
	//
	void refreshAlarmsList();
	void refreshWarningList();
	//
	void plcIntMemory150Changed(int index,int value);
	void plcIntMemory152Changed(int index,int value);
	void plcFloatMemoryChanged(int index,double value);
	void hideButtons();
	void showButtons();
	void setManualCassetteMovementClass(manualCassetteMovementClass *manualCassetteMovement_);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setAllPossitionsClass(allPosisionsClass* allP);
public slots:
	void onShowConveyorsManualsSlot();
	void continueMissionBtnSlot();
	void closeMissionBtnSlot();
	void abortMissionBtnSlot();
	void manualMissionBtnSlot();
	void addOilPushButtonSlot();
	void sendCorrectionsPushButtonSlot();
	void showAlarmsHistoryBtnSlot();
	void showWarningsHistoryBtnSlot();
	void alarmTimerSlot();
};
#endif

#ifndef CRANEM_H
#define CRANEM_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
class manualCassetteMovementClass;
#include "pcG.h"
#include "../common/apexG.h"
#include "../common/btns.h"
#include "ps.h"
#include "cm.h"
#include "convDlg.h"
#include "manCasSel.h"
class packetCraneManualClass : public QWidget	
{
	QTimer													alarmTimer;
	manualCassetteMovementClass			*manualCassetteMovement;
	tcpServerClass									*tcpServer;
	conveyorsManualClass						*conveyorsManual;
	//main
	myQPushButton										*autoPushbutton;
	myQPushButton										*manualPushbutton;
	myQPushButton										*maitenancePushbutton;
	
	myQPushButton										*horizontalResetPushbutton;
	myQPushButton										*verticalResetPushbutton;
	myQPushButton										*hooksResetPushButton;	
	
	QLabel													*stopLabel_0_24;

	//Horizontal
	QLabel													*horizontalResetLabel;    
	QLabel													*horizontalActualPosLabel;
	QLabel													*horizontalTargetPosLabel;
	QLabel													*horizontalRefSpeedLabel; 
	QLabel													*horizontalActualSpeedLabel;  
	QLabel                          *horizontalFwLabel;
	QLabel                          *horizontalBwLabel;
	QLabel                          *horizontalOnTargetLabel;

	//Vertical
	QLabel													*verticalResetLabel;     
	QLabel													*verticalActualPosLabel; 
	QLabel													*verticalTargetPosLabel; 
	QLabel													*verticalRefSpeedLabel;  
	QLabel													*verticalActualSpeedLabel; 
	QLabel                          *verticalUpLabel;
	QLabel                          *verticalDownLabel;
	QLabel                          *verticalOnTargetLabel;
  
	//Hooks
	QLabel													*hooksResetLabel;     
	QLabel													*hooksActualPosLabel; 
	QLabel													*hooksTargetPosLabel; 
	QLabel													*hooksRefSpeedLabel;  
	QLabel													*hooksActualSpeedLabel;  
	QLabel													*hooksPositionLabel;
	QLabel                          *hooksOnTargetLabel;
	QLabel                          *hooksLeftLabel;
	QLabel                          *hooksRightLabel;
	QLabel                          *hooksOnCenterLabel;

	//Anchors
	QLabel													*leftMgOutLabel;
	QLabel													*leftMgInLabel;						
	QLabel													*rightMgOutLabel;
	QLabel													*rightMgInLabel;	
 
	//conveyors
	QLabel                          *leftConveyorStopLabel;            
	QLabel													*leftConveyorBrakeLabel;		
	QLabel                          *rightConveyorBrakeLabel;
	QLabel                          *rightConveyorStopLabel;
  
	//mission
	QLabel                          *loadPosLabel;                      
	QLabel												  *unloadPosLabel;
	QLabel													*pcsStatusLabel;
	QLabel													*plcStatusLabel;

	//pcl status
	QLabel													*plcInOutLabel;
  QLabel													*thermalOverloadLabel;
	QLabel													*readyMissionLabel;
	QLabel													*homePositionLabel;

	//Mission step
	QLabel													*step1Label;
	QLabel                          *step2Label;
	QLabel                          *step3Label;
	QLabel                          *step4Label;
	QLabel                          *step5Label;
	QLabel                          *step6Label;
	QLabel                          *step7Label;
	QLabel                          *step8Label;
	QLabel                          *step9Label;
	//oil
	QLabel													*lowOilPressLabel;
	QLabel													*pressPumpLabel;
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

	/*proximitySwitchClass						*c25Break;
	proximitySwitchClass            *c24Break;
	proximitySwitchClass						*conveyorPsLeftStop;
	proximitySwitchClass            *conveyorPsRightStop;
	proximitySwitchClass						*conveyorPsLeftBrake;
  proximitySwitchClass            *conveyorPsRightBreak;
	conveyorMovmentClass						*cm1;*/

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
public slots:
	void onShowConveyorsManualsSlot();
	void continueMissionBtnSlot();
	void closeMissionBtnSlot();
	void manualMissionBtnSlot();
	void addOilPushButtonSlot();
	void showAlarmsHistoryBtnSlot();
	void showWarningsHistoryBtnSlot();
	void alarmTimerSlot();
};																	
#endif
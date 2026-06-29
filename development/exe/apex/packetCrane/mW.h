#ifndef MW_H
#define MW_H

#include "pcG.h"
#include "plc.h"
#include "aW.h"
#include "expD.h"
#include "pass.h"
#include "mMissions.h"
class mainWindowClass : public QWidget		
{
	QTimer												mainTimer;
	QTimer												commsTimer;
	QTimer												backupTimer;
	QPushButton										*plcPushButton;
	QLabel												*cranePlcCommsLabel;
	QLabel												*forkPlcCommsLabel;
	QPushButton										*userBackupPushButton;
	QRadioButton									*exportsTo23Radio;
	QRadioButton									*exportsTo22Radio;
	QRadioButton									*exportsTo21Radio;//new station east
	plcClass											*plc;
	allWidgetClass								*allWidget;
	exportsDlgClass								*exportsDlg;
	bool													forkPlcConnected;
	bool													cranePlcConnected;
	int														lastCranePlcHandShakeValue;
	int														lastForkPlcHandShakeValue;
	manualCassetteMovementClass		*manualCassetteMovement;
	QCheckBox											*autoMissionsExportsCheckBox;
	QCheckBox											*autoMissionsImportsCheckBox;
	QCheckBox											*autoMissionsExportsStation1CheckBox;
	QCheckBox											*autoMissionsExportsStation2CheckBox;
	QCheckBox											*autoMissionsImportsStationCheckBox;
	QCheckBox											*autoMissionsPackingStationCheckBox;
	QPushButton										*definePacketInIOutStationBtn;

	manualMissionsClass						*manualMissions;
	QPushButton										*manualMissionsBtn;
	QLabel												*type1EmptyCassettesLabel;
	QLabel												*type2EmptyCassettesLabel;
	QLabel												*type3EmptyCassettesLabel;

	int														packetInInOutStationSerialPortTicket=0;
	QSerialPort										*packetInInOutStationSerialPort;
Q_OBJECT								
public:										
		mainWindowClass();
		~mainWindowClass();
public slots:
	void onMainTimer();
	void signalFromCraneTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void signalFromForkTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void signalFromCrane3TuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();
	void stopSlot();
	void tempForMoveSlot();
	void signalFromPacketCranePlcSlot(QString table,int index,QString value);//for backwards compatibility
	void signalFromForkLiftPlcSlot(QString table,int index,QString value);//for backwards compatibility
	void plcButtonClickedSlot();
	void exportsButtonClickedSlot();
	void onBackupTimerSlot();
	void onCommsTimerSlot();
	void onUserBackupSlot();
	void exportsTo23RadioToggledSlot(bool checked);
	void exportsTo22RadioToggledSlot(bool checked);
	void exportsTo21RadioToggledSlot(bool checked);
	void qtAboutSlot();
	void autoMissionsExportsCheckBoxStateChangedSlot(int state);
	void autoMissionsImportsCheckBoxStateChangedSlot(int state);
	void autoMissionsExportsStation1CheckBoxStateChangedSlot(int state);
	void autoMissionsExportsStation2CheckBoxStateChangedSlot(int state);
	void autoMissionsImportsStationCheckBoxStateChangedSlot(int state);
	void autoMissionsPackingStationCheckBoxStateChangedSlot(int state);
	void manualMissionsBtnClickedSlot();
	void forkPackingButtonClickedSlot();
	void readyReadPacketInInOutStationSerialPortSlot();
	void definePacketInIOutStationBtnClickedSlot();
};																	
#endif
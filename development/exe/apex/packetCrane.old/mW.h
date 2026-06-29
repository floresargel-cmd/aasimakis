#ifndef MW_H
#define MW_H

#include <QtGui>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCheckBox>

#include "pcG.h"
#include "plc.h"
#include "oW.h"
#include "exp.h"
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
	plcClass											*plc;
	openGLWidgetClass							*openGLWidget;
	exportsClass									*exports;
	bool													forkPlcConnected;
	bool													cranePlcConnected;
	int														lastCranePlcHandShakeValue;
	int														lastForkPlcHandShakeValue;
	manualCassetteMovementClass		*manualCassetteMovement;
	QCheckBox											*autoMissionsCheckBox;
	manualMissionsClass						*manualMissions;
	QPushButton										*manualMissionsBtn;
Q_OBJECT								
public:										
    mainWindowClass();
		~mainWindowClass();
public slots:
	void onMainTimer();
	void newConnectionFromForkPlcSlot();
	void newConnectionFromCranePlcSlot();
	void signalFromCraneTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void signalFromForkTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();
	void signalFromPacketCranePlcSlot(QString table,int index,QString value);//for backwards compatibility
	void signalFromForkLiftPlcSlot(QString table,int index,QString value);//for backwards compatibility
	void plcButtonClickedSlot();
	void exportsButtonClickedSlot();
	void onBackupTimerSlot();
	void onCommsTimerSlot();
	void onUserBackupSlot();
	void exportsTo23RadioToggledSlot(bool checked);
	void exportsTo22RadioToggledSlot(bool checked);
	void qtAboutSlot();
	void autoMissionsCheckBoxStateChangedSlot(int state);
	void manualMissionsBtnClickedSlot();
};																	
#endif
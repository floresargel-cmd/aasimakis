#ifndef FORK_H
#define FORK_H
#include "pcG.h"
#include "../common/apexG.h"
#include "../common/btns.h"
#include "bitsVectorBtn.h"
class forkLiftManualClass : public QWidget
{
	tcpServerClass									*tcpServer;
	//main
	myQPushButton										*autoPushbutton;
	myQPushButton										*manualPushbutton;
	myQPushButton										*maitenancePushbutton;
	QLabel													*stopLabel_5_9;

	myQPushButton										*takePacketQPushButton;
	QPushButton											*continueMissionBtn;
	//pc comms
	QLabel													*memory160_0_label;
	QLabel													*memory160_1_label;
	QLabel													*memory160_2_label;
	QLabel													*memory160_3_label;
	QLabel													*memory160_4_label;
	//Proximity switches
	QLabel													*signal_05_03;
	QLabel													*signal_05_04;
	QLabel													*signal_05_05;
	//magretics
	QLabel													*signal_05_06;
	QLabel													*signal_05_07;
	QLabel													*signal_05_08;
	//position
	QLabel													*posXLabel;
	QLabel													*posYLabel;
	QLabel													*posZLabel;
	QLabel													*targetPosXLabel;
	QLabel													*targetPosYLabel;
	QLabel													*targetPosZLabel;
	//velocity
	QLabel													*velocityXLabel;
	QLabel													*velocityYLabel;
	QLabel													*velocityZLabel;
	QLabel													*targetVelocityXLabel;
	QLabel													*targetVelocityYLabel;
	QLabel													*targetVelocityZLabel;
	//positions
	QLabel													*signal_05_11;
	QLabel													*signal_05_12;
	QLabel													*signal_05_13;
	QLabel													*signal_05_14;
	QLabel													*signal_05_15;
	QLabel													*signal_05_16;
	QLabel													*signal_05_17;
	QLabel													*signal_05_18;//23 is with packet
	//QLabel													*signal_05_19;//22 is with packet
	//QLabel													*signal_05_22;//21 is with packet
	//alarms
	QLabel													*signal_07_00;
	QLabel													*signal_07_01;
	QLabel													*signal_07_02;
	QLabel													*signal_07_03;
	QLabel													*signal_07_04;
	QLabel													*signal_07_05;
	QLabel													*signal_07_06;
	QLabel													*signal_07_07;
	QLabel													*signal_07_08;
	QLabel													*signal_07_09;
	QLabel													*signal_07_10;
	QPushButton											*showAlarmsHistoryBtn;
	bitsVectorBtnClass							*missionFaultsBtn;
	bool														plcCan23TakeBasket;
	bool														plcCan22TakeBasket;
	bool														plcCan21TakeBasket;
	QVector<hmiPushButtonWidgetClass*>			pushButtonsVector;

	//
Q_OBJECT
	QVector<myQPushButton*>					areaButtonsVector;
public:
  forkLiftManualClass(tcpServerClass *tcpServer_,QWidget *p);
	void plcIntMemoryChanged(int index,int value);
	void plcFloatMemoryChanged(int index,double value);
	void hideButtons();
	void showButtons();
	bool get23CanTakeBasket();
	bool get22CanTakeBasket();
	bool get21CanTakeBasket();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:
	void abordMissionBtnSlot();
	void closeMissionBtnSlot();
	void continueMissionBtnSlot();
	void showAlarmsHistoryBtnSlot();
};
#endif

#ifndef PLCM_H
#define PLCM_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>

#include "cG.h"
class plcWidgetClass;
class tcpServerClass;
class plcManualClass;
#include "plcW.h"
#include "../common/btns.h"

class plcManualClass : public QWidget
{
	plcWidgetClass									*plcWidget;
	tcpServerClass									*cranePlcTcpServer;
	tcpServerClass									*conveyorsPlcTcpServer;
	//////////////////////
	int												N20_12;   //plc status
	//
	//////
	QLabel											*fromPcLabel;
	QLabel											*toPcLabel;

	QLabel											*fromPlcLabel;
	QLabel											*toPlcLabel;
	QLabel											*fromToPlcStatus;

	QLabel											*makeResetLabel;

	QLabel											*stateLabel_15_13;
	QLabel											*stateLabel_15_14;
	QLabel											*stateLabel_15_15;
/////////////////////////////////////////////////////////////////////////////////////////////
	QLabel											*plcStatusLabel_14_14;
	QLabel											*plcStatusLabel_14_15;
	QLabel											*plcStatusLabel_15_00;

	QLabel											*loadTargetZLabel;
	QLabel											*loadTargetXLabel;
	QLabel											*loadTargetYLabel;

	QLabel											*unloadTargetZLabel;
	QLabel											*unloadTargetXLabel;
	QLabel											*unloadTargetYLabel;

	QLabel											*stepTargetZLabel;
	QLabel											*stepTargetXLabel;
	QLabel											*stepTargetYLabel;

	QLabel											*realPositionZLabel;
	QLabel											*realPositionXLabel;
	QLabel											*realPositionYLabel;

	QLabel											*setSpeedZLabel;
	QLabel											*setSpeedXLabel;
	QLabel											*setSpeedYLabel;

	QLabel											*realSpeedZLabel;
	QLabel											*realSpeedXLabel;
	QLabel											*realSpeedYLabel;
	//////////////////////
	myQRadioButton									*autoRadioButton;
	myQRadioButton									*stepRadioButton;
	myQRadioButton									*maintenanceRadioButton;
	myQPushButton									*makeStepBtn;

	QLabel											*stopLabel;
	QLabel											*okLabel;
	QLabel											*CollisionLabel;
	QLabel											*alarm_13_1Label;
	QLabel											*alarm_13_2Label;
	myQPushButton									*recallStopPushButton;

	myQPushButton									*abordMissionPushButton;
	QPushButton										*continueMissionPushButton;
	myQPushButton									*command10PushButton;
	myQPushButton									*command11PushButton;
	myQPushButton									*command12PushButton;
	myQPushButton									*command13PushButton;
	myQPushButton									*command14PushButton;
	myQPushButton									*command15PushButton;
	myQPushButton									*command16PushButton;
	myQPushButton									*command17PushButton;
	myQPushButton									*command18PushButton;
	myQPushButton									*command19PushButton;

	myQPushButton									*command20PushButton;
	myQPushButton									*command21PushButton;
	myQPushButton									*command22PushButton;
	myQPushButton									*runFasterInMaintenancePushButton;
	QLabel											*resetingZLabel;
	QLabel											*resetingXLabel;
	QLabel											*resetingYLabel;
	/////////////////////////////////////////////////////
	QLabel											*signal_13_03;
	QLabel											*signal_13_04;
	QLabel											*signal_13_05;
	QLabel											*signal_13_06;
	QLabel											*signal_13_07;
	QLabel											*signal_13_08;
	QLabel											*signal_13_09;
	QLabel											*signal_13_10;
	QLabel											*signal_13_11;
	QLabel											*signal_13_12;
	QLabel											*signal_13_13;
	QLabel											*signal_13_14;
	QLabel											*signal_13_15;

	QLabel											*signal_14_00;
	QLabel											*signal_14_01;
	QLabel											*signal_14_02;
	QLabel											*signal_14_03;
	QLabel											*signal_14_04;
	QLabel											*signal_14_05;
	QLabel											*signal_14_06;
	QLabel											*signal_14_07;
	QLabel											*signal_14_08;
	QLabel											*signal_14_09;
	QLabel											*signal_14_10;
	QLabel											*signal_14_11;
	QLabel											*signal_14_12;
	QLabel											*signal_14_13;

	QLabel											*signal_16_12;
	//////////////////////
	QLabel											*switch_15_04;
	QLabel											*switch_15_05;
	QLabel											*switch_15_06;
	QLabel												  *switch_15_07;
	QLabel												  *switch_15_08;
	QLabel												  *switch_15_09;

	QLabel													*valve_16_00;
	QLabel													*valve_16_01;
	QLabel													*valve_16_02;
	QLabel													*valve_16_03;

	QPushButton											*showAlarmsHistoryBtn;
	QLabel												  *alarmLabel_16_05;
	QLabel												  *alarmLabel_16_06;
	QLabel												  *alarmLabel_16_07;
	QLabel												  *alarmLabel_16_08;
	QLabel												  *alarmLabel_16_09;
	QLabel												  *alarmLabel_16_10;
	QLabel												  *alarmLabel_16_11;

	QLabel													*plcStatusLabel_21_00;
	QLabel													*plcStatusLabel_21_01;
	QLabel													*plcStatusLabel_21_02;
	QLabel													*plcStatusLabel_21_03;
	QLabel													*plcStatusLabel_21_04;
	QLabel													*plcStatusLabel_21_05;
	QLabel													*plcStatusLabel_21_06;
	QLabel													*plcStatusLabel_21_07;
	QLabel													*plcStatusLabel_21_08;
	QLabel													*plcStatusLabel_21_09;
	QLabel													*plcStatusLabel_21_10;
	QLabel													*plcStatusLabel_21_11;
	QLabel													*plcStatusLabel_21_12;
	QLabel													*plcStatusLabel_21_13;


	int															plcModeEnum;
	enum{
		NO_MODE=0,
		AUTO_MODE,
		STEP_MODE,
		MAINTENANCE_MODE,
	};
	Q_OBJECT
public:
  plcManualClass(QWidget *p,plcWidgetClass *plcWidget_,tcpServerClass *cranePlcTcpServer_,tcpServerClass *conveyorsPlcTcpServer_);
  ~plcManualClass();
	void plcStatusChanged(int index,int value);
	void plcFloatValuesChanged(int index,float value);
public slots:
	void buttonReleasedSlot();
	void showAlarmsHistoryBtnSlot();
	//
	void autoRadioButtonSlot(bool checked);
	void setpRadioButtonSlot(bool checked);
	void maintenanceRadioButtonSlot(bool checked);
	void pinsInSlot(bool checked);
	void continueMissionPushButtonSlot(bool checked);
	//void makeStepPushButtonSlot();
	//void recallStopPushButtonSlot();

	//void command10PushButtonSlot();
	//void command11PushButtonSlot();
	//void command12PushButtonSlot();
	//void command13PushButtonSlot();
	//void command14PushButtonSlot();
	//void command15PushButtonSlot();
	//void command16PushButtonSlot();
	//void command17PushButtonSlot();
	//void command18PushButtonSlot();
	//void command19PushButtonSlot();
	//
	//void command20PushButtonSlot();
	//void command21PushButtonSlot();
	//void command22PushButtonSlot();

};
#endif

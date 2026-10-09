#ifndef W_H
#define W_H

#include <QtGui>
#include <QMainWindow>
#include <QList>
#include <QSplitter>
#include <QInputDialog>
#include <QLineEdit>
#include <QScrollArea>
#include "mPos.h"
#include "../common/apexG.h"
#include "../common/btns.h"
#include "pass.h"
class pincerClass : public QWidget
{
	tcpServerClass									*tcpServer;
	QLabel													*fromLabel;
	QLabel													*toLabel;
	QSplitter												*mainSplitter;
	int															secondSpliterPaneWidth;

	QLabel													*fromPlcLabel;
	QLabel													*toPlcLabel;
	QLabel													*fromToPlcStatus;

	myQPushButton										*moveFrom60to61Pushbutton;
	QLabel													*moveFrom60to61Label;

	myQPushButton										*moveFrom54to55Pushbutton;
	QPushButton											*moveFrom55to54Pushbutton;
	QLabel													*moveFrom54to55Label;

	QLabel													*stopLabel;
	QLabel													*okLabel;
	QLabel													*makeResetLabel;
	QLabel													*alarm_13_1Label;
	QLabel													*alarm_13_2Label;
	myQPushButton										*recallStopPushButton;

	QRadioButton									  *autoRadioButton;
	QRadioButton									  *stepRadioButton;
	QRadioButton									  *maintenanceRadioButton;

	myQPushButton										*command10PushButton;
	myQPushButton										*command11PushButton;
	myQPushButton										*command12PushButton;
	myQPushButton										*command13PushButton;
	myQPushButton										*command14PushButton;
	myQPushButton										*command15PushButton;
	myQPushButton										*command16PushButton;
	myQPushButton										*command17PushButton;
	myQPushButton										*command18PushButton;
	myQPushButton										*command19PushButton;

	myQPushButton										*command20PushButton;
	myQPushButton										*command21PushButton;
	myQPushButton										*command22PushButton;
	int															N22_12;//plc status
	bool														N22_21_10;//from ok
	bool														N22_21_11;//to ok
	bool														N22_14_08;//auto mode
	bool														N22_14_09;//step mode
	bool														N22_14_10;//executing mission
	QLabel													*resetingZLabel;
	QLabel													*resetingXLabel;
	QLabel													*resetingYLabel;

	QLabel													*stateLabel_14_10;
	QLabel													*stateLabel_14_11;
	QLabel													*stateLabel_14_12;
	QLabel													*stateLabel_21_12;
	QLabel													*stateLabel_21_14;
	QLabel													*stateLabel_21_15;

	QLabel													*alarmLabel_15_02;
	QLabel													*alarmLabel_15_03;
	QLabel													*alarmLabel_15_04;
	QLabel													*alarmLabel_15_05;
	QLabel													*alarmLabel_15_06;
	QLabel													*alarmLabel_15_07;
	QLabel													*alarmLabel_15_08;
	QLabel													*alarmLabel_15_11;
	QLabel													*alarmLabel_15_12;
	QLabel													*alarmLabel_15_13;

	QLabel													*sensor_13_14;
	QLabel													*sensor_13_15;
	QLabel													*sensor_14_00;
	QLabel													*sensor_14_01;
	QLabel													*sensor_14_02;
	QLabel													*sensor_14_03;
	QLabel													*sensor_14_04;
	QLabel													*sensor_14_05;
	QLabel													*sensor_14_06;

	QLabel													*signal_13_03;
	QLabel													*signal_13_04;
	QLabel													*signal_13_05;
	QLabel													*signal_13_06;
	QLabel													*signal_13_07;
	QLabel													*signal_13_08;
	QLabel													*signal_13_09;
	QLabel													*signal_13_10;

	QLabel													*valve_14_13;
	QLabel													*valve_14_14;
	QLabel													*valve_14_15;
	QLabel													*valve_15_00;

	QLabel													*loadTargetZLabel;
	QLabel													*loadTargetXLabel;
	QLabel													*loadTargetYLabel;

	QLabel													*unloadTargetZLabel;
	QLabel													*unloadTargetXLabel;
	QLabel													*unloadTargetYLabel;

	QLabel													*stepTargetZLabel;
	QLabel													*stepTargetXLabel;
	QLabel													*stepTargetYLabel;

	QLabel													*realPositionZLabel;
	QLabel													*realPositionXLabel;
	QLabel													*realPositionYLabel;

	QLabel													*setSpeedZLabel;
	QLabel													*setSpeedXLabel;
	QLabel													*setSpeedYLabel;

	QLabel													*realSpeedZLabel;
	QLabel													*realSpeedXLabel;
	QLabel													*realSpeedYLabel;

	QLabel													*plcStatusLabel_22_00;
	QLabel													*plcStatusLabel_22_01;
	QLabel													*plcStatusLabel_22_02;
	QLabel													*plcStatusLabel_22_03;
	QLabel													*plcStatusLabel_22_04;
	QLabel													*plcStatusLabel_22_05;
	QLabel													*plcStatusLabel_22_06;
	QLabel													*plcStatusLabel_22_07;
	QLabel													*plcStatusLabel_22_08;
	QLabel													*plcStatusLabel_22_09;
	QLabel													*plcStatusLabel_22_10;
	QLabel													*plcStatusLabel_22_11;
	QLabel													*plcStatusLabel_22_12;
	QLabel													*plcStatusLabel_22_13;


	myQPushButton										*executeBtn;
	QPushButton											*clearBtn;
	myQPushButton										*removeBasketBtn;
	myQPushButton										*makeStepBtn;
	QTimer													removeBasketBlinkingBtnTimer;

	mainPositionClass								*mainPosition10;
	mainPositionClass								*mainPosition11;
	mainPositionClass								*mainPosition12;
	mainPositionClass								*mainPosition20;
	mainPositionClass								*mainPosition21;
	mainPositionClass								*mainPosition30;
	mainPositionClass								*mainPosition40;
	mainPositionClass								*mainPosition50;
	mainPositionClass								*mainPosition51;
	mainPositionClass								*mainPosition52;
	mainPositionClass								*mainPosition53;
	mainPositionClass								*mainPosition54;
	mainPositionClass								*mainPosition55;
	mainPositionClass								*mainPosition60;
	mainPositionClass								*mainPosition61;
	QVector<mainPositionClass*>			allPositions;

	positionClass										*fromPosition;
	positionClass										*toPosition;
    Q_OBJECT
	int															plcModeEnum;
	enum{
		NO_MODE=0,
		AUTO_MODE,
		STEP_MODE,
		MAINTENANCE_MODE,
	};
public:
	pincerClass(tcpServerClass *tcpServer_);
	void plcStatusChanged(int index,int value);
	void plcFloatValuesChanged(int index,float value);
mainPositionClass *createPosition(QString title,int plcId
	,int ind0,int ind1,int ind2,int ind3,int ind4,int ind5,int ind6,int ind7,int ind8
	,int bit0,int bit1,int bit2,int bit3,int bit4,int bit5,int bit6,int bit7,int bit8
	,int com0,int com1,int com2,int com3,int com4,int com5,int com6,int com7,int com8
	,QWidget *p=NULL);
	void clearFromToPositions();
public slots:
	void buttonReleasedSlot();

	void onExecuteBtnClickedSlot();
	void onClearBtnClickedSlot();
	void onPositionClickedSlot(positionClass* pos);

	void openToReleaseBasketSlot();
	//
	void moveFrom60to61PushbuttonSlot();
	void moveFrom54to55PushbuttonSlot();
	void moveFrom55to54PushbuttonSlot();
	//
	void autoRadioButtonSlot(bool checked);
	void setpRadioButtonSlot(bool checked);
	void maintenanceRadioButtonSlot(bool checked);

	void removeBasketBlinkingBtnTimerSlot();

	void splitterMovedSlot(int pos,int index);
};
#endif

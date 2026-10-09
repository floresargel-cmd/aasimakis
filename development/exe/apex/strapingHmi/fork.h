#ifndef FORK_H
#define FORK_H

#include <QtGui>
#include <QMainWindow>
#include <QSplitter>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QGroupBox>
#include <QSplitter>
#include <QInputDialog>

#include "shG.h"
#include "../common/btns.h"
#include "move.h"
#include "ps.h"
#include "bitsVectorBtn.h"
class forkClass : public QWidget
{
	tcpServerClass									*tcpServer;
	//main
	myQPushButton										*autoPushbutton;
	myQPushButton										*manualPushbutton;
	myQPushButton										*maitenancePushbutton;
	QLabel													*stopLabel_5_9;
	bitsVectorBtnClass							*missionFaultsBtn;

	myQPushButton										*takePacketQPushButton;
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
	//
	myQPushButton										*noWareHouseModePushButton;
	QLabel													*modeLabel;
	myQPushButton										*wareHouseModePushButton;
	//
Q_OBJECT
	QVector<myQPushButton*>					areaButtonsVector;
public:
  forkClass(tcpServerClass *tcpServer_,QWidget *p);
	void plcMemoryChanged(int index,int value);
	void plcFloatMemoryChanged(int index,double value);
	void hideButtons();
	void showButtons();
public slots:
	void noWareHouseModePushButtonSlot();
	void wareHouseModePushButtonSlot();
};
#endif

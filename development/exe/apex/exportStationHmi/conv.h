#ifndef CONV_H
#define CONV_H

#include <QtGui>
#include <QMainWindow>
#include <QList>
#include <QGroupBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QScrollArea>

#include "../common/apexG.h"
#include "../common/btns.h"
#include "move.h"
#include "movement.h"
#include "ps.h"
#include "eMem.h"
class conveyorsClass : public QWidget
{
	tcpServerClass									*tcpServer;
	//main
	myQPushButton										*autoPushbutton;
	QLabel													*stopLabel_0_3;
	//feeding
	myQPushButton										*autoFeeding13Pushbutton;
	myQPushButton										*autoFeeding15Pushbutton;
	QLabel													*command51_52Label;
	//
	QLabel													*signal_01_00;
	QLabel													*signal_01_01;
	QLabel													*signal_01_02;
	QLabel													*signal_01_03;
	QLabel													*signal_01_04;
	QLabel													*signal_01_05;
	QLabel													*signal_01_06;
	QLabel													*signal_01_07;
	QLabel													*signal_01_08;
	QLabel													*signal_01_09;
	//Receiving signals from basket crane
	QLabel													*signal_02_21;
	QLabel													*signal_02_22;
	QLabel													*signal_02_23;
	QLabel													*signal_02_24;
	QLabel													*signal_02_25;
	QLabel													*signal_02_26;
	QLabel													*signal_02_27;
	QLabel													*signal_00_28;
	//Transmitting signals to basket crane
	QLabel													*signal_00_16;
	QLabel													*signal_00_17;
	QLabel													*signal_00_18;
	QLabel													*signal_00_19;
	QLabel													*signal_00_20;
	QLabel													*signal_00_21;
	QLabel													*signal_00_22;
	QLabel													*signal_00_23;
	//Warnings
	QLabel													*signal_05_00;
	QLabel													*signal_05_01;
	QLabel													*signal_05_02;
	QLabel													*signal_05_03;
	QLabel													*signal_05_04;
	QLabel													*signal_05_05;
	QLabel													*signal_05_06;
	QLabel													*signal_05_07;
	QLabel													*signal_05_08;
	QLabel													*signal_05_09;
	QLabel													*signal_05_10;
	QLabel													*signal_05_11;
	QLabel													*signal_05_12;
	QLabel													*signal_05_13;
	QLabel													*signal_05_14;
	QLabel													*signal_05_15;
	QLabel													*signal_05_16;
	QLabel													*signal_05_17;
	QLabel													*signal_05_18;
	//Alarms
	QLabel													*signal_08_00;
	QLabel													*signal_08_01;
	QLabel													*signal_08_02;
	QLabel													*signal_08_03;
	QLabel													*signal_08_04;
	QLabel													*signal_08_05;
	QLabel													*signal_08_06;
	QLabel													*signal_08_07;
	QLabel													*signal_08_08;
	QLabel													*signal_08_09;
	QLabel													*signal_08_10;
	QLabel													*signal_08_11;
	QLabel													*signal_08_12;
	//
	myQPushButton										*cylinderUpButton;
	myQPushButton										*cylinderDownButton;
	QLabel													*cylinderLabel;
	bool														cylinderUp;
	bool														cylinderDown;
	//
Q_OBJECT
	QVector<moveConveyorClass*>					areaButtonsVector;
 	QVector<conveyorMovementClass*>			conveyorMovementVector;
 	QVector<proximitySwitchClass*>			proximitySwitchVector;
 	QVector<expStationPlcMemoryClass*>	plcMemoryVector;

public:
  conveyorsClass(tcpServerClass *tcpServer_,QWidget *p);
	void plcMemoryChanged(int index,int value);
	void hideButtons();
	void showButtons();
//public slots:
};
#endif

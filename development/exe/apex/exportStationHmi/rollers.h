#ifndef ROLLERS_H
#define ROLLERS_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSplitter>
#include <QGroupBox>
#include <QScrollArea>

#include "../common/apexG.h"
#include "../common/btns.h"
#include "move.h"
#include "movement.h"
#include "ps.h"
#include "eMem.h"
class rollersClass : public QWidget	
{
	tcpServerClass									*tcpServer;
	//main
	myQPushButton										*autoPushbutton;
	QLabel													*stopLabel_0_3;
	//active commands
	QLabel													*signal_35_00;
	QLabel													*signal_35_01;
	QLabel													*signal_35_02;
	QLabel													*signal_35_03;
	QLabel													*signal_35_04;
	QLabel													*signal_35_05;
	QLabel													*signal_35_06;
	QLabel													*signal_35_07;
	//packets
	QLabel													*signal_35_16;
	QLabel													*signal_35_17;
	QLabel													*signal_35_18;
	QLabel													*signal_35_19;
	QLabel													*signal_35_20;
	//SB
	QLabel													*signal_35_21;
	QLabel													*signal_35_22;
	QLabel													*signal_35_23;
	QLabel													*signal_35_24;
	QLabel													*signal_35_25;
	//Side moving aligment
	QLabel													*signal_35_08;
	QLabel													*signal_35_09;
	QLabel													*signal_35_10;
	QLabel													*signal_35_11;
	//Side moving position
	QLabel													*signal_35_14;
	QLabel													*signal_35_13;
	QLabel													*signal_35_12;

	//Warnings
	QLabel													*signal_37_00;
	QLabel													*signal_37_01;
	QLabel													*signal_37_02;
	QLabel													*signal_37_03;
	QLabel													*signal_37_04;
	QLabel													*signal_37_05;
	QLabel													*signal_37_06;
	QLabel													*signal_37_07;
	QLabel													*signal_37_08;
	QLabel													*signal_37_09;
	QLabel													*signal_37_10;
	QLabel													*signal_37_11;
	QLabel													*signal_37_12;
	QLabel													*signal_37_13;
	QLabel													*signal_37_14;
	QLabel													*signal_37_15;
	QLabel													*signal_37_16;
	QLabel													*signal_37_17;
	//Alarms
	QLabel													*signal_38_00;
	QLabel													*signal_38_01;
	QLabel													*signal_38_02;
	QLabel													*signal_38_03;
	QLabel													*signal_38_04;
	//
Q_OBJECT
	QVector<moveConveyorClass*>			areaButtonsVector;
 	QVector<conveyorMovementClass*>	conveyorMovementVector;
public:										
  rollersClass(tcpServerClass *tcpServer_,QWidget *p);
	void plcMemoryChanged(int index,int value);
	void hideButtons();
	void showButtons();
//public slots:
};																	
#endif
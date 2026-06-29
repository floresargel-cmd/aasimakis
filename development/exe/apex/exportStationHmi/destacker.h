#ifndef DESTACKER_H
#define DESTACKER_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSplitter>
#include <QGroupBox>

#include "../common/apexG.h"
#include "../common/btns.h"
#include "move.h"
#include "movement.h"
#include "ps.h"
#include "eMem.h"
#include "sp.h"
class destackerClass : public QWidget	
{
	tcpServerClass									*tcpServer;
	spacersClass										*spacers;
	//main
	//myQPushButton										*autoPushbutton;
	//QLabel													*stopLabel_0_3;
	//pincer
	QLabel													*signal_25_00;
	QLabel													*signal_25_01;
	QLabel													*signal_25_02;
	QLabel													*signal_25_03;
	QLabel													*signal_25_04;
	QLabel													*signal_25_05;
	QLabel													*signal_25_06;
	QLabel													*signal_25_07;
	//photosencors
	QLabel													*signal_25_08;
	//grabing
	QLabel													*signal_25_15;
	QLabel													*signal_25_16;
	QLabel													*signal_25_17;
	QLabel													*signal_25_18;
	QLabel													*signal_25_19;
	QLabel													*signal_25_20;
	QLabel													*signal_25_21;
	QLabel													*signal_25_22;
	//hooks
	QLabel													*signal_25_25;
	QLabel													*signal_25_26;

	//Alarms
	QLabel													*signal_28_00;
	QLabel													*signal_28_01;
	QLabel													*signal_28_02;
	QLabel													*signal_28_03;
	QLabel													*signal_28_04;

	QLabel													*signal_28_06;
	QLabel													*signal_28_07;
	//spacers
	QLabel													*signal_28_23;
	QLabel													*signal_28_24;
	//
Q_OBJECT
public:										
  destackerClass(tcpServerClass *tcpServer_,QWidget *p);
	void plcMemoryChanged(int index,int value);
	void destackerBasketChanged(int value);
	void spacersMemoryChanged(int index,int value);//N163
	void hideButtons();
	void showButtons();
//public slots:
};																	
#endif
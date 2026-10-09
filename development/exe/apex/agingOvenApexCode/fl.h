#ifndef FL_H
#define FL_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include "aG.h"
class myFaultListWidgetItem:public QListWidgetItem
{
public:
	myFaultListWidgetItem(QString name):QListWidgetItem(name)
	{
			setForeground(QBrush(QColor(200,0,0)));
	};
};
class myStatusListWidgetItem:public QListWidgetItem
{
public:
	myStatusListWidgetItem(QString name):QListWidgetItem(name)
	{
			setForeground(QBrush(QColor(0,150,0)));
	};
};
class faultsListsClass:public QWidget
{
	tcpServerClass														*tcpServer;
	myFaultListWidgetItem											*N24_00;
	myFaultListWidgetItem											*N24_01;
	myFaultListWidgetItem											*N24_02;
	myFaultListWidgetItem											*N24_03;
	myFaultListWidgetItem											*N24_04;
	myFaultListWidgetItem											*N24_05;
	myFaultListWidgetItem											*N24_06;
	myFaultListWidgetItem											*N24_07;
	myFaultListWidgetItem											*N24_08;
	myFaultListWidgetItem											*N24_09;
	myFaultListWidgetItem											*N24_10;
	myFaultListWidgetItem											*N24_11;
	myFaultListWidgetItem											*N24_12;
	myFaultListWidgetItem											*N24_13;
	myFaultListWidgetItem											*N24_14;
	myFaultListWidgetItem											*N24_15;
	myFaultListWidgetItem											*N24_16;
	myFaultListWidgetItem											*N24_17;
	myFaultListWidgetItem											*N24_18;
	myFaultListWidgetItem											*N24_19;
	myFaultListWidgetItem											*N24_20;
	myFaultListWidgetItem											*N24_21;
	myFaultListWidgetItem											*N24_22;
	myFaultListWidgetItem											*N24_23;
	myFaultListWidgetItem											*N24_24;
	myFaultListWidgetItem											*N24_25;
	myFaultListWidgetItem											*N24_26;
	myFaultListWidgetItem											*N24_27;
	myFaultListWidgetItem											*N24_28;
	myFaultListWidgetItem											*N24_29;
	myFaultListWidgetItem											*N24_30;
	myFaultListWidgetItem											*N24_31;

	myFaultListWidgetItem											*N25_00;
	myFaultListWidgetItem											*N25_01;
	myFaultListWidgetItem											*N25_02;
	myFaultListWidgetItem											*N25_03;
	myFaultListWidgetItem											*N25_04;
	myFaultListWidgetItem											*N25_05;
	myFaultListWidgetItem											*N25_06;
	myFaultListWidgetItem											*N25_07;
	myFaultListWidgetItem											*N25_08;
	myFaultListWidgetItem											*N25_09;
	myFaultListWidgetItem											*N25_10;
	myFaultListWidgetItem											*N25_11;
	myFaultListWidgetItem											*N25_12;
	myFaultListWidgetItem											*N25_13;

	myStatusListWidgetItem										*N26_00;
	myStatusListWidgetItem										*N26_01;
	myStatusListWidgetItem										*N26_02;
	myStatusListWidgetItem										*N26_03;
	myStatusListWidgetItem										*N26_04;
	myStatusListWidgetItem										*N26_05;
	myStatusListWidgetItem										*N26_06;
	myStatusListWidgetItem										*N26_07;
	myStatusListWidgetItem										*N26_13;
	myStatusListWidgetItem										*N26_14;
	myStatusListWidgetItem										*N26_15;
	myStatusListWidgetItem										*N26_16;

	myStatusListWidgetItem										*N26_17;
	myStatusListWidgetItem										*N26_18;
	myStatusListWidgetItem										*N26_19;
	myStatusListWidgetItem										*N26_20;
	myStatusListWidgetItem										*N26_21;
	myStatusListWidgetItem										*N26_22;
	myStatusListWidgetItem										*N26_23;
	myStatusListWidgetItem										*N26_24;
	myStatusListWidgetItem										*N26_25;
	myStatusListWidgetItem										*N26_26;
	myStatusListWidgetItem										*N26_27;
	myStatusListWidgetItem										*N26_28;
	myStatusListWidgetItem										*N26_29;
	myStatusListWidgetItem										*N26_30;
	myStatusListWidgetItem										*N26_31;
public:
	Q_OBJECT
public:
	faultsListsClass(tcpServerClass *tcpServer_,QWidget *p);
	void plcStatusChanged(int index,int value);
};
#endif

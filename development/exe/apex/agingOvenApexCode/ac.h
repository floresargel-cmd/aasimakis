#ifndef AC_H
#define AC_H

#include <QtGui>
#include <QMainWindow>
#include <QGroupBox>
#include "aG.h"

class statusLabel:public QLabel
{
public:
	statusLabel(QString name,QWidget *p):QLabel(name,p)
	{
	};
	void setStatus(bool value)
	{
		if (value==true)
			setStyleSheet("QLabel{color:#000088;}");
		else
			setStyleSheet("QLabel{color:#999999;}");
	}
};
class autoCircleGroupClass:public QGroupBox
{
	tcpServerClass														*tcpServer;
	statusLabel																*statusLabel01;
	statusLabel																*statusLabel02;
	statusLabel																*statusLabel03;
	statusLabel																*statusLabel04;
	statusLabel																*statusLabel05;
	statusLabel																*statusLabel06;

	statusLabel																*statusLabel09;
	statusLabel																*statusLabel10;
	statusLabel																*statusLabel11;
	statusLabel																*statusLabel12;

	statusLabel																*statusLabel07;
	statusLabel																*statusLabel08;
	statusLabel																*statusLabel13;
	statusLabel																*statusLabel14;

	statusLabel																*statusLabel19;
	statusLabel																*statusLabel20;
	statusLabel																*statusLabel21;
public:
	Q_OBJECT
public:
	autoCircleGroupClass(tcpServerClass *tcpServer_,QWidget *p);
	void plcStatusChanged(int index,int value);
};
#endif

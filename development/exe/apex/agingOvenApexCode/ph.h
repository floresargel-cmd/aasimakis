#ifndef PH_H
#define PH_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include "aG.h"
class photoSensorClass:public QWidget 				
{
	int																		plcIndex;
	int																		plcBit;
	int																		x;
	int																		y;
	tcpServerClass												*tcpServer;
	//
	QLabel																*titleLabel;
	QLabel																*imageLabel;
public:
	int																		typeEnum;
	enum{
		TYPE_SINGLE=1,
		TYPE_DOUBLE
	};
	Q_OBJECT								
public:										
	photoSensorClass(tcpServerClass *tcpServer_,int plcIndex_,int plcBit_,int typeEnum_,int x_,int y_,QString title,QWidget *p);
	void plcValueChanged(int index,int bit,bool value);
};											
#endif

#ifndef PS_H
#define PS_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include "aG.h"
class proximitySwitchClass:public QWidget 				
{
	int																		plcIndex;
	int																		plcBit;
	tcpServerClass												*tcpServer;
	//
	QLabel																*imageLabel;
public:
	Q_OBJECT								
public:										
	proximitySwitchClass(tcpServerClass *tcpServer_,int plcIndex,int plcBit,int x_,int y_,QString title,QWidget *p);
	void plcValueChanged(int index,int bit,bool value);
};											
#endif

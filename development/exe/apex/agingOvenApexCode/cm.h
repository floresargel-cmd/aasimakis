#ifndef CM_H
#define CM_H

#include <QtGui>
#include <QMainWindow>
#include <QLabel>
#include "aG.h"

class conveyorMovmentClass:public QLabel
{
	int									plcIndex1;
	int									plcBit1;
	int									plcIndex2;
	int									plcBit2;

	QImage								*image1;
	QImage								*image2;

	tcpServerClass						*tcpServer;

	bool								firstBitUpdated;
	bool								secondBitUpdated;
	bool								bit1Value;
	bool								bit2Value;

public:
	Q_OBJECT

public:
	conveyorMovmentClass(tcpServerClass *tcpServer_,int plcIndex1_,int plcBit1_,QString image1Str,int plcIndex2_,int plcBit2_,QString image2Str,int x,int y,QWidget *p);
	void plcValueChanged(int index,int bit,bool value);
	void updateImages();

};
#endif

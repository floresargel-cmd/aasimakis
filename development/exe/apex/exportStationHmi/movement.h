#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include "ehG.h"
#include "../common/apexG.h"
class conveyorMovementClass:public QLabel 				
{
	int																		plcIndex1;
	int																		plcBit1;
	int																		plcIndex2;
	int																		plcBit2;
	QImage																*image1;
	QImage																*image2;
	bool																	firstBitUpdated;
	bool																	secondBitUpdated;
	bool																	bit1Value;
	bool																	bit2Value;
public:
	Q_OBJECT								
public:										
	conveyorMovementClass(int plcIndex1_,int plcBit1_,QString image1Str,int plcIndex2_,int plcBit2_,QString image2Str,int x,int y,QWidget *p);
	void plcValueChanged(int plcIndex,int plcValue);
	void updateImages();
};											
#endif

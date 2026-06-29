#ifndef CM_H
#define CM_H

#include "pcG.h"
#include "../common/apexG.h"
class conveyorMovmentClass:public QLabel 				
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
	conveyorMovmentClass(int plcIndex1_,int plcBit1_,QString image1Str,int plcIndex2_,int plcBit2_,QString image2Str,int x,int y,QWidget *p);
	~conveyorMovmentClass();
	void plcValueChanged(int index,int bit,bool value);
	void updateImages();
};											
#endif

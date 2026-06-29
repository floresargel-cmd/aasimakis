#ifndef PS_H
#define PS_H

#include <QtGui>
#include <QMainWindow>
#include <QLabel>
#include "../common/apexG.h"
#include "ehG.h"
class proximitySwitchClass:public QWidget 				
{
	int																		plcIndex;
	int																		plcBit;
	//
	QLabel																*imageLabel;
public:
	Q_OBJECT								
public:										
	proximitySwitchClass(int plcIndex,int plcBit,int x_,int y_,QString title,QWidget *p);
	void plcValueChanged(int index,int bit,bool value);
};											
#endif

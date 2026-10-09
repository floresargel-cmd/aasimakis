#ifndef PS_H
#define PS_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include "pcG.h"
#include "../common/apexG.h"
class packetProximitySwitchClass:public QWidget
{
	int																		plcIndex;
	int																		plcBit;
	//
	QLabel																*imageLabel;
	QLabel																*titleLabel;
	QString																styleOn;
public:
	Q_OBJECT
public:
	packetProximitySwitchClass(int plcIndex,int plcBit,int x_,int y_,QString title,QWidget *p,int dx=+10,int dy=-6,QString styleOn_="QLabel{color:#14f75a;}");
	void plcValueChanged(int index,int bit,bool value);
};
#endif

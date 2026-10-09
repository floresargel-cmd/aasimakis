#ifndef MEM_H
#define MEM_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include "pcG.h"
#include "../common/apexG.h"
class plcMemoryClass:public QWidget
{
	int																		plcIndex;
	tcpServerClass												*tcpServer;
	QLabel																*valueLabel;
	//
	int																		plcValue;
public:

public:
	Q_OBJECT
public:
	plcMemoryClass(tcpServerClass *tcpServer_,int plcIndex_,int x,int y,QString title,QString toolTip,QWidget *p);
	bool plcValueChanged(int index,int value);
	void mouseReleaseEvent(QMouseEvent *ev);
private:
	void setPlcValue(int value);
	void writeValueToPlc(int value);
};
#endif

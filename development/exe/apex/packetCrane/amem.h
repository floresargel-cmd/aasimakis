#ifndef AMEM_H
#define AMEM_H

#include "pcG.h"
#include "../common/apexG.h"
//////////////////////////////////////////////apex PlcMemoryClass
class aPlcMemoryClass:public QWidget 				
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
	aPlcMemoryClass(tcpServerClass *tcpServer_,int plcIndex_,int x,int y,QString title,QString toolTip,QWidget *p);
	bool plcValueChanged(int index,int value);
	void mouseReleaseEvent(QMouseEvent *ev);
private:
	void setPlcValue(int value);
	void writeValueToPlc(int value);
};											
#endif

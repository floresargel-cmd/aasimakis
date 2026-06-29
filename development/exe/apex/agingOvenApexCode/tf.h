#ifndef TF_H
#define TF_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include "aG.h"
class thirtyFeetClass:public QCheckBox 				
{
	tcpServerClass												*tcpServer;
	QTimer																clearCommandTimer;
	int																		truePlcCommand;
	int																		falsePlcCommand;
	int																		plcIndex;
	int																		plcBit;
public:
	Q_OBJECT
public:										
	thirtyFeetClass(tcpServerClass *tcpServer_,int truePlcCommand_,int falsePlcCommand_,int plcIndex_,int plcBit_,int x,int y,QWidget *p);
	void nextCheckState();
	void plcValueChanged(int index,int bit,bool value);
public slots:								
	void onClearCommandTimerSlot();
};											
#endif

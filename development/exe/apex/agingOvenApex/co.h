#ifndef CO_H
#define CO_H

#include <QtGui>
#include <QMainWindow>
#include <QGroupBox>
#include "aG.h"
#include "../common/btns.h"
class commandsGroupClass:public QGroupBox
{
	tcpServerClass												*tcpServer;
	//
	myQPushButton														*autoPushButton;
	myQPushButton														*manualPushButton;
	myQPushButton														*resetPushButton;
	myQPushButton														*resetFultsPushButton;
	QLabel																	*stopLabel;
public:
	Q_OBJECT
public:
	commandsGroupClass(tcpServerClass *tcpServer_,QWidget *p);
	void plcStatusChanged(int index,int value);
	void activateStop(bool a);
public slots:
	void loadOvenPushButtonSlot();
	void unloadOvenPushButtonSlot();

	///////////////////////////
	//Code added by Vahid
	///////////////////////////
	void loadOven2PushButtonSlot();
	void unloadOven2PushButtonSlot();
	///////////////////////////
};
#endif

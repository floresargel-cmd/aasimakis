#ifndef MOVE_H
#define MOVE_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include "shG.h"
#include "../common/apexG.h"
#include "../common/btns.h"
class moveConveyorClass:public myQPushButton
{
	tcpServerClass												*tcpServer;
	int																		plcCommand;
public:
	int																		typeEnum;
	enum{
		TYPE_LEFT=1,
		TYPE_RIGHT,
		TYPE_UP,
		TYPE_DOWN,
	};
	Q_OBJECT
public:
	moveConveyorClass(tcpServerClass *tcpServer_,int typeEnum_,QString plcCommandString,QString plcZeroString,QString name,int x,int y,QWidget *p);
};
#endif

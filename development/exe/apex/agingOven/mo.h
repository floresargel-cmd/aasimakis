#ifndef MO_H
#define MO_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include "aG.h"
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
	moveConveyorClass(tcpServerClass *tcpServer_,int typeEnum_,int plcCommand,QString name,int x,int y,QWidget *p,int w=30,int h=30);
	//void mousePressEvent(QMouseEvent *e);
	//void mouseReleaseEvent(QMouseEvent *e);
	//void leaveEvent(QEvent *ev);
};											
#endif

#ifndef EMULATEG_H
#define EMULATEG_H

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QDialog>
#include <QString>
#include <QtNetwork>
#include "../common/tcpTux.h"
#include "cG.h"
#define EMULATE_DB_CONNECTION "emulate"
static void moveBasket(QString from,QString to,int basketNo)
{
	if (basketNo>0)
	{
		execQuery(QString("update positions set basket=%1 where description='%2'").arg(basketNo).arg(to),EMULATE_DB_CONNECTION);
		execQuery(QString("update positions set basket=0 where description='%1'").arg(from),EMULATE_DB_CONNECTION);
	}
}
static void sendBit(tuxipClass *tuxipConnection,QString adress,int index,int bit,bool value)
{
	if (value)
		tuxipConnection->writeInteger(adress,index,BITS[bit]);
	else
		tuxipConnection->writeInteger(adress,index,0);
}
#endif

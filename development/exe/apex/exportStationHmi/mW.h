#ifndef MW_H
#define MW_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "../common/apexG.h"
#include "ehG.h"
#include "p.h"
#include "conv.h"
#include "rollers.h"
#include "destacker.h"
class mainWindowClass : public QWidget
{
	QTimer									mainTimer;
#ifdef TUX_PLC_COMMS
	tuxipServerClass				*tuxipServer;
#endif
#ifdef TUX_PLC_COMMS
	tuxipClass							*tuxIpPincer;
	tuxipClass							*tuxIpPacking;
#endif
	tcpServerClass					*pincerTcpServer;
	tcpServerClass					*packingTcpServer;
	pincerClass							*pincer;
	conveyorsClass					*conveyors;
	rollersClass						*rollers;
	destackerClass					*destacker;
Q_OBJECT
public:
    mainWindowClass();
		~mainWindowClass();
public slots:
	void onMainTimer();
	void showServerSlot();

	void signalFromPincerTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void signalFromPackingTuxPlcSlot(QString ip,QString table,int index,QVariant value);

	void signalFromPincerPlcSlot(QString table,int index,QString value);
	void signalFromPackingPlcSlot(QString table,int index,QString value);
	void qtAboutSlot();
};
#endif

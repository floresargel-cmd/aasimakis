#ifndef MW_H
#define MW_H

#include <QtGui>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "shG.h"
#include "straping.h"
#include "fork.h"
#include "SyncSerialComm.h"
#include "pass.h"
class mainWindowClass : public QWidget		
{
	QTimer									mainTimer;
	QTimer									barcodeReadTimer;
	strapingClass						*straping;
	forkClass								*fork;
	CSyncSerialComm					*barcodeReadCom;
#ifdef TUX_PLC_COMMS
	tuxipServerClass				*tuxipServer;
#endif
	tcpServerClass					*tcpServer;
	QTabWidget							*mainTabWidget;
	int											lastCurrentTabIndex;
Q_OBJECT								
public:										
    mainWindowClass();
		~mainWindowClass();
public slots:
//for backwards compatibility
	void signalFromTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();
//for backwards compatibility
	void onMainTimerSlot();
	void signalFromPlcSlot(QString table,int index,QString value);
	void onBarcodeReadTimerSlot();
	void currentTabChangedSlot(int index);
	void qtAboutSlot();
};																	
#endif
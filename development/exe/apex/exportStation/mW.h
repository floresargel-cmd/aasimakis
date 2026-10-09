#ifndef MW_H
#define MW_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>

#include "eG.h"
#include "exp.h"
#include "../common/btns.h"
class mainWindowClass : public QWidget
{
	tuxipServerClass				*tuxipServer;
	tcpServerClass					*tcpServer;
	myQPushButton						*importStationDefineNewBasketPushButton;
	myQPushButton						*importStationDefineNewScrapBasketPushButton;
	myQPushButton						*importStationBasketIsReadyForImportationPushButton;
	myQPushButton						*importStationIsWithoutBasketPushButton;
	//
	exportStationClass			*omavStation;
	exportStationClass			*assimStation;
Q_OBJECT
public:
	mainWindowClass();
	~mainWindowClass();
public slots:
	void onMainTimer();
	void onImportStationDefineNewBasketPushButtonSlot();
	void onImportStationDefineNewScrapBasketPushButtonSlot();
	void onImportStationBasketIsReadyForImportationPushButtonSlot();
	void onImportStationIsWithoutBasketPushButtonSlot();
	void signalFromTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void signalFromPlcSlot(QString table,int index,QString value);
	void showServerSlot();
	void qtAboutSlot();
};
#endif

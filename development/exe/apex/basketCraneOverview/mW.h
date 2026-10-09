#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>

#include "Gbo.h"
#include "serv.h"
#include "cW.h"
#include "../common/apexG.h"
class mainWindowClass : public QWidget
{
	tuxipServerClass									*tuxipServer;
	emptyBasketsClass									*emptyBaskets;
	totalUnagedBasketsClass						*totalUnagedBaskets;
	longUnagedBasketsClass						*longUnagedBaskets;
	availableBasketsForPackagingClass	*availableBasketsForPackaging;
	basketsInOvenClass								*basketsInOven;
	basketsAfterOvenClass							*basketsAfterOven;
	emptyBasketsNeededClass						*emptyBasketsNeeded;
Q_OBJECT
public:
	mainWindowClass();
	~mainWindowClass();
	void initializeDatabase();
	void initializePlc();
public slots:
	void onMainTimerSlot();
	void onDatabaseTimerSlot();
	void showServerSlot();
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
	void qtAboutSlot();
};
#endif

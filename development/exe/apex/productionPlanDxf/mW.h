#ifndef MW_H
#define MW_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSplitter>
#include <QInputDialog>

#include "pdG.h"
#include "bs.h"
#include "pp.h"
class mainWindowClass : public QWidget		
{
	QTimer														mainTimer;
	//
	QVBoxLayout												*basketLayout;
	basketClass												*basket;
	profileProductionClass						*profileProduction;
	//
Q_OBJECT								
public:										
	mainWindowClass();
	~mainWindowClass();
public slots:
	void mainTimerSlot();
	void productionChangedSlot();
	void f5Slot();
	void qtAboutSlot();
};																	
#endif
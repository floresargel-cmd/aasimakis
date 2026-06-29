#ifndef MAINWINDOW_H
#define MAINWINDOW_H
class groupOfCellsClass;
#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>

#include "manGrOfCells.h"
#include "cr.h"
class manualCassetteMovementClass : public QWidget
{												
	craneClass						*crane;
	QTimer								mainTimer;
	QPushButton						*returnCassetteOfEastConveyor;
	QPushButton						*returnCassetteOfWestConveyor;
	QLabel								*eastConvLaybel;
	QLabel								*westConvLaybel;
	groupOfCellsClass			*eastGroup0;
	groupOfCellsClass			*eastGroup1;
	groupOfCellsClass			*eastGroup2;
	groupOfCellsClass			*westGroup0;
	groupOfCellsClass			*westGroup1;
	groupOfCellsClass			*westGroup2;
Q_OBJECT								
public:										
	manualCassetteMovementClass(craneClass *crane_);							
	~manualCassetteMovementClass();							
	void updateFromDb();
public slots:								
	void onMainTimer();						
	void returnCassetteOfEastConveyorBtnSlot();						
	void returnCassetteOfWestConveyorBtnSlot();						

};											
#endif

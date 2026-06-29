#ifndef MMISSIONSPOS_H
#define MMISSIONSPOS_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "pcG.h"
#include "cP.h"
#include "p.h"
class manualMissionsPacketClass : public QWidget	
{
	packetPositionClass										*packetPos;
	QSpinBox															*packetSpin;
Q_OBJECT
public:		
  manualMissionsPacketClass(packetPositionClass *packetPos_);
	void updateGui();
	void saveToDataBase();
public slots:
	void basketSpinBoxValueChangedSlot(int p);
};					
class manualMissionsCellClass : public QWidget	
{
	cellPositionClass										*cell;
	QLabel															*casseteLabel;
	QLabel															*packetLabel;
Q_OBJECT
public:		
  manualMissionsCellClass(cellPositionClass *cell_);
	void updateGui();
public slots:
	void changeDataBtnClickedSlot();
};																	
#endif
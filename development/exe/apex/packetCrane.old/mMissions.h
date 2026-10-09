#ifndef MMISSIONS_H
#define MMISSIONS_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "pcG.h"
#include "ap.h"
#include "cr.h"
#include "fr.h"
#include "mMissionsP.h"
class manualMissionsClass : public QDialog
{
	craneClass													*crane;
	forkClass														*fork;
	allPosisionsClass										*allPosisions;
	QVector<manualMissionsCellClass*>		cells;
	QVector<manualMissionsPacketClass*>	packetPositions;
	//
	QCheckBox														*westConveyorActivePacketExportCheckBox;
	QCheckBox														*westConveyorActivePacketImportCheckBox;
	QCheckBox														*westConveyorActiveCassetteImportCheckBox;
	QCheckBox														*eastConveyorActivePacketExportCheckBox;
	QCheckBox														*eastConveyorActivePacketImportCheckBox;
	QCheckBox														*eastConveyorActiveCassetteImportCheckBox;

	QComboBox														*forkFromComboBox;
	QComboBox														*forkToComboBox;

	manualCassetteMovementClass					*manualCassetteMovement;
Q_OBJECT
public:
  manualMissionsClass(craneClass *cr,forkClass *fr,allPosisionsClass *all,QWidget *p=NULL);
	void updateGui();
	int exec();
public slots:
	void updateTimerSlot();
	void saveForkPosBtnSlot();
	void saveStatesBtnSlot();
	void startForkBtnSlot();
	void startCraneBtnSlot();
};
#endif

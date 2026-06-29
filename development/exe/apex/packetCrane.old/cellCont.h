#ifndef CELLCONT_H
#define CELLCONT_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "pcG.h"
#include "../common/apexG.h"
#include "../common/btns.h"

class cellContentsClass : public QDialog	
{
	int							cellNumber;
	int							packet;
	bool						isLocked;
	//
	QSpinBox				*packetSpinBox;
	QCheckBox				*isLockedCheckBox;
	QRadioButton		*cassetteInCellRadioButton;
	QRadioButton		*cassetteOnCraneRadioButton;
	QRadioButton		*cassetteOnWestConveyorRadioButton;
	QRadioButton		*cassetteOnEastConveyorRadioButton;

	QPushButton			*savePushButton;
	QPushButton			*cancelPushButton;
	bool						isPackOk;
	//
	int							cassetePosEnum;
	enum{
		CASSETTE_NO_WHERE=0,
		CASSETTE_IN_CELL,
		CASSETTTE_ON_CRANE,
		CASSETTTE_ON_WEST_CONVEYOR,
		CASSETTTE_ON_EAST_CONVEYOR,
	};
	//
Q_OBJECT
public:		
  cellContentsClass(int cellNumber_=0,QWidget *p=NULL);
	void updateGui();
	void saveToDatabase();
	void loadFromDatabase();
	void check();
signals:
	void cellDataChangedSignal(int cell);
public slots:
	void onPacketSpinBoxChangedSlot(int pack);
	void onLockedCheckButtonClickedSlot(int state);
	void onCassetteInCellRadioButtonClickedSlot(bool checked);
	void onCassetteOnCraneRadioButtonClickedSlot(bool checked);
	void onCassetteOnWestConveyorRadioButtonClickedSlot(bool checked);
	void onCassetteOnEastConveyorRadioButtonClickedSlot(bool checked);
	void onSaveBtnSlot();
};																	
#endif
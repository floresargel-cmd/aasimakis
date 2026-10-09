#ifndef CELLCONT_H
#define CELLCONT_H

#include "pcG.h"
#include "../common/apexG.h"
#include "../common/btns.h"
class cellContentsClass : public QDialog
{
	bool									takesCassette=false;
	bool									isCell=false;
	int										posId=0;
	int										cassette=0;
	int										packet=0;
	bool									isLocked=false;
	int										xCorr=0;
	int										zCorr=0;
	//
	QSpinBox							*packetSpinBox=nullptr;
	QSpinBox							*cassetteSpinBox=nullptr;
	QCheckBox							*isLockedCheckBox=nullptr;
	QSpinBox							*xCorrSpinBox=nullptr;
	QSpinBox							*zCorrSpinBox=nullptr;
	QPushButton						*savePushButton=nullptr;
	QPushButton						*cancelPushButton=nullptr;
Q_OBJECT
public:
	cellContentsClass(int posId_=0,QWidget *p=NULL);
	int getBasketNum();
	int getPacketId();
	void updateGui();
	void saveToDatabase();
	void loadFromDatabase();
	void check();
signals:
	void cellDataChangedSignal(int cell);
	void cassetteChangedSignal(int cassette);
public slots:
	void onPacketSpinBoxChangedSlot(int pack);
	void onCassetteSpinBoxSpinBoxChangedSlot(int cass);
	void onLockedCheckButtonClickedSlot(int state);
	void onXCorrSpinBoxSpinBoxChangedSlot(int pack);
	void onZCorrSpinBoxSpinBoxChangedSlot(int pack);
	void onSaveBtnSlot();
};
#endif

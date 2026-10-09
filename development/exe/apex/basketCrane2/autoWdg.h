#ifndef AUTOWDG_H
#define AUTOWDG_H

#include "Gb2.h"
#include "crane.h"
class autoWidgetClass : public QWidget
{
	craneClass									*crane;
	QCheckBox										*exportsToDestackerCheckBtn;//sb warehouse area
	QCheckBox										*exportsToPackingDestacker1CheckBtn;//packing area
	QCheckBox										*exportsToPackingDestacker2CheckBtn;//packing area
	QCheckBox										*exportsToAnodizingCheckBtn;//export
	QCheckBox										*importsFromNorthCheckBtn;
	QCheckBox										*importsFromSouthCheckBtn;
	QCheckBox										*useEpicsCheckBtn;
	Q_OBJECT
public:
	autoWidgetClass(craneClass *crane_,QWidget *p=NULL);
	~autoWidgetClass();
signals:
	void calculateExportGroupsSignal();
public slots:
	void exportsToDestackerCheckBtnStateChangedSlot(int state);
	void exportsToPackingDestacker1CheckBtnStateChangedSlot(int state);
	void exportsToPackingDestacker2CheckBtnStateChangedSlot(int state);
	void exportsToAnodizingCheckBtnStateChangedSlot(int state);
	void importsFromNorthCheckBtnStateChangedSlot(int state);
	void importsFromSouthCheckBtnStateChangedSlot(int state);
	void useEpicsCheckBtnStateChangedSlot(int state);
};
#endif







































#ifndef LS_H
#define LS_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QLabel>
#include <QSqlError>
#include <QCheckBox>
#include <QSpinBox>


#include "ppG.h"
#include "pl.h"

class layerOfSpacersClass : public QWidget
{
	layerOfSpacersClass					*nextLayer;
	layerOfSpacersClass					*previousLayer;
	//
	int													basketDBTableId;
	int													layerNumber;
	//
	int													isOnComb;
	//
	QCheckBox										*onCombCheckBox;
	//QSpinBox										*combNumberSpinBox;
	QCheckBox										*sp1CheckBox;
	QCheckBox										*sp2CheckBox;
	QCheckBox										*sp3CheckBox;
	QCheckBox										*sp4CheckBox;
	QCheckBox										*sp5CheckBox;
	QCheckBox										*sp6CheckBox;
	QCheckBox										*sp7CheckBox;
	QCheckBox										*sp8CheckBox;
	//
Q_OBJECT
public:
	layerOfSpacersClass(int layerNumber,bool isInCombs,int word/*,int combNumber*/);
	~layerOfSpacersClass();
	//
	void setNextLayerClass(layerOfSpacersClass* nl);
	void setPreviousLayerClass(layerOfSpacersClass* pl);
	void validate();
	//int getCombNumber();
	//int getMinCombNumber();
	bool getIsOnComb();
	int getSp1();
	int getSp2();
	int getSp3();
	int getSp4();
	int getSp5();
	int getSp6();
	int getSp7();
	int getSp8();
	//
	bool isActive();
public slots:
	//void combNumberSpinBoxSlot(int v);
	void onCombCheckBoxSlot(int v);
	void sp1CheckBoxSlot(int v);
	void sp2CheckBoxSlot(int v);
	void sp3CheckBoxSlot(int v);
	void sp4CheckBoxSlot(int v);
	void sp5CheckBoxSlot(int v);
	void sp6CheckBoxSlot(int v);
	void sp7CheckBoxSlot(int v);
	void sp8CheckBoxSlot(int v);
};
#endif

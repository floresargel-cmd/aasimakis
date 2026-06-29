#ifndef POSDLG_H
#define POSDLG_H

class posDlgClass;
#include "Gb3.h"
#include "bCont.h"
#include "pos.h"
#include "allPos.h"
class posDlgClass : public QDialog				
{												
	posClass										*parrentPos;
	allPosClass								*allPoss;
	//
	QTabWidget									*mainTabWidget;
	QSpinBox										*basketSpinBox;
	QLabel											*basketInfoLabel;
	QCheckBox										*lockedCheckBox;
	basketContentsClass					*basketContents;
	QTableWidget								*basketLogTable;
	QPushButton									*okBtn;
public:										
	Q_OBJECT								
public:										
	posDlgClass(allPosClass *allPoss_,posClass *parrentPos_,QString user,QWidget *p);
	~posDlgClass();
	int myExec(bool checkPassword);
	int getBasketNum();
	void setBasketNum(int b);
	void updateBasketData();
	void accept();
	void reject();
private:										
	int exec();
public slots:								
	void basketSpinBoxValueChangedSlot(int);
};											
#endif

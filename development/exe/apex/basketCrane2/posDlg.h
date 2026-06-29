#ifndef posDlg_h
#define posDlg_h

class posDlgClass;
#include "Gb2.h"
#include "bCont.h"
#include "pos.h"
#include "allPos.h"
class posDlgClass : public QDialog				
{												
	posClass										*parrentPos;
	allPossClass								*allPoss;
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
	posDlgClass(allPossClass *allPoss_,posClass *parrentPos_,QString user,QWidget *p);
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

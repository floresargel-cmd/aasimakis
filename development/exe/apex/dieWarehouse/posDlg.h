#ifndef POSDLG_H
#define POSDLG_H

#include "Gd.h"
#include "pass.h"
class basketContentsClass;
class posClass;
class allPosClass;
class posDlgClass;
#include "pos.h"
#include "bCont.h"
class posDlgClass : public QDialog
{
	int													posId;
	QString											description;
	int													column;
	int													row;
	float												plcXPos;
	float												plcYPos;
	float												plcZPos;
	double											plcXCorrection;
	double											plcYCorrection;
	double											plcZCorrection;
	int													basketNum;
	bool												basketWithContents;
	//
	QTabWidget									*mainTabWidget;
	QSpinBox										*basketSpinBox;
	QLabel											*basketInfoLabel;
	QCheckBox										*lockedCheckBox;
	QDoubleSpinBox							*xCorrectionSpinBox;
	QDoubleSpinBox							*yCorrectionSpinBox;
	QDoubleSpinBox							*zCorrectionSpinBox;
	basketContentsClass					*basketContents;
	QTableWidget								*basketLogTable;
	QPushButton									*okBtn;
	int													type;
public:										
enum{//type
		basketNumberIsEditable=		0x0000001,
		showPositionData=					0x0000002,
		showBasketData=						0x0000004,
	};
	Q_OBJECT								
public:										
	posDlgClass(int posId_,int basketNum_,QString description_,int column_,int row_,float plcXPos_,float plcYPos_,float plcZPos_,int type_,int activeTab,QWidget *p);
	~posDlgClass();
	int myExec(bool checkPassword);
	int getBasketNum();
	void setBasketNum(int b);
	void updateBasketData();
	void lockPositionDataGui(bool l);
	void accept();
	void reject();
private:										
	int exec();
public slots:								
	void basketSpinBoxValueChangedSlot(int);
};											
#endif

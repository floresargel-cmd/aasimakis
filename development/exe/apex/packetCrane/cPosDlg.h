#ifndef CPOSDLG_H
#define CPOSDLG_H

class allConvPositionsClass;
#ifdef stationsExe
#include "Gc.h"
#endif
#include "allConvPos.h"
#include "hmiGuiWidgetsLib.h"
class cPosDlgClass : public QDialog				
{												
	int													posId;
	int													plcIndex;
	QString											description;
	int													basketNum;
	allConvPositionsClass				*allConvPositions;
	//
	QSpinBox										*basketSpinBox=nullptr;
	int													basket;
	QPushButton									*okBtn;
	QLabel											*basketOkLabel;
	QVector<bitClass*>					bitVector;
Q_OBJECT								
public:										
	cPosDlgClass(int posId_,int plcIndex_,int basketNum_,QString posDescription,allConvPositionsClass *p,bool showBasketSpinBox=true);
	~cPosDlgClass();
	int exec();
	int getBasketNum();
	void accept();
	void reject();
	void setBasketNum(int b);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:
	void basketSpinBoxChangedSlot(int b);
};											
#endif

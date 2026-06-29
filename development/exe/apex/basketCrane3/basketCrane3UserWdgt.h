#ifndef basketCrane3UserWdgt_h
#define basketCrane3UserWdgt_h

class basketCrane3UserWdgtClass;
#include "Gb3.h"
#include "conPos.h"

class basketCrane3UserWdgtClass:public QWidget				
{
	convPositionClass							*conveyor1Pos=nullptr;

	QPushButton										*setDestinationNoneBtn=nullptr;
	QPushButton										*setDestinationCrane2Btn=nullptr;
	QPushButton										*setDestinationCrane3Btn=nullptr;
	QPushButton										*setDestinationAnodizingBtn=nullptr;
	Q_OBJECT
public:										
	basketCrane3UserWdgtClass(convPositionClass *conveyor1Pos_,QWidget *p=nullptr);
	void updateGui();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:								
	void mainTimerTriggeredSlot();
	
	void setDestinationNoneBtnClickedSlot();
	void setDestinationCrane2BtnClickedSlot();
	void setDestinationCrane3BtnClickedSlot();
	void setDestinationAnodizingBtnClickedSlot();
};
#endif

#ifndef posDlgPackingFork_h
#define posDlgPackingFork_h

class posDlgfrorkPackingClass;
#include "Gf.h"
#include "posPackingFork.h"
#include "allPosPackingFork.h"
class posDlgPackingForkClass:public QDialog				
{												
	bool												isForPacketId=false;
	bool												isForRackId=false;
	//
	QSpinBox										*packetSpinBox=nullptr;
	QSpinBox										*lengthSpinBox=nullptr;
	QSpinBox										*widthSpinBox=nullptr;
	QSpinBox										*heightSpinBox=nullptr;
	QSpinBox										*woodsSpinBox=nullptr;
	QSpinBox										*weightSpinBox=nullptr;
	QLabel											*packetInfoLabel=nullptr;
	QPushButton									*okBtn;
public:										
	Q_OBJECT								
public:										
	posDlgPackingForkClass(QString title,int ticketId,bool isForPacketId_,bool isForRackId_,QWidget *p);
	~posDlgPackingForkClass();
	void setData(int packetId,int packetLength,int packetWoods,int packetWidth,int packetHeight,int packetWeigt);
	void updateFromDatabase();
	int myExec(bool checkPassword);
	void accept();
	void reject();
	int getPacketTicket();
	int getPacketLength();
	int getPacketWoods();
	int getPacketWidth();
	int getPacketHeight();
	int exec();
public slots:								
	void packetSpinBoxValueChangedSlot(int packet);
	void lengthSpinBoxValueChangedSlot(int length);
	void widthSpinBoxValueChangedSlot(int width);
	void heightSpinBoxValueChangedSlot(int height);
	void woodsSpinBoxValueChangedSlot(int woods);
	void weightSpinBoxValueChangedSlot(int weight);
};											
#endif

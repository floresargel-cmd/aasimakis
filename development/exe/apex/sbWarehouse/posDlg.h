#ifndef POSDLG_H
#define POSDLG_H

class posDlgClass;
#include "Gsw.h"
#include "pos.h"
#include "bCont.h"
class posDlgClass : public QDialog
{
	int													posId;
	bool												isSbPos;
	bool												isLocked;
	QString											description;
	int													column;
	int													row;
	float												plcXPos;
	float												plcYPos;
	float												plcZPos;
	double											plcXCorrection;
	double											plcYCorrection;
	double											plcZCorrection;
	int													cassetteNum;
	int													sbNumber;
	int													ticket;
	//
	QSpinBox										*cassetteSpinBox;
	QLabel											*cassetteInfoLabel;
	QSpinBox										*sbSpinBox;
	QLabel											*sbInfoLabel;
	QSpinBox										*ticketSpinBox;
	QLabel											*ticketInfoLabel;

	QRadioButton								*destInRadio;
	QRadioButton								*destOutRadio;
	QCheckBox										*lockedCheckBox;
	QDoubleSpinBox							*xCorrectionSpinBox;
	QDoubleSpinBox							*yCorrectionSpinBox;
	QDoubleSpinBox							*zCorrectionSpinBox;
	QPushButton									*okBtn;
	QSerialPort									*serialPort;
public:
	Q_OBJECT
public:
	posDlgClass(int posId_,bool isSbPos_,bool isLocked_,int cassetteNum_,int sbNumber_,int ticket_,QString description_,int column_,int row_,float plcXPos_,float plcYPos_,float plcZPos_,QWidget *p);
	~posDlgClass();
	int myExec(bool checkPassword);
	int getCassetteNum();
	int getSbNumber();
	int getTicket();
	bool getIsLockedChecked();
	void updateGui();
	void accept();
	void logImportExportsTable();
	void reject();
	void setDestinationWarehouse(bool d);
	bool getIsDestinationWarehouse();
private:
	int exec();
public slots:
	void cassetteSpinBoxValueChangedSlot(int value);
	void sbSpinBoxValueChangedSlot(int value);
	void ticketSpinBoxValueChangedSlot(int value);
	void destInRadioToggledSlot(bool value);
	void destOutRadioToggledSlot(bool value);
	void readyReadSerialPortSlot();
};
#endif

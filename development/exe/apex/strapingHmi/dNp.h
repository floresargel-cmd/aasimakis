#ifndef DNP_H
#define DNP_H

#include <QtGui>
#include <QMainWindow>
#include <QRadioButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QGroupBox>
#include <QPrinter>
#include <QPrinterInfo>

#include "shG.h"

class defineNewPacketClass : public QDialog	
{
	QSpinBox											*packetCodeSpinBox;
	QSpinBox											*packetLengthSpinBox;
	QSpinBox											*packetWidthSpinBox;
	QSpinBox											*packetHeightSpinBox;
	QSpinBox											*packetWeightSpinBox;
	QCheckBox											*packetToWareHouseCheckBox;

	QRadioButton									*woods2RadioButton;
	QRadioButton									*woods3RadioButton;
	QRadioButton									*woods4RadioButton;
	QRadioButton									*woods5RadioButton;
	QRadioButton									*woods6RadioButton;
	QRadioButton									*woods7RadioButton;
#ifdef USE_STORAGE_TYPE
	QRadioButton									*shortTermStorageRadioButton;
	QRadioButton									*midTermStorageRadioButton;
	QRadioButton									*longTermStorageRadioButton;
#endif
	QPushButton										*okPushbutton;
	//
	int														stationEnum;
	int														emptyCassettesInWarehouse;
public:										
	enum{
		TYPE_SB_STATION=1,
		TYPE_PACKET_STATION,
		TYPE_IMPORT_PACKET_STATION,
		TYPE_IMPORT_SB_STATION,
	};
Q_OBJECT
public:										
	defineNewPacketClass(int stationEnum_,int length,int width,int height,int weight,int noOfWoods,QWidget *p);
	void checkToWareHouse();
	void messageFromBarcode(QString msg);
	void checkEnableOkButton();
	void printNumber(int c);
public slots:
	void packetCodeSpinBoxSlot(int value);
	void packetLengthSpinBoxSlot(int value);
	void packetWidthSpinBoxSlot(int value);
	void packetHeightSpinBoxSlot(int value);
	void packetToWareHouseCheckBoxSlot();

	void woods2RadioButtonSlot();
	void woods3RadioButtonSlot();
	void woods4RadioButtonSlot();
	void woods5RadioButtonSlot();
	void woods6RadioButtonSlot();
	void woods7RadioButtonSlot();

	void shortTermStorageRadioButtonSlot();
	void midTermStorageRadioButtonSlot();
	void longTermStorageRadioButtonSlot();

	void okPushbuttonSlot();
};																	
#endif
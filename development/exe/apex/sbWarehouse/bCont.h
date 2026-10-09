#ifndef BCONT_H
#define BCONT_H
#include "Gsw.h"
class cassetteContentsClass:public QWidget
{
	int											cassetteNum;
	QSpinBox								*packetSpin;
	QLabel									*packetPositionLabel;
	QPushButton							*saveBtn;
	bool										canChangeContents;
public:
	Q_OBJECT
public:
	cassetteContentsClass(bool canChangeContents_,QWidget *w);
	~cassetteContentsClass();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void updateFromDatabase(int cassetteNum_);
	void save();
	bool checkData();
	void setPacketSpinBoxValue(int v);
public slots:
	void packetSpinBoxValueChangedSlot(int v);
	void saveBtnClickedSlot();
};
#endif

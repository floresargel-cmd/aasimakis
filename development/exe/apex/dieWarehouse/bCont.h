#ifndef BCONT_H
#define BCONT_H

#include "Gd.h"

struct scheduleStruct
{
	QString timestamp;
	bool newDayShift;
};

class basketContentsClass:public QWidget
{
	int											basketNum;
	QTableWidget						*tableWdgt;
	QSignalMapper						*contentComboMapper;
	QSignalMapper						*contentLineEditMapper;
	QPushButton							*saveBtn;
	QLabel									*weightLabel;
	QLabel									*widthLabel;
public:
	Q_OBJECT
public:
	basketContentsClass(QWidget *w);
	~basketContentsClass();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void updateData(int basketNum_);
	void save();
private:
	void updateData();
	void fillTableRow(int row,int tId,QString contentName,QString contentType);
	bool checkRows();
	scheduleStruct getProductionShiftOfDie(QString die);
signals:
	void stateChangedSignal(bool isOk);
public slots:
	void saveBtnClickedSlot();
	void contentComboMapperSlot(int);
	void contentLineEditMapperSlot(int);
};
#endif

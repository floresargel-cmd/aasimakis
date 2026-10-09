#ifndef EXPW_H
#define EXPW_H

#include "pcG.h"
#include "../common/apexG.h"
#include "../common/btns.h"

class exportsWidgetClass : public QWidget
{
	QString					table;
	QTableWidget		*contentsTableWidget;
	QTableWidget		*exportsTableWidget;
	QPushButton			*addToExportsTableBtn;
	QPushButton			*removeFromExportsTableBtn;
	QLineEdit				*filterLineEdit;

Q_OBJECT
public:
	exportsWidgetClass(QString table_,QWidget *p=NULL);
	void updateGui();
signals:
public slots:
	void addToExportsTableBtnSlot();
	void removeFromExportsTableSlot();
	void contentsTableWidgetItemSelectionChangedSlot();
	void exportsTableWidgetItemSelectionChangedSlot();
	void filterLineEditTextChangedSlot(QString f);
};
#endif

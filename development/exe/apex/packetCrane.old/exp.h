#ifndef EXP_H
#define EXP_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "pcG.h"
#include "../common/apexG.h"
#include "../common/btns.h"

class exportsClass : public QDialog	
{
	QTableWidget		*contentsTableWidget;
	QTableWidget		*exportsTableWidget;
	QPushButton			*addToExportsTableBtn;
	QPushButton			*removeFromExportsTableBtn;

Q_OBJECT
public:		
  exportsClass(QWidget *p=NULL);
	void updateGui();
	void show();
signals:
public slots:
	void addToExportsTableBtnSlot();
	void removeFromExportsTableSlot();
	void contentsTableWidgetItemSelectionChangedSlot();
	void exportsTableWidgetItemSelectionChangedSlot();
};																	
#endif
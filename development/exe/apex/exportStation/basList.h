#ifndef BASLIST_H
#define BASLIST_H

#include <QtGui>
#include <QMainWindow>
#include <QTableWidget>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "eG.h"
class basketListClass;
#include "selBas.h"
class basketListClass:public QWidget
{
	int										selectedBasketNumber;
	//
	QTableWidget					*tableView;
	QListWidget						*listView;
	//
	QToolButton 					*basketUpButton;
	QToolButton 					*basketDownButton;
	QToolButton 					*basketRemoveButton;
	QToolButton 					*basketAddButton;
public:
	int										stationTypeEnum;
	Q_OBJECT
public:
	basketListClass(int stationTypeEnum_,QWidget *w);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void lockGui();
	void unLockGui();
	void updateGui();
	void updateList(bool forceUpdate=false);
	void updateTable();
public slots:
	void listSelectionChanged(QListWidgetItem* selected,QListWidgetItem* deselected);
	//
	void basketUpSlot();
	void basketDownSlot();
	void basketRemoveSlot();
	void basketAddSlot();
};
#endif

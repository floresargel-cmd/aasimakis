#ifndef SELBAS_H
#define SELBAS_H
class cassetteListClass;
#include <QtGui>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QSplitter>
#include <QSqlRecord>
#include <QToolButton>
#include <QHeaderView>
#include <QCompleter>
#include <QList.h>
#include "basList.h"
class selectBasketClass:public QDialog
{
	int								stationTypeEnum;
	int								selectedBasketNumber;
	//
	QListWidget				*listView;
	QLineEdit					*profileFilterLineEdit;
	QLineEdit					*orderFilterLineEdit;
	QTableWidget			*tableView;
	//
	basketListClass		*parentList;
	//
	QToolButton 			*cancelButton;
	QToolButton 			*addSelectedButton;
public:
	Q_OBJECT
public:
	selectBasketClass(int stationTypeEnums_,basketListClass *parentList_,QWidget *w);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void updateList(QString profileFilter,QString orderFilter);
	void updateTable();
public slots:
	void listSelectionChanged(QListWidgetItem* selected,QListWidgetItem* deselected);
	//
	void profileFilterTextChangedSlot(const QString&);
	void orderFilterTextChangedSlot(const QString&);
	void addSelectedSlot();
};
#endif

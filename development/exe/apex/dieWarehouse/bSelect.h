#ifndef BSELECT_H
#define BSELECT_H
#include "Gd.h"
#include "expWdg.h"
#include "bCont.h"
class basketSelectClass:public QDialog
{
	QString											title;
	QString											dbTable;
	QListWidget									*listView;
	QLineEdit										*profileFilter;
	exportStationWidgetClass		*exportStationW;
	QToolButton 								*cancelButton;
	QToolButton 								*addSelectedButton;
	QStringListModel						*filterCompleterModel;
	//
	basketContentsClass					*basketContents;
	int													selectedBasketNum;
public:
	Q_OBJECT
public:
	basketSelectClass(QString title_,QString dbTable_,exportStationWidgetClass *exportStationW_);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void updateList(QString str);
	void updateTable();
	void addBasket(int b);
	int exec();
public slots:
	void listSelectionChanged(QListWidgetItem* selected,QListWidgetItem* deselected);
	void filterTextChangedSlot(const QString&);
	void addSelectedSlot();
};
#endif

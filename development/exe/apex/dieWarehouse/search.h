#ifndef SEARCH_H
#define SEARCH_H
#include "Gd.h"
#include "expWdg.h"
#include "bCont.h"
#include "allPos.h"
class searchWidgetClass:public QWidget
{
	QListWidget									*listView;
	QLineEdit										*filterLineEdit;
	allPossClass								*allPoss;
	int													selectedBasket;
public:
	Q_OBJECT
public:
	searchWidgetClass(allPossClass *allPoss_,QWidget *p);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void updateList(QString str);
	QString tooltTipForBasket(int basket);
public slots:
	void filterTextChangedSlot(const QString&);
	void listSelectionChangedSlot(QListWidgetItem* selected,QListWidgetItem* deselected);
};
#endif

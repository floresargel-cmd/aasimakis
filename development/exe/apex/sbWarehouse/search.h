#ifndef SEARCH_H
#define SEARCH_H
class cassetteListClass;
#include "Gsw.h"
#include "expWdg.h"
#include "bCont.h"
#include "allPos.h"
class searchWidgetClass:public QWidget
{
	QListWidget									*listView;
	QLineEdit										*filterLineEdit;
	allPossClass								*allPoss;
	int													selectedCassette;
public:
	Q_OBJECT
public:
	searchWidgetClass(allPossClass *allPoss_,QWidget *p);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void updateList(QString str);
	QString tooltTipForCassette(int cassette);
signals:
	void posSelectedSignal(int pos);
public slots:
	void filterTextChangedSlot(const QString&);
	void listSelectionChangedSlot(QListWidgetItem* selected,QListWidgetItem* deselected);
};
#endif

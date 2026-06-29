#ifndef BSELECTS_H
#define BSELECTS_H
#include "Gd.h"
#include "expWdg.h"
#include "bCont.h"
class basketSelectForScheduleClass:public QDialog
{
	QListWidget									*shiftList;
	QListWidget									*diesList;
	QToolButton 								*addCassettesButton;
	QToolButton 								*addAllCassettesButton;
	QString											selectedShift;
public:
	Q_OBJECT
public:
	basketSelectForScheduleClass(QWidget *p);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int exec();
	void updateShiftList();
	void updateDiesList();
	void addCassettesOfListWidgetItems(QList<QListWidgetItem*> items);
public slots:
	void shiftListSelectionChanged(QListWidgetItem* selected,QListWidgetItem* deselected);
	void addCassettesButtonSlot();
	void addAllCassettesButtonSlot();
};
#endif

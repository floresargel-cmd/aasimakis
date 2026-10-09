#ifndef BSELECT_H
#define BSELECT_H
#include "../sbWarehouse/Gsw.h"
#include "expWdg.h"
class cassetteSelectClass:public QDialog
{
	QString											title;
	QString											dbTable;
	QListWidget									*listView;
	QRadioButton								*cassetteRadioBtn;
	QRadioButton								*sbRadioBtn;
	exportStationWidgetClass		*exportStationW;
	QToolButton 								*addSelectedButton;
	QLineEdit										*profileFilter;
	int													selectedCassetteNum;
enum{
	roleCassetteNumber=Qt::UserRole,
};
public:
	Q_OBJECT
public:
	cassetteSelectClass(QString title_,QString dbTable_,exportStationWidgetClass *exportStationW_);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void updateList();
	void addCassette(int b);
	int exec();
public slots:
	void updateListSlot();
	void listSelectionChanged(QListWidgetItem* selected,QListWidgetItem* deselected);
	void filterTextChangedSlot(const QString&);
	void addSelectedSlot();
};
#endif

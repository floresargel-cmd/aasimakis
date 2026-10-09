#ifndef EXPWDG_H
#define EXPWDG_H

class exportStationWidgetClass;
#include "Gb3.h"
#include "bSelect.h"
class exportStationWidgetClass:public QWidget
{
	QString								dbTable;
	QString								title;
	basketSelectClass		  *basketSelect;
	QListWidget						*exportsList;
	QToolButton						*basketUpBtn;//move up a selected item of the list, to increase its priority
	QToolButton						*basketDownBtn;//move down a selected item of the list, to decrease its priority
	QToolButton						*basketAddBtn;//add selected basket item to exports list
	QToolButton						*basketRemoveBtn;//remove selected basket item from exports list
	int										selectedBasket;
	QToolBox							*parrentToolBox=nullptr;
	int										parrentToolBoxIndex=0;
	Q_OBJECT
public:
	exportStationWidgetClass(QString dbTable_,QString title_,QWidget *p=NULL);
	void updateFromDatabase();
public slots:
	void setToolBoxClass(QToolBox *parrentToolBox_,int parrentToolBoxIndex_);
	void dbUpdateTimerSlot();
	void exportsListSelectionChangedSlot(QListWidgetItem* selected,QListWidgetItem* deselected);
	void basketUpBtnClickedSlot();
	void basketDownBtnClickedSlot();
	void basketAddBtnClickedSlot();
	void basketRemoveBtnClickedSlot();
	void resizeEvent(QResizeEvent *e);
signals:
	void listChangedSignal();
};
#endif

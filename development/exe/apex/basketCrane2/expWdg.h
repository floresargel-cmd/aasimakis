#ifndef EXPWDG_H
#define EXPWDG_H

class exportStationWidgetClass;
#include "Gb2.h"
class exportStationWidgetClass:public QWidget
{
	QString								dbTable;
	QString								title;
	QListWidget						*exportsList;
	QToolButton						*basketUpBtn;
	QToolButton						*basketDownBtn;
	QToolButton						*basketRemoveBtn;
	int										selectedBasket;
	QToolBox							*parrentToolBox=nullptr;
	int										parrentToolBoxIndex=0;
	Q_OBJECT								
public:										
	exportStationWidgetClass(QString dbTable_,QString title_,QWidget *p=NULL);
	~exportStationWidgetClass();
	void setToolBoxClass(QToolBox *parrentToolBox_,int parrentToolBoxIndex_);
	void updateFromDatabase();
public slots:
	void dbUpdateTimerSlot();
	void exportsListSelectionChangedSlot(QListWidgetItem* selected,QListWidgetItem* deselected);
	void basketUpSlot();
	void basketDownSlot();
	void basketRemoveSlot();
	void resizeEvent(QResizeEvent *e);
signals:
	void listChangedSignal();
};											
#endif

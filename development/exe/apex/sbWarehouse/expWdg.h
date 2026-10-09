#ifndef EXPWDG_H
#define EXPWDG_H
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
class cassetteSelectClass;
class exportStationWidgetClass;
#include "bSelect.h"
////////////////////////////////////////////////////////////////////////////
class myListWidget : public QListWidget
{
public:
	Q_OBJECT
public:
enum{
	roleId=Qt::UserRole,
	roleCassette,
};
public:
	myListWidget();
	Qt::DropAction supportedDropActions();
	void startDrag(Qt::DropActions supportedActions);
	void dragEnterEvent(QDragEnterEvent* ev);
	void dropEvent(QDropEvent *ev);
	void dragMoveEvent(QDragMoveEvent *ev);
signals:
	void itemsRemovedSignal(QVector<int> itemRemovedTIds);
	void itemDropedOnSignal(int itemDroppedOnTId,QString cassettes);
};
////////////////////////////////////////////////////////////////////////////
class exportStationWidgetClass : public QWidget
{
	QString								dbTable;
	QString								title;
	myListWidget					*exportsList;
	QToolButton						*cassetteUpBtn;
	QToolButton						*cassetteDownBtn;
	QToolButton						*cassetteAddBtn;
	QToolButton						*cassetteRemoveBtn;
	cassetteSelectClass		*cassetteSelect;
	QToolBox							*parrentToolBox;
	int										parrentToolBoxIndex;
	QLabel								*displayLabel;
	bool									canUserChangeList;
	//bool									hasChecks;
	Q_OBJECT
public:	exportStationWidgetClass(QString dbTable_,QString title_,QToolBox *parrentToolBox_,int parrentToolBoxIndex_,QLabel *displayLabel_,bool canUserChangeList_,QWidget *p);
	~exportStationWidgetClass();
	void updateFromDatabase();
	void updateGui();
//	void setHasChecks(bool h);
public slots:
	void dbUpdateTimerSlot();
	void exportsListSelectionChangedSlot();
	void cassetteUpSlot();
	void cassetteDownSlot();
	void cassetteAddSlot();
	void cassetteRemoveSlot();
	void resizeEvent(QResizeEvent *e);
	void itemRemovedSlot(QVector<int> itemRemovedTIds);
	void itemDropedOnSlot(int itemDroppedOnTId,QString cassettes);
signals:
	void listChangedSignal();
};
#endif

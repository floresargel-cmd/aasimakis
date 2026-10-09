#ifndef EXPWDG_H
#define EXPWDG_H

#include "Gd.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
class basketSelectClass;
class exportStationWidgetClass;
class basketSelectForScheduleClass;
#include "bSelect.h"
#include "bSelectS.h"
class exportStationWidgetClass : public QWidget
{
	int														plcId;
	int														myBarcodeReaderMode;
	QString												dbTable;
	QString												title;
	QListWidget										*exportsList;
	QToolButton										*basketUpBtn;
	QToolButton										*basketDownBtn;
	QToolButton										*basketAddBtn;
	QToolButton										*addDiesFromEpicsBtn;
	QToolButton										*basketRemoveBtn;
	basketSelectClass							*basketSelect;
	int														selectedBasket;
	basketSelectForScheduleClass	*basketSelectForSchedule;
	Q_OBJECT
public:
	exportStationWidgetClass(QString dbTable_,QString title_,int plcId_=0,QWidget *p=NULL);
	~exportStationWidgetClass();
	void updateFromDatabase();
public slots:
	void dbUpdateTimerSlot();
	void exportsListSelectionChangedSlot(QListWidgetItem* selected,QListWidgetItem* deselected);
	void basketUpSlot();
	void basketDownSlot();
	void basketAddSlot();
	void basketRemoveSlot();
	void addDiesFromEpicsBtnSlot();
	void	resizeEvent(QResizeEvent *e);
signals:
	void listChangedSignal();
	void titleChangedSignal(QString t);
public slots:
	void newBarcodeReadSlot(int barcodeReaderMode,int basket,QString profile,int quandity,float length,int cardNo,QString color,int soNo,int soLine);
};
#endif

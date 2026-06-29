#ifndef EXP_H
#define EXP_H

#include "Gd.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "expWdg.h"
class exportStationClass : public QWidget				
{
	QString												dbTable;
	QString												title;
	exportStationWidgetClass			*exportStationWidget;
	QToolBox											*parrentToolBox;
	int														parrentToolBoxIndex;
	Q_OBJECT								
public:										
	exportStationClass(QString dbTable_,QString title_,int plcId,QWidget *p=NULL);
	~exportStationClass();
	void setToolBox(QToolBox *parrentToolBox_,int parrentToolBoxIndex_);
	//
	exportStationWidgetClass* getExportStationWidget();
public slots:								
	void titleChangedSlot(QString t);						
};											
#endif

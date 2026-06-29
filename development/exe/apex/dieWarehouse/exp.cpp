#include "pch.h"
#include "exp.h"
exportStationClass::exportStationClass(QString dbTable_,QString title_,int plcId,QWidget *p):QWidget(p)
{
	setVisible(false);
	dbTable=dbTable_;
	title=title_;
	setWindowTitle(title);
	parrentToolBox=NULL;
	parrentToolBoxIndex=-1;
	//
	exportStationWidget=new exportStationWidgetClass(dbTable,title,plcId,this);
	connect(exportStationWidget,SIGNAL(titleChangedSignal(QString)),this,SLOT(titleChangedSlot(QString)));
}
exportStationClass::~exportStationClass()
{
}
void exportStationClass::setToolBox(QToolBox *parrentToolBox_,int parrentToolBoxIndex_)
{
	parrentToolBox=parrentToolBox_;
	parrentToolBoxIndex=parrentToolBoxIndex_;
}
void exportStationClass::titleChangedSlot(QString t)
{
	if (parrentToolBox)
		parrentToolBox->setItemText(parrentToolBoxIndex,t);
}
exportStationWidgetClass* exportStationClass::getExportStationWidget()
{
	return exportStationWidget;
}



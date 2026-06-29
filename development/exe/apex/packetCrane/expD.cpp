#include "pch.h"
#include "expD.h"
exportsDlgClass::exportsDlgClass(QWidget *p):QDialog(p)
{
	resize(700,1000);
	QVBoxLayout *mainLayout=new QVBoxLayout();this->setLayout(mainLayout);
	QTabWidget *mainTabWidget=new QTabWidget();mainLayout->addWidget(mainTabWidget);
	exports=new exportsWidgetClass("exports",this);									mainTabWidget->addTab(exports,tr("Exports"));
	exportsStation1=new exportsWidgetClass("exportsStation1",this);	mainTabWidget->addTab(exportsStation1,tr("Exports station 1"));
	exportsStation2=new exportsWidgetClass("exportsStation2",this);	mainTabWidget->addTab(exportsStation2,tr("Exports station 2"));
	exportsPacking=new exportsWidgetClass("exportsPacking",this);		mainTabWidget->addTab(exportsPacking,tr("Exports Packing"));
	connect(mainTabWidget,SIGNAL(currentChanged(int)),this,SLOT(mainTabWidgetCurrentChangedSlot(int)));
}
int exportsDlgClass::exec()
{
	updateGui();
	return QDialog::exec();
}
void exportsDlgClass::updateGui()
{
	exports->updateGui();
	exportsStation1->updateGui();
	exportsStation2->updateGui();
}
void exportsDlgClass::mainTabWidgetCurrentChangedSlot(int index)
{
	if (index==0)
		exports->updateGui();
	if (index==1)
		exportsStation1->updateGui();
	if (index==2)
		exportsStation2->updateGui();
	if (index==3)
		exportsPacking->updateGui();
}
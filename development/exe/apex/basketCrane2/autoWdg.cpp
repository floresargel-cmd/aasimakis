#include "pch.h"
#include "autoWdg.h"
autoWidgetClass::autoWidgetClass(craneClass *crane_,QWidget *p):QWidget(p)
{
	crane=crane_;
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);mainLayout->setMargin(1);

	exportsToDestackerCheckBtn=new QCheckBox(tr("Exports to Destaker B"),this);
	mainLayout->addWidget(exportsToDestackerCheckBtn);
	connect(exportsToDestackerCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(exportsToDestackerCheckBtnStateChangedSlot(int)));
	exportsToDestackerCheckBtn->setEnabled(true);

	exportsToPackingDestacker1CheckBtn=new QCheckBox(tr("Exports to Packing Destaker 1"),this);
	mainLayout->addWidget(exportsToPackingDestacker1CheckBtn);
	connect(exportsToPackingDestacker1CheckBtn,SIGNAL(stateChanged(int)),this,SLOT(exportsToPackingDestacker1CheckBtnStateChangedSlot(int)));
	exportsToPackingDestacker1CheckBtn->setEnabled(true);

	exportsToPackingDestacker2CheckBtn=new QCheckBox(tr("Exports to Packing Destaker 2"),this);
	mainLayout->addWidget(exportsToPackingDestacker2CheckBtn);
	connect(exportsToPackingDestacker2CheckBtn,SIGNAL(stateChanged(int)),this,SLOT(exportsToPackingDestacker2CheckBtnStateChangedSlot(int)));
	exportsToPackingDestacker2CheckBtn->setEnabled(true);

	exportsToAnodizingCheckBtn=new QCheckBox(tr("Exports to Anodizing"),this);
	mainLayout->addWidget(exportsToAnodizingCheckBtn);
	connect(exportsToAnodizingCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(exportsToAnodizingCheckBtnStateChangedSlot(int)));
	exportsToAnodizingCheckBtn->setEnabled(true);

	importsFromNorthCheckBtn=new QCheckBox(tr("Imports from North"),this);
	mainLayout->addWidget(importsFromNorthCheckBtn);
	connect(importsFromNorthCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(importsFromNorthCheckBtnStateChangedSlot(int)));
	importsFromNorthCheckBtn->setEnabled(true);

	importsFromSouthCheckBtn=new QCheckBox(tr("Imports from South"),this);
	mainLayout->addWidget(importsFromSouthCheckBtn);
	connect(importsFromSouthCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(importsFromSouthCheckBtnStateChangedSlot(int)));
	importsFromSouthCheckBtn->setEnabled(true);

	useEpicsCheckBtn=new QCheckBox(tr("Epics"),this);
	mainLayout->addWidget(useEpicsCheckBtn);
	connect(useEpicsCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(useEpicsCheckBtnStateChangedSlot(int)));
	useEpicsCheckBtn->setEnabled(true);
	//QWidget *autoExportsWdgt=new QWidget();mainLayout->addWidget(autoExportsWdgt);
	//QHBoxLayout *autoExportsWdgtL=new QHBoxLayout();autoExportsWdgt->setLayout(autoExportsWdgtL);autoExportsWdgtL->setMargin(0);

	//autoExportsCheckBtn=new QCheckBox(tr("Auto exports"),this);
	//autoExportsWdgtL->addWidget(autoExportsCheckBtn);
	//connect(autoExportsCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(autoExportsCheckBtnStateChangedSlot(int)));
	//autoExportsCheckBtn->setEnabled(true);
	//QPushButton *calculateExportGroupsBtn=new QPushButton(QIcon(":/reset.png"),tr(""));
	//autoExportsWdgtL->addWidget(calculateExportGroupsBtn);
	//connect(calculateExportGroupsBtn,SIGNAL(clicked()),this,SLOT(calculateExportGroupsBtnClickedSlot()));
	//autoExportsWdgtL->addStretch(1);
	mainLayout->addStretch(1);
}
autoWidgetClass::~autoWidgetClass()
{
}
void autoWidgetClass::exportsToDestackerCheckBtnStateChangedSlot(int state)
{
	if (exportsToDestackerCheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoExportsToDestaker);
	else
		crane->removeFromAutoMode(craneClass::autoExportsToDestaker);
}
void autoWidgetClass::exportsToPackingDestacker1CheckBtnStateChangedSlot(int state)
{
	if (exportsToPackingDestacker1CheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoExportsToPackingDestaker1);
	else
		crane->removeFromAutoMode(craneClass::autoExportsToPackingDestaker1);
}
void autoWidgetClass::exportsToPackingDestacker2CheckBtnStateChangedSlot(int state)
{
	if (exportsToPackingDestacker2CheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoExportsToPackingDestaker2);
	else
		crane->removeFromAutoMode(craneClass::autoExportsToPackingDestaker2);
}
void autoWidgetClass::exportsToAnodizingCheckBtnStateChangedSlot(int state)
{
	if (exportsToAnodizingCheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoExportsToAnodizing);
	else
		crane->removeFromAutoMode(craneClass::autoExportsToAnodizing);
}
void autoWidgetClass::importsFromNorthCheckBtnStateChangedSlot(int state)
{
	if (importsFromNorthCheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoImportsFromNorth);
	else
		crane->removeFromAutoMode(craneClass::autoImportsFromNorth);
}
void autoWidgetClass::importsFromSouthCheckBtnStateChangedSlot(int state)
{
	if (importsFromSouthCheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoImportsFromSouth);
	else
		crane->removeFromAutoMode(craneClass::autoImportsFromSouth);
}
void autoWidgetClass::useEpicsCheckBtnStateChangedSlot(int state)
{
	if (useEpicsCheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoExportsUseEpics);
	else
		crane->removeFromAutoMode(craneClass::autoExportsUseEpics);
}

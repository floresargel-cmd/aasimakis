#include "pch.h"
#include "autoWdg.h"
autoWidgetClass::autoWidgetClass(tuxipServerClass *tuxipServer_,craneClass *crane_,QWidget *p):QWidget(p)
{
	tuxipServer=tuxipServer_;
	crane=crane_;
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);mainLayout->setMargin(1);
	//west
	QGroupBox *westG=new QGroupBox("West");QVBoxLayout *westGL=new QVBoxLayout();westG->setLayout(westGL);mainLayout->addWidget(westG);
	autoWestImportsCheckBtn=new QCheckBox(tr("Imports"),this);
	westGL->addWidget(autoWestImportsCheckBtn);
	connect(autoWestImportsCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(autoWestImportsCheckBtnSlot(int)));

	autoWestExportsEmptySbCheckBtn=new QCheckBox(tr("Export empty SB "),this);
	westGL->addWidget(autoWestExportsEmptySbCheckBtn);
	connect(autoWestExportsEmptySbCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(autoWestExportsEmptySbCheckBtnSlot(int)));

	autoWestExportsFullSbCheckBtn=new QCheckBox(tr("Export full SB"),this);
	westGL->addWidget(autoWestExportsFullSbCheckBtn);
	connect(autoWestExportsFullSbCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(autoWestExportsFullSbCheckBtnSlot(int)));
	
	autoWestExportsEmptyCassettesCheckBtn=new QCheckBox(tr("Export empty cassettes"),this);
	westGL->addWidget(autoWestExportsEmptyCassettesCheckBtn);
	connect(autoWestExportsEmptyCassettesCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(autoWestExportsEmptyCassettesCheckBtnSlot(int)));
	//triple
	QGroupBox *tripleG=new QGroupBox(tr("Triple"));QVBoxLayout *tripleGL=new QVBoxLayout();tripleG->setLayout(tripleGL);mainLayout->addWidget(tripleG);
	tripleImportsCheckBtn=new QCheckBox(tr("Imports"),this);
	tripleGL->addWidget(tripleImportsCheckBtn);
	connect(tripleImportsCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(tripleImportsCheckBtnSlot(int)));
	
	tripleExportsWestCheckBtn=new QCheckBox(tr("Export to west"),this);
	tripleGL->addWidget(tripleExportsWestCheckBtn);
	connect(tripleExportsWestCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(tripleExportsWestCheckBtnSlot(int)));
	
	tripleExportsEastCheckBtn=new QCheckBox(tr("Export to east"),this);
	tripleGL->addWidget(tripleExportsEastCheckBtn);
	connect(tripleExportsEastCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(tripleExportsEastCheckBtnSlot(int)));
	//double
	QGroupBox *doubleG=new QGroupBox(tr("Double"));QVBoxLayout *doubleGL=new QVBoxLayout();doubleG->setLayout(doubleGL);mainLayout->addWidget(doubleG);
	doubleImportsCheckBtn=new QCheckBox(tr("Imports"),this);
	doubleGL->addWidget(doubleImportsCheckBtn);
	connect(doubleImportsCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(doubleImportsCheckBtnSlot(int)));
	
	doubleExportEmptyCassettesCheckBtn=new QCheckBox(tr("Export empty cassettes"),this);
	doubleGL->addWidget(doubleExportEmptyCassettesCheckBtn);
	connect(doubleExportEmptyCassettesCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(doubleExportEmptyCassettesCheckBtnSlot(int)));

	doubleExportFullSBCheckBtn=new QCheckBox(tr("Export full SB"),this);
	doubleGL->addWidget(doubleExportFullSBCheckBtn);
	connect(doubleExportFullSBCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(doubleExportFullSBCheckBtnSlot(int)));

	if (false)
	{
		tripleImportsCheckBtn->setEnabled(false);
		tripleExportsWestCheckBtn->setEnabled(false);
		tripleExportsEastCheckBtn->setEnabled(false);

		doubleImportsCheckBtn->setEnabled(false);
		doubleExportEmptyCassettesCheckBtn->setEnabled(false);
		doubleExportFullSBCheckBtn->setEnabled(false);
	}
	mainLayout->addStretch(1);
}
autoWidgetClass::~autoWidgetClass()
{
}
void autoWidgetClass::autoWestImportsCheckBtnSlot(int state)
{
	if (state==Qt::Checked)
		crane->addToAutoMode(craneClass::autoModeEnum::autoWestImports);
	else
		crane->removeFromAutoMode(craneClass::autoModeEnum::autoWestImports);
}
void autoWidgetClass::autoWestExportsEmptySbCheckBtnSlot(int state)
{
	if (state==Qt::Checked)
		crane->addToAutoMode(craneClass::autoModeEnum::autoWestExportsEmptySb);
	else
		crane->removeFromAutoMode(craneClass::autoModeEnum::autoWestExportsEmptySb);
}
void autoWidgetClass::autoWestExportsFullSbCheckBtnSlot(int state)
{
	if (state==Qt::Checked)
		crane->addToAutoMode(craneClass::autoModeEnum::autoWestExportsFullSb);
	else
		crane->removeFromAutoMode(craneClass::autoModeEnum::autoWestExportsFullSb);
}
void autoWidgetClass::autoWestExportsEmptyCassettesCheckBtnSlot(int state)
{
	if (state==Qt::Checked)
		crane->addToAutoMode(craneClass::autoModeEnum::autoWestExportsEmptyCassettes);
	else
		crane->removeFromAutoMode(craneClass::autoModeEnum::autoWestExportsEmptyCassettes);
}
void autoWidgetClass::tripleImportsCheckBtnSlot(int state)
{
	if (state==Qt::Checked)
		crane->addToAutoMode(craneClass::autoModeEnum::tripleImports);
	else
		crane->removeFromAutoMode(craneClass::autoModeEnum::tripleImports);
}
void autoWidgetClass::tripleExportsWestCheckBtnSlot(int state)
{
	if (state==Qt::Checked)
		crane->addToAutoMode(craneClass::autoModeEnum::tripleExportsWest);
	else
		crane->removeFromAutoMode(craneClass::autoModeEnum::tripleExportsWest);
}
void autoWidgetClass::tripleExportsEastCheckBtnSlot(int state)
{
	if (state==Qt::Checked)
		crane->addToAutoMode(craneClass::autoModeEnum::tripleExportsEast);
	else
		crane->removeFromAutoMode(craneClass::autoModeEnum::tripleExportsEast);
}

void autoWidgetClass::doubleImportsCheckBtnSlot(int state)
{
	if (state==Qt::Checked)
		crane->addToAutoMode(craneClass::autoModeEnum::doubleExportImports);
	else
		crane->removeFromAutoMode(craneClass::autoModeEnum::doubleExportImports);
}
void autoWidgetClass::doubleExportEmptyCassettesCheckBtnSlot(int state)
{
	if (state==Qt::Checked)
		crane->addToAutoMode(craneClass::autoModeEnum::doubleExportEmptyCassettes);
	else
		crane->removeFromAutoMode(craneClass::autoModeEnum::doubleExportEmptyCassettes);
}
void autoWidgetClass::doubleExportFullSBCheckBtnSlot(int state)
{
	if (state==Qt::Checked)
		crane->addToAutoMode(craneClass::autoModeEnum::doubleExportFullSB);
	else
		crane->removeFromAutoMode(craneClass::autoModeEnum::doubleExportFullSB);
}
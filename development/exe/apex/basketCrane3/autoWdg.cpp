#include "pch.h"
#include "autoWdg.h"
autoWidgetClass::autoWidgetClass(craneClass *crane_,QWidget *p):crane{crane_},QWidget(p)
{
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	mainLayout->setMargin(1);

	destackerWorking1CheckBtn=new QCheckBox(tr("Destacker Work Position 1"),this);
	mainLayout->addWidget(destackerWorking1CheckBtn);
	connect(destackerWorking1CheckBtn,&QCheckBox::stateChanged,this,&autoWidgetClass::destackerWorking1CheckBtnStateChangedSlot);
	destackerWorking1CheckBtn->setEnabled(true);

	destackerWorking2CheckBtn=new QCheckBox(tr("Destacker Work Position 2"),this);
	mainLayout->addWidget(destackerWorking2CheckBtn);
	connect(destackerWorking2CheckBtn,&QCheckBox::stateChanged,this,&autoWidgetClass::destackerWorking2CheckBtnStateChangedSlot);
	destackerWorking2CheckBtn->setEnabled(true);

	crane2CheckBtn=new QCheckBox(tr("Crane 2"),this);
	mainLayout->addWidget(crane2CheckBtn);
	connect(crane2CheckBtn,&QCheckBox::stateChanged,this,&autoWidgetClass::crane2CheckBtnStateChangedSlot);
	crane2CheckBtn->setEnabled(true);

	returnEmptyCheckBtn=new QCheckBox(tr("Return Empty"),this);
	mainLayout->addWidget(returnEmptyCheckBtn);
	connect(returnEmptyCheckBtn,&QCheckBox::stateChanged,this,&autoWidgetClass::returnEmptyCheckBtnStateChangedSlot);
	returnEmptyCheckBtn->setEnabled(true);

	mainLayout->addStretch(1);
}
void autoWidgetClass::destackerWorking1CheckBtnStateChangedSlot(int state)
{
	if (destackerWorking1CheckBtn->isChecked())
		crane->addToAutoMode(craneClass::toDestackerWorking1Pos);
	else
		crane->removeFromAutoMode(craneClass::toDestackerWorking1Pos);
}
void autoWidgetClass::destackerWorking2CheckBtnStateChangedSlot(int state)
{
	if (destackerWorking2CheckBtn->isChecked())
		crane->addToAutoMode(craneClass::toDestackerWorking2Pos);
	else
		crane->removeFromAutoMode(craneClass::toDestackerWorking2Pos);
}
void autoWidgetClass::crane2CheckBtnStateChangedSlot()
{
	if (crane2CheckBtn->isChecked())
		crane->addToAutoMode(craneClass::toFromCrane2);
	else
		crane->removeFromAutoMode(craneClass::toFromCrane2);
}
void autoWidgetClass::returnEmptyCheckBtnStateChangedSlot()
{
	if (returnEmptyCheckBtn->isChecked())
		crane->addToAutoMode(craneClass::returnEmpty);
	else
		crane->removeFromAutoMode(craneClass::returnEmpty);
}


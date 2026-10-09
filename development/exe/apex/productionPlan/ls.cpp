#include "ls.h"
layerOfSpacersClass::layerOfSpacersClass(int layerNumber,bool isInCombs,int word/*,int combNumber*/):QWidget()
{
	setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	nextLayer=previousLayer=NULL;
	//
	QVector<bool> wordVector=deprecatedCalculateBits(word);
	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);
	mainLayout->addWidget(new QLabel(tr("%1").arg(layerNumber+1)));
	//onCombCheckBox
	onCombCheckBox=new QCheckBox(tr("On comb"));mainLayout->addWidget(onCombCheckBox);
	onCombCheckBox->setChecked(isInCombs);
	connect(onCombCheckBox,SIGNAL(stateChanged(int)),this,SLOT(onCombCheckBoxSlot(int)));
	//combNumberSpinBox
	//combNumberSpinBox=new QSpinBox();mainLayout->addWidget(combNumberSpinBox);
	//connect(combNumberSpinBox,SIGNAL(valueChanged(int)),this,SLOT(combNumberSpinBoxSlot(int)));
	//combNumberSpinBox->blockSignals(true);
	//combNumberSpinBox->setMinimum(1);
	//combNumberSpinBox->setMaximum(MAX_COMBS);
	//combNumberSpinBox->setValue(combNumber);
	//combNumberSpinBox->blockSignals(false);
	//sp
	sp1CheckBox=new QCheckBox(tr("1"));if(wordVector[0])sp1CheckBox->setChecked(true);mainLayout->addWidget(sp1CheckBox);connect(sp1CheckBox,SIGNAL(stateChanged(int)),this,SLOT(sp1CheckBoxSlot(int)));
	sp2CheckBox=new QCheckBox(tr("2"));if(wordVector[1])sp2CheckBox->setChecked(true);mainLayout->addWidget(sp2CheckBox);connect(sp2CheckBox,SIGNAL(stateChanged(int)),this,SLOT(sp2CheckBoxSlot(int)));
	sp3CheckBox=new QCheckBox(tr("3"));if(wordVector[2])sp3CheckBox->setChecked(true);mainLayout->addWidget(sp3CheckBox);connect(sp3CheckBox,SIGNAL(stateChanged(int)),this,SLOT(sp3CheckBoxSlot(int)));
	sp4CheckBox=new QCheckBox(tr("4"));if(wordVector[3])sp4CheckBox->setChecked(true);mainLayout->addWidget(sp4CheckBox);connect(sp4CheckBox,SIGNAL(stateChanged(int)),this,SLOT(sp4CheckBoxSlot(int)));
	sp5CheckBox=new QCheckBox(tr("5"));if(wordVector[4])sp5CheckBox->setChecked(true);mainLayout->addWidget(sp5CheckBox);connect(sp5CheckBox,SIGNAL(stateChanged(int)),this,SLOT(sp5CheckBoxSlot(int)));
	sp6CheckBox=new QCheckBox(tr("6"));if(wordVector[5])sp6CheckBox->setChecked(true);mainLayout->addWidget(sp6CheckBox);connect(sp6CheckBox,SIGNAL(stateChanged(int)),this,SLOT(sp6CheckBoxSlot(int)));
	sp7CheckBox=new QCheckBox(tr("7"));if(wordVector[6])sp7CheckBox->setChecked(true);mainLayout->addWidget(sp7CheckBox);connect(sp7CheckBox,SIGNAL(stateChanged(int)),this,SLOT(sp7CheckBoxSlot(int)));
	sp8CheckBox=new QCheckBox(tr("8"));if(wordVector[7])sp8CheckBox->setChecked(true);mainLayout->addWidget(sp8CheckBox);connect(sp8CheckBox,SIGNAL(stateChanged(int)),this,SLOT(sp8CheckBoxSlot(int)));
}
layerOfSpacersClass::~layerOfSpacersClass()
{
}
//
void layerOfSpacersClass::validate()
{
	if (previousLayer)
	{
		if (previousLayer->isActive())//&&(previousLayer->getMinCombNumber()<MAX_COMBS))
		{
			show();
			onCombCheckBox->setEnabled(true);
			//combNumberSpinBox->setEnabled(true);
			sp1CheckBox->setEnabled(true);
			sp2CheckBox->setEnabled(true);
			sp3CheckBox->setEnabled(true);
			sp4CheckBox->setEnabled(true);
			sp5CheckBox->setEnabled(true);
			sp6CheckBox->setEnabled(true);
			sp7CheckBox->setEnabled(true);
			sp8CheckBox->setEnabled(true);
		}
		else
		{
			hide();
			sp1CheckBox->setChecked(false);
			sp2CheckBox->setChecked(false);
			sp3CheckBox->setChecked(false);
			sp4CheckBox->setChecked(false);
			sp5CheckBox->setChecked(false);
			sp6CheckBox->setChecked(false);
			sp7CheckBox->setChecked(false);
			sp8CheckBox->setChecked(false);
		}
		//int cn=0;
		//if (previousLayer->getIsOnComb())
		//	cn=previousLayer->getCombNumber()+1;
		//else
		//	cn=previousLayer->getMinCombNumber();
		//cn=cn>MAX_COMBS?MAX_COMBS:cn;
		//combNumberSpinBox->blockSignals(true);
		//combNumberSpinBox->setMinimum(cn);
		//combNumberSpinBox->setValue(cn);
		//combNumberSpinBox->blockSignals(false);
	}
	if (nextLayer)
		nextLayer->validate();
}
bool layerOfSpacersClass::isActive()
{
	if (sp1CheckBox->isChecked())return true;
	if (sp2CheckBox->isChecked())return true;
	if (sp3CheckBox->isChecked())return true;
	if (sp4CheckBox->isChecked())return true;
	if (sp5CheckBox->isChecked())return true;
	if (sp6CheckBox->isChecked())return true;
	if (sp7CheckBox->isChecked())return true;
	if (sp8CheckBox->isChecked())return true;
	return false;
}
void layerOfSpacersClass::setNextLayerClass(layerOfSpacersClass* nl)
{
	nextLayer=nl;
}
void layerOfSpacersClass::setPreviousLayerClass(layerOfSpacersClass* pl)
{
	previousLayer=pl;
}
bool layerOfSpacersClass::getIsOnComb()
{
	return onCombCheckBox->isChecked();
}
//int layerOfSpacersClass::getCombNumber()
//{
//	if (onCombCheckBox->isChecked())
//		return combNumberSpinBox->value();
//	else
//		return 0;
//}
//int layerOfSpacersClass::getMinCombNumber()
//{
//	return combNumberSpinBox->minimum();
//}
int layerOfSpacersClass::getSp1()
{
	return sp1CheckBox->isChecked()?1:0;
}
int layerOfSpacersClass::getSp2()
{
	return sp2CheckBox->isChecked()?1:0;
}
int layerOfSpacersClass::getSp3()
{
	return sp3CheckBox->isChecked()?1:0;
}
int layerOfSpacersClass::getSp4()
{
	return sp4CheckBox->isChecked()?1:0;
}
int layerOfSpacersClass::getSp5()
{
	return sp5CheckBox->isChecked()?1:0;
}
int layerOfSpacersClass::getSp6()
{
	return sp6CheckBox->isChecked()?1:0;
}
int layerOfSpacersClass::getSp7()
{
	return sp7CheckBox->isChecked()?1:0;
}
int layerOfSpacersClass::getSp8()
{
	return sp8CheckBox->isChecked()?1:0;
}
/////
//void layerOfSpacersClass::combNumberSpinBoxSlot(int v)
//{
//	onCombCheckBox->setChecked(true);
//	if (nextLayer)
//		nextLayer->validate();
//}
void layerOfSpacersClass::onCombCheckBoxSlot(int v)
{
	//bool in=v==Qt::Checked?true:false;
	////if (combNumberSpinBox->minimum()==MAX_COMBS)
	////	onCombCheckBox->setChecked(false);
	//if (previousLayer)
	//{
	//	int cn=0;
	//	if (previousLayer->getIsOnComb())
	//		cn=previousLayer->getCombNumber()+1;
	//	else
	//		cn=previousLayer->getMinCombNumber();
	//	cn=cn>MAX_COMBS?MAX_COMBS:cn;
	//	combNumberSpinBox->blockSignals(true);
	//	combNumberSpinBox->setMinimum(cn);
	//	combNumberSpinBox->setValue(cn);
	//	combNumberSpinBox->blockSignals(false);
	//}
	//if (nextLayer)
	//	nextLayer->validate();
}
void layerOfSpacersClass::sp1CheckBoxSlot(int v)
{
	if (nextLayer)
		nextLayer->validate();
}
void layerOfSpacersClass::sp2CheckBoxSlot(int v)
{
	if (nextLayer)
		nextLayer->validate();
}
void layerOfSpacersClass::sp3CheckBoxSlot(int v)
{
	if (nextLayer)
		nextLayer->validate();
}
void layerOfSpacersClass::sp4CheckBoxSlot(int v)
{
	if (nextLayer)
		nextLayer->validate();
}
void layerOfSpacersClass::sp5CheckBoxSlot(int v)
{
	if (nextLayer)
		nextLayer->validate();
}
void layerOfSpacersClass::sp6CheckBoxSlot(int v)
{
	if (nextLayer)
		nextLayer->validate();
}
void layerOfSpacersClass::sp7CheckBoxSlot(int v)
{
	if (nextLayer)
		nextLayer->validate();
}
void layerOfSpacersClass::sp8CheckBoxSlot(int v)
{
	if (nextLayer)
		nextLayer->validate();
}

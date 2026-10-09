#include "op.h"
optionsClass::optionsClass(QWidget *p):QDialog(p)
{
	setWindowTitle(tr("Options-production plan sfotware"));
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	//dataLayout
	QGridLayout *dataLayout=new QGridLayout();mainLayout->addLayout(dataLayout);
	//pcThatAboveThatTheLayerCanSupportAnOtherLayer
	dataLayout->addWidget(new QLabel(tr("Minimum stable width of layer:")),0,0);
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox=new QSpinBox();
	dataLayout->addWidget(pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox,0,1);
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->setSuffix(tr("%"));
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->setMinimum(66);
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->setMaximum(90);
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->setValue(GETONE(QString("select value from settings where name='namelayerMinSuportLength      '")).toInt());
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->setMinimumWidth(100);
	//maxNoOfProfilesToLoadSpinBox
	dataLayout->addWidget(new QLabel(tr("Max no of profiles to load (0 for all):")),1,0);
	maxNoOfProfilesToLoadSpinBox=new QSpinBox();
	dataLayout->addWidget(maxNoOfProfilesToLoadSpinBox,1,1);
	maxNoOfProfilesToLoadSpinBox->setMinimum(0);
	maxNoOfProfilesToLoadSpinBox->setMaximum(INT_MAX);
	maxNoOfProfilesToLoadSpinBox->setValue(MAX_NO_OF_PROFILES_TO_LOAD==INT_MAX?0:MAX_NO_OF_PROFILES_TO_LOAD);
	maxNoOfProfilesToLoadSpinBox->setMinimumWidth(100);
	//angleForArcDrawingSpinBox
	dataLayout->addWidget(new QLabel(tr("Angle for drawing arcs:")),2,0);
	angleForArcDrawingSpinBox=new QDoubleSpinBox();
	dataLayout->addWidget(angleForArcDrawingSpinBox,2,1);
	angleForArcDrawingSpinBox->setSuffix(QString("degrees"));
	angleForArcDrawingSpinBox->setMinimum(10);
	angleForArcDrawingSpinBox->setMaximum(45);
	angleForArcDrawingSpinBox->setValue(GETONE(QString("select value from settings where name='arcAngleForLine               '")).toDouble());
	//btnsLayout
	QHBoxLayout *btnsLayout=new QHBoxLayout();mainLayout->addLayout(btnsLayout);
	QPushButton *okBtn=new QPushButton(tr("Ok"),this);
	btnsLayout->addWidget(okBtn);
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	QPushButton *cancelBtn=new QPushButton(tr("Cancel"),this);
	btnsLayout->addWidget(cancelBtn);
	connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
	//
	mainLayout->addStretch(1);
}
optionsClass::~optionsClass()
{
}
/////
void optionsClass::accept()
{
	MAX_NO_OF_PROFILES_TO_LOAD=maxNoOfProfilesToLoadSpinBox->value();
	if (MAX_NO_OF_PROFILES_TO_LOAD<1)
		MAX_NO_OF_PROFILES_TO_LOAD=INT_MAX;
	apexExecQuery(QString("update settings set value='%1' where name='namelayerMinSuportLength      '").arg(pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->value()));
	apexExecQuery(QString("update users set maxNoOfProfilesToLoad='%1' where login='%2'").arg(MAX_NO_OF_PROFILES_TO_LOAD).arg(LOGINNAME));
	apexExecQuery(QString("update settings set value='%1' where name='arcAngleForLine               '").arg(angleForArcDrawingSpinBox->value()));
	ARC_ANGLE_FOR_LINE=angleForArcDrawingSpinBox->value();
	apexExecQuery(QString("update users set needsUpdate=1"));
	QDialog::accept();
}
void optionsClass::reject()
{
	QDialog::reject();
}

#include "pch.h"
#include "autoWdg.h"
autoWidgetClass::autoWidgetClass(craneClass *crane_,workStationClass	*workStation_,tuxipClass *tuxip_,QWidget *p):QWidget(p)
{
	crane=crane_;
	workStation=workStation_;
	tuxip=tuxip_;
	isBasketContentOk=true;
	basketOnWork=0;
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);mainLayout->setMargin(1);
	QGridLayout *upLayout=new QGridLayout();upLayout->setMargin(1);mainLayout->addLayout(upLayout);
	QVBoxLayout *downLayout=new QVBoxLayout();downLayout->setMargin(1);mainLayout->addLayout(downLayout);
	//
	exportsCheckBtn=new QCheckBox(tr("Automatic missions"),this);
	upLayout->addWidget(exportsCheckBtn,0,0,1,2);
	connect(exportsCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(exportsCheckBtnStateChangedSlot(int)));
	exportsCheckBtn->setEnabled(true);
	//
	upLayout->addWidget(new QLabel(tr("Basket:")),1,0);
	basketLabel=new QLabel("");
	upLayout->addWidget(basketLabel,1,1);

	returnBtn=new QPushButton(QIcon(":/saveAndReturn.png"),tr("Return"),this);
	connect(returnBtn,SIGNAL(clicked()),this,SLOT(saveAndReturnBtnClickedSlot()));
	upLayout->addWidget(returnBtn,1,2);

	//
	QSplitter *downSplitter=new QSplitter(Qt::Vertical);
	downSplitter->setStyleSheet(splitterVerticalStyle);
	downLayout->addWidget(downSplitter);
	basketContents=new basketContentsClass(this);
	downSplitter->addWidget(basketContents);
	//
	exportStationWidget=new exportStationWidgetClass(exportsTable,tr("Exports"),exitPlcId,this);
	downSplitter->addWidget(exportStationWidget);
	connect(exportStationWidget,SIGNAL(listChangedSignal()),crane,SLOT(missionsTimerSlot()));
	connect(workStation,SIGNAL(basketChangedSignal(int)),this,SLOT(basketChangedSlot(int)));
	basketChangedSlot(workStation->getBasketNumber());
	connect(basketContents,SIGNAL(stateChangedSignal(bool)),this,SLOT(basketContentsStateChangedSlot(bool)));
	//connect(exportStation->getExportStationWidget(),SIGNAL(listChangedSignal()),allPos->getWorkStation(),SLOT(loadFromDatabaseSlot()));
}
autoWidgetClass::~autoWidgetClass()
{
}
void autoWidgetClass::enableReturnButton()
{
	returnBtn->setEnabled((basketOnWork!=0)&&isBasketContentOk);
}
void autoWidgetClass::basketContentsStateChangedSlot(bool isOk)
{
	isBasketContentOk=isOk;
	enableReturnButton();
}
void autoWidgetClass::saveAndReturnBtnClickedSlot()
{
	basketContents->save();
	workStation->setIsReadyToReturn(true);
	tuxip->writeInteger("bRobotToPcI",22,32);
	tuxip->writeInteger("bRobotToPcI",22,0);
}
void autoWidgetClass::basketChangedSlot(int basketOnWork_)
{
	basketOnWork=basketOnWork_;
	basketContents->updateData(basketOnWork);
	enableReturnButton();
}
void autoWidgetClass::exportsCheckBtnStateChangedSlot(int state)
{
	if (exportsCheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoExports);
	else
		crane->removeFromAutoMode(craneClass::autoExports);
}

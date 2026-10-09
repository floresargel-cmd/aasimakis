#include "options.h"
optionsClass::optionsClass()
{
	forGroupOrders=NULL;
	aftGroupOrders=NULL;
	//
	maxHeightLineEdit=new QLineEdit();
	maxWeightForLineEdit=new QLineEdit();
	maxWeightAftLineEdit=new QLineEdit();
	//
	QGridLayout *mainLayout=new QGridLayout();
	int row=0;
	//row
	mainLayout->addWidget(new QLabel(tr("Maximum available height:")),row,0);
	mainLayout->addWidget(maxHeightLineEdit,row,1);maxHeightLineEdit->setText("2650");
	mainLayout->addWidget(new QLabel(tr("mm")),row,2);
	//
	row=1;
	mainLayout->addWidget(new QLabel(tr("Maximum weight forward:")),row,0);
	mainLayout->addWidget(maxWeightForLineEdit,row,1);maxWeightForLineEdit->setText("10000");
	mainLayout->addWidget(new QLabel(tr("Kgr")),row,2);
	//
	row=2;
	mainLayout->addWidget(new QLabel(tr("Maximum weight backward:")),row,0);
	mainLayout->addWidget(maxWeightAftLineEdit,row,1);maxWeightAftLineEdit->setText("13000");
	mainLayout->addWidget(new QLabel(tr("Kgr")),row,2);
	//
	mainLayout->setColumnStretch(3,1);
	mainLayout->setRowStretch(3,1);
	//
	setLayout(mainLayout);
	connect(maxHeightLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(maxHeightLineEditChanged()));
	connect(maxWeightForLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(maxWeightForLineEditChanged()));
	connect(maxWeightAftLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(maxWeightAftLineEditChanged()));
	connect(&mainTimer, SIGNAL(timeout()), this, SLOT(onMainTimer()));
	mainTimer.start(1*1000);
}
void optionsClass::maxHeightLineEditChanged()
{
	if (forGroupOrders)
		forGroupOrders->setMaxHeight(getMaxAvailableHeight());
	if (aftGroupOrders)
		aftGroupOrders->setMaxHeight(getMaxAvailableHeight());
}
void optionsClass::maxWeightForLineEditChanged()
{
	if (forGroupOrders)
		forGroupOrders->setMaxWeight(getMaxWeightFor());
}
void optionsClass::maxWeightAftLineEditChanged()
{
	if (aftGroupOrders)
		aftGroupOrders->setMaxWeight(getMaxWeightAft());
}
void optionsClass::setForGroupOfOrders(groupOfColumnsClass *g)
{
	forGroupOrders=g;
}
void optionsClass::setAftGroupOfOrders(groupOfColumnsClass *g)
{
	aftGroupOrders=g;
}
void optionsClass::onMainTimer()
{

}
int optionsClass::getMaxAvailableHeight()
{
	return maxHeightLineEdit->text().toInt();
}
int optionsClass::getMaxWeightFor()
{
	return maxWeightForLineEdit->text().toInt();
}
int optionsClass::getMaxWeightAft()
{
	return maxWeightAftLineEdit->text().toInt();
}

#include "pRes.h"
positionResetClass::positionResetClass(QString title,int plcNoBasketCommand_,int plcFullBasketCommand_,int plcEmptyBasketCommand_):QGroupBox(title)
{
	buttonGroup=new QButtonGroup();
	abovePosition=NULL;
	plcNoBasketCommand=plcNoBasketCommand_;
	plcFullBasketCommand=plcFullBasketCommand_;
	plcEmptyBasketCommand=plcEmptyBasketCommand_;
	activeCommand=plcNoBasketCommand;
	QHBoxLayout *mainLayout=new QHBoxLayout();QWidget::setLayout(mainLayout);
	noBasketRadioButton=new QRadioButton(tr("No SB (%1)").arg(plcNoBasketCommand));mainLayout->addWidget(noBasketRadioButton);buttonGroup->addButton(noBasketRadioButton,plcNoBasketCommand);noBasketRadioButton->setChecked(true);
	emptyBasketRadioButton=new QRadioButton(tr("Empty SB (%1)").arg(plcEmptyBasketCommand));mainLayout->addWidget(emptyBasketRadioButton);buttonGroup->addButton(emptyBasketRadioButton,plcEmptyBasketCommand);
	fullBasketRadioButton=new QRadioButton(tr("Full SB (%1)").arg(plcFullBasketCommand));mainLayout->addWidget(fullBasketRadioButton);buttonGroup->addButton(fullBasketRadioButton,plcFullBasketCommand);
	connect(buttonGroup,SIGNAL(buttonReleased(int)), this, SLOT(buttonGroupButtonReleased(int)));
	isPosEnabled=false;
}
void positionResetClass::setAbovePosition(positionResetClass* a)
{
	abovePosition=a;
}
void positionResetClass::showAll()
{
	noBasketRadioButton->show();
	emptyBasketRadioButton->show();
	fullBasketRadioButton->show();
	noBasketRadioButton->setEnabled(true);		
	fullBasketRadioButton->setEnabled(true);	
	emptyBasketRadioButton->setEnabled(true);

	if (abovePosition)
		abovePosition->showAll();
}
void positionResetClass::hideNoBasket()
{
	emptyBasketRadioButton->setChecked(true);
	activeCommand=plcEmptyBasketCommand;
	noBasketRadioButton->hide();

	noBasketRadioButton->setEnabled(true);		
	fullBasketRadioButton->setEnabled(true);	
	emptyBasketRadioButton->setEnabled(true);

	if (abovePosition)
		abovePosition->hideNoBasket();
}
void positionResetClass::hideFullBasket()
{
	emptyBasketRadioButton->setChecked(true);
	activeCommand=plcEmptyBasketCommand;
	fullBasketRadioButton->hide();

	noBasketRadioButton->setEnabled(true);		
	fullBasketRadioButton->setEnabled(true);	
	emptyBasketRadioButton->setEnabled(true);

	if (abovePosition)
		abovePosition->hideFullBasket();
}
void positionResetClass::buttonGroupButtonReleased(int id)
{
	if (id==plcNoBasketCommand)
	{
		activeCommand=plcNoBasketCommand;
		if (abovePosition)
			abovePosition->disable();
	}
	else if (id==plcFullBasketCommand)
	{
		activeCommand=plcFullBasketCommand;
		if (abovePosition)
			abovePosition->enable();
	}
	else if (id==plcEmptyBasketCommand)
	{
		activeCommand=plcEmptyBasketCommand;
		if (abovePosition)
			abovePosition->enable();
	}
}
void positionResetClass::initialize()
{
	enable();
	noBasketRadioButton->setChecked(true);
	if (abovePosition)
		abovePosition->disable();
}
void positionResetClass::disable()
{
	noBasketRadioButton->setChecked(true);
	activeCommand=plcNoBasketCommand;
	noBasketRadioButton->setEnabled(false);
	fullBasketRadioButton->setEnabled(false);
	emptyBasketRadioButton->setEnabled(false);
	isPosEnabled=false;
	if (abovePosition)
		abovePosition->disable();
}
void positionResetClass::enable()
{
	if (!isPosEnabled)
	{
		noBasketRadioButton->setChecked(true);
		activeCommand=plcNoBasketCommand;
		noBasketRadioButton->setEnabled(true);		
		fullBasketRadioButton->setEnabled(true);	
		emptyBasketRadioButton->setEnabled(true);
		isPosEnabled=true;
	}
}
int positionResetClass::getActiveCommand()
{
	return activeCommand;
}
#ifdef EMULATE
bool positionResetClass::getIsNoBasket()
{
	if (activeCommand==plcNoBasketCommand)
		return true;
	return false;
}
bool positionResetClass::getIsFullBasket()
{
	if (activeCommand==plcFullBasketCommand)
		return true;
	return false;
}
bool positionResetClass::getIsEmptyBasket()
{
	if (activeCommand==plcEmptyBasketCommand)
		return true;
	return false;
}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////mainPositionResetClass////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
mainPositionResetClass::mainPositionResetClass(tcpServerClass *tcpServer_,QVector<int> plcCommands,QString title,QWidget *p):QDialog(p)
{
	QDialog::setWindowTitle(QString("Define contents of '%1'").arg(title));
	tcpServer=tcpServer_;
	firstLevelPosition=secondLevelPosition=thirdLevelPosition=NULL;
	if (plcCommands.count()==3)
		firstLevelPosition=new positionResetClass(tr("First level"),plcCommands[0],plcCommands[1],plcCommands[2]);
	else if (plcCommands.count()==6)
	{
		firstLevelPosition=new positionResetClass(tr("First level"),plcCommands[0],plcCommands[1],plcCommands[2]);
		secondLevelPosition=new positionResetClass(tr("Second level"),plcCommands[3],plcCommands[4],plcCommands[5]);
		firstLevelPosition->setAbovePosition(secondLevelPosition);
	}
	else if (plcCommands.count()==9)
	{
		firstLevelPosition=new positionResetClass(tr("First level"),plcCommands[0],plcCommands[1],plcCommands[2]);
		secondLevelPosition=new positionResetClass(tr("Second level"),plcCommands[3],plcCommands[4],plcCommands[5]);
		thirdLevelPosition=new positionResetClass(tr("Third level"),plcCommands[6],plcCommands[7],plcCommands[8]);
		firstLevelPosition->setAbovePosition(secondLevelPosition);
		secondLevelPosition->setAbovePosition(thirdLevelPosition);
	}
	firstLevelPosition->initialize();
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	if (thirdLevelPosition)mainLayout->addWidget(thirdLevelPosition);
	if (secondLevelPosition)mainLayout->addWidget(secondLevelPosition);
	if (firstLevelPosition)mainLayout->addWidget(firstLevelPosition);
	QHBoxLayout *btnsLayout=new QHBoxLayout();mainLayout->addLayout(btnsLayout);
	QPushButton *canclePushButton=new QPushButton(tr("Cancel"));connect(canclePushButton,SIGNAL(clicked()),this,SLOT(cancelButtonClickedSlot()));btnsLayout->addWidget(canclePushButton);
	QPushButton *okPushButton=new QPushButton(tr("Ok"));connect(okPushButton,SIGNAL(clicked()),this,SLOT(okButtonClickedSlot()));btnsLayout->addWidget(okPushButton);
	showAll();
}
void mainPositionResetClass::showAll()
{
	firstLevelPosition->showAll();
}
void mainPositionResetClass::hideNoBasket()
{
	firstLevelPosition->hideNoBasket();
}
void mainPositionResetClass::hideFullBasket()
{
	firstLevelPosition->hideFullBasket();
}
positionResetClass* mainPositionResetClass::getFirstLevelPosition()
{
	return firstLevelPosition;
}
positionResetClass* mainPositionResetClass::getSecondLevelPosition()
{
	return secondLevelPosition;
}
positionResetClass* mainPositionResetClass::getThirdLevelPosition()
{
	return thirdLevelPosition;
}
void mainPositionResetClass::okButtonClickedSlot()
{
	if (firstLevelPosition)
		tcpServer->sendString(QString("N22|4|%1").arg(firstLevelPosition->getActiveCommand()));
	if (secondLevelPosition)
		tcpServer->sendString(QString("N22|4|%1").arg(secondLevelPosition->getActiveCommand()));
	if (thirdLevelPosition)
		tcpServer->sendString(QString("N22|4|%1").arg(thirdLevelPosition->getActiveCommand()));
	tcpServer->sendString(QString("N22|4|0"));
	QDialog::accept();
}
void mainPositionResetClass::cancelButtonClickedSlot()
{
	QDialog::reject();
}
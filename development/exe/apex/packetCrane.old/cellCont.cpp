#include "cellCont.h"
cellContentsClass::cellContentsClass(int cellNumber_,QWidget *p):QDialog(p)
{
	cellNumber=cellNumber_!=0?cellNumber_:1;
	packet=GETFIRST(QString("select packetId from positions where posId=%1").arg(cellNumber)).toInt();
	setMinimumWidth(350);
	setWindowTitle(tr("Properies of cell %1").arg(cellNumber));

	isPackOk=false;
	QVBoxLayout *mainLayout=new QVBoxLayout();
  setLayout(mainLayout);
	////cell layout
	//QHBoxLayout *cellLayout=new QHBoxLayout();mainLayout->addLayout(cellLayout);
	//cellLayout->addWidget(new QLabel(tr("Cell number:")));
	//cellSpinBox=new QSpinBox();cellLayout->addWidget(cellSpinBox);
	//cellSpinBox->setMinimum(-780);
	//cellSpinBox->setMaximum(+780);
	//connect(cellSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onCellNumberSpinBoxChangedSlot(int)));
	//packet layout
	QHBoxLayout *packetLayout=new QHBoxLayout();mainLayout->addLayout(packetLayout);
	packetLayout->addWidget(new QLabel(tr("Packet number:")));
	packetSpinBox=new QSpinBox();packetLayout->addWidget(packetSpinBox);
	packetSpinBox->setMinimum(0);
	packetSpinBox->setMaximum(INT_MAX);
	connect(packetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onPacketSpinBoxChangedSlot(int)));
	//locked layout
	QHBoxLayout *isLockedLayout=new QHBoxLayout();mainLayout->addLayout(isLockedLayout);
	isLockedLayout->addWidget(new QLabel(tr("Is locked:")));
	isLockedCheckBox=new QCheckBox();isLockedLayout->addWidget(isLockedCheckBox);
	connect(isLockedCheckBox,SIGNAL(stateChanged (int)),this,SLOT(onLockedCheckButtonClickedSlot(int)));
	//cassette pos layout
	QVBoxLayout *cassettePosLayout=new QVBoxLayout();mainLayout->addLayout(cassettePosLayout);
	cassetteInCellRadioButton=new QRadioButton(tr("Cassette is in cell"));
	cassettePosLayout->addWidget(cassetteInCellRadioButton);
	connect(cassetteInCellRadioButton,SIGNAL(toggled(bool)),this,SLOT(onCassetteInCellRadioButtonClickedSlot(bool)));
	cassetteOnCraneRadioButton=new QRadioButton(tr("Cassette is on crane"));
	cassettePosLayout->addWidget(cassetteOnCraneRadioButton);
	connect(cassetteOnCraneRadioButton,SIGNAL(toggled(bool)),this,SLOT(onCassetteOnCraneRadioButtonClickedSlot(bool)));
	cassetteOnWestConveyorRadioButton=new QRadioButton(tr("Cassette is on west conveyor"));
	cassettePosLayout->addWidget(cassetteOnWestConveyorRadioButton);
	connect(cassetteOnWestConveyorRadioButton,SIGNAL(toggled(bool)),this,SLOT(onCassetteOnWestConveyorRadioButtonClickedSlot(bool)));
	cassetteOnEastConveyorRadioButton=new QRadioButton(tr("Cassette is on east conveyor"));
	cassettePosLayout->addWidget(cassetteOnEastConveyorRadioButton);
	connect(cassetteOnEastConveyorRadioButton,SIGNAL(toggled(bool)),this,SLOT(onCassetteOnEastConveyorRadioButtonClickedSlot(bool)));
	//buttons layout
	QHBoxLayout *buttonsLayout=new QHBoxLayout();mainLayout->addLayout(buttonsLayout);
	savePushButton=new QPushButton(tr("Save"));buttonsLayout->addWidget(savePushButton);
	connect(savePushButton,SIGNAL(clicked()),this,SLOT(onSaveBtnSlot()));
	cancelPushButton=new QPushButton(tr("Cancel"));buttonsLayout->addWidget(cancelPushButton);
	connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(reject()));

	loadFromDatabase();
	check();
}
void cellContentsClass::updateGui()
{
	if (cellNumber!=0)
	{
		packetSpinBox->setValue(packet);
		if (isLocked)
			isLockedCheckBox->setChecked(true);
		else
			isLockedCheckBox->setChecked(false);
		cassetteInCellRadioButton->setChecked(false);
		cassetteOnCraneRadioButton->setChecked(false);
		cassetteOnWestConveyorRadioButton->setChecked(false);
		cassetteOnEastConveyorRadioButton->setChecked(false);
		if (cassetePosEnum==CASSETTE_IN_CELL)
			cassetteInCellRadioButton->setChecked(true);
		else if (cassetePosEnum==CASSETTTE_ON_CRANE)
			cassetteOnCraneRadioButton->setChecked(true);
		else if (cassetePosEnum==CASSETTTE_ON_WEST_CONVEYOR)
			cassetteOnWestConveyorRadioButton->setChecked(true);
		else if (cassetePosEnum==CASSETTTE_ON_EAST_CONVEYOR)
			cassetteOnEastConveyorRadioButton->setChecked(true);
		packetSpinBox->setEnabled(true);
		isLockedCheckBox->setEnabled(true);
		cassetteInCellRadioButton->setEnabled(true);
		cassetteOnCraneRadioButton->setEnabled(true);
		cassetteOnWestConveyorRadioButton->setEnabled(true);
		cassetteOnEastConveyorRadioButton->setEnabled(true);
	}
	else
	{
		packetSpinBox->setValue(0);
		isLockedCheckBox->setChecked(false);
		cassetteInCellRadioButton->setChecked(false);
		cassetteOnCraneRadioButton->setChecked(false);
		cassetteOnWestConveyorRadioButton->setChecked(false);
		cassetteOnEastConveyorRadioButton->setChecked(false);

		packetSpinBox->setEnabled(false);
		isLockedCheckBox->setEnabled(false);
		cassetteInCellRadioButton->setEnabled(false);
		cassetteOnCraneRadioButton->setEnabled(false);
		cassetteOnWestConveyorRadioButton->setEnabled(false);
		cassetteOnEastConveyorRadioButton->setEnabled(false);
	}
}
void cellContentsClass::saveToDatabase()
{
	if (cellNumber!=0)
	{
		apexExecQuery(QString("update positions set packetId=%1 where posId=%2").arg(packet).arg(cellNumber));
		apexExecQuery(QString("update positions set isLocked=%1 where posId=%2").arg(isLocked?"1":"0").arg(cellNumber));

		apexExecQuery(QString("update positions set cassette=0,packetId=0 where cassette=%1").arg(cellNumber));
		if (cassetePosEnum==CASSETTTE_ON_CRANE)
			apexExecQuery(QString("update positions set cassette=%1,packetId=%2 where posId=%3").arg(cellNumber).arg(packet).arg(POS_ID_CRANE));
		if (cassetePosEnum==CASSETTE_IN_CELL)
			apexExecQuery(QString("update positions set cassette=%1,packetId=%2 where posId=%1").arg(cellNumber).arg(packet));
		if (cassetePosEnum==CASSETTTE_ON_WEST_CONVEYOR)
			apexExecQuery(QString("update positions set cassette=%1,packetId=%2 where posId=%3").arg(cellNumber).arg(packet).arg(POS_ID_WESTCONV));
		if (cassetePosEnum==CASSETTTE_ON_EAST_CONVEYOR)
			apexExecQuery(QString("update positions set cassette=%1,packetId=%2 where posId=%3").arg(cellNumber).arg(packet).arg(POS_ID_EASTCONV));
		emit cellDataChangedSignal(cellNumber);
	}
}
void cellContentsClass::loadFromDatabase()
{
	if (cellNumber!=0)
	{
		packet=GETFIRST(QString("select packetId from positions where posId=%1").arg(cellNumber)).toInt();
		isLocked=GETFIRST(QString("select isLocked from positions where posId=%1").arg(cellNumber)).toBool();
		int posIdOfCassette=GETFIRST(QString("select posId from positions where cassette=%1").arg(cellNumber)).toInt();
		if (posIdOfCassette==POS_ID_CRANE)cassetePosEnum=CASSETTTE_ON_CRANE;
		else if (posIdOfCassette==POS_ID_WESTCONV)cassetePosEnum=CASSETTTE_ON_WEST_CONVEYOR;
		else if (posIdOfCassette==POS_ID_EASTCONV)cassetePosEnum=CASSETTTE_ON_EAST_CONVEYOR;
		else if (posIdOfCassette==cellNumber)cassetePosEnum=CASSETTE_IN_CELL;
		else cassetePosEnum=CASSETTE_NO_WHERE;
	}
	updateGui();
}
void cellContentsClass::check()
{
	int packetPid=GETFIRST(QString("select tid from packets where packetId=%1").arg(packet)).toInt();
	if ((packet!=0)&&(packetPid==0))
	{
		isPackOk=false;
		packetSpinBox->setStyleSheet("QSpinBox{color:red}");
	}
	else
	{
		isPackOk=true;
		packetSpinBox->setStyleSheet("QSpinBox{color:black}");
	}
	savePushButton->setEnabled(isPackOk);
}
void cellContentsClass::onPacketSpinBoxChangedSlot(int pack)
{
	packet=pack;
	check();
	//
}
void cellContentsClass::onLockedCheckButtonClickedSlot(int state)
{
	if (state==Qt::Checked)
		isLocked=true;
	else
		isLocked=false;
}
void cellContentsClass::onCassetteInCellRadioButtonClickedSlot(bool s)
{
	if (s)cassetePosEnum=CASSETTE_IN_CELL;
}
void cellContentsClass::onCassetteOnCraneRadioButtonClickedSlot(bool s)
{
	if (s)cassetePosEnum=CASSETTTE_ON_CRANE;
}
void cellContentsClass::onCassetteOnWestConveyorRadioButtonClickedSlot(bool s)
{
	if (s)cassetePosEnum=CASSETTTE_ON_WEST_CONVEYOR;
}
void cellContentsClass::onCassetteOnEastConveyorRadioButtonClickedSlot(bool s)
{
	if (s)cassetePosEnum=CASSETTTE_ON_EAST_CONVEYOR;
}
void cellContentsClass::onSaveBtnSlot()
{
	saveToDatabase();
}

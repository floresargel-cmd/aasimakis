#include "pch.h"
#include "cellCont.h"
cellContentsClass::cellContentsClass(int posId_,QWidget *p):QDialog(p)
{
	posId=posId_;
	takesCassette=((posId<MAX_CELL_POS_ID)||(posId==POS_ID_WESTCONV)||(posId==POS_ID_EASTCONV))||((posId>=stationExport2PlcId)&&(posId<=stationBackExport1PlcId))||((posId>=packingExport2PlcId)&&(posId<=packingBackExport1PlcId));
	isCell=(posId<MAX_CELL_POS_ID);
	packet=GETFIRST(QString("select packetId from positions where posId=%1").arg(posId)).toInt();
	cassette=GETFIRST(QString("select cassette from positions where posId=%1").arg(posId)).toInt();
	setMinimumWidth(350);
	setWindowTitle(tr("Properies of %1 (%2)").arg(GETFIRST(QString("select posName from positions where posId=%1").arg(posId)).toString().trimmed()).arg(posId));

	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	if (takesCassette)
	{
		QHBoxLayout *cassetteLayout=new QHBoxLayout();mainLayout->addLayout(cassetteLayout);
		cassetteLayout->addWidget(new QLabel(tr("Cassette number:")));
		cassetteSpinBox=new QSpinBox();cassetteLayout->addWidget(cassetteSpinBox);
		cassetteSpinBox->setMinimum(-MAX_CELL_POS_ID);
		cassetteSpinBox->setMaximum(MAX_CELL_POS_ID);
		connect(cassetteSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onCassetteSpinBoxSpinBoxChangedSlot(int)));
	}
	//packet layout
	QHBoxLayout *packetLayout=new QHBoxLayout();mainLayout->addLayout(packetLayout);
	packetLayout->addWidget(new QLabel(tr("Packet number:")));
	packetSpinBox=new QSpinBox();packetLayout->addWidget(packetSpinBox);
	packetSpinBox->setMinimum(0);
	packetSpinBox->setMaximum(INT_MAX);
	connect(packetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onPacketSpinBoxChangedSlot(int)));
	if (isCell)
	{
		//locked layout
		QHBoxLayout *isLockedLayout=new QHBoxLayout();mainLayout->addLayout(isLockedLayout);
		isLockedLayout->addWidget(new QLabel(tr("Is locked:")));
		isLockedCheckBox=new QCheckBox();isLockedLayout->addWidget(isLockedCheckBox);
		connect(isLockedCheckBox,SIGNAL(stateChanged (int)),this,SLOT(onLockedCheckButtonClickedSlot(int)));
		//corrections layout
		QGridLayout *correctionLayout=new QGridLayout();mainLayout->addLayout(correctionLayout);
		correctionLayout->addWidget(new QLabel(tr("X correction[mm]:")),0,0);
		xCorrSpinBox=new QSpinBox();correctionLayout->addWidget(xCorrSpinBox,0,1);
		xCorrSpinBox->setMinimum(-100);
		xCorrSpinBox->setMaximum(+100);
		connect(xCorrSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onXCorrSpinBoxSpinBoxChangedSlot(int)));
		correctionLayout->addWidget(new QLabel(tr("Z correction[mm]:")),1,0);
		zCorrSpinBox=new QSpinBox();correctionLayout->addWidget(zCorrSpinBox,1,1);
		zCorrSpinBox->setMinimum(-100);
		zCorrSpinBox->setMaximum(+100);
		connect(zCorrSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onZCorrSpinBoxSpinBoxChangedSlot(int)));
	}
	//buttons layout
	QHBoxLayout *buttonsLayout=new QHBoxLayout();mainLayout->addLayout(buttonsLayout);
	savePushButton=new QPushButton(tr("Ok"));buttonsLayout->addWidget(savePushButton);
	connect(savePushButton,SIGNAL(clicked()),this,SLOT(onSaveBtnSlot()));
	cancelPushButton=new QPushButton(tr("Cancel"));buttonsLayout->addWidget(cancelPushButton);
	connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(reject()));

	loadFromDatabase();
	check();
}
int cellContentsClass::getBasketNum()
{
	return cassetteSpinBox->value();
}
int cellContentsClass::getPacketId()
{
	return packetSpinBox->value();
}
void cellContentsClass::updateGui()
{
	if (posId!=0)
	{
		if (packetSpinBox)packetSpinBox->setValue(packet);
		if (cassetteSpinBox)cassetteSpinBox->setValue(cassette);
		if (isLockedCheckBox)isLockedCheckBox->setChecked(isLocked);
		if (packetSpinBox)packetSpinBox->setEnabled(true);
		if (cassetteSpinBox)cassetteSpinBox->setEnabled(true);
		if (isLockedCheckBox)isLockedCheckBox->setEnabled(true);
	}
	else
	{
		if (packetSpinBox)packetSpinBox->setValue(0);
		if (cassetteSpinBox)cassetteSpinBox->setValue(0);
		if (isLockedCheckBox)isLockedCheckBox->setChecked(false);
		if (packetSpinBox)packetSpinBox->setEnabled(false);
		if (cassetteSpinBox)cassetteSpinBox->setEnabled(false);
		if (isLockedCheckBox)isLockedCheckBox->setEnabled(false);
	}
	if (xCorrSpinBox)xCorrSpinBox->setValue(xCorr);
	if (zCorrSpinBox)zCorrSpinBox->setValue(zCorr);
}
void cellContentsClass::saveToDatabase()
{
	if (posId!=0)
	{
		apexExecQuery(QString("update positions set cassette=%1,packetId=%2,isLocked=%3,xCorr=%4,zCorr=%5 where posId=%6").arg(cassette).arg(packet).arg(isLocked?1:0).arg(xCorr).arg(zCorr).arg(posId));
		loadFromDatabase();
		emit cellDataChangedSignal(posId);
		emit cassetteChangedSignal(cassette);
	}
	QDialog::accept();
}
void cellContentsClass::loadFromDatabase()
{
	if (posId!=0)
	{
		packet=GETFIRST(QString("select packetId from positions where posId=%1").arg(posId)).toInt();
		cassette=GETFIRST(QString("select cassette from positions where posId=%1").arg(posId)).toInt();
		isLocked=GETFIRST(QString("select isLocked from positions where posId=%1").arg(posId)).toBool();
		xCorr=GETFIRST(QString("select xCorr from positions where posId=%1").arg(posId)).toInt();
		zCorr=GETFIRST(QString("select zCorr from positions where posId=%1").arg(posId)).toInt();
	}
	updateGui();
}
void cellContentsClass::check()
{
	bool isPackOk=true;
	int packetPid=GETFIRST(QString("select tid from packets where packetId=%1").arg(packet)).toInt();
	int packetPos=GETFIRST(QString("select posId from positions where packetId=%1").arg(packet)).toInt();
	if ((packet!=0)&&(packetPid==0))
	{
		isPackOk=false;
		packetSpinBox->setStyleSheet("QSpinBox{color:red}");
		packetSpinBox->setToolTip(tr("packet:%1 not defined in packets table").arg(packet));
	}
	else if ((packet!=0)&&(packetPos!=posId)&&(packetPos!=0))
	{
		isPackOk=false;
		packetSpinBox->setStyleSheet("QSpinBox{color:red}");
		packetSpinBox->setToolTip(tr("packet:%1 in position %2").arg(packet).arg(packetPos));
	}
	else
	{
		isPackOk=true;
		packetSpinBox->setStyleSheet("QSpinBox{color:black}");
	}
	//
	bool isCassOk=true;
	if (takesCassette)
	{
		int cassette=cassetteSpinBox->value();
		int cassettePos=GETFIRST(QString("select posId from positions where cassette=%1").arg(cassette)).toInt();
		if (posId>MAX_CELL_POS_ID)
		{
			if ((cassette!=0)&&(cassettePos!=posId)&&(cassettePos!=0))
			{
				isCassOk=false;
				cassetteSpinBox->setStyleSheet("QSpinBox{color:red}");
				cassetteSpinBox->setToolTip(tr("Cassette on position:%1").arg(cassettePos));
			}
			else
			{
				isCassOk=true;
				cassetteSpinBox->setStyleSheet("QSpinBox{color:black}");
			}
		}
		else//cell
		{
			if ((cassette!=0)&&(cassettePos!=0)&&(cassettePos!=posId))
			{
				isCassOk=false;
				cassetteSpinBox->setStyleSheet("QSpinBox{color:red}");
				cassetteSpinBox->setToolTip(tr("Cassette on position:%1").arg(cassettePos));
			}
			else
			{
				isCassOk=true;
				cassetteSpinBox->setStyleSheet("QSpinBox{color:black}");
			}
		}
	}
	savePushButton->setEnabled(isPackOk&&isCassOk);
}
void cellContentsClass::onPacketSpinBoxChangedSlot(int pack)
{
	packet=pack;
	check();
}
void cellContentsClass::onXCorrSpinBoxSpinBoxChangedSlot(int c)
{
	xCorr=c;
	check();
}
void cellContentsClass::onZCorrSpinBoxSpinBoxChangedSlot(int c)
{
	zCorr=c;
	check();
}
void cellContentsClass::onCassetteSpinBoxSpinBoxChangedSlot(int pack)
{
	cassette=pack;
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
void cellContentsClass::onSaveBtnSlot()
{
	saveToDatabase();
}

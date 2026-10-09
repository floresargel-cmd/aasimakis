#include "pch.h"
#include "posDlg.h"
#include "allPos.h"
extern bool makeTicketSpinReadOnly;
posDlgClass::posDlgClass(int posId_,bool isSbPos_,bool isLocked_,int cassetteNum_,int sbNumber_,int ticket_,QString description_,int column_,int row_,float plcXPos_,float plcYPos_,float plcZPos_,QWidget *p):QDialog(p)
{
	posId=posId_;
	isSbPos=isSbPos_;
	isLocked=isLocked_;
	description=description_;
	column=column_;
	row=row_;
	plcXPos=plcXPos_;
	plcYPos=plcYPos_;
	plcZPos=plcZPos_;
	cassetteNum=cassetteNum_;
	sbNumber=sbNumber_;
	ticket=ticket_;
	serialPort=NULL;
	//
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select dx,dy,dz from pos where posId=%1").arg(posId),mDb);
	plcXCorrection=plcYCorrection=plcZCorrection=0.;
	if (dataVV.count()==1)
	{
		plcXCorrection=dataVV[0][0].toDouble();
		plcYCorrection=dataVV[0][1].toDouble();
		plcZCorrection=dataVV[0][2].toDouble();
	}
	resize(1000,500);
	setWindowTitle(tr("Position %1,%2 %3").arg(posId).arg(description).arg(column>0?tr("(%3,%4)").arg(column).arg(row):""));
	int row=0;
	QVBoxLayout *mainL=new QVBoxLayout();mainL->setMargin(2);QDialog::setLayout(mainL);
	QGridLayout *dataWidgetLayout=new QGridLayout();dataWidgetLayout->setMargin(2);mainL->addLayout(dataWidgetLayout);
	QGridLayout *tableLayout=new QGridLayout();tableLayout->setMargin(2);mainL->addLayout(tableLayout);
	QHBoxLayout *btnsLayout=new QHBoxLayout();btnsLayout->setMargin(2);mainL->addLayout(btnsLayout);

	//cassette
	if (!isSbPos)
	{
		dataWidgetLayout->addWidget(new QLabel(tr("Cassette:")),row,0);
		cassetteSpinBox=new QSpinBox();
		cassetteSpinBox->setMinimum(minCassetteId);
		cassetteSpinBox->setSingleStep(1);
		cassetteSpinBox->setMaximum(maxCassetteId);
		cassetteSpinBox->setValue(cassetteNum);
		dataWidgetLayout->addWidget(cassetteSpinBox,row,1);
		connect(cassetteSpinBox,SIGNAL(valueChanged(int)),this,SLOT(cassetteSpinBoxValueChangedSlot(int)));
		cassetteInfoLabel=new QLabel(tr(""));cassetteInfoLabel->setMinimumWidth(200);
		dataWidgetLayout->addWidget(cassetteInfoLabel,row,2);
	}
	//
	//sb
	row++;
	dataWidgetLayout->addWidget(new QLabel(tr("Sb:")),row,0);
	sbSpinBox=new QSpinBox();
	sbSpinBox->setMinimum(0);sbSpinBox->setMaximum(INT_MAX);sbSpinBox->setValue(sbNumber);
	dataWidgetLayout->addWidget(sbSpinBox,row,1);
	connect(sbSpinBox,SIGNAL(valueChanged(int)),this,SLOT(sbSpinBoxValueChangedSlot(int)));
	sbInfoLabel=new QLabel(tr(""));sbInfoLabel->setMinimumWidth(200);
	dataWidgetLayout->addWidget(sbInfoLabel,row,2);
	//ticket
	row++;
	dataWidgetLayout->addWidget(new QLabel(tr("Ticket:")),row,0);
	ticketSpinBox=new QSpinBox();
	ticketSpinBox->setMinimum(0);
	ticketSpinBox->setSingleStep(1);
	ticketSpinBox->setMaximum(INT_MAX);
	ticketSpinBox->setValue(ticket);
	dataWidgetLayout->addWidget(ticketSpinBox,row,1);
	connect(ticketSpinBox,SIGNAL(valueChanged(int)),this,SLOT(ticketSpinBoxValueChangedSlot(int)));
	ticketInfoLabel=new QLabel(tr(""));ticketInfoLabel->setMinimumWidth(200);
	dataWidgetLayout->addWidget(ticketInfoLabel,row,2);

	destInRadio=NULL;
	destOutRadio=NULL;
	if ( (posId==pincer1_50_id)||(posId==pincer1_52_id)||(posId==pincer1_53_id)||(posId==pincer1_54_id) )
	{
		row++;
		dataWidgetLayout->addWidget(new QLabel(tr("Destination:")),row,0);
		dataWidgetLayout->addWidget(destInRadio=new QRadioButton(tr("in:")),row,1);
		connect(destInRadio,SIGNAL(toggled(bool)),this,SLOT(destInRadioToggledSlot(bool)));
		dataWidgetLayout->addWidget(destOutRadio=new QRadioButton(tr("out:")),row,2);
		connect(destOutRadio,SIGNAL(toggled(bool)),this,SLOT(destOutRadioToggledSlot(bool)));
	}
	xCorrectionSpinBox=yCorrectionSpinBox=zCorrectionSpinBox=NULL;
	if (posId<craneId)
	{
		row++;
		lockedCheckBox=new QCheckBox(tr("locked"));
		dataWidgetLayout->addWidget(lockedCheckBox,row,0);
		lockedCheckBox->setChecked(isLocked);
		//corrections
		row++;
		dataWidgetLayout->addWidget(new QLabel(tr("plc")),row,1);
		dataWidgetLayout->addWidget(new QLabel(tr("correction")),row,2);
		row++;
		dataWidgetLayout->addWidget(new QLabel(tr("X")),row,0);
		dataWidgetLayout->addWidget(new QLabel(QString("%1").arg(plcXPos)),row,1);
		xCorrectionSpinBox=new QDoubleSpinBox();xCorrectionSpinBox->setMinimum(-maxCorrection);xCorrectionSpinBox->setMaximum(maxCorrection);xCorrectionSpinBox->setValue(plcXCorrection);dataWidgetLayout->addWidget(xCorrectionSpinBox,row,2);
		row++;
		dataWidgetLayout->addWidget(new QLabel(tr("Y")),row,0);
		dataWidgetLayout->addWidget(new QLabel(QString("%1").arg(plcYPos)),row,1);
		yCorrectionSpinBox=new QDoubleSpinBox();yCorrectionSpinBox->setMinimum(-maxCorrection);yCorrectionSpinBox->setMaximum(maxCorrection);yCorrectionSpinBox->setValue(plcYCorrection);dataWidgetLayout->addWidget(yCorrectionSpinBox,row,2);
		row++;
		dataWidgetLayout->addWidget(new QLabel(tr("Z")),row,0);
		dataWidgetLayout->addWidget(new QLabel(QString("%1").arg(plcZPos)),row,1);
		zCorrectionSpinBox=new QDoubleSpinBox();zCorrectionSpinBox->setMinimum(-maxCorrection);zCorrectionSpinBox->setMaximum(maxCorrection);zCorrectionSpinBox->setValue(plcZCorrection);dataWidgetLayout->addWidget(zCorrectionSpinBox,row,2);
	}
	dataWidgetLayout->setColumnStretch(3,1);
	//
	QTableWidget *posLogTable=new QTableWidget();tableLayout->addWidget(posLogTable);
	posLogTable->setAlternatingRowColors(true);
	posLogTable->horizontalHeader()->setStretchLastSection(true);
	posLogTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	posLogTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	posLogTable->setColumnCount(3);
	posLogTable->setHorizontalHeaderLabels(QStringList()<<tr("cassette")<<tr("info")<<tr("time"));
	posLogTable->setWordWrap (true);

	QVector<QVector<QVariant>> corrVV=execTableQuery(QString("select cassette,info,time from positionsLog where posId=%1 order by tId desc limit 5").arg(posId),mDb);
	posLogTable->setRowCount(dataVV.count());
	for (int i=0;i<corrVV.count();i++)
	{
		posLogTable->setItem(i,0,new QTableWidgetItem(corrVV[i][0].toString()));
		posLogTable->setItem(i,1,new QTableWidgetItem(corrVV[i][1].toString().trimmed()));
		posLogTable->setItem(i,2,new QTableWidgetItem(corrVV[i][2].toString().trimmed()));
	}
	//
	okBtn=new QPushButton(tr("Ok"),this);
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	btnsLayout->addWidget(okBtn);

	if (posId==pincer1_50_id)
	{
		QString portStr="COM2";
		serialPort=new QSerialPort(this);
		serialPort->setPortName(portStr);
		serialPort->setBaudRate(QSerialPort::Baud9600);
		serialPort->setFlowControl(QSerialPort::NoFlowControl);
		serialPort->setParity(QSerialPort::NoParity);
		serialPort->setDataBits(QSerialPort::Data8);
		serialPort->setStopBits(QSerialPort::OneStop);
		ticketSpinBox->setReadOnly(makeTicketSpinReadOnly);
		if (!serialPort->open(QIODevice::ReadOnly))
			QMessageBox::critical(this, tr("Error"),tr("Error opening serial port %1.\nError:%2").arg(portStr).arg(serialPort->errorString()));
		connect(serialPort,SIGNAL(readyRead()),this,SLOT(readyReadSerialPortSlot()));
	}
}
posDlgClass::~posDlgClass()
{
}
int posDlgClass::myExec(bool checkPassword)
{
	updateGui();
	if ((!checkPassword)||(getPassword("Password is needed to open this window.","Please give the password",passWeak)))
		return QDialog::exec();
	return 0;
}
int posDlgClass::exec()
{
	return QDialog::exec();
}
int posDlgClass::getCassetteNum()
{
	if (!isSbPos)
		return cassetteSpinBox->value();
	return 0;
}
int posDlgClass::getSbNumber()
{
	return sbSpinBox->value();
}
int posDlgClass::getTicket()
{
	return ticketSpinBox->value();
}
bool posDlgClass::getIsLockedChecked()
{
	return lockedCheckBox->isChecked();
}
void posDlgClass::accept()
{
	if (xCorrectionSpinBox&&yCorrectionSpinBox&&zCorrectionSpinBox)
		execQuery(QString("update pos set dx=%1,dy=%2,dz=%3 where posId=%4").arg(xCorrectionSpinBox->value()).arg(yCorrectionSpinBox->value()).arg(zCorrectionSpinBox->value()).arg(posId),mDb);
	QDialog::accept();
}
void posDlgClass::logImportExportsTable()
{
	if ( (posId==pincer1_50_id)||(posId==pincer1_52_id)||(posId==pincer1_53_id)||(posId==pincer1_54_id) )
	{
		if (destInRadio&&destInRadio->isChecked())
			logImportExportGlobal(getTicket(),getSbNumber(),description,"",logImportTrigger,mDb);
		if (destOutRadio&&destOutRadio->isChecked())
			logImportExportGlobal(getTicket(),getSbNumber(),"",description,logExportTrigger,mDb);
	}
}
void posDlgClass::reject()
{
	QDialog::reject();
}
void posDlgClass::cassetteSpinBoxValueChangedSlot(int c)
{
	updateGui();
}
void posDlgClass::sbSpinBoxValueChangedSlot(int state)
{
	updateGui();
}
void posDlgClass::ticketSpinBoxValueChangedSlot(int value)
{
	updateGui();
}
void posDlgClass::setDestinationWarehouse(bool d)
{
	destInRadio->blockSignals(true);
	destOutRadio->blockSignals(true);
	destInRadio->setChecked(d);
	destOutRadio->setChecked(!d);
	destInRadio->blockSignals(false);
	destOutRadio->blockSignals(false);
	destInRadio->setChecked(d);
}
bool posDlgClass::getIsDestinationWarehouse()
{
	return destInRadio->isChecked();
}
void posDlgClass::destInRadioToggledSlot(bool value)
{
	//if (value)
	//	logImportExportGlobal(getFirst(QString("select ticket from pos where posId=%1").arg(posId),mDb).toInt(),getFirst(QString("select sbNumber from pos where posId=%1").arg(posId),mDb).toInt(),description,"",logImportTrigger,mDb);
}
void posDlgClass::destOutRadioToggledSlot(bool value)
{
	//if (value)
	//	logImportExportGlobal(getFirst(QString("select ticket from pos where posId=%1").arg(posId),mDb).toInt(),getFirst(QString("select sbNumber from pos where posId=%1").arg(posId),mDb).toInt(),"",description,logExportTrigger,mDb);
}
void posDlgClass::updateGui()
{
	if ((!isSbPos)&&(cassetteSpinBox->value()==0))
	{
		sbSpinBox->setValue(0);
		ticketSpinBox->setValue(0);
	}
	if (sbSpinBox->value()==0)
		ticketSpinBox->setValue(0);

	if (destInRadio&&destOutRadio)
	{
		if (sbSpinBox->value()==0)
		{
			destOutRadio->blockSignals(true);
			destOutRadio->setChecked(false);
			destOutRadio->setEnabled(false);
			destOutRadio->blockSignals(false);

			destInRadio->blockSignals(true);
			destInRadio->setChecked(false);
			destInRadio->setEnabled(false);
			destInRadio->blockSignals(false);
		}
		else
		{
			destInRadio->setEnabled(true);
			destOutRadio->setEnabled(true);
		}
	}
	/////
	okBtn->setEnabled(true);
	if (!isSbPos)
	{
		int posOfCassette=getPosIdOfCassetteGlobal(cassetteSpinBox->value(),mDb);
		if ((cassetteSpinBox->value()!=0)&&(posOfCassette!=0)&&(posOfCassette!=posId))
		{
			QString posOfCassetteDesc=getDescriptionOfPosIdGlobal(posOfCassette,mDb);
			cassetteInfoLabel->setText(tr("Cassette %1 is on position \"%2\" (id:%3)").arg(cassetteSpinBox->value()).arg(posOfCassetteDesc).arg(posOfCassette));
			cassetteInfoLabel->setStyleSheet("QLabel{color:red;}");
			okBtn->setEnabled(false);
		}
		else
		{
			cassetteInfoLabel->setText(tr(""));
			cassetteInfoLabel->setStyleSheet("QLabel{color:blue;}");
		}
	}
	//sb
	int posOfSb=getPosIdOfSbGlobal(sbSpinBox->value(),mDb);
	if ((sbSpinBox->value()!=0)&&(posOfSb!=0)&&(posOfSb!=posId))
	{
		QString posOfSbDesc=getDescriptionOfPosIdGlobal(posOfSb,mDb);
		sbInfoLabel->setText(tr("SB %1 is on position \"%2\" (id:%3)").arg(sbSpinBox->value()).arg(posOfSbDesc).arg(posOfSb));
		sbInfoLabel->setStyleSheet("QLabel{color:red;}");
		okBtn->setEnabled(false);
	}
	else
	{
		sbInfoLabel->setText(tr(""));
		sbInfoLabel->setStyleSheet("QLabel{color:blue;}");
	}
	//ticket
	int posOfTicket=getPosIdOfTicketGlobal(ticketSpinBox->value(),mDb);
	if ((ticketSpinBox->value()!=0)&&(posOfTicket!=0)&&(posOfTicket!=posId))
	{
		QString posOfTicketDesc=getDescriptionOfPosIdGlobal(posOfTicket,mDb);
		ticketInfoLabel->setText(tr("Ticket %1 is on position \"%2\" (id:%3)").arg(ticketSpinBox->value()).arg(posOfTicketDesc).arg(posOfTicket));
		ticketInfoLabel->setStyleSheet("QLabel{color:red;}");
		okBtn->setEnabled(false);
	}
	else
	{
		ticketInfoLabel->setText(tr(""));
		ticketInfoLabel->setStyleSheet("QLabel{color:blue;}");
	}
}
void posDlgClass::readyReadSerialPortSlot()
{
	QString str=serialPort->readAll();
	int ticket=str.toInt();
	ticketSpinBox->setValue(ticket);
	updateGui();
}

#include "pch.h"
#include "sbWarehouse.h"
extern bool makeTicketSpinReadOnly;
sbWarehouseClass::sbWarehouseClass(tuxipClass *tuxipConnection_,QWidget *p):QWidget(p)
{
	tuxipConnection=tuxipConnection_;
	QVBoxLayout *mainL=new QVBoxLayout();mainL->setMargin(2);QWidget::setLayout(mainL);
	//
	mainL->addWidget(new QLabel(tr("Sb number:")));
	sbSpinBox=new QSpinBox();mainL->addWidget(sbSpinBox);
	sbSpinBox->setMinimum(0);
	sbSpinBox->setMaximum(INT_MAX);
	sbSpinBox->setValue(0);
	connect(sbSpinBox,SIGNAL(valueChanged(int)),this,SLOT(sbSpinBoxChangedSlot(int)));
	mainL->addWidget(sbInfoLabel=new QLabel(tr("")));
	sbInfoLabel->setMinimumWidth(125);

	mainL->addWidget(new QLabel(tr("Ticket/Rack number:")));
	ticketSpinBox=new QSpinBox();mainL->addWidget(ticketSpinBox);
	ticketSpinBox->setMinimum(0);
	ticketSpinBox->setMaximum(INT_MAX);
	ticketSpinBox->setValue(0);
	connect(ticketSpinBox,SIGNAL(valueChanged(int)),this,SLOT(ticketSpinBoxChangedSlot(int)));

	mainL->addWidget(ticketInfoLabel=new QLabel(tr("")));
	ticketInfoLabel->setMinimumWidth(125);

	okBtn=new QPushButton(tr("Ok"),this);
	connect(okBtn,SIGNAL(clicked()),this,SLOT(okButtonClicledSlot()));
	mainL->addWidget(okBtn);

	exportFront=new exportStationWidgetClass(exportsFrontTable,tr("Exports to destacker"),NULL,-1,NULL,true,this);
	mainL->addWidget(exportFront);
	mainL->addStretch(1);

	QString portStr="COM1";
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
sbWarehouseClass::~sbWarehouseClass()
{
}
void sbWarehouseClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane2Ip)
	{
		if (index==(pincer1_53_id-craneId))
		{
			if (table=="posSb")
				sbSpinBox->setValue(value.toInt());
			if (table=="posTicket")
				ticketSpinBox->setValue(value.toInt());
		}
	}
}
void sbWarehouseClass::readyReadSerialPortSlot()
{
	QString str=serialPort->readAll();
	int ticket=str.toInt();
	ticketSpinBox->setValue(ticket);
	updateGui();
}

void sbWarehouseClass::updateGui()
{
	if (sbSpinBox->value()==0)
		ticketSpinBox->setValue(0);
	/////
	okBtn->setEnabled(true);
	int posOfSb=getPosIdOfSbGlobal(sbSpinBox->value(),mDb);
	if ((sbSpinBox->value()!=0)&&(posOfSb!=0)&&(posOfSb!=(pincer1_53_id)))
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
	if ((ticketSpinBox->value()!=0)&&(posOfTicket!=0)&&(posOfTicket!=(pincer1_53_id)))
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
void sbWarehouseClass::sbSpinBoxChangedSlot(int b)
{
	updateGui();
}
void sbWarehouseClass::ticketSpinBoxChangedSlot(int b)
{
	updateGui();
}
void sbWarehouseClass::okButtonClicledSlot()
{
	tuxipConnection->writeInteger("posSb",54,sbSpinBox->value());
	tuxipConnection->writeInteger("posTicket",54,ticketSpinBox->value());
	tuxipConnection->writeBit("rConveyorState",54,23,true);
}

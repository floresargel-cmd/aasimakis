#include "pch.h"
#include "serialPortDlg.h"
serialPortDlgClass::serialPortDlgClass(tuxipClass *forkTuxip_,tuxipClass *oldStrappingTux_,QString comStr,QWidget *p):QDialog(p)
{
	forkTuxip=forkTuxip_;
	oldStrappingTux=oldStrappingTux_;
	setWindowIcon(QIcon(":/assimakis.png"));
	setWindowTitle(tr("Serial log"));
	resize(300,500);
	QVBoxLayout *mainL=new QVBoxLayout();mainL->setMargin(0);setLayout(mainL);
///
	logTable=new QTableWidget();mainL->addWidget(logTable);
	logTable->setAlternatingRowColors(true);
	logTable->horizontalHeader()->setStretchLastSection(true);
	logTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	logTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	logTable->setColumnCount(2);
	logTable->setHorizontalHeaderLabels(QStringList()<<tr("Time")<<tr("Message"));
	logTable->setWordWrap (true);
	//
	readTimer=new QTimer(this);
	connect(readTimer,SIGNAL(timeout()),this,SLOT(readTimerTriggeredSlot()));

	serialPort=new QSerialPort(this);
	serialPort->setPortName(comStr);
	serialPort->setBaudRate(QSerialPort::Baud9600);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);
	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setStopBits(QSerialPort::OneStop);
	if (!serialPort->open(QIODevice::ReadWrite))
	{
		QString errStr=tr("Error opening serial port %1.\nError:%2").arg(comStr).arg(serialPort->errorString());
		QMessageBox::critical(this, tr("Error"),errStr);
	}
	connect(serialPort,SIGNAL(readyRead()),this,SLOT(readyReadSerialPortSlot()));
	connect(serialPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(errorOccurredSlot(QSerialPort::SerialPortError)));

	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerTriggeredSlot()));
	mainTimer->start(1000);
}
serialPortDlgClass::~serialPortDlgClass()
{
}
void serialPortDlgClass::setTypeEnum(serialPortDlgClass::typeEnum type_)
{
	type=type_;
}
serialPortDlgClass::typeEnum serialPortDlgClass::getTypeEnum()
{
	return type;
}
QString serialPortDlgClass::getName()
{
	if (type==typeEnum::none)return QString("None");
	else if (type==typeEnum::formingStation1)return QString("Forming Station 1");
	else if (type==typeEnum::formingStation2)return QString("Forming Station 2");
	else if (type==typeEnum::strappingOldExit)return QString("Strapping Old Exit");
	else if (type==typeEnum::strappingNewExit)return QString("Strapping New Exit");
	else if (type==typeEnum::inputOutputStation)return QString("Input Output Station");
	return QString("None");
}
void serialPortDlgClass::mainTimerTriggeredSlot()
{
}
void serialPortDlgClass::log(QString str)
{
	execQuery(QString("insert into packingForkBarcodeScansLog (message,dateStr) values ('%1','%2')").arg(str).arg(timeForFile),pDb);
	logTable->insertRow(0);
	logTable->setItem(0,0,new QTableWidgetItem(timeVerbose));
	logTable->setItem(0,1,new QTableWidgetItem(str));
	if (logTable->rowCount()>100)
	{
		for (int i=0;i<(logTable->rowCount()-80);i++)
			logTable->removeRow(logTable->rowCount()-1);
	}
	logTable->scrollToTop();
}
void serialPortDlgClass::errorOccurredSlot(QSerialPort::SerialPortError serialPortError)
{
}
void serialPortDlgClass::readyReadSerialPortSlot()
{
	readTimer->start(50);
}
void serialPortDlgClass::newScan(long scanId)
{
	if ( (type==typeEnum::formingStation1)||(type==typeEnum::formingStation2))
	{
		QVector<QVariant> lengthV=execQuery(QString("select pcLen from RackDetail where RackID=%1").arg(scanId),eDb);
		if (lengthV.count()>1)
		{
			QMessageBox::critical(this,"Error",tr("Epics returned more than one Lengths for %1").arg(scanId));
			return;
		}
		int length=0;
		if (lengthV.count()==1)
			length=lengthV[0].toFloat()*1000;
		log(tr("%1:%2mm (%3)").arg(scanId).arg(length).arg(getName()));
		if (type==typeEnum::formingStation1)
		{
			forkTuxip->writeInteger("packetId",packetFormingStation1PlcId,scanId);
			forkTuxip->writeInteger("packetLength",packetFormingStation1PlcId,length);
		}
		if (type==typeEnum::formingStation2)
		{
			forkTuxip->writeInteger("packetId",packetFormingStation2PlcId,scanId);
			forkTuxip->writeInteger("packetLength",packetFormingStation2PlcId,length);
		}
	}
	if (type==typeEnum::strappingOldExit)
	{
		log(tr("%1 (%3)").arg(scanId).arg(getName()));
		oldStrappingTux->writeFloat("F163",11,scanId);
	}
	if (type==typeEnum::strappingNewExit)
	{
		log(tr("%1 (%3)").arg(scanId).arg(getName()));
		forkTuxip->writeInteger("packetId",strappingNewOutPlcId,scanId);
	}
}
void serialPortDlgClass::readTimerTriggeredSlot()
{
	serialData.append(serialPort->readAll());
	if (serialData.contains('\n'))
	{
		serialData=serialData.remove('\n').remove('\r');
		long rackId=serialData.left(6).toInt();
		serialData.clear();
		readTimer->stop();
		newScan(rackId);
	}
}

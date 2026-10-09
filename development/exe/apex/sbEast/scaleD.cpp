#include "pch.h"
#include "scaleD.h"
scaleDlgClass::scaleDlgClass(tuxipClass *tuxip_,QWidget *p):QDialog(p)
{
	tuxip=tuxip_;
	setWindowIcon(QIcon(":/assimakis.png"));
	setWindowTitle(tr("Scale log"));
	resize(300,500);
	currentWeight=0.f;
	station1NoSbReadyToReturn=station1WithSbReadyToReturn=station2NoSbReadyToReturn=station2WithSbReadyToReturn=false;
	myAction=NULL;
	scaleIsOn=false;
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);setLayout(mainLayout);

	QToolBar *mainToolBar=new QToolBar(tr("Tools"));mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
	mainLayout->addWidget(mainToolBar);
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	resetScaleAction=new QAction(QIcon(":/reset.png"),tr("Reset"),this);
	resetScaleAction->setToolTip(tr("Signals scale to reset its value."));
	resetScaleAction->setStatusTip(tr("Signals scale to reset its value."));
	connect(resetScaleAction,SIGNAL(triggered()),this,SLOT(resetScaleActionTriggeredSlot()));
	mainToolBar->addAction(resetScaleAction);
	///
	logTable=new QTableWidget();mainLayout->addWidget(logTable);
	logTable->setAlternatingRowColors(true);
	logTable->horizontalHeader()->setStretchLastSection(true);
	logTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	logTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	logTable->setColumnCount(2);
	logTable->setHorizontalHeaderLabels(QStringList()<<tr("Time")<<tr("Message"));
	logTable->setWordWrap (true);
	//
	readTimer=new QTimer(this);
	connect(readTimer,SIGNAL(timeout()),this,SLOT(readTimerSlot()));

	serialPort=new QSerialPort(this);
	serialPort->setPortName("COM3");
	serialPort->setBaudRate(QSerialPort::Baud9600);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);
	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setStopBits(QSerialPort::OneStop);
	if (!serialPort->open(QIODevice::ReadWrite))
	{
		QString errStr=tr("Error opening serial port %1.\nError:%2").arg(serialPort->portName()).arg(serialPort->errorString());
		QMessageBox::critical(this, tr("Error"),errStr);
		log(errStr);
	}
	QTimer *getWeightTimer=new QTimer(this);
	connect(getWeightTimer,SIGNAL(timeout()),this,SLOT(getWeightTimerSlot()));
	getWeightTimer->start(1000);
	connect(serialPort,SIGNAL(readyRead()),this,SLOT(readyReadSerialPortSlot()));
	connect(serialPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(errorOccurredSlot(QSerialPort::SerialPortError)));
//
	QTimer *checkScaleZeroTimer=new QTimer(this);
	connect(checkScaleZeroTimer,SIGNAL(timeout()),this,SLOT(checkScaleZeroTimerSlot()));
	checkScaleZeroTimer->start(1000);

	checkScaleIsOnTimer=new QTimer(this);
	checkScaleIsOnTimer->setSingleShot(true);
	connect(checkScaleIsOnTimer,SIGNAL(timeout()),this,SLOT(checkScaleIsOnTimerSlot()));

	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer->start(1000);
}
scaleDlgClass::~scaleDlgClass()
{
}
void scaleDlgClass::mainTimerSlot()
{
	double cassetteWeigth=68.5;
	double cassetteAndSb=150.;
	if (station1NoSbReadyToReturn)
	{
		if (abs(getCurrentWeight()-cassetteWeigth)<10.)
			tuxip->writeInteger("pcCom",9,932);
		else
			tuxip->writeInteger("pcCom",9,0);
	}
	if (station1WithSbReadyToReturn)
	{
		if (getCurrentWeight()>cassetteAndSb)
			tuxip->writeInteger("pcCom",9,932);
		else
			tuxip->writeInteger("pcCom",9,0);
	}
	if (station2NoSbReadyToReturn)
	{
		if (abs(getCurrentWeight()-cassetteWeigth)<10.)
			tuxip->writeInteger("pcCom",9,1332);
		else
			tuxip->writeInteger("pcCom",9,0);
	}
	if (station2WithSbReadyToReturn)
	{
		if (getCurrentWeight()>cassetteAndSb)
			tuxip->writeInteger("pcCom",9,1332);
		else
			tuxip->writeInteger("pcCom",9,0);
	}
}
void scaleDlgClass::setMyAction(QAction *myAction_)
{
	myAction=myAction_;
	checkScaleIsOnTimerSlot();
}
void scaleDlgClass::log(QString str)
{
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
void scaleDlgClass::errorOccurredSlot(QSerialPort::SerialPortError serialPortError)
{
	log(serialPort->errorString());
}
void scaleDlgClass::resetScaleActionTriggeredSlot()
{
	if (!serialPort->isOpen())
		return;
	char makeZero[11]{0x44,0x30,0x31,0x4B,0x5A,0x0D,0x0A,0x00,0x00,0x00,0x00};
	serialPort->write(makeZero);
	log(tr("Reset scale"));
}
void scaleDlgClass::getWeightTimerSlot()
{
	if (!serialPort->isOpen())
		return;
	//char weightRequest[11]{0x44,0x30,0x31,0x4B,0x57,0x0D,0x0A,0x00,0x00,0x00,0x00};
	char weightRequest[11]{0x44,0x30,0x30,0x4B,0x57,0x0D,0x0A,0x00,0x00,0x00,0x00};
	qint64 ans0=serialPort->write(weightRequest);
}
void scaleDlgClass::readyReadSerialPortSlot()
{
	readTimer->start(50);
}
void scaleDlgClass::readTimerSlot()
{
	serialData.append(serialPort->readAll());
	if ((!serialData.isEmpty())&&(serialData[serialData.length()-1]=='\n'))
	{
		log(serialData);
		float oldWeight=currentWeight;
		currentWeight=serialData.toFloat();
		scaleIsOn=true;
		checkScaleIsOnTimer->start(2000);
		serialData.clear();
		readTimer->stop();
		if (myAction)
			myAction->setText(tr("Scale: %1 Kgr").arg(currentWeight));
		emit currentWeightChangedSignal();
	}
}
void scaleDlgClass::checkScaleIsOnTimerSlot()
{
	scaleIsOn=false;
	myAction->setText(tr("Error reading scale"));
	emit currentWeightChangedSignal();
}
float scaleDlgClass::getCurrentWeight()
{
	return scaleIsOn?currentWeight:-99999;
}
bool scaleDlgClass::getScaleIsOn()
{
	return scaleIsOn;
}
void scaleDlgClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (table=="rConveyorState")
	{
		if (index==9)
		{
			station1NoSbReadyToReturn=getValueOfBit(value.toInt(),20);
			station1WithSbReadyToReturn=getValueOfBit(value.toInt(),21);
		}
		if (index==13)
		{
			station2NoSbReadyToReturn=getValueOfBit(value.toInt(),20);
			station2WithSbReadyToReturn=getValueOfBit(value.toInt(),21);
		}
	}
}
void scaleDlgClass::checkScaleZeroTimerSlot()
{
	if (!scaleIsOn)
		return;
}

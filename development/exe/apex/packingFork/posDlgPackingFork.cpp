#include "pch.h"
#include "posDlgPackingFork.h"
posDlgPackingForkClass::posDlgPackingForkClass(QString title,int ticketId,bool isForPacketId_,bool isForRackId_,QWidget *p):QDialog(p)
{
	isForPacketId=isForPacketId_;
	isForRackId=isForRackId_;
	//
	setWindowTitle(title);
	QVBoxLayout *mainL=new QVBoxLayout();QDialog::setLayout(mainL);
	QGridLayout *dataL=new QGridLayout();dataL->setMargin(2);mainL->addLayout(dataL);
	QHBoxLayout *btnsL=new QHBoxLayout();btnsL->setMargin(2);mainL->addLayout(btnsL);
	//dataL
	int row=0;
	dataL->addWidget(new QLabel(tr("Packet:")),row,0);
	dataL->addWidget(packetSpinBox=new QSpinBox(),row,1);
	packetSpinBox->setMinimum(0);
	if (isForRackId)packetSpinBox->setMaximum(maxRackId);
	if (isForPacketId)packetSpinBox->setMaximum(maxTicketIdId);
	packetSpinBox->setValue(ticketId);
	dataL->addWidget(packetInfoLabel=new QLabel(tr("")),row,2);packetInfoLabel->setMinimumWidth(200);
	row++;
	dataL->addWidget(new QLabel(tr("Length:")),row,0);
	dataL->addWidget(lengthSpinBox=new QSpinBox(),row,1);
	lengthSpinBox->setMinimum(0);
	lengthSpinBox->setMaximum(bigLength);
	if (isForPacketId)
	{
		row++;
		dataL->addWidget(new QLabel(tr("Woods:")),row,0);
		dataL->addWidget(woodsSpinBox=new QSpinBox(),row,1);
		woodsSpinBox->setMinimum(0);
		woodsSpinBox->setMaximum(5);
		row++;
		dataL->addWidget(new QLabel(tr("Width [maximum 585mm]:")),row,0);
		dataL->addWidget(widthSpinBox=new QSpinBox(),row,1);
		widthSpinBox->setMinimum(1);
		widthSpinBox->setMaximum(585);
		row++;
		dataL->addWidget(new QLabel(tr("Height [maximum 480mm]:")),row,0);
		dataL->addWidget(heightSpinBox=new QSpinBox(),row,1);
		heightSpinBox->setMinimum(1);
		heightSpinBox->setMaximum(480);
		row++;
		dataL->addWidget(new QLabel(tr("Weight:")),row,0);
		dataL->addWidget(weightSpinBox=new QSpinBox(),row,1);
		weightSpinBox->setMinimum(1);
		weightSpinBox->setMaximum(900);
	}
	//btnsL
	okBtn=new QPushButton(tr("Ok"),this);
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	btnsL->addWidget(okBtn);
	if (isForPacketId)
		updateFromDatabase();
	//
	connect(packetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(packetSpinBoxValueChangedSlot(int)));
	connect(packetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(lengthSpinBoxValueChangedSlot(int)));
	connect(packetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(widthSpinBoxValueChangedSlot(int)));
	connect(packetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(heightSpinBoxValueChangedSlot(int)));
	connect(packetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(woodsSpinBoxValueChangedSlot(int)));
	connect(packetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(weightSpinBoxValueChangedSlot(int)));
}
posDlgPackingForkClass::~posDlgPackingForkClass()
{
}
void posDlgPackingForkClass::setData(int packetId,int packetLength,int packetWoods,int packetWidth,int packetHeight,int packetWeigt)
{
	packetSpinBox->setValue(packetId);
	lengthSpinBox->setValue(packetLength);
	if (widthSpinBox)woodsSpinBox->setValue(packetWoods);
	if (widthSpinBox)widthSpinBox->setValue(packetWidth);
	if (heightSpinBox)heightSpinBox->setValue(packetHeight);
	if (weightSpinBox)weightSpinBox->setValue(packetWeigt);
}
void posDlgPackingForkClass::updateFromDatabase()
{
	bool isRackId=(packetSpinBox->value()>=minRackId)&&(packetSpinBox->value()<=maxRackId);
	bool isPacketId=(packetSpinBox->value()>minTicketIdId)&&(packetSpinBox->value()<maxTicketIdId);
	if (isForPacketId&&isPacketId)
	{
		QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select length,woods,width,height,weight from packets where packetId=%1").arg(packetSpinBox->value()),pDb);
		if (dataVV.count())
			setData(packetSpinBox->value(),dataVV[0][0].toInt(),dataVV[0][1].toInt(),dataVV[0][2].toInt(),dataVV[0][3].toInt(),dataVV[0][4].toInt());
		else
			setData(packetSpinBox->value(),0,0,0,0,0);
	}
}
int posDlgPackingForkClass::myExec(bool checkPassword)
{
	if ((!checkPassword)||(getPassword("Password is needed to open this window.","Please give the password",passWeak)))
		return QDialog::exec();
	return 0;
}
int posDlgPackingForkClass::exec()
{
	return QDialog::exec();
}
void posDlgPackingForkClass::accept()
{
	if (isForPacketId)
	{
		if (getFirst(QString("select tid from packets where packetId=%1").arg(packetSpinBox->value()),pDb).toInt()==0)
			execQuery(QString("insert into packets (packetId,sb) values(%1,1)").arg(packetSpinBox->value()),pDb);
		execQuery(QString("update packets set length=%1,width=%2,height=%3,woods=%4,weight=%5,sb=1 where packetId=%6").arg(lengthSpinBox->value()).arg(widthSpinBox->value()).arg(heightSpinBox->value()).arg(woodsSpinBox->value()).arg(weightSpinBox->value()).arg(packetSpinBox->value()),pDb);
	}
	QDialog::accept();
}
void posDlgPackingForkClass::reject()
{
	QDialog::reject();
}
void evableSpin(QSpinBox *spin,bool enable)
{
	if (!spin)
		return;
	if (!enable)
		spin->setValue(0);
	spin->setEnabled(enable);
}
void posDlgPackingForkClass::packetSpinBoxValueChangedSlot(int packet)
{
	bool isRackId=(packetSpinBox->value()>=minRackId)&&(packetSpinBox->value()<=maxRackId);
	bool isPacketId=(packetSpinBox->value()>minTicketIdId)&&(packetSpinBox->value()<maxTicketIdId);
	bool isPacketOk=true;
	if (isPacketId)
	{
		int packetWithPos=getFirst(QString("select posId from positions where packetId=%1").arg(packet),pDb).toInt();
		isPacketOk=((packetWithPos==0)||(packetWithPos==positionInputOutputPosId));
		packetInfoLabel->setText(packetWithPos==0?"":tr("Packet in position %1").arg(packetWithPos));
	}
	else
		packetInfoLabel->setText("");
	evableSpin(lengthSpinBox,isRackId||(isPacketId&&isPacketOk));
	evableSpin(widthSpinBox	,(isPacketId&&isPacketOk));
	evableSpin(heightSpinBox,(isPacketId&&isPacketOk));
	evableSpin(woodsSpinBox	,(isPacketId&&isPacketOk));
	evableSpin(weightSpinBox,(isPacketId&&isPacketOk));
	if (isRackId)
	{
		if (widthSpinBox)widthSpinBox->setValue(0);
		if (heightSpinBox)heightSpinBox->setValue(0);
		if (woodsSpinBox)woodsSpinBox->setValue(0);
		if (weightSpinBox)weightSpinBox->setValue(0);
	}
#ifndef packingStrapingExe
	if (isRackId)
		lengthSpinBox->setValue(getFirst(QString("select pcLen from RackDetail where RackID=%1").arg(packetSpinBox->value()),eDb).toFloat()*1000);
#endif
	okBtn->setEnabled((packet==0)||isRackId||(isPacketId&&isPacketOk));
	updateFromDatabase();
}
void posDlgPackingForkClass::lengthSpinBoxValueChangedSlot(int length)
{
}
void posDlgPackingForkClass::widthSpinBoxValueChangedSlot(int width)
{
}
void posDlgPackingForkClass::heightSpinBoxValueChangedSlot(int height)
{
}
void posDlgPackingForkClass::woodsSpinBoxValueChangedSlot(int woods)
{
}
void posDlgPackingForkClass::weightSpinBoxValueChangedSlot(int weight)
{
}
int posDlgPackingForkClass::getPacketTicket()
{
	return packetSpinBox->value();
}
int posDlgPackingForkClass::getPacketLength()
{
	return lengthSpinBox->value();
}
int posDlgPackingForkClass::getPacketWoods()
{
	return woodsSpinBox->value();
}
int posDlgPackingForkClass::getPacketWidth()
{
	return widthSpinBox->value();
}
int posDlgPackingForkClass::getPacketHeight()
{
	return heightSpinBox->value();
}

#include "pch.h"
#include "bCont.h"
cassetteContentsClass::cassetteContentsClass(bool canChangeContents_,QWidget *w):QWidget(w)
{
	canChangeContents=canChangeContents_;
	cassetteNum=0;
	QVBoxLayout *mainLayout=new QVBoxLayout(this);mainLayout->setMargin(0);setLayout(mainLayout);
	QHBoxLayout *packetLayout=new QHBoxLayout();packetLayout->setMargin(2);mainLayout->addLayout(packetLayout);
	QHBoxLayout *btnsLayout=new QHBoxLayout();btnsLayout->setMargin(2);mainLayout->addLayout(btnsLayout);
	mainLayout->addStretch(1);
	//packetLayout
	packetSpin=new QSpinBox();packetLayout->addWidget(packetSpin);
	packetSpin->setMinimum(0);
	packetSpin->setMaximum(INT_MAX);
	connect(packetSpin,SIGNAL(valueChanged(int)),this,SLOT(packetSpinBoxValueChangedSlot(int)));
	packetPositionLabel=new QLabel();packetLayout->addWidget(packetPositionLabel);
	//btnsLayout
	saveBtn=new QPushButton(QIcon(":/save.png"),tr("Save"),this);
	connect(saveBtn,SIGNAL(clicked()),this,SLOT(saveBtnClickedSlot()));
	btnsLayout->addWidget(saveBtn);
	packetSpin->setEnabled(canChangeContents);
	saveBtn->setEnabled(canChangeContents);
	checkData();

}
cassetteContentsClass::~cassetteContentsClass()
{
}
void cassetteContentsClass::updateFromDatabase(int cassetteNum_)
{
	cassetteNum=cassetteNum_;
	if (cassetteNum!=0)
	{
		int packet=getFirst(QString("select packet from cassetteContents where cassette=%1").arg(cassetteNum),mDb).toInt();
		packetSpin->setValue(packet);
	}
	else
		packetSpin->setValue(0);
}
void cassetteContentsClass::saveBtnClickedSlot()
{
	save();
}
void cassetteContentsClass::save()
{
	execQuery(QString("delete from cassetteContents where cassette=%1").arg(cassetteNum),mDb);
	if ((packetSpin->value()!=0)&&(cassetteNum!=0))
		execQuery(QString("insert into cassetteContents (cassette,packet,isPacketPlacedOnCenter) values (%1,%2,1)").arg(cassetteNum).arg(packetSpin->value()),mDb);
	updateFromDatabase(cassetteNum);
}
bool cassetteContentsClass::checkData()
{
	bool isPacketOk=false;
	int cassetteOfPacket=getFirst(QString("select cassette from cassetteContents where cassette<>%1 and packet=%2").arg(cassetteNum).arg(packetSpin->value()),mDb).toInt();
	bool isDefined=true;//(getFirst(QString("select code from dimension where code=%1").arg(packetSpin->value()),mDb).toInt()==packetSpin->value());
	isPacketOk=(cassetteNum!=0)&&(cassetteOfPacket==0)&&isDefined;
	saveBtn->setEnabled(canChangeContents&&isPacketOk);
	if (cassetteOfPacket!=0)
		packetPositionLabel->setText(tr("Packet %1 is in cassette %2").arg(packetSpin->value()).arg(cassetteOfPacket));
	else if (!isDefined)
		packetPositionLabel->setText(tr("Packet %1 is not defined").arg(packetSpin->value()));
	else
		packetPositionLabel->setText(tr("OK"));
	return isPacketOk;
}
void cassetteContentsClass::packetSpinBoxValueChangedSlot(int v)
{
	int packet=v;
	checkData();
}
void cassetteContentsClass::setPacketSpinBoxValue(int v)
{
	packetSpin->setValue(v);
}
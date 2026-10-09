#include "mMissionsP.h"

manualMissionsPacketClass::manualMissionsPacketClass(packetPositionClass *packetPos_):QWidget(packetPos_)
{
	packetPos=packetPos_;
	QHBoxLayout *mainLayout=new QHBoxLayout();this->setLayout(mainLayout);mainLayout->setMargin(1);
	QString name;
	if (packetPos->getPosId()==POS_ID_NEWSB)
		name=tr("New SB");
	if (packetPos->getPosId()==POS_ID_NEWPACKET)
		name=tr("New packet");
	if (packetPos->getPosId()==POS_ID_SB_PACKET_IMPORT)
		name=tr("Import");
	mainLayout->addWidget(new QLabel(tr("%1 (%2)").arg(name).arg(packetPos->getPositionIdForForkPlc())));
	packetSpin=new QSpinBox();
	packetSpin->setMinimum(0);
	packetSpin->setMaximum(INT_MAX);
	mainLayout->addWidget(packetSpin);
	mainLayout->addStretch(1);
	connect(packetSpin,SIGNAL(valueChanged(int)),this,SLOT(basketSpinBoxValueChangedSlot(int)));
	updateGui();
}
void manualMissionsPacketClass::updateGui()
{
	packetSpin->setValue(packetPos->getPacketId());
}
void manualMissionsPacketClass::basketSpinBoxValueChangedSlot(int p)
{
	bool isDefined=((packetSpin->value()==0)||(getFirst(QString("select woods from packets where packetId=%1").arg(p),MAIN_DB_CONNECTION).toInt()!=0));
	if (isDefined)
		packetSpin->setStyleSheet("QSpinBox{color:#33aa33;}");
	else
		packetSpin->setStyleSheet("QSpinBox{color:#aa3333;}");
}
void manualMissionsPacketClass::saveToDataBase()
{
	bool isDefined=((packetSpin->value()==0)||(getFirst(QString("select woods from packets where packetId=%1").arg(packetSpin->value()),MAIN_DB_CONNECTION).toInt()!=0));
	if (isDefined)
		packetPos->setPacketId(packetSpin->value(),false);
	updateGui();
}
//////////////////////////
manualMissionsCellClass::manualMissionsCellClass(cellPositionClass *cell_):QWidget(cell_)
{
	cell=cell_;
	QGridLayout *mainLayout=new QGridLayout();this->setLayout(mainLayout);mainLayout->setMargin(1);
	mainLayout->addWidget(new QLabel(tr("%1").arg(cell->getPosId())),0,0);
	QPushButton *changeDataBtn=new QPushButton(QIcon(":/reset.png"),tr(""),this);
	connect(changeDataBtn,SIGNAL(clicked()),this,SLOT(changeDataBtnClickedSlot()));
	mainLayout->addWidget(changeDataBtn,0,1);
	//
	casseteLabel=new QLabel("");
	mainLayout->addWidget(casseteLabel,1,0);
	packetLabel=new QLabel("");
	mainLayout->addWidget(packetLabel,1,1);
	//

}
void manualMissionsCellClass::updateGui()
{
	casseteLabel->setText(tr("%1").arg(cell->getCassette()));
	packetLabel->setText(tr("%1").arg(cell->getPacketId()));
}
void manualMissionsCellClass::changeDataBtnClickedSlot()
{
	cell->showDlg();
}

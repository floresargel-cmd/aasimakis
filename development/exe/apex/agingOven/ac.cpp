#include "ac.h"
autoCircleGroupClass::autoCircleGroupClass(tcpServerClass *tcpServer_,QWidget *p):QGroupBox(tr("Auto cycles"),p)
{
	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);
	QVBoxLayout *layout01=new QVBoxLayout();mainLayout->addLayout(layout01);
	QVBoxLayout *layout02=new QVBoxLayout();mainLayout->addLayout(layout02);
	QVBoxLayout *layout03=new QVBoxLayout();mainLayout->addLayout(layout03);
	QVBoxLayout *layout04=new QVBoxLayout();mainLayout->addLayout(layout04);
	//
	//layout01
	statusLabel01=new statusLabel(tr("Load conveyor to oven conveyor (24 ft)"								 ),this);layout01->addWidget(statusLabel01);
	statusLabel02=new statusLabel(tr("Load conveyor to oven conveyor (30 ft)"								 ),this);layout01->addWidget(statusLabel02);
	statusLabel03=new statusLabel(tr("Oven conveyor to unload conveyor (24 ft)"							 ),this);layout01->addWidget(statusLabel03);
	statusLabel04=new statusLabel(tr("Oven conveyor to unload conveyor (30 ft)"							 ),this);layout01->addWidget(statusLabel04);
	statusLabel05=new statusLabel(tr("Unload  conveyor 1 to cross tranfer conveyor 1"				 ),this);layout01->addWidget(statusLabel05);
	statusLabel06=new statusLabel(tr("Unload  conveyor 2 to cross tranfer conveyor 2"				 ),this);layout01->addWidget(statusLabel06);
	layout01->addStretch(1);																																 
	//layout02																																							 
	statusLabel09=new statusLabel(tr("Return conveyor 1 to return conveyor 2 "							 ),this);layout02->addWidget(statusLabel09);
	statusLabel10=new statusLabel(tr("Return conveyor 2 to return conveyor 3 "							 ),this);layout02->addWidget(statusLabel10);
	statusLabel11=new statusLabel(tr("Return conveyor 3 to return conveyor 4 "							 ),this);layout02->addWidget(statusLabel11);
	statusLabel12=new statusLabel(tr("Return conveyor 2 to return conveyor 4 (30 ft) "			 ),this);layout02->addWidget(statusLabel12);
	layout02->addStretch(1);
	//layout03
	statusLabel07=new statusLabel(tr("Cross transfer conveyor 2 to return conveyor 1 (24 ft)"),this);layout03->addWidget(statusLabel07);
	statusLabel08=new statusLabel(tr("Cross transfer conveyor 2 to return conveyor 2 (30 ft)"),this);layout03->addWidget(statusLabel08);
	statusLabel13=new statusLabel(tr("Cross transfer conveyor 1 to return conveyor 1 (24 ft)"),this);layout03->addWidget(statusLabel13);
	statusLabel14=new statusLabel(tr("Cross transfer conveyor 1 to return conveyor 2 (30 ft)"),this);layout03->addWidget(statusLabel14);
	layout03->addStretch(1);
	//layout04
	statusLabel19=new statusLabel(tr("Cross transfer conveyor to oven position"							 ),this);layout04->addWidget(statusLabel19);
	statusLabel20=new statusLabel(tr("Cross transfer conveyor 1 to return position"					 ),this);layout04->addWidget(statusLabel20);
	statusLabel21=new statusLabel(tr("Cross transfer conveyor 2 to return position"					 ),this);layout04->addWidget(statusLabel21);
	layout04->addStretch(1);
	//
}
void autoCircleGroupClass::plcStatusChanged(int index,int value)
{
	if (index==23)
	{
		QVector<bool> bitsVector=deprecatedCalculateBits(value);
		statusLabel01->setStatus(bitsVector[ 1]);
		statusLabel02->setStatus(bitsVector[ 2]);
		statusLabel03->setStatus(bitsVector[ 3]);
		statusLabel04->setStatus(bitsVector[ 4]);
		statusLabel05->setStatus(bitsVector[ 5]);
		statusLabel06->setStatus(bitsVector[ 6]);

		statusLabel09->setStatus(bitsVector[ 9]);
		statusLabel10->setStatus(bitsVector[10]);
		statusLabel11->setStatus(bitsVector[11]);
		statusLabel12->setStatus(bitsVector[12]);

		statusLabel07->setStatus(bitsVector[ 7]);
		statusLabel08->setStatus(bitsVector[ 8]);
		statusLabel13->setStatus(bitsVector[13]);
		statusLabel14->setStatus(bitsVector[14]);

		statusLabel19->setStatus(bitsVector[19]);
		statusLabel20->setStatus(bitsVector[20]);
		statusLabel21->setStatus(bitsVector[21]);
	}
}

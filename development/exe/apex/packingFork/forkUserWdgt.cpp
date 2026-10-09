#include "pch.h"
#include "forkUserWdgt.h"
void forkUserWdgtEnableBtn(QPushButton *btn,bool enable)
{
	if (!btn)
		return;
	btn->setStyleSheet(enable?buttonStyleActive:buttonStyle);
	btn->setEnabled(enable);
}
forkUserWdgtClass::forkUserWdgtClass(allPosPackingForkClass *allPos,forkPackingClass *fork_,tuxipClass *forkTuxip_,QWidget *p):QWidget(p)
{
	fork=fork_;
	forkTuxip=forkTuxip_;
	QVBoxLayout *mainL=new QVBoxLayout();mainL->setMargin(2);QWidget::setLayout(mainL);
	//Missions
	{
		QGroupBox *groupB=new QGroupBox(tr("Auto Missions"));mainL->addWidget(groupB);
		QVBoxLayout *groupL=new QVBoxLayout();groupL->setMargin(2);groupB->setLayout(groupL);
		autoMissionsCheckBox=new QCheckBox(tr("Enable"));groupL->addWidget(autoMissionsCheckBox);
		connect(autoMissionsCheckBox,SIGNAL(stateChanged(int)),this,SLOT(autoMissionsCheckBoxStateChangedSlot(int)));
	}
	//station 1
	{
		QGroupBox *groupB=new QGroupBox(tr("Station 1"));mainL->addWidget(groupB);
		QVBoxLayout *groupL=new QVBoxLayout();groupL->setMargin(2);groupB->setLayout(groupL);
		groupL->addWidget(forming1PacketLabel=new QLabel());forming1PacketLabel->setStyleSheet(styleLabelGr);
		groupL->addWidget(sendForming1PacketToOldStrappingBtn=new QPushButton(QIcon(":/return.png"),tr("To Strapping")));connect(sendForming1PacketToOldStrappingBtn,SIGNAL(clicked()),this,SLOT(sendForming1PacketToOldStrappingBtnClickedSlot()));
		groupL->addWidget(sendForming1PacketToNewStrappingBtn=new QPushButton(QIcon(":/return.png"),tr("To Wrapping")));connect(sendForming1PacketToNewStrappingBtn,SIGNAL(clicked()),this,SLOT(sendForming1PacketToNewStrappingBtnClickedSlot()));
		groupL->addWidget(sendForming1PacketToInOutBtn=new QPushButton(QIcon(":/return.png"),tr("Out")));connect(sendForming1PacketToInOutBtn,SIGNAL(clicked()),this,SLOT(sendForming1PacketToInOutBtnClickedSlot()));
	}
	//station 2
	{
		QGroupBox *groupB=new QGroupBox(tr("Station 2"));mainL->addWidget(groupB);
		QVBoxLayout *groupL=new QVBoxLayout();groupL->setMargin(2);groupB->setLayout(groupL);
		groupL->addWidget(forming2PacketLabel=new QLabel());forming1PacketLabel->setStyleSheet(styleLabelGr);
		groupL->addWidget(sendForming2PacketToOldStrappingBtn=new QPushButton(QIcon(":/return.png"),tr("To Strapping")));connect(sendForming2PacketToOldStrappingBtn,SIGNAL(clicked()),this,SLOT(sendForming2PacketToOldStrappingBtnClickedSlot()));
		groupL->addWidget(sendForming2PacketToNewStrappingBtn=new QPushButton(QIcon(":/return.png"),tr("To Wrapping")));connect(sendForming2PacketToNewStrappingBtn,SIGNAL(clicked()),this,SLOT(sendForming2PacketToNewStrappingBtnClickedSlot()));
		groupL->addWidget(sendForming2PacketToInOutBtn=new QPushButton(QIcon(":/return.png"),tr("Out")));connect(sendForming2PacketToInOutBtn,SIGNAL(clicked()),this,SLOT(sendForming2PacketToInOutBtnClickedSlot()));
	}
	//Strapping Old
	{
		QGroupBox *groupB=new QGroupBox(tr("Strapping"));mainL->addWidget(groupB);
		QVBoxLayout *groupL=new QVBoxLayout();groupL->setMargin(2);groupB->setLayout(groupL);
		groupL->addWidget(strappingOldPacketDefinedLabel=new QLabel());
		groupL->addWidget(strappingOldPlcReadyLabel=new QLabel());
		groupL->addWidget(sendStrappingOldOutPacketToInOutBtn=new QPushButton(QIcon(":/return.png"),tr("Out")));connect(sendStrappingOldOutPacketToInOutBtn,SIGNAL(clicked()),this,SLOT(sendStrappingOldOutPacketToInOutBtnClickedSlot()));
	}
	//Strapping New
	{
		QGroupBox *groupB=new QGroupBox(tr("Wrapping"));mainL->addWidget(groupB);
		QVBoxLayout *groupL=new QVBoxLayout();groupL->setMargin(2);groupB->setLayout(groupL);
		groupL->addWidget(strappingNewPacketDefinedLabel=new QLabel());
		groupL->addWidget(strappingNewPlcReadyLabel=new QLabel());
		groupL->addWidget(sendStrappingNewOutPacketToInOutBtn=new QPushButton(QIcon(":/return.png"),tr("Out")));connect(sendStrappingNewOutPacketToInOutBtn,SIGNAL(clicked()),this,SLOT(sendStrappingNewOutPacketToInOutBtnClickedSlot()));
	}
	//station In/Out
	{
		QGroupBox *groupB=new QGroupBox(tr("Station In/Out"));mainL->addWidget(groupB);
		QVBoxLayout *groupL=new QVBoxLayout();groupL->setMargin(2);groupB->setLayout(groupL);
		groupL->addWidget(inOutPacketDefinedLabel=new QLabel());
		groupL->addWidget(inOutPlcReadyToGetLabel=new QLabel());
		groupL->addWidget(inOutPlcReadyToSendLabel=new QLabel());
		groupL->addWidget(sendInOutPacketToWarehouseBtn		=new QPushButton(QIcon(":/return.png"),tr("To Warehouse")));connect(sendInOutPacketToWarehouseBtn		,SIGNAL(clicked()),this,SLOT(sendInOutPacketToWarehouseBtnClickedSlot()));
		groupL->addWidget(sendInOutPacketToOldStrappingBtn=new QPushButton(QIcon(":/return.png"),tr("To Strapping")));connect(sendInOutPacketToOldStrappingBtn,SIGNAL(clicked()),this,SLOT(sendInOutPacketToOldStrappingBtnClickedSlot()));
		groupL->addWidget(sendInOutPacketToNewStrappingBtn=new QPushButton(QIcon(":/return.png"),tr("To Wrapping")));connect(sendInOutPacketToNewStrappingBtn,SIGNAL(clicked()),this,SLOT(sendInOutPacketToNewStrappingBtnClickedSlot()));
		groupL->addWidget(clearPacketInInOutBtn=new QPushButton(QIcon(":/delete.png"),tr("Clear Packet")));connect(clearPacketInInOutBtn,SIGNAL(clicked()),this,SLOT(clearPacketInInOutBtnClickedSlot()));
	}
	//HCA
	{
		QGroupBox *groupB=new QGroupBox(tr("HCA"));mainL->addWidget(groupB);
		QVBoxLayout *groupL=new QVBoxLayout();groupL->setMargin(2);groupB->setLayout(groupL);
		bitVector<<new bitClass("pcCom",16,0,bitClass::TYPE_CHANGE_COLOR,tr("Crane Ok"),tr("Crane Not Ok"),styleLabelG,styleLabelO,"","","","",this);groupL->addWidget(bitVector.last());
		bitVector<<new bitClass("ConveyorState",40,8,bitClass::TYPE_CHANGE_COLOR,tr("Conveyors Ok"),tr("Conveyors Not Ok"),styleLabelG,styleLabelO,"","","","",this);groupL->addWidget(bitVector.last());
		groupL->addWidget(craneMissionLabel=new QLabel());craneMissionLabel->setStyleSheet(styleLabelB);
	}
	packetFormingStation1Pos=allPos->getPosWithPlcId(packetFormingStation1PlcId);
	packetFormingStation2Pos=allPos->getPosWithPlcId(packetFormingStation2PlcId);
	strappingOldInPos=allPos->getPosWithPlcId(strappingOldInPlcId);
	strappingOldOutPos=allPos->getPosWithPlcId(strappingOldOutPlcId);
	strappingNewInPos=allPos->getPosWithPlcId(strappingNewInPlcId);
	strappingNewOutPos=allPos->getPosWithPlcId(strappingNewOutPlcId);
	inputOutputPos=allPos->getPosWithPlcId(inputOutputPlcId);
	warehouseConveyor=allPos->getPosWithPlcId(warehouseConveyorPlcId);
	mainL->addStretch(1);
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerTriggeredSlot()));
	mainTimer->start(1000);
}
void forkUserWdgtClass::updateGui()
{
	if (!fork->getIsIdle())
	{
		forkUserWdgtEnableBtn(sendForming1PacketToOldStrappingBtn,false);
		forkUserWdgtEnableBtn(sendForming1PacketToNewStrappingBtn,false);
		forkUserWdgtEnableBtn(sendForming1PacketToInOutBtn,false);
		forkUserWdgtEnableBtn(sendForming2PacketToOldStrappingBtn,false);
		forkUserWdgtEnableBtn(sendForming2PacketToNewStrappingBtn,false);
		forkUserWdgtEnableBtn(sendStrappingOldOutPacketToInOutBtn,false);
		forkUserWdgtEnableBtn(sendStrappingNewOutPacketToInOutBtn,false);
		forkUserWdgtEnableBtn(sendForming2PacketToInOutBtn,false);
		forkUserWdgtEnableBtn(sendInOutPacketToWarehouseBtn,false);
		forkUserWdgtEnableBtn(sendInOutPacketToOldStrappingBtn,false);
		forkUserWdgtEnableBtn(sendInOutPacketToNewStrappingBtn,false);
		forkUserWdgtEnableBtn(clearPacketInInOutBtn,false);
	}
	else
	{
		forkUserWdgtEnableBtn(sendForming1PacketToOldStrappingBtn	,fork->getCanSendMission(packetFormingStation1Pos,strappingOldInPos));
		forkUserWdgtEnableBtn(sendForming1PacketToNewStrappingBtn	,fork->getCanSendMission(packetFormingStation1Pos,strappingNewInPos));
		forkUserWdgtEnableBtn(sendForming1PacketToInOutBtn				,fork->getCanSendMission(packetFormingStation1Pos,inputOutputPos));

		forkUserWdgtEnableBtn(sendForming2PacketToOldStrappingBtn	,fork->getCanSendMission(packetFormingStation2Pos,strappingOldInPos));
		forkUserWdgtEnableBtn(sendForming2PacketToNewStrappingBtn	,fork->getCanSendMission(packetFormingStation2Pos,strappingNewInPos));
		forkUserWdgtEnableBtn(sendForming2PacketToInOutBtn				,fork->getCanSendMission(packetFormingStation2Pos,inputOutputPos));

		forkUserWdgtEnableBtn(sendStrappingOldOutPacketToInOutBtn,fork->getCanSendMission(strappingOldOutPos,inputOutputPos));
		forkUserWdgtEnableBtn(sendStrappingNewOutPacketToInOutBtn,fork->getCanSendMission(strappingNewOutPos,inputOutputPos));

		forkUserWdgtEnableBtn(sendInOutPacketToWarehouseBtn				,fork->getCanSendMission(inputOutputPos,warehouseConveyor));
		forkUserWdgtEnableBtn(sendInOutPacketToOldStrappingBtn		,fork->getCanSendMission(inputOutputPos,strappingOldInPos));
		forkUserWdgtEnableBtn(sendInOutPacketToNewStrappingBtn		,fork->getCanSendMission(inputOutputPos,strappingNewInPos));
		forkUserWdgtEnableBtn(clearPacketInInOutBtn								,inputOutputPos->getHasTicket());
	}
	//forming1PacketLabel
	forming1PacketLabel->setText(tr("No Packet"));forming1PacketLabel->setStyleSheet(styleLabelGr);
	if (packetFormingStation1Pos->getPlcPacketTicket())
	{
		forming1PacketLabel->setText(tr("Packet Ready%1").arg(packetFormingStation1Pos->getNeedsWrapping()?tr(" Wrapping"):tr(" No Wrapping")));
		forming1PacketLabel->setStyleSheet(styleLabelG);
	}
	//forming2PacketLabel
	forming2PacketLabel->setText(tr("No Packet"));forming2PacketLabel->setStyleSheet(styleLabelGr);
	if (packetFormingStation2Pos->getPlcPacketTicket())
	{
		forming2PacketLabel->setText(tr("Packet Ready%1").arg(packetFormingStation2Pos->getNeedsWrapping()?tr(" Wrapping"):tr(" No Wrapping")));
		forming2PacketLabel->setStyleSheet(styleLabelG);
	}
	//crane
	if (plcCraneToPos==packingExport1PlcId)
		craneMissionLabel->setText(tr("HCA:Comming With Cassette"));
	else if (plcCraneFromPos||plcCraneToPos)
		craneMissionLabel->setText(tr("HCA:From %1 to %2").arg(plcCraneFromPos).arg(plcCraneToPos));
	else
		craneMissionLabel->setText(tr("HCA:Idle"));

	bool inOutPacketDefined=( (inputOutputPos->getPlcPacketTicket()>0)&&(inputOutputPos->getPlcPacketLength()>0));
	inOutPacketDefinedLabel->setText(inOutPacketDefined?tr("Packet Defined"):tr("No Packet Defined"));
	inOutPacketDefinedLabel->setStyleSheet(inOutPacketDefined?styleLabelG:styleLabelGr);

	bool inOutPacketPlcReadyToGet=inputOutputPos->getIsPlcReadyToGetPacket();
	inOutPlcReadyToGetLabel->setText(inOutPacketPlcReadyToGet?tr("PLC Ready to Get"):tr("PLC Not Ready to Get"));
	inOutPlcReadyToGetLabel->setStyleSheet(inOutPacketPlcReadyToGet?styleLabelG:styleLabelGr);

	bool inOutPacketPlcReadyToSend=inputOutputPos->getIsPlcReadyToSendPacket();
	inOutPlcReadyToSendLabel->setText(inOutPacketPlcReadyToSend?tr("PLC Ready to Send"):tr("PLC Not Ready to Send"));
	inOutPlcReadyToSendLabel->setStyleSheet(inOutPacketPlcReadyToSend?styleLabelG:styleLabelGr);

	bool strappingOldPacketDefined=strappingOldOutPos->getIsPacketDefined();
	strappingOldPacketDefinedLabel->setText(strappingOldPacketDefined?tr("Packet Defined"):tr("No Packet Defined"));
	strappingOldPacketDefinedLabel->setStyleSheet(strappingOldPacketDefined?styleLabelG:styleLabelGr);
	bool strappingOldPacketPlcReady=strappingOldOutPos->getIsPlcReadyToSendPacket();
	strappingOldPlcReadyLabel->setText(strappingOldPacketPlcReady?tr("PLC Ready"):tr("PLC Not Ready"));
	strappingOldPlcReadyLabel->setStyleSheet(strappingOldPacketPlcReady?styleLabelG:styleLabelGr);

	bool strappingNewPacketDefined=strappingNewOutPos->getIsPacketDefined();
	strappingNewPacketDefinedLabel->setText(strappingNewPacketDefined?tr("Packet Defined"):tr("No Packet Defined"));
	strappingNewPacketDefinedLabel->setStyleSheet(strappingNewPacketDefined?styleLabelG:styleLabelGr);
	bool strappingNewPacketPlcReady=strappingNewOutPos->getIsPlcReadyToSendPacket();
	strappingNewPlcReadyLabel->setText(strappingNewPacketPlcReady?tr("PLC Ready"):tr("PLC Not Ready"));
	strappingNewPlcReadyLabel->setStyleSheet(strappingNewPacketPlcReady?styleLabelG:styleLabelGr);


}
void forkUserWdgtClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((ip==packetCraneIp)&&((table=="pcCom")||(table=="ConveyorState")))
	{
		for (int i=0;i<bitVector.count();i++)
			bitVector[i]->updateValue(table,index,value.toInt());
		if (index==17)
			plcCraneFromPos=value.toInt();
		if (index==18)
			plcCraneToPos=value.toInt();
	}
}
void forkUserWdgtClass::sendMissionToFork(int from,int to)
{
	if (fork->getIsIdle())
	{
		if (!fork->sendMission(from,to))
			QMessageBox::warning(this,"Warning",tr("Could Not Send Mission"));
	}
	updateGui();
}
void forkUserWdgtClass::mainTimerTriggeredSlot()
{
	updateGui();
}
void forkUserWdgtClass::autoMissionsCheckBoxStateChangedSlot(int state)
{
	fork->setAutoMissionsEnabled(autoMissionsCheckBox->isChecked());
}
void forkUserWdgtClass::sendForming1PacketToOldStrappingBtnClickedSlot()
{
	sendMissionToFork(packetFormingStation1PlcId,strappingOldInPlcId);
}
void forkUserWdgtClass::sendForming1PacketToNewStrappingBtnClickedSlot()
{
	sendMissionToFork(packetFormingStation1PlcId,strappingNewInPlcId);
}
void forkUserWdgtClass::sendForming1PacketToInOutBtnClickedSlot()
{
	sendMissionToFork(packetFormingStation1PlcId,inputOutputPlcId);
}

void forkUserWdgtClass::sendForming2PacketToOldStrappingBtnClickedSlot()
{
	sendMissionToFork(packetFormingStation2PlcId,strappingOldInPlcId);
}
void forkUserWdgtClass::sendForming2PacketToNewStrappingBtnClickedSlot()
{
	sendMissionToFork(packetFormingStation2PlcId,strappingNewInPlcId);
}
void forkUserWdgtClass::sendForming2PacketToInOutBtnClickedSlot()
{
	sendMissionToFork(packetFormingStation2PlcId,inputOutputPlcId);
}

void forkUserWdgtClass::sendStrappingOldOutPacketToInOutBtnClickedSlot()
{
	sendMissionToFork(strappingOldOutPlcId,inputOutputPlcId);
}
void forkUserWdgtClass::sendStrappingNewOutPacketToInOutBtnClickedSlot()
{
	sendMissionToFork(strappingNewOutPlcId,inputOutputPlcId);
}

void forkUserWdgtClass::sendInOutPacketToWarehouseBtnClickedSlot()
{
	sendMissionToFork(inputOutputPlcId,warehouseConveyorPlcId);
}
void forkUserWdgtClass::sendInOutPacketToOldStrappingBtnClickedSlot()
{
	sendMissionToFork(inputOutputPlcId,strappingOldInPlcId);
}
void forkUserWdgtClass::sendInOutPacketToNewStrappingBtnClickedSlot()
{
	sendMissionToFork(inputOutputPlcId,strappingNewInPlcId);
}
void forkUserWdgtClass::clearPacketInInOutBtnClickedSlot()
{
	inputOutputPos->setPacketTicket(0);
	forkTuxip->writeInteger("packetId",inputOutputPlcId,0);
	forkTuxip->writeInteger("packetLength",inputOutputPlcId,0);
	forkTuxip->writeInteger("packetWoods",inputOutputPlcId,0);
	updateGui();
}

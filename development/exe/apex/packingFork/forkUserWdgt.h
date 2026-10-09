#ifndef forkUserWdgt_h
#define forkUserWdgt_h

class forkUserWdgtClass;
#include "Gf.h"
#include "allPosPackingFork.h"
#include "forkPacking.h"
#include "gView.h"
#include "hmiGuiWidgetsLib.h"
#include "serv.h"
#include "hmiDlgForkPacking.h"

class forkUserWdgtClass:public QWidget
{
	tuxipClass										*forkTuxip=nullptr;
	forkPackingClass							*fork=nullptr;
	QCheckBox											*autoMissionsCheckBox;

	posPackingForkClass						*packetFormingStation1Pos=nullptr;
	posPackingForkClass						*packetFormingStation2Pos=nullptr;
	posPackingForkClass						*strappingOldInPos=nullptr;
	posPackingForkClass						*strappingOldOutPos=nullptr;
	posPackingForkClass						*strappingNewInPos=nullptr;
	posPackingForkClass						*strappingNewOutPos=nullptr;
	posPackingForkClass						*inputOutputPos=nullptr;
	posPackingForkClass						*warehouseConveyor=nullptr;

	QLabel												*forming1PacketLabel;
	QPushButton										*sendForming1PacketToOldStrappingBtn=nullptr;
	QPushButton										*sendForming1PacketToNewStrappingBtn=nullptr;
	QPushButton										*sendForming1PacketToInOutBtn=nullptr;

	QLabel												*forming2PacketLabel;
	QPushButton										*sendForming2PacketToOldStrappingBtn=nullptr;
	QPushButton										*sendForming2PacketToNewStrappingBtn=nullptr;
	QPushButton										*sendForming2PacketToInOutBtn=nullptr;

	QPushButton										*sendStrappingOldOutPacketToInOutBtn=nullptr;

	QPushButton										*sendStrappingNewOutPacketToInOutBtn=nullptr;

	QLabel												*inOutPacketDefinedLabel;
	QLabel												*inOutPlcReadyToGetLabel;
	QLabel												*inOutPlcReadyToSendLabel;
	QPushButton										*sendInOutPacketToWarehouseBtn=nullptr;
	QPushButton										*sendInOutPacketToOldStrappingBtn=nullptr;
	QPushButton										*sendInOutPacketToNewStrappingBtn=nullptr;
	QPushButton										*clearPacketInInOutBtn=nullptr;

	QLabel												*strappingOldPacketDefinedLabel;
	QLabel												*strappingOldPlcReadyLabel;

	QLabel												*strappingNewPacketDefinedLabel;
	QLabel												*strappingNewPlcReadyLabel;

	QVector<bitClass*>						bitVector;
	QLabel												*craneMissionLabel;
	int														plcCraneFromPos=0;
	int														plcCraneToPos=0;
	Q_OBJECT
public:
	forkUserWdgtClass(allPosPackingForkClass *allPos,forkPackingClass *fork_,tuxipClass *forkTuxip_,QWidget *p=nullptr);
	void updateGui();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void sendMissionToFork(int from,int to);
public slots:
	void mainTimerTriggeredSlot();
	void autoMissionsCheckBoxStateChangedSlot(int state);

	void sendForming1PacketToOldStrappingBtnClickedSlot();
	void sendForming1PacketToNewStrappingBtnClickedSlot();
	void sendForming1PacketToInOutBtnClickedSlot();

	void sendForming2PacketToOldStrappingBtnClickedSlot();
	void sendForming2PacketToNewStrappingBtnClickedSlot();
	void sendForming2PacketToInOutBtnClickedSlot();

	void sendStrappingOldOutPacketToInOutBtnClickedSlot();
	void sendStrappingNewOutPacketToInOutBtnClickedSlot();

	void sendInOutPacketToWarehouseBtnClickedSlot();
	void sendInOutPacketToOldStrappingBtnClickedSlot();
	void sendInOutPacketToNewStrappingBtnClickedSlot();
	void clearPacketInInOutBtnClickedSlot();
};
#endif

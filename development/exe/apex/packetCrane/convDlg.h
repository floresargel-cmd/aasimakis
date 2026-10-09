#ifndef CONVDLG_H
#define CONVDLG_H

#include "pcG.h"
#include "../common/apexG.h"
#include "../common/btns.h"
#include "amem.h"
#include "ps.h"
#include "cm.h"
class conveyorsManualClass : public QDialog
{
	tcpServerClass									*tcpServer;

	QLabel													*imageLabel;

	packetProximitySwitchClass			*c25Break;
	packetProximitySwitchClass      *c24Break;

	packetProximitySwitchClass			*conveyorPsLeftStop;
	packetProximitySwitchClass      *conveyorPsRightStop;
	packetProximitySwitchClass			*conveyorPsLeftBrake;
  packetProximitySwitchClass      *conveyorPsRightBreak;

	packetProximitySwitchClass			*cranePsPositionLeft;
	packetProximitySwitchClass			*forkPsPositionLeft;
	packetProximitySwitchClass			*cranePsPositionRight;
	packetProximitySwitchClass			*forkPsPositionRight;

	packetProximitySwitchClass			*conveyorPhotoLeftAnticollision;
	packetProximitySwitchClass			*conveyorPhotoRightAnticollision;
	conveyorMovmentClass						*c24Cm;
	conveyorMovmentClass						*c25Cm;
	conveyorMovmentClass						*rCm;

	aPlcMemoryClass                 *casseteNumberOnRobotMem;
	aPlcMemoryClass                 *casseteNumberOnC24Mem;
	aPlcMemoryClass                 *casseteNumberOnC25Mem;


	//
Q_OBJECT
	QVector<myQPushButton*>					buttonsVector;
public:
  conveyorsManualClass(tcpServerClass *tcpServer_,QWidget *p);
  ~conveyorsManualClass();

	void packetCranePlcIntMemory150Changed(int index,int value);
	void packetCranePlcIntMemory152Changed(int index,int value);
	void packetCranePlcFloatMemoryChanged(int index,double value);
};
#endif

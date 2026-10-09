#ifndef PLC_H
#define PLC_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
class packetCraneManualClass;
#include "pcG.h"
#include "../common/apexG.h"
#include "../common/btns.h"
#include "forkM.h"
#include "craneM.h"
#include "manCasSel.h"
#include "craneM.h"
class plcClass : public QDialog
{
#ifdef TUX_PLC_COMMS
	tuxipServerClass								*tuxipServer;
	tuxipClass											*tuxIpCrane;
	tuxipClass											*tuxIpFork;
#endif
	tcpServerClass									*packetCraneTcpServer;
	tcpServerClass									*forkLiftTcpServer;
	packetCraneManualClass					*packetCraneManual;
	forkLiftManualClass							*forkLiftManual;
	int															lastCranePlcHandShakeValue;
	int															lastForkPlcHandShakeValue;
Q_OBJECT
public:
  plcClass(QWidget *p);
  ~plcClass();
	//
#ifdef TUX_PLC_COMMS
	tuxipServerClass *getTuxServer();
	tuxipClass *getCraneTuxClass();
	tuxipClass *getForkTuxClass();
	void showTuxServerCommsDialog();
	void addTables();
#endif
	tcpServerClass *getPacketCraneTcpServer();
	tcpServerClass *getForkLiftTcpServer();
	//
	forkLiftManualClass *getForkLiftManualClass();
	int getLastCranePlcHandShakeValue(){return lastCranePlcHandShakeValue;};
	int getLastForkPlcHandShakeValue(){return lastForkPlcHandShakeValue;};
	void packetCranePlcIntMemory150Changed(int index,int value);
	void packetCranePlcIntMemory152Changed(int index,int value);
	void packetCranePlcFloatMemoryChanged(int index,double value);
	void forkLiftPlcIntMemoryChanged(int index,int value);
	void forkLiftPlcFloatMemoryChanged(int index,double value);
	bool isConectionsEstablished();
	void setManualCassetteMovementClass(manualCassetteMovementClass *manualCassetteMovement);
};
#endif

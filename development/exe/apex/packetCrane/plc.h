#ifndef PLC_H	
#define PLC_H

class packetCraneManualClass;
#include "pcG.h"
#include "../common/apexG.h"
#include "../common/btns.h"
#include "forkM.h"
#include "craneM.h"
#include "manCasSel.h"
#include "craneM.h"
#include "allConvPos.h"
class plcClass : public QDialog	
{
	tuxipServerClass										*tuxipServer;
	tuxipClass													*tuxIpCrane;
	tuxipClass													*tuxIpFork;
	tuxipClass													*tuxIpCrane3;
	tcpServerClass											*packetCraneTcpServer;
	tcpServerClass											*forkLiftTcpServer;
	packetCraneManualClass							*packetCraneManual;
	forkLiftManualClass									*forkLiftManual;
	allConvPositionsClass								*allConvPositionsEastGroup;
	allConvPositionsClass								*allConvPositionsPackingGroup;
	int																	lastCranePlcHandShakeValue;
	int																	lastForkPlcHandShakeValue;
Q_OBJECT
public:										
	plcClass(QString craneIp,QString forkIp,QString crane3Ip,bool noPlc,QWidget *p);
	~plcClass();
	//
	tuxipServerClass *getTuxServer();
	tuxipClass *getCraneTuxClass();
	tuxipClass *getForkTuxClass();
	tuxipClass *getCrane3TuxClass();
	void showTuxServerCommsDialog();
	void setIsAutoUpdatingFromPlc(bool v);
	tcpServerClass *getPacketCraneTcpServer();
	tcpServerClass *getForkLiftTcpServer();
	void setAllPossitionsClass(allPosisionsClass* allP);
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
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:
	void stopSlot();
};																	
#endif
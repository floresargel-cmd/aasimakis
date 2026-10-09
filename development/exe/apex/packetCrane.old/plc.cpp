#include "plc.h"
QVector<QProcess*> externalProcessesVector;

plcClass::plcClass(QWidget *p):QDialog(p)
{
	lastCranePlcHandShakeValue=lastForkPlcHandShakeValue=0;
	setGeometry(280,10,1350,1000);
	setWindowFlags(Qt::Window);
/////////////////////////////////
#ifdef VB_PLC_COMMS
	QWidget *packetCraneTcpServerWidget=new QWidget();
	packetCraneTcpServer=new tcpServerClass(PACKET_CRANE_READ_PORT,PACKET_CRANE_WRITE_PORT,packetCraneTcpServerWidget,false);

	packetCraneTcpServer->addTableMonitor(tableMonitorClass::TYPE_INT   ,"N150",50);
	packetCraneTcpServer->addTableMonitor(tableMonitorClass::TYPE_INT   ,"N152",50);
	packetCraneTcpServer->addTableMonitor(tableMonitorClass::TYPE_DOUBLE,"F151",50);
	int refreshRate=500;

	QProcess *packetCraneProc=new QProcess();
	QStringList packetCraneArgs;
	packetCraneArgs<<craneIp<<packetCraneTcpServer->getIp()<<QString().number(PACKET_CRANE_WRITE_PORT)<<QString().number(PACKET_CRANE_READ_PORT)<<QString().number(refreshRate)
		<<"N150"<<QString().number(50)<<QString().number(refreshRate)
		<<"N152"<<QString().number(50)<<QString().number(refreshRate)
		<<"F151"<<QString().number(50)<<QString().number(refreshRate);
	packetCraneProc->start(QDir::currentPath()+"//comsPacketCrane//comsPacketCrane.exe ",packetCraneArgs);
	externalProcessesVector.append(packetCraneProc);
/////////////////////////////////
	QWidget *forkLiftTcpServerWidget=new QWidget();
	forkLiftTcpServer=new tcpServerClass(STRAPING_READ_PORT_FOR_WARE_HOUSE_PC,STRAPING_WRITE_PORT_FOR_WARE_HOUSE_PC,forkLiftTcpServerWidget,false);

	forkLiftTcpServer->addTableMonitor(tableMonitorClass::TYPE_INT   ,"N160",25);
	forkLiftTcpServer->addTableMonitor(tableMonitorClass::TYPE_DOUBLE,"F161",11);
	forkLiftTcpServer->addTableMonitor(tableMonitorClass::TYPE_DOUBLE,"F162",5);
//	forkLiftTcpServer->addTableMonitor(tableMonitorClass::TYPE_INT   ,"N170",11);
	QProcess *forkLiftProc=new QProcess();
	QStringList forkLiftArgs;
	forkLiftArgs<<forkIp<<forkLiftTcpServer->getIp()<<QString().number(STRAPING_WRITE_PORT_FOR_WARE_HOUSE_PC)<<QString().number(STRAPING_READ_PORT_FOR_WARE_HOUSE_PC)<<QString().number(refreshRate)
		<<"N160"<<QString().number(25)<<QString().number(refreshRate)
		<<"F161"<<QString().number(11)<<QString().number(refreshRate)
		<<"F162"<<QString().number(5)<<QString().number(refreshRate)
		//<<"N170"<<QString().number(23)<<QString().number(100)
		;
	forkLiftProc->start(QDir::currentPath()+"//comsForkLift//comsForkLift.exe ",forkLiftArgs);
	externalProcessesVector.append(forkLiftProc);
#endif
#ifdef TUX_PLC_COMMS
#ifdef _DEBUG
	tuxipServer=new tuxipServerClass(this);
#else
	tuxipServer=new tuxipServerClass(this);
#endif
	tuxipServer->addConnection(packetCraneIp,"crane");
	tuxIpCrane=tuxipServer->getTuxipClassWithIp(packetCraneIp);
	packetCraneTcpServer=new tcpServerClass(tuxIpCrane);

	tuxipServer->addConnection(forkIp,"fork");
#ifdef USE_LOCAL_DB
	tuxIpFork=tuxipServer->getTuxipClassWithIndex(1);
#else
	tuxIpFork=tuxipServer->getTuxipClassWithIp(forkIp);
#endif
	forkLiftTcpServer=new tcpServerClass(tuxIpFork);
	//
#endif
/////////////////////////////////
	QTabWidget *mainTabWidget=new QTabWidget();
	packetCraneManual=new packetCraneManualClass(packetCraneTcpServer,this);mainTabWidget->addTab(packetCraneManual,tr("Crane"));
	forkLiftManual=new forkLiftManualClass(forkLiftTcpServer,this);mainTabWidget->addTab(forkLiftManual,tr("Fork lift"));
#ifdef VB_PLC_COMMS
	mainTabWidget->addTab(packetCraneTcpServerWidget,tr("Packet crane PLC comms"));
	mainTabWidget->addTab(forkLiftTcpServerWidget,tr("Fork lift PLC comms"));
#endif
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	mainLayout->addWidget(mainTabWidget);
}
plcClass::~plcClass()
{
	delete packetCraneManual;
	delete forkLiftManual;
	delete forkLiftTcpServer;
	delete packetCraneTcpServer;
#ifdef TUX_PLC_COMMS
	delete tuxipServer;
#endif
}
forkLiftManualClass *plcClass::getForkLiftManualClass()
{
	return forkLiftManual;
}
#ifdef TUX_PLC_COMMS
tuxipServerClass *plcClass::getTuxServer()
{
	return tuxipServer;
}
tuxipClass *plcClass::getCraneTuxClass()
{
	return tuxIpCrane;
}
tuxipClass *plcClass::getForkTuxClass()
{
	return tuxIpFork;
}
void plcClass::showTuxServerCommsDialog()
{
	tuxipServer->showDialog();
}
void plcClass::addTables()
{
	tuxIpCrane->addTable("N150",plcTableClass::TYPE_INT,0,49,750);
	tuxIpCrane->addTable("N152",plcTableClass::TYPE_INT,0,49,750);
	tuxIpCrane->addTable("F151",plcTableClass::TYPE_FLOAT,0,49,750);

	tuxIpFork->addTable("N160",plcTableClass::TYPE_INT,0,24,750);
	tuxIpFork->addTable("F161",plcTableClass::TYPE_FLOAT,0,10,750);
	tuxIpFork->addTable("F162",plcTableClass::TYPE_FLOAT,0,4,750);

	tuxIpCrane->setIsAutoUpdatingFromPlc(true);
	tuxIpFork->setIsAutoUpdatingFromPlc(true);

}
#endif
void plcClass::setManualCassetteMovementClass(manualCassetteMovementClass *manualCassetteMovement)
{
	packetCraneManual->setManualCassetteMovementClass(manualCassetteMovement);
}
tcpServerClass* plcClass::getPacketCraneTcpServer()
{
	return packetCraneTcpServer;
}
tcpServerClass* plcClass::getForkLiftTcpServer()
{
	return forkLiftTcpServer;
}
bool plcClass::isConectionsEstablished()
{
#ifdef VB_PLC_COMMS
	if ((packetCraneTcpServer->isConectionEstablished()&&forkLiftTcpServer->isConectionEstablished()))
		return true;
	return false;
#endif
#ifdef TUX_PLC_COMMS
	return (!tuxipServer->getHasError());
#endif
}
void plcClass::packetCranePlcIntMemory150Changed(int index,int value)
{
	packetCraneManual->plcIntMemory150Changed(index,value);
	if (index==25)
	{
		packetCraneTcpServer->sendString(QString("N150|24|%1").arg(value));
		lastCranePlcHandShakeValue=value;
	}
}
void plcClass::packetCranePlcIntMemory152Changed(int index,int value)
{
	packetCraneManual->plcIntMemory152Changed(index,value);
}
void plcClass::packetCranePlcFloatMemoryChanged(int index,double value)
{
	packetCraneManual->plcFloatMemoryChanged(index,value);
}
void plcClass::forkLiftPlcIntMemoryChanged(int index,int value)
{
	forkLiftManual->plcIntMemoryChanged(index,value);
	if (index==10)
	{
		forkLiftTcpServer->sendString(QString("N160|11|%1").arg(value));
		lastForkPlcHandShakeValue=value;
	}
}
void plcClass::forkLiftPlcFloatMemoryChanged(int index,double value)
{
	forkLiftManual->plcFloatMemoryChanged(index,value);
}

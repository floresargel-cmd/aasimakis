#include "pch.h"
#include "plc.h"
//QVector<QProcess*> externalProcessesVector;
tuxipClass *tuxIpCraneGlobalPtr;

plcClass::plcClass(QString craneIp,QString forkIp,QString crane3Ip,bool noPlc,QWidget *p):QDialog(p)
{
	QDialog::setWindowTitle(tr("Packing Area HMI"));
	lastCranePlcHandShakeValue=lastForkPlcHandShakeValue=0;
	setGeometry(280,10,1350,1000);
	setWindowFlags(Qt::Window);
/////////////////////////////////
	tuxipServer=new tuxipServerClass(this);
	tuxIpCraneGlobalPtr=tuxipServer->addConnection(craneIp,"crane");
	tuxIpCrane=tuxipServer->getTuxipClassWithIp(craneIp);
	packetCraneTcpServer=new tcpServerClass(tuxIpCrane);

	tuxIpFork=tuxipServer->addConnection(forkIp,"fork");
	forkLiftTcpServer=new tcpServerClass(tuxIpFork);

	tuxIpCrane->addTable("N150",plcTableClass::TYPE_INT,0,49,200);
	tuxIpCrane->addTable("N152",plcTableClass::TYPE_INT,0,49,200);
	tuxIpCrane->addTable("F151",plcTableClass::TYPE_FLOAT,0,49,200);
	tuxIpCrane->addTable("ConveyorBasket",plcTableClass::TYPE_INT,30,45,200);
	tuxIpCrane->addTable("ConveyorState",plcTableClass::TYPE_INT,30,45,200);
	tuxIpCrane->addTable("ConveyorData",plcTableClass::TYPE_INT,30,45,200);
	tuxIpCrane->addTable("pcCom",plcTableClass::TYPE_INT,0,35,200);

	tuxIpFork->addTable("N160",plcTableClass::TYPE_INT,0,24,200);
	tuxIpFork->addTable("F161",plcTableClass::TYPE_FLOAT,0,10,200);
	tuxIpFork->addTable("F162",plcTableClass::TYPE_FLOAT,0,4,200);

	tuxIpCrane3=tuxipServer->addConnection(crane3Ip,"crane 3");
	tuxIpCrane3->addTable("packetId",plcTableClass::TYPE_INT,0,75,100);
	tuxIpCrane3->addTable("packetLength",plcTableClass::TYPE_INT,0,75,100);
	tuxIpCrane3->addTable("packetWoods",plcTableClass::TYPE_INT,0,75,100);
	tuxIpCrane3->addTable("fc1ToPcI",plcTableClass::TYPE_INT,0,50,100);
	tuxIpCrane3->addTable("fc1ToPcF",plcTableClass::TYPE_FLOAT,0,50,100);
	tuxIpCrane3->addTable("pcCom",plcTableClass::TYPE_INT,0,9,100);

	//
/////////////////////////////////
	QTabWidget *mainTabWidget=new QTabWidget();
	packetCraneManual=new packetCraneManualClass(packetCraneTcpServer,this);mainTabWidget->addTab(packetCraneManual,tr("Crane"));
	forkLiftManual=new forkLiftManualClass(forkLiftTcpServer,this);mainTabWidget->addTab(forkLiftManual,tr("Fork lift"));
	allConvPositionsEastGroup=new allConvPositionsClass(tuxIpCrane,nullptr,allConvPositionsClass::eastGroup,this);mainTabWidget->addTab(allConvPositionsEastGroup,tr("Station East"));
	allConvPositionsPackingGroup=new allConvPositionsClass(tuxIpCrane,tuxIpCrane3,allConvPositionsClass::packingGroup,this);mainTabWidget->addTab(allConvPositionsPackingGroup,tr("Station Packing"));
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	mainLayout->addWidget(mainTabWidget);

	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);
}
plcClass::~plcClass()
{
	delete packetCraneManual;
	delete forkLiftManual;
	delete forkLiftTcpServer;
	delete packetCraneTcpServer;
	delete tuxipServer;
}
void plcClass::setAllPossitionsClass(allPosisionsClass* allP)
{
	packetCraneManual->setAllPossitionsClass(allP);
}
void plcClass::stopSlot()
{
	tuxIpCrane->writeInteger("N150",20,166);
	tuxIpCrane->writeInteger("N150",20,0);
}
forkLiftManualClass *plcClass::getForkLiftManualClass()
{
	return forkLiftManual;
}
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
tuxipClass *plcClass::getCrane3TuxClass()
{
	return tuxIpCrane3;
}
void plcClass::showTuxServerCommsDialog()
{
	tuxipServer->showDialog();
}
void plcClass::setIsAutoUpdatingFromPlc(bool v)
{
	tuxIpCrane->setIsAutoUpdatingFromPlc(v);
	tuxIpFork->setIsAutoUpdatingFromPlc(v);
	tuxIpCrane3->setIsAutoUpdatingFromPlc(v);
}
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
	return (!tuxipServer->getHasError());
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
void plcClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	packetCraneManual->dataInPlcChanged(ip,table,index,value);
	allConvPositionsEastGroup->dataInPlcChanged(ip,table,index,value);
	allConvPositionsPackingGroup->dataInPlcChanged(ip,table,index,value);
	forkLiftManual->dataInPlcChanged(ip,table,index,value);
}

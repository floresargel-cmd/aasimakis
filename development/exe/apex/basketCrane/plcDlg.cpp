#include "plcDlg.h"
QVector<QProcess*> externalProcessesVector;

plcDialogClass::plcDialogClass(plcWidgetClass *plcWidget_)
{
	setGeometry(250,50,1500,1100);
	setWindowIcon(QIcon(":/logo_16x16.png"));
	plcWidget=plcWidget_;
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
#ifdef VB_PLC_COMMS
	QTabWidget *mainTabWidget=new QTabWidget();
	QWidget *plcWidgetForTab=new QWidget();mainTabWidget->addTab(plcWidgetForTab,tr("PLC"));
	QWidget *tcpWidget=new QWidget();mainTabWidget->addTab(tcpWidget,tr("communications"));
#endif
	//
	/////////////////////////////////////////////////////////////////////////cranePlcTcp
#ifdef VB_PLC_COMMS
	//tcpWidget
	QVBoxLayout *tcpWidgetLayout=new QVBoxLayout();tcpWidget->setLayout(tcpWidgetLayout);	
	QTabWidget *commsTabWidget=new QTabWidget();
	tcpWidgetLayout->addWidget(commsTabWidget);
	QWidget *craneTcpWidget=new QWidget();commsTabWidget->addTab(craneTcpWidget,tr("crane"));
	QWidget *conveyorsTcpWidget=new QWidget();commsTabWidget->addTab(conveyorsTcpWidget,tr("conveyors"));
#ifdef EMULATE
	cranePlcTcpServer=new tcpServerClass(CRANE_READ_PORT,CRANE_WRITE_PORT,craneTcpWidget,true,false);
#else
	cranePlcTcpServer=new tcpServerClass(CRANE_READ_PORT,CRANE_WRITE_PORT,craneTcpWidget,false,true);
#endif
#ifdef EMULATE
	conveyorsPlcTcpServer=new tcpServerClass(AGEING_OVEN_READ_PORT,AGEING_OVEN_WRITE_PORT,conveyorsTcpWidget,true,false);
#else
	conveyorsPlcTcpServer=new tcpServerClass(AGEING_OVEN_READ_PORT,AGEING_OVEN_WRITE_PORT,conveyorsTcpWidget,false,true);
#endif
	cranePlcTcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,"N20",25);
	cranePlcTcpServer->addTableMonitor(tableMonitorClass::TYPE_DOUBLE,"F21",18);
	conveyorsPlcTcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,"N11",27);
#endif
#ifdef TUX_PLC_COMMS
	tuxipServer=new tuxipServerClass(this);
	tuxIpCrane=tuxipServer->addConnection(basketCraneIp,"basket crane");
	cranePlcTcpServer=new tcpServerClass(tuxIpCrane);

	tuxIpConveyors=tuxipServer->addConnection(conveyorsIp,"oven");
	conveyorsPlcTcpServer=new tcpServerClass(tuxIpConveyors);

	tuxIpNewOven=tuxipServer->addConnection(newOvenIp,"oven");
	//
#endif
#ifdef VB_PLC_COMMS
	mainLayout->addWidget(mainTabWidget);
#endif
#ifdef TUX_PLC_COMMS
	QWidget *plcWidgetForTab=new QWidget();
	mainLayout->setMargin(2);
	mainLayout->addWidget(plcWidgetForTab);
#endif
	plcManual=new plcManualClass(plcWidgetForTab,plcWidget,cranePlcTcpServer,conveyorsPlcTcpServer);
	//
}
plcDialogClass::~plcDialogClass()
{
	delete plcManual;
	delete tuxipServer;
	delete cranePlcTcpServer;
	delete conveyorsPlcTcpServer;
}
#ifdef TUX_PLC_COMMS
tuxipServerClass *plcDialogClass::getTuxServer()
{
	return tuxipServer;
}
tuxipClass *plcDialogClass::getCrainTuxClass()
{
	return tuxIpCrane;
}
tuxipClass *plcDialogClass::getConveyorTuxClass()
{
	return tuxIpConveyors;
}
tuxipClass *plcDialogClass::getNewOvenTuxClass()
{
	return tuxIpNewOven;
}
#endif
void plcDialogClass::startExternalSoft()
{
#ifdef VB_PLC_COMMS
#ifndef EMULATE
	QProcess *craneProc=new QProcess();
	QStringList craneArgs;
	craneArgs<<craneIp<<cranePlcTcpServer->getIp()<<QString().number(CRANE_WRITE_PORT)<<QString().number(CRANE_READ_PORT)<<QString().number(100)<<"N20"<<QString().number(25)<<QString().number(100)<<"F21"<<QString().number(18)<<QString().number(500);
	craneProc->start(QDir::currentPath()+"//hmiBasketCrane//hmiBasketCrane.exe ",craneArgs);
	externalProcessesVector.append(craneProc);

	QProcess *conveyorsProc=new QProcess();
	QStringList conveyorsArgs;
	conveyorsArgs<<conveyorsIp<<conveyorsPlcTcpServer->getIp()<<QString().number(AGEING_OVEN_WRITE_PORT)<<QString().number(AGEING_OVEN_READ_PORT)<<QString().number(100)<<"N11"<<QString().number(27)<<QString().number(100);
	conveyorsProc->start(QDir::currentPath()+"//hmiAgingOven//hmiAgingOven.exe ",conveyorsArgs);
	externalProcessesVector.append(conveyorsProc);
#endif
#endif
}
#ifdef TUX_PLC_COMMS
void plcDialogClass::addTables()
{
	tuxIpCrane->addTable("N20",plcTableClass::TYPE_INT,0,24,750);
	tuxIpCrane->addTable("F21",plcTableClass::TYPE_FLOAT,0,17,750);
	tuxIpConveyors->addTable("N11",plcTableClass::TYPE_INT,0,36,750);
	tuxIpNewOven->addTable("N11",plcTableClass::TYPE_INT,0,9,750);
#ifdef EMULATE
	tuxIpCrane->addTable("401",plcTableClass::TYPE_INT,0,1,750);
	tuxIpCrane->addTable("411",plcTableClass::TYPE_INT,0,1,750);
	tuxIpConveyors->addTable("basketsForOvenReady",plcTableClass::TYPE_INT,0,5,750);
#endif
	tuxIpCrane->setIsAutoUpdatingFromPlc(true);
	tuxIpConveyors->setIsAutoUpdatingFromPlc(true);
	tuxIpNewOven->setIsAutoUpdatingFromPlc(true);
}
void plcDialogClass::showTuxServerCommsDialog()
{
	tuxipServer->showDialog();
}
#endif
tcpServerClass* plcDialogClass::getCranePlcTcpServer()
{
	return cranePlcTcpServer;
}
tcpServerClass* plcDialogClass::getConveyorsPlcTcpServer()
{
	return conveyorsPlcTcpServer;
}
void plcDialogClass::plcStatusChanged(int index,int value)
{
	plcManual->plcStatusChanged(index,value);
}
void plcDialogClass::plcFloatValuesChanged(int index,float value)
{
	plcManual->plcFloatValuesChanged(index,value);
}

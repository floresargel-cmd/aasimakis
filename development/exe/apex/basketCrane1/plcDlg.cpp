#include "plcDlg.h"
#include "configuration/ConfigurationManager.h"

QVector<QProcess*> externalProcessesVector;
static QMap<QString, tuxipClass*> g_tuxConnections;

plcDialogClass::plcDialogClass(plcWidgetClass *plcWidget_)
{
	const BasketCrane::Configuration::PlcSettings& plcSettings =
		BasketCrane::Configuration::ConfigurationManager::Instance().Plc();
	const QString craneIp = plcSettings.CraneIp();
	const QString conveyorsAddress = plcSettings.ConveyorsIp();
	const QString newOvenAddress = plcSettings.NewOvenIp();

	setGeometry(250, 50, 1500, 1100);
	setWindowIcon(QIcon(":/logo_16x16.png"));
	plcWidget = plcWidget_;
	//
	QVBoxLayout *mainLayout = new QVBoxLayout(); 
	setLayout(mainLayout);

#ifdef VB_PLC_COMMS
	QTabWidget *mainTabWidget=new QTabWidget();
	QWidget *plcWidgetForTab=new QWidget();mainTabWidget->addTab(plcWidgetForTab,tr("PLC"));
	QWidget *tcpWidget=new QWidget();mainTabWidget->addTab(tcpWidget,tr("communications"));
#endif

#ifdef VB_PLC_COMMS
	//tcpWidget
	QVBoxLayout *tcpWidgetLayout=new QVBoxLayout();tcpWidget->setLayout(tcpWidgetLayout);	
	QTabWidget *commsTabWidget=new QTabWidget();
	tcpWidgetLayout->addWidget(commsTabWidget);
	QWidget *craneTcpWidget=new QWidget();commsTabWidget->addTab(craneTcpWidget,tr("crane"));
	QWidget *conveyorsTcpWidget=new QWidget();commsTabWidget->addTab(conveyorsTcpWidget,tr("conveyors"));
#ifdef EMULATE
	cranePlcTcpServer=new tcpServerClass(plcSettings.CraneReadPort(),plcSettings.CraneWritePort(),craneTcpWidget,true,false);
#else
	cranePlcTcpServer=new tcpServerClass(plcSettings.CraneReadPort(),plcSettings.CraneWritePort(),craneTcpWidget,false,true);
#endif
#ifdef EMULATE
	conveyorsPlcTcpServer=new tcpServerClass(plcSettings.ConveyorsReadPort(),plcSettings.ConveyorsWritePort(),conveyorsTcpWidget,true,false);
#else
	conveyorsPlcTcpServer=new tcpServerClass(plcSettings.ConveyorsReadPort(),plcSettings.ConveyorsWritePort(),conveyorsTcpWidget,false,true);
#endif
	cranePlcTcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,"N20",25);
	cranePlcTcpServer->addTableMonitor(tableMonitorClass::TYPE_DOUBLE,"F21",18);
	conveyorsPlcTcpServer->addTableMonitor(tableMonitorClass::TYPE_INT,"N11",27);
#endif

#ifdef TUX_PLC_COMMS
	/* @@@@@ - AB, saw the CONNECTIONS climbing to 20+ in the TuxServer, so removing this for now to stop the clutter.
	*		   Changing how the TuxServer connections are created and stored.  Reusing a single class per PLCs.
	LOG("TUX_PLC_COMMS - plcDialogClass", "Creating tuxipServerClass and connections");
	tuxipServer = new tuxipServerClass(this);
	LOG("TUX_PLC_COMMS - basket crane", basketCraneIp);
	tuxIpCrane = tuxipServer->addConnection(basketCraneIp, "basket crane");
	cranePlcTcpServer = new tcpServerClass(tuxIpCrane);
	LOG("TUX_PLC_COMMS - oven1", conveyorsIp);
	tuxIpConveyors = tuxipServer->addConnection(conveyorsIp, "oven1");
	conveyorsPlcTcpServer = new tcpServerClass(tuxIpConveyors);
	LOG("TUX_PLC_COMMS - oven2", newOvenIp);
	tuxIpNewOven = tuxipServer->addConnection(newOvenIp, "oven2");
	*/
	// @@@@@ - AB: create or reuse a single tuxipClass per IP (prevents multiple simultaneous connections)
	LOG("TUX_PLC_COMMS - plcDialogClass", "Creating tuxipServerClass and connections");
	tuxipServer = new tuxipServerClass(this);

	auto getOrCreateTuxConnection = [&](const QString& ip, const QString& comment)->tuxipClass* {
		if (g_tuxConnections.contains(ip)) {
			LOG("plcDlg.cpp", QString("Re-using existing tuxip connection for %1").arg(ip)); // @@@@@ - AB
			return g_tuxConnections.value(ip);
		}
		LOG("plcDlg.cpp", QString("Adding new tuxip connection for %1").arg(ip)); // @@@@@ - AB
		tuxipClass* conn = tuxipServer->addConnection(ip, comment);
		g_tuxConnections.insert(ip, conn);
		return conn;
		};

	LOG("TUX_PLC_COMMS - basket crane", craneIp);
	tuxIpCrane = getOrCreateTuxConnection(craneIp, plcSettings.CraneDescription());
	cranePlcTcpServer = new tcpServerClass(tuxIpCrane);

	LOG("TUX_PLC_COMMS - oven1", conveyorsAddress);
	tuxIpConveyors = getOrCreateTuxConnection(conveyorsAddress, plcSettings.ConveyorsDescription());
	conveyorsPlcTcpServer = new tcpServerClass(tuxIpConveyors);

	LOG("TUX_PLC_COMMS - oven2", newOvenAddress);
	tuxIpNewOven = getOrCreateTuxConnection(newOvenAddress, plcSettings.NewOvenDescription());
#endif

#ifdef VB_PLC_COMMS
	mainLayout->addWidget(mainTabWidget);
#endif

#ifdef TUX_PLC_COMMS
	QWidget *plcWidgetForTab=new QWidget();
	mainLayout->setMargin(2);
	mainLayout->addWidget(plcWidgetForTab);
#endif

	plcManual = new plcManualClass(plcWidgetForTab, plcWidget, cranePlcTcpServer, conveyorsPlcTcpServer);
}

plcDialogClass::~plcDialogClass()
{
	// @@@@@ - AB: Clean up cached tuxip connections to ensure library resources/timers are closed.
#ifdef TUX_PLC_COMMS
	for (auto it = g_tuxConnections.begin(); it != g_tuxConnections.end(); ++it) {
		tuxipClass* t = it.value();
		if (!t) continue;
		// ask each tui wrapper to stop auto updates; this should stop its timers
		t->setIsAutoUpdatingFromPlc(false); // @@@@@ - AB
		// if tuxipClass exposes a disconnect/close method, call it here. Example:
		// t->disconnectFromPlc(); // implement in tuxipClass if missing
		t->disconnect();
		delete t; // call destructor to free resources (assumes destructor performs library cleanup)
	}
	g_tuxConnections.clear();
#endif

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

tuxipClass *plcDialogClass::getCraneTuxClass()
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
	const BasketCrane::Configuration::PlcSettings& plcSettings =
		BasketCrane::Configuration::ConfigurationManager::Instance().Plc();
#ifndef EMULATE
	QProcess *craneProc=new QProcess();
	QStringList craneArgs;
	craneArgs<<plcSettings.CraneIp()<<cranePlcTcpServer->getIp()<<QString().number(plcSettings.CraneWritePort())<<QString().number(plcSettings.CraneReadPort())<<QString().number(100)<<"N20"<<QString().number(25)<<QString().number(100)<<"F21"<<QString().number(18)<<QString().number(500);
	craneProc->start(plcSettings.CraneHmiExecutable(),craneArgs);
	externalProcessesVector.append(craneProc);

	QProcess *conveyorsProc=new QProcess();
	QStringList conveyorsArgs;
	conveyorsArgs<<plcSettings.ConveyorsIp()<<conveyorsPlcTcpServer->getIp()<<QString().number(plcSettings.ConveyorsWritePort())<<QString().number(plcSettings.ConveyorsReadPort())<<QString().number(100)<<"N11"<<QString().number(27)<<QString().number(100);
	conveyorsProc->start(plcSettings.ConveyorsHmiExecutable(),conveyorsArgs);
	externalProcessesVector.append(conveyorsProc);
#endif
#endif
}

#ifdef TUX_PLC_COMMS
void plcDialogClass::addTables()
{
	const BasketCrane::Configuration::PlcSettings& plcSettings =
		BasketCrane::Configuration::ConfigurationManager::Instance().Plc();
	const int pollingMs = theOneBCToRuleThemAll
		? plcSettings.EnabledPollingMilliseconds()
		: plcSettings.StandbyPollingMilliseconds();
	// DMS basket-ID register is needed for both active and standby connections.
	tuxIpConveyors->addTable("N12", plcTableClass::TYPE_INT, 5, 5, pollingMs);
	if (theOneBCToRuleThemAll) {
		tuxIpCrane->addTable("N20", plcTableClass::TYPE_INT, 0, 24, pollingMs);
		tuxIpCrane->addTable("F21", plcTableClass::TYPE_FLOAT, 0, 17, pollingMs);
		tuxIpCrane->addTable("B25", plcTableClass::TYPE_INT, 28, 28, pollingMs);
		tuxIpConveyors->addTable("N11", plcTableClass::TYPE_INT, 0, 49, pollingMs);
		tuxIpNewOven->addTable("N11", plcTableClass::TYPE_INT, 0, 9, pollingMs);
	}
	else {
		tuxIpCrane->addTable("N20", plcTableClass::TYPE_INT, 0, 24, pollingMs);
		tuxIpCrane->addTable("F21", plcTableClass::TYPE_FLOAT, 0, 17, pollingMs);
		tuxIpCrane->addTable("B25", plcTableClass::TYPE_INT, 28, 28, pollingMs);
		tuxIpConveyors->addTable("N11", plcTableClass::TYPE_INT, 0, 49, pollingMs);
		tuxIpNewOven->addTable("N11", plcTableClass::TYPE_INT, 0, 9, pollingMs);
	}
#ifdef EMULATE
	/* -- AB - Remvoing this from EMULATE for now @@@@@
	tuxIpCrane->addTable("401", plcTableClass::TYPE_INT, 0, 1, 750);
	tuxIpCrane->addTable("411",plcTableClass::TYPE_INT,0,1,750);
	tuxIpConveyors->addTable("basketsForOvenReady",plcTableClass::TYPE_INT,0,5,750);
	*/
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

void plcDialogClass::plcStatusChanged(int index, int value)
{
	plcManual->plcStatusChanged(index, value);
}

void plcDialogClass::plcFloatValuesChanged(int index, float value)
{
	plcManual->plcFloatValuesChanged(index, value);
}

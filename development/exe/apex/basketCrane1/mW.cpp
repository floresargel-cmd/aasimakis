#include "mW.h"
#include "configuration/ConfigurationManager.h"
#include "configuration/DatabaseConfigurator.h"
#include "generated/build_version.h"
#include "../common/execDBQuery.h"
#include "dbTableStatus.h"
#include "plcNetworkStatus.h"
#include "timerStatus.h"
#include "b.h"
#include "icon/resource.h"
#include <QApplication>
#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QLibraryInfo>
#include <QDialogButtonBox>
#include <QFrame>
#include <QHeaderView>
#include <QIntValidator>
#include <QLineEdit>
#include <QTableWidget>
#include <windows.h>


#include "../common/applogger.h"
#include "../common/execDBQuery.h"

static mainWindowClass* g_dbTableStatusTarget = 0;
void setDbTableActive(const QString& tableName, bool isRead) { if (g_dbTableStatusTarget) g_dbTableStatusTarget->setDbTableActive(tableName, isRead); }
void setDbTableIdle(const QString& tableName) { if (g_dbTableStatusTarget) g_dbTableStatusTarget->setDbTableIdle(tableName); }

static mainWindowClass* g_plcNetworkStatusTarget = 0;
void setPlcNetworkActive(const QString& networkName, bool isRead) { if (g_plcNetworkStatusTarget) g_plcNetworkStatusTarget->setPlcNetworkActive(networkName, isRead); }
void setPlcNetworkIdle(const QString& networkName) { if (g_plcNetworkStatusTarget) g_plcNetworkStatusTarget->setPlcNetworkIdle(networkName); }
void setPlcNetworkError(const QString& networkName) { if (g_plcNetworkStatusTarget) g_plcNetworkStatusTarget->setPlcNetworkError(networkName); }
void setPlcNetworkConnecting(const QString& networkName) { if (g_plcNetworkStatusTarget) g_plcNetworkStatusTarget->setPlcNetworkConnecting(networkName); }

static mainWindowClass* g_timerStatusTarget = 0;
void setMainTimerFiring() { if (g_timerStatusTarget) g_timerStatusTarget->setMainTimerFiring(); }
void setMainTimerIdle() { if (g_timerStatusTarget) g_timerStatusTarget->setMainTimerIdle(); }
void setAutoModeTimerFiring() { if (g_timerStatusTarget) g_timerStatusTarget->setAutoModeTimerFiring(); }
void setAutoModeTimerIdle() { if (g_timerStatusTarget) g_timerStatusTarget->setAutoModeTimerIdle(); }

static void logDatabaseEndpoint(const QSqlDatabase& database,
	const QString& profileName,
	const QString& configuredCatalog,
	const QString& tables)
{
	QString server = database.hostName();
	QString activeCatalog = configuredCatalog;
	QSqlQuery identityQuery(database);

	// Ask SQL Server for the effective endpoint after USE has executed. For an
	// ODBC DSN, QSqlDatabase::databaseName() is the DSN rather than the server.
	if (identityQuery.exec("SELECT @@SERVERNAME, DB_NAME()") && identityQuery.next())
	{
		server = identityQuery.value(0).toString();
		activeCatalog = identityQuery.value(1).toString();
	}
	else
	{
		LOG("WARNING: DATABASE", QString("Could not resolve SQL server identity for connection '%1': %2")
			.arg(database.connectionName()).arg(identityQuery.lastError().text()));
	}

	if (server.trimmed().isEmpty())
		server = "<server hidden by ODBC DSN>";

	LOG("DATABASE CONNECTION", QString("connection='%1'; profile='%2'; driver='%3'; DSN='%4'; server='%5'; database='%6'; user='%7'; tables='%8'")
		.arg(database.connectionName()).arg(profileName).arg(database.driverName())
		.arg(database.databaseName()).arg(server).arg(activeCatalog)
		.arg(database.userName()).arg(tables));
}

static QPixmap loadApexLogo()
{
	QPixmap logo;
	const HMODULE module = GetModuleHandle(NULL);
	const HRSRC resource = FindResource(module, MAKEINTRESOURCE(IDR_APEX_LOGO), RT_RCDATA);
	if (!resource)
	{
		LOG("ERROR: UI", "APEX logo resource was not found in the executable");
		return logo;
	}

	const HGLOBAL loadedResource = LoadResource(module, resource);
	const DWORD resourceSize = SizeofResource(module, resource);
	const void* resourceData = loadedResource ? LockResource(loadedResource) : NULL;
	if (!resourceData || resourceSize == 0
		|| !logo.loadFromData(reinterpret_cast<const uchar*>(resourceData), static_cast<uint>(resourceSize), "PNG"))
	{
		LOG("ERROR: UI", "APEX logo resource could not be decoded");
		return QPixmap();
	}

	return logo;
}

// #define inMyLaptop //@@@@@ - TODO: Remember to comment this out before building for production.
// #define SEMILIVE //@@@@@ - This allows the app to run even if 1 is already present.  Don't allow AUTO mode in this mode, though.
// With SEMILIVE, it connects to the TEST MSSQL02 Database.  It requires the moverDB2DB app to be running as well.
// The moverDB2DB app copies data from the LIVE database to the TEST MSSQL02 database, and also updates the PLCs with the latest data from the LIVE database.  
// This allows us to run this app in a "semi-live" mode, where we can test new features without affecting the LIVE database or PLCs.  
// However, since the PLCs are being updated with the latest data from the LIVE database, we need to be careful when testing new features that might affect the PLCs, 
// as it could potentially cause issues if there are bugs in the new code.

QVector<int>	*alloyIdArray;			// one on one with alloyDescription
QStringList		*alloyDescription;		// one on one with alloyIdArray
QStringList		*profilePicFileName;	// one on one with profileIntName
QVector<int>	*profileIntName;		// one on one with profilePicFileName

mainWindowClass::mainWindowClass()
{
	openGLWidget = 0;
	plcWidget = 0;
	plcStatusWidget = 0;
	plcStatusPreferredY = 0;

	const BasketCrane::Configuration::ConfigurationManager& configuration =
		BasketCrane::Configuration::ConfigurationManager::Instance();
	const QString currentHost = getAppHostname();
	const QString environmentMode = configuration.Application().EnvironmentMode();
	const bool observerExecutable = QFileInfo(QCoreApplication::applicationFilePath())
		.baseName().contains("Observer", Qt::CaseInsensitive);
	const bool liveRequested = environmentMode.compare("Live", Qt::CaseInsensitive) == 0
		&& !observerExecutable;
	const bool observerRequested = observerExecutable
		|| environmentMode.compare("LiveObserver", Qt::CaseInsensitive) == 0;
	bool liveHostAuthorized = false;
	const QStringList liveEnabledHosts = configuration.Application().LiveEnabledHosts();
	for (int i = 0; i < liveEnabledHosts.count(); ++i)
	{
		if (liveEnabledHosts[i].compare(currentHost, Qt::CaseInsensitive) == 0)
		{
			liveHostAuthorized = true;
			break;
		}
	}
#if defined(inMyLaptop) || defined(SEMILIVE) || defined(EMULATE)
	theOneBCToRuleThemAll = false;
	const bool observerMode = false;
#else
	theOneBCToRuleThemAll = liveRequested && liveHostAuthorized;
	const bool observerMode = observerRequested && liveHostAuthorized;
#endif
	setDatabaseReadOnlyMode(observerMode);
	const QString effectiveEnvironment = theOneBCToRuleThemAll
		? "Live" : (observerMode ? "LiveObserver" : "Test");
	LOG("CONFIGURATION", QString("environmentRequested='%1'; effectiveEnvironment='%2'; host='%3'; liveHostAuthorized=%4; file='%5'")
		.arg(environmentMode)
		.arg(effectiveEnvironment)
		.arg(currentHost)
		.arg(liveHostAuthorized ? "true" : "false")
		.arg(configuration.FileName()));

	setWindowIcon(QIcon(":/logo_16x16.png"));

	//
	LOG("mW.cpp", "initializeDatabase()");
	initializeDatabase();
	loadBasketDataCache();
	//
	LOG("mW.cpp", "setLayout(mainLayout)");
	QHBoxLayout *mainLayout = new QHBoxLayout(); mainLayout->setMargin(0);
	QVBoxLayout *rootLayout = new QVBoxLayout();
	rootLayout->setMargin(0);
	rootLayout->setSpacing(0);

	QWidget *appHeader = new QWidget(this);
	appHeader->setObjectName("appHeader");
	appHeader->setFixedHeight(54);
	QHBoxLayout *appHeaderLayout = new QHBoxLayout(appHeader);
	appHeaderLayout->setContentsMargins(18, 5, 18, 5);
	appHeaderLayout->setSpacing(12);
	QLabel *brandLogo = new QLabel(appHeader);
	brandLogo->setObjectName("brandLogo");
	brandLogo->setAlignment(Qt::AlignCenter);
	brandLogo->setFixedSize(116, 42);
	const QPixmap headerLogo = loadApexLogo();
	if (!headerLogo.isNull())
	{
		brandLogo->setPixmap(headerLogo.scaled(112, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		brandLogo->setToolTip(tr("APEX Aluminum Extrusions"));
	}
	else
	{
		brandLogo->setText("APEX");
	}
	appHeaderLayout->addWidget(brandLogo);
	QFrame *brandDivider = new QFrame(appHeader);
	brandDivider->setFrameShape(QFrame::VLine);
	brandDivider->setStyleSheet("color:#334155;");
	appHeaderLayout->addWidget(brandDivider);
	QVBoxLayout *titleStack = new QVBoxLayout();
	titleStack->setSpacing(0);
	QLabel *applicationTitle = new QLabel(tr("Basket Crane Control"), appHeader);
	applicationTitle->setObjectName("applicationTitle");
	QLabel *applicationSubtitle = new QLabel(tr("Material flow and aging operations"), appHeader);
	applicationSubtitle->setObjectName("applicationSubtitle");
	titleStack->addWidget(applicationTitle);
	titleStack->addWidget(applicationSubtitle);
	appHeaderLayout->addLayout(titleStack);
	appHeaderLayout->addStretch(1);
	QLabel *hostIdentity = new QLabel(QString("HOST  %1").arg(appHostname.toUpper()), appHeader);
	hostIdentity->setObjectName("hostIdentity");
	appHeaderLayout->addWidget(hostIdentity);
	QLabel *systemMode = new QLabel(theOneBCToRuleThemAll
		? tr("LIVE SYSTEM") : (observerMode ? tr("LIVE OBSERVER") : tr("TEST SYSTEM")), appHeader);
	systemMode->setObjectName("systemMode");
	systemMode->setProperty("mode", theOneBCToRuleThemAll ? "live" : (observerMode ? "observer" : "test"));
	systemMode->setAlignment(Qt::AlignCenter);
	systemMode->setToolTip(observerMode
		? tr("Reads Live databases. Database writes and PLC commands are blocked.")
		: QString());
	appHeaderLayout->addWidget(systemMode);
	rootLayout->addWidget(appHeader);
	rootLayout->addLayout(mainLayout, 1);
	// Status bar: database table activity indicators (green=read, blue=write, grey=idle)
	QWidget *statusBarWidget = new QWidget(this);
	statusBarWidget->setObjectName("systemStatusStrip");
	statusBarWidget->setStyleSheet("#systemStatusStrip{background:#0b1220;border-top:1px solid #263449;}");
	statusBarWidget->setFixedHeight(32);
	QHBoxLayout *statusBarLayout = new QHBoxLayout(statusBarWidget);
	statusBarLayout->setContentsMargins(4, 2, 4, 2);
	statusBarLayout->setSpacing(6);	
	
	dbAreaLabel = new QLabel("DB:", statusBarWidget);
	QString styleHeaderGrey("QLabel{font-size:10px;font-weight:700;background:transparent;color:#94a3b8;padding:2px 3px;}");
	dbAreaLabel->setStyleSheet(styleHeaderGrey);
	dbAreaLabel->setAutoFillBackground(true);
	statusBarLayout->addWidget(dbAreaLabel);
	QStringList tableNames = QStringList() << "basket_details" << "basket_location" << "baskets" << "basketHistory"
		<< "logCranePlc" << "logResetPlc" << "positions" << "temperTypes" << "toAssimakisExports" << "toOmavExports";
	QString styleGrey("QLabel{font-size:10px;background-color:#1e293b;color:#94a3b8;border:1px solid #334155;padding:2px 6px;border-radius:5px;}");
	for (int i = 0; i < tableNames.size(); ++i) {
		QLabel *lbl = new QLabel(tableNames[i], statusBarWidget);
		lbl->setStyleSheet(styleGrey);
		lbl->setAutoFillBackground(true);
		dbTableStatusLabels.insert(tableNames[i], lbl);
		statusBarLayout->addWidget(lbl);
	}
	
	plcAreaLabel = new QLabel("PLC(s):", statusBarWidget);
	plcAreaLabel->setStyleSheet(styleHeaderGrey);
	plcAreaLabel->setAutoFillBackground(true);
	statusBarLayout->addWidget(plcAreaLabel);
	// PLC network status indicators (green=read, blue=write, red=error, grey=idle)
	QStringList plcNetworkNames = QStringList() << PLC_NETWORK_CRANE << PLC_NETWORK_CONVEYORS << PLC_NETWORK_NEW_OVEN;
	for (int i = 0; i < plcNetworkNames.size(); ++i) {
		QLabel *lbl = new QLabel(plcNetworkNames[i], statusBarWidget);
		lbl->setStyleSheet(styleGrey);
		lbl->setAutoFillBackground(true);
		plcNetworkStatusLabels.insert(plcNetworkNames[i], lbl);
		statusBarLayout->addWidget(lbl);
	}

	sysAreaLabel = new QLabel("SYS:", statusBarWidget);
	sysAreaLabel->setStyleSheet(styleHeaderGrey);
	sysAreaLabel->setAutoFillBackground(true);
	statusBarLayout->addWidget(sysAreaLabel);
	// Timer status indicators (green=firing, grey=idle)
	mainTimerStatusLabel = new QLabel("mainTimer", statusBarWidget);
	mainTimerStatusLabel->setStyleSheet(styleGrey);
	mainTimerStatusLabel->setAutoFillBackground(true);
	statusBarLayout->addWidget(mainTimerStatusLabel);
	autoModeTimerStatusLabel = new QLabel("autoModeTimer", statusBarWidget);
	autoModeTimerStatusLabel->setStyleSheet(styleGrey);
	autoModeTimerStatusLabel->setAutoFillBackground(true);
	statusBarLayout->addWidget(autoModeTimerStatusLabel);
	statusBarLayout->addStretch(1);
	rootLayout->addWidget(statusBarWidget);
	setLayout(rootLayout);
	g_dbTableStatusTarget = this;
	g_plcNetworkStatusTarget = this;
	g_timerStatusTarget = this;

	// 250ms delayed fade-to-grey timers for status labels
	mainTimerIdleTimer = new QTimer(this);
	mainTimerIdleTimer->setSingleShot(true);
	connect(mainTimerIdleTimer, &QTimer::timeout, this, &mainWindowClass::onMainTimerIdleTimeout);
	autoModeTimerIdleTimer = new QTimer(this);
	autoModeTimerIdleTimer->setSingleShot(true);
	connect(autoModeTimerIdleTimer, &QTimer::timeout, this, &mainWindowClass::onAutoModeTimerIdleTimeout);

	//
	plcStatusWidget = new QWidget();
	plcWidget = new plcWidgetClass(plcStatusWidget);
	openGLWidget = new openGLWidgetClass(plcWidget->getCranePlcTcpServer(), plcWidget->getConveyorsPlcTcpServer(), 0, 0);
	mainLayout->addWidget(openGLWidget);
	plcStatusWidget->setParent(openGLWidget);

	// basket_details is also updated by other production applications. Keep the
	// efficient bulk cache, but periodically replace the snapshot and recalculate
	// the displayed state so empty/full baskets cannot remain stale indefinitely.
	basketDataRefreshTimer.setInterval(15000);
	connect(&basketDataRefreshTimer, &QTimer::timeout,
		this, &mainWindowClass::refreshBasketDataDisplay);
	basketDataRefreshTimer.start();
	LOG("BASKET CACHE", "Automatic display refresh enabled: interval=15000ms");

	QLabel *versionLabel = new QLabel(QString("VERSION  %1").arg(APEX_BASKET_CRANE_VERSION), openGLWidget);
	versionLabel->setObjectName("buildVersion");
	versionLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
	versionLabel->setAlignment(Qt::AlignCenter);
	versionLabel->setStyleSheet("#buildVersion{color:#cbd5e1;background:#111827;border:1px solid #334155;border-radius:6px;padding:7px 12px;font-size:9pt;font-weight:600;}");
	versionLabel->setToolTip(tr("Application and installer version"));

	QHBoxLayout *versionRow = new QHBoxLayout();
	versionRow->addStretch(1);
	versionRow->addWidget(versionLabel);
	QVBoxLayout *brandingOverlayLayout = new QVBoxLayout(openGLWidget);
	brandingOverlayLayout->setContentsMargins(0, 0, 22, 18);
	brandingOverlayLayout->addStretch(1);
	brandingOverlayLayout->addLayout(versionRow);

#ifdef TUX_PLC_COMMS
	// Show PLC networks as "connecting" (yellow) until first data or error
	setPlcNetworkConnecting(PLC_NETWORK_CRANE);
	setPlcNetworkConnecting(PLC_NETWORK_CONVEYORS);
	setPlcNetworkConnecting(PLC_NETWORK_NEW_OVEN);
#endif

	// Auto mode flags
	LOG("mW.cpp", "Auto mode flags");
	QVBoxLayout *autoGroupBoxLayout = new QVBoxLayout();
	autoGroupBoxLayout->setContentsMargins(10, 12, 10, 8);
	autoGroupBoxLayout->setSpacing(1);

	QCheckBox	*beforStackerQCheckBox = new QCheckBox(tr("Before stacker"), this);
	connect(beforStackerQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(beforeStackerAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(beforStackerQCheckBox);
	beforStackerQCheckBox->setChecked(false);

	QCheckBox	*afterStackerQCheckBox = new QCheckBox(tr("After stacker"), this);
	connect(afterStackerQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(afterStackerAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(afterStackerQCheckBox);
	afterStackerQCheckBox->setChecked(false);

	QCheckBox	*beforeStacker2QCheckBox = new QCheckBox(tr("Before stacker 2"), this);
	connect(beforeStacker2QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(beforeStacker2AutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(beforeStacker2QCheckBox);
	beforeStacker2QCheckBox->setChecked(false);

	QCheckBox	*afterStacker2QCheckBox = new QCheckBox(tr("After stacker 2"), this);
	connect(afterStacker2QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(afterStacker2AutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(afterStacker2QCheckBox);
	afterStacker2QCheckBox->setChecked(false);

	QCheckBox *pressToOvenQCheckBox = new QCheckBox(tr("Press# to oven#"), this);
	connect(pressToOvenQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(pressToOvenCheckBtnSlot(int)));
	autoGroupBoxLayout->addWidget(pressToOvenQCheckBox);
	pressToOvenQCheckBox->setChecked(false);

	QCheckBox	*unloadOvenQCheckBox = new QCheckBox(tr("Unload oven"), this);
	connect(unloadOvenQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(unloadOvenAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(unloadOvenQCheckBox);
	unloadOvenQCheckBox->setChecked(false);
	unloadOvenQCheckBox->setEnabled(true);

	QCheckBox	*unloadOven2QCheckBox = new QCheckBox(tr("Unload oven 2"), this);
	connect(unloadOven2QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(unloadOven2AutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(unloadOven2QCheckBox);
	unloadOven2QCheckBox->setChecked(false);
	unloadOven2QCheckBox->setEnabled(true);

	QCheckBox	*exportsQCheckBox=new QCheckBox(tr("Exports (full->out)"),this);
	connect(exportsQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(exportsAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(exportsQCheckBox);
	exportsQCheckBox->setChecked(false);

	QCheckBox	*returningEmptyQCheckBox=new QCheckBox(tr("Imports (empty<-in)"),this);
	connect(returningEmptyQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(returningEmptyAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(returningEmptyQCheckBox);
	returningEmptyQCheckBox->setChecked(false);

	//QCheckBox	*scrapQCheckBox=new QCheckBox(tr("Scrap"),this);
	//connect(scrapQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(scrapAutoModeSlot(int)));
	//autoGroupBoxLayout->addWidget(scrapQCheckBox);
	//scrapQCheckBox->setChecked(false);
	//scrapQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*fromBufferToOvenQCheckBox = new QCheckBox(tr("From buffer to oven"),this);
	connect(fromBufferToOvenQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(fromBufferToOvenAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(fromBufferToOvenQCheckBox);
	fromBufferToOvenQCheckBox->setChecked(false);

	QCheckBox	*exportsBeforeImportsQCheckBox = new QCheckBox(tr("Exports before imports"),this);
	connect(exportsBeforeImportsQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(exportsBeforeImportsCheckBtnSlot(int)));
	autoGroupBoxLayout->addWidget(exportsBeforeImportsQCheckBox);
	exportsBeforeImportsQCheckBox->setChecked(configuration.Operations().ExportsBeforeImports());

	QHBoxLayout *numberOfEmptyPosInBufferSpinBoxLayout=new QHBoxLayout();
	autoGroupBoxLayout->addLayout(numberOfEmptyPosInBufferSpinBoxLayout);
	QLabel *numberOfEmptyPosInBufferSpinBoxLabel=new QLabel(tr("Empty buffer"));
	numberOfEmptyPosInBufferSpinBoxLayout->addWidget(numberOfEmptyPosInBufferSpinBoxLabel);
	QSpinBox *numberOfEmptyPosInBufferSpinBox=new QSpinBox();
	numberOfEmptyPosInBufferSpinBox->setFixedWidth(64);
	numberOfEmptyPosInBufferSpinBox->setAlignment(Qt::AlignCenter);
	numberOfEmptyPosInBufferSpinBoxLayout->addStretch(1);
	numberOfEmptyPosInBufferSpinBoxLayout->addWidget(numberOfEmptyPosInBufferSpinBox);
	connect(numberOfEmptyPosInBufferSpinBox,SIGNAL(valueChanged(int)),this,SLOT(numberOfEmptyPosInBufferSpinBoxSlot(int)));
	numberOfEmptyPosInBufferSpinBox->setMinimum(configuration.Operations().EmptyBufferMinimum());
	numberOfEmptyPosInBufferSpinBox->setMaximum(configuration.Operations().EmptyBufferMaximum());
	numberOfEmptyPosInBufferSpinBox->setValue(configuration.Operations().EmptyBufferDefault());
	openGLWidget->setNumberOfPosToBeKeptEmptyInBuffer(numberOfEmptyPosInBufferSpinBox->value());

	QCheckBox *rearrangeBasketsQCheckBox = new QCheckBox(tr("Rearrange baskets"),this);
	connect(rearrangeBasketsQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(rearrangeBasketsCheckBtnSlot(int)));
	autoGroupBoxLayout->addWidget(rearrangeBasketsQCheckBox);
	rearrangeBasketsQCheckBox->setChecked(false);
	
	QGroupBox *autoGroupBox = new QGroupBox(tr("Automation"),openGLWidget);
	autoGroupBox->setGeometry(25, 20, 200, 310);
	autoGroupBox->setLayout(autoGroupBoxLayout);
	autoGroupBox->show();
	// End auto flags

	//
	int w = 185; int h = 27; int m = 5; int x = 25; int y = 340;

	QCheckBox *takeBasketsFromStackerOutQCheckBox = new QCheckBox(tr("New baskets out"), openGLWidget);
	connect(takeBasketsFromStackerOutQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(takeBasketsFromStackerOutSlot(int)));
	takeBasketsFromStackerOutQCheckBox->setGeometry(27, y, 170, 20);
	takeBasketsFromStackerOutQCheckBox->show();
	takeBasketsFromStackerOutQCheckBox->setChecked(false);

	////////////////////Use empty buffer
	LOG("mW.cpp", "Use empty buffer");
	QVBoxLayout *emptyBasketsPosGroupBoxLayout=new QVBoxLayout();

	useEmptyBasketsPosCheckBox = new QCheckBox(tr("Use"), this);
	useEmptyBasketsPosCheckBox->setChecked(false);
	useEmptyBasketsPosCheckBox->setToolTip(tr("Use the empty buffer area to store 20 feet baskets if necessary."));
	emptyBasketsPosGroupBoxLayout->addWidget(useEmptyBasketsPosCheckBox);
	connect(useEmptyBasketsPosCheckBox, SIGNAL(stateChanged(int)), this, SLOT(useEmptyBasketsPosCheckBoxSlot(int)));

	emptyEmptyBasketsPosCheckBox = new QCheckBox(tr("Empty"), this);
	emptyEmptyBasketsPosCheckBox->setChecked(false);
	emptyEmptyBasketsPosCheckBox->setToolTip(tr("Empty the empty buffer area from 20 feet baskets when you have time."));
	emptyBasketsPosGroupBoxLayout->addWidget(emptyEmptyBasketsPosCheckBox);
	connect(emptyEmptyBasketsPosCheckBox, SIGNAL(stateChanged(int)), this, SLOT(emptyEmptyBasketsPosCheckBoxSlot(int)));

	QGroupBox *fullBasketsToEmptyPosGroupBox=new QGroupBox(tr("20 ft empty-basket buffer"),openGLWidget);
	fullBasketsToEmptyPosGroupBox->setGeometry(25, y += 25 + m, 190, 90);
	fullBasketsToEmptyPosGroupBox->setLayout(emptyBasketsPosGroupBoxLayout);
	fullBasketsToEmptyPosGroupBox->show();

	// Oven selection flags
	LOG("mW.cpp", "Oven selection flags");
	QVBoxLayout *OvenSelGroupBoxLayout = new QVBoxLayout();

	QCheckBox *Oven1EnableQCheckBox = new QCheckBox(tr("Oven1 Enable"), this);
	connect(Oven1EnableQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven1EnableAutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven1EnableQCheckBox);
	Oven1EnableQCheckBox->setChecked(configuration.Operations().Oven1Enabled());

	QCheckBox	*Oven2EnableQCheckBox = new QCheckBox(tr("Oven2 Enable"), this);
	connect(Oven2EnableQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven2EnableAutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven2EnableQCheckBox);
	Oven2EnableQCheckBox->setChecked(configuration.Operations().Oven2Enabled());

	QCheckBox	*Oven1Not24QCheckBox = new QCheckBox(tr("Not longer than 24f in Oven1"), this);
	connect(Oven1Not24QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven1Not24AutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven1Not24QCheckBox);
	Oven1Not24QCheckBox->setChecked(configuration.Operations().Oven1Maximum24Feet());

	QCheckBox	*Oven2Not24QCheckBox = new QCheckBox(tr("Just Longer than 24f in Oven2"), this);
	connect(Oven2Not24QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven2Not24AutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven2Not24QCheckBox);
	Oven2Not24QCheckBox->setChecked(configuration.Operations().Oven2OnlyOver24Feet());

	QGroupBox *OvenSelGroupBox = new QGroupBox(tr("Oven routing"), openGLWidget);
	OvenSelGroupBox->setGeometry(235, 20, 245, 115);
	OvenSelGroupBox->setLayout(OvenSelGroupBoxLayout);
	OvenSelGroupBox->show();

	// Next oven 1 cycle
	LOG("mW.cpp", "Next oven 1 cycle");
	QVBoxLayout *nextOvenCycleGroupBoxLayout = new QVBoxLayout();

	QRadioButton *nextOvenCycle185RadioButton = new QRadioButton("185C 5 hours 30 minutes", this);
	connect(nextOvenCycle185RadioButton, SIGNAL(toggled(bool)), this, SLOT(nextOvenCycle185RadioButtonSlot(bool)));
	nextOvenCycleGroupBoxLayout->addWidget(nextOvenCycle185RadioButton);

	QRadioButton *nextOvenCycle230RadioButton = new QRadioButton("230C 8 hours 50 minutes", this);
	connect(nextOvenCycle230RadioButton, SIGNAL(toggled(bool)), this, SLOT(nextOvenCycle230RadioButtonSlot(bool)));
	nextOvenCycleGroupBoxLayout->addWidget(nextOvenCycle230RadioButton);

	if (openGLWidget->getNextOvenLoadingCycleTemperEnum() == allPositionsClass::NEXT_OVEN_CYCLE_TEMPER_185)
		nextOvenCycle185RadioButton->setChecked(true);
	else
		nextOvenCycle230RadioButton->setChecked(true);

	QGroupBox *nextOvenCycleGroupBox = new QGroupBox(tr("Oven 1 - next cycle"), openGLWidget);
	nextOvenCycleGroupBox->setGeometry(235, 140, 285, 82);
	nextOvenCycleGroupBox->setLayout(nextOvenCycleGroupBoxLayout);
	nextOvenCycleGroupBox->show();

	// Mext oven 2 cycle
	LOG("mW.cpp", "Next oven 2 cycle");
	QVBoxLayout *nextOven2CycleGroupBoxLayout = new QVBoxLayout();

	QRadioButton *nextOven2Cycle185RadioButton = new QRadioButton("185C 5 hours 30 minutes", this);
	connect(nextOven2Cycle185RadioButton, SIGNAL(toggled(bool)), this, SLOT(nextOven2Cycle185RadioButtonSlot(bool)));
	nextOven2CycleGroupBoxLayout->addWidget(nextOven2Cycle185RadioButton);

	QRadioButton *nextOven2Cycle230RadioButton = new QRadioButton("230C 8 hours 50 minutes", this);
	connect(nextOven2Cycle230RadioButton, SIGNAL(toggled(bool)), this, SLOT(nextOven2Cycle230RadioButtonSlot(bool)));
	nextOven2CycleGroupBoxLayout->addWidget(nextOven2Cycle230RadioButton);

	////////////////////////
	//What is next Oven2 Cycle
	//????????????????????????
	//?????????????????????????
	//??????????????????????????
	//////////////////////////////
	if (openGLWidget->getNextOvenLoadingCycleTemperEnum() == allPositionsClass::NEXT_OVEN_CYCLE_TEMPER_185)
		nextOven2Cycle185RadioButton->setChecked(true);
	else
		nextOven2Cycle230RadioButton->setChecked(true);

	QGroupBox *nextOven2CycleGroupBox = new QGroupBox(tr("Oven 2 - next cycle"), openGLWidget);
	nextOven2CycleGroupBox->setGeometry(235, 227, 285, 82);
	nextOven2CycleGroupBox->setLayout(nextOven2CycleGroupBoxLayout);
	nextOven2CycleGroupBox->show();

	//
	//takeScrapBasketBtn=new QPushButton(tr("Take scrap basket"),openGLWidget);takeScrapBasketBtn->setGeometry(x,y+=95+m,w,h);  takeScrapBasketBtn 	->show();
	y += 95 + m;
	QPushButton *unlockBasketAfterStackerBtn = new QPushButton(tr("Unlock after stacker"), openGLWidget);
	unlockBasketAfterStackerBtn->setProperty("warning", true);
	unlockBasketAfterStackerBtn->setGeometry(x, y += h + m, w, h);
	unlockBasketAfterStackerBtn->show();

	QPushButton *unlockBasketReturningFromOvenBtn = new QPushButton(tr("Unlock from oven"), openGLWidget);
	unlockBasketReturningFromOvenBtn->setProperty("warning", true);
	unlockBasketReturningFromOvenBtn->setGeometry(x, y += h + m, w, h);
	unlockBasketReturningFromOvenBtn->show();

	QPushButton *basketTrackerBtn = new QPushButton(tr("Find basket"), openGLWidget);
	basketTrackerBtn->setProperty("accent", true);
	basketTrackerBtn->setGeometry(x, y += h + m, w, h);
	basketTrackerBtn->setToolTip(tr("Locate a basket and review its recent movement history."));
	basketTrackerBtn->show();

	plcStatusPreferredY = y += h + m + 8;
	plcStatusWidget->setGeometry(x, plcStatusPreferredY, w, 310);
	plcStatusWidget->show();
	positionPlcStatusWidget();

	//connect(takeScrapBasketBtn,SIGNAL(clicked()),this,SLOT(takeScrapBasketBtnSlot()));
	LOG("mW.cpp", "connecting unlockBaset* SIGNAL(s)");
	connect(unlockBasketAfterStackerBtn, SIGNAL(clicked()), this, SLOT(unlockBasketAfterStackerBtnSlot()));
	connect(unlockBasketReturningFromOvenBtn, SIGNAL(clicked()), this, SLOT(unlockBasketReturningFromOvenBtnSlot()));
	connect(basketTrackerBtn, &QPushButton::clicked, this, &mainWindowClass::showBasketTrackerSlot);
	connect(openGLWidget, SIGNAL(scrapBasketPositionChangedSignal(int)), this, SLOT(scrapBasketPositionChangedSlot(int)));

#ifdef VB_PLC_COMMS
#ifndef EMULATE
	connect(plcWidget->getCranePlcTcpServer(),SIGNAL(incomingSignal(QString,int,QString)),this,SLOT(signalFromCranePlcSlot(QString,int,QString)));
	connect(plcWidget->getConveyorsPlcTcpServer(),SIGNAL(incomingSignal(QString,int,QString)),this,SLOT(signalFromConveyorsPlcSlot(QString,int,QString)));
#endif
#endif

#ifdef TUX_PLC_COMMS
	LOG("mW.cpp", "Using TUX_PLC_COMMS");
	connect(plcWidget->getCraneTuxClass(), SIGNAL(dataInPlcChangedSignal(QString, QString, int, QVariant)), this, SLOT(signalFromCraneTuxPlcSlot(QString, QString, int, QVariant)));
	connect(plcWidget->getConveyorTuxClass(), SIGNAL(dataInPlcChangedSignal(QString, QString, int, QVariant)), this, SLOT(signalFromConveyorsTuxPlcSlot(QString, QString, int, QVariant)));
	connect(plcWidget->getNewOvenTuxClass(), SIGNAL(dataInPlcChangedSignal(QString, QString, int, QVariant)), this, SLOT(signalFromConveyorsTuxPlcSlot(QString, QString, int, QVariant)));

#endif

	LOG("mW.cpp", "Setting mainTimer (15s)");
	connect(&mainTimer, SIGNAL(timeout()), this, SLOT(onMainTimer()));
	mainTimer.start(configuration.Timers().MainWindowMilliseconds());
	if (!observerMode)
		clearLogTable();
	scrapBasketPositionChangedSlot(openGLWidget->getBasketTableIdOfScrapBasketPos());

#ifdef TUX_PLC_COMMS
	LOG("mW.cpp", "Using TUX_PLC_COMMS (2nd ifdef)");
	QAction *showServerAction = new QAction("", this);
	addAction(showServerAction);
	showServerAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	connect(showServerAction, SIGNAL(triggered()), this, SLOT(showServerSlot()));

#ifdef EMULATE
	LOG("mW.cpp", "EMULATE");
	plcEmulateWindow = new plcEmulateWindowClass(plcWidget->getCraneTuxClass(), plcWidget->getConveyorTuxClass(), this);
	plcEmulateWindow->show();
#endif
	LOG("mW.cpp", "plcWidget->addTables()");
	plcWidget->addTables();

#ifdef EMULATE
	LOG("mW.cpp", "EMULATE");
	plcEmulateWindow->tuxIsReady();
#endif
#endif

#ifdef EMULATE
	//plcWidget->getCranePlcTcpServer()->sendCurrentValuesToPlc();
	//plcWidget->getConveyorsPlcTcpServer()->sendCurrentValuesToPlc();
#endif;

#ifdef VB_PLC_COMMS
	LOG("mW.cpp", "Using VB_PLC_COMMS");
#ifndef EMULATE
	plcWidget->startExternalSoft();
#endif;
#endif;

	QAction *qtAboutAction = new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
	connect(qtAboutAction, SIGNAL(triggered()), this, SLOT(qtAboutSlot()));
	addAction(qtAboutAction);

	QAction *basketTrackerAction = new QAction(this);
	basketTrackerAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
	connect(basketTrackerAction, &QAction::triggered, this, &mainWindowClass::showBasketTrackerSlot);
	addAction(basketTrackerAction);

	if (observerMode)
	{
		const QList<QAbstractButton*> controls = openGLWidget->findChildren<QAbstractButton*>();
		for (int i = 0; i < controls.count(); ++i)
			controls[i]->setEnabled(false);
		const QList<QSpinBox*> spinBoxes = openGLWidget->findChildren<QSpinBox*>();
		for (int i = 0; i < spinBoxes.count(); ++i)
			spinBoxes[i]->setEnabled(false);
		basketTrackerBtn->setEnabled(true);
	}
}

void mainWindowClass::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	QTimer::singleShot(0, this, [this]() { positionPlcStatusWidget(); });
}

void mainWindowClass::positionPlcStatusWidget()
{
	if (!openGLWidget || !plcStatusWidget)
		return;

	const int panelHeight = 310;
	const int bottomMargin = 8;
	const int maximumY = qMax(0, openGLWidget->height() - panelHeight - bottomMargin);
	const int panelY = qMin(plcStatusPreferredY, maximumY);
	plcStatusWidget->setGeometry(plcStatusWidget->x(), panelY, plcStatusWidget->width(), panelHeight);
	plcStatusWidget->raise();
}

mainWindowClass::~mainWindowClass()
{
#ifdef VB_PLC_COMMS
	terminateExternalProccesses();
#endif
	delete plcWidget;
}

void mainWindowClass::signalFromCraneTuxPlcSlot(QString ip, QString table, int index, QVariant value)
{
	setPlcNetworkActive(PLC_NETWORK_CRANE, true);
	LOG("mW.cpp", "signalFromCraneTuxPlcSlot(" + ip + ", " + table + ", " + QString::number(index) + ", " + value.toString() + ")");
#ifdef TUX_PLC_COMMS
	signalFromCranePlcSlot(table, index, value.toString());
#endif
#ifdef EMULATE
	mainWindowClass::plcEmulateWindow->cranePlcDataChanged(table, index, value.toString());
#endif
	setPlcNetworkIdle(PLC_NETWORK_CRANE);
}

void mainWindowClass::signalFromConveyorsTuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
	if (ip == conveyorsIp)
		setPlcNetworkActive(PLC_NETWORK_CONVEYORS, true);
	else if (ip == newOvenIp)
		setPlcNetworkActive(PLC_NETWORK_NEW_OVEN, true);
	LOG("mW.cpp", "signalFromConveyorsTuxPlcSlot(" + ip + ", " + table + ", " + index + ", " + value.toString() + ")");
#ifdef TUX_PLC_COMMS
	signalFromConveyorsPlcSlot(ip,table,index,value.toString());
#endif
#ifdef EMULATE
	plcEmulateWindow->conveyorPlcDataChanged(table,index,value.toString());
#endif
	if (ip == conveyorsIp)
		setPlcNetworkIdle(PLC_NETWORK_CONVEYORS);
	else if (ip == newOvenIp)
		setPlcNetworkIdle(PLC_NETWORK_NEW_OVEN);
}

void mainWindowClass::showServerSlot()
{
	LOG("mW.cpp", "showServerSlot()");
#ifdef TUX_PLC_COMMS
	plcWidget->showTuxServerCommsDialog();
#endif
}

void mainWindowClass::showBasketTrackerSlot()
{
	QDialog dialog(this);
	dialog.setWindowTitle(tr("Basket Tracker"));
	dialog.setWindowIcon(QIcon(":/logo_16x16.png"));
	dialog.resize(900, 650);

	QVBoxLayout *layout = new QVBoxLayout(&dialog);
	QHBoxLayout *searchLayout = new QHBoxLayout();
	QLabel *basketIdLabel = new QLabel(tr("Basket ID:"), &dialog);
	QLineEdit *basketIdEdit = new QLineEdit(&dialog);
	basketIdEdit->setValidator(new QIntValidator(1, 2147483647, basketIdEdit));
	basketIdEdit->setPlaceholderText(tr("Enter the basket table ID"));
	QPushButton *searchButton = new QPushButton(tr("Search"), &dialog);
	searchButton->setProperty("accent", true);
	searchButton->setDefault(true);
	searchLayout->addWidget(basketIdLabel);
	searchLayout->addWidget(basketIdEdit, 1);
	searchLayout->addWidget(searchButton);
	layout->addLayout(searchLayout);

	QLabel *statusLabel = new QLabel(tr("Enter a basket ID to locate it."), &dialog);
	statusLabel->setWordWrap(true);
	statusLabel->setMinimumHeight(52);
	statusLabel->setStyleSheet("QLabel{font-size:14px;font-weight:bold;background:#e5e7eb;color:#111827;padding:8px;border-radius:3px;}");
	layout->addWidget(statusLabel);

	QLabel *positionsLabel = new QLabel(tr("Current recorded position(s)"), &dialog);
	positionsLabel->setStyleSheet("QLabel{font-weight:bold;}");
	layout->addWidget(positionsLabel);

	QTableWidget *positionsTable = new QTableWidget(&dialog);
	positionsTable->setColumnCount(6);
	positionsTable->setHorizontalHeaderLabels(QStringList()
		<< tr("Description") << tr("Position") << tr("Index")
		<< tr("Locked") << tr("Mission target") << tr("Target index"));
	positionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	positionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	positionsTable->setAlternatingRowColors(true);
	positionsTable->horizontalHeader()->setStretchLastSection(true);
	positionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	layout->addWidget(positionsTable, 2);

	QLabel *historyLabel = new QLabel(tr("Recent movement history"), &dialog);
	historyLabel->setStyleSheet("QLabel{font-weight:bold;}");
	layout->addWidget(historyLabel);

	QTableWidget *historyTable = new QTableWidget(&dialog);
	historyTable->setColumnCount(4);
	historyTable->setHorizontalHeaderLabels(QStringList()
		<< tr("Event") << tr("From / position") << tr("Destination") << tr("Date"));
	historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	historyTable->setAlternatingRowColors(true);
	historyTable->horizontalHeader()->setStretchLastSection(true);
	historyTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	historyTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
	layout->addWidget(historyTable, 3);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
	connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
	layout->addWidget(buttonBox);

	auto setTableText = [](QTableWidget *table, int row, int column, const QString& text)
	{
		QTableWidgetItem *item = new QTableWidgetItem(text.trimmed());
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		table->setItem(row, column, item);
	};

	auto searchBasket = [=]()
	{
		bool basketIdIsValid = false;
		int basketId = basketIdEdit->text().toInt(&basketIdIsValid);
		if (!basketIdIsValid || basketId <= 0)
		{
			statusLabel->setText(tr("Enter a valid positive basket ID."));
			statusLabel->setStyleSheet("QLabel{font-size:14px;font-weight:bold;background:#fef3c7;color:#92400e;padding:8px;border-radius:3px;}");
			basketIdEdit->setFocus();
			return;
		}

		positionsTable->setRowCount(0);
		historyTable->setRowCount(0);

		QString positionOrder = " order by posNumber, posIndex";
		QString positionWhere = QString(" from positions where basketTableId = %1").arg(basketId);

		setDbTableActive("positions", true);
		QVector<QVariant> positionDescriptions = execDBQuery("select description" + positionWhere + positionOrder, MAIN_DB_CONNECTION);
		QVector<QVariant> positionNumbers = execDBQuery("select posNumber" + positionWhere + positionOrder, MAIN_DB_CONNECTION);
		QVector<QVariant> positionIndexes = execDBQuery("select posIndex" + positionWhere + positionOrder, MAIN_DB_CONNECTION);
		QVector<QVariant> positionLocks = execDBQuery("select locked" + positionWhere + positionOrder, MAIN_DB_CONNECTION);
		QVector<QVariant> targetNumbers = execDBQuery("select targetNumber" + positionWhere + positionOrder, MAIN_DB_CONNECTION);
		QVector<QVariant> targetIndexes = execDBQuery("select targetIndex" + positionWhere + positionOrder, MAIN_DB_CONNECTION);
		int activeMissionCount = GETONEATMOST("select count(*) from positions where targetNumber <> 0").toInt();
		setDbTableIdle("positions");

		int positionCount = positionNumbers.count();
		positionsTable->setRowCount(positionCount);
		for (int row = 0; row < positionCount; ++row)
		{
			setTableText(positionsTable, row, 0, row < positionDescriptions.count() ? positionDescriptions[row].toString() : QString());
			setTableText(positionsTable, row, 1, positionNumbers[row].toString());
			setTableText(positionsTable, row, 2, row < positionIndexes.count() ? positionIndexes[row].toString() : QString());
			setTableText(positionsTable, row, 3, row < positionLocks.count() && positionLocks[row].toInt() == basketPositionClass::LOCKED ? tr("Yes") : tr("No"));
			setTableText(positionsTable, row, 4, row < targetNumbers.count() && targetNumbers[row].toInt() != 0 ? targetNumbers[row].toString() : QString());
			setTableText(positionsTable, row, 5, row < targetIndexes.count() && targetIndexes[row].toInt() != 0 ? targetIndexes[row].toString() : QString());
		}

		setDbTableActive("baskets", true);
		int basketRecordCount = GETONEATMOST(QString("select count(*) from baskets where pid = %1").arg(basketId)).toInt();
		setDbTableIdle("baskets");

		setDbTableActive("toAssimakisExports", true);
		int assimakisExportCount = GETONEATMOST(QString("select count(*) from toAssimakisExports where basketTableId = %1").arg(basketId)).toInt();
		setDbTableIdle("toAssimakisExports");
		setDbTableActive("toOmavExports", true);
		int omavExportCount = GETONEATMOST(QString("select count(*) from toOmavExports where basketTableId = %1").arg(basketId)).toInt();
		setDbTableIdle("toOmavExports");

		QString historyWhere = QString(" from basketHistory where basketTableId = %1").arg(basketId);
		QString historyOrder = " order by sec desc, id desc";
		setDbTableActive("basketHistory", true);
		QVector<QVariant> historyTypes = execDBQuery("select top 50 type" + historyWhere + historyOrder, MAIN_DB_CONNECTION);
		QVector<QVariant> historyPositions = execDBQuery("select top 50 position" + historyWhere + historyOrder, MAIN_DB_CONNECTION);
		QVector<QVariant> historyDestinations = execDBQuery("select top 50 destination" + historyWhere + historyOrder, MAIN_DB_CONNECTION);
		QVector<QVariant> historyDates = execDBQuery("select top 50 date" + historyWhere + historyOrder, MAIN_DB_CONNECTION);
		setDbTableIdle("basketHistory");

		int historyCount = historyTypes.count();
		historyTable->setRowCount(historyCount);
		for (int row = 0; row < historyCount; ++row)
		{
			setTableText(historyTable, row, 0, historyTypes[row].toString());
			setTableText(historyTable, row, 1, row < historyPositions.count() ? historyPositions[row].toString() : QString());
			setTableText(historyTable, row, 2, row < historyDestinations.count() ? historyDestinations[row].toString() : QString());
			setTableText(historyTable, row, 3, row < historyDates.count() ? historyDates[row].toString() : QString());
		}

		QString latestEvent = historyCount > 0 ? historyTypes[0].toString().trimmed().toLower() : QString();
		QString latestDestination = historyDestinations.count() > 0 ? historyDestinations[0].toString().trimmed() : QString();
		QString statusText;
		QString statusStyle;

		if (positionCount > 1)
		{
			statusText = tr("DUPLICATE RECORD: basket %1 is assigned to %2 positions. Stop automatic handling and reconcile the database with the physical basket.")
				.arg(basketId).arg(positionCount);
			statusStyle = "QLabel{font-size:14px;font-weight:bold;background:#fee2e2;color:#991b1b;padding:8px;border-radius:3px;}";
		}
		else if (positionCount == 1 && latestEvent == "new" && activeMissionCount > 0)
		{
			statusText = tr("IN TRANSIT: basket %1 is associated with an active crane mission. Recorded position: %2-%3.")
				.arg(basketId).arg(positionNumbers[0].toInt()).arg(positionIndexes.value(0).toInt());
			statusStyle = "QLabel{font-size:14px;font-weight:bold;background:#dbeafe;color:#1e40af;padding:8px;border-radius:3px;}";
		}
		else if (positionCount == 1)
		{
			statusText = tr("LOCATED: basket %1 is recorded at %2-%3 (%4).")
				.arg(basketId).arg(positionNumbers[0].toInt()).arg(positionIndexes.value(0).toInt())
				.arg(positionDescriptions.value(0).toString().trimmed());
			statusStyle = "QLabel{font-size:14px;font-weight:bold;background:#dcfce7;color:#166534;padding:8px;border-radius:3px;}";
		}
		else if (assimakisExportCount > 0 || omavExportCount > 0)
		{
			QStringList queues;
			if (assimakisExportCount > 0) queues << tr("Assimakis");
			if (omavExportCount > 0) queues << tr("Omav");
			statusText = tr("QUEUED FOR EXPORT: basket %1 has no internal position and is present in the %2 export queue.")
				.arg(basketId).arg(queues.join(" / "));
			statusStyle = "QLabel{font-size:14px;font-weight:bold;background:#fef3c7;color:#92400e;padding:8px;border-radius:3px;}";
		}
		else if (basketRecordCount > 0)
		{
			statusText = tr("NOT LOCATED: basket %1 exists in the basket register but is not assigned to any internal position or export queue.")
				.arg(basketId);
			if (!latestDestination.isEmpty())
				statusText += tr(" Last recorded destination: %1.").arg(latestDestination);
			statusText += tr(" Verify whether it physically left the system before marking it missing.");
			statusStyle = "QLabel{font-size:14px;font-weight:bold;background:#ffedd5;color:#9a3412;padding:8px;border-radius:3px;}";
		}
		else if (historyCount > 0)
		{
			statusText = tr("HISTORY ONLY: basket %1 has movement history but no basket-register or current-position record. Database reconciliation is required.")
				.arg(basketId);
			statusStyle = "QLabel{font-size:14px;font-weight:bold;background:#fee2e2;color:#991b1b;padding:8px;border-radius:3px;}";
		}
		else
		{
			statusText = tr("UNKNOWN BASKET: no basket, position, export-queue, or history record was found for ID %1.").arg(basketId);
			statusStyle = "QLabel{font-size:14px;font-weight:bold;background:#e5e7eb;color:#374151;padding:8px;border-radius:3px;}";
		}

		statusLabel->setText(statusText);
		statusLabel->setStyleSheet(statusStyle);
	};

	connect(searchButton, &QPushButton::clicked, searchBasket);
	connect(basketIdEdit, &QLineEdit::returnPressed, searchBasket);
	basketIdEdit->setFocus();
	dialog.exec();
}

//for backwards compatibility
void mainWindowClass::signalFromCranePlcSlot(QString table,int index,QString value)
{
	LOG("mW.cpp", "signalFromCranePlcSlot(" + table + ", " + QString::number(index) + ", " + value + ")");
	if (table == "N20") {
		setDbTableActive("logCranePlc", false);
		execDBQuery(QString("insert into logCranePlc (address,value,X,Y,Z,date,sec) values('%1[%2]',%3,%4,%5,%6,'%7',%8)")
			.arg(table).arg(index).arg(value).arg(openGLWidget->getCraneX()).arg(openGLWidget->getCraneY()).arg(openGLWidget->getCraneZ()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION);
		setDbTableIdle("logCranePlc");
	}

	openGLWidget->signalFromCranePlc(table, index, value);
	plcWidget->signalFromCranePlc(table, index, value);

	if (table == "N20")
	{
		if (index == 22) {
			setDbTableActive("logResetPlc", false);
			execDBQuery(QString("insert into logResetPlc (type,value,date,sec) values('Z',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")
				+ QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION);
			setDbTableIdle("logResetPlc");
		}

		if (index == 23) {
			setDbTableActive("logResetPlc", false);
			execDBQuery(QString("insert into logResetPlc (type,value,date,sec) values('X',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")
				+ QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION);
			setDbTableIdle("logResetPlc");
		}

		if (index == 24) {
			setDbTableActive("logResetPlc", false);
			execDBQuery(QString("insert into logResetPlc (type,value,date,sec) values('Y',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")
				+ QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION);
			setDbTableIdle("logResetPlc");
		}
	}
}

//for backwards compatibility
void mainWindowClass::signalFromConveyorsPlcSlot(QString ip, QString table, int index, QString value)
{
	LOG("mW.cpp", "signalFromConveyorsTuxPlcSlot(" + ip + ", " + table + ", " + QString::number(index) + ", " + value + ")");
	if (ip == conveyorsIp) {
		LOG("mW.cpp", "signalFromConveyorsTuxPlcSlot() - CONVEYOR");
		openGLWidget->signalFromConveyorsPlc(table, index, value);
	}

	if (ip == newOvenIp) {
		LOG("mW.cpp", "signalFromConveyorsTuxPlcSlot() - NEWOVEN");
		openGLWidget->signalFromNewOvenConveyorsPlc(table, index, value);
	}
}

void mainWindowClass::clearLogTable()
{
#ifndef EMULATE
	setDbTableActive("basketHistory", false);
	execDBQuery(QString("delete from basketHistory where id not in (select top 100000 id from basketHistory order by sec desc)"), MAIN_DB_CONNECTION);
	setDbTableIdle("basketHistory");
#endif
}

static const QString s_dbTableStyleGrey("QLabel{font-size:10px;background:#1e293b;color:#94a3b8;border:1px solid #334155;padding:2px 6px;border-radius:5px;}");

void mainWindowClass::setDbTableActive(const QString& tableName, bool isRead)
{
	QMap<QString, QTimer*>::iterator ti = dbTableIdleTimers.find(tableName);
	if (ti != dbTableIdleTimers.end()) {
		(*ti)->stop();
	}
	QMap<QString, QLabel*>::iterator it = dbTableStatusLabels.find(tableName);
	if (it != dbTableStatusLabels.end()) {
		QString style = isRead
			? QString("QLabel{font-size:10px;background:#166534;color:#dcfce7;border:1px solid #22c55e;padding:2px 6px;border-radius:5px;}")
			: QString("QLabel{font-size:10px;background:#075985;color:#e0f2fe;border:1px solid #38bdf8;padding:2px 6px;border-radius:5px;}");
		(*it)->setStyleSheet(style);
	}
}

void mainWindowClass::setDbTableIdle(const QString& tableName)
{
	QTimer*& timer = dbTableIdleTimers[tableName];
	if (!timer) {
		timer = new QTimer(this);
		timer->setSingleShot(true);
		QString key = tableName;
		connect(timer, &QTimer::timeout, this, [this, key]() { setDbTableIdleNow(key); });
	}
	timer->stop();
	timer->start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().StatusFadeMilliseconds());
}

void mainWindowClass::setDbTableIdleNow(const QString& tableName)
{
	QMap<QString, QLabel*>::iterator it = dbTableStatusLabels.find(tableName);
	if (it != dbTableStatusLabels.end())
		(*it)->setStyleSheet(s_dbTableStyleGrey);
}

static const QString s_plcNetworkStyleYellow("QLabel{font-size:10px;background:#78350f;color:#fef3c7;border:1px solid #f59e0b;padding:2px 6px;border-radius:5px;}");
static const QString s_plcNetworkStyleRed("QLabel{font-size:10px;background:#7f1d1d;color:#fee2e2;border:1px solid #ef4444;padding:2px 6px;border-radius:5px;}");

void mainWindowClass::setPlcNetworkActive(const QString& networkName, bool isRead)
{
	int r = 0;
	if (isRead) {
		r = 1;
	}
	LOG("mW.cpp", "INFO: PLC Network Active:" + networkName + " isRead=" + QString::number(isRead) + "|" + QString::number(r));
	LOG("mW.cpp", "INFO: removing errors (setPlcNetworkActive)");
	plcNetworkInError.remove(networkName);
	LOG("mW.cpp", "INFO: removing network connecting");
	plcNetworkConnecting.remove(networkName);
	LOG("mW.cpp", "INFO: Setting up plcNetworkIdleTimers...");
	QMap<QString, QTimer*>::iterator ti = plcNetworkIdleTimers.find(networkName);
	if (ti != plcNetworkIdleTimers.end()) {
		LOG("mW.cpp", "INFO: Stopping timer for network " + networkName);
		(*ti)->stop();
	}
	QMap<QString, QLabel*>::iterator it = plcNetworkStatusLabels.find(networkName);
	if (it != plcNetworkStatusLabels.end()) {
		LOG("mW.cpp", "INFO: Toggling read status LED " + networkName);
		QString style = isRead
			? QString("QLabel{font-size:10px;background:#166534;color:#dcfce7;border:1px solid #22c55e;padding:2px 6px;border-radius:5px;}")
			: QString("QLabel{font-size:10px;background:#075985;color:#e0f2fe;border:1px solid #38bdf8;padding:2px 6px;border-radius:5px;}");
		(*it)->setStyleSheet(style);
		LOG("mW.cpp", "Done toggling read stauts LED " + networkName);
	}
}

void mainWindowClass::setPlcNetworkIdle(const QString& networkName)
{
	
	LOG("mW.cpp", "INFO: PLC Network Is Idle:" + networkName);
	LOG("mW.cpp", "INFO: removing errors (setPlcNetworkIdle)");
	plcNetworkInError.remove(networkName);
	LOG("mW.cpp", "INFO: removing network connecting");
	plcNetworkConnecting.remove(networkName);
	QTimer*& timer = plcNetworkIdleTimers[networkName];
	LOG("mW.cpp", "INFO: Checking for timer...");
	if (!timer) {
		LOG("mW.cpp", "INFO: Creating a new timer");
		timer = new QTimer(this);
		timer->setSingleShot(true);
		QString key = networkName;
		connect(timer, &QTimer::timeout, this, [this, key]() { setPlcNetworkIdleNow(key); });
		LOG("mW.cpp", "INFO: Timer succesfully created");
	}
	else {
		LOG("mW.cpp", "INFO: Timer already existed");
	}
	LOG("mW.cpp", "INFO: Stopping previous timer");
	timer->stop();
	LOG("mW.cpp", "INFO: starting new timer");
	timer->start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().StatusFadeMilliseconds());
	LOG("mW.cpp", "INFO: PLC Network Idle Timer Started for " + networkName);
	LOG("mW.cpp", "INFO: PLC Network Is Idle END FUNCTION");
}

void mainWindowClass::setPlcNetworkIdleNow(const QString& networkName)
{
	LOG("mW.cpp", "INFO: PLC Network Idle Now: " + networkName);
	if (plcNetworkInError.contains(networkName) || plcNetworkConnecting.contains(networkName)) {
		
		return;
	}
	QMap<QString, QLabel*>::iterator it = plcNetworkStatusLabels.find(networkName);
	if (it != plcNetworkStatusLabels.end())
		(*it)->setStyleSheet(s_dbTableStyleGrey);
}

void mainWindowClass::setPlcNetworkConnecting(const QString& networkName)
{
	LOG("mW.cpp", "INFO: PLC Network Connecting: " + networkName);
	plcNetworkInError.remove(networkName);
	plcNetworkConnecting.insert(networkName);
	QMap<QString, QTimer*>::iterator ti = plcNetworkIdleTimers.find(networkName);
	if (ti != plcNetworkIdleTimers.end())
		(*ti)->stop();
	QMap<QString, QLabel*>::iterator it = plcNetworkStatusLabels.find(networkName);
	if (it != plcNetworkStatusLabels.end())
		(*it)->setStyleSheet(s_plcNetworkStyleYellow);
}

void mainWindowClass::setPlcNetworkError(const QString& networkName)
{
	LOG("mW.cpp", "INFO: PLC Network Error: " + networkName);
	plcNetworkConnecting.remove(networkName);
	plcNetworkInError.insert(networkName);
	QMap<QString, QTimer*>::iterator ti = plcNetworkIdleTimers.find(networkName);
	if (ti != plcNetworkIdleTimers.end())
		(*ti)->stop();
	QMap<QString, QLabel*>::iterator it = plcNetworkStatusLabels.find(networkName);
	if (it != plcNetworkStatusLabels.end())
		(*it)->setStyleSheet(s_plcNetworkStyleRed);
}

static const QString s_timerStyleGrey("QLabel{font-size:10px;background:#1e293b;color:#94a3b8;border:1px solid #334155;padding:2px 6px;border-radius:5px;}");
static const QString s_timerStyleGreen("QLabel{font-size:10px;background:#166534;color:#dcfce7;border:1px solid #22c55e;padding:2px 6px;border-radius:5px;}");

void mainWindowClass::setMainTimerFiring()
{
	mainTimerIdleTimer->stop();
	if (mainTimerStatusLabel) mainTimerStatusLabel->setStyleSheet(s_timerStyleGreen);
}

void mainWindowClass::setMainTimerIdle()
{
	mainTimerIdleTimer->stop();
	mainTimerIdleTimer->start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().StatusFadeMilliseconds());
}

void mainWindowClass::onMainTimerIdleTimeout()
{
	if (mainTimerStatusLabel) mainTimerStatusLabel->setStyleSheet(s_timerStyleGrey);
}

void mainWindowClass::setAutoModeTimerFiring()
{
	autoModeTimerIdleTimer->stop();
	if (autoModeTimerStatusLabel) autoModeTimerStatusLabel->setStyleSheet(s_timerStyleGreen);
}

void mainWindowClass::setAutoModeTimerIdle()
{
	autoModeTimerIdleTimer->stop();
	autoModeTimerIdleTimer->start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().StatusFadeMilliseconds());
}

void mainWindowClass::onAutoModeTimerIdleTimeout()
{
	if (autoModeTimerStatusLabel) autoModeTimerStatusLabel->setStyleSheet(s_timerStyleGrey);
}

void mainWindowClass::refreshBasketDataDisplay()
{
	if (!openGLWidget)
		return;
	LOG("BASKET CACHE", "Periodic basket data refresh started");
	loadBasketDataCache();
	openGLWidget->recalculateBasketContentsFromCache();
	LOG("BASKET CACHE", "Periodic basket data refresh completed");
}

void mainWindowClass::takeScrapBasketBtnSlot()
{
	// LOG("mW.cpp", "takeScrapBasket()");
		openGLWidget->takeScrapBasket();
}

void mainWindowClass::unlockBasketAfterStackerBtnSlot()
{
	// LOG("mW.cpp", "unlockBasketAfterStackerBtnSlot()");
		openGLWidget->unlockBasketAfterStacker();
}

void mainWindowClass::unlockBasketReturningFromOvenBtnSlot()
{
	// LOG("mW.cpp", "unlockBasketReturningFromOvenBtnSlot()");
		openGLWidget->unlockBasketReturningFromOven();
}

void mainWindowClass::scrapBasketPositionChangedSlot(int basketTableId)
{
	// LOG("mW.cpp", "scrapBasketPositionChangedSlot(basketTableID=" + QString(basketTableId) + ") == NO OP!");
	//if (basketTableId>0)
	//	takeScrapBasketBtn->setEnabled(true);
	//else
	//	takeScrapBasketBtn->setEnabled(false);
}

// Stacker 1
void mainWindowClass::takeBasketsFromStackerOutSlot(int checkState)
{
	// LOG("mW.cpp", "takeBasketsFromStackerOutSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->setTakeBasketsFromStackerOut(false);

	if (checkState == Qt::Checked)
		openGLWidget->setTakeBasketsFromStackerOut(true);
}
void mainWindowClass::beforeStackerAutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "beforeStackerAutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_BEFORE_STACKER);

	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_BEFORE_STACKER);
}
void mainWindowClass::afterStackerAutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "afterStackerAutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_AFTER_STACKER);

	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_AFTER_STACKER);
}

// Stacker 2
void mainWindowClass::beforeStacker2AutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "beforeStacker2AutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_BEFORE_STACKER2);

	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_BEFORE_STACKER2);
}
void mainWindowClass::afterStacker2AutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "afterStacker2AutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_AFTER_STACKER2);

	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_AFTER_STACKER2);
}


//////////////////////////////////////////////////////
// Added by Vahid
//////////////////////////////////////////////////////
void mainWindowClass::Oven1EnableAutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "Oven1EnableAutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_OVEN1_ENABLE);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_OVEN1_ENABLE);
}

void mainWindowClass::Oven2EnableAutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "Oven2EnableAutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_OVEN2_ENABLE);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_OVEN2_ENABLE);
}

void mainWindowClass::Oven1Not24AutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "Oven1Not24AutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_OVEN1_NOT24);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_OVEN1_NOT24);
}

void mainWindowClass::Oven2Not24AutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "Oven2Not24AutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_OVEN2_ONLY24);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_OVEN2_ONLY24);
}

void mainWindowClass::pressToOvenCheckBtnSlot(int checkState)
{
	// LOG("mW.cpp", "pressToOvenCheckBtnSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_PRESSTOOVEN_ENABLE);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_PRESSTOOVEN_ENABLE);
}

void mainWindowClass::nextOven2Cycle230RadioButtonSlot(bool state)
{
	// LOG("mW.cpp", "nextOven2Cycle230RadioButtonSlot(" + QString(state) + ")");
	if (state)
		openGLWidget->setNextOven2LoadingCycleTemperEnum(allPositionsClass::NEXT_OVEN_CYCLE_TEMPER_230);
}

void mainWindowClass::nextOven2Cycle185RadioButtonSlot(bool state)
{
	// LOG("mW.cpp", "nextOven2Cycle185RadioButtonSlot(" + QString(state) + ")");
	if (state)
		openGLWidget->setNextOven2LoadingCycleTemperEnum(allPositionsClass::NEXT_OVEN_CYCLE_TEMPER_185);
}
//////////////////////////////////////////////////////
// End of Code Added by Vahid
//////////////////////////////////////////////////////

void mainWindowClass::unloadOvenAutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "unloadOvenAutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_UNLOAD_OVEN);

	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_UNLOAD_OVEN);
}

void mainWindowClass::unloadOven2AutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "unloadOven2AutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_UNLOAD_OVEN2);

	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_UNLOAD_OVEN2);
}

void mainWindowClass::exportsAutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "exportsAutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_EXPORTS);

	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_EXPORTS);
}

void mainWindowClass::returningEmptyAutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "returningEmptyAutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_RETURNING_EMPTY);

	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_RETURNING_EMPTY);
}

void mainWindowClass::scrapAutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "scrapAutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_SCRAP);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_SCRAP);
}

void mainWindowClass::fromBufferToOvenAutoModeSlot(int checkState)
{
	// LOG("mW.cpp", "fromBufferToOvenAutoModeSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_FROM_BUFFER_TO_OVEN);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_FROM_BUFFER_TO_OVEN);
}

void mainWindowClass::exportsBeforeImportsCheckBtnSlot(int checkState)
{
	// LOG("mW.cpp", "exportsBeforeImportsCheckBtnSlot(" + QString(checkState) + ")");
	openGLWidget->setExportBeforeImports(checkState == Qt::Checked);
}

void mainWindowClass::rearrangeBasketsCheckBtnSlot(int checkState)
{
	// LOG("mW.cpp", "rearrangeBasketsCheckBtnSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_MOVE_24F_FROM30F_TO_24F_POS);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_MOVE_24F_FROM30F_TO_24F_POS);
}

void mainWindowClass::numberOfEmptyPosInBufferSpinBoxSlot(int n)
{
	// LOG("mW.cpp", "numberOfEmptyPosInBufferSpinBoxSlot(" + QString(n) + ")");
	openGLWidget->setNumberOfPosToBeKeptEmptyInBuffer(n);
}

void mainWindowClass::nextOvenCycle230RadioButtonSlot(bool state)
{
	// LOG("mW.cpp", "nextOvenCycle230RadioButtonSlot(" + QString(state) + ")");
	if (state)
		openGLWidget->setNextOvenLoadingCycleTemperEnum(allPositionsClass::NEXT_OVEN_CYCLE_TEMPER_230);
}

void mainWindowClass::nextOvenCycle185RadioButtonSlot(bool state)
{
	// LOG("mW.cpp", "nextOvenCycle185RadioButtonSlot(" + QString(state) + ")");
	if (state)
		openGLWidget->setNextOvenLoadingCycleTemperEnum(allPositionsClass::NEXT_OVEN_CYCLE_TEMPER_185);
}

void mainWindowClass::emptyEmptyBasketsPosCheckBoxSlot(int checkState)
{
	// LOG("mW.cpp", "emptyEmptyBasketsPosCheckBoxSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Checked)
	{
		useEmptyBasketsPosCheckBox->setChecked(false);
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_EMPTY_EMPTYBASKETS_POS);
	}
	else
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_EMPTY_EMPTYBASKETS_POS);
}

void mainWindowClass::useEmptyBasketsPosCheckBoxSlot(int checkState)
{
	// LOG("mW.cpp", "useEmptyBasketsPosCheckBoxSlot(" + QString(checkState) + ")");
	if (checkState == Qt::Checked)
	{
		emptyEmptyBasketsPosCheckBox->setChecked(false);
		openGLWidget->addToAutoMode(allPositionsClass::AUTO_USE_EMPTYBASKETS_POS);
	}
	else
		openGLWidget->removeFromAutoMode(allPositionsClass::AUTO_USE_EMPTYBASKETS_POS);
}

void mainWindowClass::plcDlgSlot()
{
	// LOG("mW.cpp", "plcDlgSlot()");
	plcWidget->show();
}

void mainWindowClass::onMainTimer()
{
	// LOG("mW.cpp", "onMainTimer()");
	if (theOneBCToRuleThemAll) {
		setWindowTitle(tr("apex basketCrane, assimakis sa ", "main window title..") + QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss"));
	}
	else {
		setWindowTitle(tr("(NOT ENABLED) apex basketCrane, assimakis sa ", "main window title..") + QDateTime::currentDateTime().toString("yyyy - MM - dd") + QTime::currentTime().toString(" hh:mm:ss"));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////database///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool tableExists(QString tableName, QString connection)
{
	// LOG("mW.cpp", "tableExists(" + tableName + ", " + connection + ")");
#ifdef EMULATE
//	QVector<QVariant> tableArray = execQuery(QString("SELECT name FROM sqlite_master WHERE name='")+tableName+"'",connection);
	//QVector<QVariant> tableArray = execQuery(QString("SELECT [name] FROM sys.tables WHERE [name] = '") + tableName + "'", connection);
	// 
	//if (tableArray.count() == 0) //table does not exist 
	//	return false;

	return true;
#else
	QVector<QVariant> tableArray = execDBQuery(QString("SELECT [name] FROM sys.tables WHERE [name] ='") + tableName + "'", connection);
	if (tableArray.count() == 0)//table does not exist 
		return false;

	return true;
#endif
}

void createTable(QString tableName,QStringList colNames,QStringList colTypes,QString connection)
{
	// LOG("mW.cpp", "createTable(" + tableName + ", colNames, colTypes, " + connection + ")");
#ifdef EMULATE
	// QVector<QVariant> tableArray=execQuery(QString("SELECT name FROM sqlite_master WHERE name='")+tableName+"'",connection);
	// QVector<QVariant> tableArray = execQuery(QString("SELECT [name] FROM sys.tables WHERE [name] = '") + tableName + "'", connection);
	if (!tableExists(tableName, connection))  //table does not exist 
	{	
		QString stmnt = QString("CREATE TABLE ") + tableName + " (";
		for (int i=0; i < colNames.length(); i++)
		{
			stmnt += colNames[i] + " " + colTypes[i];
			if (i != (colNames.length() - 1))
				stmnt += ",";
		}
		stmnt += ")";
		execQuery(stmnt, connection);
	}
#else
	QString stmnt = QString("IF NOT EXISTS (SELECT [name] FROM sys.tables WHERE [name] = '")+tableName+"') ";
	stmnt += QString("CREATE TABLE ")+tableName+" (";
	for (int i=0; i < colNames.length(); i++)
	{
		stmnt += colNames[i] + " " + colTypes[i];
		if (i != (colNames.length() - 1))
			stmnt += ",";
	}
	stmnt += ")";
	execDBQuery(stmnt, connection);
#endif
}


#ifdef EMULATE
void addEmptyBasket(int posNumber,int posIndex,int basketNumber)
{
	setDbTableActive("positions", false);
	execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(posNumber).arg(posIndex).arg(basketNumber).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
	setDbTableIdle("positions");
	//execQuery(QString("insert into baskets (number,type) values(%1,%2)").arg(basketNumber).arg(BASKET_NORMAL),MAIN_DB_CONNECTION);
}
void addFullBasket(int posNumber,int posIndex,int basketNumber,QString description,int typeOfProfiles)
{
	setDbTableActive("positions", false);
	execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('%1',%2,%3,%4,%5,0,0)").arg(description).arg(posNumber).arg(posIndex).arg(basketNumber).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
	setDbTableIdle("positions");
	//execQuery(QString("insert into baskets (number,type) values(%1,%2)").arg(basketNumber).arg(BASKET_NORMAL),MAIN_DB_CONNECTION);
	int noOfProfiles=1+static_cast<double>(4.0)*(static_cast<double>(rand())/static_cast<double>(RAND_MAX));
	for (int i=0;i<noOfProfiles;i++)
	{
		double length=0;
		if (typeOfProfiles==BASKET_CONTENS_LESS_THAN_20)
			length=3.+static_cast<double>(3.)*static_cast<double>(rand())/static_cast<double>(RAND_MAX);
		else if (typeOfProfiles==BASKET_CONTENS_LESS_THAN_24)
			length=3.+static_cast<double>(4.3)*static_cast<double>(rand())/static_cast<double>(RAND_MAX);
		else if (typeOfProfiles==BASKET_CONTENS_LESS_THAN_30)
			length=length=3.+static_cast<double>(6.0)*static_cast<double>(rand())/static_cast<double>(RAND_MAX);
		int no=10+static_cast<double>(100.0)*(static_cast<double>(rand())/static_cast<double>(RAND_MAX));
		if (basketNumber>0) {
			setDbTableActive("basket_details", false);
			execQuery(QString("insert into basket_details (basketnum,dienum,Pc,PcLen,temper) values(%1,%2,%3,%4,'%5')").arg(basketNumber).arg(rand()).arg(no).arg(length*1000.).arg("T1"),APEX_DB_CONNECTION);
			setDbTableIdle("basket_details");
		}
	}
}
#endif

void mainWindowClass::demoFillTables(bool force)
{	
	LOG("mW.cpp", "demoFillTables(" + QString(force) + ")");
#ifdef EMULATE
	//positions
	QDialog *dlg=new QDialog(this);
	dlg->setWindowTitle("Demo fill the area with baskets.");
	QGridLayout *dlgMainLayout=new QGridLayout();dlg->setLayout(dlgMainLayout);
	
	dlgMainLayout->addWidget(new QLabel("Number of empty baskets:"),dlgMainLayout->rowCount(),0);
	QSpinBox *emptyBasketsSpinBox=new QSpinBox();emptyBasketsSpinBox->setMinimum(0);emptyBasketsSpinBox->setMaximum(20);emptyBasketsSpinBox->setValue(10);
	dlgMainLayout->addWidget(emptyBasketsSpinBox,dlgMainLayout->rowCount()-1,1);

	dlgMainLayout->addWidget(new QLabel("Number of full baskets:"),dlgMainLayout->rowCount(),0);
	QSpinBox *fullBasketsSpinBox=new QSpinBox();fullBasketsSpinBox->setMinimum(0);fullBasketsSpinBox->setMaximum(20);fullBasketsSpinBox->setValue(10);
	dlgMainLayout->addWidget(fullBasketsSpinBox,dlgMainLayout->rowCount()-1,1);

	dlgMainLayout->addWidget(new QLabel("Number of baskets before oven:"),dlgMainLayout->rowCount(),0);
	QSpinBox *basketsBeforeOvenSpinBox=new QSpinBox();basketsBeforeOvenSpinBox->setMinimum(0);basketsBeforeOvenSpinBox->setMaximum(20);basketsBeforeOvenSpinBox->setValue(6);
	dlgMainLayout->addWidget(basketsBeforeOvenSpinBox,dlgMainLayout->rowCount()-1,1);

	dlgMainLayout->addWidget(new QLabel("Number of baskets in oven:"),dlgMainLayout->rowCount(),0);
	QSpinBox *basketsInOvenSpinBox=new QSpinBox();basketsInOvenSpinBox->setMinimum(0);basketsInOvenSpinBox->setMaximum(16);basketsInOvenSpinBox->setValue(16);
	dlgMainLayout->addWidget(basketsInOvenSpinBox,dlgMainLayout->rowCount()-1,1);

	dlgMainLayout->addWidget(new QLabel("Number of baskets after oven:"),dlgMainLayout->rowCount(),0);
	QSpinBox *basketsAfterOvenSpinBox=new QSpinBox();basketsAfterOvenSpinBox->setMinimum(0);basketsAfterOvenSpinBox->setMaximum(36);basketsAfterOvenSpinBox->setValue(30);
	dlgMainLayout->addWidget(basketsAfterOvenSpinBox,dlgMainLayout->rowCount()-1,1);

	QPushButton *cancelBtn=new QPushButton(tr("Cancel"));
	QPushButton *okBtn=new QPushButton(tr("Reset"));
	okBtn->setDefault(true);
	dlgMainLayout->addWidget(cancelBtn,dlgMainLayout->rowCount(),0);
	dlgMainLayout->addWidget(okBtn,dlgMainLayout->rowCount()-1,1);
	connect(cancelBtn,SIGNAL(clicked()),dlg,SLOT(reject()));
	connect(okBtn,SIGNAL(clicked()),dlg,SLOT(accept()));
	if (force)
		dlg->exec();
	else
	{
		if (dlg->exec()!=QDialog::Accepted)
			return;
	}
	//if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='positions'"),MAIN_DB_CONNECTION).count() > 0)
	//	execQuery(QString("delete from positions"),MAIN_DB_CONNECTION);
	//if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='temperTypes'"),MAIN_DB_CONNECTION).count()>0)
	//	execQuery(QString("delete from temperTypes"),MAIN_DB_CONNECTION);
	//if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='toAssimakisExports'"),MAIN_DB_CONNECTION).count()>0)
	//	execQuery(QString("delete from toAssimakisExports"),MAIN_DB_CONNECTION);
	//if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='toOmavExports'"),MAIN_DB_CONNECTION).count()>0)
	//	execQuery(QString("delete from toOmavExports"),MAIN_DB_CONNECTION);
	//if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='basket_details'"),APEX_DB_CONNECTION).count()>0)
	//	execQuery(QString("delete from basket_details"),APEX_DB_CONNECTION);
	//if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='basket_location'"),APEX_DB_CONNECTION).count()>0)
	//	execQuery(QString("delete from basket_location"),APEX_DB_CONNECTION);
	//if (execQuery(QString("SELECT name FROM sqlite_master WHERE name ='positions'"),EMULATE_DB_CONNECTION).count()>0)
	//	execQuery(QString("drop table positions"), EMULATE_DB_CONNECTION);
	//if (execQuery(QString("SELECT name FROM sqlite_master WHERE name ='baskets'"),MAIN_DB_CONNECTION).count()>0)

	//if (execQuery(QString("SELECT [name] FROM sys.tables WHERE [name] = 'positions'"), MAIN_DB_CONNECTION).count() > 0)
		setDbTableActive("positions", false);
		execQuery(QString("delete from positions"), MAIN_DB_CONNECTION);
		setDbTableIdle("positions");
	//if (execQuery(QString("SELECT [name] FROM sys.tables WHERE [name] = 'temperTypes'"), MAIN_DB_CONNECTION).count() > 0)
		setDbTableActive("temperTypes", false);
		execQuery(QString("delete from temperTypes"), MAIN_DB_CONNECTION);
		setDbTableIdle("temperTypes");
	//if (execQuery(QString("SELECT [name] FROM sys.tables WHERE [name] = 'toAssimakisExports'"), MAIN_DB_CONNECTION).count() > 0)
		setDbTableActive("toAssimakisExports", false);
		execQuery(QString("delete from toAssimakisExports"), MAIN_DB_CONNECTION);
		setDbTableIdle("toAssimakisExports");
	//if (execQuery(QString("SELECT [name] FROM sys.tables WHERE [name] = 'toOmavExports'"), MAIN_DB_CONNECTION).count() > 0)
		setDbTableActive("toOmavExports", false);
		execQuery(QString("delete from toOmavExports"), MAIN_DB_CONNECTION);
		setDbTableIdle("toOmavExports");
	//if (execQuery(QString("SELECT [name] FROM sys.tables WHERE [name] = 'basket_details'"), APEX_DB_CONNECTION).count() > 0)
		setDbTableActive("basket_details", false);
		execQuery(QString("delete from basket_details"), APEX_DB_CONNECTION);
		setDbTableIdle("basket_details");
	//if (execQuery(QString("SELECT [name] FROM sys.tables WHERE [name] = 'basket_location'"), APEX_DB_CONNECTION).count() > 0)
		setDbTableActive("basket_location", false);
		execQuery(QString("delete from basket_location"), APEX_DB_CONNECTION);
		setDbTableIdle("basket_location");
	//if (execQuery(QString("SELECT [name] FROM sys.tables WHERE [name] = 'positions'"), EMULATE_DB_CONNECTION).count() > 0)
		// execQuery(QString("drop table positions"), EMULATE_DB_CONNECTION);
	//if (execQuery(QString("SELECT [name] FROM sys.tables WHERE [name] = 'baskets'"), MAIN_DB_CONNECTION).count() > 0)
	//{
		//execQuery(QString("drop table baskets"), MAIN_DB_CONNECTION);
#ifdef EMULATE
	//QString IDENTITY_STR = "autoincrement";
	QString IDENTITY_STR = "IDENTITY";
#else
	QString IDENTITY_STR = "IDENTITY";
#endif
		createTable("baskets",QStringList()<<"pid"<<"number"<<"type"<<"secFilled",QStringList()<<"integer PRIMARY KEY "+IDENTITY_STR<<"int"<<"int"<<"int",MAIN_DB_CONNECTION);
//	}
	//
	setDbTableActive("temperTypes", true);
	if (execQuery(QString("SELECT temper FROM temperTypes WHERE temper='T1'"),MAIN_DB_CONNECTION).count()==0) {
		setDbTableIdle("temperTypes");
		setDbTableActive("temperTypes", false);
		execQuery(QString("insert into temperTypes (temper,temperType) values('T1',%1)").arg(basketClass::BASKET_TEMPER_185),MAIN_DB_CONNECTION);
		setDbTableIdle("temperTypes");
	} else
		setDbTableIdle("temperTypes");
	//
	int basketNumber=1;
	//emptyBasketsSpinBox
	int emptyBasket=0;
	int pos4[]={300,310,320,330,340,350,360,370,380,390,800,820,840,860,880};
	int count=0;

	for (int i = 1; i < 100; i++)
	{
		setDbTableActive("basket_location", false);
		execQuery(QString("insert into basket_location (basketnum, basketlocation, audittime) values(%1, 1, null)").arg(i), APEX_DB_CONNECTION);
		execQuery(QString("insert into basket_location (basketnum, basketlocation, audittime) values(%1, 2, null)").arg(i), APEX_DB_CONNECTION);
		execQuery(QString("insert into basket_location (basketnum, basketlocation, audittime) values(%1, 3, null)").arg(i), APEX_DB_CONNECTION);
		setDbTableIdle("basket_location");
	}

	for (int i = 0; i < 15; i++)  // size of pos4[] above.
	{
		for (int j=1; j<=4; j++)
		{
			emptyBasket++;;
			addEmptyBasket(pos4[i], j, emptyBasket < emptyBasketsSpinBox->value() ? basketNumber++ : 0);
		}
	}

	//fullBasketsSpinBox
	int fullBasket = 0;
	fullBasket++; addFullBasket(200,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++; addFullBasket(200,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++; addFullBasket(200,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++; addFullBasket(200,4,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	
	fullBasket++; addFullBasket(210,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++; addFullBasket(210,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++; addFullBasket(210,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++; addFullBasket(210,4,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);

	fullBasket++; addFullBasket(220,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++; addFullBasket(220,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(220,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(220,4,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);

	fullBasket++;addFullBasket(230,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(230,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(230,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	
	fullBasket++;addFullBasket(240,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(240,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(240,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);

	fullBasket++;addFullBasket(250,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(250,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(250,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);
	
	fullBasket++;addFullBasket(260,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(260,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(260,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	
	fullBasket++; addFullBasket(270, 1, fullBasket < fullBasketsSpinBox->value() ? basketNumber++ : 0, "30 feet", BASKET_CONTENS_LESS_THAN_30);
	fullBasket++; addFullBasket(270, 2, fullBasket < fullBasketsSpinBox->value() ? basketNumber++ : 0, "30 feet", BASKET_CONTENS_LESS_THAN_30);
	fullBasket++; addFullBasket(270, 3, fullBasket < fullBasketsSpinBox->value() ? basketNumber++ : 0, "30 feet", BASKET_CONTENS_LESS_THAN_30);

	fullBasket++; addFullBasket(280, 1, fullBasket < fullBasketsSpinBox->value() ? basketNumber++ : 0, "30 feet", BASKET_CONTENS_LESS_THAN_24);
	fullBasket++; addFullBasket(280, 2, fullBasket < fullBasketsSpinBox->value() ? basketNumber++ : 0, "30 feet", BASKET_CONTENS_LESS_THAN_30);
	fullBasket++; addFullBasket(280, 3, fullBasket < fullBasketsSpinBox->value() ? basketNumber++ : 0, "30 feet", BASKET_CONTENS_LESS_THAN_30);
	fullBasket++; addFullBasket(280, 4, fullBasket < fullBasketsSpinBox->value() ? basketNumber++ : 0, "30 feet", BASKET_CONTENS_LESS_THAN_30);

	fullBasket++; addFullBasket(290, 1, fullBasket < fullBasketsSpinBox->value() ? basketNumber++ : 0, "30 feet", BASKET_CONTENS_LESS_THAN_30);
	fullBasket++; addFullBasket(290, 2, fullBasket < fullBasketsSpinBox->value() ? basketNumber++ : 0, "30 feet", BASKET_CONTENS_LESS_THAN_30);
	fullBasket++; addFullBasket(290, 3, fullBasket < fullBasketsSpinBox->value() ? basketNumber++ : 0, "30 feet", BASKET_CONTENS_LESS_THAN_30);
	fullBasket++; addFullBasket(290, 4, fullBasket < fullBasketsSpinBox->value() ? basketNumber++ : 0, "30 feet", BASKET_CONTENS_LESS_THAN_30);

	//basketsBeforeOvenSpinBox
	int basketsBeforeOven=0;
	basketsBeforeOven++; addFullBasket(130, 1, basketsBeforeOven<basketsBeforeOvenSpinBox->value() ? basketNumber++ : 0, "before oven", BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++; addFullBasket(130, 2, basketsBeforeOven<basketsBeforeOvenSpinBox->value() ? basketNumber++ : 0, "before oven", BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++; addFullBasket(130, 3, basketsBeforeOven<basketsBeforeOvenSpinBox->value() ? basketNumber++ : 0, "before oven", BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++; addFullBasket(130, 4, basketsBeforeOven<basketsBeforeOvenSpinBox->value() ? basketNumber++ : 0, "before oven", BASKET_CONTENS_LESS_THAN_24);

	basketsBeforeOven++;addFullBasket(120,1,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(120,2,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(120,3,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(120,4,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);

	basketsBeforeOven++;addFullBasket(110,1,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(110,2,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(110,3,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(110,4,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);

	basketsBeforeOven++;addFullBasket(100,1,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(100,2,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(100,3,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(100,4,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);

	basketsBeforeOven++;addFullBasket(140,1,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(140,2,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(140,3,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(140,4,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
		basketsBeforeOven++;addFullBasket(150,1,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(150,2,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(150,3,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(150,4,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);

	//basketsInOvenSpinBox
	int basketsInOven=0;
	basketsInOven++; addFullBasket(600, 1, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(600, 2, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(600, 3, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(600, 4, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(610, 1, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(610, 2, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(610, 3, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(610, 4, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(620, 1, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(620, 2, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(620, 3, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(620, 4, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(630, 1, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(630, 2, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(630, 3, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++; addFullBasket(630, 4, basketsInOven<basketsInOvenSpinBox->value() ? basketNumber++ : 0, "in oven", BASKET_CONTENS_LESS_THAN_24);

	addFullBasket(640,1,0,"before ove x 2",0);
	addFullBasket(640,2,0,"before ove x 2",0);
	addFullBasket(640,3,0,"before ove x 2",0);
	addFullBasket(640,4,0,"before ove x 2",0);
	addFullBasket(650,1,0,"before ove x 2",0);
	addFullBasket(650,2,0,"before ove x 2",0);
	addFullBasket(650,3,0,"before ove x 2",0);
	addFullBasket(650,4,0,"before ove x 2",0);

	//basketsAfterOvenSpinBox
	int basketsAfterOven=0;
	basketsAfterOven++; addFullBasket(660, 1, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(660, 2, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(660, 3, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(660, 4, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(670, 1, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(670, 2, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(670, 3, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(670, 4, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(680, 1, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(680, 2, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(680, 3, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(680, 4, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(690, 1, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(690, 2, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(690, 3, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(690, 4, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(700, 1, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(700, 2, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(700, 3, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(700, 4, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(710, 1, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(710, 2, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(710, 3, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(710, 4, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
		addFullBasket(730,1,0,"after oven x 2",0);
	addFullBasket(730,2,0,"after oven x 2",0);
	addFullBasket(730,3,0,"after oven x 2",0);
	addFullBasket(730,4,0,"after oven x 2",0);

	addFullBasket(1010, 1, 0, "in new oven", 0);
	addFullBasket(1010, 2, 0, "in new oven", 0);
	addFullBasket(1010, 3, 0, "in new oven", 0);
	addFullBasket(1010, 4, 0, "in new oven", 0);
	addFullBasket(1020, 1, 0, "in new oven", 0);
	addFullBasket(1020, 2, 0, "in new oven", 0);
	addFullBasket(1020, 3, 0, "in new oven", 0);
	addFullBasket(1020, 4, 0, "in new oven", 0);
	addFullBasket(1030, 1, 0, "in new oven", 0);
	addFullBasket(1030, 2, 0, "in new oven", 0);
	addFullBasket(1030, 3, 0, "in new oven", 0);
	addFullBasket(1030, 4, 0, "in new oven", 0);

	addFullBasket(1040, 1, 0, "after new oven", 0);
	addFullBasket(1040, 2, 0, "after new oven", 0);
	addFullBasket(1040, 3, 0, "after new oven", 0);
	addFullBasket(1040, 4, 0, "after new oven", 0);
	addFullBasket(1050, 1, 0, "after new oven", 0);
	addFullBasket(1050, 2, 0, "after new oven", 0);
	addFullBasket(1050, 3, 0, "after new oven", 0);
	addFullBasket(1050, 4, 0, "after new oven", 0);
	addFullBasket(1060, 1, 0, "after new oven", 0);
	addFullBasket(1060, 2, 0, "after new oven", 0);
	addFullBasket(1060, 3, 0, "after new oven", 0);
	addFullBasket(1060, 4, 0, "after new oven", 0);

	basketsAfterOven++; addFullBasket(720, 1, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(720, 2, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(720, 3, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(720, 4, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "after oven", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(160, 1, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "returnigFromOven160", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(160, 2, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "returnigFromOven160", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(160, 3, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "returnigFromOven160", BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++; addFullBasket(160, 4, basketsAfterOven<basketsAfterOvenSpinBox->value() ? basketNumber++ : 0, "returnigFromOven160", BASKET_CONTENS_LESS_THAN_24);

	addFullBasket(  1, 1, 0, "crane", 0);
	addFullBasket(400, 1, 0, "before stacker", 0);
	addFullBasket(900, 1, basketNumber++, "stacker", BASKET_CONTENS_LESS_THAN_24);
	addFullBasket(410, 1, basketNumber++, "after stacker", BASKET_CONTENS_LESS_THAN_24);
	addFullBasket(420, 1, 0, "scrap", 0);
	addFullBasket(450, 1, 0, "before stacker 2", 0);
	addFullBasket(910, 1, 0, "stacker 2", 0);
	addFullBasket(460, 1, 0, "after stacker 2", 0);
	addFullBasket(520, 1, 0, "to destacker 2", 0);
	addFullBasket(510, 1, 0, "to destacker 1", 0);
	addFullBasket(500, 1, 0, "from destacker", 0);
	
	for (int j = 1; j <= 1000; j++) {
		setDbTableActive("baskets", false);
		execQuery(QString("insert into baskets (number,type) values(%1,%2)").arg(j).arg(BASKET_NORMAL), MAIN_DB_CONNECTION);
		setDbTableIdle("baskets");
	}
#endif
}

void mainWindowClass::initializeDatabase()
{
	using BasketCrane::Configuration::ConfigurationManager;
	using BasketCrane::Configuration::DatabaseConfigurator;
	using BasketCrane::Configuration::DatabaseProfile;

	const ConfigurationManager& configuration = ConfigurationManager::Instance();
	const BasketCrane::Configuration::DatabaseSettings& databaseSettings = configuration.Database();
	LOG("mW.cpp", "INFO:  Start: mainWindowClass::initializeDatabase");
#ifdef EMULATE
	LOG("mW.cpp", "EMULATE");
	bool forceDemoFillTables = configuration.Emulation().ForceDemoFillTables();
#endif

#if defined(inMyLaptop) || defined(SEMILIVE) || defined(EMULATE)
	const bool useLiveProfile = false;
#else
	const bool observerRequested = configuration.Application().EnvironmentMode().compare("LiveObserver", Qt::CaseInsensitive) == 0;
	const bool useLiveProfile = theOneBCToRuleThemAll || isDatabaseReadOnlyMode();
	if (!useLiveProfile)
	{
		const bool liveRequested = configuration.Application().EnvironmentMode().compare("Live", Qt::CaseInsensitive) == 0;
		QMessageBox::warning(0, "Warning - " + appHostname,
			(liveRequested || observerRequested)
				? "LIVE was requested, but this host is not authorized. The application will use the configured TEST databases and limited PLC communications."
				: "The application is configured for TEST databases and limited PLC communications.");
	}
#endif

	const DatabaseProfile& mainProfile = databaseSettings.MainProfile(useLiveProfile);
	const DatabaseProfile& apexProfile = databaseSettings.ApexProfile(useLiveProfile);

	LOG("mW.cpp", QString("Opening main database profile '%1'").arg(mainProfile.Name()));
	QSqlDatabase mainDb = QSqlDatabase::addDatabase(databaseSettings.Driver(), MAIN_DB_CONNECTION);
	DatabaseConfigurator::Apply(mainDb, mainProfile);
	if (!mainDb.open())
		EXIT(tr("Error in main database"), mainDb.lastError().text());

	const QString mainCatalog = databaseSettings.MainCatalog();
	if (databaseSettings.CreateCatalogs())
		execDBQuery(QString("if not exists(select * from sys.databases where name='%1') create database [%1]").arg(mainCatalog), MAIN_DB_CONNECTION);
	execDBQuery(QString("USE [%1]").arg(mainCatalog), MAIN_DB_CONNECTION);
	logDatabaseEndpoint(mainDb, mainProfile.Name(), mainCatalog,
		"positions,baskets,basketHistory,toAssimakisExports,toOmavExports");

	LOG("mW.cpp", QString("Opening Apex database profile '%1'").arg(apexProfile.Name()));
	QSqlDatabase apexDb = QSqlDatabase::addDatabase(databaseSettings.Driver(), APEX_DB_CONNECTION);
	DatabaseConfigurator::Apply(apexDb, apexProfile);
	if (!apexDb.open())
		EXIT(tr("Error in Apex database"), apexDb.lastError().text());

	const QString apexCatalog = databaseSettings.ApexCatalog();
	if (databaseSettings.CreateCatalogs())
		execDBQuery(QString("if not exists(select * from sys.databases where name='%1') create database [%1]").arg(apexCatalog), APEX_DB_CONNECTION);
	execDBQuery(QString("USE [%1]").arg(apexCatalog), APEX_DB_CONNECTION);
	logDatabaseEndpoint(apexDb, apexProfile.Name(), apexCatalog,
		"basket_details,basket_location,temperTypes");

	if (isDatabaseReadOnlyMode())
	{
		LOG("CONFIGURATION", "LIVE OBSERVER skipped database schema creation and maintenance");
		return;
	}

	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
#ifdef EMULATE
	 QString IDENTITY_STR = "autoincrement";
	//QString IDENTITY_STR="IDENTITY";
#else
	QString IDENTITY_STR="IDENTITY";
#endif
	//checkCreate tables
	LOG("mW.cpp", "checkCreate tables");
	QStringList colNames;QStringList colTypes;
	
	//positionsTable
	LOG("mW.cpp", "postionsTables");
	colNames.clear();colTypes.clear();
	colNames<<"pid"	<<"description"<<"posNumber" <<"posIndex" <<"basketTableId"	<<"locked"<<"targetNumber" <<"targetIndex";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(20)"<<"int" <<"int" << "int" << "int"   <<"int" << "int"   ;
	LOG("mW.cpp", "checkCreate tables - createTable = positions");
	createTable("positions",colNames,colTypes,MAIN_DB_CONNECTION);
	
	//basketsTable
	LOG("mW.cpp", "basketsTables");
	colNames.clear();colTypes.clear();
	colNames<<"pid"<<"number"<<"type"<<"secFilled"	;
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"int"<<"int" <<"int";
	LOG("mW.cpp", "checkCreate tables - createTable = baskets");
	createTable("baskets",colNames,colTypes,MAIN_DB_CONNECTION);
	
	//basketHistory
	LOG("mW.cpp", "basketsHistory");
	colNames.clear();colTypes.clear();
	colNames<<"id"<<"type"<<"position"<<"destination"<<"basketTableId"<<"date"<<"sec";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(10)"<<"char(40)"<<"char(40)"   <<"int" << "char(20)"<<"int";
	LOG("mW.cpp", "checkCreate tables - createTable = basketHistory");
	createTable("basketHistory",colNames,colTypes,MAIN_DB_CONNECTION);
	
	//toAssimakisExports
	LOG("mW.cpp", "toASsimakisExports");
	colNames.clear();colTypes.clear();
	colNames<<"id"<<"basketTableId"<<"priority";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"int" << "int"			;
	LOG("mW.cpp", "checkCreate tables - createTable = toAssimakisExports");
	createTable("toAssimakisExports",colNames,colTypes,MAIN_DB_CONNECTION);
	
	//toOmavExports
	LOG("mW.cpp", "toOmavExports");
	colNames.clear();colTypes.clear();
	colNames<<"id"<<"basketTableId"<<"priority";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"int"		        <<"int"			;
	LOG("mW.cpp", "checkCreate tables - createTable = toOmaxExports");
	createTable("toOmavExports",colNames,colTypes,MAIN_DB_CONNECTION);
	
	//temperTypes
	LOG("mW.cpp", "temperTypes");
	colNames.clear();colTypes.clear();		///T52 ...   basketClass::temperEnum BASKET_TEMPER_000,BASKET_TEMPER_185,BASKET_TEMPER_230
	colNames<<"id"											 <<"temper"  <<"temperType";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(50)"<<"int"			  ;
	LOG("mW.cpp", "checkCreate tables - createTable = temperTypes");
	createTable("temperTypes",colNames,colTypes,MAIN_DB_CONNECTION);
	
	//ovenLoads
	LOG("mW.cpp", "ovenLoads");
	colNames.clear();colTypes.clear();		
	colNames<<"id"														<<"ovenId"  <<"date";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR <<"int"			<<"char(20)"			  ;
	LOG("mW.cpp", "checkCreate tables - createTable = oventLoads");
	createTable("ovenLoads",colNames,colTypes,MAIN_DB_CONNECTION);
	
	//ovenLoadsContents
	LOG("mW.cpp", "ovenLoadsContents");
	colNames.clear();colTypes.clear();		
	colNames<<"id"														<<"ovenId"  <<"basketId"<<"profile"<<"sequence"<<"SONum"		<<"SOItemNum" <<"Lot"			<<"pc"	<<"PcLen"	<<"Wt"	<<"PcPulled"<<"temper" 	;
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR <<"char(20)"<<"int"			<<"int"		 <<"int"		 <<"char(50)"	<<"char(50)"	<<"char(50)"<<"real"<<"real"	<<"real"<<"real"		<<"char(50)";
	LOG("mW.cpp", "checkCreate tables - createTable = ovenLoadsContents");
	createTable("ovenLoadsContents",colNames,colTypes,MAIN_DB_CONNECTION);
	
	////verboseLog
	//colNames.clear();colTypes.clear();		///T52 ...   basketClass::temperEnum BASKET_TEMPER_000,BASKET_TEMPER_185,BASKET_TEMPER_230
	//colNames<<"id"											 <<"temper"  <<"temperType";
	//colTypes<<"integer PRIMARY KEY "+IDENTITY<<"char(50)"<<"int"			  ;
	//createTable("verboseLog",colNames,colTypes,MAIN_DB_CONNECTION);
	
	//logCranePlc Table
	LOG("mW.cpp", "logCranePlc");
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"address"  <<"value"<<"X"     <<"Y"     <<"Z"     <<"date"	   <<"sec";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(15)" <<"int"  <<"float" <<"float" <<"float" <<"char(20)"<<"int";
	LOG("mW.cpp", "checkCreate tables - createTable = logCranePlc");
	createTable("logCranePlc",colNames,colTypes,MAIN_DB_CONNECTION);

#ifndef EMULATE
	setDbTableActive("logCranePlc", false);
	execDBQuery(QString("delete from logCranePlc where tid not in(select top 50000 tid from logCranePlc order by sec desc)"),MAIN_DB_CONNECTION);
	setDbTableIdle("logCranePlc");
#endif
	
	//logResetPlc Table
	LOG("mW.cpp", "logResetPlc");
	colNames.clear();colTypes.clear();
	colNames<<"tid"								  <<"type"	  <<"value"<<"date"	   <<"sec";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(15)" <<"float"<<"char(20)"<<"int";
	LOG("mW.cpp", "checkCreate tables - createTable = logResetPlc");
	createTable("logResetPlc",colNames,colTypes,MAIN_DB_CONNECTION);

#ifndef EMULATE
	setDbTableActive("logResetPlc", false);
	execDBQuery(QString("delete from logResetPlc where tid not in(select top 50000 tid from logResetPlc order by sec desc)"),MAIN_DB_CONNECTION);
	setDbTableIdle("logResetPlc");
#endif

	//logErrorPlc Table
	LOG("mW.cpp", "logErrorPlc");
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"type"	    <<"bit"     <<"description"<<"date"	   <<"sec";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(15)" <<"char(10)"<<"char(100)"  <<"char(20)"<<"int";
	LOG("mW.cpp", "checkCreate tables - createTable = logErrorPlc");
	createTable("logErrorPlc",colNames,colTypes,MAIN_DB_CONNECTION);

#ifndef EMULATE
	execDBQuery(QString("delete from logErrorPlc where tid not in(select top 1000 tid from logErrorPlc order by sec desc)"),MAIN_DB_CONNECTION);
#endif

	//users Table
	LOG("mW.cpp", "users");
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"name"	    <<"pass"    ;
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(15)" <<"char(10)";
	LOG("mW.cpp", "checkCreate tables - createTable = users");
	createTable("users",colNames,colTypes,MAIN_DB_CONNECTION);

#ifndef EMULATE
	QString initialAdminUser = configuration.Security().InitialAdminUser();
	QString initialAdminPassword = configuration.Security().InitialAdminPassword();
	initialAdminUser.replace("'", "''");
	initialAdminPassword.replace("'", "''");
	execDBQuery(QString("if not exists (select name from users where name='%1') insert into users (name,pass) values('%1','%2')")
		.arg(initialAdminUser).arg(initialAdminPassword), MAIN_DB_CONNECTION);
#endif

#ifdef EMULATE
	//basket_details
	LOG("mW.cpp", "basket_details");
	colNames.clear();colTypes.clear();
	colNames<<"basketnum"<<"sequence"<<"dienum"<<"SONum" <<"SOItemNum"<<"Lot" <<"Pc"  <<"PcLen"<<"Wt"  <<"LotCompleteFlag"<<"PcPulled"<<"temper"  ;
	colTypes<<"int" <<"int" <<"int" <<"char(50)"<<"char(50)" <<"char(50)" <<"real"<<"real" <<"real"<<"bit" << "real" <<"char(50)";
	createTable("basket_details",colNames,colTypes,APEX_DB_CONNECTION);
	//basket_location
	colNames.clear();colTypes.clear();
	colNames<<"basketnum"<<"basketlocation"	<<"audittime";
	colTypes<<"int" << "int" << "datetime" ;
	createTable("basket_location",colNames,colTypes,APEX_DB_CONNECTION);
#endif

#ifdef EMULATE
	demoFillTables(forceDemoFillTables);
#endif
	LOG("mW.cpp", "checkCreate tables - execQuery to set initial positions data");
	if (getFirst(QString("select max(posNumber) from positions"), MAIN_DB_CONNECTION).toInt()==1000)
	{
		setDbTableActive("positions", false);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1010,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1010,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1010,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1010,4,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1020,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1020,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1020,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1020,4,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1030,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1030,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1030,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1030,4,0,1,0,0)"),MAIN_DB_CONNECTION);

		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1040,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1040,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1040,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1040,4,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1050,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1050,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1050,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1050,4,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1060,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1060,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1060,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execDBQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1060,4,0,1,0,0)"),MAIN_DB_CONNECTION);
		setDbTableIdle("positions");
	}

	LOG("mW.cpp", "INFO: Done: mainWindowClass::initializeDatabase");
}

void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build: %1 %2").arg(__DATE__).arg(__TIME__));
}


// @@@@@ -- AB For detecting PLC network errors, we will rely on the existing PLC classes to determine if there is an error, and then we will set the appropriate 
// error state in the UI.  This way, we can leverage the existing logic in the PLC classes for determining if there is an error, and we can keep the UI logic separate 
// from the PLC logic.  
// The PLC classes will need to have a method for determining if there is an error, and then we can call that method from this timer slot to update the UI accordingly.
void mainWindowClass::plcHealth()
{
	// Crane PLC
	if (auto craneTux = plcWidget->getCraneTuxClass()) {
		if (craneTux->getHasError()) {
			setPlcNetworkError(PLC_NETWORK_CRANE);
		}
		// else: do nothing here; the green/grey is still driven by real data
	}

	// Conveyors PLC
	if (auto convTux = plcWidget->getConveyorTuxClass()) {
		if (convTux->getHasError()) {
			setPlcNetworkError(PLC_NETWORK_CONVEYORS);
		}
	}

	// New Oven PLC
	if (auto ovenTux = plcWidget->getNewOvenTuxClass()) {
		if (ovenTux->getHasError()) {
			setPlcNetworkError(PLC_NETWORK_NEW_OVEN);
		}
	}
}

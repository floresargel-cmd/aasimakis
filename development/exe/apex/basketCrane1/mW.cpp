#include "mW.h"
#include "dbTableStatus.h"
#include "plcNetworkStatus.h"
#include "timerStatus.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QDir>
#include <QLibraryInfo>


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
	// Lazy ass programming, just set the variable from the logger.h...
	theOneBCToRuleThemAll = isAppHostEnabled(getAppHostname());

	setWindowIcon(QIcon(":/logo_16x16.png"));

#ifdef EMULATE
	qApp->addLibraryPath("C:/assimakis/development/#workingDir/apex/basketCrane1/");
	qApp->addLibraryPath("C:/assimakis/development/#workingDir/apex/basketCrane1/plugins");
	qApp->addLibraryPath("C:/assimakis/development/#workingDir/apex/basketCrane1/plugins/sqldrivers");
	qApp->addLibraryPath("C:/assimakis/development/#workingDir/apex/basketCrane1/plugins/sqldrivers/dll");
	// QMessageBox::warning(0,"Warning","Emulation mode - not to be used in production!");
#endif
#ifdef inMyLaptop
	qApp->addLibraryPath("C:/assimakis/development/#workingDir/apex/basketCrane1/");
	qApp->addLibraryPath("C:/assimakis/development/#workingDir/apex/basketCrane1/plugins");
	qApp->addLibraryPath("C:/assimakis/development/#workingDir/apex/basketCrane1/plugins/sqldrivers");
	qApp->addLibraryPath("C:/assimakis/development/#workingDir/apex/basketCrane1/plugins/sqldrivers/dll");
	// QMessageBox::warning(0,"Warning","inMyLaptop mode - not to be used in production!");
#endif

	//
	LOG("mW.cpp", "initializeDatabase()");
	initializeDatabase();
	//
	LOG("mW.cpp", "setLayout(mainLayout)");
	QHBoxLayout *mainLayout = new QHBoxLayout(); mainLayout->setMargin(0);
	QVBoxLayout *rootLayout = new QVBoxLayout();
	rootLayout->setMargin(0);
	rootLayout->addLayout(mainLayout);
	// Status bar: database table activity indicators (green=read, blue=write, grey=idle)
	QWidget *statusBarWidget = new QWidget(this);
	statusBarWidget->setFixedHeight(28);
	QHBoxLayout *statusBarLayout = new QHBoxLayout(statusBarWidget);
	statusBarLayout->setContentsMargins(4, 2, 4, 2);
	statusBarLayout->setSpacing(6);	
	
	dbAreaLabel = new QLabel("DB:", statusBarWidget);
	QString styleHeaderGrey("QLabel{font-size:14px;font-weight:bold;background-color:#f0f0f0;color:#000;padding:2px 6px;border-radius:2px;}");
	dbAreaLabel->setStyleSheet(styleHeaderGrey);
	dbAreaLabel->setAutoFillBackground(true);
	statusBarLayout->addWidget(dbAreaLabel);
	QStringList tableNames = QStringList() << "basket_details" << "basket_location" << "baskets" << "basketHistory"
		<< "logCranePlc" << "logResetPlc" << "positions" << "temperTypes" << "toAssimakisExports" << "toOmavExports";
	QString styleGrey("QLabel{font-size:11px;background-color:#808080;color:#fff;padding:2px 6px;border-radius:2px;}");
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
	QWidget *plcWidgetTemp = new QWidget();
	plcWidget = new plcWidgetClass(plcWidgetTemp);
	openGLWidget = new openGLWidgetClass(plcWidget->getCranePlcTcpServer(), plcWidget->getConveyorsPlcTcpServer(), 0, 0);
	mainLayout->addWidget(openGLWidget);
	plcWidgetTemp->setParent(openGLWidget);

#ifdef TUX_PLC_COMMS
	// Show PLC networks as "connecting" (yellow) until first data or error
	setPlcNetworkConnecting(PLC_NETWORK_CRANE);
	setPlcNetworkConnecting(PLC_NETWORK_CONVEYORS);
	setPlcNetworkConnecting(PLC_NETWORK_NEW_OVEN);
#endif

	// Auto mode flags
	LOG("mW.cpp", "Auto mode flags");
	QVBoxLayout *autoGroupBoxLayout = new QVBoxLayout();

	QCheckBox	*beforStackerQCheckBox = new QCheckBox(tr("Before stacker"), this);
	connect(beforStackerQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(beforeStackerAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(beforStackerQCheckBox);
	beforStackerQCheckBox->setChecked(false);
	beforStackerQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa};");

	QCheckBox	*afterStackerQCheckBox = new QCheckBox(tr("After stacker"), this);
	connect(afterStackerQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(afterStackerAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(afterStackerQCheckBox);
	afterStackerQCheckBox->setChecked(false);
	afterStackerQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*beforeStacker2QCheckBox = new QCheckBox(tr("Before stacker 2"), this);
	connect(beforeStacker2QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(beforeStacker2AutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(beforeStacker2QCheckBox);
	beforeStacker2QCheckBox->setChecked(false);
	beforeStacker2QCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa};");

	QCheckBox	*afterStacker2QCheckBox = new QCheckBox(tr("After stacker 2"), this);
	connect(afterStacker2QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(afterStacker2AutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(afterStacker2QCheckBox);
	afterStacker2QCheckBox->setChecked(false);
	afterStacker2QCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox *pressToOvenQCheckBox = new QCheckBox(tr("Press# to oven#"), this);
	connect(pressToOvenQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(pressToOvenCheckBtnSlot(int)));
	autoGroupBoxLayout->addWidget(pressToOvenQCheckBox);
	pressToOvenQCheckBox->setChecked(false);
	pressToOvenQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*unloadOvenQCheckBox = new QCheckBox(tr("Unload oven"), this);
	connect(unloadOvenQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(unloadOvenAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(unloadOvenQCheckBox);
	unloadOvenQCheckBox->setChecked(false);
	unloadOvenQCheckBox->setEnabled(true);
	unloadOvenQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*unloadOven2QCheckBox = new QCheckBox(tr("Unload oven 2"), this);
	connect(unloadOven2QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(unloadOven2AutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(unloadOven2QCheckBox);
	unloadOven2QCheckBox->setChecked(false);
	unloadOven2QCheckBox->setEnabled(true);
	unloadOven2QCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*exportsQCheckBox=new QCheckBox(tr("Exports (full->out)"),this);
	connect(exportsQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(exportsAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(exportsQCheckBox);
	exportsQCheckBox->setChecked(false);
	exportsQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*returningEmptyQCheckBox=new QCheckBox(tr("Imports (empty<-in)"),this);
	connect(returningEmptyQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(returningEmptyAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(returningEmptyQCheckBox);
	returningEmptyQCheckBox->setChecked(false);
	returningEmptyQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	//QCheckBox	*scrapQCheckBox=new QCheckBox(tr("Scrap"),this);
	//connect(scrapQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(scrapAutoModeSlot(int)));
	//autoGroupBoxLayout->addWidget(scrapQCheckBox);
	//scrapQCheckBox->setChecked(false);
	//scrapQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*fromBufferToOvenQCheckBox = new QCheckBox(tr("From buffer to oven"),this);
	connect(fromBufferToOvenQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(fromBufferToOvenAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(fromBufferToOvenQCheckBox);
	fromBufferToOvenQCheckBox->setChecked(false);
	fromBufferToOvenQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*exportsBeforeImportsQCheckBox = new QCheckBox(tr("Exports before imports"),this);
	connect(exportsBeforeImportsQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(exportsBeforeImportsCheckBtnSlot(int)));
	autoGroupBoxLayout->addWidget(exportsBeforeImportsQCheckBox);
	exportsBeforeImportsQCheckBox->setChecked(false);
	exportsBeforeImportsQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QHBoxLayout *numberOfEmptyPosInBufferSpinBoxLayout=new QHBoxLayout();
	autoGroupBoxLayout->addLayout(numberOfEmptyPosInBufferSpinBoxLayout);
	QLabel *numberOfEmptyPosInBufferSpinBoxLabel=new QLabel(tr("Empty buffer"));
	numberOfEmptyPosInBufferSpinBoxLabel->setStyleSheet("QLabel{color:#aaaaaa;}");
	numberOfEmptyPosInBufferSpinBoxLayout->addWidget(numberOfEmptyPosInBufferSpinBoxLabel);
	QSpinBox *numberOfEmptyPosInBufferSpinBox=new QSpinBox();numberOfEmptyPosInBufferSpinBoxLayout->addWidget(numberOfEmptyPosInBufferSpinBox);
	connect(numberOfEmptyPosInBufferSpinBox,SIGNAL(valueChanged(int)),this,SLOT(numberOfEmptyPosInBufferSpinBoxSlot(int)));
	numberOfEmptyPosInBufferSpinBox->setMinimum(3);
	numberOfEmptyPosInBufferSpinBox->setMaximum(7);
	numberOfEmptyPosInBufferSpinBox->setValue(4);
	openGLWidget->setNumberOfPosToBeKeptEmptyInBuffer(numberOfEmptyPosInBufferSpinBox->value());

	QCheckBox *rearrangeBasketsQCheckBox = new QCheckBox(tr("Rearrange baskets"),this);
	connect(rearrangeBasketsQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(rearrangeBasketsCheckBtnSlot(int)));
	autoGroupBoxLayout->addWidget(rearrangeBasketsQCheckBox);
	rearrangeBasketsQCheckBox->setChecked(false);
	rearrangeBasketsQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");
	
	QGroupBox *autoGroupBox = new QGroupBox(tr("Auto mode"),openGLWidget);
	autoGroupBox->setStyleSheet("QGroupBox{color:#aaaaaa;}");
	autoGroupBox->setGeometry(25,20,165,270);
	autoGroupBox->setLayout(autoGroupBoxLayout);
	autoGroupBox->show();
	// End auto flags

	//
	int w = 140; int h = 25; int m = 5; int x = 30; int y = 295;

	QCheckBox *takeBasketsFromStackerOutQCheckBox = new QCheckBox(tr("New baskets out"), openGLWidget);
	connect(takeBasketsFromStackerOutQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(takeBasketsFromStackerOutSlot(int)));
	takeBasketsFromStackerOutQCheckBox->setGeometry(27, y, 170, 20);
	takeBasketsFromStackerOutQCheckBox->show();
	takeBasketsFromStackerOutQCheckBox->setChecked(false);
	takeBasketsFromStackerOutQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa};");

	////////////////////Use empty buffer
	LOG("mW.cpp", "Use empty buffer");
	QVBoxLayout *emptyBasketsPosGroupBoxLayout=new QVBoxLayout();

	useEmptyBasketsPosCheckBox = new QCheckBox(tr("Use"), this);
	useEmptyBasketsPosCheckBox->setChecked(false);
	useEmptyBasketsPosCheckBox->setToolTip(tr("Use the empty buffer area to store 20 feet baskets if necessary."));
	useEmptyBasketsPosCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa};");
	emptyBasketsPosGroupBoxLayout->addWidget(useEmptyBasketsPosCheckBox);
	connect(useEmptyBasketsPosCheckBox, SIGNAL(stateChanged(int)), this, SLOT(useEmptyBasketsPosCheckBoxSlot(int)));

	emptyEmptyBasketsPosCheckBox = new QCheckBox(tr("Empty"), this);
	emptyEmptyBasketsPosCheckBox->setChecked(false);
	emptyEmptyBasketsPosCheckBox->setToolTip(tr("Empty the empty buffer area from 20 feet baskets when you have time."));
	emptyEmptyBasketsPosCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa};");
	emptyBasketsPosGroupBoxLayout->addWidget(emptyEmptyBasketsPosCheckBox);
	connect(emptyEmptyBasketsPosCheckBox, SIGNAL(stateChanged(int)), this, SLOT(emptyEmptyBasketsPosCheckBoxSlot(int)));

	QGroupBox *fullBasketsToEmptyPosGroupBox=new QGroupBox(tr("20feet in empty buffer "),openGLWidget);
	fullBasketsToEmptyPosGroupBox->setStyleSheet("QGroupBox{color:#aaaaaa;}");
	fullBasketsToEmptyPosGroupBox->setGeometry(25, y += 25 + m, 150, 90);
	fullBasketsToEmptyPosGroupBox->setLayout(emptyBasketsPosGroupBoxLayout);
	fullBasketsToEmptyPosGroupBox->show();

	// Oven selection flags
	LOG("mW.cpp", "Oven selection flags");
	QVBoxLayout *OvenSelGroupBoxLayout = new QVBoxLayout();

	QCheckBox *Oven1EnableQCheckBox = new QCheckBox(tr("Oven1 Enable"), this);
	connect(Oven1EnableQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven1EnableAutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven1EnableQCheckBox);
	Oven1EnableQCheckBox->setChecked(true);
	Oven1EnableQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*Oven2EnableQCheckBox = new QCheckBox(tr("Oven2 Enable"), this);
	connect(Oven2EnableQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven2EnableAutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven2EnableQCheckBox);
	Oven2EnableQCheckBox->setChecked(true);
	Oven2EnableQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*Oven1Not24QCheckBox = new QCheckBox(tr("Not longer than 24f in Oven1"), this);
	connect(Oven1Not24QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven1Not24AutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven1Not24QCheckBox);
	Oven1Not24QCheckBox->setChecked(true);
	Oven1Not24QCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*Oven2Not24QCheckBox = new QCheckBox(tr("Just Longer than 24f in Oven2"), this);
	connect(Oven2Not24QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven2Not24AutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven2Not24QCheckBox);
	Oven2Not24QCheckBox->setChecked(true);
	Oven2Not24QCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QGroupBox *OvenSelGroupBox = new QGroupBox(tr("Ovens Selections"), openGLWidget);
	OvenSelGroupBox->setStyleSheet("QGroupBox{color:#aaaaaa;}");
	OvenSelGroupBox->setGeometry(200, 20, 190, 110);
	OvenSelGroupBox->setLayout(OvenSelGroupBoxLayout);
	OvenSelGroupBox->show();

	// Next oven 1 cycle
	LOG("mW.cpp", "Next oven 1 cycle");
	QVBoxLayout *nextOvenCycleGroupBoxLayout = new QVBoxLayout();

	QRadioButton *nextOvenCycle185RadioButton = new QRadioButton("185C 6 hours", this);
	connect(nextOvenCycle185RadioButton, SIGNAL(toggled(bool)), this, SLOT(nextOvenCycle185RadioButtonSlot(bool)));
	nextOvenCycleGroupBoxLayout->addWidget(nextOvenCycle185RadioButton);
	nextOvenCycle185RadioButton->setStyleSheet("QRadioButton	{color:#aaaaaa;}");

	QRadioButton *nextOvenCycle230RadioButton = new QRadioButton("230C 6 hours", this);
	connect(nextOvenCycle230RadioButton, SIGNAL(toggled(bool)), this, SLOT(nextOvenCycle230RadioButtonSlot(bool)));
	nextOvenCycleGroupBoxLayout->addWidget(nextOvenCycle230RadioButton);
	nextOvenCycle230RadioButton->setStyleSheet("QRadioButton{color:#aaaaaa;}");

	if (openGLWidget->getNextOvenLoadingCycleTemperEnum() == allPositionsClass::NEXT_OVEN_CYCLE_TEMPER_185)
		nextOvenCycle185RadioButton->setChecked(true);
	else
		nextOvenCycle230RadioButton->setChecked(true);

	QGroupBox *nextOvenCycleGroupBox = new QGroupBox(tr("Next Oven cycle"), openGLWidget);
	nextOvenCycleGroupBox->setStyleSheet("QGroupBox{color:#aaaaaa;}");
	nextOvenCycleGroupBox->setGeometry(200, 135, 140, 75);
	nextOvenCycleGroupBox->setLayout(nextOvenCycleGroupBoxLayout);
	nextOvenCycleGroupBox->show();

	// Mext oven 2 cycle
	LOG("mW.cpp", "Next oven 2 cycle");
	QVBoxLayout *nextOven2CycleGroupBoxLayout = new QVBoxLayout();

	QRadioButton *nextOven2Cycle185RadioButton = new QRadioButton("185C 6 hours", this);
	connect(nextOven2Cycle185RadioButton, SIGNAL(toggled(bool)), this, SLOT(nextOven2Cycle185RadioButtonSlot(bool)));
	nextOven2CycleGroupBoxLayout->addWidget(nextOven2Cycle185RadioButton);
	nextOven2Cycle185RadioButton->setStyleSheet("QRadioButton	{color:#aaaaaa;}");

	QRadioButton *nextOven2Cycle230RadioButton = new QRadioButton("230C 6 hours", this);
	connect(nextOven2Cycle230RadioButton, SIGNAL(toggled(bool)), this, SLOT(nextOven2Cycle230RadioButtonSlot(bool)));
	nextOven2CycleGroupBoxLayout->addWidget(nextOven2Cycle230RadioButton);
	nextOven2Cycle230RadioButton->setStyleSheet("QRadioButton{color:#aaaaaa;}");

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

	QGroupBox *nextOven2CycleGroupBox = new QGroupBox(tr("Next Oven2 cycle"), openGLWidget);
	nextOven2CycleGroupBox->setStyleSheet("QGroupBox{color:#aaaaaa;}");
	nextOven2CycleGroupBox->setGeometry(200, 215, 140, 75);
	nextOven2CycleGroupBox->setLayout(nextOven2CycleGroupBoxLayout);
	nextOven2CycleGroupBox->show();

	//
	//takeScrapBasketBtn=new QPushButton(tr("Take scrap basket"),openGLWidget);takeScrapBasketBtn->setGeometry(x,y+=95+m,w,h);  takeScrapBasketBtn 	->show();
	y += 95 + m;
	QPushButton *unlockBasketAfterStackerBtn = new QPushButton(tr("Unlock after stacker"), openGLWidget);
	unlockBasketAfterStackerBtn->setGeometry(x, y += h + m, w, h);
	unlockBasketAfterStackerBtn->show();

	QPushButton *unlockBasketReturningFromOvenBtn = new QPushButton(tr("Unlock from oven"), openGLWidget);
	unlockBasketReturningFromOvenBtn->setGeometry(x, y += h + m, w, h);
	unlockBasketReturningFromOvenBtn->show();

	plcWidgetTemp->setGeometry(x - 15, y += h + m + 50, 170, 330);
	plcWidgetTemp->show();

	//connect(takeScrapBasketBtn,SIGNAL(clicked()),this,SLOT(takeScrapBasketBtnSlot()));
	LOG("mW.cpp", "connecting unlockBaset* SIGNAL(s)");
	connect(unlockBasketAfterStackerBtn, SIGNAL(clicked()), this, SLOT(unlockBasketAfterStackerBtnSlot()));
	connect(unlockBasketReturningFromOvenBtn, SIGNAL(clicked()), this, SLOT(unlockBasketReturningFromOvenBtnSlot()));
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
	// mainTimer.start(1500);
	mainTimer.start(3000);  // @@@@@ - Slowing down the main timer to 3 seconds, since we don't need it to run every 1.5 seconds, and it was causing some performance issues.
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

static const int STATUS_LABEL_FADE_MS = 250;
static const QString s_dbTableStyleGrey("QLabel{font-size:11px;background-color:#808080;color:#fff;padding:2px 6px;border-radius:2px;}");

void mainWindowClass::setDbTableActive(const QString& tableName, bool isRead)
{
	QMap<QString, QTimer*>::iterator ti = dbTableIdleTimers.find(tableName);
	if (ti != dbTableIdleTimers.end()) {
		(*ti)->stop();
	}
	QMap<QString, QLabel*>::iterator it = dbTableStatusLabels.find(tableName);
	if (it != dbTableStatusLabels.end()) {
		QString style = isRead
			? QString("QLabel{font-size:11px;background-color:#09c500;color:#000;padding:2px 6px;border-radius:2px;}")
			: QString("QLabel{font-size:11px;background-color:#3399ff;color:#fff;padding:2px 6px;border-radius:2px;}");
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
	timer->start(STATUS_LABEL_FADE_MS);
}

void mainWindowClass::setDbTableIdleNow(const QString& tableName)
{
	QMap<QString, QLabel*>::iterator it = dbTableStatusLabels.find(tableName);
	if (it != dbTableStatusLabels.end())
		(*it)->setStyleSheet(s_dbTableStyleGrey);
}

static const QString s_plcNetworkStyleYellow("QLabel{font-size:11px;background-color:#cc9900;color:#000;padding:2px 6px;border-radius:2px;}");
static const QString s_plcNetworkStyleRed("QLabel{font-size:11px;background-color:#cc0000;color:#fff;padding:2px 6px;border-radius:2px;}");

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
			? QString("QLabel{font-size:11px;background-color:#09c500;color:#000;padding:2px 6px;border-radius:2px;}")
			: QString("QLabel{font-size:11px;background-color:#3399ff;color:#fff;padding:2px 6px;border-radius:2px;}");
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
	timer->start(STATUS_LABEL_FADE_MS);
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

static const QString s_timerStyleGrey("QLabel{font-size:11px;background-color:#808080;color:#fff;padding:2px 6px;border-radius:2px;}");
static const QString s_timerStyleGreen("QLabel{font-size:11px;background-color:#09c500;color:#000;padding:2px 6px;border-radius:2px;}");

void mainWindowClass::setMainTimerFiring()
{
	mainTimerIdleTimer->stop();
	if (mainTimerStatusLabel) mainTimerStatusLabel->setStyleSheet(s_timerStyleGreen);
}

void mainWindowClass::setMainTimerIdle()
{
	mainTimerIdleTimer->stop();
	mainTimerIdleTimer->start(STATUS_LABEL_FADE_MS);
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
	autoModeTimerIdleTimer->start(STATUS_LABEL_FADE_MS);
}

void mainWindowClass::onAutoModeTimerIdleTimeout()
{
	if (autoModeTimerStatusLabel) autoModeTimerStatusLabel->setStyleSheet(s_timerStyleGrey);
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
	LOG("mW.cpp", "INFO:  Start: mainWindowClass::initializeDatabase");
#ifdef EMULATE
	LOG("mW.cpp", "EMULATE");
	bool forceDemoFillTables = false;
#endif
	//MAIN_DB_CONNECTION
#ifdef inMyLaptop
	LOG("mW.cpp", "inMyLaptop");
	#ifdef EMULATE
		
		// qDebug() << "Qt version:" << QT_VERSION_STR;
		// qDebug() << "QT_PLUGIN_PATH environment variable:" << qgetenv("QT_PLUGIN_PATH");
		// qDebug() << "Qt lib plugins path:" << QLibraryInfo::location(QLibraryInfo::PluginsPath);
		// qDebug() << "QT_PLUGIN_PATH:" << qgetenv("QT_PLUGIN_PATH");
		// qDebug() << "Library paths:" << QCoreApplication::libraryPaths();
		// qDebug() << "Drivers available:" << QSqlDatabase::drivers();
		// QString a = QDir::current().absolutePath();
		// qDebug() << "emulationBasketCrane.db needs to be in the absolute path...";
		// qDebug() << "Current absolute path: " + a;
		
		// if (!QFile(QDir::current().absolutePath()+"/emulationBasketCrane.db").exists()) 
		// 	forceDemoFillTables = true;
		// qDebug() << "Current xdirectory:" << QDir::current().absolutePath();
		// qDebug() << "Available SQL drivers:" << QSqlDatabase::drivers();
		// qDebug() << "MAIN_DB_CONNECTION: " << MAIN_DB_CONNECTION;
		// QSqlDatabase mainDb = QSqlDatabase::addDatabase("QSQLITE", MAIN_DB_CONNECTION);
		// mainDb.setDatabaseName(QDir::current().absolutePath()+"/emulationBasketCrane.db");//odbc name
		// if (!mainDb.open()) 
		// 	EXIT(tr("Error in database emulationBasketCrane.db"),mainDb.lastError().text());
	
		//QSqlDatabase mainDb = QSqlDatabase::addDatabase("QODBC", MAIN_DB_CONNECTION);
		//mainDb.setDatabaseName("mainConnectionTest");  //odbc name
		//// mainDb.setUserName("apex2023");
		//// mainDb.setPassword("apex2023");
		//mainDb.setUserName("peadm2");
		//mainDb.setPassword("peadm2");
		//if (!mainDb.open())
		//	EXIT(tr("Error in database"), mainDb.lastError().text());
		//execQuery("if not exists(select * from sys.databases where [name] = 'basketCraneData') create database basketCraneData", MAIN_DB_CONNECTION);
		//execQuery("USE basketCraneData", MAIN_DB_CONNECTION);

		// @@@@@ - Use SAE01MSSQL02 as the test DB.
		QSqlDatabase mainDb = QSqlDatabase::addDatabase("QODBC", MAIN_DB_CONNECTION);
		mainDb.setDatabaseName("apexBasketCraneTest");//odbc name
		// mainDb.setUserName("sa");
		// mainDb.setPassword("123");
		mainDb.setUserName("peadm2");
		mainDb.setPassword("peadm2");
		if (!mainDb.open())
			EXIT(tr("Error in database"), mainDb.lastError().text());
		execQuery("if not exists(select * from sys.databases where name='basketCraneData') create database basketCraneData", MAIN_DB_CONNECTION);
		// execQuery("USE basketCraneDataTest", MAIN_DB_CONNECTION);
		execQuery("USE basketCraneData", MAIN_DB_CONNECTION); // No need for xxxTest, this is MSSQL02, the test server.
		
	#else
		LOG("mW.cpp", "inMyLaptop - mainDb: setDatabaseName & execQuery");
		QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
		mainDb.setDatabaseName("apexBasketCraneTest");//odbc name
		// mainDb.setUserName("sa"); 
		// mainDb.setPassword("123");
		mainDb.setUserName("peadm2");
		mainDb.setPassword("peadm2");
		if (!mainDb.open()) 
			EXIT(tr("Error in database"),mainDb.lastError().text());
		execQuery("if not exists(select * from sys.databases where name='basketCraneData') create database basketCraneData",MAIN_DB_CONNECTION);
		execQuery("USE basketCraneData",MAIN_DB_CONNECTION);  // No need for xxxTest, this is MSSQL02, the test server.
	#endif
#else
	QSqlDatabase mainDb = QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	#ifdef SEMILIVE
		mainDb.setDatabaseName("apexBasketCraneTest");//odbc name - SEMILIVE (this ODBC connection MUST point to SAE01MSSQL02, the test server).
		mainDb.setUserName("peadm2");
		mainDb.setPassword("peadm2");
	#else
		// We are live!  But check to make sure we are the one basket crane which is enabled/live
		if (theOneBCToRuleThemAll) {
			mainDb.setDatabaseName("apexBasketCraneMainOdbc");//odbc name - LIVE!
			mainDb.setUserName("sa");
			mainDb.setPassword("123");
		}
		else {
			// Since we are not the LIVE BC, use the test DB on MSSQL02, which is the same DB that SEMILIVE uses.
			QMessageBox::warning(0,"Warning - " + appHostname, "This version is not ENABLED - it will read/write to the TEST EIPCS Database, and has limited PLC communications.");
			mainDb.setDatabaseName("apexBasketCraneTest");//odbc name - SEMILIVE (this ODBC connection MUST point to SAE01MSSQL02, the test server).
			mainDb.setUserName("peadm2");
			mainDb.setPassword("peadm2");
		}
	#endif	
	
	if (!mainDb.open()) 
		EXIT(tr("Error in database"),mainDb.lastError().text());
	execDBQuery("if not exists(select * from sys.databases where name='basketCraneData') create database basketCraneData",MAIN_DB_CONNECTION);
	execDBQuery("USE basketCraneData",MAIN_DB_CONNECTION);
#endif
	///////////////////////////////////////////////////////////APEX_DB_CONNECTION
#ifdef inMyLaptop
	#ifdef EMULATE
	//	if (!QFile(QDir::current().absolutePath()+"/emulationApex.db").exists())forceDemoFillTables=true;
	//	QSqlDatabase apexDb=QSqlDatabase::addDatabase("QSQLITE",APEX_DB_CONNECTION);
	//	apexDb.setDatabaseName(QDir::current().absolutePath()+"/emulationApex.db");//odbc name
	//	if (!apexDb.open()) 
	//		EXIT(tr("Error in database emulationApex.db "),apexDb.lastError().text());
	///*
	//	QSqlDatabase apexDb = QSqlDatabase::addDatabase("QODBC", APEX_DB_CONNECTION);
	//	apexDb.setDatabaseName("apexConnectionTest");//odbc name
	//	apexDb.setUserName("apex2023");
	//	apexDb.setPassword("apex2023");
	//	if (!apexDb.open())
	//		EXIT(tr("Error in database"), apexDb.lastError().text());
	//	execQuery("if not exists(select * from sys.databases where name='PEManager_tmp') create database PEManager_tmp", APEX_DB_CONNECTION);
	//	execQuery("USE PEManager_tmp", APEX_DB_CONNECTION);
	//	*/

		// @@@@@ - Use SAE01MSSQL02 as the test DB.
		LOG("mW.cpp", "EMULATE - apexDb: setDatabaseName & execQuery");
		QSqlDatabase apexDb = QSqlDatabase::addDatabase("QODBC", APEX_DB_CONNECTION);
		apexDb.setDatabaseName("apexEpicsOdbcTest");//odbc name
		apexDb.setUserName("peadm2");
		apexDb.setPassword("peadm2");
		if (!apexDb.open())
			EXIT(tr("Error in database"), apexDb.lastError().text());
		execQuery("if not exists(select * from sys.databases where name='PEManager_tmp') create database PEManager_tmp", APEX_DB_CONNECTION);
		execQuery("USE PEManager_tmp", APEX_DB_CONNECTION);
	#else
		LOG("mW.cpp", "inMyLaptop - apexDb: setDatabaseName & execQuery");
		QSqlDatabase apexDb=QSqlDatabase::addDatabase("QODBC",APEX_DB_CONNECTION);
		apexDb.setDatabaseName("apexEpicsOdbcTest");//odbc name
		apexDb.setUserName("peadm2"); 
		apexDb.setPassword("peadm2");
		if (!apexDb.open()) 
			EXIT(tr("Error in database"),apexDb.lastError().text());
		execQuery("if not exists(select * from sys.databases where name='PEManager_tmp') create database PEManager_tmp",APEX_DB_CONNECTION);
		execQuery("USE PEManager_tmp",APEX_DB_CONNECTION);
	#endif
#else
	QSqlDatabase apexDb=QSqlDatabase::addDatabase("QODBC",APEX_DB_CONNECTION);
	#ifdef SEMILIVE
		apexDb.setDatabaseName("apexEpicsOdbcTest");//odbc name - SEMILIVE (this ODBC connection MUST point to SAE01MSSQL02, the test server).
		apexDb.setUserName("peadm2");
		apexDb.setPassword("peadm2");
	#else
		// We are live!  But check to make sure we are the one basket crane which is enabled/live
		if (theOneBCToRuleThemAll) {
			apexDb.setDatabaseName("apexEpicsOdbc");//odbc name - LIVE!
			apexDb.setUserName("peadm2");
			apexDb.setPassword("peadm2");
		}
		else {
			// Since we are not the LIVE BC, use the test DB on MSSQL02, which is the same DB that SEMILIVE uses.
			apexDb.setDatabaseName("apexEpicsOdbcTest");//odbc name - SEMILIVE (this ODBC connection MUST point to SAE01MSSQL02, the test server).
			apexDb.setUserName("peadm2");
			apexDb.setPassword("peadm2");
		}
	#endif

	execDBQuery("USE PEManager_tmp",APEX_DB_CONNECTION);
#endif

///////////////////////////////////////EMULATE_DB_CONNECTION
#ifdef EMULATE
	//if (!QFile(QDir::current().absolutePath()+"/emulationPlc.db").exists())forceDemoFillTables=true;
	//QSqlDatabase emulateDb = QSqlDatabase::addDatabase("QSQLITE",EMULATE_DB_CONNECTION);
	//emulateDb.setDatabaseName(QDir::current().absolutePath()+"/emulationPlc.db");//odbc name
	//if (!emulateDb.open()) 
	//	EXIT(tr("Error in database emulationPlc.db"),emulateDb.lastError().text());
		// Log database path and plugin/driver diagnostics before attempting open.
		// QString emulateDbPath = QDir::current().absoluteFilePath("emulationPlc.db");
		// if (!QFile(emulateDbPath).exists()) forceDemoFillTables = true;
		// forceDemoFillTables = true;
		forceDemoFillTables = false;
		// QSqlDatabase mainDb = QSqlDatabase::addDatabase("QODBC", MAIN_DB_CONNECTION);
		LOG("mW.cpp", "EMULATE - apexDb: setDatabaseName & execQuery - forceDemoFillTables");
		mainDb.setDatabaseName("apexBasketCraneTest");//odbc name
		// mainDb.setUserName("sa");
		// mainDb.setPassword("123");
		mainDb.setUserName("peadm2");
		mainDb.setPassword("peadm2");
		// if (!mainDb.open())
		// 	EXIT(tr("Error in database"), mainDb.lastError().text());
		// execQuery("if not exists(select * from sys.databases where name='positions') create database positions", MAIN_DB_CONNECTION);
		// execQuery("USE basketCraneData", MAIN_DB_CONNECTION); // No need for xxxTest, this is MSSQL02, the test server.
		LOG("mW.cpp", "EMULATE - apexDb: setDatabaseName & execQuery - forceDemoFillTables -- DONE");
		// Ok, this should create a positions table - but there should already be one on MSSQL02, so it should do nothing. 
		// If the table doesn't exist, it will be created. If it does exist, it will be left as is.
		// I also have a python utility which can keep populating this table based on the REAL database table in MSSQL01.


		// QSqlDatabase emulateDb = QSqlDatabase::addDatabase("QSQLITE", EMULATE_DB_CONNECTION);
		// emulateDb.setDatabaseName(emulateDbPath); // explicit absolute path

#endif
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
	execDBQuery("if not exists (select name from users where name='admin') insert into users (name,pass) values('admin','123')",MAIN_DB_CONNECTION);
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
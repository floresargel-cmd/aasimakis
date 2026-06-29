#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qglobal.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qnamespace.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qdir.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qfile.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qiodevice.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qsettings.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qtextstream.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qcoreapplication.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qtimer.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qdatetime.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qlist.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstringlist.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/image/qicon.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/kernel/qkeysequence.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qinputdialog.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qmessagebox.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qaction.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qboxlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/styles/qstyle.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qbuttongroup.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qframe.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlabel.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qmainwindow.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qpushbutton.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qsplitter.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qtoolbar.h"
#include "../../common/apexG.h"
#include "../Application/Services/MainWindowOrchestrator.h"
#include "../Infrastructure/database/SqlHelper.h"
#include "bs.h"
#include "bst.h"
#include "he.h"
#include "mW.h"
#include "op.h"
#include "ovGL.h"
#include "pp.h"
#include "ppG.h"
#include "ppl.h"
#include <climits>


#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/image/qpixmap.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qapplication.h"
#include "LegacyGlobalState.h"
#include "LegacyLoggingBridge.h"
#include <memory>


mainWindowClass* mainWindowClass::_instance = NULL;

#ifdef VB_PLC_COMMS
QVector<QProcess*> externalProcessesVector;
#endif
static bool g_refreshInProgress = false;
#ifdef USE_LOCAL_DB
QString omavStacketIp = "192.168.0.111";
#else
QString omavStacketIp = "192.168.105.235";
#endif

void createTable(QString tableName, QStringList colNames, QStringList colTypes)
{
	QString stmnt = QString("IF NOT EXISTS (SELECT [name] FROM sys.tables WHERE [name] ='") + tableName + "')";
	stmnt += QString("CREATE TABLE ") + tableName + " (";
	for (int i = 0; i < colNames.length(); i++)
	{
		stmnt += colNames[i] + " " + colTypes[i];
		if (i != (colNames.length() - 1))
			stmnt += ",";
	}
	stmnt += ")";
	SqlHelper::ExecQuery(stmnt, MAIN_DB_CONNECTION);
}

mainWindowClass::mainWindowClass(
	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator)
{
	_instance = this;
	_orchestrator = orchestrator;
	if (_orchestrator)
	{
		_loginName = _orchestrator->GetLoginName();
		_privilegeType = _orchestrator->GetPrivilegeType();
		_globalPressNumber = _orchestrator->GetGlobalPressNumber();
		_havePlcComms = _orchestrator->GetHavePlcComms();
	}
	plcHartBitCounter = 0;
	oven = NULL;
	tcpServer = NULL;
	tcpDialog = NULL;
	basketStacking = NULL;
	profileProductionLayout = NULL;
	helpDlg = NULL;

	centralRootWidget = NULL;
	centralRootLayout = NULL;
	headerCard = NULL;
	headerLayout = NULL;
	titleLabel = NULL;
	subtitleLabel = NULL;
	userPillLabel = NULL;
	pressPillLabel = NULL;
	timePillLabel = NULL;
	press1SegmentButton = NULL;
	press2SegmentButton = NULL;
	pressSegmentGroup = NULL;
	refreshButton = NULL;
	optionsButton = NULL;
	contentCard = NULL;
	contentCardLayout = NULL;
	mainSplitter = NULL;
	mainLayout = NULL;
	mainToolBar = NULL;
	basketPanel = NULL;
	basketPanelLayout = NULL;
	basketPanelTitle = NULL;
	basketPanelSubTitle = NULL;

	profilePanel = NULL;
	profilePanelLayout = NULL;
	profilePanelTitle = NULL;
	profilePanelSubTitle = NULL;

	setWindowIcon(QIcon(":/logo_16x16.png"));
	setFocusPolicy(Qt::StrongFocus);

#ifdef TUX_PLC_COMMS
	if (QFile("settings.txt").exists())
	{
		QString settingsFileName = QString("settings.txt");
		QFile file(settingsFileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QCoreApplication::quit();
			return;
		}

		QTextStream in(&file);
		QString line = in.readLine();
		file.close();

		if (line == "stacker pc")
			orchestrator->SetHavePlcComms(true);
		else
			orchestrator->SetHavePlcComms(false);
	}
	else
	{
		QMessageBox msgBox(this);
		msgBox.setWindowIcon(QIcon(":/logo_16x16.png"));
		msgBox.setText(QString("Is this the stacker PC?"));
		msgBox.setInformativeText(QString("If you select 'Yes', data will be sent to the stacker plc.\nThis answer is stored in the settings.txt file, located in the executable's directory.\nIf you need to change this answer delete the settings.txt file."));
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);

		int ans = msgBox.exec();

		QString settingsFileName = QString("settings.txt");
		QFile file(settingsFileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QCoreApplication::quit();
			return;
		}
		QTextStream out(&file);
		if (ans == QMessageBox::Yes)
		{
			out << "stacker pc";
			orchestrator->SetHavePlcComms(true);
		}
		else
		{
			out << "no stacker pc";
			orchestrator->SetHavePlcComms(false);
		}
		file.close();
	}
#endif

#ifdef VB_PLC_COMMS
	orchestrator->SetHavePlcComms(QDir("plcComms").exists());
#endif

	// ✅ DO NOT reinitialize runtime or DB here
// Bootstrapper already handled everything

// Optional: validate config loaded
	if (LegacyGlobals::MainDbConnectionName.isEmpty())
	{
		QMessageBox::critical(this, tr("Error"), QString("Main DB connection not initialized."));
		QCoreApplication::quit();
		return;
	}

	// Optional logging (recommended)
	LegacyBridge::LogInfo(QString("Using DB Connection: %1").arg(LegacyGlobals::MainDbConnectionName));
	LegacyBridge::LogInfo(QString("User: %1").arg(_loginName));
	LegacyBridge::LogInfo(QString("Privilege: %1").arg(LegacyGlobals::PrivilegeType));



	LegacyBridge::LogInfo(QString("user loged in"));
}

mainWindowClass::~mainWindowClass()
{
	if (_instance == this)
		_instance = NULL;

	LegacyBridge::LogInfo(QString("user loged in"));

#ifdef VB_PLC_COMMS
	terminateExternalProccesses();
#endif
}

mainWindowClass* mainWindowClass::GetInstance()
{
	return _instance;
}

void mainWindowClass::setOrchestrator(std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator)
{
	_orchestrator = orchestrator;
}

std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> mainWindowClass::getOrchestrator() const
{
	return _orchestrator;
}

void mainWindowClass::applyModernTheme()
{
	setStyleSheet(
		"QMainWindow { background-color: #f6f8fb; }"

		"QWidget { font-size: 12px; }"

		"QFrame, QMainWindow { background-color: #f6f8fb; }"

		"QLabel { color: #1f2937; background: transparent; border: none; }"
		"QLabel:disabled { color: #374151; background: transparent; border: none; }"

		"QFrame#HeaderCard, QFrame#ContentCard {"
		"  background-color: #ffffff;"
		"  border: 1px solid #e5e7eb;"
		"  border-radius: 16px;"
		"}"

		"QFrame#InnerPaneCard {"
		"  background-color: #ffffff;"
		"  border: 1px solid #e5e7eb;"
		"  border-radius: 14px;"
		"}"

		"QFrame#InnerPaneCard QLabel {"
		"  color: #374151;"
		"  background: transparent;"
		"  border: none;"
		"}"

		"QFrame#InnerPaneCard QLabel:disabled {"
		"  color: #374151;"
		"  background: transparent;"
		"  border: none;"
		"}"

		"QLabel#TitleLabel {"
		"  font-size: 24px;"
		"  font-weight: bold;"
		"  color: #111827;"
		"}"

		"QLabel#SubtitleLabel {"
		"  font-size: 11px;"
		"  color: #6b7280;"
		"}"

		"QLabel#PaneTitleLabel {"
		"  font-size: 15px;"
		"  font-weight: bold;"
		"  color: #111827;"
		"  background-color: transparent;"
		"  border: none;"
		"}"

		"QLabel#PaneSubTitleLabel {"
		"  font-size: 11px;"
		"  color: #6b7280;"
		"  background-color: transparent;"
		"  border: none;"
		"}"

		"QLabel#PillLabel {"
		"  background-color: #f3f4f6;"
		"  color: #374151;"
		"  border: 1px solid #e5e7eb;"
		"  border-radius: 12px;"
		"  padding: 6px 12px;"
		"  font-size: 11px;"
		"  font-weight: 600;"
		"}"

		"QPushButton#PrimaryButton {"
		"  background-color: #2563eb;"
		"  color: white;"
		"  border: 1px solid #2563eb;"
		"  border-radius: 10px;"
		"  padding: 9px 16px;"
		"  font-weight: bold;"
		"}"

		"QPushButton#PrimaryButton:hover { background-color: #1d4ed8; }"
		"QPushButton#PrimaryButton:pressed { background-color: #1e40af; }"

		"QPushButton#SecondaryButton {"
		"  background-color: #ffffff;"
		"  color: #1f2937;"
		"  border: 1px solid #d1d5db;"
		"  border-radius: 10px;"
		"  padding: 9px 16px;"
		"  font-weight: 600;"
		"}"

		"QPushButton#SecondaryButton:hover { background-color: #f9fafb; }"

		"QPushButton#SegmentButton {"
		"  background-color: #ffffff;"
		"  color: #374151;"
		"  border: 1px solid #d1d5db;"
		"  border-radius: 10px;"
		"  padding: 8px 14px;"
		"  font-weight: 600;"
		"  min-width: 90px;"
		"}"

		"QPushButton#SegmentButton[active=\"true\"] {"
		"  background-color: #eff6ff;"
		"  color: #1d4ed8;"
		"  border: 1px solid #93c5fd;"
		"}"

		"QToolBar {"
		"  background-color: #ffffff;"
		"  border: 1px solid #e5e7eb;"
		"  padding: 4px 8px;"
		"  spacing: 6px;"
		"}"

		"QSplitter::handle { background-color: #e5e7eb; }"
		"QSplitter::handle:horizontal { width: 8px; }"
	);
}

void mainWindowClass::buildHeader()
{
	headerCard = new QFrame(this);
	headerCard->setObjectName("HeaderCard");

	headerLayout = new QHBoxLayout();
	headerLayout->setContentsMargins(20, 18, 20, 18);
	headerLayout->setSpacing(14);
	headerCard->setLayout(headerLayout);

	QVBoxLayout* leftLayout = new QVBoxLayout();
	leftLayout->setContentsMargins(0, 0, 0, 0);
	leftLayout->setSpacing(8);

	QHBoxLayout* leftTopRow = new QHBoxLayout();
	leftTopRow->setContentsMargins(0, 0, 0, 0);
	leftTopRow->setSpacing(10);

	// ✅ LOGO
	logoLabel = new QLabel(this);

	QString logoPath = QCoreApplication::applicationDirPath() + "/Resources/images/apexlogo.png";
	// try also: "/resource/images/apexlogo.png" if your folder name is lowercase

	QPixmap logoPixmap(logoPath);

	if (logoPixmap.isNull())
	{
		QMessageBox::critical(this, "Fatal", QString("Logo failed to load from:\n%1").arg(logoPath));
	}
	else
	{
		logoLabel->setPixmap(
			logoPixmap.scaled(120, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}

	// ✅ TITLE TEXT
	QVBoxLayout* titleLayout = new QVBoxLayout();
	titleLayout->setContentsMargins(0, 0, 0, 0);
	titleLayout->setSpacing(2);

	titleLabel = new QLabel("Production Stacking Plan");
	titleLabel->setObjectName("TitleLabel");

	subtitleLabel = new QLabel("Enterprise layout");
	subtitleLabel->setObjectName("SubtitleLabel");

	titleLayout->addWidget(titleLabel);
	titleLayout->addWidget(subtitleLabel);

	// COMBINE
	leftTopRow->addWidget(logoLabel);
	leftTopRow->addLayout(titleLayout);
	leftTopRow->addStretch();

	QHBoxLayout* pillRow = new QHBoxLayout();
	pillRow->setContentsMargins(0, 0, 0, 0);
	pillRow->setSpacing(8);

	userPillLabel = new QLabel();
	userPillLabel->setObjectName("PillLabel");

	pressPillLabel = new QLabel();
	pressPillLabel->setObjectName("PillLabel");

	timePillLabel = new QLabel();
	timePillLabel->setObjectName("PillLabel");

	pillRow->addWidget(userPillLabel);
	pillRow->addWidget(pressPillLabel);
	pillRow->addWidget(timePillLabel);
	pillRow->addStretch();

	leftLayout->addLayout(leftTopRow);
	leftLayout->addLayout(pillRow);

	headerLayout->addLayout(leftLayout, 1);

	QVBoxLayout* rightLayout = new QVBoxLayout();
	rightLayout->setContentsMargins(0, 0, 0, 0);
	rightLayout->setSpacing(10);

	QHBoxLayout* segmentLayout = new QHBoxLayout();
	segmentLayout->setContentsMargins(0, 0, 0, 0);
	segmentLayout->setSpacing(8);

	press1SegmentButton = new QPushButton("Press 1");
	press1SegmentButton->setObjectName("SegmentButton");
	press1SegmentButton->setProperty("active", "true");

	press2SegmentButton = new QPushButton("Press 2");
	press2SegmentButton->setObjectName("SegmentButton");
	press2SegmentButton->setProperty("active", "false");

	pressSegmentGroup = new QButtonGroup(this);
	pressSegmentGroup->addButton(press1SegmentButton, 1);
	pressSegmentGroup->addButton(press2SegmentButton, 2);

	connect(press1SegmentButton, SIGNAL(clicked()), this, SLOT(press1SegmentClickedSlot()));
	connect(press2SegmentButton, SIGNAL(clicked()), this, SLOT(press2SegmentClickedSlot()));

	segmentLayout->addWidget(press1SegmentButton);
	segmentLayout->addWidget(press2SegmentButton);

	QHBoxLayout* actionLayout = new QHBoxLayout();
	actionLayout->setContentsMargins(0, 0, 0, 0);
	actionLayout->setSpacing(8);

	refreshButton = new QPushButton("Refresh");
	refreshButton->setObjectName("PrimaryButton");

	optionsButton = new QPushButton("Options");
	optionsButton->setObjectName("SecondaryButton");

	connect(refreshButton, SIGNAL(clicked()), this, SLOT(f5Slot()));
	connect(optionsButton, SIGNAL(clicked()), this, SLOT(optionsSlot()));

	actionLayout->addWidget(refreshButton);
	actionLayout->addWidget(optionsButton);

	rightLayout->addLayout(segmentLayout);
	rightLayout->addLayout(actionLayout);

	headerLayout->addLayout(rightLayout);
}

void mainWindowClass::applyInitialSplitterSizes()
{
	if (!mainSplitter)
		return;

	int totalWidth = mainSplitter->width();
	if (totalWidth <= 0)
		return;

	int rightWidth = 300;
	if (rightWidth > totalWidth / 3)
		rightWidth = totalWidth / 3;

	int leftWidth = totalWidth - rightWidth;

	QList<int> sizes;
	sizes << leftWidth << rightWidth;
	mainSplitter->setSizes(sizes);
}

void mainWindowClass::buildContentArea()
{
	contentCard = new QFrame(this);
	contentCard->setObjectName("ContentCard");

	contentCardLayout = new QVBoxLayout();
	contentCardLayout->setContentsMargins(14, 14, 14, 14);
	contentCardLayout->setSpacing(0);
	contentCard->setLayout(contentCardLayout);

	mainSplitter = new QSplitter(Qt::Horizontal, this);
	mainSplitter->setChildrenCollapsible(false);
	mainSplitter->setHandleWidth(8);
	mainSplitter->setOpaqueResize(true);

	// Left: basket stacking modern pane
	basketPanel = new QFrame(this);
	basketPanel->setObjectName("InnerPaneCard");
	basketPanelLayout = new QVBoxLayout();
	basketPanelLayout->setContentsMargins(14, 14, 14, 14);
	basketPanelLayout->setSpacing(10);
	basketPanel->setLayout(basketPanelLayout);

	basketPanelTitle = new QLabel("Basket Layout");
	basketPanelTitle->setObjectName("PaneTitleLabel");

	basketPanelSubTitle = new QLabel("Live stacking visualization");
	basketPanelSubTitle->setObjectName("PaneSubTitleLabel");

	basketStacking = new basketStackingClass(this);
	basketStacking->setStyleSheet(
		"background-color: #f8fafc;"
		"border: 1px solid #e5e7eb;"
		"border-radius: 10px;"
	);
	basketStacking->setMinimumWidth(640);

	basketPanelLayout->addWidget(basketPanelTitle);
	basketPanelLayout->addWidget(basketPanelSubTitle);
	basketPanelLayout->addWidget(basketStacking, 1);

	// Right: production profile modern pane
	profilePanel = new QFrame(this);
	profilePanel->setObjectName("InnerPaneCard");
	profilePanelLayout = new QVBoxLayout();
	profilePanelLayout->setContentsMargins(14, 14, 14, 14);
	profilePanelLayout->setSpacing(10);
	profilePanel->setLayout(profilePanelLayout);

	profilePanelTitle = new QLabel("Production Queue");
	profilePanelTitle->setObjectName("PaneTitleLabel");

	profilePanelSubTitle = new QLabel("Profiles, quantities, and selection details");
	profilePanelSubTitle->setObjectName("PaneSubTitleLabel");

	profileProductionLayout = new profileProductionLayoutClass();
	profileProductionLayout->setStyleSheet(
		"background-color: #f8fafc;"
		"border: 1px solid #e5e7eb;"
		"border-radius: 10px;"
	);
	profileProductionLayout->setOrchestrator(_orchestrator);
	profileProductionLayout->setMinimumWidth(260);
	profileProductionLayout->setMaximumWidth(320);

	profilePanelLayout->addWidget(profilePanelTitle);
	profilePanelLayout->addWidget(profilePanelSubTitle);
	profilePanelLayout->addWidget(profileProductionLayout, 1);

	mainSplitter->addWidget(basketPanel);
	mainSplitter->addWidget(profilePanel);
	mainSplitter->setStretchFactor(0, 6);
	mainSplitter->setStretchFactor(1, 1);

	contentCardLayout->addWidget(mainSplitter);

	connect(profileProductionLayout, SIGNAL(productionChangedSignal(bool)), this, SLOT(productionChangedSlot(bool)));
	connect(basketStacking, SIGNAL(basketSelectedSignal(basketClass*)), this, SLOT(basketSelectedSlot(basketClass*)));
	connect(profileProductionLayout, SIGNAL(productionProfileSelectedSignal(profileProductionClass*)), this, SLOT(productionProfileSelectedSlot(profileProductionClass*)));
}

void mainWindowClass::buildToolbar()
{
	mainToolBar = new QToolBar(tr("Tools"));
	mainToolBar->setAllowedAreas(Qt::TopToolBarArea);
	mainToolBar->setMovable(false);
	mainToolBar->setFloatable(false);
	mainToolBar->setVisible(false);
	addToolBar(Qt::TopToolBarArea, mainToolBar);
}

void mainWindowClass::buildShortcuts()
{
	QAction* f5Action = new QAction("", this);
	addAction(f5Action);
	f5Action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
	connect(f5Action, SIGNAL(triggered()), this, SLOT(f5Slot()));

	QAction* optionsAction = new QAction("", this);
	addAction(optionsAction);
	optionsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	connect(optionsAction, SIGNAL(triggered()), this, SLOT(optionsSlot()));

	QAction* keyLeftAction = new QAction("", this);
	addAction(keyLeftAction);
	keyLeftAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
	connect(keyLeftAction, SIGNAL(triggered()), this, SLOT(keyLeftSlot()));

	QAction* keyRightAction = new QAction("", this);
	addAction(keyRightAction);
	keyRightAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
	connect(keyRightAction, SIGNAL(triggered()), this, SLOT(keyRightSlot()));

	QAction* delAction = new QAction("", this);
	addAction(delAction);
	delAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));
	connect(delAction, SIGNAL(triggered()), this, SLOT(keyDelSlot()));

	QAction* plusAction = new QAction("", this);
	addAction(plusAction);
	plusAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus));
	connect(plusAction, SIGNAL(triggered()), this, SLOT(keyPlusSlot()));

	QAction* nAction = new QAction("", this);
	addAction(nAction);
	nAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	connect(nAction, SIGNAL(triggered()), this, SLOT(keyNSlot()));

	QAction* pAction = new QAction("", this);
	addAction(pAction);
	pAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	connect(pAction, SIGNAL(triggered()), this, SLOT(keyPSlot()));

#ifdef VB_PLC_COMMS
	if (_havePlcComms)
	{
		QAction* plcCommsAction = new QAction("", this);
		addAction(plcCommsAction);
		plcCommsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
		connect(plcCommsAction, SIGNAL(triggered()), this, SLOT(showPlcComsSlot()));
	}
#endif

	QAction* helpAction = new QAction("", this);
	addAction(helpAction);
	helpAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(helpAction, SIGNAL(triggered()), this, SLOT(helpActionSlot()));

	QAction* qtAboutAction = new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
	connect(qtAboutAction, SIGNAL(triggered()), this, SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
}

void mainWindowClass::updatePressSegmentState()
{
	if (!press1SegmentButton || !press2SegmentButton)
		return;

	const bool isPress1 = (_globalPressNumber == 1);

	press1SegmentButton->setProperty("active", isPress1 ? "true" : "false");
	press2SegmentButton->setProperty("active", isPress1 ? "false" : "true");

	press1SegmentButton->style()->unpolish(press1SegmentButton);
	press1SegmentButton->style()->polish(press1SegmentButton);
	press1SegmentButton->update();

	press2SegmentButton->style()->unpolish(press2SegmentButton);
	press2SegmentButton->style()->polish(press2SegmentButton);
	press2SegmentButton->update();
}

void mainWindowClass::updateHeaderStatus()
{
	QString pressText = (_globalPressNumber == 1) ? "Press 1" : "Press 2";

	if (subtitleLabel)
		subtitleLabel->setText(QString("Live Dashboard"));

	if (userPillLabel)
		userPillLabel->setText(QString("User: %1").arg(_loginName));

	if (pressPillLabel)
		pressPillLabel->setText(QString("Active: %1").arg(pressText));

	if (timePillLabel)
	{
		QDateTime nextRun = _nextRefreshRunAt;
		timePillLabel->setText(
			QString("Now: %1 | Next: %2")
			.arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
			.arg(nextRun.toString("hh:mm:ss"))
		);
	}

	if (basketPanelSubTitle)
		basketPanelSubTitle->setText(QString("Live stacking visualization • %1").arg(pressText));

	if (profilePanelSubTitle)
		profilePanelSubTitle->setText(QString("Profiles and details • %1").arg(pressText));

	updatePressSegmentState();
}

void mainWindowClass::applyInitialLayoutSlot()
{
	applyInitialSplitterSizes();
}

void mainWindowClass::createGUI()
{
	applyModernTheme();

	centralRootWidget = new QWidget(this);
	centralRootLayout = new QVBoxLayout();
	centralRootLayout->setContentsMargins(14, 14, 14, 14);
	centralRootLayout->setSpacing(12);
	centralRootWidget->setLayout(centralRootLayout);
	setCentralWidget(centralRootWidget);

	buildHeader();
	centralRootLayout->addWidget(headerCard);

	buildContentArea();
	centralRootLayout->addWidget(contentCard, 1);

	buildToolbar();
	buildShortcuts();

	connect(&mainTimer, SIGNAL(timeout()), this, SLOT(mainTimerSlot()));
	mainTimerSlot();


	connect(&checkDatabaseTimer, SIGNAL(timeout()), this, SLOT(checkDatabaseTimerSlot()));
	checkDatabaseTimerSlot();
	QSettings settings(
		QDir(QCoreApplication::applicationDirPath()).filePath("app.ini"),
		QSettings::IniFormat);

	// Main UI refresh timer
	int mainIntervalSec = settings.value("StackPlan/MainTimerSeconds", 60).toInt();
	if (mainIntervalSec <= 0) mainIntervalSec = 60;

	// DB polling timer
	int dbIntervalSec = settings.value("StackPlan/DbCheckSeconds", 1).toInt();
	if (dbIntervalSec <= 0) dbIntervalSec = 1;

	mainTimer.start(mainIntervalSec * 1000);
	checkDatabaseTimer.start(dbIntervalSec * 1000);
	_nextRefreshRunAt = QDateTime::currentDateTime().addMSecs(checkDatabaseTimer.interval());

	// ✅ NOW SAFE
	LogNextRefreshTime(mainTimer.interval());

	helpDlg = new helpClass(this);

	if (_havePlcComms)
	{
#ifdef VB_PLC_COMMS
		QWidget* tcpServerWidget = new QWidget(this);
		tcpServer = new tcpServerClass(STACKER_READ_PORT, STACKER_WRITE_PORT, tcpServerWidget, false);
		basketStacking->setTcpServer(tcpServer);
		connect(tcpServer, SIGNAL(incomingSignal(QString, int, QString)), this, SLOT(signalFromPlcSlot(QString, int, QString)));
		tcpServer->addTableMonitor(tableMonitorClass::TYPE_INT, STACKER_TABLE_WRITE, 40);
		tcpServer->addTableMonitor(tableMonitorClass::TYPE_INT, STACKER_TABLE_READ, 40);

		tcpDialog = new QDialog(this);
		QVBoxLayout* tcpDialogLayout = new QVBoxLayout();
		tcpDialogLayout->addWidget(tcpServerWidget);
		tcpDialog->setLayout(tcpDialogLayout);

		QProcess* stackerProc = new QProcess();
		QStringList stackerArgs;
		stackerArgs << omavStacketIp << tcpServer->getIp() << QString().number(STACKER_WRITE_PORT) << QString().number(STACKER_READ_PORT) << QString().number(100)
			<< STACKER_TABLE_WRITE << QString().number(50) << QString().number(100)
			<< STACKER_TABLE_READ << QString().number(50) << QString().number(100);
		stackerProc->start(QDir::currentPath() + "//plcComms//omavStackerPlcComms.exe ", stackerArgs);
		externalProcessesVector.append(stackerProc);
#endif

#ifdef TUX_PLC_COMMS
		tuxipServer = NULL;
		tcpServer = NULL;
#endif

		if (_havePlcComms)
			connect(&plcHartBitTimer, SIGNAL(timeout()), this, SLOT(plcHartBitTimerSlot()));
		plcHartBitTimer.start(5000);
	}

	resize(screenWidth, screenHeight);
	updateHeaderStatus();

	QTimer::singleShot(120, this, SLOT(finalizeInitialLayoutSlot()));

}

void mainWindowClass::signalFromTuxPlcSlot(QString ip, QString table, int index, QVariant value)
{
	Q_UNUSED(ip);
#ifdef TUX_PLC_COMMS
	signalFromPlcSlot(table, index, value.toString());
#endif
}

void mainWindowClass::showServerSlot()
{
#ifdef TUX_PLC_COMMS
	tuxipServer->showDialog();
#endif
}

void mainWindowClass::signalFromPlcSlot(QString table, int index, QString value)
{
	auto orchestrator = getOrchestrator();
	if (orchestrator->GetHavePlcComms() && basketStacking)
		basketStacking->stackerPlcMemoryChanged(table, index, value);
}

void mainWindowClass::finalizeInitialLayoutSlot()
{
	applyInitialSplitterSizes();

	if (basketStacking)
		basketStacking->setNeedsToBeCalculated(true);

	if (profileProductionLayout)
		profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);

	f5Slot();
}

void mainWindowClass::showPlcComsSlot()
{
#ifdef VB_PLC_COMMS
	if (tcpDialog)
		tcpDialog->show();
#endif
}

void mainWindowClass::helpActionSlot()
{
	if (helpDlg)
		helpDlg->show();
}

void mainWindowClass::f5Slot()
{
	if (g_refreshInProgress)
		return;

	g_refreshInProgress = true;

	if (refreshButton)
		refreshButton->setEnabled(false);

	QApplication::setOverrideCursor(Qt::WaitCursor);

	if (_orchestrator)
		_orchestrator->ResetUserNeedsUpdate();

	if (basketStacking && profileProductionLayout)
	{
		if (oven)
			oven->clearLayout();

		profileProductionLayout->loadFromDatabase();

		basketStacking->setNeedsToBeCalculated(true);
		basketStacking->calculateLayout(profileProductionLayout);

		if (oven)
			oven->recalculate(basketStacking->getBasketsVector());

		updateHeaderStatus();
	}

	QApplication::restoreOverrideCursor();

	if (refreshButton)
		refreshButton->setEnabled(true);

	g_refreshInProgress = false;
}

void mainWindowClass::optionsSlot()
{
	optionsClass* options = new optionsClass(this, _loginName);
	options->exec();
	delete options;
}

void mainWindowClass::keyLeftSlot()
{
	profileProductionLayout->moveProfileLeft();
	if (profileProductionLayout->getNeedsToBeLoadedFromDatabase())
		f5Slot();
}

void mainWindowClass::keyRightSlot()
{
	profileProductionLayout->moveProfileRight();
	if (profileProductionLayout->getNeedsToBeLoadedFromDatabase())
		f5Slot();
}

void mainWindowClass::keyDelSlot()
{
	profileProductionClass* selected = profileProductionLayout->getSelectedProfileProduction();
	if (selected)
	{
		QMessageBox msgBox(this);
		msgBox.setWindowIcon(QIcon(":/logo_16x16.png"));
		msgBox.setText(QString("You pressed Ctrl+Del."));
		msgBox.setInformativeText(QString("Do you want to delete production order for die %1?").arg(selected->getProfileCode()));
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		if (msgBox.exec() == QMessageBox::Yes)
		{
			profileProductionLayout->deleteProfile();
			basketStacking->setNeedsToBeCalculated(true);
			profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
			f5Slot();
		}
	}
}

void mainWindowClass::keyPlusSlot()
{
	int dieNumb = 0;
	dieNumb = QInputDialog::getInt(
		this,
		tr("This will create a virtual production"),
		tr("Specify the die that you want to add into the production schedule:"),
		1001,
		1001,
		INT_MAX);

	if (dieNumb > 0)
	{
		if (_orchestrator && _orchestrator->CreateVirtualProduction(dieNumb))
		{
			basketStacking->setNeedsToBeCalculated(true);
			profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
		}
		else
		{
			QMessageBox::critical(this, tr("Error"), tr("Failed to create virtual production."));
		}
	}
}

void mainWindowClass::keyNSlot()
{
	profileProductionClass* selected = profileProductionLayout->getSelectedProfileProduction();
	if (selected)
	{
		if (QMessageBox::question(
			this,
			tr("Change profile sequence?"),
			tr("Is the profile %1 the next to be added into the basket?").arg(selected->getProfileCode()),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::Yes) == QMessageBox::Yes)
		{
			if (!_orchestrator || !_orchestrator->MarkProductionAsNext(selected->getProfilesEpicsTableId()))
			{
				QMessageBox::critical(this, tr("Error"), tr("Failed to mark production as next."));
			}
		}
	}
}

void mainWindowClass::keyPSlot()
{
	oven = new ovenGroupLayoutClass(this);
	connect(oven, SIGNAL(productionSequenceChangedSignal()), this, SLOT(productionChangedFromOvenLayoutSlot()));
	connect(oven, SIGNAL(f5Signal()), this, SLOT(f5Slot()));
	oven->recalculate(basketStacking->getBasketsVector());
	oven->show();
}

void mainWindowClass::mainTimerSlot()
{

	// ✅ log next scheduled run again
	LogNextRefreshTime(checkDatabaseTimer.interval());

	setWindowTitle(tr("Production Stacking Plan APEX • %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
	updateHeaderStatus();
}

void mainWindowClass::checkDatabaseTimerSlot()
{
	if (g_refreshInProgress)
		return;

	if (_orchestrator)
	{
		_orchestrator->RunPeriodicChecks();
		_orchestrator->ResetUserNeedsUpdate();
	}

	if (basketStacking)
		basketStacking->setNeedsToBeCalculated(true);

	if (profileProductionLayout)
		profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);

	f5Slot();

	LogNextRefreshTime(checkDatabaseTimer.interval());
	_nextRefreshRunAt = QDateTime::currentDateTime().addMSecs(checkDatabaseTimer.interval());
}

void mainWindowClass::productionChangedFromOvenLayoutSlot()
{
	basketStacking->setNeedsToBeCalculated(true);
	profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
}

void mainWindowClass::plcHartBitTimerSlot()
{
	if (_havePlcComms)
	{
		if (plcHartBitCounter >= 10000)
			plcHartBitCounter = 0;
		plcHartBitCounter++;
		if (tcpServer)
			tcpServer->sendString(QString("%1|0|%2").arg(STACKER_TABLE_WRITE).arg(plcHartBitCounter));
	}
}

void mainWindowClass::productionChangedSlot(bool needsReloadFromDatabase)
{
	if (g_refreshInProgress)
		return;

	NOTIFY_OTHERS;

	if (needsReloadFromDatabase)
	{
		profileProductionLayout->loadFromDatabase();
		profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
	}

	basketStacking->setNeedsToBeCalculated(true);

	if (oven)
		oven->setNeedsToBeCalculated(true);
}

void mainWindowClass::basketSelectedSlot(basketClass* b)
{
	if (b && profileProductionLayout)
		profileProductionLayout->setVisibleProfile(b->getFirstProfileCode());
}

void mainWindowClass::productionProfileSelectedSlot(profileProductionClass* p)
{
	if (p && basketStacking)
		basketStacking->setVisibleBasketWithProfileTableId(p->getProfilesEpicsTableId());
}

void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this, tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}

void mainWindowClass::press1RadioBtnClickedSlot(bool checked)
{
	Q_UNUSED(checked);
	if (_globalPressNumber == 2)
	{
		_globalPressNumber = 1;

		if (_orchestrator)
			_orchestrator->SetGlobalPressNumber(1);

		if (basketStacking)
			basketStacking->setNeedsToBeCalculated(true);
		if (profileProductionLayout)
			profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
		updateHeaderStatus();
		f5Slot();
	}
	else
	{
		updateHeaderStatus();
	}
}

void mainWindowClass::press2RadioBtnClickedSlot(bool checked)
{
	Q_UNUSED(checked);
	if (_globalPressNumber == 1)
	{
		_globalPressNumber = 2;
		if (_orchestrator)
			_orchestrator->SetGlobalPressNumber(2); //

		if (basketStacking)
			basketStacking->setNeedsToBeCalculated(true);
		if (profileProductionLayout)
			profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
		updateHeaderStatus();
		f5Slot();
	}
	else
	{
		updateHeaderStatus();
	}
}

void mainWindowClass::press1SegmentClickedSlot()
{
	if (_globalPressNumber != 1)
	{
		_globalPressNumber = 1;

		if (_orchestrator)
			_orchestrator->SetGlobalPressNumber(1);

		if (basketStacking)
			basketStacking->setNeedsToBeCalculated(true);
		if (profileProductionLayout)
			profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
		updateHeaderStatus();
		f5Slot();
	}
	else
	{
		updateHeaderStatus();
	}
}

void mainWindowClass::press2SegmentClickedSlot()
{
	if (_globalPressNumber != 2)
	{
		_globalPressNumber = 2;
		if (_orchestrator)
			_orchestrator->SetGlobalPressNumber(2); //
		if (basketStacking)
			basketStacking->setNeedsToBeCalculated(true);
		if (profileProductionLayout)
			profileProductionLayout->setNeedsToBeLoadedFromDatabase(true);
		updateHeaderStatus();
		f5Slot();
	}
	else
	{
		updateHeaderStatus();
	}
}

void mainWindowClass::setMainAppWidth(int w)
{
	screenWidth = w;
}

void mainWindowClass::setMainAppHeight(int h)
{
	screenHeight = h;
}

void mainWindowClass::setPress1(bool p1)
{
	press1 = p1;
}

void mainWindowClass::setPress2(bool p2)
{
	press2 = p2;
}

void mainWindowClass::showMainToolBarWidget(bool visibility)
{
	if (mainToolBar)
		mainToolBar->setVisible(visibility);
}

void mainWindowClass::LogNextRefreshTime(int intervalMs)
{
	QDateTime nextRun = QDateTime::currentDateTime().addMSecs(intervalMs);

	QString message = QString("Next refresh run at: %1")
		.arg(nextRun.toString("yyyy-MM-dd hh:mm:ss"));

	LegacyBridge::LogInfo(message);
}
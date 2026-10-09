#ifndef MW_H
#define MW_H

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qtimer.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qdialog.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qboxlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qbuttongroup.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qframe.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlabel.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qmainwindow.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qpushbutton.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qsplitter.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qtoolbar.h"
#include "../../common/tcpTux.h"
#include "bs.h"
#include "pp.h"
#include <serv.h>

#include "bst.h"
#include "he.h"
#include "ovGL.h"
#include "ppl.h"

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qdatetime.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include <memory>

namespace StackingPlan
{
	namespace Application
	{
		namespace Services
		{
			class MainWindowOrchestrator;
		}
	}
}

class mainWindowClass : public QMainWindow
{
	Q_OBJECT

private:
	static mainWindowClass* _instance;

	QTimer mainTimer;
	QTimer checkDatabaseTimer;
	QTimer plcHartBitTimer;
	QDateTime _nextRefreshRunAt;

	QHBoxLayout* mainLayout;

	int screenWidth = 1920;
	int screenHeight = 1080;

	QToolBar* mainToolBar;

	bool press1 = false;
	bool press2 = false;

#ifdef TUX_PLC_COMMS
	tuxipServerClass* tuxipServer;
#endif

	tcpServerClass* tcpServer;
	QDialog* tcpDialog;

	basketStackingClass* basketStacking;
	profileProductionLayoutClass* profileProductionLayout;
	helpClass* helpDlg;
	ovenGroupLayoutClass* oven;

	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> _orchestrator;

	int plcHartBitCounter;

	QWidget* centralRootWidget;
	QVBoxLayout* centralRootLayout;

	QFrame* headerCard;
	QHBoxLayout* headerLayout;

	QLabel* titleLabel;
	QLabel* subtitleLabel;
	QLabel* logoLabel;

	QLabel* userPillLabel;
	QLabel* pressPillLabel;
	QLabel* timePillLabel;

	QPushButton* press1SegmentButton;
	QPushButton* press2SegmentButton;
	QButtonGroup* pressSegmentGroup;

	QPushButton* refreshButton;
	QPushButton* optionsButton;

	QFrame* contentCard;
	QVBoxLayout* contentCardLayout;
	QSplitter* mainSplitter;
	QFrame* basketPanel;
	QVBoxLayout* basketPanelLayout;
	QLabel* basketPanelTitle;
	QLabel* basketPanelSubTitle;

	QFrame* profilePanel;
	QVBoxLayout* profilePanelLayout;
	QLabel* profilePanelTitle;
	QLabel* profilePanelSubTitle;

private:
	void applyModernTheme();
	void buildHeader();
	void buildContentArea();
	void buildToolbar();
	void buildShortcuts();
	void updateHeaderStatus();
	void updatePressSegmentState();
	void applyInitialSplitterSizes();
	QString _loginName;
	int _privilegeType = 0;
	int _globalPressNumber = 1;
	bool _havePlcComms = false;

public:
	mainWindowClass(
		std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator);

	~mainWindowClass();

	static mainWindowClass* GetInstance();

	void createGUI();

	void setMainAppWidth(int w);
	void setMainAppHeight(int h);

	int getMainAppWidth() { return screenWidth; }
	int getMainAppHeight() { return screenHeight; }

	void setPress1(bool p1);
	void setPress2(bool p2);

	bool getPress1() { return press1; }
	bool getPress2() { return press2; }

	void showMainToolBarWidget(bool visibility);
	void LogNextRefreshTime(int intervalMs);

	void setOrchestrator(std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator);
	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> getOrchestrator() const;

public slots:
	void mainTimerSlot();
	void checkDatabaseTimerSlot();
	void plcHartBitTimerSlot();
	void productionChangedSlot(bool needsReloadFromDatabase);
	void productionChangedFromOvenLayoutSlot();
	void f5Slot();
	void optionsSlot();
	void keyLeftSlot();
	void keyRightSlot();
	void keyDelSlot();
	void keyPlusSlot();
	void keyNSlot();
	void keyPSlot();
	void showPlcComsSlot();
	void helpActionSlot();
	void basketSelectedSlot(basketClass* b);
	void productionProfileSelectedSlot(profileProductionClass* p);
	void signalFromPlcSlot(QString table, int index, QString value);
	void signalFromTuxPlcSlot(QString ip, QString table, int index, QVariant value);
	void showServerSlot();
	void qtAboutSlot();
	void press1RadioBtnClickedSlot(bool checked);
	void press2RadioBtnClickedSlot(bool checked);
	void applyInitialLayoutSlot();
	void press1SegmentClickedSlot();
	void press2SegmentClickedSlot();
	void finalizeInitialLayoutSlot();
};

#endif

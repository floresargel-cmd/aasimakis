#include "../../../../libs/_deploy/includes/utilities/varClasses.h"
#include "../App/ApplicationBootstrapper.h"
#include "mW.h"
#include <Gu.h>


#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#endif

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/plugin/qplugin.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstringlist.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qmessagebox.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qapplication.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qglobal.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qlogging.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qdir.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qfileinfo.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qcoreapplication.h"
#include "ppG.h"


#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void messageOutPut(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	Q_UNUSED(type);
	Q_UNUSED(context);

	if (msg.left(6) == "libpng")
		return;

	QMessageBox::critical(0, "Fatal", msg);
	return;
}
#else
void messageOutPut(QtMsgType type, const char* msg)
{
	Q_UNUSED(type);

	QMessageBox::critical(0, "Fatal", msg);
}
#endif

#ifdef _DEBUG
#endif

int main(int argc, char* argv[])
{
	QDir dir(QDir::current());
	dir.cdUp();
	dir.cdUp();
	dir.cdUp();

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	qApp->addLibraryPath(dir.absolutePath() + "/libs/#qt/qt.5.4.0.2013x64/dlls/");
	qApp->addLibraryPath(QDir::current().absolutePath());

#ifdef _DEBUG
	qInstallMessageHandler(messageOutPut);
#endif

#else
	qApp->addLibraryPath(dir.absolutePath() + "/libs/#qt/qt.4.8.1.2010x32/dlls/");

#ifdef _DEBUG
	qInstallMsgHandler(messageOutPut);
#endif
#endif

	QApplication app(argc, argv);

	QStringList args = QCoreApplication::arguments();

	bool isRestarting = args.contains("--restart");

	if (!isRestarting)
	{
		QFileInfo pathInfo(argv[0]);
		if (isExeRuningClass::isRunning(pathInfo.fileName()))
		{
			uExit("Error", QString("The application %1 is already running.").arg(pathInfo.fileName()));
		}
	}

	StackingPlan::App::ApplicationBootstrapper bootstrapper;
	bootstrapper.Initialize();

	mainWindowClass* mainW = new mainWindowClass(
		bootstrapper.GetMainWindowOrchestrator());

	mainW->setMainAppWidth(STARTUP_SCREEN_WIDTH);
	mainW->setMainAppHeight(STARTUP_SCREEN_HEIGHT);
	mainW->setPress1(STARTUP_PRESS1);
	mainW->setPress2(STARTUP_PRESS2);

	for (int pos = 0; args.size() > pos; pos++)
	{
		if (args[pos].startsWith("--screenX="))
		{
			QString w = args[pos].mid(10, -1);
			mainW->setMainAppWidth(w.toInt());
		}

		if (args[pos].startsWith("--screenY="))
		{
			QString h = args[pos].mid(10, -1);
			mainW->setMainAppHeight(h.toInt());
		}
	}

	if (args.contains("--press1"))
	{
		mainW->setPress1(true);
		mainW->setPress2(false);
	}

	if (args.contains("--press2"))
	{
		mainW->setPress1(false);
		mainW->setPress2(true);
	}

	mainW->createGUI();
	mainW->resize(mainW->getMainAppWidth(), mainW->getMainAppHeight());
	mainW->showMaximized();

	mainW->setPress1(args.contains("--press1"));
	mainW->setPress2(args.contains("--press2"));

	if ((mainW->getPress1() && mainW->getPress2()) || (!mainW->getPress1() && !mainW->getPress2()))
	{
	}
	else if (mainW->getPress1())
	{
		mainW->press1RadioBtnClickedSlot(true);
		mainW->showMainToolBarWidget(false);
	}
	else if (mainW->getPress2())
	{
		mainW->press2RadioBtnClickedSlot(true);
		mainW->showMainToolBarWidget(false);
	}

	return app.exec();
}

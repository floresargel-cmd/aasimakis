#include "pch.h"
#include "mW.h"
#include "environment.h"
#include "observerWindow.h"
#include "splashBranding.h"
#include <QLockFile>
#include <QDir>
#include <QSysInfo>
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
void messageOutPut(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	if (msg.contains("libpng"))return;if (msg.contains("libvlc"))return;if (msg.contains("QWidget::paintEngine: Should no longer be called"))return;
	QMessageBox::critical(0,"Fatal",msg);
}
QSplashScreen *splash;
int main(int argc, char *argv[])
{
#ifdef _DEBUG
	qInstallMessageHandler(messageOutPut);
#endif
	QApplication app(argc, argv);
	QString configPath = QCoreApplication::applicationDirPath() + "/basketCrane2.ini";
	const QStringList arguments = app.arguments();
	if (arguments.size() != 1) {
		if (arguments.size() != 3 || arguments[1] != "--config") {
			QMessageBox::critical(nullptr, "Configuration", "Usage: apexBasketCrane2.exe [--config path-to-ini]");
			return 1;
		}
		configPath = QFileInfo(arguments[2]).absoluteFilePath();
	}
	Crane2::Environment& environment = Crane2::Environment::instance();
	QString error;
	if (!environment.load(configPath, QSysInfo::machineHostName(), &error)) {
		QMessageBox::critical(nullptr, "Configuration", error);
		return 1;
	}
	// One instance per mode, independent of the executable name/config filename.
	// LiveObserver and Test can coexist with Live without taking its lock.
	QLockFile instanceLock(QDir::tempPath() + "/apexBasketCrane2-" + environment.name() + ".lock");
	instanceLock.setStaleLockTime(0);
	if (!instanceLock.tryLock()) {
		QMessageBox::critical(nullptr, "Already running", "Basket Crane 2 is already running in " + environment.name() + " mode.");
		return 1;
	}
	if (!environment.openDatabases(&error)) {
		QMessageBox::critical(nullptr, "Database configuration", error);
		return 1;
	}
	// Do not construct the control window in observer mode: its constructors,
	// timers, log widgets and recovery routines have database/PLC side effects.
	if (environment.isObserver()) {
		Crane2::ObserverWindow window;
		window.showMaximized();
		return app.exec();
	}
	QPixmap pixmap=basketCraneSplashPixmap();
	splash=new QSplashScreen(pixmap);
	splash->setFont(QFont("Arial",12));
	splash->show();
	mainWindowClass window;
	splash->finish(&window);
	window.showMaximized();
	return app.exec();
}

#include "mW.h"
#include <QtPlugin>
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif
//Q_IMPORT_PLUGIN(qsqlodbc)
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void messageOutPut(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	if (msg.left(6)=="libpng")
		return;
	QMessageBox::critical(0,"Fatal",msg);
	return;
}
#else
void messageOutPut(QtMsgType type, const char *msg)
{
	QMessageBox::critical(0,"Fatal",msg);
}
#endif
#ifdef _DEBUG
//#include "vld.h"
#endif

/* comment out by Gene
int main(int argc, char *argv[])
{
	QDir dir(QDir::current());dir.cdUp();dir.cdUp();dir.cdUp();
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	qApp->addLibraryPath(dir.absolutePath()+"/libs/#qt/qt.5.4.0.2013x64/dlls/");
	qApp->addLibraryPath(QDir::current().absolutePath());
	#ifdef _DEBUG
		qInstallMessageHandler(messageOutPut);
	#endif
#else
	qApp->addLibraryPath(dir.absolutePath()+"/libs/#qt/qt.4.8.1.2010x32/dlls/");
	#ifdef _DEBUG
		qInstallMsgHandler(messageOutPut);
	#endif
#endif
	QApplication app(argc,argv);
	QFileInfo pathInfo(argv[0]);
	if ((isExeRuningClass::isRunning(pathInfo.fileName())))
		uExit("Error",QString("The application %1 is already running.").arg(pathInfo.fileName()));
	mainWindowClass mainW;
	mainW.resize(1920,1080);
	
	mainW.show();
	return app.exec();
}
*/
/* new code below added by Gene*/
int main(int argc, char* argv[])
{
	QDir dir(QDir::current()); dir.cdUp(); dir.cdUp(); dir.cdUp();

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

	if (!isRestarting) {
		QFileInfo pathInfo(argv[0]);
		if (isExeRuningClass::isRunning(pathInfo.fileName())) {
			uExit("Error", QString("The application %1 is already running.").arg(pathInfo.fileName()));
		}
	}



































































	mainWindowClass mainW;
	mainW.resize(1920, 1080);
	mainW.showMaximized();
	mainW.show();

	return app.exec();
}

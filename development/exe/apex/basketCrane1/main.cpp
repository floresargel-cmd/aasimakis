#include "mW.h"
#include <QtPlugin>
#include <QtGlobal>

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

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);  //moved here by gene
	qApp->addLibraryPath("C:/assimakis/development/#builds/apexbasketcrane1");
	qApp->addLibraryPath("C:/assimakis/development/libs/#qt/qt.5.4.0.2013x64/dlls/");
	qApp->addLibraryPath(QDir::current().absolutePath());
	qApp->addLibraryPath(QCoreApplication::applicationDirPath());
	// for( int x=0; x < QSqlDatabase::drivers().count(); x++)
	// 	LOG("SQL Drivers", "SQL driver:" + QSqlDatabase::drivers()[x]);
	QDir dir(QDir::current());dir.cdUp();dir.cdUp();dir.cdUp();
	// LOG("Qt plugins path", QLibraryInfo::location(QLibraryInfo::PluginsPath));

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	qApp->addLibraryPath(dir.absolutePath()+"/libs/#qt/qt.5.4.0.2013x64/dlls/");
	qApp->addLibraryPath("C:/assimakis/development/libs/#qt/qt.5.4.0.2013x64/dlls/");
	qDebug() << QCoreApplication::libraryPaths();
	qApp->addLibraryPath(QDir::current().absolutePath());
	#ifdef _DEBUG
		qInstallMessageHandler(messageOutPut);
	#endif
	//for (int x = 0; x < QCoreApplication::libraryPaths().count(); x++)
	// 	LOG("Library Path(s)", "library path:" + QCoreApplication::libraryPaths()[x]);

#else
	qApp->addLibraryPath(dir.absolutePath()+"/libs/#qt/qt.4.8.1.2010x32/dlls/");
	#ifdef _DEBUG
		qInstallMsgHandler(messageOutPut);
	#endif
	for (int x = 0; x < QCoreApplication::libraryPaths().count(); x++)
		LOG("Library Path(s)", "library path:" + QCoreApplication::libraryPaths()[x]);

#endif
	
	// ensure local plugins folder is searched first (do this immediately after creating QApplication)
	QString localPlugins = QCoreApplication::applicationDirPath() + "/plugins";
	QCoreApplication::addLibraryPath(localPlugins);
	// LOG("Plugin Path(s)", localPlugins);
	// LOG("QLibraryInfo::PluginsPath:", QLibraryInfo::location(QLibraryInfo::PluginsPath));
	// for( int x=0; x < QCoreApplication::libraryPaths().count(); x++)
	//	LOG("SQL Drivers", "SQL driver:" + QCoreApplication::libraryPaths()[x]);
	// for (int x = 0; x < QSqlDatabase::drivers().count(); x++)
	//	LOG("Available SQL driver(s)", QSqlDatabase::drivers()[x]);

	// coment out by gene  QApplication app(argc, argv);
	QFileInfo pathInfo(argv[0]);

	if ((isExeRuningClass::isRunning(pathInfo.fileName())))
		uExit("Error",QString("The application %1 is already running.").arg(pathInfo.fileName()));

	mainWindowClass mainWindow;
	mainWindow.resize(800,800);
	mainWindow.showMaximized();
	
	return app.exec();
}

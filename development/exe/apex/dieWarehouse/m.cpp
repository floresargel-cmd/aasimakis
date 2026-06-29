#include "pch.h"
#include "mW.h"
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif
QSplashScreen *splash;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void messageOutPut(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	if (msg.contains("libvlc"))
		return;
	if (msg.contains("libpng warning"))
		return;
	if (msg.contains("QWidget::paintEngine: Should no longer be called"))
		return;
	QMessageBox::critical(0,"Fatal",msg);
}
#else
void messageOutPut(QtMsgType type, const char *msg)
{
	QMessageBox::critical(0,"Fatal",msg);
}
#endif
bool adminMode;
#ifdef _DEBUG
//#include "vld.h"
#endif
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
	QApplication app(argc, argv);
	QFileInfo pathInfo(argv[0]);
	if ((isExeRuningClass::isRunning(pathInfo.fileName())))
		uExit("Error",QString("The application %1 is already running.").arg(pathInfo.fileName()));
	//
	bool useTranslation=false;
	//
	adminMode=false;

#ifdef _DEBUG
	adminMode=true;
#endif
	QPixmap pixmap(":/splash.png");
	splash=new QSplashScreen(pixmap);
	splash->setFont(QFont("Arial",12));
	splash->show();
	mainWindowClass window(useTranslation);
	splash->finish(&window);
	window.show();
	window.showMaximized();
	return app.exec(); 
}  

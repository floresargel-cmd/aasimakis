#include "mw.h"
#include <qmessagebox.h>
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
#endif
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
int main(int argc, char *argv[])
{
	QDir dir(QDir::current());dir.cdUp();dir.cdUp();dir.cdUp();dir.cdUp();
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
	mainWindowClass window;
	window.resize(1475,700);
	window.show();
	return app.exec();
}

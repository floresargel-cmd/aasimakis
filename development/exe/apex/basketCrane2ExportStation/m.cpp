#include "pch.h"
#include "mW.h"
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
bool makeTicketSpinReadOnly=false;
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
	QFileInfo pathInfo(argv[0]);
	if ((isExeRuningClass::isRunning(pathInfo.fileName())))
		uExit("Error",QString("The application %1 is already running.").arg(pathInfo.fileName()));
	if (QGuiApplication::queryKeyboardModifiers()==Qt::ControlModifier)
		makeTicketSpinReadOnly=false;
	QPixmap pixmap(":/splash.png");
	splash=new QSplashScreen(pixmap);
	splash->setFont(QFont("Arial",12));
	splash->show();
	mainWindowClass window;
	splash->finish(&window);
	window.show();
	return app.exec();
}

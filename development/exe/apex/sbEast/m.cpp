#include "pch.h"
#include "mW.h"
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);
void messageOutPut(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	if (msg.contains("libpng"))return;if (msg.contains("libvlc"))return;if (msg.contains("QWidget::paintEngine: Should no longer be called"))return;
	QMessageBox::critical(0,"Fatal",msg);
}
QSplashScreen *splash;
int main(int argc, char *argv[])
{
	adminMode=false;
#ifdef _DEBUG
	adminMode=true;
	qInstallMessageHandler(messageOutPut);
#endif
	QApplication app(argc, argv);
	QFileInfo pathInfo(argv[0]);
	if ((isExeRuningClass::isRunning(pathInfo.fileName())))
		uExit("Error",QString("The application %1 is already running.").arg(pathInfo.fileName()));
	//
	if((GetAsyncKeyState(VK_CONTROL)&0x8000)||(GetAsyncKeyState(VK_LCONTROL)&0x8000)||(GetAsyncKeyState(VK_RCONTROL)&0x8000))
	{
		adminMode=getPassword("Password is needed to start in admin mode.","Please give the password for admin mode",passStrong);
	}
	QPixmap pixmap(":/splash.png");
	splash=new QSplashScreen(pixmap);
	splash->setFont(QFont("Arial",12));
	splash->show();
	mainWindowClass window;
	splash->finish(&window);
	window.showMaximized();
	return app.exec(); 
}  

#include "mw.h"
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
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
bool isExeRuning(QString nameOfExe)
{
	//check if is running
    DWORD aProcesses[9024], cbNeeded, cProcesses;
    unsigned int i;
	int numOfExeFoundRunning=0;
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return false;
	 cProcesses = cbNeeded / sizeof(DWORD);
	for ( i = 0; i < cProcesses; i++ )
	{
		if( aProcesses[i] != 0 )
		{
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
			HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE, aProcesses[i] );
			if (NULL != hProcess )
			{
				HMODULE hMod; DWORD cbNeeded;
				if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),&cbNeeded) )
				{
					GetModuleBaseName( hProcess, hMod, szProcessName,sizeof(szProcessName)/sizeof(TCHAR) );
					if (QString(szProcessName)==nameOfExe)
						numOfExeFoundRunning++;
				}   
			}
			CloseHandle( hProcess );
		}
	}
	if (numOfExeFoundRunning==2)
	{
		return true;
	}
	return false;
}                                                           



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
	app.setStyle("Cleanlooks");
	QFileInfo pathInfo(argv[0]);
	if ((isExeRuning(pathInfo.fileName())))
		EXIT(QString::fromLocal8Bit("Error"),QString::fromLocal8Bit("The application ")+pathInfo.fileName()+QString::fromLocal8Bit(" is allready runing."));

	mainWindowClass mainWindow;
	mainWindow.setGeometry(QRect(20,30,1800,1000));
	mainWindow.showMaximized();
	return app.exec();
}

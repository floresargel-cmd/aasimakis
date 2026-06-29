#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Gd.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "allPos.h"
#include "crane.h"
#include "exp.h"
#include "usageDlg.h"
#include "autoWdg.h"
#include "settingsDlg.h"
#include "search.h"
#include "hmiB.h"
class mainWindowClass : public QMainWindow				
{												
	tuxipServerClass											*tuxipServer;
	allPossClass													*allPos;
	craneClass														*crane;
	myQGraphicsViewClass									*layoutGraphicsView;
	autoWidgetClass												*autoWidget;
	//
	QAction																*showTuxServerAction;
	hmiDlgBasicClass											*hmiDlgBasic;
	handShakeClass												*handShake;
	Q_OBJECT								
public:										
	mainWindowClass(bool useTranslation);
	~mainWindowClass();
	QString takeDatabaseBackup(QString fName);
	void clearBackups(QString dir);
	void keyPressed(QChar c);
private:									
public slots:								
	void mainTimerSlot();						
	void onBackupDatabaseTimerSlot();						
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();						
	void createReportActionSlot();	
	void userDatabaseBackupActionSlot();			
	void showUsageDialogSlot();
	void qtAboutSlot();
	void openWorkStationDlg();
	void stopSlot();
	void backupFinishedSlot(int,QProcess::ExitStatus);
	void resetGraphicsViewZoom();
	void hmiDlgBasicButtonClickedSlot();
	void handShakeStateChangedSlot(bool s);
};											
#endif

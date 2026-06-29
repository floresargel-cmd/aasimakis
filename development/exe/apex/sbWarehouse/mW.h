#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Gsw.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "allPos.h"
#include "crane.h"
#include "expWdg.h"
#include "usageDlg.h"
#include "autoWdg.h"
#include "ximage.h"
#include "search.h"
#include "hmiB.h"
#include "logW.h"
#include "smtpMine.h"
#define QUAZIP_STATIC
#include "JlCompress.h"
#include "ximage.h"
#include "pincer1.h"
#include "pincer2.h"
#include "pincer1Conv.h"
#include "pincer2Conv.h"
class mainWindowClass : public QMainWindow				
{												
	tuxipServerClass											*tuxipServer;
	allPossClass													*allPos;
	craneClass														*crane;
	pincer1Class													*pincer1;
	pincer1ConvPositionsClass							*pincer1Conv;
	pincer2Class													*pincer2;
	pincer2ConvPositionsClass							*pincer2Conv;
	myQGraphicsViewClass									*layoutGraphicsView;
	QGraphicsScene												*layoutScene;
	exportStationWidgetClass							*exportFront;
	exportStationWidgetClass							*exportTriple1;
	exportStationWidgetClass							*exportTriple2;
	exportStationWidgetClass							*exportDouble;
	autoWidgetClass												*autoWidget;
	//
	QAction																*showTuxServerAction;
	QAction																*destackerSbReadyAction;
	QPushButton														*showPincer1DlgBtn;
	QPushButton														*showPincer1ConvDlgBtn;
#ifndef sbWarehouse
	QPushButton														*showPincer2DlgBtn;
	QPushButton														*showPincer2ConvDlgBtn;
#endif
	QDialog																*pincer1Dlg;
	QDialog																*pincer1ConvDlg;
#ifndef sbWarehouse
	QDialog																*pincer2Dlg;
	QDialog																*pincer2ConvDlg;
#endif
	hmiDlgBasicClass											*hmiDlgBasic;
	handShakeClass												*handShakeSbCrane;
	bool																	handShakeSbCommsOk;
	handShakeClass												*handShakeCrane2;
	bool																	handShakeCrane2CommsOk;
	QVector<logWidgetClass*>							logWidgets;
	QDialog																*dtmDlg;
	QLabel																*messageLabel;

	bool																	isRemoteBackupServerOnline;
	QLabel																*remoteBackupCheckLabel;
	QString																backupMessage;
	QSerialPort														*serialPort;
	QString																serialPortStr;
	QLabel																*serialPortCheckLabel;
	QTimer																*serialPortNotReadingTimer;
	QVector<hmiPushButtonWidgetClass*>		pushButtonsVector;
	Q_OBJECT								
public:										
	mainWindowClass();
	~mainWindowClass();
	QString takeDatabaseBackup(QString fName);
	void clearBackups(QString dir);
	void setPlcComments();
	void showRemoteBackupError(QString message);
	void sendEmptySbEmail(QString subject,QString emailText);
private:									
public slots:								
	void mainTimerSlot();						
	void statisticsTimerSlot();						
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();						
	void showPincer1DlgSlot();						
	void showPincer1ConvDlgActionDlgSlot();						
	void showPincer2DlgSlot();						
	void showPincer2ConvDlgActionDlgSlot();						
	void createReportActionSlot();	
	void userDatabaseBackupActionSlot();			
	void showUsageDialogSlot();
	void destackerSbReadyActionSlot();
	void qtAboutSlot();
	void stopSlot();
	void resetGraphicsViewZoom();
	void hmiDlgBasicButtonClickedSlot();
	void handShakeSbCraneStateChangedSlot(bool s);
	void handShakeCrane2ChangedSlot(bool s);
	void dtmBtnSlot();
	void zoomToPositionSlot(int posId);
	void backupTimerSlot();

	void onCheckRemoteBackupTimerSlot();						
	void readyReadSerialPortSlot();
	void serialPortNotReadingTimerSlot();
	void checkTicketWeightTimerSlot();
};
#endif

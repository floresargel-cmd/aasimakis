#ifndef mw_H
#define mw_H

#include "Gf.h"
#include "rollersDlg.h"
#include "forkPackingWdgt.h"
#include "serialPortDlg.h"
#include "forkUserWdgt.h"
class mainWindowClass : public QMainWindow				
{												
	tuxipServerClass											*tuxipServer;
	forkPackingWdgtClass									*forkPackingWdgt=nullptr;
	rollersDlgClass												*rollersDlg;
	QAction																*showTuxServerAction;
	handShakeClass												*handShakeCrane3;
	QToolBox															*toolBox;
	serialPortDlgClass										*serialPortStation1Dlg=nullptr;
	serialPortDlgClass										*serialPortStation2Dlg=nullptr;
	forkUserWdgtClass											*forkUserWdgt=nullptr;

	QAction																*showSerialPortStation1DlgAction=nullptr;
	QAction																*showSerialPortStation2DlgAction=nullptr;
	//QAction																*setSerialPortForForming1Action=nullptr;
	//QAction																*setSerialPortForForming2Action=nullptr;
	QProgressBar													*laserScanerOkProgressBar;
	int																		laserScanerTimeToReset=10;
	Q_OBJECT								
public:										
	mainWindowClass();
	~mainWindowClass();
public slots:								
	void mainTimerTriggeredSlot();						
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();
	void showSerialPortStation1DlgSlot();
	void showSerialPortStation2DlgSlot();
	void setSerialPortForForming1StationActionTriggeredSlot();
	void setSerialPortForForming2StationActionTriggeredSlot();
	void qtAboutSlot();
	void stopSlot();
	void hmiDlgButtonClickedSlot();
	void rollersDlgButtonClickedSlot();
	void handShakeCrane3ChangedSlot(bool s);
};											
#endif

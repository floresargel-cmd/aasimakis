#ifndef mw_H
#define mw_H

#include "Gstr.h"
#include "stationWdgt.h"
#include "../packingFork/serialPortDlg.h"
class mainWindowClass : public QMainWindow				
{												
	tuxipServerClass											*tuxipServer;
	stationWdgtClass											*stationWdgt=nullptr;
	serialPortDlgClass										*serialPortDlg=nullptr;
	QAction																*showTuxServerAction;
	handShakeClass												*handShakeCrane3=nullptr;;
	handShakeClass												*handShakeOldStrapping=nullptr;;
	QToolBox															*toolBox;
	QAction																*showSerialDlgAction=nullptr;
	QVector<hmiPushButtonWidgetClass*>		pushButtonsVector;	
	Q_OBJECT								
public:										
	mainWindowClass();
	~mainWindowClass();
public slots:								
	void mainTimerTriggeredSlot();						
	void definePacketInExitActionTriggeredSlot();
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();						
	void qtAboutSlot();
	void showSerialDlgSlot();
	void handShakeChangedSlot(bool s);
};											
#endif

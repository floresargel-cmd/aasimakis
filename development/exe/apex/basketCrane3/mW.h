#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Gb3.h"
#include "allPos.h"
#include "autoWdg.h"
#include "conW.h"
#include "crane.h"
#include "destW.h"
#include "expWdg.h"

#include "gView.h"
#include "hmiB.h"
#include "hmiGuiWidgetsLib.h"
#include "search.h"
#include "serv.h"
#include "settingsDlg.h"
#include "usageDlg.h"
#include "basketCrane3UserWdgt.h"
class mainWindowClass : public QMainWindow				
{												
	tuxipServerClass											*tuxipServer;
	allPosClass													  *allPos;
	craneClass														*crane;
	exportStationWidgetClass							*exportsToCrane2Wdgt;
	exportStationWidgetClass							*exportStationWidgetDestackerPosition1Wdgt;
	exportStationWidgetClass							*exportStationWidgetDestackerPosition2Wdgt;
	basketCrane3UserWdgtClass							*basketCrane3UserWdgt=nullptr;
	myQGraphicsViewClass									*layoutGraphicsView;
	autoWidgetClass												*autoWidget;
	conveyorsWidgetClass									*conveyorsWidget;
	destackerWidgetClass									*destackerWidget;
	QDialog																*conveyorsDlg;
	QDialog																*destackerDlg;
	hmiDlgBasicClass											*hmiDlgBasic;
	QAction																*showTuxServerAction;
	QPushButton														*returnDestacker1PosBasketBtn;
	QPushButton														*returnDestacker2PosBasketBtn;
	handShakeClass												*handShakeCrane3;
	handShakeClass												*handShakeCrane2;
	QToolBox															*toolBox;
	QRadioButton													*missionsRadioButton;
	QRadioButton													*modifyRadioButton;
	Q_OBJECT								
public:										
	mainWindowClass();
	~mainWindowClass();
	void plcCommsCheckStatus();
public slots:								
	void mainTimerTimeOutSlot();
	void updateGuiTimerTimeOutSlot();
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();						
	void showConVDlgActionTriggeredSlot();
	void showDestackerDlgActionTriggeredSlot();
	void qtAboutSlot();
	void stopSlot();
	void resetGraphicsViewZoom();
	void hmiDlgBasicButtonClickedSlot();
  void handShakeCrane3ChangedSlot();
	void handShakeCrane2ChangedSlot();

	void missionsRadioButtonClickedSlot(bool s);
	void modifyRadioButtonClickedSlot(bool s);
	void returnDestacker1PosBasketBtnClicked();
	void returnDestacker2PosBasketBtnClicked();
};											
#endif

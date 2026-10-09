#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GsE.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "../sbWarehouse/gView.h"
#include "../sbWarehouse/allPos.h"
#include "../sbWarehouse/expWdg.h"
#include "../sbWarehouse/search.h"
#include "../sbWarehouse/pincer2.h"
#include "../sbWarehouse/pincer2Conv.h"
#include "scaleD.h"
#include "newSbD.h"
class mainWindowClass : public QMainWindow
{
	tuxipServerClass											*tuxipServer;
	allPossClass													*allPos;
	pincer2Class													*pincer2;
	pincer2ConvPositionsClass							*pincer2Conv;

	myQGraphicsViewClass									*layoutGraphicsView;
	QGraphicsScene												*layoutScene;
	exportStationWidgetClass							*exportTriple1;
	exportStationWidgetClass							*exportTriple2;
	exportStationWidgetClass							*exportDouble;

	//
	QAction																*showTuxServerAction;
	QPushButton														*showPincer2DlgBtn;
	QPushButton														*showPincer2ConvDlgBtn;
	QDialog																*pincer2Dlg;
	QDialog																*pincer2ConvDlg;
	handShakeClass												*handShakeSbCrane;
	QLabel																*messageLabel;

	QVector<hmiPushButtonWidgetClass*>		pushButtonsVector;
	scaleDlgClass													*scaleDlg;
	newSbDlgClass													*newSbDlg;
	Q_OBJECT
public:
	mainWindowClass();
	~mainWindowClass();
	void setPlcComments();
public slots:
	void mainTimerSlot();
	void statisticsTimerSlot();
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();
	void showPincer2DlgSlot();
	void showPincer2ConvDlgSlot();
	void qtAboutSlot();
	void stopSlot();
	void resetGraphicsViewZoom();
	void handShakeSbCraneStateChangedSlot(bool s);

	void showScaleDlgTriggeredSlot();
	void showNewSbDlgTriggeredSlot();
};
#endif

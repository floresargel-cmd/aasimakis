#ifndef stEastW_H
#define stEastW_H
class stationEastWidgetClass;
#include "Gs.h"
#include "..\packetCrane\expW.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
class stationEastWidgetClass: public QWidget
{
	tuxipClass														*tuxip;
	QLabel																*stationCassetteLabel1;
	QLabel																*stationCassetteLabel2;
	QPushButton														*returnEmptyCassetteBtn1;
	QPushButton														*returnEmptyCassetteBtn2;
	QPushButton														*returnFullCassetteBtn1;
	QPushButton														*returnFullCassetteBtn2;
	exportsWidgetClass										*stationExportsList1;
	exportsWidgetClass										*stationExportsList2;
	QVector<hmiPushButtonWidgetClass*>		pushButtonsVector;
	int																		station1Cassette;
	int																		station2Cassette;
public:
public:
	Q_OBJECT
public:
	stationEastWidgetClass(tuxipClass	*tuxip_,QWidget *p);
	void updateGui();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
private slots:
	void updateGuiTimerSlot();
	void returnEmptyCassetteBtn1ClickedSlot();
	void returnEmptyCassetteBtn2ClickedSlot();
	void returnFullCassetteBtn1ClickedSlot();
	void returnFullCassetteBtn2ClickedSlot();
};
#endif

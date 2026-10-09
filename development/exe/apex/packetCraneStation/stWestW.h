#ifndef stWestW_H
#define stWestW_H
class stationWestWidgetClass;
#include "Gs.h"
#include "..\packetCrane\expW.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
class stationWestWidgetClass: public QWidget
{
	tuxipClass														*tuxip=nullptr;
	QLabel																*stationCassetteLabel=nullptr;
	QPushButton														*returnEmptyCassetteBtn=nullptr;
	QPushButton														*returnFullCassetteBtn=nullptr;
	exportsWidgetClass										*stationExportsList=nullptr;
	QVector<hmiPushButtonWidgetClass*>		pushButtonsVector;
	int																		stationCassette=0;
public:
public:
	Q_OBJECT
public:
	stationWestWidgetClass(tuxipClass	*tuxip_,QWidget *p);
	void updateGui();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
private slots:
	void updateGuiTimerSlot();
	void returnEmptyCassetteBtnClickedSlot();
	void returnFullCassetteBtnnClickedSlot();
};
#endif

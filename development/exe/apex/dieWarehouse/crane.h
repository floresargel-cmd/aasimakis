#ifndef CRANE_H
#define CRANE_H

#include "Gd.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "craneWdg.h"
#include "triplePos.h"
#include "wSt.h"
#include "crConv.h"
class craneClass;
class craneConveyorClass;
#include "allPos.h"
class craneClass : public QWidget
{
	QTimer											*missionsTimer;
	tuxipClass									*tuxip;
	QGraphicsScene							*layoutScene;
	allPossClass								*allPos;
	craneConveyorClass					*conveyor;
	dxfGroupOfGraphicsItemClass *topViewItem;
	dxfGroupOfGraphicsItemClass *sideViewUpItem;
//	dxfGroupOfGraphicsItemClass *sideViewDownItem;
	dxfGroupOfGraphicsItemClass *carriageUpItem;
//	dxfGroupOfGraphicsItemClass *carriageDownItem;
	dxfGroupOfGraphicsItemClass *carriageSideItem;
	dxfGroupOfGraphicsItemClass *hooksItem;
	dxfGroupOfGraphicsItemClass *convItem;
	craneWidgetClass						*craneWidget;
	//
	workStationClass						*workStation;
	//
	float												plcX;
	float												plcZ;
	float												plcYA;
	float												plcYB;
	int													autoMode;
	Q_OBJECT
public:
	enum {//autoMode
		autoExports										=0x00000001,
	};
public:
	craneClass(tuxipServerClass *tuxipServer,tuxipClass *tuxipConnection,allPossClass *allPos_,QGraphicsScene *layoutScene_,bool useTranslation,QWidget *p=NULL);
	~craneClass();
	QWidget* getCraneWidget();
	void setStations(workStationClass *workStation_);
	void loadFromDatabase();
	craneConveyorClass *getCraneConveyor();
	void checkForNextMissionInDatabase();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void sendManualMission(posClass *from,posClass *to);
	void addToAutoMode(int mode);
	void removeFromAutoMode(int mode);
public slots:
	void missionsTimerSlot();
};
#endif

#ifndef CRANE_H
#define CRANE_H

class craneClass;
#include "Gb3.h"
#include "allPos.h"
#include "carriage.h"
#include "craneWdg.h"

#include "gView.h"
#include "hmiGuiWidgetsLib.h"
#include "pos.h"
#include "serv.h"
class craneClass:public QWidget
{
	Q_OBJECT
public:
	enum autoModesEnum {
		none										=0x0000000000000,
		toFromCrane2						=0x0000000000001,
		toDestackerWorking1Pos	=0x0000000000002,
		toDestackerWorking2Pos	=0x0000000000004,
		returnEmpty							=0x0000000000008,
	};
private:
	QTimer											*missionsTimer;
	tuxipClass									*tuxip;
	QGraphicsScene							*layoutScene;
	allPosClass							 	  *allPos;
	carriageClass								*carriage;
	gItemClass									*craneXItem;
	gItemClass									*craneZItem;
	gItemClass									*destackerXItem;
	gItemClass									*destackerZItem;

	craneWidgetClass						*craneWidget;

	posClass										*toFromCrane2Pos;
	posClass										*destackerWorking1Pos;
	posClass										*destackerWorking2Pos;

	float												cranePlcX=0.f;
	float												cranePlcZ=0.f;
	float                       destackerPlcX=0.f;
	float                       destackerPlcZ=0.f;
	autoModesEnum	autoMode;
public:
	craneClass(tuxipServerClass *tuxipServer,tuxipClass *tuxipConnection,allPosClass *allPos_,QGraphicsScene *layoutScene_,QWidget *p=nullptr);
	QWidget* getCraneWidget() const;
	void setStations();
	void loadFromDatabase();
	carriageClass *getCarriage() const;
	void checkForNextMissionInDatabase();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void sendManualMission(posClass *from,posClass *to);
	void addToAutoMode(autoModesEnum mode);
	void removeFromAutoMode(autoModesEnum mode);
	bool canGetNewMission();
	void setDisplayMode(int m);
	bool uncoverBasketForExport(int basket);
public slots:
	void missionsTimerTimeoutSlot();
};
#endif

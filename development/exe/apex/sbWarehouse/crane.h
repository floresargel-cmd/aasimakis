#ifndef CRANE_H
#define CRANE_H
class craneClass;

#include "Gsw.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "triplePos.h"
#include "crConv.h"
#include "craneWdg.h"
#include "allPos.h"
class craneClass : public QWidget				
{
	QTimer											*missionsTimer;
	tuxipClass									*tux;
	tuxipClass									*tuxAutoCrane;
	QGraphicsScene							*layoutScene;
	allPossClass								*allPos;
	craneConveyorClass					*conveyor;
	gItemClass									*topViewItem;
	gItemClass									*sideViewUpItem;
	gItemClass									*sideViewDownItem;
	gItemClass									*carriageUpItem;
	gItemClass									*carriageDownItem;
	gItemClass									*carriageSideItem;
	gItemClass									*drawerItem;
	craneWidgetClass						*craneWidget;
	//
	float												plcX;
	float												plcZ;
	float												plcY;
	bool												isPlcConnected;
	Q_OBJECT								
public:
	enum autoModeEnum
	{
		none													=0x00000000,
		autoWestImports								=0x00000001,
		autoWestExportsEmptySb				=0x00000002,
		autoWestExportsFullSb					=0x00000004,
		autoWestExportsEmptyCassettes	=0x00000008,

		tripleImports									=0x00000010,
		tripleExportsWest							=0x00000020,
		tripleExportsEast							=0x00000040,

		doubleExportImports						=0x00000100,
		doubleExportEmptyCassettes		=0x00000200,
		doubleExportFullSB						=0x00000400,
	};
	autoModeEnum								autoMode;
public:										
	craneClass(tuxipClass *tux_,tuxipClass *tuxAutoCrane_,allPossClass *allPos_,QGraphicsScene *layoutScene_,bool useTranslation,QWidget *p);
	~craneClass();
	QWidget* getCraneWidget();
	void startMissions();
	void loadFromDatabase();
	craneConveyorClass *getCraneConveyor();
	void checkForNextMissionInDatabase();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void sendManualMission(posClass *from,posClass *to);
	void addToAutoMode(int mode);
	void removeFromAutoMode(int mode);
	void setIsPlcConnected(bool v);
	void startMission(posClass *from,posClass *to);
	bool getHasNextMission();

	bool getImportsFromWest();
	bool sendEmptySbToWest();
	bool sendExportsToWest();
	bool sendEmptyCassettesToWest();

	bool getImportsFromTriple();
	bool sendExportsToTripleWest();
	bool sendExportsToTripleEast();

	bool getImportsFromDouble();
	bool sendExportsToDouble();
	bool sendEmptyToDouble();
public slots:								
	void missionsTimerSlot();
};											
#endif

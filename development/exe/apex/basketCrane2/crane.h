#ifndef CRANE_H
#define CRANE_H

class craneClass;
#include "Gb2.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "carriage.h"
#include "craneWdg.h"
#include "allPos.h"
class craneClass : public QWidget
{
	QTimer															*missionsTimer;
	tuxipClass													*tuxip;
	QGraphicsScene											*layoutScene;
	allPossClass												*allPos;
	carriageClass												*carriage;
	gItemClass													*craneXItem;
	gItemClass													*craneYItem;
	craneWidgetClass										*craneWidget;
	posClass														*toRotatingNorth;
	posClass														*toRotatingSouth;
	posClass														*rotatingNorth;
	posClass														*exit1;
	posClass														*exit2;
	posClass														*destackerBuffer;
	posClass														*destacker;
	QVector<columnClass>								columnsWithBasketsForExportV;
	//
	//
	float												plcX;
	float												plcY;
	float												plcZ;
	int													autoMode;
	Q_OBJECT
public:
	enum {
		autoNone											=0x00000000,
		autoExportsToDestaker					=0x00000001,//sb warehouse area
		autoExportsToAnodizing				=0x00000002,//send basket out
		autoImportsFromNorth					=0x00000004,
		autoImportsFromSouth					=0x00000008,
		autoExportsToPackingDestaker1	=0x00000020,//packing area
		autoExportsToPackingDestaker2	=0x00000040,//packing area
		autoExportsUseEpics						=0x00000080,//packing area
	};
public:
	craneClass(tuxipServerClass *tuxipServer,tuxipClass *tuxipConnection,allPossClass *allPos_,QGraphicsScene *layoutScene_,QWidget *p=NULL);
	~craneClass();
	QWidget* getCraneWidget();
	void setStations();
	void loadFromDatabase();
	carriageClass *getCarriage();
	void checkForNextMissionInDatabase();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void sendManualMission(posClass *from,posClass *to);
	void addToAutoMode(int mode);
	void removeFromAutoMode(int mode);
	bool canGetNewMission();
	void setDisplayMode(int m);
	bool uncoverBasketForExport(int basket);
	posClass *getFreePosForBasketInPos(posClass *p);
	void addBasketForPackingSb(posClass *pos);
	void addBasketForPackingDestacker(posClass *pos,int destackerPos,QString crane2ExportsTable,QString crane3ExportsTable);
	void addExportsFromEpicsData();
public slots:
	void missionsTimerSlot();
};
#endif

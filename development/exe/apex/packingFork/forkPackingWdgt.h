#ifndef forkPackingWdgt_h
#define forkPackingWdgt_h

class forkPackingWdgtClass;
#include "Gf.h"
#include "allPosPackingFork.h"
#include "forkPacking.h"
#include "gView.h"
#include "hmiGuiWidgetsLib.h"
#include "serv.h"
#include "hmiDlgForkPacking.h"

class forkPackingWdgtClass:public QWidget				
{
	tuxipClass										*craneTuxip=nullptr;
	tuxipClass										*forkTuxip=nullptr;
	allPosPackingForkClass				*allPos=nullptr;
	hmiDlgForkPackingClass				*hmiDlg=nullptr;
	forkPackingClass							*fork=nullptr;
	myQGraphicsViewClass					*layoutGraphicsView=nullptr;
	QGraphicsScene								*layoutScene=nullptr;
	Q_OBJECT
public:										
	forkPackingWdgtClass(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,QWidget *p=nullptr);
	allPosPackingForkClass* getAllPosPtr();
	forkPackingClass* getForkPtr();
	void initializeChildren(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_);
	void showHmiDlg();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	bool getIsForkIdle();
	bool getIsAboutToBeIdle();
	bool sendMissionToFork(int from,int to);
	void returnEmptyCassetteFromNorthConveyor();
	void returnFullCassetteFromNorthConveyor();
public slots:								
	void hmiBtnClickedSlot();
	void zoomExtentsBtnClickedSlot();
	void returnEmptyCassetteFromNorthConveyorSlot();
};
#endif

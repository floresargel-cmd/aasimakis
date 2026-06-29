#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>

#include "cG.h"
#include "oW.h"
#include "plcW.h"
#ifdef EMULATE
#include "emulate.mW.h"
#endif
class mainWindowClass : public QWidget				
{

	QTimer								mainTimer;
	openGLWidgetClass			*openGLWidget;
	//QPushButton						*takeScrapBasketBtn;
	plcWidgetClass				*plcWidget;
	//
	QCheckBox							*useEmptyBasketsPosCheckBox;
	QCheckBox							*emptyEmptyBasketsPosCheckBox;
#ifdef EMULATE
	QTimer								sendBasketsToOvenTimer;
	plcEmulateWindowClass	*plcEmulateWindow;
#endif
   Q_OBJECT								
public:										
  mainWindowClass();
	~mainWindowClass();
	void initializeDatabase();
	void demoFillTables(bool force);
	void clearLogTable();
public slots:								
	void onMainTimer();
	void beforeStackerAutoModeSlot(int checkState);
	void afterStackerAutoModeSlot(int checkState);
	//////////////////////////////////////////////////////
	// Added by Vahid
	//////////////////////////////////////////////////////
	void Oven1EnableAutoModeSlot(int checkState);
	void Oven2EnableAutoModeSlot(int checkState);
	void Oven1Not24AutoModeSlot(int checkState);
	void Oven2Not24AutoModeSlot(int checkState);
	void nextOven2Cycle230RadioButtonSlot(bool state);
	void nextOven2Cycle185RadioButtonSlot(bool state);
	//////////////////////////////////////////////////////
	// End of Code Added by Vahid
	//////////////////////////////////////////////////////
	void unloadOvenAutoModeSlot(int checkState);
	void exportsAutoModeSlot(int checkState);
	void returningEmptyAutoModeSlot(int checkState);
	void scrapAutoModeSlot(int checkState);
	void fromBufferToOvenAutoModeSlot(int checkState);
	void exportsBeforeImportsCheckBtnSlot(int checkState);
	void rearrangeBasketsCheckBtnSlot(int checkState);
	void numberOfEmptyPosInBufferSpinBoxSlot(int s);

	void nextOvenCycle230RadioButtonSlot(bool state);
	void nextOvenCycle185RadioButtonSlot(bool state);
	
	void takeScrapBasketBtnSlot();
	void unlockBasketAfterStackerBtnSlot();
	void unlockBasketReturningFromOvenBtnSlot();
	void takeBasketsFromStackerOutSlot(int checkState);
	void scrapBasketPositionChangedSlot(int basketTableId);
	void useEmptyBasketsPosCheckBoxSlot(int checkState);
	void emptyEmptyBasketsPosCheckBoxSlot(int checkState);

	void signalFromCraneTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void signalFromConveyorsTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();
//for backwards compatibility
	void signalFromCranePlcSlot(QString table,int index,QString value);
	void signalFromConveyorsPlcSlot(QString ip,QString table,int index,QString value);
//for backwards compatibility

	void plcDlgSlot();
	void qtAboutSlot();
};											
#endif



#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QMap>
#include <QSet>

#include "cG.h"
#include "oW.h"
#include "plcW.h"

#ifdef EMULATE
#include "emulate.mW.h"
#endif

class mainWindowClass : public QWidget				
{
	QTimer						mainTimer;
	QTimer						basketDataRefreshTimer;
	openGLWidgetClass			*openGLWidget;
	//QPushButton				*takeScrapBasketBtn;
	plcWidgetClass				*plcWidget;
	QWidget						*plcStatusWidget;
	int							plcStatusPreferredY;

	QCheckBox					*useEmptyBasketsPosCheckBox;
	QCheckBox					*emptyEmptyBasketsPosCheckBox;

	QMap<QString, QLabel*>		dbTableStatusLabels;
	QMap<QString, QLabel*>		plcNetworkStatusLabels;
	QLabel						*mainTimerStatusLabel;
	QLabel						*autoModeTimerStatusLabel;
	QLabel* dbAreaLabel;
	QLabel* plcAreaLabel;
	QLabel* sysAreaLabel;

	// 250ms delayed fade-to-grey: when Idle is requested we start a timer; when it fires we set grey. When Active is requested we stop the timer and set active color.
	QTimer						*mainTimerIdleTimer;
	QTimer						*autoModeTimerIdleTimer;
	QMap<QString, QTimer*>		dbTableIdleTimers;
	QMap<QString, QTimer*>		plcNetworkIdleTimers;
	QSet<QString>				plcNetworkInError;
	QSet<QString>				plcNetworkConnecting;

#ifdef EMULATE
	QTimer						sendBasketsToOvenTimer;
	plcEmulateWindowClass		*plcEmulateWindow;
#endif

	Q_OBJECT								

public:										
	mainWindowClass();
	~mainWindowClass();

	void initializeDatabase();
	void demoFillTables(bool force);
	void clearLogTable();

	void setDbTableActive(const QString& tableName, bool isRead);
	void setDbTableIdle(const QString& tableName);

	void setPlcNetworkActive(const QString& networkName, bool isRead);
	void setPlcNetworkIdle(const QString& networkName);
	void setPlcNetworkError(const QString& networkName);
	void setPlcNetworkConnecting(const QString& networkName);

	void setMainTimerFiring();
	void setMainTimerIdle();
	void setAutoModeTimerFiring();
	void setAutoModeTimerIdle();

public slots:								
	void onMainTimer();
	void beforeStackerAutoModeSlot(int checkState);
	void afterStackerAutoModeSlot(int checkState);
	void beforeStacker2AutoModeSlot(int checkState);
	void afterStacker2AutoModeSlot(int checkState);

	void pressToOvenCheckBtnSlot(int checkState);

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
	void unloadOven2AutoModeSlot(int checkState);
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

	void signalFromCraneTuxPlcSlot(QString ip, QString table, int index, QVariant value);
	void signalFromConveyorsTuxPlcSlot(QString ip, QString table, int index, QVariant value);
	void showServerSlot();
	void showBasketTrackerSlot();

//for backwards compatibility
	void signalFromCranePlcSlot(QString table, int index, QString value);
	void signalFromConveyorsPlcSlot(QString ip, QString table, int index, QString value);

	// @@@@@ -- AB, try to wire up some error hanlding slots for detecting when a PLC network goes idle or has an error. 
	// These are called from plcWidget when it detects the condition, and we can use them to update the UI (e.g. show red/grey status labels) and also to log the event.
	void plcHealth();
	
	//for backwards compatibility

	void plcDlgSlot();
	void qtAboutSlot();

protected:
	void resizeEvent(QResizeEvent *event) override;

private slots:
	void onMainTimerIdleTimeout();
	void onAutoModeTimerIdleTimeout();
	void refreshBasketDataDisplay();

private:
	void setDbTableIdleNow(const QString& tableName);
	void setPlcNetworkIdleNow(const QString& networkName);
	void positionPlcStatusWidget();
};
#endif



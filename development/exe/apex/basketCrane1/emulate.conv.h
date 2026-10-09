#ifndef EMULATECONV_H
#define EMULATECONV_H

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QDialog>
#include <QtNetwork>
#include "emulate.G.h"
#include "../common/tcpTux.h"

class conveyorsEmulatorClass : public QWidget
{
	QTimer									mainTimer;
	QTimer									propagateBasketsTimer;
	QTime										time;
  tuxipClass							*conveyorTuxip;
	//
	bool										ovenEntranseReady;
	QTimer									stayInOvenTimer;
	QSpinBox								*stayInOvenTimeSpinBox;
	QPushButton							*ovenReadyPushButton;
	Q_OBJECT
public:
   conveyorsEmulatorClass(tuxipClass *conveyorTuxip_);
private:
	void takeBasketsInToOven();
	void takeBasketsOutFromOven();
	bool hasBasketsInOven();
public:
	void plcDataChanged(QString table,int index,QString value);
	void tuxIsReady();
private slots:
  void mainTimerSlot();
 	void onStayInOvenTimerSpinBoxSlot(int t);
 	void onOvenReadySlot();
 	void propagateBasketsTimerSlot();
};
#endif

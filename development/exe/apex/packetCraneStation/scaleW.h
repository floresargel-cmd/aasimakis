#ifndef SCALEW_H
#define SCALEW_H

#include <QApplication>
#include <QtGui>
#include <QMainWindow>
#include <QToolBar>
#include <QLabel>
#include <QSerialPort>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QDoubleSpinBox>

#include "Gs.h"
#include "serv.h"
class scaleDlgClass : public QDialog
{
	tuxipClass									*tuxip;
	QSerialPort									*serialPort;
	QTableWidget								*logTable;
	QAction											*resetScaleAction;
	QTimer											*readTimer;
	QString											serialData;
	float												currentWeight;
	bool												station1NoSbReadyToReturn;
	bool												station1WithSbReadyToReturn;
	bool												station2NoSbReadyToReturn;
	bool												station2WithSbReadyToReturn;
	QTimer											*checkScaleIsOnTimer;
	bool												scaleIsOn;
	QAction											*myAction;
	Q_OBJECT
public:
	scaleDlgClass(tuxipClass *tuxip_,QWidget *p);
	~scaleDlgClass();
	void log(QString str);
	float getCurrentWeight();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setMyAction(QAction *myAction_);
	bool getScaleIsOn();
public:
signals:
	void currentCassetteChangedSignal();
	void currentWeightChangedSignal();
public slots:
	void mainTimerSlot();
	void checkScaleZeroTimerSlot();
	void getWeightTimerSlot();
	void resetScaleActionTriggeredSlot();
	void readyReadSerialPortSlot();
	void readTimerSlot();
	void checkScaleIsOnTimerSlot();
	void errorOccurredSlot(QSerialPort::SerialPortError error);
};
#endif

#ifndef SCALEW_H
#define SCALEW_H

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
	bool												station1ReadyToReturn;
	bool												station2ReadyToReturn;
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
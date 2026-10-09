#ifndef PLCW_H
#define PLCW_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>

#include "cG.h"

class plcWidgetClass;

#include "plcDlg.h"
#include "pass.h"

class plcWidgetClass : public QWidget
{
	plcDialogClass				*plcDialog;

	tcpServerClass				*cranePlcTcpServer;
	tcpServerClass				*conveyorsPlcTcpServer;
	tcpServerClass				*stacker2PlcTcpServer;

	QPushButton					*showPlcManualsBtn;
	QLabel						*fromPosLabel;
	QLabel						*stopLabel;
	QLabel						*toPosLabel;
	QLabel						*status0Label;
	QLabel						*status1Label;
	QLabel						*status2Label;
	QLabel						*error0Label;
	QLabel						*error1Label;
	QLabel						*makeResetLabel;
	QLabel						*laserEncoderAlarmLabel;
	QLabel						*overCraneSafetySensorAlarmLabel;

	Q_OBJECT

public:
	plcWidgetClass(QWidget *p);
	~plcWidgetClass();

	void setFromPosLabelText(QString str);
	void setToPosLabelText(QString str);
	void setStopLabelText(QString str,QString style);
	void setStatus0LabelText(QString str,QString style);
	void setStatus1LabelText(QString str,QString style);
	void setStatus2LabelText(QString str, QString style);
	void setError0LabelText(QString str, QString style);
	void setError1LabelText(QString str,QString style);
	void setMakeResetLabelText(QString str,QString style);
	void setLaserEncoderError(bool active);
	void setOverCraneSafetySensorError(bool active);

#ifdef TUX_PLC_COMMS
	tuxipClass *getCraneTuxClass();
	tuxipClass *getConveyorTuxClass();
	tuxipClass *getNewOvenTuxClass();

	void addTables();
	void showTuxServerCommsDialog();
#endif

	tcpServerClass* getCranePlcTcpServer();
	tcpServerClass* getConveyorsPlcTcpServer();
	void startExternalSoft();
	void signalFromCranePlc(QString table,int index,QString value);
	void signalFromConveyorsPlc(QString table,int index,QString value);

public slots:
	void onShowPlcManualsBtnSlot();
};
#endif

#ifndef EXP_H
#define EXP_H

#include <QtGui>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QList.h>

#include "eG.h"

#include "cont.h"
#include "basList.h"
#include "pass.h"
class exportStationClass:public QWidget
{
	QTimer												flashTimer;
	tcpServerClass								*tcpServer;
	bool													flashBit;
	QString												curentUser;
	basketContentsClass						*basketContents;
	basketListClass								*basketList;
	//
	QLabel												*stationNameLabel;
#ifdef PASS_ACTIVE
	QLabel												*userNameLabel;
	QPushButton 									*loginPushButton;
	QPushButton 									*logoutPushButton;
#endif
	QLabel												*basketLabel;
	QLabel												*nextBasketLabel;
	QLabel												*alarmLabel;
	bool													alarmIsActive;
public:
	int														stationTypeEnum;
public:
	Q_OBJECT
public:
	exportStationClass(int stationTypeEnum_,tcpServerClass *tcpServer_,QWidget *w);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void lockGui();
	void unLockGui();
	void setAlarm(QString str);
	void plcValueChanged(int index,int value);
public slots:
	void basketChangedSlot();
	void onFlashTimer();
#ifdef PASS_ACTIVE
	void loginSlot();
	void logoutSlot();
#endif
};
#endif

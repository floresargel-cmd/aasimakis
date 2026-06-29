#ifndef CONT_H
#define CONT_H

#include <QtGui>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTableWidget>
#include <QHeaderView>
#include <QToolButton>
#include <QList.h>
#include "eG.h"
#include "pass.h"
class basketContentsClass:public QWidget
{
	QTimer							mainTimer;
	tcpServerClass			*tcpServer;
	int									stationId;
	int									basketTableId;
	QToolButton 				*returnBasketPushButton;
	//
	QTableWidget				*tableView;
	//
	QString							curentUser;
	//
public:
	Q_OBJECT
public:
	basketContentsClass(int stationId_,tcpServerClass *tcpServer_,QWidget *w);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef PASS_ACTIVE
	void lockGui();
	void unLockGui(QString curentUser_);
#endif
	void updateData(int basTabbleId);
	void plcStatusChanged(int index,int value);
	void updateTable();
signals:
	void basketChangedSignal();
public slots:
	void onMainTimer();
	void returnBasketPushButtonSlot();
	//void buttonReleasedSlot();
};
#endif

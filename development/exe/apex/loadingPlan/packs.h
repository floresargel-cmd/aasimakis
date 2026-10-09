#ifndef PACKS_H
#define PACKS_H

#include <QtGui>
#include <QWidget>
#include <QTabWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTimer>
#include <QDateTime>
#include <QTime>
#include <QStandardItem>
#include <QTableView>

#include "G.h"
#include "pack.h"
#include "packW.h"
#include "loadPlan.h"

class packetsClass : public QWidget
{
	QTimer											mainTimer;
	QTableView									*packetsTableView;
	QStandardItemModel					*packetsModel;
	loadingPlanClass						*loadingPlan;
	//
	QVector<packetWidgetClass*>	packetsVector;
	//
	int													activeLoading;
	QLabel											*totalWeightLabelLabel;
	QLabel											*totalWeightLabel;
	//
    Q_OBJECT
public:
    packetsClass(QWidget *p);
	int getNoOfPackets(){return packetsVector.size();}
	packetWidgetClass* getPacket(int i){if ((i>=0)&&(i<packetsVector.size()))return packetsVector[i];else return NULL;}
	void setLoadingPlan(loadingPlanClass *lp);
	int getActiveLoadingId(){return activeLoading;}
protected:
    void initialize();
	void updateVisual();
private slots:
    void onMainTimer();
    void tableChanged(QStandardItem* item);
private:
};
#endif

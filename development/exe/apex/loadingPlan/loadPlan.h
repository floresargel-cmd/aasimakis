
#ifndef LOADPLAN_H
#define LOADPLAN_H
class loadingPlanClass;
class packetsClass;
class optionsClass;
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
#include <QPrinter>
#include <QHBoxLayout>
#include <QSplitter>
#include <QInputDialog>
#include <QPushButton>

#include "G.h"
#include "packs.h"
#include "packCol.h"
#include "options.h"
#include "groupOfCol.h"
#include "optim.h"

class loadingPlanClass : public QWidget
{
	QTimer								mainTimer;
	//
	packetsClass					*packets;
	optionsClass					*options;
	groupOfColumnsClass		*initialOrders;
	groupOfColumnsClass		*forGroupOrders;
	groupOfColumnsClass		*aftGroupOrders;
	optimizerClass				*optimizer;
	//
    Q_OBJECT
public:
    loadingPlanClass(packetsClass *packets_,optionsClass *options_,QWidget *p);
	//
	void calculate();
	groupOfColumnsClass *getForGroupOfOrders();
	groupOfColumnsClass *getAftGroupOfOrders();
	packetsClass *getAllPackets(){return packets;};
	//optim
	void loadPacket(int index,int col);
	void clearPackets();
	int getActiveLoadingId();
	void exportLoading();
	void printLabels();
	void exportToPdf();
	void startManualLoading();
	void clearManualExportOrder();
private slots:
    void onMainTimer();
private:
};
#endif

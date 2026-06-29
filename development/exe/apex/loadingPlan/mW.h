
#ifndef MAINW_H
#define MAINW_H

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
#include <QVBoxLayout>
#include <QAction>

#include "G.h"
//#include "plcDataLib.h"
#include "options.h"
#include "packs.h"
#include "loadPlan.h"
class mainWindow : public QWidget
{
	//plcDataStructureClass	*loadingCranePlcData;
	QTimer								mainTimer;
	packetsClass					*packets;
	optionsClass					*options;
	loadingPlanClass			*loadingPlan;
	optimizerClass				*optimizer;
    Q_OBJECT
public:
	mainWindow();
protected:
	void initializeDatabase();
private slots:
	void onMainTimer();
	void qtAboutSlot();
};
#endif


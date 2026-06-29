
#ifndef OPTIONS_H
#define OPTIONS_H

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

#include "G.h"
#include "packs.h"
#include "groupOfCol.h"
class optionsClass : public QWidget
{
	QTimer					mainTimer;
	//
	QLineEdit				*maxHeightLineEdit;
	QLineEdit				*maxWeightForLineEdit;
	QLineEdit				*maxWeightAftLineEdit;
	//
	groupOfColumnsClass		*forGroupOrders;
	groupOfColumnsClass		*aftGroupOrders;
	//
    Q_OBJECT
public:
    optionsClass();
	//
	int getMaxAvailableHeight();
	int getMaxWeightFor();
	int getMaxWeightAft();
	//
	void setForGroupOfOrders(groupOfColumnsClass *g);
	void setAftGroupOfOrders(groupOfColumnsClass *g);
private slots:
    void maxHeightLineEditChanged();
    void maxWeightForLineEditChanged();
    void maxWeightAftLineEditChanged();
    void onMainTimer();
private:
};
#endif


#ifndef OPTIM_H
#define OPTIM_H

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
#include <QProgressBar>

class optimizerClass;
#include "G.h"
#include "packs.h"
#include "loadPlan.h"
#include "groupOfCol.h"

//#include <qwt_scale_map.h>
//#include <qwt_plot_curve.h>
//#include <qwt_symbol.h>
//#include <qwt_plot.h>
//#include <qwt_painter.h>
//#include <qwt_plot_canvas.h>
//#include <qwt_legend.h>
//#include <qwt_scale_widget.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////plotClass///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class plotClass : public QwtPlot
//{
//	QwtPlotCurve			*progressCurve;
//    Q_OBJECT
//public:
//    plotClass(QWidget *parent);
//	void calculateFitCurve(int gen,double obj);
//private:
//    void alignScales();
//};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////optimColumn/////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class optimColumn
{
	int maxHeight;
	int id;
	//
	double xPos;
	QVector<int>			ordersVector;
	QVector<int>			ordersHeightVector;
	QVector<packetClass*>	packetsVector;
	//
	double yCenter;
	double yMoment;
	double totalWeight;
	int height;
public:
	optimColumn(){xPos=id=yCenter=totalWeight=yMoment=0.;maxHeight=0;}
	optimColumn(int id_,double maxHeight_);
	void clearPackets();
	void appendPacket(packetClass* p);
	void optimizePackets();
	void calculate();
	double calculateObjective();
	void definePacketPos(QVector<int> *packetsPosArray);
	double getYmoment(){return yMoment;}
	double getXmoment(){return totalWeight*xPos;}
	bool hasOrder(int ord);
	int getNoOfPacketsWithOrder(int ord);
	int getHeightOfOrder(int ord);
	int getTotalWight(){return totalWeight;}
	int getPacketsNo(){return packetsVector.count();}
	packetClass* getPacket(int i){return packetsVector[i];}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////optimizerProgressClass//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class optimizerProgressClass : public QDialog
{
    Q_OBJECT
	QProgressBar			*progressBar;
	QProgressBar			*targetBar;
public:
	optimizerProgressClass();
	void setMaxProgress(int m);
	int getMax(){return progressBar->maximum();}
	void setProgress(int p,int obj);
private slots:
	void accept();
	void reject();
signals:
    void done();
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////optimizerClass//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class optimizerClass : public QWidget
{
    Q_OBJECT
	//plotClass				*plot;
	QTimer					mainTimer;
	optimizerProgressClass	*optimizerProgress;
	QPushButton				*startOptimPushButton;
	QPushButton				*exportLoadingPushButton;
	QPushButton				*printPushButton;
	QPushButton				*printLabelsPushButton;
	QPushButton				*startLoadingPushButton;
	QPushButton				*clearManualExportOrderPushButton;
	QLabel					*generationsLabel;
	QLabel					*ojectiveLabel;

	QLabel					*colObjLabel;
	QLabel					*xObjLabel;
	QLabel					*yObjLabel;
	QLabel					*firstOrderObjLabel;
	QLabel					*orderInOneGroupObjLabel;
	QLabel					*piramidObjLabel;
	QLabel					*dWObjLabel;

	//
	loadingPlanClass		*loadingPlan;
	//
	QVector<optimColumn>	packetColumns;
	//
	bool					stopGaul;
//
public:
    optimizerClass(loadingPlanClass *loadingPlan_,QWidget *p);
	//
	double calculateObjective(int* packets,bool updateQui);
	void newSolution(QVector<int> packets,int generation,double bestEver,bool forceUpdate);
	void newGeneration(int generation,double bestEver);
	//
	void setMaxProgress(int m);
	void setProgress(int p,int obj);
	bool continueGaul();
private slots:
	void onStartOptimPushButtonClicked();
	void onExportLoadingPushButtonClicked();
	void onExportToPrintPushButtonClicked();
	//void onPrintLabelsPushButtonClicked();
	//void onStartLoadingPushButtonClicked();
	void onDoneFromProgressBar();
	void onClearExportOrderPushButtonClicked();
	void onMainTimer();
private:
};
#endif

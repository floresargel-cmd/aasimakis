#include "pch.h"
#include "usageDlg.h"
usageDialogClass::usageDialogClass(QWidget *p):QDialog(p)
{
	resize(1600,600);
	return;
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(2);setLayout(mainLayout);
	QHBoxLayout *menuLayout=new QHBoxLayout();mainLayout->addLayout(menuLayout);
	QHBoxLayout *dataLayout=new QHBoxLayout();mainLayout->addLayout(dataLayout);
	//
	menuLayout->addWidget(new QLabel(tr("Calculation interval [hours]:")));
	calculationIntervalSpinBox=new QSpinBox();
	calculationIntervalSpinBox->setMinimum(1);
	calculationIntervalSpinBox->setMaximum(INT_MAX);
	calculationIntervalSpinBox->setSingleStep(1);
	calculationIntervalSpinBox->setValue(2);
	menuLayout->addWidget(calculationIntervalSpinBox);
	menuLayout->addWidget(new QLabel(tr("Empty cells:")));
	menuLayout->addWidget(new QLabel(getOne(QString("select count(tid) from pos where basket=0 and locked=0"),db).toString()));
	menuLayout->addWidget(new QLabel(tr("Full cells:")));
	menuLayout->addWidget(new QLabel(getOne(QString("select count(tid) from pos where basket<>0"),db).toString()));
	menuLayout->addStretch(1);
	connect(calculationIntervalSpinBox,SIGNAL(valueChanged(int)),this,SLOT(calculationIntervalSpinBoxValueChangedSlot(int)));
	//
	plot=new QCustomPlot(this);
//	plot->setMinimumWidth(800);
	dataLayout->addWidget(plot);
	plot->yAxis->setRange(0,100);
  plot->yAxis->setBasePen(QPen(Qt::white, 1));
  plot->yAxis->setTickPen(QPen(Qt::white, 1));
  plot->yAxis->setSubTickPen(QPen(Qt::white, 1));
  plot->yAxis->setTickLabelColor(Qt::white);
  plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  plot->yAxis->grid()->setSubGridVisible(true);
  plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
	plot->yAxis ->grid()->setZeroLinePen(Qt::NoPen);
  plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
	plot->yAxis->setLabel("% usage");

	plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  plot->xAxis->setBasePen(QPen(Qt::white, 1));
 	plot->xAxis->setTickPen(QPen(Qt::white, 1));
  plot->xAxis->setSubTickPen(QPen(Qt::white, 1));
  plot->xAxis->setTickLabelColor(Qt::white);
  plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  plot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  plot->xAxis->grid()->setSubGridVisible(true);
	plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
	//plot->xAxis->setDateTimeFormat("hh:mm:ss\nyyyy/MMMM/dd");
	plot->xAxis->setLabel("date");

 	plot->xAxis->setAutoTicks(false);
	plot->xAxis->setAutoTickLabels(false);
	plot->xAxis->setTickLabelRotation(90);
	plot->xAxis->setSubTickCount(24);
	plot->xAxis->setTickLength(0, 4);
	plot->xAxis->grid()->setVisible(true);


  QLinearGradient plotGradient;
  plotGradient.setStart(0, 0);
  plotGradient.setFinalStop(0, 350);
  plotGradient.setColorAt(0, QColor(150, 150, 150	));
  plotGradient.setColorAt(1, QColor(50, 50, 50));
  plot->setBackground(plotGradient);
  QLinearGradient axisRectGradient;
  axisRectGradient.setStart(0, 0);
  axisRectGradient.setFinalStop(0, 350);
  axisRectGradient.setColorAt(0, QColor(80, 80, 80));
  axisRectGradient.setColorAt(1, QColor(30, 30, 30));
  plot->axisRect()->setBackground(axisRectGradient);

	plot->legend->setVisible(true);
	plot->legend->setBrush(plotGradient);
	totalUsage=plot->addGraph();
	totalUsage->setName(tr("Total"));
	totalUsage->setPen(QPen(QColor(200,200,250)));
	totalUsage->setBrush(QBrush(QColor(200,200,250,50)));

	pickingUsage=plot->addGraph();
	pickingUsage->setName(tr("Picking"));
	pickingUsage->setPen(QPen(QColor(200,250,200)));
	pickingUsage->setBrush(QBrush(QColor(200,250,200,100)));

	stationI2Usage=plot->addGraph();
	stationI2Usage->setName(tr("I2"));
	stationI2Usage->setPen(QPen(QColor(250,200,200)));
	stationI2Usage->setBrush(QBrush(QColor(250,200,200,100)));
	
	stationE2Usage=plot->addGraph();
	stationE2Usage->setName(tr("E2"));
	stationE2Usage->setPen(QPen(QColor(250,250,200)));
	stationE2Usage->setBrush(QBrush(QColor(250,250,200,100)));

	packetsUsage=plot->addGraph();
	packetsUsage->setName(tr("Packets"));
	packetsUsage->setPen(QPen(QColor(250,150,150)));
	packetsUsage->setBrush(QBrush(QColor(250,150,150,100),Qt::SolidPattern));
	//
	importsExportsTable=new QTableWidget();dataLayout->addWidget(importsExportsTable);
	importsExportsTable->setAlternatingRowColors(true);
	importsExportsTable->horizontalHeader()->setStretchLastSection(true);
	importsExportsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	importsExportsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	importsExportsTable->setColumnCount(5);
	importsExportsTable->setHorizontalHeaderLabels(QStringList()<<tr("Day")<<tr("Total")<<tr("Imports")<<tr("Exports")<<tr("Date"));
	importsExportsTable->setWordWrap(true);
	importsExportsTable->setMaximumWidth(700);
	update();
} 
usageDialogClass::~usageDialogClass()
{  
}
void usageDialogClass::calculationIntervalSpinBoxValueChangedSlot(int interval)
{
	update();
}
void usageDialogClass::fillGraph(QCPGraph *g,QVector<QVector<QVariant>> dataVV)
{
	double total=0;
	g->clearData();
	if (dataVV.count()==0)
		return;
	QVector<missionClass> missionsVector;
	for (int i=0;i<dataVV.count();i++)
		missionsVector<<missionClass(dataVV[i][0].toInt(),dataVV[i][1].toInt(),dataVV[i][2].toInt(),dataVV[i][3].toInt());
	int dt=calculationIntervalSpinBox->value()*3600;
	if (g==totalUsage)
	{
		minT=missionsVector[0].start;
		maxT=QDateTime::currentDateTime().toTime_t();
	}
	int from=maxT;
	int to=from-dt;
	if (to<minT)
		to=minT-1;
	QVector<double> dateV;
	QVector<double> usageV;
	while (from>minT)
	{
		int usedTime=0;
		for (int i=0;i<missionsVector.count();i++)
			usedTime+=missionsVector[i].getUsage(from,to);
		double v=100.*((double)usedTime/(double)(from-to));
		dateV<<from<<to;
		if (v>100.)
			v=100.;
		usageV<<v<<v;
		from=to;
		to=from-dt;
		total+=usedTime;
	}
	g->setData(dateV,usageV);
	g->setName(g->name().split(' ')[0]+QString(" %1%").arg(100.*total/(maxT-minT),0,'f',2));
}
void usageDialogClass::update()
{
//	int getLogSecOld=(QDateTime::currentMSecsSinceEpoch()/1000)-(10*24*3600);
//	maxT=minT=0;
//	fillGraph(totalUsage,execTableQuery(QString("select fromPos,toPos,startSec,endSec from missionsTimeLog where endSec<>0 and startSec>%2 order by tid").arg(getLogSecOld),db));
//	fillGraph(pickingUsage,execTableQuery(QString("select fromPos,toPos,startSec,endSec from missionsTimeLog where endSec<>0 and startSec>%3 and (fromPos=%1 or fromPos=%2 or toPos=%1 or toPos=%2) order by tid").arg(picking1PlcId).arg(exportE1PlcId).arg(getLogSecOld),db));
//	fillGraph(stationI2Usage,execTableQuery(QString("select fromPos,toPos,startSec,endSec from missionsTimeLog where endSec<>0 and startSec>%2 and (fromPos=%1 or toPos=%1) order by tid").arg(importI2PlcId).arg(getLogSecOld),db));
//	fillGraph(stationE2Usage,execTableQuery(QString("select fromPos,toPos,startSec,endSec from missionsTimeLog where endSec<>0 and startSec>%2 and (fromPos=%1 or toPos=%1) order by tid").arg(exportE2PlcId).arg(getLogSecOld),db));
//	fillGraph(packetsUsage,execTableQuery(QString("select fromPos,toPos,startSec,endSec from missionsTimeLog where endSec<>0 and startSec>%3 and (fromPos=%1 or fromPos=%2 or toPos=%1 or toPos=%2) order by tid").arg(importI3PlcId).arg(exportE3PlcId).arg(getLogSecOld),db));
//	plot->xAxis->rescale(true);
//	plot->xAxis->setDateTimeFormat("hh:mm\nMMMM/dd");
//	plot->xAxis->setTickLabelRotation(-90);
//	plot->xAxis->setTickStep(3600*8);
//	//plot->yAxis->rescale(true);
//	plot->yAxis->setRange(0,100);
//#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
//	qint64 now=(QDateTime::currentMSecsSinceEpoch()/1000);
//	qint64 secsSinceStartOfDay=(QTime::currentTime().msecsSinceStartOfDay()/1000);
//	qint64 startOfNextDay=now-secsSinceStartOfDay+24*3600;
//#else
//	qint64 startOfNextDay=(QDateTime::currentMSecsSinceEpoch()/1000);
//#endif
//	QVector<double> dateV;
//	QVector<double> usageV;
//	qint64 t=startOfNextDay;
//	qint64 dt=24*3600;
//	QVector<double> ticks;
//	QVector<QString> plotLabels;
//	QVector<QString> labels;
//	while (t>(minT-dt))
//	{
//		ticks<<t;
//		plotLabels<<QDateTime::fromMSecsSinceEpoch(t*1000).toString("MMMM/dd");//.toString("hh:mm:ss\nyyyy/MMMM/dd")
//		t-=dt;
//	}
//	plot->xAxis->setTickVector(ticks);
//	plot->xAxis->setTickVectorLabels(plotLabels);
//	plot->replot();
//	//
//	qint64 shiftDt=8*3600;
//	qint64 secsOfFirstShift=6*3600;
//	qint64 secsOfSecondShift=14*3600;
//	qint64 secsOfThirdShift=22*3600;
//	qint64 curentShiftSecs=now-secsSinceStartOfDay+secsOfFirstShift;
//	if (curentShiftSecs<now)
//		curentShiftSecs=now-secsSinceStartOfDay+secsOfSecondShift;
//	if (curentShiftSecs<now)
//		curentShiftSecs=now-secsSinceStartOfDay+secsOfThirdShift;
//	t=curentShiftSecs;
//	QVector<int> allV;
//	QVector<int> importsV;
//	QStringList dates;
//	QStringList days;
//	while (t>(minT-shiftDt))
//	{
//		QString toStr=QDateTime::fromMSecsSinceEpoch(t*1000).toString("yyyy/MM/dd hh:mm:ss");//.
//		QString fromStr=QDateTime::fromMSecsSinceEpoch((t-shiftDt)*1000).toString("yyyy/MM/dd hh:mm:ss");//.
//		allV<<getFirst(QString("select count(tid) from missionsTimeLog where time<'%1' and time>'%2'").arg(toStr).arg(fromStr),db).toInt();
//		importsV<<getFirst(QString("select count(tid) from missionsTimeLog where time<'%1' and time>'%2' and fromPos=%3").arg(toStr).arg(fromStr).arg(importI2PlcId),db).toInt();
//		dates<<fromStr+"-"+toStr;
//		days<<QDateTime::fromMSecsSinceEpoch(t*1000).toString("dddd");
//		t-=shiftDt;
//	}
//	importsExportsTable->clearContents();
//	importsExportsTable->setRowCount(dates.count()-1);
//	for (int i=0;i<importsExportsTable->rowCount();i++)
//	{
//		importsExportsTable->setItem(i,0,new QTableWidgetItem(days[i]));
//		importsExportsTable->setItem(i,1,new QTableWidgetItem(QString("%1").arg(allV[i])));
//		importsExportsTable->setItem(i,2,new QTableWidgetItem(QString("%1").arg(importsV[i])));
//		importsExportsTable->setItem(i,3,new QTableWidgetItem(QString("%1").arg(allV[i]-importsV[i])));
//		importsExportsTable->setItem(i,4,new QTableWidgetItem(dates[i]));
//		if ((days[i]=="Saturday")||(days[i]=="Sunday"))
//		{
//			importsExportsTable->item(i,0)->setData(Qt::BackgroundRole,QColor(200,255,200));
//			importsExportsTable->item(i,1)->setData(Qt::BackgroundRole,QColor(200,255,200));
//			importsExportsTable->item(i,2)->setData(Qt::BackgroundRole,QColor(200,255,200));
//			importsExportsTable->item(i,3)->setData(Qt::BackgroundRole,QColor(200,255,200));
//			importsExportsTable->item(i,4)->setData(Qt::BackgroundRole,QColor(200,255,200));
//		}
//	}
//	importsExportsTable->resizeColumnToContents(0);
//	importsExportsTable->resizeColumnToContents(4);
}
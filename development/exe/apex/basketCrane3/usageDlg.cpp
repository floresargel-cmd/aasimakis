#include "pch.h"
#include "usageDlg.h"
usageDialogClass::usageDialogClass(QWidget *p):QDialog(p)
{
	QDialog::setWindowTitle(tr("Usage"));
	QDialog::resize(1600,600);
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(2);setLayout(mainLayout);
	QHBoxLayout *menuLayout=new QHBoxLayout();mainLayout->addLayout(menuLayout);
	QHBoxLayout *dataLayout=new QHBoxLayout();mainLayout->addLayout(dataLayout);

	menuLayout->addWidget(new QLabel(tr("Calculation interval [hours]:")));
	calculationIntervalSpinBox=new QSpinBox();
	calculationIntervalSpinBox->setMinimum(1);
	calculationIntervalSpinBox->setMaximum(INT_MAX);
	calculationIntervalSpinBox->setSingleStep(1);
	calculationIntervalSpinBox->setValue(2);
	menuLayout->addWidget(calculationIntervalSpinBox);
	menuLayout->addStretch(1);
	connect(calculationIntervalSpinBox,SIGNAL(valueChanged(int)),this,SLOT(calculationIntervalSpinBoxValueChangedSlot(int)));

	plot=new QCustomPlot(this);//Statistics-per-hour graphs
	plot->setMinimumWidth(800);
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
	plot->xAxis->setDateTimeFormat("hh:mm:ss\nyyyy/MMMM/dd");
	plot->xAxis->setLabel("Date-Time");

 	plot->xAxis->setAutoTicks(false);
	plot->xAxis->setAutoTickLabels(false);
	plot->xAxis->setTickLabelRotation(90);
	plot->xAxis->setSubTickCount(24);
	plot->xAxis->setTickLength(0, 4);
	plot->xAxis->grid()->setVisible(true);

  QLinearGradient plotGradient;//to give effect: lighter on top, gradually darker on botton
  plotGradient.setStart(0, 0);
  plotGradient.setFinalStop(0, 350);
  plotGradient.setColorAt(0, QColor(150, 150, 150	));
  plotGradient.setColorAt(1, QColor(50, 50, 50));
  plot->setBackground(plotGradient);
  QLinearGradient axisRectGradient;//for effect on the area of the 2 axis
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

	destackerPos1Usage=plot->addGraph();
	destackerPos1Usage->setName(tr("Working Position 1"));
	destackerPos1Usage->setPen(QPen(QColor(200,250,200)));
	destackerPos1Usage->setBrush(QBrush(QColor(200,250,200,100)));

	destackerPos2Usage=plot->addGraph();
	destackerPos2Usage->setName(tr("Working Position 2"));
	destackerPos2Usage->setPen(QPen(QColor(250,200,200)));
	destackerPos2Usage->setBrush(QBrush(QColor(250,200,200,100)));

	missionsTable=new QTableWidget();
	dataLayout->addWidget(missionsTable);
	missionsTable->setAlternatingRowColors(true);
  missionsTable->horizontalHeader()->setStretchLastSection(true);
	missionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	missionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	missionsTable->setColumnCount(5);//columns for the next 5 fields
	missionsTable->setHorizontalHeaderLabels(QStringList()<<tr("Day")<<tr("Total")<<tr("Work Position 1")<<tr("Work Position 2")<<tr("Date"));
	missionsTable->setWordWrap(true);
	missionsTable->setMaximumWidth(700);
	update();
} 
void usageDialogClass::calculationIntervalSpinBoxValueChangedSlot(int interval)
{
	update();
}
void usageDialogClass::fillGraph(QCPGraph *statsGraph,QVector<QVector<QVariant>> dataVV)
{
	statsGraph->clearData();
	if (dataVV.count()==0)
		return;
	QVector<missionClass> missionsVector;
	for (int i=0;i<dataVV.count();i++)
		missionsVector<<missionClass(dataVV[i][0].toInt(),dataVV[i][1].toInt(),dataVV[i][2].toInt(),dataVV[i][3].toInt());
	if (statsGraph==totalUsage)  ////?? what about when statsGraph==destackerPos1Usage or statsGraph==destackerPos2Usage
	{
		minT=missionsVector[0].start;
		maxT=QDateTime::currentDateTime().toTime_t();
	}
	int dt=calculationIntervalSpinBox->value()*3600;
	int from=maxT;//show statistics plots from current time, backwards...
	int to=from-dt;//... in chunks defined by the spinbox (e.g. 1 hour=3600 , or 2 hours=7200)
	if (to<minT)//do not go too far back in time, even if the user asks it through the spinbox value
		to=minT-1;
	QVector<double> dateV;
	QVector<double> usageV;
	double totalTime=0;//total time-frame for the statistics
	while (from>minT)
	{
		int usedTime=0;//time when missions were happening, against the total time displayed in the graph
		for (int i=0;i<missionsVector.count();i++)
			usedTime+=missionsVector[i].getUsage(from,to);
		double percentage=100.*((double)usedTime/(double)(from-to));
		dateV<<from<<to;
		if (percentage>100.)
			percentage=100.;
		usageV<<percentage<<percentage;//inserting the same percentage for the stert and end of hour to get the relevant horizontal line of usage
		from=to;
		to=from-dt;
		totalTime+=usedTime;
	}
	statsGraph->setData(dateV,usageV);
	statsGraph->setName(QString("%1 %2").arg(statsGraph->name()).arg(QString(" %1%").arg(100.*totalTime/(maxT-minT),0,'f',2)));
}
void usageDialogClass::update()
{
	maxT=minT=0;
	fillGraph(totalUsage,execTableQuery(QString("select fromPos,toPos,startSec,endSec from crane3MissionsTimeLog where endSec<>0 order by tid"),bDb));
	fillGraph(destackerPos1Usage,execTableQuery(QString("select fromPos,toPos,startSec,endSec from crane3MissionsTimeLog where endSec<>0 and (fromPos=%1 or toPos=%1) order by tid").arg(destackerWorkingPos1PlcId+1),bDb));
	fillGraph(destackerPos2Usage,execTableQuery(QString("select fromPos,toPos,startSec,endSec from crane3MissionsTimeLog where endSec<>0 and (fromPos=%1 or toPos=%1) order by tid").arg(destackerWorkingPos2PlcId+1),bDb));
	plot->xAxis->rescale(true);
	plot->xAxis->setDateTimeFormat("hh:mm\nMMMM/dd");
	plot->xAxis->setTickLabelRotation(-90);
	plot->xAxis->setTickStep(3600*8);
  plot->yAxis->rescale(true);
	plot->yAxis->setRange(0,100);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	qint64 now=(QDateTime::currentMSecsSinceEpoch()/1000);
	qint64 secsSinceStartOfDay=(QTime::currentTime().msecsSinceStartOfDay()/1000);
	qint64 startOfNextDay=now-secsSinceStartOfDay+24*3600;
#else
	qint64 startOfNextDay=(QDateTime::currentMSecsSinceEpoch()/1000);
#endif
	QVector<double> dateV;
	QVector<double> usageV;
	qint64 t=startOfNextDay;
	qint64 dt=24*3600;
	QVector<double> ticks;
	QVector<QString> plotLabels;
	QVector<QString> labels;
	while (t>(minT-dt))
	{
		ticks<<t;
		plotLabels<<QDateTime::fromMSecsSinceEpoch(t*1000).toString("MMMM/dd");//.toString("hh:mm:ss\nyyyy/MMMM/dd")
		t-=dt;//go backwards our current  time t, by the amount of seconds of a whole day (24 hours)
	}
	plot->xAxis->setTickVector(ticks);
	plot->xAxis->setTickVectorLabels(plotLabels);
	plot->replot();

	qint64 shiftDt=8*3600;//each day has 3 such shifts: 06:00-14:00, 14:00-22:00 and 22:00-06:00
	qint64 secsOfFirstShift=6*3600;//number of secs from midnight until start of 1st shift
	qint64 secsOfSecondShift=14*3600;//number of secs from midnight until start of 2nd shift
	qint64 secsOfThirdShift=22*3600;//number of secs from midnight until start of 3rd shift
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	qint64 curentShiftSecs=now-secsSinceStartOfDay+secsOfFirstShift;
	if (curentShiftSecs<now)
		curentShiftSecs=now-secsSinceStartOfDay+secsOfSecondShift;
	if (curentShiftSecs<now)
		curentShiftSecs=now-secsSinceStartOfDay+secsOfThirdShift;
	t=curentShiftSecs;
#endif
	QVector<int> allV;
	QVector<int> destacker1PosV; 
	QVector<int> destacker2PosV; 
	QStringList dates;
	QStringList days;
	while (t>(minT-shiftDt))
	{
		QString toStr=QDateTime::fromMSecsSinceEpoch(t*1000).toString("yyyy/MM/dd hh:mm:ss");//.
		QString fromStr=QDateTime::fromMSecsSinceEpoch((t-shiftDt)*1000).toString("yyyy/MM/dd hh:mm:ss");//.
		allV<<getFirst(QString("select count(tid) from crane3MissionsTimeLog where time<'%1' and time>'%2'").arg(toStr).arg(fromStr),bDb).toInt();
		destacker1PosV<<getFirst(QString("select count(tid) from crane3MissionsTimeLog where time<'%1' and time>'%2' and (fromPos=%3 or toPos=%3)").arg(toStr).arg(fromStr).arg(destackerWorkingPos1PlcId+1),bDb).toInt();
		destacker2PosV<<getFirst(QString("select count(tid) from crane3MissionsTimeLog where time<'%1' and time>'%2' and (fromPos=%3 or toPos=%3)").arg(toStr).arg(fromStr).arg(destackerWorkingPos2PlcId+1),bDb).toInt();
		dates<<fromStr+"-"+toStr;
		days<<QDateTime::fromMSecsSinceEpoch(t*1000).toString("dddd");
		t-=shiftDt;
	}
	missionsTable->clearContents();
	missionsTable->setRowCount(dates.count()/*-1*/);////?? Why -1 and not show the earliest row??
	for (int i=0;i<dates.count();i++)
	{
		int row=0;
		missionsTable->setItem(i,row++,new QTableWidgetItem(days[i]));
		missionsTable->setItem(i,row++,new QTableWidgetItem(QString("%1").arg(allV[i])));
		missionsTable->setItem(i,row++,new QTableWidgetItem(QString("%1").arg(destacker1PosV[i])));
		missionsTable->setItem(i,row++,new QTableWidgetItem(QString("%1").arg(destacker2PosV[i])));
		missionsTable->setItem(i,row++,new QTableWidgetItem(dates[i]));
		if ((days[i]=="Saturday")||(days[i]=="Sunday"))//use your PC local settings for how to spell the days here!
		{
			missionsTable->item(i,0)->setData(Qt::BackgroundRole,QColor(200,255,200));
			missionsTable->item(i,1)->setData(Qt::BackgroundRole,QColor(200,255,200));
			missionsTable->item(i,2)->setData(Qt::BackgroundRole,QColor(200,255,200));
			missionsTable->item(i,3)->setData(Qt::BackgroundRole,QColor(200,255,200));
			missionsTable->item(i,4)->setData(Qt::BackgroundRole,QColor(200,255,200));
		}
	}
	missionsTable->resizeColumnToContents(0);
	missionsTable->resizeColumnToContents(4);
}
#include "pch.h"
#include "logW.h"

logWidgetClass::logWidgetClass(QString dtLogDbName_,QString plcIp_,QString plcTable_,int plcIndex_,int plcBit_,QString machine_,QString operation_,int durationToShow_,bool whenOnIsOk_,QWidget *p):QWidget(p)
{
	dtLogDbName=dtLogDbName_;
	plcIp=plcIp_;
	plcTable=plcTable_;
	plcIndex=plcIndex_;
	plcBit=plcBit_;
	machine=machine_;
	operation=operation_;
	durationToShow=durationToShow_;
	whenOnIsOk=whenOnIsOk_;
	//
	machinesTid=getFirst(QString("select tId from machines where machine='%1' and operation='%2'").arg(machine).arg(operation),dtLogDbName).toInt();
	if (machinesTid==0)
	{
		execQuery(QString("insert into machines (machine,operation) values('%1','%2')").arg(machine).arg(operation),dtLogDbName);
		machinesTid=getFirst(QString("select tId from machines where machine='%1' and operation='%2'").arg(machine).arg(operation),dtLogDbName).toInt();
	}
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);setLayout(mainLayout);
	QHBoxLayout *upLayout=new QHBoxLayout();mainLayout->addLayout(upLayout);
	QHBoxLayout *downLayout=new QHBoxLayout();mainLayout->addLayout(downLayout);
	//
	activeLabel=new QLabel();
	upLayout->addWidget(activeLabel);
	QPushButton *updateDatabaseBtn=new QPushButton(QIcon(":/reset.png"),tr("Refresh"),this);
	upLayout->addWidget(updateDatabaseBtn);
	connect(updateDatabaseBtn,SIGNAL(clicked()),this,SLOT(updtateFromDatabaseBtnSlot()));

	//
	table=new QTableWidget();
	table->setAlternatingRowColors(true);
	table->horizontalHeader()->setStretchLastSection(true);
	//table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->setColumnCount(4);
	table->setHorizontalHeaderLabels(QStringList()<<tr("Reason")<<tr("Comment")<<tr("Time")<<tr("Duration"));
	connect(table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));

	downLayout->addWidget(table,1);
	//
	QVector<QVariant> reasonD=execQuery(QString("select reason from reasons where machinesTid=%1 order by tId").arg(machinesTid),dtLogDbName);
	reasonsList<<tr("...");
	for (int i=0;i<reasonD.count();i++)
		reasonsList<<reasonD[i].toString().trimmed();
	//
	reasonsComboMapper=new QSignalMapper(this);
	connect(reasonsComboMapper,SIGNAL(mapped(int)),this,SLOT(reasonsComboMapperSlot(int)));

	QTimer *updateGuiTimer=new QTimer(this);
	connect(updateGuiTimer,SIGNAL(timeout()),this,SLOT(updateGuiTimerSlot()));
	updateGuiTimer->start(2000);
	updateGui();
	updateFromDataBase();
}
logWidgetClass::~logWidgetClass()
{
}
QString logWidgetClass::getDisplayText()
{
	return QString("%1.%2").arg(machine).arg(operation);
}
int logWidgetClass::getMachinesTid()
{
	return machinesTid;
}
QString progressBarRedStyleSheet =  "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 10px;}";
QString progressBarGreenStyleSheet= "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #00FF50,stop: 0.4999 #00FF20,stop: 0.5 #00FF19,stop: 1 #00FF00 );border-bottom-right-radius: 7px;}";
QString progressBarBlueStyleSheet=  "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #0099FF,stop: 0.4999 #0050FF,stop: 0.5 #0049FF,stop: 1 #0000FF );border-bottom-right-radius: 7px;}";
void logWidgetClass::updateFromDataBase()
{
	QString startDayStr=QDate::currentDate().addDays(-1).toString("yyyy/MM/dd");
	int maxSecs=getFirst(QString("select max(endSec-startSec) from log where machinesTid=%1 and time>'%2'").arg(machinesTid).arg(startDayStr),dtLogDbName).toInt();
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select log.tId,reasons.reason,comment,endSec-startSec as diff,time,startSec,endSec from log left join reasons on log.reasonsTid=reasons.tId where log.machinesTid=%1 and (endSec-startSec)>%2 and time>'%3'order by time desc").arg(machinesTid).arg(durationToShow).arg(startDayStr),dtLogDbName);
	table->blockSignals(true);
	table->clearContents();
	table->setRowCount(dataVV.count());
	reasonsComboMapper->removeMappings(this);
	reasonsComboV.clear();
	QVector<QProgressBar*> progressBarsV;
	for (int i=0;i<dataVV.count();i++)
	{
		int logTid=dataVV[i][0].toInt();
		QComboBox *tCombo=new QComboBox(this);
		reasonsComboV<<tCombo;
		connect(tCombo,SIGNAL(currentIndexChanged(int)),reasonsComboMapper,SLOT(map()));
		tCombo->addItems(reasonsList);
		for (int j=0;j<tCombo->count();j++)
			tCombo->setItemData(j,logTid,Qt::UserRole);
		tCombo->setCurrentText(dataVV[i][1].toString().trimmed());
		table->setCellWidget(i,0,tCombo);
		reasonsComboMapper->setMapping(tCombo,i);
		//table->cellWidget(i,0))->setData(Qt::UserRole,logTid);
		//table->item(i,0)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);

		table->setItem(i,1,new QTableWidgetItem(dataVV[i][2].toString().trimmed()));
		table->item(i,1)->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		table->item(i,1)->setData(Qt::UserRole,logTid);

		table->setItem(i,2,new QTableWidgetItem(dataVV[i][4].toString().trimmed()));
		table->item(i,2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

		QProgressBar *pr=new QProgressBar(this);
		pr->setFormat("%v sec");
		pr->setMinimum(0);
		pr->setMaximum(maxSecs);
		pr->setValue(dataVV[i][3].toInt());
		qlonglong diff=dataVV[i][3].toLongLong();
		qlonglong start=dataVV[i][5].toLongLong();
		qlonglong end=dataVV[i][6].toLongLong();
		pr->setToolTip(tr("Duration: %1").arg(QDateTime::fromTime_t(diff).toUTC().toString("hh:mm:ss"))
			+tr("\nFrom %1").arg(QDateTime::fromTime_t(start).toLocalTime().toString("hh:mm:ss"))
			+tr("\nTo %1").arg(QDateTime::fromTime_t(end).toLocalTime().toString("hh:mm:ss")));
		pr->setAlignment(Qt::AlignCenter);

		if (dataVV[i][3].toInt()<(0.2*maxSecs))
			pr->setStyleSheet(progressBarGreenStyleSheet);
		else if (dataVV[i][3].toInt()<(0.5*maxSecs))
			pr->setStyleSheet(progressBarBlueStyleSheet);
		else
			pr->setStyleSheet(progressBarRedStyleSheet);
		table->setCellWidget(i,3,pr);
	}
	table->blockSignals(false);
	table->resizeColumnToContents(0);
	table->resizeColumnToContents(1);
	table->resizeColumnToContents(2);
	table->update();
}
QString secondsToDHMS(qlonglong duration)
{
  QString res;
  int seconds = (int) (duration % 60);
  duration /= 60;
  int minutes = (int) (duration % 60);
  duration /= 60;
  int hours = (int) (duration % 24);
  int days = (int) (duration / 24);
  if((hours == 0)&&(days == 0))
      return res.sprintf("%02d:%02d", minutes, seconds);
  if (days == 0)
      return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
  return res.sprintf("%dd%02d:%02d:%02d", days, hours, minutes, seconds);
}
void logWidgetClass::updateGui()
{
	qlonglong start=getFirst(QString("select startSec from log where machinesTid=%1 and endSec=0").arg(machinesTid),dtLogDbName).toLongLong();
	if (start>0)
	{
#ifndef downTimeLog
		execQuery(QString("update log set currentSecs=%1 where endSec=0 and machinesTid=%2").arg(QDateTime::currentDateTime().toTime_t()).arg(machinesTid),dtLogDbName);
#endif
		qlonglong nowSec=getFirst(QString("select currentSecs from log where machinesTid=%1 and endSec=0").arg(machinesTid),dtLogDbName).toLongLong();
		QString lala=tr("Active for: %1").arg(secondsToDHMS(nowSec-start));
		activeLabel->setText(tr("Active for: %1").arg(secondsToDHMS(nowSec-start)));
	}
	else
		activeLabel->setText(tr("Inactive"));
}
void logWidgetClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
#ifdef downTimeLog
	return;
#endif
	if ((plcIp==ip)&&(plcTable==table)&&(plcIndex==index))
	{
		bool needsUpdate=false;
		bool isDown=!HAVECOMMONBIT(value.toInt(),BITS[plcBit]);
		if (!whenOnIsOk)
			isDown=!isDown;
		if (isDown)
		{
			bool allreadyLogged=(getFirst(QString("select count(tId) from log where machinesTid=%1 and endSec=0").arg(machinesTid),dtLogDbName).toInt()>0);
			if (!allreadyLogged)
			{
				execQuery(QString("insert into log (machinesTid,reasonsTid,comment,startSec,endSec,time,currentSecs) values(%1,0,'',%2,0,'%3',%4)").arg(machinesTid).arg(QDateTime::currentDateTime().toTime_t()).arg(timeForLog).arg(QDateTime::currentDateTime().toTime_t()),dtLogDbName);
				needsUpdate=true;
			}
		}
		else
		{
			qlonglong startSec=getFirst(QString("select startSec from log where machinesTid=%1 and endSec=0").arg(machinesTid),dtLogDbName).toInt();
			qlonglong nowSec=QDateTime::currentDateTime().toTime_t();
			if ((nowSec-startSec)>5)
			{
				execQuery(QString("update log set endSec=%1 where endSec=0 and machinesTid=%2").arg(nowSec).arg(machinesTid),dtLogDbName);
				needsUpdate=true;
			}
			else 
				execQuery(QString("delete from log where endSec=0 and machinesTid=%1").arg(machinesTid),dtLogDbName);
		}
		if (needsUpdate)
			updateFromDataBase();
	}
}
void logWidgetClass::reasonsComboMapperSlot(int comboIndex)
{
	if (comboIndex<reasonsComboV.count())
	{
		int reasonsTid=getFirst(QString("select tId from reasons where reason='%1' and machinesTid=%2").arg(reasonsComboV[comboIndex]->currentText()).arg(machinesTid),dtLogDbName).toInt();
		execQuery(QString("update log set reasonsTid=%1 where tId=%2").arg(reasonsTid).arg(reasonsComboV[comboIndex]->currentData(Qt::UserRole).toInt()),dtLogDbName);
		updateFromDataBase();
	}
}
void logWidgetClass::itemChangedSlot(QTableWidgetItem *item)
{
	if (item->column()==1)//comment
	{
		execQuery(QString("update log set comment='%1' where tId=%2").arg(item->text().replace("'","").replace("\"","")).arg(item->data(Qt::UserRole).toInt()),dtLogDbName);
	}
	updateFromDataBase();
}
void logWidgetClass::updtateFromDatabaseBtnSlot()
{
	updateFromDataBase();
}
void logWidgetClass::updateGuiTimerSlot()
{
	if (QWidget::isVisible())
		updateGui();
}
#ifndef CW_H
#define CW_H

#include <QtGui>
#include <QMainWindow>

#include "Gbo.h"
#include "../common/apexG.h"
/////////////////////////////////////////////////////////////////////////////////////emptyBasketsClass
class emptyBasketsClass : public QWidget				
{
	QLabel			*valueLabel;
Q_OBJECT								
public:										
	emptyBasketsClass()
	{
		QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
		QLabel *labelQLabel=new QLabel(tr("Available empty baskets"));labelQLabel->setAlignment(Qt::AlignCenter);
		labelQLabel->setStyleSheet("QLabel{font-size:20px;color:#000055;}");
		valueLabel=new QLabel(tr("..."));valueLabel->setAlignment(Qt::AlignCenter);
		mainLayout->addWidget(valueLabel);
		mainLayout->addWidget(labelQLabel);
		mainLayout->addStretch(1);
	}
	~emptyBasketsClass()
	{
	}
	void updateFromDatabase()
	{
		QVector<QVariant> basketsInEmptyBufferVector=execQuery("select basketTableId from positions where basketTableId<>0 and description like 'empty%'",MAIN_DB_CONNECTION);
		QVector<QVariant> fullBasketsVector=execQuery("select basketnum from basket_details where Pc>0",APEX_DB_CONNECTION);
		int noOfEmpty=0;
		QString toolTipStr=tr("Baskets:\n");
		for (int i=0;i<basketsInEmptyBufferVector.count();i++)
		{
			if (fullBasketsVector.indexOf(basketsInEmptyBufferVector[i])==-1)
			{
				toolTipStr+="\n"+basketsInEmptyBufferVector[i].toString();
				noOfEmpty++;
			}
		}
		setToolTip(toolTipStr);
		if (noOfEmpty>5)
			valueLabel->setStyleSheet("QLabel{font-size:150px;color:#000055;}");
		else
			valueLabel->setStyleSheet("QLabel{font-size:150px;color:#770000;}");
		valueLabel->setText(QString("%1").arg(noOfEmpty));
	}
};
/////////////////////////////////////////////////////////////////////////////////////totalUnagedBasketsClass
class totalUnagedBasketsClass : public QWidget				
{
	QLabel			*valueLabel;
Q_OBJECT								
public:										
	totalUnagedBasketsClass()
	{
		QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
		QLabel *labelQLabel=new QLabel(tr("Unaged baskets total"));labelQLabel->setAlignment(Qt::AlignCenter);
		labelQLabel->setStyleSheet("QLabel{font-size:20px;color:#000055;}");
		valueLabel=new QLabel(tr("..."));valueLabel->setAlignment(Qt::AlignCenter);
		mainLayout->addWidget(valueLabel);
		mainLayout->addWidget(labelQLabel);
		mainLayout->addStretch(1);
	}
	~totalUnagedBasketsClass()
	{
	}
	void updateFromDatabase()
	{
		QVector<QVariant> basketsInBufferVector=execQuery("select basketTableId from positions where basketTableId<>0 and (posNumber=200 or  posNumber=230 or  posNumber=240 or  posNumber=250 or  posNumber=260 or  posNumber=270 or  posNumber=280 or  posNumber=210 or  posNumber=220 or (posNumber>=300 and posNumber<=390))",MAIN_DB_CONNECTION);
		QVector<QVariant> fullBasketsVector=execQuery("select basketnum from basket_details where Pc>0",APEX_DB_CONNECTION);
		int noOfUnAged=0;
		QString toolTipStr=tr("Baskets:");
		for (int i=0;i<basketsInBufferVector.count();i++)
		{
			if (fullBasketsVector.indexOf(basketsInBufferVector[i])!=-1)
			{
				QVariant time=GETFIRST(QString("select audittime from basket_location where basketnum=%1 and basketlocation=%2").arg(basketsInBufferVector[i].toInt()).arg(3),APEX_DB_CONNECTION);
				if (time.isNull())
				{
					toolTipStr+="\n"+basketsInBufferVector[i].toString();
					noOfUnAged++;
				}
			}
		}
		setToolTip(toolTipStr);
		if (noOfUnAged>0)
			valueLabel->setStyleSheet("QLabel{font-size:150px;color:#770000;}");
		else
			valueLabel->setStyleSheet("QLabel{font-size:150px;color:#000055;}");
		valueLabel->setText(QString("%1").arg(noOfUnAged));
	}
};
/////////////////////////////////////////////////////////////////////////////////////totalUnagedBasketsClass
class longUnagedBasketsClass : public QWidget				
{
	QLabel			*valueLabel;
Q_OBJECT								
public:										
	longUnagedBasketsClass()
	{
		QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
		QLabel *labelQLabel=new QLabel(tr("Unaged baskets more than 24 feet"));labelQLabel->setAlignment(Qt::AlignCenter);
		labelQLabel->setStyleSheet("QLabel{font-size:20px;color:#000055;}");
		valueLabel=new QLabel(tr("..."));valueLabel->setAlignment(Qt::AlignCenter);
		mainLayout->addWidget(valueLabel);
		mainLayout->addWidget(labelQLabel);
		mainLayout->addStretch(1);
	}
	~longUnagedBasketsClass()
	{
	}
	void updateFromDatabase()
	{
		QVector<QVariant> basketsInBufferVector=execQuery("select basketTableId from positions where basketTableId<>0 and (posNumber=200 or  posNumber=230 or  posNumber=240 or  posNumber=250 or  posNumber=260 or  posNumber=270 or  posNumber=280 or  posNumber=210 or  posNumber=220 or (posNumber>=300 and posNumber<=390))",MAIN_DB_CONNECTION);
		QVector<QVariant> fullBasketsVector=execQuery("select basketnum from basket_details where Pc>0 and PcLen>7366",APEX_DB_CONNECTION);
		int noOfUnAged=0;
		QString toolTipStr=tr("Baskets:");
		for (int i=0;i<basketsInBufferVector.count();i++)
		{
			if (fullBasketsVector.indexOf(basketsInBufferVector[i])!=-1)
			{
				QVariant time=GETFIRST(QString("select audittime from basket_location where basketnum=%1 and basketlocation=%2").arg(basketsInBufferVector[i].toInt()).arg(3),APEX_DB_CONNECTION);
				if (time.isNull())
				{
					toolTipStr+="\n"+basketsInBufferVector[i].toString();
					noOfUnAged++;
				}
			}
		}
		setToolTip(toolTipStr);
		if (noOfUnAged>0)
			valueLabel->setStyleSheet("QLabel{font-size:150px;color:#770000;}");
		else
			valueLabel->setStyleSheet("QLabel{font-size:150px;color:#000055;}");
		valueLabel->setText(QString("%1").arg(noOfUnAged));
	}
};
/////////////////////////////////////////////////////////////////////////////////////availableBasketsForPackagingClass
class availableBasketsForPackagingClass : public QWidget				
{
	QLabel			*valueLabel;
Q_OBJECT								
public:										
	availableBasketsForPackagingClass()
	{
		QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
		QLabel *labelQLabel=new QLabel(tr("Available baskets for packaging"));labelQLabel->setAlignment(Qt::AlignCenter);
		labelQLabel->setStyleSheet("QLabel{font-size:20px;color:#000055;}");
		valueLabel=new QLabel(tr("..."));valueLabel->setAlignment(Qt::AlignCenter);
		mainLayout->addWidget(valueLabel);
		mainLayout->addWidget(labelQLabel);
		mainLayout->addStretch(1);
	}
	~availableBasketsForPackagingClass()
	{
	}
	void updateFromDatabase()
	{
		QVector<QVariant> basketsInBufferVector=execQuery("select basketTableId from positions where basketTableId<>0 and (posNumber=200 or  posNumber=230 or  posNumber=240 or  posNumber=250 or  posNumber=260 or  posNumber=270 or  posNumber=280 or  posNumber=210 or  posNumber=220 or  posNumber=160 or (posNumber>=300 and posNumber<=390))",MAIN_DB_CONNECTION);
		QVector<QVariant> fullBasketsVector=execQuery("select basketnum from basket_details where Pc>0",APEX_DB_CONNECTION);
		int noOfAged=0;
		QString toolTipStr=tr("Baskets:");
		for (int i=0;i<basketsInBufferVector.count();i++)
		{
			if (fullBasketsVector.indexOf(basketsInBufferVector[i])!=-1)
			{
				QVariant time=GETFIRST(QString("select audittime from basket_location where basketnum=%1 and basketlocation=%2").arg(basketsInBufferVector[i].toInt()).arg(3),APEX_DB_CONNECTION);
				if (!time.isNull())
				{
					toolTipStr+="\n"+basketsInBufferVector[i].toString();
					noOfAged++;
				}
			}
		}
		setToolTip(toolTipStr);
		if (noOfAged>10)
			valueLabel->setStyleSheet("QLabel{font-size:150px;color:#770000;}");
		else
			valueLabel->setStyleSheet("QLabel{font-size:150px;color:#000055;}");
		valueLabel->setText(QString("%1").arg(noOfAged));
	}
};
/////////////////////////////////////////////////////////////////////////////////////basketsInOvenClass
class basketsInOvenClass : public QWidget				
{
	QLabel			*valueLabel;
	QLabel			*remainingTimeLabel;
Q_OBJECT								
public:										
	basketsInOvenClass()
	{
		QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
		QLabel *labelQLabel=new QLabel(tr("Baskets in aging oven"));labelQLabel->setAlignment(Qt::AlignCenter);
		labelQLabel->setStyleSheet("QLabel{font-size:20px;color:#000055;}");
		remainingTimeLabel=new QLabel(tr("..."));remainingTimeLabel->setAlignment(Qt::AlignCenter);
		remainingTimeLabel->setStyleSheet("QLabel{font-size:20px;color:#005500;}");
		valueLabel=new QLabel(tr("..."));valueLabel->setAlignment(Qt::AlignCenter);
		valueLabel->setStyleSheet("QLabel{font-size:150px;color:#000055;}");
		mainLayout->addWidget(valueLabel);
		mainLayout->addWidget(remainingTimeLabel);
		mainLayout->addWidget(labelQLabel);
		mainLayout->addStretch(1);
	}
	~basketsInOvenClass()
	{
	}
	void updateFromDatabase()
	{
		QVector<QVariant> basketsInOvenVector=execQuery("select basketTableId from positions where basketTableId<>0 and posNumber>=600 and posNumber<=650",MAIN_DB_CONNECTION);
		valueLabel->setText(QString("%1").arg(basketsInOvenVector.count()));
	}
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value)
	{
		if ((ip==conveyorsIp)&&(table=="N11")&&(index==35))
		{
			QString timeStr;
			int time=value.toInt();
			int hours=time/(60);time-=hours*(60);
			int minutes=time;
			if (hours>0)
				timeStr+=QString("%1 hours ").arg(hours);
			if (minutes>0)
				timeStr+=QString("%1 minutes ").arg(minutes);
			if (value.toInt()>0)
				timeStr+="left";
			remainingTimeLabel->setText(timeStr);
		}
	}
};
/////////////////////////////////////////////////////////////////////////////////////basketsAfterOvenClass
class basketsAfterOvenClass : public QWidget				
{
	QLabel			*valueLabel;
Q_OBJECT								
public:										
	basketsAfterOvenClass()
	{
		QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
		QLabel *labelQLabel=new QLabel(tr("Baskets after the aging oven"));labelQLabel->setAlignment(Qt::AlignCenter);
		labelQLabel->setStyleSheet("QLabel{font-size:20px;color:#000055;}");
		valueLabel=new QLabel(tr("..."));valueLabel->setAlignment(Qt::AlignCenter);
		valueLabel->setStyleSheet("QLabel{font-size:150px;color:#000055;}");
		mainLayout->addWidget(valueLabel);
		mainLayout->addWidget(labelQLabel);
		mainLayout->addStretch(1);
	}
	~basketsAfterOvenClass()
	{
	}
	void updateFromDatabase()
	{
		QVector<QVariant> basketsInOvenVector=execQuery("select basketTableId from positions where basketTableId<>0 and posNumber>=660 and posNumber<=730",MAIN_DB_CONNECTION);
		valueLabel->setText(QString("%1").arg(basketsInOvenVector.count()));
	}
};
/////////////////////////////////////////////////////////////////////////////////////emptyBasketsNeededClass
class emptyBasketsNeededClass : public QWidget				
{
	QLabel			*valueLabel;
	QLabel			*labelQLabel;
	int					maxBillets;
Q_OBJECT								
public:										
	emptyBasketsNeededClass()
	{
		QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
		labelQLabel=new QLabel(tr("..."));labelQLabel->setAlignment(Qt::AlignCenter);
		labelQLabel->setStyleSheet("QLabel{font-size:20px;color:#000055;}");
		valueLabel=new QLabel(tr("..."));valueLabel->setAlignment(Qt::AlignCenter);
		valueLabel->setStyleSheet("QLabel{font-size:150px;color:#000055;}");
		mainLayout->addWidget(valueLabel);
		mainLayout->addWidget(labelQLabel);
		mainLayout->addStretch(1);
		maxBillets=10;
	}
	~emptyBasketsNeededClass()
	{
	}
	void updateFromDatabase()
	{
		QVector<QVariant> soNumsToComeVector=execQuery("select soNum from epicsProduction where status=0 order by productionOrder",productionPlanDbConnection);
		int billets=0;
		int basketsNeeded=0;
		for (int i=0;i<soNumsToComeVector.count();i++)
		{
			billets+=GETFIRST(QString("select count (soNum) from Billets where soNum='%1'").arg(soNumsToComeVector[i].toString()),productionPlanDbConnection).toInt();
			basketsNeeded=GETFIRST(QString("select baskets from epicsProduction where soNum='%1'").arg(soNumsToComeVector[i].toString()),productionPlanDbConnection).toInt();
			if (billets>maxBillets)
				break;
		}
		labelQLabel->setText(tr("Baskets needed for the next %1 billets").arg(billets));
		valueLabel->setText(QString("%1").arg(basketsNeeded));
	}
};
#endif



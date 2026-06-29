#include "pch.h"
#include "carriageW.h"
carriageWidgetClass::carriageWidgetClass(QWidget *p):QGroupBox(p)
{
	logGuiName="crane";
	plcCraneStepTexts.add(carriageClass::plcStepIdle							,tr("Idle")			                                    ,tr("Plc status:%1\nIdle")																				.arg(carriageClass::plcStepIdle)	);
	plcCraneStepTexts.add(carriageClass::plcStepStart							,tr("Active mission")																,tr("Plc status:%1\nActive mission")															.arg(carriageClass::plcStepStart)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoToXYLoad				,tr("Go to xy target load position")								,tr("Plc status:%1\nGo to xy target load position")								.arg(carriageClass::plcStepGoToXYLoad)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoToZLoad					,tr("Go to z target load position")									,tr("Plc status:%1\nGo to z target load position")								.arg(carriageClass::plcStepGoToZLoad)	);
	plcCraneStepTexts.add(carriageClass::plcStepPinsGetBasket			,tr("Pins out to get basket")												,tr("Plc status:%1\nPins out to get basket")											.arg(carriageClass::plcStepPinsGetBasket)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoTo0Z						,tr("Go to 0 Z")																		,tr("Plc status:%1\nGo to 0 Z")																		.arg(carriageClass::plcStepGoTo0Z)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoToXYUnLoad			,tr("Go to xy target unload position with basket")	,tr("Plc status:%1\nGo to xy target unload position with basket")	.arg(carriageClass::plcStepGoToXYUnLoad)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoToZUnload				,tr("Go to z target unload position with basket")		,tr("Plc status:%1\nGo to z target unload position with basket")	.arg(carriageClass::plcStepGoToZUnload)	);
	plcCraneStepTexts.add(carriageClass::plcStepPinsReleaseBasket	,tr("Pins in to release basket")										,tr("Plc status:%1\nPins in to release basket")										.arg(carriageClass::plcStepPinsReleaseBasket)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoToZeroZ					,tr("Go to 0 Z")																		,tr("Plc status:%1\nGo to 0 Z")																		.arg(carriageClass::plcStepGoToZeroZ)	);
	plcCraneStepTexts.add(carriageClass::plcStepFinished					,tr("Mission finished")															,tr("Plc status:%1\nThe mission finished succesfully")						.arg(carriageClass::plcStepFinished)	);

	setTitle(tr("Crane"));
	setToolTip(tr("Crane status"));
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(1);setLayout(mainLayout);
	//active
	QGroupBox *activeGroupBox=new QGroupBox(tr("Active mission"));mainLayout->addWidget(activeGroupBox);
	QVBoxLayout *activeGroupBoxLayout=new QVBoxLayout();activeGroupBox->setLayout(activeGroupBoxLayout);
	activeMissionLabel=new QLabel();activeGroupBoxLayout->addWidget(activeMissionLabel);
	activeCraneStepLabel=new QLabel();activeGroupBoxLayout->addWidget(activeCraneStepLabel);
	activeHooksStepLabel=new QLabel();activeGroupBoxLayout->addWidget(activeHooksStepLabel);
	//next
	QGroupBox *nextGroupBox=new QGroupBox(tr("Next mission"));mainLayout->addWidget(nextGroupBox);
	QVBoxLayout *nextGroupBoxLayout=new QVBoxLayout();nextGroupBox->setLayout(nextGroupBoxLayout);
	nextMissionLabel=new QLabel();nextGroupBoxLayout->addWidget(nextMissionLabel);
	//logGuiList
	logGuiList=new QListWidget();logGuiList->setMinimumHeight(20);mainLayout->addWidget(logGuiList);
	QPushButton *popUpListBtn=new QPushButton(QIcon(":/popUp.png"),"");
	logGuiList->setCornerWidget(popUpListBtn);
	connect(popUpListBtn,SIGNAL(clicked()),this,SLOT(popUpList()));
	connect(logGuiList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(popUpList()));
	loadFromDataBase();
}
carriageWidgetClass::~carriageWidgetClass()
{
}
void carriageWidgetClass::popUpList()
{
	QDialog *dlg=new QDialog(this);
	dlg->setWindowTitle(tr("Crane log"));
	dlg->resize(1100,600);
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(1);dlg->setLayout(mainLayout);
	QTableWidget *logTable=new QTableWidget();mainLayout->addWidget(logTable);
	logTable->setAlternatingRowColors(true);
	logTable->horizontalHeader()->setStretchLastSection(true);
	logTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	logTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	logTable->setColumnCount(4);
	logTable->setHorizontalHeaderLabels(QStringList()<<tr("Message")<<tr("Description")<<tr("type")<<"Time");
	logTable->setWordWrap (true);
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 300 mess,details,type,time from c2logGui where messenger='%1' order by tid desc").arg(logGuiName),bDb);
	logTable->setRowCount(dataVV.count());
	for (int i=0;i<dataVV.count();i++)
	{
		logTable->setItem(i,0,new QTableWidgetItem(dataVV[i][0].toString().trimmed()));
		logTable->setItem(i,1,new QTableWidgetItem(dataVV[i][1].toString().trimmed()));
		logTable->setItem(i,2,new QTableWidgetItem(dataVV[i][2].toString().trimmed()));
		logTable->setItem(i,3,new QTableWidgetItem(dataVV[i][3].toString().trimmed()));
		if (dataVV[i][2].toString().trimmed()==errorStr)
		{
			logTable->item(i,0)->setBackground(Qt::red);
			logTable->item(i,1)->setBackground(Qt::red);
			logTable->item(i,2)->setBackground(Qt::red);
			logTable->item(i,3)->setBackground(Qt::red);
		}
	}
	logTable->resizeColumnsToContents();
	dlg->exec();
}
void carriageWidgetClass::loadFromDataBase()
{
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 100 mess,details,type,time from c2logGui where messenger='%1' order by tid desc").arg(logGuiName),bDb);
	for (int i=0;i<dataVV.count();i++)
	{
		QListWidgetItem *item=new QListWidgetItem(dataVV[i][0].toString().trimmed());
		item->setToolTip(QString("%1\n%2\n%3").arg(dataVV[i][2].toString().trimmed()).arg(dataVV[i][1].toString().trimmed()).arg(dataVV[i][3].toString().trimmed()));
		if (dataVV[i][2].toString().trimmed()==errorStr)
			item->setForeground(Qt::red);
		else
			item->setForeground(Qt::blue);
		logGuiList->addItem(item);
	}
}
void carriageWidgetClass::addMessageToGui(QString mess,QString details,QString type)
{
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 1 mess,details,type,time from c2logGui where messenger='%1' order by tid desc").arg(logGuiName),bDb);
	bool isTheSameWithLast=false;
	if (dataVV.count()>0)
	{
		if (  (mess==dataVV[0][0].toString().trimmed())  &&  (details==dataVV[0][1].toString().trimmed())  &&  (type==dataVV[0][2].toString().trimmed())  )   
			isTheSameWithLast=true;
	}
	if (!isTheSameWithLast)
	{
		logGui(mess,details,logGuiName,type);
		QListWidgetItem *item=new QListWidgetItem(mess);
		item->setToolTip(QString("%1\n%2\n%3").arg(type).arg(details).arg(timeForLog));
		if (type==errorStr)
			item->setForeground(Qt::red);
		else
			item->setForeground(Qt::blue);
		logGuiList->insertItem(0,item);
	}
	else
	{
		if (logGuiList->count()>0)
		{
			execQuery(QString("update c2logGui set time='%1' where messenger='%2' and tid in (select max(tid) from c2logGui where messenger='%2')").arg(timeForLog).arg(logGuiName),bDb);
			logGuiList->item(0)->setToolTip(QString("%1\n%2\n%3").arg(type).arg(details).arg(timeForLog));
		}
	}
}
void carriageWidgetClass::setActiveFrom(QString fDescription)
{
	activeFromDescription=fDescription;
	updateGui();
}
void carriageWidgetClass::setActiveTo(QString tDescription)
{
	activeToDescription=tDescription;
	updateGui();
}

void carriageWidgetClass::setNextFrom(QString fDescription)
{
	nextFromDescription=fDescription;
	updateGui();
}
void carriageWidgetClass::setNextTo(QString tDescription)
{
	nextToDescription=tDescription;
	updateGui();
}
void carriageWidgetClass::setActiveCraneStep(int s)
{
	activeCraneStep=s;
	addMessageToGui(QString("Plc step:%1").arg(activeCraneStep),QString("Plc step of active mission has changed:\n%1\n%2").arg(getCraneStepText(activeCraneStep)).arg(getCraneStepDescription(activeCraneStep)),infoStr);
	updateGui();
}
QString carriageWidgetClass::getCraneStepText(int s)
{
	return plcCraneStepTexts.getTextOfIndex(s);
}
QString carriageWidgetClass::getCraneStepDescription(int s)
{
	return plcCraneStepTexts.getDescriptionOfIndex(s);
}
void carriageWidgetClass::updateGui()
{
	activeCraneStepLabel->setText(getCraneStepText(activeCraneStep));
	activeMissionLabel->setText(QString("%1 %2").arg((activeFromDescription.length()>0)?tr("from:%1").arg(activeFromDescription):"").arg((activeToDescription.length()>0)?tr("to:%1").arg(activeToDescription):""));
	nextMissionLabel->setText(QString("%1 %2").arg((nextFromDescription.length()>0)?tr("from:%1").arg(nextFromDescription):"").arg((nextToDescription.length()>0)?tr("to:%1").arg(nextToDescription):""));
}

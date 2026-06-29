#include "pch.h"
#include "crConvW.h"
craneConveyorWidgetClass::craneConveyorWidgetClass(QString logGuiName_,QWidget *p):QGroupBox(p)
{
	logGuiName=logGuiName_;
	plcStepTexts.add(craneConveyorClass::plcStepIdle											,tr("Idle")																					,tr("Plc status:%1\nWithout an active mission").arg(craneConveyorClass::plcStepIdle)	);
	plcStepTexts.add(craneConveyorClass::plcStepStart											,tr("Active mission")																,tr("Plc status:%1\nWith an active mission").arg(craneConveyorClass::plcStepStart)	);
	plcStepTexts.add(craneConveyorClass::plcStepGoToXZLoad								,tr("Go to xz target load position")								,tr("Plc status:%1\nGo to xz target load position").arg(craneConveyorClass::plcStepGoToXZLoad)	);
	plcStepTexts.add(craneConveyorClass::plcStepGoToYLoad									,tr("Hooks out to take cassette")										,tr("Plc status:%1\nHooks out to take cassette").arg(craneConveyorClass::plcStepGoToYLoad)	);
	plcStepTexts.add(craneConveyorClass::plcStepBasketToConveyor					,tr("Take basket to conveyor")											,tr("Plc status:%1\nTake basket to conveyor").arg(craneConveyorClass::plcStepBasketToConveyor)	);
	plcStepTexts.add(craneConveyorClass::plcStepHooksUp										,tr("Hooks up to take cassette")										,tr("Plc status:%1\nHooks up to take cassette").arg(craneConveyorClass::plcStepHooksUp)	);
	plcStepTexts.add(craneConveyorClass::plcStepHooksToCenterWithCassette	,tr("Hooks to center with cassette")								,tr("Plc status:%1\nHooks to center with cassette").arg(craneConveyorClass::plcStepHooksToCenterWithCassette)	);
	plcStepTexts.add(craneConveyorClass::plcStepGoToXZUnLoad							,tr("Go to xz target unload position with cassette"),tr("Plc status:%1\nIs moving with cassette to the x and z p").arg(craneConveyorClass::plcStepGoToXZUnLoad)	);
	plcStepTexts.add(craneConveyorClass::plcStepGoToYUnLoad								,tr("Hooks out to drop cassette")										,tr("Plc status:%1\nHooks out to put cassette into cell").arg(craneConveyorClass::plcStepGoToYUnLoad)	);
	plcStepTexts.add(craneConveyorClass::plcStepHooksDown									,tr("Hooks down")																		,tr("Plc status:%1\nHooks down").arg(craneConveyorClass::plcStepHooksDown)	);
	plcStepTexts.add(craneConveyorClass::plcStepHooksToCenterNoCassette		,tr("Hooks to center without cassette")							,tr("Plc status:%1\nReturn hooks to center").arg(craneConveyorClass::plcStepHooksToCenterNoCassette)	);
	plcStepTexts.add(craneConveyorClass::plcStepFinished									,tr("Mission finished")															,tr("Plc status:%1\nThe mission finished succesfully").arg(craneConveyorClass::plcStepFinished)	);
	activeStep=craneConveyorClass::plcStepIdle;

	setTitle(tr("Cariage"));
	setToolTip(tr("Cariage status"));
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(1);setLayout(mainLayout);
	//active
	QGroupBox *activeGroupBox=new QGroupBox(tr("Active mission"));mainLayout->addWidget(activeGroupBox);//activeGroupBox->setStyleSheet(groupBoxStyle3);
	QVBoxLayout *activeGroupBoxLayout=new QVBoxLayout();activeGroupBox->setLayout(activeGroupBoxLayout);
	activeMissionLabel=new QLabel();activeGroupBoxLayout->addWidget(activeMissionLabel);
	activeStepLabel=new QLabel();activeGroupBoxLayout->addWidget(activeStepLabel);
	//next
	QGroupBox *nextGroupBox=new QGroupBox(tr("Next mission"));mainLayout->addWidget(nextGroupBox);//nextGroupBox->setStyleSheet(groupBoxStyle3);
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
craneConveyorWidgetClass::~craneConveyorWidgetClass()
{
}
void craneConveyorWidgetClass::popUpList()
{
	QDialog *dlg=new QDialog(this);
	dlg->setWindowTitle(tr("Conveyor log"));
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
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select mess,details,type,time from logGui where messenger='%1' order by tId desc limit 300").arg(logGuiName),mDb);
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
void craneConveyorWidgetClass::loadFromDataBase()
{
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select mess,details,type,time from logGui where messenger='%1' order by tId desc limit 100").arg(logGuiName),mDb);
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
void craneConveyorWidgetClass::addMessageToGui(QString mess,QString details,QString type)
{
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select mess,details,type,time from logGui where messenger='%1' order by tId desc limit 1").arg(logGuiName),mDb);
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
			execQuery(QString("update logGui set time='%1' where messenger='%2' and tId=(select m from(select max(tId) as m from logGui where messenger='%2') as t)").arg(timeForLog).arg(logGuiName),mDb);
			logGuiList->item(0)->setToolTip(QString("%1\n%2\n%3").arg(type).arg(details).arg(timeForLog));
		}
	}
}
void craneConveyorWidgetClass::setActiveFrom(QString fDescription)
{
	activeFromDescription=fDescription;
	updateGui();
}
void craneConveyorWidgetClass::setActiveTo(QString tDescription)
{
	activeToDescription=tDescription;
	updateGui();
}
void craneConveyorWidgetClass::setActiveStep(int s)
{
	activeStep=s;
	addMessageToGui(QString("Plc step:%1").arg(activeStep),QString("Plc step of active mission changed:\n%1\n%2").arg(getStepText(activeStep)).arg(getStepDescription(activeStep)),infoStr);
	updateGui();
}
void craneConveyorWidgetClass::setNextFrom(QString fDescription)
{
	nextFromDescription=fDescription;
	updateGui();
}
void craneConveyorWidgetClass::setNextTo(QString tDescription)
{
	nextToDescription=tDescription;
	updateGui();
}
QString craneConveyorWidgetClass::getStepText(int s)
{
	return plcStepTexts.getTextOfIndex(s);
}
QString craneConveyorWidgetClass::getStepDescription(int s)
{
	return plcStepTexts.getDescriptionOfIndex(s);
}
void craneConveyorWidgetClass::updateGui()
{
	activeStepLabel->setText(getStepText(activeStep));
	activeStepLabel->setToolTip(getStepDescription(activeStep));
	activeMissionLabel->setText(QString("%1 %2").arg((activeFromDescription.length()>0)?tr("from:%1").arg(activeFromDescription):"").arg((activeToDescription.length()>0)?tr("to:%1").arg(activeToDescription):""));
	nextMissionLabel->setText(QString("%1 %2").arg((nextFromDescription.length()>0)?tr("from:%1").arg(nextFromDescription):"").arg((nextToDescription.length()>0)?tr("to:%1").arg(nextToDescription):""));
}

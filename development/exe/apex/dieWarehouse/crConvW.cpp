#include "pch.h"
#include "crConvW.h"
craneConveyorWidgetClass::craneConveyorWidgetClass(QWidget *p):QGroupBox(p)
{
	setStyleSheet(groupBoxStyle2);
	logGuiName="craneConveyor";
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepIdle													,tr("Idle")																				,tr("Plc status:%1\nThe conveyor is without an active mission")																				.arg(craneConveyorClass::plcCraneStepIdle)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepStart													,tr("Active mission")															,tr("Plc status:%1\nThe conveyor has an active mission")																							.arg(craneConveyorClass::plcCraneStepStart)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepGoToXZLoad										,tr("Go to xz target load position")							,tr("Plc status:%1\nIs moving to go to the x and z position of the load target cell/position")				.arg(craneConveyorClass::plcCraneStepGoToXZLoad)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepGoToYLoad											,tr("Car idle.Hooks step active.")								,tr("Plc status:%1\nCar idle.Hooks step active.")																.arg(craneConveyorClass::plcCraneStepGoToYLoad)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneHooksUp														,tr("Car idle.Hooks step active.")								,tr("Plc status:%1\nCar idle.Hooks step active.")																			.arg(craneConveyorClass::plcCraneHooksUp)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepBasketToConveyor							,tr("Car idle.Hooks step active.")								,tr("Plc status:%1\nCar idle.Hooks step active.")																															.arg(craneConveyorClass::plcCraneStepBasketToConveyor)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepHooksTakingBasketIn						,tr("Car idle.Hooks step active.")								,tr("Plc status:%1\nCar idle.Hooks step active.")																					.arg(craneConveyorClass::plcCraneStepHooksTakingBasketIn)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepHooksTakingBasketInConveyor		,tr("Car idle.Hooks step active.")								,tr("Plc status:%1\nCar idle.Hooks step active.")																					.arg(craneConveyorClass::plcCraneStepHooksTakingBasketInConveyor)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepHooksTakingBasketInHooksDown	,tr("Car idle.Hooks step active.")							,tr("Plc status:%1\nCar idle.Hooks step active.")																				.arg(craneConveyorClass::plcCraneStepHooksTakingBasketInHooksDown)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepHooksTakingBasketInHooksIn		,tr("Car idle.Hooks step active.")							,tr("Plc status:%1\nCar idle.Hooks step active.")												.arg(craneConveyorClass::plcCraneStepHooksTakingBasketInHooksIn)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepGoToXZUnLoad									,tr("Go to xz target unload position with basket"),tr("Plc status:%1\nIs moving to go to the x and z position of the un load target cell/position")			.arg(craneConveyorClass::plcCraneStepGoToXZUnLoad)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepConveyorMovingBasketToCell		,tr("Car idle.Hooks step active.")								,tr("Plc status:%1\nCar idle.Hooks step active.")																						.arg(craneConveyorClass::plcCraneStepConveyorMovingBasketToCell)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepConveyorMovingBasketOut				,tr("Car idle.Hooks step active. ")							  ,tr("Plc status:%1\nCar idle.Hooks step active.")																			.arg(craneConveyorClass::plcCraneStepConveyorMovingBasketOut)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepHooksDownAnchorsIn						,tr("Car idle.Hooks step active.")								,tr("Plc status:%1\nCar idle.Hooks step active.")																												.arg(craneConveyorClass::plcCraneStepHooksDownAnchorsIn)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepHooksAnchorsIn								,tr("Car idle.Hooks step active.")								,tr("Plc status:%1\nCar idle.Hooks step active.")																				.arg(craneConveyorClass::plcCraneStepHooksAnchorsIn)	);
	plcCraneStepTexts.add(craneConveyorClass::plcCraneStepFinished											,tr("Mission finished")														,tr("Plc status:%1\nThe mission finished succesfully")																								.arg(craneConveyorClass::plcCraneStepFinished)	);
	activeCraneStep=craneConveyorClass::plcHooksStepIdle;

	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepIdle													,tr("Idle")																				,tr("Plc status:%1\nThe conveyor is without an active mission")																				.arg(craneConveyorClass::plcHooksStepIdle)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepStart													,tr("Active mission")															,tr("Plc status:%1\nThe conveyor has an active mission")																							.arg(craneConveyorClass::plcHooksStepStart)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepGoToXZLoad										,tr("Go to xz target load position")							,tr("Plc status:%1\nIs moving to go to the x and z position of the load target cell/position")				.arg(craneConveyorClass::plcHooksStepGoToXZLoad)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepGoToYLoad											,tr("Hooks out to take basket")										,tr("Plc status:%1\nThe hooks are moving to go out to pick the basket")																.arg(craneConveyorClass::plcHooksStepGoToYLoad)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksHooksUp														,tr("Hooks up to take basket")										,tr("Plc status:%1\nThe hooks are moving up and pick the basket")																			.arg(craneConveyorClass::plcHooksHooksUp)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepBasketToConveyor							,tr("Basket to conveyor")													,tr("Plc status:%1\nBasket to conveyor")																															.arg(craneConveyorClass::plcHooksStepBasketToConveyor)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooksTakingBasketIn						,tr("Take basket in with hooks")									,tr("Plc status:%1\nHooks are moving to take the basket on")																					.arg(craneConveyorClass::plcHooksStepHooksTakingBasketIn)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooksTakingBasketInConveyor		,tr("Take basket in with hooks")									,tr("Plc status:%1\nHooks are moving to take the basket on")																					.arg(craneConveyorClass::plcHooksStepHooksTakingBasketInConveyor)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooksTakingBasketInHooksDown	,tr("Take basket in with hooks down")							,tr("Plc status:%1\nHooks took basket in and are moving down")																				.arg(craneConveyorClass::plcHooksStepHooksTakingBasketInHooksDown)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooksTakingBasketInHooksIn		,tr("Take basket in with hooks back in")					,tr("Plc status:%1\nHooks took basket in, are down and are reterning back in")												.arg(craneConveyorClass::plcHooksStepHooksTakingBasketInHooksIn)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooks55												,tr("Hooks down")					,tr("Plc status:%1\nHooks down")												.arg(craneConveyorClass::plcHooksStepHooks55)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooks56												,tr("Hooks travel down under cassette")						,tr("Plc status:%1\nHooks travel down under cassette")												.arg(craneConveyorClass::plcHooksStepHooks56)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooks57												,tr("Hooks up inside the cassette")								,tr("Plc status:%1\nHooks up inside the cassette")												.arg(craneConveyorClass::plcHooksStepHooks57)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooks58												,tr("Hooks move the cassette")										,tr("Plc status:%1\nHooks move the cassette")												.arg(craneConveyorClass::plcHooksStepHooks58)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooks59												,tr("Hooks down")																	,tr("Plc status:%1\nHooks down")												.arg(craneConveyorClass::plcHooksStepHooks59)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooks60												,tr("Hooks travel down under cassette to opposite side")					,tr("Plc status:%1\nHooks travel down under cassette to opposite side")												.arg(craneConveyorClass::plcHooksStepHooks60)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooks61												,tr("Hooks up on opposite side")									,tr("Plc status:%1\nHooks up on opposite side")												.arg(craneConveyorClass::plcHooksStepHooks61)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepGoToXZUnLoad									,tr("Go to xz target unload position with basket"),tr("Plc status:%1\nIs moving to go to the x and z position of the un load target cell/position")			.arg(craneConveyorClass::plcHooksStepGoToXZUnLoad)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepConveyorMovingBasketToCell		,tr("Hooks out pushing basket")										,tr("Plc status:%1\nThe hooks are pushing the basket out")																						.arg(craneConveyorClass::plcHooksStepConveyorMovingBasketToCell)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepConveyorMovingBasketOut				,tr("Conveyor is moving basket out ")							,tr("Plc status:%1\nThe basket is moving out with the conveyor")																			.arg(craneConveyorClass::plcHooksStepConveyorMovingBasketOut)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooksDownAnchorsIn						,tr("Hooks down")																	,tr("Plc status:%1\nThe hooks are moving down")																												.arg(craneConveyorClass::plcHooksStepHooksDownAnchorsIn)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepHooksAnchorsIn								,tr("Hooks down,anchors in")											,tr("Plc status:%1\nThe hooks are moving down,anchors inside")																				.arg(craneConveyorClass::plcHooksStepHooksAnchorsIn)	);
	plcHooksStepTexts.add(craneConveyorClass::plcHooksStepFinished											,tr("Mission finished")														,tr("Plc status:%1\nThe mission finished succesfully")																								.arg(craneConveyorClass::plcHooksStepFinished)	);
	activeCraneStep=craneConveyorClass::plcHooksStepIdle;

	setTitle(tr("Conveyor"));
	setToolTip(tr("Conveyor status"));
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(1);setLayout(mainLayout);
	//active
	QGroupBox *activeGroupBox=new QGroupBox(tr("Active mission"));mainLayout->addWidget(activeGroupBox);activeGroupBox->setStyleSheet(groupBoxStyle3);
	QVBoxLayout *activeGroupBoxLayout=new QVBoxLayout();activeGroupBox->setLayout(activeGroupBoxLayout);
	activeMissionLabel=new QLabel();activeGroupBoxLayout->addWidget(activeMissionLabel);
	activeCraneStepLabel=new QLabel();activeGroupBoxLayout->addWidget(activeCraneStepLabel);
	activeHooksStepLabel=new QLabel();activeGroupBoxLayout->addWidget(activeHooksStepLabel);
	//next
	QGroupBox *nextGroupBox=new QGroupBox(tr("Next mission"));mainLayout->addWidget(nextGroupBox);nextGroupBox->setStyleSheet(groupBoxStyle3);
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
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 300 mess,details,type,time from logGui where messenger='%1' order by tid desc").arg(logGuiName),db);
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
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 100 mess,details,type,time from logGui where messenger='%1' order by tid desc").arg(logGuiName),db);
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
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 1 mess,details,type,time from logGui where messenger='%1' order by tid desc").arg(logGuiName),db);
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
			execQuery(QString("update logGui set time='%1' where messenger='%2' and tid in (select max(tid) from logGui where messenger='%2')").arg(timeForLog).arg(logGuiName),db);
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
void craneConveyorWidgetClass::setActiveCraneStep(int s)
{
	activeCraneStep=s;
	addMessageToGui(QString("Plc step:%1").arg(activeCraneStep),QString("Plc step of active mission has changed:\n%1\n%2").arg(getCraneStepText(activeCraneStep)).arg(getCraneStepDescription(activeCraneStep)),infoStr);
	updateGui();
}
void craneConveyorWidgetClass::setActiveHooksStep(int s)
{
	activeHooksStep=s;
	addMessageToGui(QString("Plc step:%1").arg(activeHooksStep),QString("Plc step of active mission has changed:\n%1\n%2").arg(getHooksStepText(activeHooksStep)).arg(getHooksStepDescription(activeHooksStep)),infoStr);
	updateGui();
}
QString craneConveyorWidgetClass::getHooksStepText(int s)
{
	return plcHooksStepTexts.getTextOfIndex(s);
}
QString craneConveyorWidgetClass::getCraneStepText(int s)
{
	return plcCraneStepTexts.getTextOfIndex(s);
}
QString craneConveyorWidgetClass::getCraneStepDescription(int s)
{
	return plcCraneStepTexts.getDescriptionOfIndex(s);
}
QString craneConveyorWidgetClass::getHooksStepDescription(int s)
{
	return plcHooksStepTexts.getDescriptionOfIndex(s);
}
void craneConveyorWidgetClass::updateGui()
{
	activeCraneStepLabel->setText(getCraneStepText(activeCraneStep));
	activeHooksStepLabel->setToolTip(getHooksStepDescription(activeHooksStep));
	activeMissionLabel->setText(QString("%1 %2").arg((activeFromDescription.length()>0)?tr("from:%1").arg(activeFromDescription):"").arg((activeToDescription.length()>0)?tr("to:%1").arg(activeToDescription):""));
	//
	nextMissionLabel->setText(QString("%1 %2").arg((nextFromDescription.length()>0)?tr("from:%1").arg(nextFromDescription):"").arg((nextToDescription.length()>0)?tr("to:%1").arg(nextToDescription):""));
}

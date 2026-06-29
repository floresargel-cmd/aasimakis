#include "mW.h"

#define inMyLaptop

QVector<int>	*alloyIdArray;			// one on one with alloyDescription
QStringList		*alloyDescription;		// one on one with alloyIdArray
QStringList		*profilePicFileName;	// one on one with profileIntName
QVector<int>	*profileIntName;		// one on one with profilePicFileName
mainWindowClass::mainWindowClass()
{
	setWindowIcon(QIcon(":/logo_16x16.png"));
#ifdef EMULATE
	QMessageBox::critical(0,"Fatal","Emulation mode");
#endif
#ifdef inMyLaptop
	QMessageBox::critical(0,"Fatal","inMyLaptop mode");
#endif
	//
	initializeDatabase();
	//
	QHBoxLayout *mainLayout=new QHBoxLayout();mainLayout->setMargin(0);
	setLayout(mainLayout);
	//
	QWidget *plcWidgetTemp=new QWidget();
	plcWidget=new plcWidgetClass(plcWidgetTemp);
	openGLWidget=new openGLWidgetClass(plcWidget->getCranePlcTcpServer(),plcWidget->getConveyorsPlcTcpServer(),0,0);
	mainLayout->addWidget(openGLWidget);
	plcWidgetTemp->setParent(openGLWidget);
	//
	//
	


	QVBoxLayout *autoGroupBoxLayout = new QVBoxLayout();

	QCheckBox	*beforStackerQCheckBox = new QCheckBox(tr("Before stacker"), this);
	connect(beforStackerQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(beforeStackerAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(beforStackerQCheckBox);
	beforStackerQCheckBox->setChecked(false);
	beforStackerQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa};");

	QCheckBox	*afterStackerQCheckBox=new QCheckBox(tr("After stacker"),this);
	connect(afterStackerQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(afterStackerAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(afterStackerQCheckBox);
	afterStackerQCheckBox->setChecked(false);
	afterStackerQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*unloadOvenQCheckBox=new QCheckBox(tr("Unload oven"),this);
	connect(unloadOvenQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(unloadOvenAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(unloadOvenQCheckBox);
	unloadOvenQCheckBox->setChecked(false);

	unloadOvenQCheckBox->setEnabled(true);

	unloadOvenQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*exportsQCheckBox=new QCheckBox(tr("Exports"),this);
	connect(exportsQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(exportsAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(exportsQCheckBox);
	exportsQCheckBox->setChecked(false);
	exportsQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*returningEmptyQCheckBox=new QCheckBox(tr("Imports"),this);
	connect(returningEmptyQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(returningEmptyAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(returningEmptyQCheckBox);
	returningEmptyQCheckBox->setChecked(false);
	returningEmptyQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*scrapQCheckBox=new QCheckBox(tr("Scrap"),this);
	connect(scrapQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(scrapAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(scrapQCheckBox);
	scrapQCheckBox->setChecked(false);
	scrapQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*fromBufferToOvenQCheckBox=new QCheckBox(tr("From buffer to oven"),this);
	connect(fromBufferToOvenQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(fromBufferToOvenAutoModeSlot(int)));
	autoGroupBoxLayout->addWidget(fromBufferToOvenQCheckBox);
	fromBufferToOvenQCheckBox->setChecked(false);
	fromBufferToOvenQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*exportsBeforeImportsQCheckBox=new QCheckBox(tr("Exports before imports"),this);
	connect(exportsBeforeImportsQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(exportsBeforeImportsCheckBtnSlot(int)));
	autoGroupBoxLayout->addWidget(exportsBeforeImportsQCheckBox);
	exportsBeforeImportsQCheckBox->setChecked(false);
	exportsBeforeImportsQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QHBoxLayout *numberOfEmptyPosInBufferSpinBoxLayout=new QHBoxLayout();autoGroupBoxLayout->addLayout(numberOfEmptyPosInBufferSpinBoxLayout);
	QLabel *numberOfEmptyPosInBufferSpinBoxLabel=new QLabel(tr("Empty buffer"));
	numberOfEmptyPosInBufferSpinBoxLabel->setStyleSheet("QLabel{color:#aaaaaa;}");
	numberOfEmptyPosInBufferSpinBoxLayout->addWidget(numberOfEmptyPosInBufferSpinBoxLabel);
	QSpinBox *numberOfEmptyPosInBufferSpinBox=new QSpinBox();numberOfEmptyPosInBufferSpinBoxLayout->addWidget(numberOfEmptyPosInBufferSpinBox);
	connect(numberOfEmptyPosInBufferSpinBox,SIGNAL(valueChanged(int)),this,SLOT(numberOfEmptyPosInBufferSpinBoxSlot(int)));
	numberOfEmptyPosInBufferSpinBox->setMinimum(3);
	numberOfEmptyPosInBufferSpinBox->setMaximum(7);
	numberOfEmptyPosInBufferSpinBox->setValue(4);
	openGLWidget->setNumberOfPosToBeKeptEmptyInBuffer(numberOfEmptyPosInBufferSpinBox->value());

	QCheckBox	*rearrangeBasketsQCheckBox=new QCheckBox(tr("Rearrange baskets"),this);
	connect(rearrangeBasketsQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(rearrangeBasketsCheckBtnSlot(int)));
	autoGroupBoxLayout->addWidget(rearrangeBasketsQCheckBox);
	rearrangeBasketsQCheckBox->setChecked(false);
	rearrangeBasketsQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");
	
	
	QGroupBox *autoGroupBox=new QGroupBox(tr("Auto mode"),openGLWidget);
	autoGroupBox->setStyleSheet("QGroupBox{color:#aaaaaa;}");
	autoGroupBox->setGeometry(30,20,150,250);
	autoGroupBox->setLayout(autoGroupBoxLayout);
	autoGroupBox->show();
	//
	int w=140;int h=25;int m=5;int x=30;int y=280;

	QCheckBox	*takeBasketsFromStackerOutQCheckBox=new QCheckBox(tr("New baskets out"),openGLWidget);
	connect(takeBasketsFromStackerOutQCheckBox,SIGNAL(stateChanged(int)),this,SLOT(takeBasketsFromStackerOutSlot(int)));
	takeBasketsFromStackerOutQCheckBox->setGeometry(35,y,145,20);
	takeBasketsFromStackerOutQCheckBox 	->show();
	takeBasketsFromStackerOutQCheckBox->setChecked(false);
	takeBasketsFromStackerOutQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa};");
////////////////////
	QVBoxLayout *nextOvenCycleGroupBoxLayout=new QVBoxLayout();

	QRadioButton *nextOvenCycle185RadioButton=new QRadioButton("185C 6 hours",this);
	connect(nextOvenCycle185RadioButton,SIGNAL(toggled(bool)),this,SLOT(nextOvenCycle185RadioButtonSlot(bool)));
	nextOvenCycleGroupBoxLayout->addWidget(nextOvenCycle185RadioButton);
	nextOvenCycle185RadioButton->setStyleSheet("QRadioButton	{color:#aaaaaa;}");

	QRadioButton *nextOvenCycle230RadioButton=new QRadioButton("230C 6 hours",this);
	connect(nextOvenCycle230RadioButton,SIGNAL(toggled(bool)),this,SLOT(nextOvenCycle230RadioButtonSlot(bool)));
	nextOvenCycleGroupBoxLayout->addWidget(nextOvenCycle230RadioButton);
	nextOvenCycle230RadioButton->setStyleSheet("QRadioButton{color:#aaaaaa;}");

	if (openGLWidget->getNextOvenLoadingCycleTemperEnum()==allPosisionsClass::NEXT_OVEN_CYCLE_TEMPER_185)
		nextOvenCycle185RadioButton->setChecked(true);
	else 
		nextOvenCycle230RadioButton->setChecked(true);
	//
	QGroupBox *nextOvenCycleGroupBox=new QGroupBox(tr("Next Oven cycle"),openGLWidget);
	nextOvenCycleGroupBox->setStyleSheet("QGroupBox{color:#aaaaaa;}");
	nextOvenCycleGroupBox->setGeometry(30,y+=25,150,90);
	nextOvenCycleGroupBox->setLayout(nextOvenCycleGroupBoxLayout);
	nextOvenCycleGroupBox->show();

	///////////////////////////////////
	//Added by Vahid
	///////////////////////////////////
	QVBoxLayout *nextOven2CycleGroupBoxLayout = new QVBoxLayout();

	QRadioButton *nextOven2Cycle185RadioButton = new QRadioButton("185C 6 hours", this);
	connect(nextOven2Cycle185RadioButton, SIGNAL(toggled(bool)), this, SLOT(nextOven2Cycle185RadioButtonSlot(bool)));
	nextOven2CycleGroupBoxLayout->addWidget(nextOven2Cycle185RadioButton);
	nextOven2Cycle185RadioButton->setStyleSheet("QRadioButton	{color:#aaaaaa;}");

	QRadioButton *nextOven2Cycle230RadioButton = new QRadioButton("230C 6 hours", this);
	connect(nextOven2Cycle230RadioButton, SIGNAL(toggled(bool)), this, SLOT(nextOven2Cycle230RadioButtonSlot(bool)));
	nextOven2CycleGroupBoxLayout->addWidget(nextOven2Cycle230RadioButton);
	nextOven2Cycle230RadioButton->setStyleSheet("QRadioButton{color:#aaaaaa;}");

	////////////////////////
	//What is next Oven2 Cycle
	//????????????????????????
	//?????????????????????????
	//??????????????????????????
	//////////////////////////////
	if (openGLWidget->getNextOvenLoadingCycleTemperEnum() == allPosisionsClass::NEXT_OVEN_CYCLE_TEMPER_185)
		nextOven2Cycle185RadioButton->setChecked(true);
	else
		nextOven2Cycle230RadioButton->setChecked(true);
	//
	QGroupBox *nextOven2CycleGroupBox = new QGroupBox(tr("Next Oven2 cycle"), openGLWidget);
	nextOven2CycleGroupBox->setStyleSheet("QGroupBox{color:#aaaaaa;}");
	nextOven2CycleGroupBox->setGeometry(200, y , 150, 90);
	nextOven2CycleGroupBox->setLayout(nextOven2CycleGroupBoxLayout);
	nextOven2CycleGroupBox->show();
	///////////////////////////////////
	//End of Code by Vahid
	///////////////////////////////////


////////////////////Use empty buffer
	QVBoxLayout *emptyBasketsPosGroupBoxLayout=new QVBoxLayout();

	useEmptyBasketsPosCheckBox=new QCheckBox(tr("Use"),this);
	useEmptyBasketsPosCheckBox->setChecked(false);
	useEmptyBasketsPosCheckBox->setToolTip(tr("Use the empty buffer area to store 20 feet baskets if necessary."));
	useEmptyBasketsPosCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa};");
	emptyBasketsPosGroupBoxLayout->addWidget(useEmptyBasketsPosCheckBox);
	connect(useEmptyBasketsPosCheckBox,SIGNAL(stateChanged(int)),this,SLOT(useEmptyBasketsPosCheckBoxSlot(int)));

	emptyEmptyBasketsPosCheckBox=new QCheckBox(tr("Empty"),this);
	emptyEmptyBasketsPosCheckBox->setChecked(false);
	emptyEmptyBasketsPosCheckBox->setToolTip(tr("Empty the empty buffer area from 20 feet baskets when you have time."));
	emptyEmptyBasketsPosCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa};");
	emptyBasketsPosGroupBoxLayout->addWidget(emptyEmptyBasketsPosCheckBox);
	connect(emptyEmptyBasketsPosCheckBox,SIGNAL(stateChanged(int)),this,SLOT(emptyEmptyBasketsPosCheckBoxSlot(int)));

	QGroupBox *fullBasketsToEmptyPosGroupBox=new QGroupBox(tr("20feet in empty buffer "),openGLWidget);
	fullBasketsToEmptyPosGroupBox->setStyleSheet("QGroupBox{color:#aaaaaa;}");
	fullBasketsToEmptyPosGroupBox->setGeometry(30,y+=95+m,150,90);
	fullBasketsToEmptyPosGroupBox->setLayout(emptyBasketsPosGroupBoxLayout);
	fullBasketsToEmptyPosGroupBox->show();
	//
	//takeScrapBasketBtn=new QPushButton(tr("Take scrap basket"),openGLWidget);takeScrapBasketBtn->setGeometry(x,y+=95+m,w,h);  takeScrapBasketBtn 	->show();
	y+=95+m;
	QPushButton *unlockBasketAfterStackerBtn=new QPushButton(tr("Unlock after stacker"),openGLWidget);unlockBasketAfterStackerBtn->setGeometry(x,y+=h+m,w,h);  unlockBasketAfterStackerBtn 	->show();
	QPushButton *unlockBasketReturningFromOvenBtn=new QPushButton(tr("Unlock from oven"),openGLWidget);unlockBasketReturningFromOvenBtn->setGeometry(x,y+=h+m,w,h);  unlockBasketReturningFromOvenBtn 	->show();

	plcWidgetTemp->setGeometry(x-15,y+=h+m+50,170,330);plcWidgetTemp->show();

	//connect(takeScrapBasketBtn,SIGNAL(clicked()),this,SLOT(takeScrapBasketBtnSlot()));
	connect(unlockBasketAfterStackerBtn,SIGNAL(clicked()),this,SLOT(unlockBasketAfterStackerBtnSlot()));
	connect(unlockBasketReturningFromOvenBtn,SIGNAL(clicked()),this,SLOT(unlockBasketReturningFromOvenBtnSlot()));
	connect(openGLWidget,SIGNAL(scrapBasketPositionChangedSignal(int)),this,SLOT(scrapBasketPositionChangedSlot(int)));

#ifdef VB_PLC_COMMS
#ifndef EMULATE
	connect(plcWidget->getCranePlcTcpServer(),SIGNAL(incomingSignal(QString,int,QString)),this,SLOT(signalFromCranePlcSlot(QString,int,QString)));
	connect(plcWidget->getConveyorsPlcTcpServer(),SIGNAL(incomingSignal(QString,int,QString)),this,SLOT(signalFromConveyorsPlcSlot(QString,int,QString)));
#endif
#endif
#ifdef TUX_PLC_COMMS
	connect(plcWidget->getCrainTuxClass(),SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromCraneTuxPlcSlot(QString,QString,int,QVariant)));
	connect(plcWidget->getConveyorTuxClass(),SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromConveyorsTuxPlcSlot(QString,QString,int,QVariant)));
	connect(plcWidget->getNewOvenTuxClass(),SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromConveyorsTuxPlcSlot(QString,QString,int,QVariant)));
#endif
	//
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimer()));
	mainTimer.start(1500);
	clearLogTable();
	scrapBasketPositionChangedSlot(openGLWidget->getBasketTableIdOfScrapBasketPos());
	//
	//
#ifdef TUX_PLC_COMMS
	QAction *showServerAction=new QAction("",this);
	addAction(showServerAction);
	showServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	connect(showServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));
#ifdef EMULATE
	plcEmulateWindow=new plcEmulateWindowClass(plcWidget->getCrainTuxClass(),plcWidget->getConveyorTuxClass(),this);
	plcEmulateWindow->show();
#endif
	plcWidget->addTables();
#ifdef EMULATE
	plcEmulateWindow->tuxIsReady();
#endif
#endif
#ifdef EMULATE
	//plcWidget->getCranePlcTcpServer()->sendCurrentValuesToPlc();
	//plcWidget->getConveyorsPlcTcpServer()->sendCurrentValuesToPlc();
#endif;

#ifdef VB_PLC_COMMS
#ifndef EMULATE
	plcWidget->startExternalSoft();
#endif;
#endif;
	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
	
	
	//////////////////////////////////////////////////////
	// Added by Vahid
	//////////////////////////////////////////////////////
	QVBoxLayout *OvenSelGroupBoxLayout = new QVBoxLayout();


	QCheckBox *Oven1EnableQCheckBox = new QCheckBox(tr("Oven1 Enable"), this);
	connect(Oven1EnableQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven1EnableAutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven1EnableQCheckBox);
	Oven1EnableQCheckBox->setChecked(true);
	Oven1EnableQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*Oven2EnableQCheckBox = new QCheckBox(tr("Oven2 Enable"), this);
	connect(Oven2EnableQCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven2EnableAutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven2EnableQCheckBox);
	Oven2EnableQCheckBox->setChecked(true);
	Oven2EnableQCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*Oven1Not24QCheckBox = new QCheckBox(tr("Not longer than 24f in Oven1"), this);
	connect(Oven1Not24QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven1Not24AutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven1Not24QCheckBox);
	Oven1Not24QCheckBox->setChecked(true);
	Oven1Not24QCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QCheckBox	*Oven2Not24QCheckBox = new QCheckBox(tr("Just Longer than 24f in Oven2"), this);
	connect(Oven2Not24QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(Oven2Not24AutoModeSlot(int)));
	OvenSelGroupBoxLayout->addWidget(Oven2Not24QCheckBox);
	Oven2Not24QCheckBox->setChecked(true);
	Oven2Not24QCheckBox->setStyleSheet("QCheckBox{color:#aaaaaa;}");

	QGroupBox *OvenSelGroupBox = new QGroupBox(tr("Ovens Selections"), openGLWidget);
	OvenSelGroupBox->setStyleSheet("QGroupBox{color:#aaaaaa;}");
	OvenSelGroupBox->setGeometry(200, 20, 250, 150);
	OvenSelGroupBox->setLayout(OvenSelGroupBoxLayout);
	OvenSelGroupBox->show();


	//////////////////////////////////////////////////////
	// End of code added by Vahid
	//////////////////////////////////////////////////////

}
mainWindowClass::~mainWindowClass()
{
#ifdef VB_PLC_COMMS
	terminateExternalProccesses();
#endif
	delete plcWidget;
}
void mainWindowClass::signalFromCraneTuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
#ifdef TUX_PLC_COMMS
	signalFromCranePlcSlot(table,index,value.toString());
#endif
#ifdef EMULATE
	plcEmulateWindow->cranePlcDataChanged(table,index,value.toString());
#endif
}
void mainWindowClass::signalFromConveyorsTuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
#ifdef TUX_PLC_COMMS
	signalFromConveyorsPlcSlot(ip,table,index,value.toString());
#endif
#ifdef EMULATE
	plcEmulateWindow->conveyorPlcDataChanged(table,index,value.toString());
#endif
}
void mainWindowClass::showServerSlot()
{
#ifdef TUX_PLC_COMMS
	plcWidget->showTuxServerCommsDialog();
#endif
}
//for backwards compatibility
void mainWindowClass::signalFromCranePlcSlot(QString table,int index,QString value)
{
	if (table=="N20")
		execQuery(QString("insert into logCranePlc (address,value,X,Y,Z,date,sec) values('%1[%2]',%3,%4,%5,%6,'%7',%8)")
		.arg(table).arg(index).arg(value).arg(openGLWidget->getCraneX()).arg(openGLWidget->getCraneY()).arg(openGLWidget->getCraneZ()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()),MAIN_DB_CONNECTION);

	openGLWidget->signalFromCranePlc(table,index,value);
	plcWidget->signalFromCranePlc(table,index,value);
	if (table=="N20")
	{
		if (index==22)execQuery(QString("insert into logResetPlc (type,value,date,sec) values('Z',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()),MAIN_DB_CONNECTION);
		if (index==23)execQuery(QString("insert into logResetPlc (type,value,date,sec) values('X',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()),MAIN_DB_CONNECTION);
		if (index==24)execQuery(QString("insert into logResetPlc (type,value,date,sec) values('Y',%1,'%2',%3)").arg(value).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()),MAIN_DB_CONNECTION);
	}
}
//for backwards compatibility
void mainWindowClass::signalFromConveyorsPlcSlot(QString ip,QString table,int index,QString value)
{
	if (ip==conveyorsIp)
		openGLWidget->signalFromConveyorsPlc(table,index,value);
	if (ip==newOvenIp)
		openGLWidget->signalFromNewOvenConveyorsPlc(table,index,value);
}
void mainWindowClass::clearLogTable()
{
#ifndef EMULATE
	execQuery(QString("delete from basketHistory where id not in(select top 100000 id from basketHistory order by sec desc)"),MAIN_DB_CONNECTION);
#endif
}
void mainWindowClass::takeScrapBasketBtnSlot()
{
		openGLWidget->takeScrapBasket();
}
void mainWindowClass::unlockBasketAfterStackerBtnSlot()
{
		openGLWidget->unlockBasketAfterStacker();
}
void mainWindowClass::unlockBasketReturningFromOvenBtnSlot()
{
		openGLWidget->unlockBasketReturningFromOven();
}
void mainWindowClass::scrapBasketPositionChangedSlot(int basketTableId)
{
	//if (basketTableId>0)
	//	takeScrapBasketBtn->setEnabled(true);
	//else
	//	takeScrapBasketBtn->setEnabled(false);
}
void mainWindowClass::takeBasketsFromStackerOutSlot(int checkState)
{
	if (checkState==Qt::Unchecked)
		openGLWidget->setTakeBasketsFromStackerOut(false);
	if (checkState==Qt::Checked)
		openGLWidget->setTakeBasketsFromStackerOut(true);
}
void mainWindowClass::beforeStackerAutoModeSlot(int checkState)
{
	if (checkState==Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_BEFORE_STACKER);
	if (checkState==Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_BEFORE_STACKER);
}
void mainWindowClass::afterStackerAutoModeSlot(int checkState)
{
	if (checkState==Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_AFTER_STACKER);
	if (checkState==Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_AFTER_STACKER);
}

//////////////////////////////////////////////////////
// Added by Vahid
//////////////////////////////////////////////////////
void mainWindowClass::Oven1EnableAutoModeSlot(int checkState)
{
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_OVEN1_ENABLE);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_OVEN1_ENABLE);
}

void mainWindowClass::Oven2EnableAutoModeSlot(int checkState)
{
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_OVEN2_ENABLE);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_OVEN2_ENABLE);
}

void mainWindowClass::Oven1Not24AutoModeSlot(int checkState)
{
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_OVEN1_NOT24);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_OVEN1_NOT24);
}

void mainWindowClass::Oven2Not24AutoModeSlot(int checkState)
{
	if (checkState == Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_OVEN2_ONLY24);
	if (checkState == Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_OVEN2_ONLY24);
}

void mainWindowClass::nextOven2Cycle230RadioButtonSlot(bool state)
{
	if (state)
		openGLWidget->setNextOven2LoadingCycleTemperEnum(allPosisionsClass::NEXT_OVEN2_CYCLE_TEMPER_230);
}

void mainWindowClass::nextOven2Cycle185RadioButtonSlot(bool state)
{
	if (state)
		openGLWidget->setNextOven2LoadingCycleTemperEnum(allPosisionsClass::NEXT_OVEN2_CYCLE_TEMPER_185);
}
//////////////////////////////////////////////////////
// End of Code Added by Vahid
//////////////////////////////////////////////////////

void mainWindowClass::unloadOvenAutoModeSlot(int checkState)
{
	if (checkState==Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_UNLOAD_OVEN);
	if (checkState==Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_UNLOAD_OVEN);
}
void mainWindowClass::exportsAutoModeSlot(int checkState)
{
	if (checkState==Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_EXPORTS);
	if (checkState==Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_EXPORTS);
}
void mainWindowClass::returningEmptyAutoModeSlot(int checkState)
{
	if (checkState==Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_RETURNING_EMPTY);
	if (checkState==Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_RETURNING_EMPTY);
}
void mainWindowClass::scrapAutoModeSlot(int checkState)
{
	if (checkState==Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_SCRAP);
	if (checkState==Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_SCRAP);
}
void mainWindowClass::fromBufferToOvenAutoModeSlot(int checkState)
{
	if (checkState==Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_FROM_BUFFER_TO_OVEN);
	if (checkState==Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_FROM_BUFFER_TO_OVEN);
}
void mainWindowClass::exportsBeforeImportsCheckBtnSlot(int checkState)
{
	openGLWidget->setExportBeforeImports(checkState==Qt::Checked);
}
void mainWindowClass::rearrangeBasketsCheckBtnSlot(int checkState)
{
	if (checkState==Qt::Unchecked)
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_MOVE_24F_FROM30F_TO_24F_POS);
	if (checkState==Qt::Checked)
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_MOVE_24F_FROM30F_TO_24F_POS);
}
void mainWindowClass::numberOfEmptyPosInBufferSpinBoxSlot(int n)
{
	openGLWidget->setNumberOfPosToBeKeptEmptyInBuffer(n);
}
void mainWindowClass::nextOvenCycle230RadioButtonSlot(bool state)
{
	if (state)
		openGLWidget->setNextOvenLoadingCycleTemperEnum(allPosisionsClass::NEXT_OVEN_CYCLE_TEMPER_230);
}
void mainWindowClass::nextOvenCycle185RadioButtonSlot(bool state)
{
	if (state)
		openGLWidget->setNextOvenLoadingCycleTemperEnum(allPosisionsClass::NEXT_OVEN_CYCLE_TEMPER_185);
}
void mainWindowClass::emptyEmptyBasketsPosCheckBoxSlot(int checkState)
{
	if (checkState==Qt::Checked)
	{
		useEmptyBasketsPosCheckBox->setChecked(false);
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_EMPTY_EMPTYBASKETS_POS);
	}
	else
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_EMPTY_EMPTYBASKETS_POS);
}
void mainWindowClass::useEmptyBasketsPosCheckBoxSlot(int checkState)
{
	if (checkState==Qt::Checked)
	{
		emptyEmptyBasketsPosCheckBox->setChecked(false);
		openGLWidget->addToAutoMode(allPosisionsClass::AUTO_USE_EMPTYBASKETS_POS);
	}
	else
		openGLWidget->removeFromAutoMode(allPosisionsClass::AUTO_USE_EMPTYBASKETS_POS);
}
void mainWindowClass::plcDlgSlot()
{
	plcWidget->show();
}
void mainWindowClass::onMainTimer()
{
	setWindowTitle(tr("apex basketCrane, assimakis sa ","main window title..")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss"));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////database///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool tableExists(QString tableName,QString connection)
{
#ifdef EMULATE
	QVector<QVariant> tableArray=execQuery(QString("SELECT name FROM sqlite_master WHERE name='")+tableName+"'",connection);
	if (tableArray.count()==0)//table does not exist 
		return false;
	return true;
#else
	QVector<QVariant> tableArray=execQuery(QString("SELECT [name] FROM sys.tables WHERE [name] ='")+tableName+"'",connection);
	if (tableArray.count()==0)//table does not exist 
		return false;
	return true;
#endif
}
void createTable(QString tableName,QStringList colNames,QStringList colTypes,QString connection)
{
#ifdef EMULATE
	QVector<QVariant> tableArray=execQuery(QString("SELECT name FROM sqlite_master WHERE name='")+tableName+"'",connection);
	if (!tableExists(tableName,connection))//table does not exist 
	{	
		QString stmnt=QString("CREATE TABLE ")+tableName+" (";
		for (int i=0;i<colNames.length();i++)
		{
			stmnt+=colNames[i]+" "+colTypes[i];
			if (i!=(colNames.length()-1))
				stmnt+=",";
		}
		stmnt+=")";
		execQuery(stmnt,connection);
	}
#else
	QString stmnt=QString("IF NOT EXISTS (SELECT [name] FROM sys.tables WHERE [name] ='")+tableName+"') ";
	stmnt+=QString("CREATE TABLE ")+tableName+" (";
	for (int i=0;i<colNames.length();i++)
	{
		stmnt+=colNames[i]+" "+colTypes[i];
		if (i!=(colNames.length()-1))
			stmnt+=",";
	}
	stmnt+=")";
	execQuery(stmnt,connection);
#endif
}
#ifdef EMULATE
void addEmptyBasket(int posNumber,int posIndex,int basketNumber)
{
	execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(posNumber).arg(posIndex).arg(basketNumber).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
	//execQuery(QString("insert into baskets (number,type) values(%1,%2)").arg(basketNumber).arg(BASKET_NORMAL),MAIN_DB_CONNECTION);
}
void addFullBasket(int posNumber,int posIndex,int basketNumber,QString description,int typeOfProfiles)
{
	execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('%1',%2,%3,%4,%5,0,0)").arg(description).arg(posNumber).arg(posIndex).arg(basketNumber).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
	//execQuery(QString("insert into baskets (number,type) values(%1,%2)").arg(basketNumber).arg(BASKET_NORMAL),MAIN_DB_CONNECTION);
	int noOfProfiles=1+static_cast<double>(4.0)*(static_cast<double>(rand())/static_cast<double>(RAND_MAX));
	for (int i=0;i<noOfProfiles;i++)
	{
		double length=0;
		if (typeOfProfiles==BASKET_CONTENS_LESS_THAN_20)
			length=3.+static_cast<double>(3.)*static_cast<double>(rand())/static_cast<double>(RAND_MAX);
		else if (typeOfProfiles==BASKET_CONTENS_LESS_THAN_24)
			length=3.+static_cast<double>(4.3)*static_cast<double>(rand())/static_cast<double>(RAND_MAX);
		else if (typeOfProfiles==BASKET_CONTENS_LESS_THAN_30)
			length=length=3.+static_cast<double>(6.0)*static_cast<double>(rand())/static_cast<double>(RAND_MAX);
		int no=10+static_cast<double>(100.0)*(static_cast<double>(rand())/static_cast<double>(RAND_MAX));
		if (basketNumber>0)
			execQuery(QString("insert into basket_details (basketnum,dienum,Pc,PcLen,temper) values(%1,%2,%3,%4,'%5')").arg(basketNumber).arg(rand()).arg(no).arg(length*1000.).arg("T1"),APEX_DB_CONNECTION);
	}
}

#endif
void mainWindowClass::demoFillTables(bool force)
{	
#ifdef EMULATE
	//positions
	QDialog *dlg=new QDialog(this);
	dlg->setWindowTitle("Demo fill the area with baskets.");
	QGridLayout *dlgMainLayout=new QGridLayout();dlg->setLayout(dlgMainLayout);
	
	dlgMainLayout->addWidget(new QLabel("Number of empty baskets:"),dlgMainLayout->rowCount(),0);
	QSpinBox *emptyBasketsSpinBox=new QSpinBox();emptyBasketsSpinBox->setMinimum(0);emptyBasketsSpinBox->setMaximum(20);emptyBasketsSpinBox->setValue(10);
	dlgMainLayout->addWidget(emptyBasketsSpinBox,dlgMainLayout->rowCount()-1,1);

	dlgMainLayout->addWidget(new QLabel("Number of full baskets:"),dlgMainLayout->rowCount(),0);
	QSpinBox *fullBasketsSpinBox=new QSpinBox();fullBasketsSpinBox->setMinimum(0);fullBasketsSpinBox->setMaximum(20);fullBasketsSpinBox->setValue(10);
	dlgMainLayout->addWidget(fullBasketsSpinBox,dlgMainLayout->rowCount()-1,1);

	dlgMainLayout->addWidget(new QLabel("Number of baskets before oven:"),dlgMainLayout->rowCount(),0);
	QSpinBox *basketsBeforeOvenSpinBox=new QSpinBox();basketsBeforeOvenSpinBox->setMinimum(0);basketsBeforeOvenSpinBox->setMaximum(20);basketsBeforeOvenSpinBox->setValue(6);
	dlgMainLayout->addWidget(basketsBeforeOvenSpinBox,dlgMainLayout->rowCount()-1,1);

	dlgMainLayout->addWidget(new QLabel("Number of baskets in oven:"),dlgMainLayout->rowCount(),0);
	QSpinBox *basketsInOvenSpinBox=new QSpinBox();basketsInOvenSpinBox->setMinimum(0);basketsInOvenSpinBox->setMaximum(16);basketsInOvenSpinBox->setValue(16);
	dlgMainLayout->addWidget(basketsInOvenSpinBox,dlgMainLayout->rowCount()-1,1);

	dlgMainLayout->addWidget(new QLabel("Number of baskets after oven:"),dlgMainLayout->rowCount(),0);
	QSpinBox *basketsAfterOvenSpinBox=new QSpinBox();basketsAfterOvenSpinBox->setMinimum(0);basketsAfterOvenSpinBox->setMaximum(36);basketsAfterOvenSpinBox->setValue(30);
	dlgMainLayout->addWidget(basketsAfterOvenSpinBox,dlgMainLayout->rowCount()-1,1);


	QPushButton *cancelBtn=new QPushButton(tr("Cancel"));
	QPushButton *okBtn=new QPushButton(tr("Reset"));
	okBtn->setDefault(true);
	dlgMainLayout->addWidget(cancelBtn,dlgMainLayout->rowCount(),0);
	dlgMainLayout->addWidget(okBtn,dlgMainLayout->rowCount()-1,1);
	connect(cancelBtn,SIGNAL(clicked()),dlg,SLOT(reject()));
	connect(okBtn,SIGNAL(clicked()),dlg,SLOT(accept()));
	if (force)
		dlg->exec();
	else
	{
		if (dlg->exec()!=QDialog::Accepted)
			return;
	}
	if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='positions'"),MAIN_DB_CONNECTION).count()>0)
		execQuery(QString("delete from positions"),MAIN_DB_CONNECTION);
	if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='temperTypes'"),MAIN_DB_CONNECTION).count()>0)
		execQuery(QString("delete from temperTypes"),MAIN_DB_CONNECTION);
	if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='toAssimakisExports'"),MAIN_DB_CONNECTION).count()>0)
		execQuery(QString("delete from toAssimakisExports"),MAIN_DB_CONNECTION);
	if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='toOmavExports'"),MAIN_DB_CONNECTION).count()>0)
		execQuery(QString("delete from toOmavExports"),MAIN_DB_CONNECTION);
	if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='basket_details'"),APEX_DB_CONNECTION).count()>0)
		execQuery(QString("delete from basket_details"),APEX_DB_CONNECTION);
	if (execQuery(QString("SELECT name FROM sqlite_master WHERE name='basket_location'"),APEX_DB_CONNECTION).count()>0)
		execQuery(QString("delete from basket_location"),APEX_DB_CONNECTION);
	if (execQuery(QString("SELECT name FROM sqlite_master WHERE name ='positions'"),EMULATE_DB_CONNECTION).count()>0)
		execQuery(QString("drop table positions"),EMULATE_DB_CONNECTION);
	if (execQuery(QString("SELECT name FROM sqlite_master WHERE name ='baskets'"),MAIN_DB_CONNECTION).count()>0)
	{
		execQuery(QString("drop table baskets"),MAIN_DB_CONNECTION);
#ifdef EMULATE
	QString IDENTITY_STR="autoincrement";
#else
	QString IDENTITY_STR="IDENTITY";
#endif
		createTable("baskets",QStringList()<<"pid"<<"number"<<"type"<<"secFilled",QStringList()<<"integer PRIMARY KEY "+IDENTITY_STR<<"int"<<"int"<<"int",MAIN_DB_CONNECTION);
	}
	//
	if (execQuery(QString("SELECT temper FROM temperTypes WHERE temper='T1'"),MAIN_DB_CONNECTION).count()==0)
		execQuery(QString("insert into temperTypes (temper,temperType) values('T1',%1)").arg(basketClass::BASKET_TEMPER_185),MAIN_DB_CONNECTION);
	//
	int basketNumber=1;
	//emptyBasketsSpinBox
	int emptyBasket=0;
	int pos4[]={300,310,320,330,340,350,360,370,380,390};
	int count=0;
	for (int i=1;i<100;i++)
	{
		execQuery(QString("insert into basket_location (basketnum,basketlocation,audittime) values(%1,1,null)").arg(i),APEX_DB_CONNECTION);
		execQuery(QString("insert into basket_location (basketnum,basketlocation,audittime) values(%1,2,null)").arg(i),APEX_DB_CONNECTION);
		execQuery(QString("insert into basket_location (basketnum,basketlocation,audittime) values(%1,3,null)").arg(i),APEX_DB_CONNECTION);
	}
	for (int i=0;i<10;i++)
	{
		for (int j=1;j<=4;j++)
		{
			emptyBasket++;;
			addEmptyBasket(pos4[i],j,emptyBasket<emptyBasketsSpinBox->value()?basketNumber++:0);
		}
	}
	//fullBasketsSpinBox
	int fullBasket=0;
	fullBasket++;addFullBasket(200,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(200,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(200,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(200,4,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	
	fullBasket++;addFullBasket(210,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(210,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(210,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(210,4,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);

	fullBasket++;addFullBasket(220,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(220,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(220,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(220,4,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);

	fullBasket++;addFullBasket(230,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(230,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(230,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	
	fullBasket++;addFullBasket(240,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(240,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(240,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);

	fullBasket++;addFullBasket(250,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(250,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(250,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);
	
	fullBasket++;addFullBasket(260,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(260,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(260,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"24 feet",BASKET_CONTENS_LESS_THAN_24);
	
	fullBasket++;addFullBasket(270,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(270,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(270,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
		
	fullBasket++;addFullBasket(280,1,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_24);
	fullBasket++;addFullBasket(280,2,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	fullBasket++;addFullBasket(280,3,fullBasket<fullBasketsSpinBox->value()?basketNumber++:0,"30 feet",BASKET_CONTENS_LESS_THAN_30);
	//basketsBeforeOvenSpinBox
	int basketsBeforeOven=0;
	basketsBeforeOven++;addFullBasket(130,1,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(130,2,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(130,3,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(130,4,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);

	basketsBeforeOven++;addFullBasket(120,1,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(120,2,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(120,3,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(120,4,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);

	basketsBeforeOven++;addFullBasket(110,1,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(110,2,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(110,3,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(110,4,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);

	basketsBeforeOven++;addFullBasket(100,1,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(100,2,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(100,3,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);
	basketsBeforeOven++;addFullBasket(100,4,basketsBeforeOven<basketsBeforeOvenSpinBox->value()?basketNumber++:0,"before oven",BASKET_CONTENS_LESS_THAN_24);



	basketsBeforeOven++;addFullBasket(140,1,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(140,2,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(140,3,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(140,4,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);

	basketsBeforeOven++;addFullBasket(150,1,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(150,2,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(150,3,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	basketsBeforeOven++;addFullBasket(150,4,0,"before oven x 2",BASKET_CONTENS_LESS_THAN_30);
	//basketsInOvenSpinBox
	int basketsInOven=0;
	basketsInOven++;addFullBasket(600,1,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(600,2,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(600,3,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(600,4,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(610,1,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(610,2,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(610,3,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(610,4,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(620,1,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(620,2,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(620,3,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(620,4,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(630,1,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(630,2,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(630,3,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	basketsInOven++;addFullBasket(630,4,basketsInOven<basketsInOvenSpinBox->value()?basketNumber++:0,"in oven",BASKET_CONTENS_LESS_THAN_24);
	addFullBasket(640,1,0,"before ove x 2",0);
	addFullBasket(640,2,0,"before ove x 2",0);
	addFullBasket(640,3,0,"before ove x 2",0);
	addFullBasket(640,4,0,"before ove x 2",0);
	addFullBasket(650,1,0,"before ove x 2",0);
	addFullBasket(650,2,0,"before ove x 2",0);
	addFullBasket(650,3,0,"before ove x 2",0);
	addFullBasket(650,4,0,"before ove x 2",0);

	//basketsAfterOvenSpinBox
	int basketsAfterOven=0;
	basketsAfterOven++;addFullBasket(660,1,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(660,2,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(660,3,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(660,4,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(670,1,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(670,2,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(670,3,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(670,4,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(680,1,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(680,2,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(680,3,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(680,4,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(690,1,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(690,2,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(690,3,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(690,4,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);

	basketsAfterOven++;addFullBasket(700,1,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(700,2,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(700,3,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(700,4,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(710,1,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(710,2,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(710,3,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(710,4,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	addFullBasket(730,1,0,"after oven x 2",0);
	addFullBasket(730,2,0,"after oven x 2",0);
	addFullBasket(730,3,0,"after oven x 2",0);
	addFullBasket(730,4,0,"after oven x 2",0);
	basketsAfterOven++;addFullBasket(720,1,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(720,2,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(720,3,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(720,4,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(160,1,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(160,2,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(160,3,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);
	basketsAfterOven++;addFullBasket(160,4,basketsAfterOven<basketsAfterOvenSpinBox->value()?basketNumber++:0,"after oven",BASKET_CONTENS_LESS_THAN_24);

	addFullBasket(1,1,0,"crane",0);
	addFullBasket(400,1,0,"before stacker",0);
	addFullBasket(900,1,0,"stacker",0);
	addFullBasket(410,1,0,"after stacker",0);
	addFullBasket(420,1,0,"scrap",0);
	addFullBasket(520,1,0,"to destacker 2",0);
	addFullBasket(510,1,0,"to destacker 1",0);
	addFullBasket(500,1,0,"from destacker",0);
	for (int j=1;j<=1000;j++)
		execQuery(QString("insert into baskets (number,type) values(%1,%2)").arg(j).arg(BASKET_NORMAL),MAIN_DB_CONNECTION);
#endif
}
void mainWindowClass::initializeDatabase()
{
#ifdef EMULATE
	bool forceDemoFillTables=false;
#endif
	//MAIN_DB_CONNECTION
#ifdef inMyLaptop
	#ifdef EMULATE
	QString a=QDir::current().absolutePath();
		if (!QFile(QDir::current().absolutePath()+"/emulationBasketCrane.db").exists())forceDemoFillTables=true;
		QSqlDatabase mainDb=QSqlDatabase::addDatabase("QSQLITE",MAIN_DB_CONNECTION);
		mainDb.setDatabaseName(QDir::current().absolutePath()+"/emulationBasketCrane.db");//odbc name
		if (!mainDb.open()) 
			EXIT(tr("Error in database"),mainDb.lastError().text());
	#else
		QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
		mainDb.setDatabaseName("local");//odbc name
		mainDb.setUserName("sa"); 
		mainDb.setPassword("123");
		if (!mainDb.open()) 
			EXIT(tr("Error in database"),mainDb.lastError().text());
		execQuery("if not exists(select * from sys.databases where name='basketCraneData') create database basketCraneData",MAIN_DB_CONNECTION);
		execQuery("USE basketCraneData",MAIN_DB_CONNECTION);
	#endif
#else
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("apexBasketCraneMainOdbc");//odbc name
	mainDb.setUserName("sa"); 
	mainDb.setPassword("123");
	if (!mainDb.open()) 
		EXIT(tr("Error in database"),mainDb.lastError().text());
	execQuery("if not exists(select * from sys.databases where name='basketCraneData') create database basketCraneData",MAIN_DB_CONNECTION);
	execQuery("USE basketCraneData",MAIN_DB_CONNECTION);
#endif
	///////////////////////////////////////////////////////////APEX_DB_CONNECTION
#ifdef inMyLaptop
	#ifdef EMULATE
		if (!QFile(QDir::current().absolutePath()+"/emulationApex.db").exists())forceDemoFillTables=true;
		QSqlDatabase apexDb=QSqlDatabase::addDatabase("QSQLITE",APEX_DB_CONNECTION);
		apexDb.setDatabaseName(QDir::current().absolutePath()+"/emulationApex.db");//odbc name
		if (!apexDb.open()) 
			EXIT(tr("Error in database"),apexDb.lastError().text());
	#else
		QSqlDatabase apexDb=QSqlDatabase::addDatabase("QODBC",APEX_DB_CONNECTION);
		apexDb.setDatabaseName("local");//odbc name
		apexDb.setUserName("sa"); 
		apexDb.setPassword("123");
		if (!apexDb.open()) 
			EXIT(tr("Error in database"),apexDb.lastError().text());
		execQuery("if not exists(select * from sys.databases where name='PEManager_tmp') create database PEManager_tmp",APEX_DB_CONNECTION);
		execQuery("USE PEManager_tmp",APEX_DB_CONNECTION);
	#endif
#else
	QSqlDatabase apexDb=QSqlDatabase::addDatabase("QODBC",APEX_DB_CONNECTION);
	apexDb.setDatabaseName("apexEpicsOdbc");//odbc name
	apexDb.setUserName("peadm2"); 
	apexDb.setPassword("peadm2");
		execQuery("USE PEManager_tmp",APEX_DB_CONNECTION);
#endif
	///////////////////////////////////////EMULATE_DB_CONNECTION
#ifdef EMULATE
	if (!QFile(QDir::current().absolutePath()+"/emulationPlc.db").exists())forceDemoFillTables=true;
	QSqlDatabase emulateDb = QSqlDatabase::addDatabase("QSQLITE",EMULATE_DB_CONNECTION);
	emulateDb.setDatabaseName(QDir::current().absolutePath()+"/emulationPlc.db");//odbc name
	if (!emulateDb.open()) 
		EXIT(tr("Error in database"),emulateDb.lastError().text());
#endif
	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
#ifdef EMULATE
	QString IDENTITY_STR="autoincrement";
#else
	QString IDENTITY_STR="IDENTITY";
#endif
	//checkCreate tables
	QStringList colNames;QStringList colTypes;
	//positionsTable
	colNames.clear();colTypes.clear();
	colNames<<"pid"											<<"description"<<"posNumber" <<"posIndex" <<"basketTableId"	<<"locked"<<"targetNumber" <<"targetIndex" ;
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(20)"		<<"int"				<<"int"			 <<"int"						<<"int"   <<"int"					<<"int"   ;
	createTable("positions",colNames,colTypes,MAIN_DB_CONNECTION);
	if (getFirst(QString("select count(pid) from positions"),MAIN_DB_CONNECTION).toInt()==0)
	{
		//empty
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(300).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(300).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(300).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(300).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(310).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(310).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(310).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(310).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(320).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(320).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(320).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(320).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(330).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(330).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(330).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(330).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(340).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(340).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(340).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(340).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(350).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(350).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(350).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(350).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(360).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(360).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(360).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(360).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(370).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(370).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(370).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(370).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(380).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(380).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(380).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(380).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(390).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(390).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(390).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('empty',%1,%2,%3,%4,0,0)").arg(390).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		//full
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(200).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(200).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(200).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(200).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(210).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(210).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(210).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(210).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(220).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(220).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(220).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(220).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(230).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(230).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(230).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(240).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(240).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(240).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
	
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(250).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(250).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(250).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
	
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(260).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(260).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(260).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
	
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(270).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(270).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('24 feet',%1,%2,%3,%4,0,0)").arg(270).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(280).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(280).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('30 feet',%1,%2,%3,%4,0,0)").arg(280).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		//oven
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(130).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(130).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(130).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(130).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(120).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(120).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(120).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(120).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(110).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(110).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(110).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(110).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(100).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(100).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(100).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven',%1,%2,%3,%4,0,0)").arg(100).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven x 2',%1,%2,%3,%4,0,0)").arg(140).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven x 2',%1,%2,%3,%4,0,0)").arg(140).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven x 2',%1,%2,%3,%4,0,0)").arg(140).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven x 2',%1,%2,%3,%4,0,0)").arg(140).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven x 2',%1,%2,%3,%4,0,0)").arg(150).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven x 2',%1,%2,%3,%4,0,0)").arg(150).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven x 2',%1,%2,%3,%4,0,0)").arg(150).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before oven x 2',%1,%2,%3,%4,0,0)").arg(150).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(600).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(600).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(600).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(600).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(610).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(610).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(610).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(610).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(620).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(620).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(620).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(620).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(630).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(630).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(630).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in oven',%1,%2,%3,%4,0,0)").arg(630).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before ove x 2',%1,%2,%3,%4,0,0)").arg(640).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before ove x 2',%1,%2,%3,%4,0,0)").arg(640).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before ove x 2',%1,%2,%3,%4,0,0)").arg(640).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before ove x 2',%1,%2,%3,%4,0,0)").arg(640).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before ove x 2',%1,%2,%3,%4,0,0)").arg(650).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before ove x 2',%1,%2,%3,%4,0,0)").arg(650).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before ove x 2',%1,%2,%3,%4,0,0)").arg(650).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before ove x 2',%1,%2,%3,%4,0,0)").arg(650).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(660).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(660).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(660).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(660).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(670).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(670).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(670).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(670).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(680).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(680).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(680).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(680).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(690).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(690).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(690).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(690).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(700).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(700).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(700).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(700).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(710).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(710).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(710).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(710).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven x 2',%1,%2,%3,%4,0,0)").arg(730).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven x 2',%1,%2,%3,%4,0,0)").arg(730).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven x 2',%1,%2,%3,%4,0,0)").arg(730).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven x 2',%1,%2,%3,%4,0,0)").arg(730).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(720).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(720).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(720).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(720).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(160).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(160).arg(2).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(160).arg(3).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after oven',%1,%2,%3,%4,0,0)").arg(160).arg(4).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);

		for (int b=800;b<=880;b+=20)
		{
			for (int r=1;r<=4;r++)
				execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('Empty',%1,%2,%3,%4,0,0)").arg(b).arg(r).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		}
		for (int r=1;r<=4;r++)
			execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('Full',%1,%2,%3,%4,0,0)").arg(290).arg(r).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		//var
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('crane',%1,%2,%3,%4,0,0)").arg(1).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('before stacker',%1,%2,%3,%4,0,0)").arg(400).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('stacker',%1,%2,%3,%4,0,0)").arg(900).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after stacker',%1,%2,%3,%4,0,0)").arg(410).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('scrap',%1,%2,%3,%4,0,0)").arg(420).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('to destacker 2',%1,%2,%3,%4,0,0)").arg(520).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('to destacker',%1,%2,%3,%4,0,0)").arg(510).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('from destacker',%1,%2,%3,%4,0,0)").arg(500).arg(1).arg(0).arg(basketPositionClass::UNLOCKED),MAIN_DB_CONNECTION);
	}
	//basketsTable
	colNames.clear();colTypes.clear();
	colNames<<"pid"														<<"number"	<<"type"<<"secFilled"	;
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR	<<"int"			<<"int" <<"int"				;
	createTable("baskets",colNames,colTypes,MAIN_DB_CONNECTION);
	//basketHistory
	colNames.clear();colTypes.clear();
	colNames<<"id"											 <<"type"		<<"position"<<"destination"<<"basketTableId"<<"date"	  <<"sec";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(10)"<<"char(40)"<<"char(40)"   <<"int"					<<"char(20)"<<"int";
	createTable("basketHistory",colNames,colTypes,MAIN_DB_CONNECTION);
	//toAssimakisExports
	colNames.clear();colTypes.clear();
	colNames<<"id"											 <<"basketTableId"<<"priority";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"int"						<<"int"			;
	createTable("toAssimakisExports",colNames,colTypes,MAIN_DB_CONNECTION);
	//toOmavExports
	colNames.clear();colTypes.clear();
	colNames<<"id"											 <<"basketTableId"<<"priority";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"int"		        <<"int"			;
	createTable("toOmavExports",colNames,colTypes,MAIN_DB_CONNECTION);
	//temperTypes
	colNames.clear();colTypes.clear();		///T52 ...   basketClass::temperEnum BASKET_TEMPER_000,BASKET_TEMPER_185,BASKET_TEMPER_230
	colNames<<"id"											 <<"temper"  <<"temperType";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(50)"<<"int"			  ;
	createTable("temperTypes",colNames,colTypes,MAIN_DB_CONNECTION);
	//ovenLoads
	colNames.clear();colTypes.clear();		
	colNames<<"id"														<<"ovenId"  <<"date";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR <<"int"			<<"char(20)"			  ;
	createTable("ovenLoads",colNames,colTypes,MAIN_DB_CONNECTION);
	//ovenLoadsContents
	colNames.clear();colTypes.clear();		
	colNames<<"id"														<<"ovenId"  <<"basketId"<<"profile"<<"sequence"<<"SONum"		<<"SOItemNum" <<"Lot"			<<"pc"	<<"PcLen"	<<"Wt"	<<"PcPulled"<<"temper" 	;
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR <<"char(20)"<<"int"			<<"int"		 <<"int"		 <<"char(50)"	<<"char(50)"	<<"char(50)"<<"real"<<"real"	<<"real"<<"real"		<<"char(50)";
	createTable("ovenLoadsContents",colNames,colTypes,MAIN_DB_CONNECTION);
	////verboseLog
	//colNames.clear();colTypes.clear();		///T52 ...   basketClass::temperEnum BASKET_TEMPER_000,BASKET_TEMPER_185,BASKET_TEMPER_230
	//colNames<<"id"											 <<"temper"  <<"temperType";
	//colTypes<<"integer PRIMARY KEY "+IDENTITY<<"char(50)"<<"int"			  ;
	//createTable("verboseLog",colNames,colTypes,MAIN_DB_CONNECTION);
	//logCranePlc Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"address"  <<"value"<<"X"     <<"Y"     <<"Z"     <<"date"	   <<"sec";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(15)" <<"int"  <<"float" <<"float" <<"float" <<"char(20)"<<"int";
	createTable("logCranePlc",colNames,colTypes,MAIN_DB_CONNECTION);
#ifndef EMULATE
	execQuery(QString("delete from logCranePlc where tid not in(select top 50000 tid from logCranePlc order by sec desc)"),MAIN_DB_CONNECTION);
#endif
	//logResetPlc Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"type"	    <<"value"<<"date"	   <<"sec";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(15)" <<"float"<<"char(20)"<<"int";
	createTable("logResetPlc",colNames,colTypes,MAIN_DB_CONNECTION);
#ifndef EMULATE
	execQuery(QString("delete from logResetPlc where tid not in(select top 50000 tid from logResetPlc order by sec desc)"),MAIN_DB_CONNECTION);
#endif
	//logErrorPlc Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"type"	    <<"bit"     <<"description"<<"date"	   <<"sec";
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(15)" <<"char(10)"<<"char(100)"  <<"char(20)"<<"int";
	createTable("logErrorPlc",colNames,colTypes,MAIN_DB_CONNECTION);
#ifndef EMULATE
	execQuery(QString("delete from logErrorPlc where tid not in(select top 1000 tid from logErrorPlc order by sec desc)"),MAIN_DB_CONNECTION);
#endif
	//users Table
	colNames.clear();colTypes.clear();
	colNames<<"tid"											 <<"name"	    <<"pass"    ;
	colTypes<<"integer PRIMARY KEY "+IDENTITY_STR<<"char(15)" <<"char(10)";
	createTable("users",colNames,colTypes,MAIN_DB_CONNECTION);
#ifndef EMULATE
	execQuery("if not exists (select name from users where name='admin') insert into users (name,pass) values('admin','123')",MAIN_DB_CONNECTION);
#endif
#ifdef inMyLaptop
	//basket_details
	colNames.clear();colTypes.clear();
	colNames<<"basketnum"<<"sequence"<<"dienum"<<"SONum"		<<"SOItemNum"<<"Lot"			<<"Pc"  <<"PcLen"<<"Wt"  <<"LotCompleteFlag"<<"PcPulled"<<"temper"  ;
	colTypes<<"int"			<<"int"			<<"int"		<<"char(50)"<<"char(50)" <<"char(50)"	<<"real"<<"real" <<"real"<<"bit"						<<"real"		<<"char(50)";
	createTable("basket_details",colNames,colTypes,APEX_DB_CONNECTION);
	//basket_location
	colNames.clear();colTypes.clear();
	colNames<<"basketnum"<<"basketlocation"	<<"audittime";
	colTypes<<"int"			 <<"int"						<<"datetime" ;
	createTable("basket_location",colNames,colTypes,APEX_DB_CONNECTION);
#endif
#ifdef EMULATE
	demoFillTables(forceDemoFillTables);
#endif
	if (getFirst(QString("select max(posNumber) from positions"),MAIN_DB_CONNECTION).toInt()==900)//use to be 1000
	{
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1010,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1010,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1010,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1010,4,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1020,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1020,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1020,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1020,4,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1030,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1030,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1030,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('in new oven',1030,4,0,1,0,0)"),MAIN_DB_CONNECTION);

		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1040,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1040,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1040,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1040,4,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1050,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1050,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1050,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1050,4,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1060,1,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1060,2,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1060,3,0,1,0,0)"),MAIN_DB_CONNECTION);
		execQuery(QString("insert into positions (description,posNumber,posIndex,basketTableId,locked,targetNumber,targetIndex) values('after new oven',1060,4,0,1,0,0)"),MAIN_DB_CONNECTION);
	}
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build: %1 %2").arg(__DATE__).arg(__TIME__));
}
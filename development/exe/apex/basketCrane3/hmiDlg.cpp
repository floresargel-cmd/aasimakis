#include "pch.h"
#include "hmiDlg.h"
hmiDlgClass::hmiDlgClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip_,craneClass *crane_):QDialog(crane_)
{
	crane=crane_;
	setWindowTitle(tr("Basket crane 3 HMI"));
	tuxip=tuxip_;
	resize(1800,1000);
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);
	setLayout(mainLayout);
	QSplitter *centralSplitter=new QSplitter(Qt::Vertical);mainLayout->addWidget(centralSplitter);

	QScrollArea *mainScrollArea=new QScrollArea();mainScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	mainScrollArea->setWidgetResizable(true);
	QWidget *mainScrollAreaWidget=new QWidget();
	QHBoxLayout *mainScrollAreaLayout=new QHBoxLayout();mainScrollAreaLayout->setMargin(2);mainScrollAreaWidget->setLayout(mainScrollAreaLayout);
	mainScrollArea->setWidget(mainScrollAreaWidget);
	centralSplitter->addWidget(mainScrollArea);
	QVBoxLayout *layout0=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout0);
	QVBoxLayout *layout1=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout1);
	QVBoxLayout *layout2=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout2);
	QWidget *warningsErrorsWidget=new QWidget();
	QHBoxLayout *warningsErrorsLayout=new QHBoxLayout();warningsErrorsWidget->setLayout(warningsErrorsLayout);
	bitErrorsVector=new bitLogVectorClass("CR3_PCcomDint","plcAlarms.txt",bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelG,bDb,"Errors","c3plcMessages","",false,bitLogHisoryClass::typeMsSql,this);
	bitWarningsVector=new bitLogVectorClass("CR3_PCcomDint","plcWarnings.txt",bitClass::TYPE_SHOW_HIDE,styleLabelO,styleLabelG,bDb,"Warnings","c3plcMessages","",false,bitLogHisoryClass::typeMsSql,this);
	warningsErrorsLayout->addWidget(bitWarningsVector);
	warningsErrorsLayout->addWidget(bitErrorsVector);
	centralSplitter->addWidget(warningsErrorsWidget);

	fillAxisGroup(layout0,tr("Horizontal"),0);
	fillAxisGroup(layout0,tr("Vertical"),12);
	layout0->addStretch(1);
	//layout1
	QSplitter *layout1Splitter=new QSplitter(Qt::Vertical);layout1->addWidget(layout1Splitter);
	QWidget *layout1SplitterUpWidget=new QWidget();layout1Splitter->addWidget(layout1SplitterUpWidget);
	QWidget *layout1SplitterDownWidget=new QWidget();layout1Splitter->addWidget(layout1SplitterDownWidget);
	QVBoxLayout *layout1SplitterUpWidgetL=new QVBoxLayout();layout1SplitterUpWidget->setLayout(layout1SplitterUpWidgetL);
	QVBoxLayout *layout1SplitterDownWidgetL=new QVBoxLayout();layout1SplitterDownWidget->setLayout(layout1SplitterDownWidgetL);
	//auto manual stop alarm
	QHBoxLayout *autoLayout=new QHBoxLayout();layout1SplitterUpWidgetL->addLayout(autoLayout);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",tuxip,"CR3_PCcomDint",22,21,0,"","CR3_PCcomDint",24,0,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);autoLayout->addWidget(pushButtonsVector.last());														 pushButtonsVector.last()->setMinimumWidth(150);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",tuxip,"CR3_PCcomDint",22,22,0,"","CR3_PCcomDint",24,1,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);autoLayout->addWidget(pushButtonsVector.last());												 pushButtonsVector.last()->setMinimumWidth(150);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Active Stop"),"",tuxip,"CR3_PCcomDint",22,226,0,"","CR3_PCcomDint",23,24,"","","","","",buttonStyleActive,buttonStyleRed,tr("No Active Stop"),this);autoLayout->addWidget(pushButtonsVector.last());			 pushButtonsVector.last()->setMinimumWidth(150);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Alarms"),"",tuxip,"CR3_PCcomDint",22,23,0,"","CR3_PCcomDint",24,2,"","","","","",buttonStyleActive,buttonStyleRed,tr("No Alarms"),this);autoLayout->addWidget(pushButtonsVector.last());						 pushButtonsVector.last()->setMinimumWidth(150);
	//camera-missions
	missionsTabWidget=new QTabWidget();layout1SplitterDownWidgetL->addWidget(missionsTabWidget);
	connect(missionsTabWidget,SIGNAL(tabBarDoubleClicked(int)),this,SLOT(missionsTabWidgetTabBarDoubleClickedSlot(int)));
	QWidget *missionsWidget=new QWidget();missionsTabWidget->addTab(missionsWidget,tr("Missions"));
	//missions
	QHBoxLayout *missionsMainLayout=new QHBoxLayout();missionsWidget->setLayout(missionsMainLayout);
	//missions 
	QGroupBox *missionAGroupBox=new QGroupBox(tr("Mission"));missionsMainLayout->addWidget(missionAGroupBox);
	QVBoxLayout *missionAMainGroupBoxLayout=new QVBoxLayout();missionAGroupBox->setLayout(missionAMainGroupBoxLayout);

	QGroupBox *missionANextGroupBox=new QGroupBox(tr("Next Mission"));missionAMainGroupBoxLayout->addWidget(missionANextGroupBox);
	QVBoxLayout *missionANextGroupBoxLayout=new QVBoxLayout();missionANextGroupBox->setLayout(missionANextGroupBoxLayout);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomDint",30,1,3000,tr("Next from:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionANextGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomDint",31,1,3000,tr("Next to:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionANextGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomDint",32,1,5,tr("Status:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionANextGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	QPushButton *clearNextMission=new QPushButton(tr("Clear Next Mission"));missionANextGroupBoxLayout->addWidget(clearNextMission);
	connect(clearNextMission,SIGNAL(clicked()),this,SLOT(clearNextMissionBtnClickedSlot()));

	//targets
	QWidget *targetsWidget=new QWidget();
	missionAMainGroupBoxLayout->addWidget(targetsWidget);
	QHBoxLayout *targetsWidgetL=new QHBoxLayout(); 
	targetsWidget->setLayout(targetsWidgetL);

	QGroupBox *targetsFromGroupBox=new QGroupBox(tr("Targets from")); 
	targetsWidgetL->addWidget(targetsFromGroupBox);
	QVBoxLayout *targetsFromGroupBoxL=new QVBoxLayout();
	targetsFromGroupBox->setLayout(targetsFromGroupBoxL);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",30,0,100000.,tr("X target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	targetsFromGroupBoxL->addWidget(floatPlcMemoryVector.last());
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",32,0,100000.,tr("Z target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	targetsFromGroupBoxL->addWidget(floatPlcMemoryVector.last());

	QGroupBox *targetsToGroupBox=new QGroupBox(tr("Targets to")); 
	targetsWidgetL->addWidget(targetsToGroupBox);
	QVBoxLayout *targetsToGroupBoxL=new QVBoxLayout(); 
	targetsToGroupBox->setLayout(targetsToGroupBoxL);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",33,0,100000.,tr("X target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	targetsToGroupBoxL->addWidget(floatPlcMemoryVector.last());
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",35,0,100000.,tr("Z target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	targetsToGroupBoxL->addWidget(floatPlcMemoryVector.last());

	QGroupBox *missionAActiveGroupBox=new QGroupBox(tr("Active Mission")); 
	missionAMainGroupBoxLayout->addWidget(missionAActiveGroupBox);
	QVBoxLayout *missionAActiveGroupBoxLayout=new QVBoxLayout(); 
	missionAActiveGroupBox->setLayout(missionAActiveGroupBoxLayout);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomDint",0,1,3000,tr("Active From:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);
	missionAActiveGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomDint",1,1,3000,tr("Active To:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);
	missionAActiveGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomDint",2,1,5,tr("PC Status:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);
	missionAActiveGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomDint",3,1,5,tr("PLC Status:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);
	missionAActiveGroupBoxLayout->addWidget(intPlcMemoryVector.last());
	craneStepLabel=new QLabel(); 
	missionAActiveGroupBoxLayout->addWidget(craneStepLabel);
	hooksStepLabel=new QLabel(); 
	missionAActiveGroupBoxLayout->addWidget(hooksStepLabel);
	QHBoxLayout *endAbordALayout=new QHBoxLayout(); 
	missionAActiveGroupBoxLayout->addLayout(endAbordALayout);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Abord Active Mission"),"",tuxip,"CR3_PCcomDint",22,26,0,"","CR3_PCcomDint",21,15,"","","","","",buttonStyle,buttonStyleActive,tr("Abord Active Mission"),this);
	endAbordALayout->addWidget(pushButtonsVector.last());
	abordActiveMission=pushButtonsVector.last();
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Continue Active Mission"),"",tuxip,"CR3_PCcomDint",22,48,0,"","CR3_PCcomDint",21,16,"","","","","",buttonStyle,buttonStyleActive,tr("Continue Active Mission"),this);
	endAbordALayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("End Active Mission"),"",tuxip,"CR3_PCcomDint",22,24,0,"","CR3_PCcomDint",21,17,"","","","","",buttonStyle,buttonStyleActive,tr("End Active Mission"),this);
	endAbordALayout->addWidget(pushButtonsVector.last());
	endActiveMission=pushButtonsVector.last();
	clearActiveMission=new QPushButton(tr("Clear Active Mission in database")); 
	missionAActiveGroupBoxLayout->addWidget(clearActiveMission);
	connect(clearActiveMission,SIGNAL(clicked()),this,SLOT(clearActiveMissionBtnClickedSlot()));
//layout2
	//Basket pins
	int row=0;
	QGroupBox *basketPinsGroupBox=new QGroupBox(tr("Basket pins")); 
	layout2->addWidget(basketPinsGroupBox);
	QGridLayout *basketPinsGroupBoxL=new QGridLayout(); 
	basketPinsGroupBox->setLayout(basketPinsGroupBoxL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Lock"),":/lock.png",tuxip,"CR3_PCcomDint",22,6,0,"","CR3_PCcomDint",23,9,"","","","","",buttonStyle,buttonStyleActive,tr("Lock"),this);
	basketPinsGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Unlock"),":/unLock.png",tuxip,"CR3_PCcomDint",22,5,0,"","CR3_PCcomDint",23,8,"","","","","",buttonStyle,buttonStyleActive,tr("Unlock"),this);
	basketPinsGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	bitVector<<new bitClass("CR3_PCcomDint",23,10,bitClass::TYPE_CHANGE_COLOR,tr("Locked"),tr("Locked"),styleLabelG,styleLabelGr,"","","","",this);
	basketPinsGroupBoxL->addWidget(bitVector.last(),row,0);
	bitVector<<new bitClass("CR3_PCcomDint",23,11,bitClass::TYPE_CHANGE_COLOR,tr("Unlocked"),tr("Unlocked"),styleLabelG,styleLabelGr,"","","","",this);
	basketPinsGroupBoxL->addWidget(bitVector.last(),row,1);

	QPushButton *settingsBtn=new QPushButton(QIcon(":/autoEnabled.png"),tr("Settings"),this);
	layout2->addWidget(settingsBtn);
	connect(settingsBtn,SIGNAL(clicked(bool)),this,SLOT(settingsBtnClickedSlot()));
	
	bitPositionsVector=new bitLogVectorClass("CR3_PCcomDint","plcPositions.txt",bitClass::TYPE_SHOW_HIDE,styleLabelB,styleLabelG,bDb,"Positions","c3plcMessages","",false,bitLogHisoryClass::typeMsSql,this);
	bitSignalsFromVector=new bitLogVectorClass("CR3_PCcomDint","plcInterlocksFrom.txt",bitClass::TYPE_SHOW_HIDE,styleLabelB,styleLabelG,bDb,"Interlocks signals from","c3plcMessages","",false,bitLogHisoryClass::typeMsSql,this);
	bitSignalsToVector=new bitLogVectorClass("CR3_PCcomDint","plcInterlocksTo.txt",bitClass::TYPE_SHOW_HIDE,styleLabelB,styleLabelG,bDb,"Interlocks signals to","c3plcMessages","",false,bitLogHisoryClass::typeMsSql,this);
	layout2->addWidget(bitPositionsVector);
	layout2->addWidget(bitSignalsFromVector);
	layout2->addWidget(bitSignalsToVector);
	layout2->addStretch(1);
	
	centralSplitter->setSizes(QList<int>()<<height()-100<<100);
	settings=new hmiSettingsClass(tuxip,this);
	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);
}
void hmiDlgClass::fillAxisGroup(QVBoxLayout *layout,QString title,int index)
{
	QGroupBox *groupBox=new QGroupBox(title);layout->addWidget(groupBox);
	QGridLayout *groupBoxL=new QGridLayout();groupBox->setLayout(groupBoxL);
	int row=0;
	progressVector<<new progressClass(tuxip,crane3Ip,"CR3_PCcomReal",index+2,tr("Velocity"),tr("m/min"),-120.,120.,20.,200,200,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","","","",180,1,this);
	groupBoxL->addWidget(progressVector.last(),row,0,1,2,Qt::AlignHCenter|Qt::AlignTop);
	plotsVector<<new plotClass(crane3Ip,"CR3_PCcomReal",index+3,tr("Hz"),this);
	plotsVector.last()->addNewGraph(crane3Ip,"CR3_PCcomReal",index+0,tr("Position[mm]"),1,0);
	plotsVector.last()->addNewGraph(crane3Ip,"CR3_PCcomReal",index+2,tr("Velocity[m/min]"),2,0);
	plotsVector.last()->addNewGraph(crane3Ip,"CR3_PCcomReal",index+4,tr("Current[Amps]"),3,0);
	progressVector.last()->setPlotClass(plotsVector.last());

	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",index+1,0,100000.,tr("Target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	groupBoxL->addWidget(floatPlcMemoryVector.last(),row,0);
	if (index==0)
	{
		bitVector<<new bitClass("CR3_PCcomDint",23,22,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);
		groupBoxL->addWidget(bitVector.last(),row,1);
	}
	if (index==12)
	{
		bitVector<<new bitClass("CR3_PCcomDint",23,25,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);
		groupBoxL->addWidget(bitVector.last(),row,1);
	}
	bitVector.last()->setMinimumWidth(150);
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",index+0,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	groupBoxL->addWidget(floatPlcMemoryVector.last(),row,0);
	if (index==0)
	{
		bitVector<<new bitClass("CR3_PCcomDint",23,1,bitClass::TYPE_CHANGE_COLOR,tr("Proximity on"),tr("Proximity off"),styleLabelG,styleLabelGr,"","","","",this);
		groupBoxL->addWidget(bitVector.last(),row,1);
	}
	if (index==12)
	{
		bitVector<<new bitClass("CR3_PCcomDint",23,0,bitClass::TYPE_CHANGE_COLOR,tr("Proximity on"),tr("Proximity off"),styleLabelG,styleLabelGr,"","","","",this);
		groupBoxL->addWidget(bitVector.last(),row,1);
	}
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",index+3,0,100.,tr("Hz"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	groupBoxL->addWidget(floatPlcMemoryVector.last(),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",index+4,0,100.,tr("Amps"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	groupBoxL->addWidget(floatPlcMemoryVector.last(),row,1);
	row++;
	if (index==0)
	{
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Bwd"),":/arrowLeft.png",tuxip,"CR3_PCcomDint",22,2,0,"","CR3_PCcomDint",24,4,"","","","","",buttonStyle,buttonStyleActive,tr("Bwd"),this);
		groupBoxL->addWidget(pushButtonsVector.last(),row,0);
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Fwd"),":/arrowRight.png",tuxip,"CR3_PCcomDint",22,1,0,"","CR3_PCcomDint",24,3,"","","","","",buttonStyle,buttonStyleActive,tr("Fwd"),this);
		groupBoxL->addWidget(pushButtonsVector.last(),row,1);
	}
	if (index==12)
	{
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Up"),":/arrowUp.png",tuxip,"CR3_PCcomDint",22,8,0,"","CR3_PCcomDint",24,10,"","","","","",buttonStyle,buttonStyleActive,tr("Up"),this);
		groupBoxL->addWidget(pushButtonsVector.last(),row,0);
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Down"),":/arrowDown.png",tuxip,"CR3_PCcomDint",22,7,0,"","CR3_PCcomDint",24,9,"","","","","",buttonStyle,buttonStyleActive,tr("Down"),this);
		groupBoxL->addWidget(pushButtonsVector.last(),row,1);
	}
	row++;
	if (index==12)
	{
		bitVector<<new bitClass("CR3_PCcomDint",23,17,bitClass::TYPE_CHANGE_COLOR,tr("No Obstacle"),tr("Obstacle"),styleLabelG,styleLabelO,"","","","",this);
		groupBoxL->addWidget(bitVector.last(),row,0,1,2);
	}
}
void hmiDlgClass::missionsTabWidgetTabBarDoubleClickedSlot(int index)
{
	if (index==0)
		lockAdvancedBtns(!areAdvancedButtonsLocked);
}
void hmiDlgClass::lockAdvancedBtns(bool l)
{
	if (!l)
	{
		if (!getPassword(tr("Password is needed activete advanced buttons."),tr("Please give the password"),passStrong,this))
			return;
	}
	areAdvancedButtonsLocked=l;
	if (areAdvancedButtonsLocked)
	{
		clearActiveMission->setEnabled(false);
		abordActiveMission->setEnabled(false);
		endActiveMission->setEnabled(false);
		missionsTabWidget->tabBar()->setTabIcon(0,QIcon(":/lock.png"));
	}
	else
	{
		clearActiveMission->setEnabled(true);
		abordActiveMission->setEnabled(true);
		endActiveMission->setEnabled(true);
		missionsTabWidget->tabBar()->setTabIcon(0,QIcon(":/unLock.png"));
	}
}
void hmiDlgClass::stopSlot()
{
	tuxip->writeInteger("CR3_PCcomDint",22,226);
	tuxip->writeInteger("CR3_PCcomDint",22,0);
}
void hmiDlgClass::setCarriageClass(carriageClass *c)
{
	carriage=c;
}
int hmiDlgClass::exec()
{
	lockAdvancedBtns(true);
//	if (getPassword(tr("Password is needed to open the HMI window."),tr("Please give the password"),passWeak,this))
		return QDialog::exec();
//	return 0;
}
void hmiDlgClass::clearNextMissionBtnClickedSlot()
{
	crane->getCarriage()->clearNextMission();
}
void hmiDlgClass::clearActiveMissionBtnClickedSlot()
{
	if (getPassword(tr("Password is needed to end an active mission.")),tr("Only in case Plc missions are cleared."),this)
		crane->getCarriage()->clearMissionInDb("user");
}
void hmiDlgClass::settingsBtnClickedSlot()
{
	settings->exec();
}
void hmiDlgClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (crane3Ip==ip)
	{
		if("CR3_PCcomDint"==table)
		{
			if ((index==21)||(index==23)||(index==24))
			{
				for (int i=0;i<bitVector.count();i++)
					bitVector[i]->updateValue(table,index,value.toInt());
				for (int i=0;i<pushButtonsVector.count();i++)
					pushButtonsVector[i]->updateValue(table,index,value.toInt());
			}
			if (index==18)
			{
				craneStepLabel->setText(QString("Step:%1:%2").arg(value.toInt()).arg(crane->getCarriage()->getCraneStepText(value.toInt())));
				craneStepLabel->setToolTip(crane->getCarriage()->getCraneStepDescription(value.toInt()));
			}
			for (int i=0;i<intPlcMemoryVector.count();i++)
				intPlcMemoryVector[i]->updateValue(ip,table,index,value.toInt());
			bitErrorsVector->updateValue(table,index,value.toInt());
			bitWarningsVector->updateValue(table,index,value.toInt());
			bitPositionsVector->updateValue(table,index,value.toInt());
			bitSignalsFromVector->updateValue(table,index,value.toInt());
			bitSignalsToVector->updateValue(table,index,value.toInt());
		}
		if("CR3_PCcomReal"==table)
		{
			for (int i=0;i<progressVector.count();i++)
				progressVector[i]->updateValue(ip,table,index,value.toFloat());
			for (int i=0;i<floatPlcMemoryVector.count();i++)
				floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toFloat());
			for (int i=0;i<plotsVector.count();i++)
				plotsVector[i]->dataInPlcChanged(ip,table,index,value.toFloat());
			settings->dataInPlcChanged(ip,table,index,value);
		}
	}
}
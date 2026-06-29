#include "pch.h"
#include "hmiDlgForkPacking.h"
hmiDlgForkPackingClass::hmiDlgForkPackingClass(tuxipClass *tuxip_,forkPackingClass *fork_):QDialog(fork_)
{
	resize(1300,1000);
	tuxip=tuxip_;
	fork=fork_;
	setWindowTitle(tr("Packing Fork HMI"));
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);
	setLayout(mainLayout);
	QSplitter *centralSplitter=new QSplitter(Qt::Vertical);mainLayout->addWidget(centralSplitter);

	QScrollArea *mainScrollArea=new QScrollArea();mainScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	mainScrollArea->setWidgetResizable(true);
	QWidget *mainScrollAreaWidget=new QWidget();
	QHBoxLayout *mainScrollAreaLayout=new QHBoxLayout();mainScrollAreaLayout->setMargin(2);mainScrollAreaWidget->setLayout(mainScrollAreaLayout);
	mainScrollArea->setWidget(mainScrollAreaWidget);
	centralSplitter->addWidget(mainScrollArea);
	QVBoxLayout *layout0=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout0);layout0->setMargin(2);
	QVBoxLayout *layout1=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout1);
	QVBoxLayout *layout2=new QVBoxLayout();mainScrollAreaLayout->addLayout(layout2);
	QWidget *warningsErrorsWidget=new QWidget();
	QHBoxLayout *warningsErrorsLayout=new QHBoxLayout();warningsErrorsWidget->setLayout(warningsErrorsLayout);
	bitErrorsVector=new bitLogVectorClass("fc1ToPcI","packingForkAlarms.txt",bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelG,pDb,"Errors","packingForkPlcMessages","",false,bitLogHisoryClass::typeMsSql,this);
	bitWarningsVector=new bitLogVectorClass("fc1ToPcI","packingForkWarnings.txt",bitClass::TYPE_SHOW_HIDE,styleLabelO,styleLabelG,pDb,"Warnings","packingForkPlcMessages","",false,bitLogHisoryClass::typeMsSql,this);
	warningsErrorsLayout->addWidget(bitWarningsVector);
	warningsErrorsLayout->addWidget(bitErrorsVector);
	centralSplitter->addWidget(warningsErrorsWidget);

	fillAxisGroup(layout0,tr("Horizontal X"),0);
	fillAxisGroup(layout0,tr("Horizontal Y"),11);
	fillAxisGroup(layout0,tr("Vertical"),6);
	fillAxisGroup(layout1,tr("Rotation"),16);
	layout0->addStretch(1);

	QGroupBox *missionGroupBox=new QGroupBox(tr("Mission"));layout1->addWidget(missionGroupBox);
	QGridLayout *missionGroupBoxL=new QGridLayout();missionGroupBoxL->setMargin(2);missionGroupBox->setLayout(missionGroupBoxL);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,tuxip->getIp(),"fc1ToPcI",4,0,INT_MAX,tr("Active from:"),"","value","","from pos",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionGroupBoxL->addWidget(intPlcMemoryVector.last(),1,0);
	intPlcMemoryVector<<new intPlcMemoryClass(tuxip,tuxip->getIp(),"fc1ToPcI",5,0,INT_MAX,tr("Active To:"),"","value","","to pos",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);missionGroupBoxL->addWidget(intPlcMemoryVector.last(),1,1);

	bitVector<<new bitClass("fc1ToPcI",12,1,bitClass::TYPE_CHANGE_COLOR,tr("Ok for new mission"),tr(""),styleLabelG,styleLabelGr,"","","","",this);missionGroupBoxL->addWidget(bitVector.last(),3,0,1,2);
	bitVector<<new bitClass("fc1ToPcI",12,2,bitClass::TYPE_CHANGE_COLOR,tr("On mission"),tr("Not on mission"),styleLabelG,styleLabelGr,"","","","",this);missionGroupBoxL->addWidget(bitVector.last(),3,2,1,2);
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Continue"),"",tuxip,"fc1ToPcI",5,13,0,"","fc1ToPcI",0,0,"","","","","",buttonStyle,"",tr("Continue"),this);missionGroupBoxL->addWidget(pushButtonsVector.last(),4,0,1,4);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Abort"),"",tuxip,"fc1ToPcI",22,26,0,"","fc1ToPcI",21,15,"","","","","",buttonStyle,buttonStyleActive,tr("Abort"),this);missionGroupBoxL->addWidget(pushButtonsVector.last(),5,0,1,2);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("End"),"",tuxip,"fc1ToPcI",22,24,0,"","fc1ToPcI",21,17,"","","","","",buttonStyle,buttonStyleActive,tr("End"),this);missionGroupBoxL->addWidget(pushButtonsVector.last(),5,2,1,2);
	int row=0;
	QGroupBox *modeGroupBox=new QGroupBox(tr("Mode"));layout1->addWidget(modeGroupBox);
	QGridLayout *modeGroupBoxL=new QGridLayout();modeGroupBoxL->setMargin(2);modeGroupBox->setLayout(modeGroupBoxL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",tuxip,"fc1ToPcI",22,21,0,"","fc1ToPcI",12,3,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",tuxip,"fc1ToPcI",22,22,0,"","fc1ToPcI",12,4,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Alarms"),"",tuxip,"fc1ToPcI",22,23,0,"","fc1ToPcI",13,27,"","","","","",buttonStyleActive,buttonStyleRed,tr("Reset Alarms"),this);modeGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Stop"),"",tuxip,"fc1ToPcI",22,226,0,"","fc1ToPcI",12,6,"","","","","",buttonStyleActive,buttonStyleRed,tr("Stop"),this);modeGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	
	bitLogVectorV<<new bitLogVectorClass("fc1ToPcI","packingForkAutoMissionSteps.txt",bitClass::TYPE_SHOW_HIDE,styleLabelG,styleLabelGr,pDb,"","","",false, bitLogHisoryClass::typeMsSql,this);layout1->addWidget(bitLogVectorV.last());
	bitLogVectorV<<new bitLogVectorClass("fc1ToPcI","packingForkPositions.txt",bitClass::TYPE_SHOW_HIDE,styleLabelB,styleLabelGr,pDb,"","","",false,bitLogHisoryClass::typeMsSql,this);layout1->addWidget(bitLogVectorV.last());
	
	QHBoxLayout *settingsBox=new QHBoxLayout();layout1->addLayout(settingsBox);
	QPushButton *settingsBtn=new QPushButton(QIcon(":/autoEnabled.png"),tr("Settings"),this);
	settingsBox->addWidget(settingsBtn);
	connect(settingsBtn,SIGNAL(clicked(bool)),this,SLOT(settingsBtnClickedSlot()));
	//
	QGroupBox *signalsStrappingGroupBox=new QGroupBox(tr("Signals Strapping"));layout2->addWidget(signalsStrappingGroupBox);
	QGridLayout *signalsStrappingGroupBoxL=new QGridLayout();signalsStrappingGroupBoxL->setMargin(2);signalsStrappingGroupBox->setLayout(signalsStrappingGroupBoxL);
	row=0;
	bitVector<<new bitClass("fc1ToPcI",12,7,bitClass::TYPE_CHANGE_COLOR,tr("Input Rollers Ok"),tr("Input Rollers Not Ok"),styleLabelG,styleLabelGr,"","","","",this);signalsStrappingGroupBoxL->addWidget(bitVector.last(),row,0);
	bitVector<<new bitClass("fc1ToPcI",12,9,bitClass::TYPE_CHANGE_COLOR,tr("Output Rollers Ok"),tr("Output Rollers Not Ok"),styleLabelG,styleLabelGr,"","","","",this);signalsStrappingGroupBoxL->addWidget(bitVector.last(),row,1);

	QGroupBox *signalsWrappingGroupBox=new QGroupBox(tr("Signals Wrapping"));layout2->addWidget(signalsWrappingGroupBox);
	QGridLayout *signalsWrappingGroupBoxL=new QGridLayout();signalsWrappingGroupBoxL->setMargin(2);signalsWrappingGroupBox->setLayout(signalsWrappingGroupBoxL);
	row=0;
	bitVector<<new bitClass("fc1ToPcI",12,8,bitClass::TYPE_CHANGE_COLOR,tr("Input Rollers Ok"),tr("Input Rollers Not Ok"),styleLabelG,styleLabelGr,"","","","",this);signalsWrappingGroupBoxL->addWidget(bitVector.last(),row,0);
	bitVector<<new bitClass("fc1ToPcI",12,10,bitClass::TYPE_CHANGE_COLOR,tr("Output Rollers Ok"),tr("Output Rollers Not Ok"),styleLabelG,styleLabelGr,"","","","",this);signalsWrappingGroupBoxL->addWidget(bitVector.last(),row,1);
	
	QGroupBox *signalsInOutGroupBox=new QGroupBox(tr("Signals In/Out"));layout2->addWidget(signalsInOutGroupBox);
	QGridLayout *signalsInOutGroupBoxL=new QGridLayout();signalsInOutGroupBoxL->setMargin(2);signalsInOutGroupBox->setLayout(signalsInOutGroupBoxL);
	bitVector<<new bitClass("fc1ToPcI",12,11,bitClass::TYPE_CHANGE_COLOR,tr("Packet Ok"),tr("Packet Not Ok"),styleLabelG,styleLabelGr,"","","","",this);signalsInOutGroupBoxL->addWidget(bitVector.last(),row,0);
	layout2->addStretch(1);
	//
	centralSplitter->setSizes(QList<int>()<<900<<100);
	settings=new hmiSettingsPackingForkClass(tuxip,this);
	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);

}
hmiDlgForkPackingClass::~hmiDlgForkPackingClass()
{
}
void hmiDlgForkPackingClass::fillAxisGroup(QVBoxLayout *layout,QString title,int index)
{
	QGroupBox *groupBox=new QGroupBox(title);layout->addWidget(groupBox);
	QGridLayout *groupBoxL=new QGridLayout();groupBoxL->setMargin(1);groupBox->setLayout(groupBoxL);
	int row=0;
	if (index==16)
	{
		progressVector<<new progressClass(tuxip,crane3Ip,"fc1ToPcF",index+2,tr("Velocity"),tr("deg/min"),-120.,120.,20.,200,200,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","","","",180,1,this);groupBoxL->addWidget(progressVector.last(),row,0,1,4,Qt::AlignHCenter|Qt::AlignTop);
	}
	else
	{
		progressVector<<new progressClass(tuxip,crane3Ip,"fc1ToPcF",index+2,tr("Velocity"),tr("m/min"),-120.,120.,20.,200,200,QColor(100,100,125),QColor(200,200,225),progressClass::TYPE_ARC|progressClass::TYPE_FLOAT,"","","","",180,1,this);groupBoxL->addWidget(progressVector.last(),row,0,1,4,Qt::AlignHCenter|Qt::AlignTop);
	}
	plotsVector<<new plotClass(crane3Ip,"fc1ToPcF",index+3,tr("Hz"),this);
	/*if (index==16)
		plotsVector.last()->addNewGraph(crane3Ip,"fc1ToPcF",index+0,tr("Position[Degs]"),1,0);
	else
		plotsVector.last()->addNewGraph(crane3Ip,"fc1ToPcF",index+0,tr("Position[mm]"),1,0);*/
	plotsVector.last()->addNewGraph(crane3Ip,"fc1ToPcF",index+2,tr("Velocity[m/min]"),2,0);
	plotsVector.last()->addNewGraph(crane3Ip,"fc1ToPcF",index+4,tr("Current[Amps]"),3,0);
	progressVector.last()->setPlotClass(plotsVector.last());

	row++;
	if (index==16)
	{
		floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"fc1ToPcF",index+1,0,100000.,tr("Target[degrees]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);groupBoxL->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	}
	else
	{
		floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"fc1ToPcF",index+1,0,100000.,tr("Target[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);groupBoxL->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	}	
	if (index==0)
	{
		bitVector<<new bitClass("fc1ToPcI",13,22,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,2,1,2);
	}
	if (index==6)
	{
		bitVector<<new bitClass("fc1ToPcI",13,23,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,2,1,2);
	}
	if (index==11)
	{
		bitVector<<new bitClass("fc1ToPcI",13,25,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,2,1,2);
	}
	if (index==16)
	{
		bitVector<<new bitClass("fc1ToPcI",13,28,bitClass::TYPE_CHANGE_COLOR,tr("on target"),tr("not on target"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,2,1,2);
	}
	bitVector.last()->setMinimumWidth(150);
	row++;
	if (index==16)
	{
		floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"fc1ToPcF",index+0,0,100000.,tr("Position[degrees]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);groupBoxL->addWidget(floatPlcMemoryVector.last(),row,0);
	}
	else
	{
		floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"fc1ToPcF",index+0,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);groupBoxL->addWidget(floatPlcMemoryVector.last(),row,0,1,2);
	}
	if (index==0)
	{
		bitVector<<new bitClass("fc1ToPcI",13,1,bitClass::TYPE_CHANGE_COLOR,tr("Proximity on"),tr("Proximity off"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,2,1,2);
	}
	if (index==6)
	{
		bitVector<<new bitClass("fc1ToPcI",13,15,bitClass::TYPE_CHANGE_COLOR,tr("Proximity on"),tr("Proximity off"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,2,1,2);
	}
	if (index==11)
	{
		bitVector<<new bitClass("fc1ToPcI",13,0,bitClass::TYPE_CHANGE_COLOR,tr("Proximity on"),tr("Proximity off"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,2,1,2);
	}
	if (index==16)
	{
		bitVector<<new bitClass("fc1ToPcI",13,6,bitClass::TYPE_CHANGE_COLOR,tr("Reset Proximity on"),tr("Reset Proximity off"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,2);
		bitVector<<new bitClass("fc1ToPcI",13,7,bitClass::TYPE_CHANGE_COLOR,tr("Proximity 90 degs on"),tr("Proximity 90 degs off"),styleLabelG,styleLabelGr,"","","","",this);groupBoxL->addWidget(bitVector.last(),row,3);
	}
	row++;
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"fc1ToPcF",index+3,0,100.,tr("Hz"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);groupBoxL->addWidget(floatPlcMemoryVector.last(),row,1);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"fc1ToPcF",index+4,0,100.,tr("Amps"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);groupBoxL->addWidget(floatPlcMemoryVector.last(),row,2);
	if (index==0)
	{
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Bwd"),":/arrowLeft.png",tuxip,"fc1ToPcI",22,2,0,"","fc1ToPcI",14,4,"","","","","",buttonStyle,buttonStyleActive,tr("Bwd"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,0);
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Fwd"),":/arrowRight.png",tuxip,"fc1ToPcI",22,1,0,"","fc1ToPcI",14,3,"","","","","",buttonStyle,buttonStyleActive,tr("Fwd"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,3);
	}
	if (index==11)
	{
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Bwd"),":/arrowUp.png",tuxip,"fc1ToPcI",22,4,0,"","fc1ToPcI",14,6,"","","","","",buttonStyle,buttonStyleActive,tr("Bwd"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,0);
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Fwd"),":/arrowDown.png",tuxip,"fc1ToPcI",22,3,0,"","fc1ToPcI",14,5,"","","","","",buttonStyle,buttonStyleActive,tr("Fwd"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,3);
		
	}
	if (index==6)
	{
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Up"),":/arrowUp.png",tuxip,"fc1ToPcI",22,8,0,"","fc1ToPcI",14,10,"","","","","",buttonStyle,buttonStyleActive,tr("Up"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,0);
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Down"),":/arrowDown.png",tuxip,"fc1ToPcI",22,7,0,"","fc1ToPcI",14,9,"","","","","",buttonStyle,buttonStyleActive,tr("Down"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,3);
	}
	if (index==16)
	{
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Fwd"),":/arrowLeft.png",tuxip,"fc1ToPcI",22,5,0,"","fc1ToPcI",13,29,"","","","","",buttonStyle,buttonStyleActive,tr("Fwd"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,0);
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Bwd"),":/arrowRight.png",tuxip,"fc1ToPcI",22,6,0,"","fc1ToPcI",13,30,"","","","","",buttonStyle,buttonStyleActive,tr("Bwd"),this);groupBoxL->addWidget(pushButtonsVector.last(),row,3);
	}
}

void hmiDlgForkPackingClass::stopSlot()
{
	tuxip->writeInteger("fc1ToPcI",22,226);
	tuxip->writeInteger("fc1ToPcI",22,0);
}
int hmiDlgForkPackingClass::exec()
{
	return QDialog::exec();
}

void hmiDlgForkPackingClass::settingsBtnClickedSlot()
{
	settings->exec();
}
void hmiDlgForkPackingClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (crane3Ip==ip)
	{
		if ("fc1ToPcI"==table)
		{
			for (int i=0;i<bitVector.count();i++)
				bitVector[i]->updateValue(table,index,value.toInt());
			for (int i=0;i<pushButtonsVector.count();i++)
				pushButtonsVector[i]->updateValue(table,index,value.toInt());
			for (int i=0;i<intPlcMemoryVector.count();i++)
				intPlcMemoryVector[i]->updateValue(ip,table,index,value.toInt());
			bitErrorsVector->updateValue(table,index,value.toInt());
			bitWarningsVector->updateValue(table,index,value.toInt());
			for (int i=0;i<bitLogVectorV.count();i++)
				bitLogVectorV[i]->updateValue(table,index,value.toInt());
			settings->dataInPlcChanged(ip,table,index,value);
		}
		if("fc1ToPcF"==table)
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
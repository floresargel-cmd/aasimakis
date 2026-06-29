#include "pch.h"
#include "stationWdgt.h"
stationWdgtClass::stationWdgtClass(tuxipClass *oldStrappingTux_,tuxipClass *forkTux_,QWidget *p):QWidget(p)
{
	oldStrappingTux=oldStrappingTux_;
	forkTux=forkTux_;
	setWindowTitle(tr("Strapping HMI"));

	QVBoxLayout *topL=new QVBoxLayout();
	topL->setMargin(0);
	setLayout(topL);
	QScrollArea *scrollArea=new QScrollArea();
	scrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	topL->addWidget(scrollArea);
	scrollArea->setWidgetResizable(true);
	QWidget *scrollAreaWdgt=new QWidget();
	QHBoxLayout *mainL=new QHBoxLayout();
	mainL->setMargin(0);
	scrollAreaWdgt->setLayout(mainL);
	scrollArea->setWidget(scrollAreaWdgt);
	QVBoxLayout *main0L=new QVBoxLayout();main0L->setMargin(0);mainL->addLayout(main0L);
	QVBoxLayout *main1L=new QVBoxLayout();main1L->setMargin(0);mainL->addLayout(main1L);
	QVBoxLayout *main2L=new QVBoxLayout();main2L->setMargin(0);mainL->addLayout(main2L);
	QVBoxLayout *main3L=new QVBoxLayout();main3L->setMargin(0);mainL->addLayout(main3L);
	//
	int spinFontSize=15;
	int spinSize=25;
	if (strappingStationExeEnumG==strappingStationExeEnumClass::oldStrapping)
	{
		posOut=new posPackingForkClass(0,nullptr,forkTux,strappingOldOutPlcId,1172.f,2385.f,2000.,0.,tr("Strapping Station 1"),nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,this);
		//Mode
		{
			QGroupBox *modeGroupBox=new QGroupBox("Mode");main0L->addWidget(modeGroupBox);modeGroupBox->setMinimumWidth(200);
			QVBoxLayout *modeGroupBoxL=new QVBoxLayout();modeGroupBoxL->setMargin(2);modeGroupBox->setLayout(modeGroupBoxL);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",oldStrappingTux,"N162",2,814,0,"","N162",1,10,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",oldStrappingTux,"N162",2,815,0,"","N162",1,10,"","","","","",buttonStyleActive,buttonStyle,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Packet Measurements"),":/reset.png",oldStrappingTux,"N162",2,829,0,"","N162",0,0,"","","","","",buttonStyle,buttonStyle,tr("Reset Packet Measurements"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
			bitVector<<new bitClass("N162",1,20,bitClass::TYPE_CHANGE_COLOR,tr("Ready For Strapping"),tr(""),styleLabelG,styleLabelGr,"","","","",this);modeGroupBoxL->addWidget(bitVector.last());
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Start Strapping Cycle"),":/return.png",oldStrappingTux,"N162",2,834,0,"","N162",1,21,"","","","","",buttonStyle,buttonStyleO,tr("Start Strapping Cycle"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
			bitLogVector<<new bitLogVectorClass("N162","strappingOldAlarms.txt",bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelG,"","Errors","","",false,bitLogHisoryClass::typeMsSql,this);modeGroupBoxL->addWidget(bitLogVector.last());bitLogVector.last()->setMinimumHeight(500);
		}
		//Packet in
		{
			QGroupBox *packetGroupBox=new QGroupBox("Packet In Entrance");main1L->addWidget(packetGroupBox);packetGroupBox->setMinimumWidth(225);
			QVBoxLayout *packetGroupBoxL=new QVBoxLayout();packetGroupBoxL->setMargin(2);packetGroupBox->setLayout(packetGroupBoxL);
			QHBoxLayout *packetOnOffL=new QHBoxLayout();packetOnOffL->setMargin(2);packetGroupBoxL->addLayout(packetOnOffL);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("On"),"",oldStrappingTux,"N162",2,832,0,"","N162",0,0,"","","","","",buttonStyle,buttonStyle,tr("On"),this);packetOnOffL->addWidget(pushButtonsVector.last());
			bitVector<<new bitClass("N162",1,13,bitClass::TYPE_CHANGE_COLOR,tr("Packet"),tr(""),styleLabelG,styleLabelGr,"","","","",this);packetOnOffL->addWidget(bitVector.last());
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Off"),"",oldStrappingTux,"N162",2,833,0,"","N162",0,0,"","","","","",buttonStyle,buttonStyle,tr("Off"),this);packetOnOffL->addWidget(pushButtonsVector.last());
			intPlcMemoryVector<<new intPlcMemoryClass(forkTux,forkTux->getIp(),"packetId",strappingOldInPlcId,0,9999,tr("Rack Id:"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,"",this);packetGroupBoxL->addWidget(intPlcMemoryVector.last());		intPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			intPlcMemoryVector<<new intPlcMemoryClass(forkTux,forkTux->getIp(),"packetLength",strappingOldInPlcId,0,7660,tr("Length:"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,"",this);packetGroupBoxL->addWidget(intPlcMemoryVector.last());intPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			floatPlcMemoryVector<<new floatPlcMemoryClass(oldStrappingTux,oldStrappingTux->getIp(),"F163",1,0,1000,tr("Width[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,0,"",this);packetGroupBoxL->addWidget(floatPlcMemoryVector.last());	floatPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			floatPlcMemoryVector<<new floatPlcMemoryClass(oldStrappingTux,oldStrappingTux->getIp(),"F163",2,0,1000,tr("Height[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,0,"",this);packetGroupBoxL->addWidget(floatPlcMemoryVector.last());floatPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
		}
		//Packet Out
		{
			QGroupBox *packetGroupBox=new QGroupBox("Packet In Exit");main1L->addWidget(packetGroupBox);packetGroupBox->setMinimumWidth(225);
			QVBoxLayout *packetGroupBoxL=new QVBoxLayout();packetGroupBoxL->setMargin(2);packetGroupBox->setLayout(packetGroupBoxL);
			QHBoxLayout *packetOnOffL=new QHBoxLayout();packetOnOffL->setMargin(2);packetGroupBoxL->addLayout(packetOnOffL);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("On"),"",oldStrappingTux,"N162",2,816,0,"","N162",0,0,"","","","","",buttonStyle,buttonStyle,tr("On"),this);packetOnOffL->addWidget(pushButtonsVector.last());
			bitVector<<new bitClass("N162",1,12,bitClass::TYPE_CHANGE_COLOR,tr("Packet"),tr(""),styleLabelG,styleLabelGr,"","","","",this);packetOnOffL->addWidget(bitVector.last());
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Off"),"",oldStrappingTux,"N162",2,817,0,"","N162",0,0,"","","","","",buttonStyle,buttonStyle,tr("Off"),this);packetOnOffL->addWidget(pushButtonsVector.last());
			floatPlcMemoryVector<<new floatPlcMemoryClass(oldStrappingTux,oldStrappingTux->getIp(),"F163",11,0,999999,tr("Ticket"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,0,"",this);packetGroupBoxL->addWidget(floatPlcMemoryVector.last());	floatPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			floatPlcMemoryVector<<new floatPlcMemoryClass(oldStrappingTux,oldStrappingTux->getIp(),"F163",7,0,bigLength,tr("Length"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,0,"",this);packetGroupBoxL->addWidget(floatPlcMemoryVector.last());floatPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			floatPlcMemoryVector<<new floatPlcMemoryClass(oldStrappingTux,oldStrappingTux->getIp(),"F163",10,0,5,tr("Woods"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,0,"",this);packetGroupBoxL->addWidget(floatPlcMemoryVector.last());				floatPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			floatPlcMemoryVector<<new floatPlcMemoryClass(oldStrappingTux,oldStrappingTux->getIp(),"F163",8,0,1000,tr("Width"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,0,"",this);packetGroupBoxL->addWidget(floatPlcMemoryVector.last());				floatPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			floatPlcMemoryVector<<new floatPlcMemoryClass(oldStrappingTux,oldStrappingTux->getIp(),"F163",9,0,1000,tr("Height"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,0,"",this);packetGroupBoxL->addWidget(floatPlcMemoryVector.last());			floatPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
		}		
		bitLogVector<<new bitLogVectorClass("N162","strappingOldWarnings.txt",bitClass::TYPE_SHOW_HIDE,styleLabelO,styleLabelG,"","Warnings","","",false,bitLogHisoryClass::typeMsSql,this);main1L->addWidget(bitLogVector.last());bitLogVector.last()->setMinimumHeight(300);
		{//Roller in Manuals
			QGroupBox *rollerInManualGroupBox=new QGroupBox("Roller Entrance Manual");main2L->addWidget(rollerInManualGroupBox);
			QGridLayout *rollerInManualGroupBoxL=new QGridLayout();rollerInManualGroupBoxL->setMargin(2);rollerInManualGroupBox->setLayout(rollerInManualGroupBoxL);
			int row=0;
			bitVector<<new bitClass("N162",7,0,bitClass::TYPE_CHANGE_COLOR,tr("Packet Photocell"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerInManualGroupBoxL->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("FWD"),":/arrowUp.png",oldStrappingTux,"N162",2,823,0,"","N162",8,1,"","","","","",buttonStyle,buttonStyleActive,tr("FWD"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			floatPlcMemoryVector<<new floatPlcMemoryClass(oldStrappingTux,oldStrappingTux->getIp(),"F163",16,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);rollerInManualGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("BWD"),":/arrowDown.png",oldStrappingTux,"N162",2,824,0,"","N162",8,2,"","","","","",buttonStyle,buttonStyleActive,tr("BWD"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,3);
			row++;
			bitVector<<new bitClass("N162",8,7,bitClass::TYPE_CHANGE_COLOR,tr("Rollers Open"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerInManualGroupBoxL->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Open Rollers"),":/openArrows.png",oldStrappingTux,"N162",2,840,0,"","N162",8,3,"","","","","",buttonStyle,buttonStyleActive,tr("Open Rollers"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			floatPlcMemoryVector<<new floatPlcMemoryClass(oldStrappingTux,oldStrappingTux->getIp(),"F163",17,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);rollerInManualGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Close Rollers"),":/closeArrows.png",oldStrappingTux,"N162",2,841,0,"","N162",8,4,"","","","","",buttonStyle,buttonStyleActive,tr("Close Rollers"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,3);
			//bitVector<<new bitClass("N162",8,8,bitClass::TYPE_CHANGE_COLOR,tr("Rollers Closed"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerInManualGroupBoxL->addWidget(bitVector.last(),row,4);
			row++;
			bitVector<<new bitClass("N162",8,9,bitClass::TYPE_CHANGE_COLOR,tr("Height Mesurment Up"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerInManualGroupBoxL->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Height Mesurment Up"),":/arrowUp.png",oldStrappingTux,"N162",2,507,0,"","N162",8,5,"","","","","",buttonStyle,buttonStyleActive,tr("Height Mesurment Up"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			floatPlcMemoryVector<<new floatPlcMemoryClass(oldStrappingTux,oldStrappingTux->getIp(),"F163",18,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);rollerInManualGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Height Mesurment Down"),":/arrowDown.png",oldStrappingTux,"N162",2,506,0,"","N162",8,6,"","","","","",buttonStyle,buttonStyleActive,tr("Height Mesurment Down"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,3);
		}
		//Roller out Manuals
		{
			QGroupBox *rollerOutManualGroupBox=new QGroupBox("Roller Exit Manual");main2L->addWidget(rollerOutManualGroupBox);
			QGridLayout *rollerOutManualGroupBoxL=new QGridLayout();rollerOutManualGroupBoxL->setMargin(2);rollerOutManualGroupBox->setLayout(rollerOutManualGroupBoxL);
			int row=0;
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("FWD"),":/arrowUp.png",oldStrappingTux,"N162",2,818,0,"","N162",8,16,"","","","","",buttonStyle,buttonStyle,tr("FWD"),this);rollerOutManualGroupBoxL->addWidget(pushButtonsVector.last(),row,0,1,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("BWD"),":/arrowDown.png",oldStrappingTux,"N162",2,819,0,"","N162",8,17,"","","","","",buttonStyle,buttonStyle,tr("BWD"),this);rollerOutManualGroupBoxL->addWidget(pushButtonsVector.last(),row,3,1,2);
			row++;
			bitVector<<new bitClass("N162",8,11,bitClass::TYPE_CHANGE_COLOR,tr("Photo Cell 1"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerOutManualGroupBoxL->addWidget(bitVector.last(),row,0);
			bitVector<<new bitClass("N162",8,12,bitClass::TYPE_CHANGE_COLOR,tr("Photo Cell 2"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerOutManualGroupBoxL->addWidget(bitVector.last(),row,1);
			bitVector<<new bitClass("N162",8,13,bitClass::TYPE_CHANGE_COLOR,tr("Photo Cell 3"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerOutManualGroupBoxL->addWidget(bitVector.last(),row,2);
			bitVector<<new bitClass("N162",8,14,bitClass::TYPE_CHANGE_COLOR,tr("Photo Cell 4"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerOutManualGroupBoxL->addWidget(bitVector.last(),row,3);
			bitVector<<new bitClass("N162",8,15,bitClass::TYPE_CHANGE_COLOR,tr("Photo Cell 5"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerOutManualGroupBoxL->addWidget(bitVector.last(),row,4);
		}
		//Roller in/out Manuals
		{
			QGroupBox *rollerInOutManualGroupBox=new QGroupBox("Roller Entrance/Exit Manual");main2L->addWidget(rollerInOutManualGroupBox);
			QGridLayout *rollerInOutManualGroupBoxL=new QGridLayout();rollerInOutManualGroupBoxL->setMargin(2);rollerInOutManualGroupBox->setLayout(rollerInOutManualGroupBoxL);
			int row=0;
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("FWD"),":/arrowUp.png",oldStrappingTux,"N162",2,825,0,"","N162",0,0,"","","","","",buttonStyle,buttonStyle,tr("FWD"),this);rollerInOutManualGroupBoxL->addWidget(pushButtonsVector.last(),row,0,1,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("BWD"),":/arrowDown.png",oldStrappingTux,"N162",2,826	,0,"","N162",0,0,"","","","","",buttonStyle,buttonStyle,tr("BWD"),this);rollerInOutManualGroupBoxL->addWidget(pushButtonsVector.last(),row,3,1,2);
		}
		//Strapping Manuals
		{
			QGroupBox *strappingManualGroupBox=new QGroupBox("Strapping Manual");main3L->addWidget(strappingManualGroupBox);
			QGridLayout *strappingManualGroupBoxL=new QGridLayout();strappingManualGroupBoxL->setMargin(2);strappingManualGroupBox->setLayout(strappingManualGroupBoxL);
			int row=0;//strappingManualGroupBox0L
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Head Up"),":/arrowUp.png",oldStrappingTux,"N162",2,801,0,"","N162",1,3,"","","","","",buttonStyle,buttonStyleActive,tr("Head Up"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Head Down"),":/arrowDown.png",oldStrappingTux,"N162",2,802,0,"","N162",1,4,"","","","","",buttonStyle,buttonStyleActive,tr("Head Down"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			row++;
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Close Wood Holder"),":/arrowLeft.png",oldStrappingTux,"N162",2,803,0,"","N162",8,18,"","","","","",buttonStyle,buttonStyleActive,tr("Close Wood Holder"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Open Wood Holder"),":/arrowRight.png",oldStrappingTux,"N162",2,804,0,"","N162",8,19,"","","","","",buttonStyle,buttonStyleActive,tr("Open Wood Holder"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			row++;
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Small Cover Open"),":/openArrows.png",oldStrappingTux,"N162",2,805,0,"","N162",8,20,"","","","","",buttonStyle,buttonStyleActive,tr("Small Cover Open"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Small Cover Close"),":/closeArrows.png",oldStrappingTux,"N162",2,806,0,"","N162",8,21,"","","","","",buttonStyle,buttonStyleActive,tr("Small Cover Close"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			row++;
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Big Cover Open"),":/openArrows.png",oldStrappingTux,"N162",2,807,0,"","N162",8,22,"","","","","",buttonStyle,buttonStyleActive,tr("Big Cover Open"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Big Cover Close"),":/closeArrows.png",oldStrappingTux,"N162",2,808,0,"","N162",8,23,"","","","","",buttonStyle,buttonStyleActive,tr("Big Cover Close"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
		}
	}
	if (strappingStationExeEnumG==strappingStationExeEnumClass::newStrapping)
	{
		posOut=new posPackingForkClass(0,nullptr,forkTux,strappingNewOutPlcId,1172.f,2385.f,2000.,0.,tr("Strapping Station 2"),nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,this);
		//Mode
		{
			QGroupBox *modeGroupBox=new QGroupBox("Mode");main0L->addWidget(modeGroupBox);
			QVBoxLayout *modeGroupBoxL=new QVBoxLayout();modeGroupBoxL->setMargin(2);modeGroupBox->setLayout(modeGroupBoxL);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",forkTux,"N36",9,814,0,"","N36",13,10,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",forkTux,"N36",9,815,0,"","N36",14,0,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Alarms"),"",forkTux,"N36",9,820,0,"","N36",14,1,"","","","","",buttonStyleActive,buttonStyleRed,tr("Reset Alarms"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Enable Wrapping"),"",forkTux,"N36",9,503,0,"","N36",14,3,"","","","","",buttonStyleActive,buttonStyle,tr("Disable Wrapping"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Packet Measurements"),":/reset.png",forkTux,"N36",9,829,0,"","N36",0,0,"","","","","",buttonStyle,buttonStyle,tr("Reset Packet Measurements"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
			bitVector<<new bitClass("N36",13,20,bitClass::TYPE_CHANGE_COLOR,tr("Ready For Strapping"),tr(""),styleLabelG,styleLabelGr,"","","","",this);modeGroupBoxL->addWidget(bitVector.last());
			bitVector<<new bitClass("N36",13,15,bitClass::TYPE_CHANGE_COLOR,tr("Stop For Strapp"),tr(""),styleLabelG,styleLabelO,"","","","",this);modeGroupBoxL->addWidget(bitVector.last());
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Start Strapping Cycle"),":/return.png",forkTux,"N36",9,834,0,"","N36",13,2,"","","","","",buttonStyle,buttonStyleO,tr("Start Strapping Cycle"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
			bitLogVector<<new bitLogVectorClass("N36","strappingNewAlarms.txt",bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelG,"","Errors","","",false,bitLogHisoryClass::typeMsSql,this);modeGroupBoxL->addWidget(bitLogVector.last());bitLogVector.last()->setMinimumHeight(500);
		}
		//Packet in
		{
			QGroupBox *packetGroupBox=new QGroupBox("Packet In Entrance");main1L->addWidget(packetGroupBox);packetGroupBox->setMinimumWidth(225);
			QVBoxLayout *packetGroupBoxL=new QVBoxLayout();packetGroupBoxL->setMargin(2);packetGroupBox->setLayout(packetGroupBoxL);
			intPlcMemoryVector<<new intPlcMemoryClass(forkTux,forkTux->getIp(),"packetId",strappingNewInPlcId,0,9999,tr("Rack Id:"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,"",this);packetGroupBoxL->addWidget(intPlcMemoryVector.last());		intPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			intPlcMemoryVector<<new intPlcMemoryClass(forkTux,forkTux->getIp(),"packetLength",strappingNewInPlcId,0,7660,tr("Length:"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,"",this);packetGroupBoxL->addWidget(intPlcMemoryVector.last());intPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			floatPlcMemoryVector<<new floatPlcMemoryClass(forkTux,forkTux->getIp(),"F163",1,0,1000,tr("Width[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,0,"",this);packetGroupBoxL->addWidget(floatPlcMemoryVector.last());									floatPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			floatPlcMemoryVector<<new floatPlcMemoryClass(forkTux,forkTux->getIp(),"F163",2,0,1000,tr("Height[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,0,"",this);packetGroupBoxL->addWidget(floatPlcMemoryVector.last());								floatPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
		}
		//Packet Out
		{
			QGroupBox *packetGroupBox=new QGroupBox("Packet In Exit");main1L->addWidget(packetGroupBox);packetGroupBox->setMinimumWidth(225);
			QVBoxLayout *packetGroupBoxL=new QVBoxLayout();packetGroupBoxL->setMargin(2);packetGroupBox->setLayout(packetGroupBoxL);
			intPlcMemoryVector<<new intPlcMemoryClass(forkTux,forkTux->getIp(),"packetId",strappingNewOutPlcId,0,999999,tr("Ticket:"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,"",this);packetGroupBoxL->addWidget(intPlcMemoryVector.last());			 intPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			intPlcMemoryVector<<new intPlcMemoryClass(forkTux,forkTux->getIp(),"packetLength",strappingNewOutPlcId,0,7660,tr("Length:"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,"",this);packetGroupBoxL->addWidget(intPlcMemoryVector.last());		 intPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			intPlcMemoryVector<<new intPlcMemoryClass(forkTux,forkTux->getIp(),"packetWoods",strappingNewOutPlcId,0,5,tr("Woods:"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,"",this);packetGroupBoxL->addWidget(intPlcMemoryVector.last());				 intPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			floatPlcMemoryVector<<new floatPlcMemoryClass(forkTux,forkTux->getIp(),"F163",20,0,1000,tr("Width[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,0,"",this);packetGroupBoxL->addWidget(floatPlcMemoryVector.last());										 floatPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
			floatPlcMemoryVector<<new floatPlcMemoryClass(forkTux,forkTux->getIp(),"F163",21,0,1000,tr("Height[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CAN_CHANGE_VALUE_INLINE,0,"",this);packetGroupBoxL->addWidget(floatPlcMemoryVector.last());										 floatPlcMemoryVector.last()->setSize(spinFontSize,spinSize);
		}
		bitLogVector<<new bitLogVectorClass("N36","strappingNewWarnings.txt",bitClass::TYPE_SHOW_HIDE,styleLabelO,styleLabelG,"","Warnings","","",false,bitLogHisoryClass::typeMsSql,this);main1L->addWidget(bitLogVector.last());bitLogVector.last()->setMinimumHeight(500);

		{//Roller in Manuals
			QGroupBox *rollerInManualGroupBox=new QGroupBox("Roller Entrance Manual");main2L->addWidget(rollerInManualGroupBox);
			QGridLayout *rollerInManualGroupBoxL=new QGridLayout();rollerInManualGroupBoxL->setMargin(2);rollerInManualGroupBox->setLayout(rollerInManualGroupBoxL);
			int row=0;
			bitVector<<new bitClass("N36",14,16,bitClass::TYPE_CHANGE_COLOR,tr("Packet Photocell"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerInManualGroupBoxL->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("FWD"),":/arrowUp.png",forkTux,"N36",9,825,0,"","N36",14,14,"","","","","",buttonStyle,buttonStyleActive,tr("FWD"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			floatPlcMemoryVector<<new floatPlcMemoryClass(forkTux,forkTux->getIp(),"F163",6,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);rollerInManualGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("BWD"),":/arrowDown.png",forkTux,"N36",9,826,0,"","N36",14,15,"","","","","",buttonStyle,buttonStyleActive,tr("BWD"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,3);
			row++;
			bitVector<<new bitClass("N36",14,19,bitClass::TYPE_CHANGE_COLOR,tr("Rollers Open"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerInManualGroupBoxL->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Open Rollers"),":/openArrows.png",forkTux,"N36",9,505,0,"","N36",14,17,"","","","","",buttonStyle,buttonStyleActive,tr("Open Rollers"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			floatPlcMemoryVector<<new floatPlcMemoryClass(forkTux,forkTux->getIp(),"F163",7,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);rollerInManualGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Close Rollers"),":/closeArrows.png",forkTux,"N36",9,504,0,"","N36",14,18,"","","","","",buttonStyle,buttonStyleActive,tr("Close Rollers"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,3);
			bitVector<<new bitClass("N36",14,20,bitClass::TYPE_CHANGE_COLOR,tr("Rollers Closed"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerInManualGroupBoxL->addWidget(bitVector.last(),row,4);
			row++;
			bitVector<<new bitClass("N36",14,23,bitClass::TYPE_CHANGE_COLOR,tr("Height Mesurment Up"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerInManualGroupBoxL->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Height Mesurment Up"),":/arrowUp.png",forkTux,"N36",9,507,0,"","N36",14,22,"","","","","",buttonStyle,buttonStyleActive,tr("Height Mesurment Up"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			floatPlcMemoryVector<<new floatPlcMemoryClass(forkTux,forkTux->getIp(),"F163",8,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);rollerInManualGroupBoxL->addWidget(floatPlcMemoryVector.last(),row,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Height Mesurment Down"),":/arrowDown.png",forkTux,"N36",9,506,0,"","N36",14,21,"","","","","",buttonStyle,buttonStyleActive,tr("Height Mesurment Down"),this);rollerInManualGroupBoxL->addWidget(pushButtonsVector.last(),row,3);
		}
		//Roller out Manuals
		{
			QGroupBox *rollerOutManualGroupBox=new QGroupBox("Roller Exit Manual");main2L->addWidget(rollerOutManualGroupBox);
			QGridLayout *rollerOutManualGroupBoxL=new QGridLayout();rollerOutManualGroupBoxL->setMargin(2);rollerOutManualGroupBox->setLayout(rollerOutManualGroupBoxL);
			int row=0;
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("FWD"),":/arrowUp.png",forkTux,"N36",9,514,0,"","N36",14,24,"","","","","",buttonStyle,buttonStyle,tr("FWD"),this);rollerOutManualGroupBoxL->addWidget(pushButtonsVector.last(),row,0,1,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("BWD"),":/arrowDown.png",forkTux,"N36",9,515	,0,"","N36",14,25,"","","","","",buttonStyle,buttonStyle,tr("BWD"),this);rollerOutManualGroupBoxL->addWidget(pushButtonsVector.last(),row,3,1,2);
			row++;
			bitVector<<new bitClass("N36",14,26,bitClass::TYPE_CHANGE_COLOR,tr("Photo Cell 1"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerOutManualGroupBoxL->addWidget(bitVector.last(),row,0);
			bitVector<<new bitClass("N36",14,27,bitClass::TYPE_CHANGE_COLOR,tr("Photo Cell 2"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerOutManualGroupBoxL->addWidget(bitVector.last(),row,1);
			bitVector<<new bitClass("N36",14,28,bitClass::TYPE_CHANGE_COLOR,tr("Photo Cell 3"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerOutManualGroupBoxL->addWidget(bitVector.last(),row,2);
			bitVector<<new bitClass("N36",14,29,bitClass::TYPE_CHANGE_COLOR,tr("Photo Cell 4"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerOutManualGroupBoxL->addWidget(bitVector.last(),row,3);
			bitVector<<new bitClass("N36",14,30,bitClass::TYPE_CHANGE_COLOR,tr("Photo Cell 5"),tr(""),styleLabelG,styleLabelGr,"","","","",this);rollerOutManualGroupBoxL->addWidget(bitVector.last(),row,4);
		}
		//Roller in/out Manuals
		{
			QGroupBox *rollerInOutManualGroupBox=new QGroupBox("Roller Entrance/Exit Manual");main2L->addWidget(rollerInOutManualGroupBox);
			QGridLayout *rollerInOutManualGroupBoxL=new QGridLayout();rollerInOutManualGroupBoxL->setMargin(2);rollerInOutManualGroupBox->setLayout(rollerInOutManualGroupBoxL);
			int row=0;
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("FWD"),":/arrowUp.png",forkTux,"N36",9,818,0,"","N36",0,0,"","","","","",buttonStyle,buttonStyle,tr("FWD"),this);rollerInOutManualGroupBoxL->addWidget(pushButtonsVector.last(),row,0,1,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("BWD"),":/arrowDown.png",forkTux,"N36",9,819	,0,"","N36",0,0,"","","","","",buttonStyle,buttonStyle,tr("BWD"),this);rollerInOutManualGroupBoxL->addWidget(pushButtonsVector.last(),row,3,1,2);
		}
		//Wrapping Manuals
		{
			QGroupBox *wrappingManualGroupBox=new QGroupBox("Wrapping Manual");main2L->addWidget(wrappingManualGroupBox);
			QVBoxLayout *wrappingManualGroupBoxL=new QVBoxLayout();wrappingManualGroupBoxL->setMargin(2);wrappingManualGroupBox->setLayout(wrappingManualGroupBoxL);
			QGridLayout *wrappingManualGroupBox0L=new QGridLayout();wrappingManualGroupBox0L->setMargin(2);wrappingManualGroupBoxL->addLayout(wrappingManualGroupBox0L);
			QGridLayout *wrappingManualGroupBox1L=new QGridLayout();wrappingManualGroupBox1L->setMargin(2);wrappingManualGroupBoxL->addLayout(wrappingManualGroupBox1L);
			QGridLayout *wrappingManualGroupBox2L=new QGridLayout();wrappingManualGroupBox2L->setMargin(2);wrappingManualGroupBoxL->addLayout(wrappingManualGroupBox2L);
			int row=0;//wrappingManualGroupBox1L
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("To Change Streach Position"),":/reset.png",forkTux,"N36",9,500,0,"","N36",13,26,"","","","","",buttonStyle,buttonStyleActive,tr("To Change Streach Position"),this);wrappingManualGroupBox0L->addWidget(pushButtonsVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("To Holder Position"),":/reset.png",forkTux,"N36",9,501,0,"","N36",13,25,"","","","","",buttonStyle,buttonStyleActive,tr("To Holder Position"),this);wrappingManualGroupBox0L->addWidget(pushButtonsVector.last(),row,1);
			row++;
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Rotate Head"),":/reset.png",forkTux,"N36",9,502,0,"","N36",14,2,"","","","","",buttonStyle,buttonStyleActive,tr("Rotate Head"),this);wrappingManualGroupBox0L->addWidget(pushButtonsVector.last(),row,0,1,2);
			row++;
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Wrapping Head Go To Zero To Change Streach"),":/arrowUp.png",forkTux,"N36",9,700,0,"","N36",13,18,"","","","","",buttonStyle,buttonStyleActive,tr("Wrapping Head Go To Zero To Change Streach"),this);wrappingManualGroupBox0L->addWidget(pushButtonsVector.last(),row,0,1,2);
			row=0;//wrappingManualGroupBox1L
			bitVector<<new bitClass("N36",14,7,bitClass::TYPE_CHANGE_COLOR,tr("Head Left"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox1L->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Head Left"),":/arrowLeft.png",forkTux,"N36",9,508,0,"","N36",14,6,"","","","","",buttonStyle,buttonStyleActive,tr("Head Left"),this);wrappingManualGroupBox1L->addWidget(pushButtonsVector.last(),row,1);
			floatPlcMemoryVector<<new floatPlcMemoryClass(forkTux,forkTux->getIp(),"F163",4,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);wrappingManualGroupBox1L->addWidget(floatPlcMemoryVector.last(),row,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Head Right"),":/arrowRight.png",forkTux,"N36",9,509,0,"","N36",14,8,"","","","","",buttonStyle,buttonStyleActive,tr("Head Right"),this);wrappingManualGroupBox1L->addWidget(pushButtonsVector.last(),row,3);
			bitVector<<new bitClass("N36",14,9,bitClass::TYPE_CHANGE_COLOR,tr("Head Right"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox1L->addWidget(bitVector.last(),row,4);
			row++;
			bitVector<<new bitClass("N36",14,11,bitClass::TYPE_CHANGE_COLOR,tr("Head Up"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox1L->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Head Up"),":/arrowUp.png",forkTux,"N36",9,511,0,"","N36",14,10,"","","","","",buttonStyle,buttonStyleActive,tr("Head Up"),this);wrappingManualGroupBox1L->addWidget(pushButtonsVector.last(),row,1);
			floatPlcMemoryVector<<new floatPlcMemoryClass(forkTux,forkTux->getIp(),"F163",5,0,100000.,tr("Position[mm]"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);wrappingManualGroupBox1L->addWidget(floatPlcMemoryVector.last(),row,2);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Head Down"),":/arrowDown.png",forkTux,"N36",9,510,0,"","N36",14,12,"","","","","",buttonStyle,buttonStyleActive,tr("Head Down"),this);wrappingManualGroupBox1L->addWidget(pushButtonsVector.last(),row,3);
			bitVector<<new bitClass("N36",14,13,bitClass::TYPE_CHANGE_COLOR,tr("Head Down"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox1L->addWidget(bitVector.last(),row,4);
			row=0;//wrappingManualGroupBox2L
			bitVector<<new bitClass("N36",14,5,bitClass::TYPE_CHANGE_COLOR,tr("Home Position"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox2L->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Film Pincer To Home Position"),":/arrowDown.png",forkTux,"N36",9,513,0,"","N36",0,0,"","","","","",buttonStyle,buttonStyle,tr("Film Pincer To Home Position"),this);wrappingManualGroupBox2L->addWidget(pushButtonsVector.last(),row,1);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Film Pincer To Cut Position"),":/arrowUp.png",forkTux,"N36",9,512,0,"","N36",0,0,"","","","","",buttonStyle,buttonStyle,tr("Film Pincer To Cut Position"),this);wrappingManualGroupBox2L->addWidget(pushButtonsVector.last(),row,2);
			bitVector<<new bitClass("N36",14,4,bitClass::TYPE_CHANGE_COLOR,tr("Cut Position"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox2L->addWidget(bitVector.last(),row,3);
			row++;
			bitVector<<new bitClass("N36",15,3,bitClass::TYPE_CHANGE_COLOR,tr("Film Pincer Out"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox2L->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Film Pincer Out"),":/arrowLeft.png",forkTux,"N36",9,601,0,"","N36",15,1,"","","","","",buttonStyle,buttonStyleActive,tr("Film Pincer Out"),this);wrappingManualGroupBox2L->addWidget(pushButtonsVector.last(),row,1);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Film Pincer In"),":/arrowRight.png",forkTux,"N36",9,600,0,"","N36",15,0,"","","","","",buttonStyle,buttonStyleActive,tr("Film Pincer In"),this);wrappingManualGroupBox2L->addWidget(pushButtonsVector.last(),row,2);
			bitVector<<new bitClass("N36",15,2,bitClass::TYPE_CHANGE_COLOR,tr("Film Pincer In"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox2L->addWidget(bitVector.last(),row,3);
			row++;
			bitVector<<new bitClass("N36",15,6,bitClass::TYPE_CHANGE_COLOR,tr("Film Pincer to HCA"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox2L->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Film Pincer to HCA"),":/arrowLeft.png",forkTux,"N36",9,602,0,"","N36",15,4,"","","","","",buttonStyle,buttonStyleActive,tr("Film Pincer to HCA"),this);wrappingManualGroupBox2L->addWidget(pushButtonsVector.last(),row,1);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Film Pincer to Presses"),":/arrowRight.png",forkTux,"N36",9,603,0,"","N36",15,5,"","","","","",buttonStyle,buttonStyleActive,tr("Film Pincer to Presses"),this);wrappingManualGroupBox2L->addWidget(pushButtonsVector.last(),row,2);
			bitVector<<new bitClass("N36",15,7,bitClass::TYPE_CHANGE_COLOR,tr("Film Pincer to Presses"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox2L->addWidget(bitVector.last(),row,3);
			row++;
			bitVector<<new bitClass("N36",15,11,bitClass::TYPE_CHANGE_COLOR,tr("Film Pincer Open"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox2L->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Film Pincer Open"),":/arrowLeft.png",forkTux,"N36",9,605,0,"","N36",15,9,"","","","","",buttonStyle,buttonStyleActive,tr("Film Pincer Open"),this);wrappingManualGroupBox2L->addWidget(pushButtonsVector.last(),row,1);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Film Pincer Close"),":/arrowRight.png",forkTux,"N36",9,604,0,"","N36",15,8,"","","","","",buttonStyle,buttonStyleActive,tr("Film Pincer Close"),this);wrappingManualGroupBox2L->addWidget(pushButtonsVector.last(),row,2);
			bitVector<<new bitClass("N36",15,10,bitClass::TYPE_CHANGE_COLOR,tr("Film Pincer Close"),tr(""),styleLabelG,styleLabelGr,"","","","",this);wrappingManualGroupBox2L->addWidget(bitVector.last(),row,3);
		}
		//Strapping Manuals
		{
			QGroupBox *strappingManualGroupBox=new QGroupBox("Strapping Manual");main2L->addWidget(strappingManualGroupBox);
			QGridLayout *strappingManualGroupBoxL=new QGridLayout();strappingManualGroupBoxL->setMargin(2);strappingManualGroupBox->setLayout(strappingManualGroupBoxL);
			int row=0;//strappingManualGroupBox0L
			bitVector<<new bitClass("N36",13,3,bitClass::TYPE_CHANGE_COLOR,tr("Head Up"),tr(""),styleLabelG,styleLabelGr,"","","","",this);strappingManualGroupBoxL->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Head Up"),":/arrowUp.png",forkTux,"N36",9,801,0,"","N36",16,0,"","","","","",buttonStyle,buttonStyleActive,tr("Head Up"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Head Down"),":/arrowDown.png",forkTux,"N36",9,802,0,"","N36",16,1,"","","","","",buttonStyle,buttonStyleActive,tr("Head Down"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,2);
			bitVector<<new bitClass("N36",13,4,bitClass::TYPE_CHANGE_COLOR,tr("Head Down"),tr(""),styleLabelG,styleLabelGr,"","","","",this);strappingManualGroupBoxL->addWidget(bitVector.last(),row,3);
			row++;
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Close Wood Holder"),":/closeArrows.png",forkTux,"N36",9,803,0,"","N36",16,3,"","","","","",buttonStyle,buttonStyleActive,tr("Close Wood Holder"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Open Wood Holder"),":/openArrows.png",forkTux,"N36",9,804,0,"","N36",16,2,"","","","","",buttonStyle,buttonStyleActive,tr("Open Wood Holder"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,2);
			bitVector<<new bitClass("N36",13,0,bitClass::TYPE_CHANGE_COLOR,tr("Open Wood Holder"),tr(""),styleLabelG,styleLabelGr,"","","","",this);strappingManualGroupBoxL->addWidget(bitVector.last(),row,3);
			row++;
			bitVector<<new bitClass("N36",16,6,bitClass::TYPE_CHANGE_COLOR,tr("Wood Mechanism In"),tr(""),styleLabelG,styleLabelGr,"","","","",this);strappingManualGroupBoxL->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Wood Mechanism In"),":/arrowUp.png",forkTux,"N36",9,830,0,"","N36",16,4,"","","","","",buttonStyle,buttonStyleActive,tr("Wood Mechanism In"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Wood Mechanism Out"),":/arrowDown.png",forkTux,"N36",9,831,0,"","N36",16,5,"","","","","",buttonStyle,buttonStyleActive,tr("Wood Mechanism Out"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,2);
			bitVector<<new bitClass("N36",16,7,bitClass::TYPE_CHANGE_COLOR,tr("Wood Mechanism Out"),tr(""),styleLabelG,styleLabelGr,"","","","",this);strappingManualGroupBoxL->addWidget(bitVector.last(),row,3);
			row++;
			bitVector<<new bitClass("N36",13,5,bitClass::TYPE_CHANGE_COLOR,tr("Small Cover Open"),tr(""),styleLabelG,styleLabelGr,"","","","",this);strappingManualGroupBoxL->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Small Cover Open"),":/openArrows.png",forkTux,"N36",9,805,0,"","N36",16,8,"","","","","",buttonStyle,buttonStyleActive,tr("Small Cover Open"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Small Cover Close"),":/closeArrows.png",forkTux,"N36",9,806,0,"","N36",16,9,"","","","","",buttonStyle,buttonStyleActive,tr("Small Cover Close"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,2);
			bitVector<<new bitClass("N36",13,6,bitClass::TYPE_CHANGE_COLOR,tr("Small Cover Close"),tr(""),styleLabelG,styleLabelGr,"","","","",this);strappingManualGroupBoxL->addWidget(bitVector.last(),row,3);
			row++;
			bitVector<<new bitClass("N36",13,7,bitClass::TYPE_CHANGE_COLOR,tr("Big Cover Open"),tr(""),styleLabelG,styleLabelGr,"","","","",this);strappingManualGroupBoxL->addWidget(bitVector.last(),row,0);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Big Cover Open"),":/openArrows.png",forkTux,"N36",9,807,0,"","N36",16,10,"","","","","",buttonStyle,buttonStyleActive,tr("Big Cover Open"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,1);
			pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Big Cover Close"),":/closeArrows.png",forkTux,"N36",9,808,0,"","N36",16,11,"","","","","",buttonStyle,buttonStyleActive,tr("Big Cover Close"),this);strappingManualGroupBoxL->addWidget(pushButtonsVector.last(),row,2);
			bitVector<<new bitClass("N36",13,8,bitClass::TYPE_CHANGE_COLOR,tr("Big Cover Close"),tr(""),styleLabelG,styleLabelGr,"","","","",this);strappingManualGroupBoxL->addWidget(bitVector.last(),row,3);
		}
	}
	main0L->addStretch(1);
	main1L->addStretch(1);
	main2L->addStretch(1);
	main3L->addStretch(1);
}
stationWdgtClass::~stationWdgtClass()
{
}
QString stationWdgtClass::getName()
{
	if (strappingStationExeEnumG==strappingStationExeEnumClass::none)
		return tr("No Strapping");
	if (strappingStationExeEnumG==strappingStationExeEnumClass::oldStrapping)
		return tr("Old Strapping");
	if (strappingStationExeEnumG==strappingStationExeEnumClass::newStrapping)
		return tr("New Strapping");
	return QString();
}
void stationWdgtClass::definePacketInExit()
{
	if (strappingStationExeEnumG==strappingStationExeEnumClass::oldStrapping)
	{
		int packetId=oldStrappingTux->readFloatValue("F163",11);
		int packetLength=oldStrappingTux->readFloatValue("F163",7);
		int packetWoods=oldStrappingTux->readFloatValue("F163",10);
		int packetWidth=oldStrappingTux->readFloatValue("F163",8);
		int packetHeight=oldStrappingTux->readFloatValue("F163",9);
		int packetWeigt=0.;
		posDlgPackingForkClass *posDlg=new posDlgPackingForkClass(tr("Define Packet"),packetId,true,false,this);
		posDlg->setData(packetId,packetLength,packetWoods,packetWidth,packetHeight,packetWeigt);
		if (posDlg->exec()==QDialog::Accepted)
		{
			oldStrappingTux->writeFloat("F163",11,posDlg->getPacketTicket());
			oldStrappingTux->writeFloat("F163", 7,posDlg->getPacketLength());
			oldStrappingTux->writeFloat("F163",10,posDlg->getPacketWoods());
			oldStrappingTux->writeFloat("F163", 8,posDlg->getPacketWidth());
			oldStrappingTux->writeFloat("F163", 9,posDlg->getPacketHeight());
			
			forkTux->writeInteger("packetId"		,strappingOldOutPlcId,posDlg->getPacketTicket());
			forkTux->writeInteger("packetLength",strappingOldOutPlcId,posDlg->getPacketLength());
			forkTux->writeInteger("packetWoods"	,strappingOldOutPlcId,posDlg->getPacketWoods());
		}
	}
	if (strappingStationExeEnumG==strappingStationExeEnumClass::newStrapping)
	{ 
		int packetId=forkTux->readIntegerValue("packetId",strappingNewOutPlcId);
		int packetLength=forkTux->readIntegerValue("packetLength",strappingNewOutPlcId);
		int packetWoods=forkTux->readIntegerValue("packetWoods",strappingNewOutPlcId);
		int packetWidth=forkTux->readFloatValue("F163",20);
		int packetHeight=forkTux->readFloatValue("F163",21);
		int packetWeigt=0.;
		posDlgPackingForkClass *posDlg=new posDlgPackingForkClass(tr("Define Packet"),packetId,true,false,this);
		posDlg->setData(packetId,packetLength,packetWoods,packetWidth,packetHeight,packetWeigt);
		if (posDlg->exec()==QDialog::Accepted)
		{
			forkTux->writeInteger("packetId"		,strappingNewOutPlcId,posDlg->getPacketTicket());
			forkTux->writeInteger("packetLength",strappingNewOutPlcId,posDlg->getPacketLength());
			forkTux->writeInteger("packetWoods"	,strappingNewOutPlcId,posDlg->getPacketWoods());
			forkTux->writeFloat("F163",20,posDlg->getPacketWidth());
			forkTux->writeFloat("F163",21,posDlg->getPacketHeight());
		}
	}
}
void stationWdgtClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==forkTux->getIp())
	{
		for (int i=0;i<bitVector.count();i++)
			bitVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<intPlcMemoryVector.count();i++)
			intPlcMemoryVector[i]->updateValue(ip,table,index,value.toInt());
		for (int i=0;i<bitLogVector.count();i++)
			bitLogVector[i]->updateValue(table,index,value.toInt());
		if (table=="F163")
		{
			for (int i=0;i<floatPlcMemoryVector.count();i++)
				floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toFloat());
		}
		if ((table=="packetId")||(table=="packetLength")||(table=="packetWoods"))
		{
			posOut->dataInPlcChanged(ip,table,index,value);
		}
	}
	if (oldStrappingTux&&(ip==oldStrappingTux->getIp()))
	{
		if (table=="N162")
		{
			for (int i=0;i<bitVector.count();i++)
				bitVector[i]->updateValue(table,index,value.toInt());
			for (int i=0;i<pushButtonsVector.count();i++)
				pushButtonsVector[i]->updateValue(table,index,value.toInt());
			for (int i=0;i<intPlcMemoryVector.count();i++)
				intPlcMemoryVector[i]->updateValue(ip,table,index,value.toInt());
			for (int i=0;i<bitLogVector.count();i++)
				bitLogVector[i]->updateValue(table,index,value.toInt());
		}
		if (table=="F163")
		{
			for (int i=0;i<floatPlcMemoryVector.count();i++)
				floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toFloat());
		}
	}
}
void stationWdgtClass::placeHolder()
{
}
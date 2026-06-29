#include "craneM.h"
packetCraneManualClass::packetCraneManualClass(tcpServerClass *tcpServer_,QWidget *p):QWidget(p)
{
	manualCassetteMovement=NULL;
	alarmsIndex4Value=alarmsIndex5Value=warningsIndex0Value=warningsIndex1Value=warningsIndex8Value=0;
	//this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px}  QCheckBox{font-size:%1px}").arg(12));
	tcpServer=tcpServer_;
	conveyorsManual=new conveyorsManualClass(tcpServer,this);
	
	QVBoxLayout *topMainLayout=new QVBoxLayout();
  setLayout(topMainLayout);	

	QVBoxLayout *mainLayout=new QVBoxLayout();
	QSplitter *mainSplitter=new QSplitter(Qt::Vertical,this);mainLayout->addWidget(mainSplitter);
	QSplitter *upperSplitter=new QSplitter(Qt::Horizontal,this);mainSplitter->addWidget(upperSplitter);

	/////////////////////////////////////////////////////////////////////////area
	QImage image(":/packetCraneLayout.png");
	QLabel *imageLabel = new QLabel;
	imageLabel->setMinimumWidth(450);
	imageLabel->setMinimumHeight(563);
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setAlignment(Qt::AlignCenter);

	QWidget *areaWidget=new QWidget();
	QHBoxLayout *areaLayout=new QHBoxLayout();
	areaLayout->addWidget(imageLabel);
	areaWidget->setLayout(areaLayout);
	QScrollArea *area=new QScrollArea();
	area->setAlignment(Qt::AlignCenter);
	area->setBackgroundRole(QPalette::Shadow);
	area->setWidget(areaWidget);

	QWidget *leftWidgetButtons= new QWidget();
	QWidget *rightWidgetButtons=new QWidget();

	QVBoxLayout *leftButtonsLayout=new QVBoxLayout();
	leftWidgetButtons->setLayout(leftButtonsLayout);
	leftButtonsLayout->setMargin(1);

	QVBoxLayout *rightButtonsLayout=new QVBoxLayout();
	rightWidgetButtons->setLayout(rightButtonsLayout);
	rightButtonsLayout->setMargin(1);

	QPushButton *showConveyorsManualButton=new QPushButton("Conveyors",imageLabel);
	showConveyorsManualButton->setGeometry(168,530,100,30);
	connect(showConveyorsManualButton,SIGNAL(clicked()),this,SLOT(onShowConveyorsManualsSlot()));
	
	myQPushButton *runInMaintenaceModeAlarmsPushButton=new myQPushButton(tcpServer,"N150|20|145","N150|20|0",tr(""),imageLabel);
	runInMaintenaceModeAlarmsPushButton->setIcon(QIcon(":/running.png"));
	runInMaintenaceModeAlarmsPushButton->setIconSize(QSize(30,30));
	runInMaintenaceModeAlarmsPushButton->setGeometry(202,290,35,35);

	upperSplitter->addWidget(leftWidgetButtons);
	upperSplitter->addWidget(area);
	upperSplitter->addWidget(rightWidgetButtons);
	//////////////////////////////////////////////////////////////////////alarms and warning lists
	QWidget *listWidget=new QWidget();
	QHBoxLayout *listLayout=new QHBoxLayout();listWidget->setLayout(listLayout);
	QVBoxLayout *warningsListLayout=new QVBoxLayout(); listLayout->addLayout(warningsListLayout);
	QVBoxLayout *alarmsListLayout=new QVBoxLayout(); listLayout->addLayout(alarmsListLayout);

	warningsListLayout->addWidget(new QLabel(tr("Warnings"),this));
	warningsSignalListWidget = new QListWidget(this);
	warningsListLayout->addWidget(warningsSignalListWidget);
	warningsListLayout->addWidget(showWarningsHistoryBtn=new QPushButton(tr("Show warnings history")));
	connect(showWarningsHistoryBtn,SIGNAL(clicked()),this,SLOT(showWarningsHistoryBtnSlot()));
	warningsListLayout->blockSignals(true);

	alarmsListLayout->addWidget(new QLabel(tr("Alarms"),this));
	alarmsSignalListWidget = new QListWidget(this);
	alarmsListLayout->addWidget(alarmsSignalListWidget);
	alarmsListLayout->addWidget(showAlarmsHistoryBtn=new QPushButton(tr("Show alarms history")));
	connect(showAlarmsHistoryBtn,SIGNAL(clicked()),this,SLOT(showAlarmsHistoryBtnSlot()));
	alarmsSignalListWidget->blockSignals(true);


	alarmsIndex4StringList<<"N152[4].00:Reset of all axis requiered";                                                                                  //N152[4].00
	alarmsIndex4StringList<<"N152[4].01:Anchor not out.Check valve.Check magnetic switches.Check air pressure.";												                                                      //N152[4].01		
	alarmsIndex4StringList<<"N152[4].02:Hooks carriage can not push cassette to cell (import)";												                //N152[4].02
	alarmsIndex4StringList<<"N152[4].03:Vertical motion overspeed";																																		//N152[4].03
	alarmsIndex4StringList<<"N152[4].04:Cassette not out of cell (export) ";																						              //N152[4].04
	alarmsIndex4StringList<<"N152[4].05:Hooks carriage pneumatic valve feedback.Check valve.Check magnetic switches.Check air pressure.";																			                //N152[4].05
	alarmsIndex4StringList<<"N152[4].06:";															                                              //N152[4].06
	alarmsIndex4StringList<<"N152[4].07:";																							                                              //N152[4].07
	alarmsIndex4StringList<<"N152[4].08:Brake relay contactor K11F-K12F  feedback ";																		              //N152[4].08
	alarmsIndex4StringList<<"N152[4].09:";				//N152[4].09																							                                              //N152[4].09
	alarmsIndex4StringList<<"N152[4].10:Hooks not centered or anchors not in or cassette moved while crane is moving fwd-bwd.";				//N152[4].10																							                                              //N152[4].10
	alarmsIndex4StringList<<"N152[4].11:Alarm from anticollision photocells ( I 305).";																							                                              //N152[4].11
	alarmsIndex4StringList<<"N152[4].12:Alarm from anticollision photocells ( I 305).";																							                                              //N152[4].12
	alarmsIndex4StringList<<"N152[4].13:Hooks carriage not carrying cassette ";																			                  //N152[4].13
	alarmsIndex4StringList<<"N152[4].14:";                                                                      //N152[4].14
	alarmsIndex4StringList<<"N152[4].15:Target calculation fault.Check plc program.";                                                                                            //N152[4].15
	alarmsIndex4StringList<<"N152[4].16:";				                                                                  //N152[4].16
	alarmsIndex4StringList<<"N152[4].17:Conveyor 24 with cassette but no proximity activated.";									             					//N152[4].17
	alarmsIndex4StringList<<"N152[4].18:Conveyor 25 with cassette but no proximity activated.";																				//N152[4].18
	alarmsIndex4StringList<<"N152[4].19:";																							            //N152[4].19
	alarmsIndex4StringList<<"N152[4].20:Time exceeded at step 1";																							                        //N152[4].20
	alarmsIndex4StringList<<"N152[4].21:Time exceeded at step 2";																							                        //N152[4].21
	alarmsIndex4StringList<<"N152[4].22:Time exceeded at step 3";																						                          //N152[4].22
	alarmsIndex4StringList<<"N152[4].23:Time exceeded at step 4";																						                          //N152[4].23
	alarmsIndex4StringList<<"N152[4].24:Time exceeded at step 5";																						                          //N152[4].24
	alarmsIndex4StringList<<"N152[4].25:Time exceeded at step 6";																						                          //N152[4].25
	alarmsIndex4StringList<<"N152[4].26:Time exceeded at step 7";																						                          //N152[4].26
	alarmsIndex4StringList<<"N152[4].27:Time exceeded at step 8";																						                          //N152[4].27
	alarmsIndex4StringList<<"N152[4].28:";																						                                                //N152[4].28
	alarmsIndex4StringList<<"N152[4].29:";																						                                                //N152[4].29
	alarmsIndex4StringList<<"N152[4].30:";																						                                                //N152[4].30
	alarmsIndex4StringList<<"N152[4].31:";	
																																																									              
	alarmsIndex5StringList<<"N152[5].00:Fault moving robot carriage up";                                                              //N152[5].00
	alarmsIndex5StringList<<"N152[5].01:Fault moving robot carriage down";																							              //N152[5].01
	alarmsIndex5StringList<<"N152[5].02:Underspeed detection on robot carriage vertical motion";												              //N152[5].02
	alarmsIndex5StringList<<"N152[5].03:Overspeed detection on robot carriage vertical motion";											                  //N152[5].03
	alarmsIndex5StringList<<"N152[5].04:Underspeed detection on robot horizontal motion";														                  //N152[5].04
	alarmsIndex5StringList<<"N152[5].05:Overspeed detection on robot horizontal motion";																              //N152[5].05
	alarmsIndex5StringList<<"N152[5].06:Underspeed detection on hooks carriage  motion";																              //N152[5].06
	alarmsIndex5StringList<<"N152[5].07:Overspeed detection on hooks carriage  motion";																                //N152[5].07
	alarmsIndex5StringList<<"N152[5].08:Fault on accumulator charging ";																								              //N152[5].08
	alarmsIndex5StringList<<"N152[5].09:Thermal overload Q1,Q11,Q12 or inverter U1 fault";															              //N152[5].09
	alarmsIndex5StringList<<"N152[5].10:Thermal overload Q2,Q21,Q22,Q23,Q24 or inverter U2 fault";											              //N152[5].10
	alarmsIndex5StringList<<"N152[5].11:Thermal overload Q3,Q4 or inverter U3 fault";																	                //N152[5].11
	alarmsIndex5StringList<<"N152[5].12:Power supply 24V DC or 110V AC fault";																					              //N152[5].12
	alarmsIndex5StringList<<"N152[5].13:Communication with fork lift crane lost";																											//N152[5].14
	alarmsIndex5StringList<<"N152[5].14:Check encoder of horizontal motion 1A1.Encoder has not been reseted correctly.";							//N152[5].15
	alarmsIndex5StringList<<"N152[5].15:Check encoder of vertical motion 1A0.Encoder has not been reseted correctly.";								//N152[5].16
	alarmsIndex5StringList<<"N152[5].16:Check encoder of hooks carriage motion 2A0.Encoder has not been reseted correctly.";					//N152[5].17
	alarmsIndex5StringList<<"N152[5].17:Encoder 1A0 fault.Count pulses with brake closed";																					  //N152[5].13
	alarmsIndex5StringList<<"N152[5].18:Encoder 1A1 fault.Count pulses with brake closed";																					  //N152[5].18
	alarmsIndex5StringList<<"N152[5].19:Encoder 2A0 fault.Count pulses with brake closed";																					  //N152[5].19
	alarmsIndex5StringList<<"N152[5].20:Hydraulic alarm.Check oil pressure.";																																														//N152[5].20
	alarmsIndex5StringList<<"N152[5].21:Input 329:Contact cable chain safety IN_CT_SAFETY";																						//N152[5].21
	alarmsIndex5StringList<<"N152[5].22:";																																							              //N152[5].22
	alarmsIndex5StringList<<"N152[5].23:";																																							              //N152[5].23
	alarmsIndex5StringList<<"N152[5].24:";																																							              //N152[5].24
	alarmsIndex5StringList<<"N152[5].25:";																																							              //N152[5].25
	alarmsIndex5StringList<<"N152[5].26:";																																							              //N152[5].26
	alarmsIndex5StringList<<"N152[5].27:";																																							              //N152[5].27
	alarmsIndex5StringList<<"N152[5].28:";																																							              //N152[5].28
	alarmsIndex5StringList<<"N152[5].29:";																																							              //N152[5].29
	alarmsIndex5StringList<<"N152[5].30:";																																							              //N152[5].30
	alarmsIndex5StringList<<"N152[5].31:";	
	//																																																								                  
	warningsIndex8StringList<<"N152[8].00:Conveyor 24 not in fork lift crane position";
	warningsIndex8StringList<<"N152[8].01:Conveyor 24 not in robot position";
	warningsIndex8StringList<<"N152[8].02:Conveyor 25 not in fork lift crane position";
	warningsIndex8StringList<<"N152[8].03:Conveyor 25 not in robot position";
	warningsIndex8StringList<<"N152[8].04:Out of target on unload position ";
	warningsIndex8StringList<<"N152[8].05:Out of home position or 25 out of position";
	warningsIndex8StringList<<"N152[8].06:Out of home position or 24 out of position";
	warningsIndex8StringList<<"N152[8].07:Out of home position or 24-25 out of position";
	warningsIndex8StringList<<"N152[8].08:Air pressure low";
	warningsIndex8StringList<<"N152[8].09:Communications with fork lift crane lost.";
	warningsIndex8StringList<<"N152[8].10:Auto switch on OP ";
	warningsIndex8StringList<<"N152[8].11:Cable chain jumper cable disconnected.";
	warningsIndex8StringList<<"N152[8].12:Mission time exceeded .";
	warningsIndex8StringList<<"N152[8].13:Warning anticollision photecells (I 305)  activated.";
	warningsIndex8StringList<<"N152[8].14";
	warningsIndex8StringList<<"N152[8].15";
	warningsIndex8StringList<<"N152[8].16";
	warningsIndex8StringList<<"N152[8].17";
	warningsIndex8StringList<<"N152[8].18";
	warningsIndex8StringList<<"N152[8].19";
	warningsIndex8StringList<<"N152[8].20";
	warningsIndex8StringList<<"N152[8].21";
	warningsIndex8StringList<<"N152[8].22";
	warningsIndex8StringList<<"N152[8].23";
	warningsIndex8StringList<<"N152[8].24";
	warningsIndex8StringList<<"N152[8].25";
	warningsIndex8StringList<<"N152[8].26";
	warningsIndex8StringList<<"N152[8].27";
	warningsIndex8StringList<<"N152[8].28";
	warningsIndex8StringList<<"N152[8].29";
	warningsIndex8StringList<<"N152[8].30";
	warningsIndex8StringList<<"N152[8].31";

	//////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////Left Buttons Layout////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
  //horizontalGroupBox
  QGroupBox *horizontalGroupBox=new QGroupBox(tr("Horizontal"));leftButtonsLayout->addWidget(horizontalGroupBox);
	QGridLayout *horizontalGroupBoxLayout=new QGridLayout();horizontalGroupBox->setLayout(horizontalGroupBoxLayout);
	//1st column
	horizontalGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|105","N150|20|0",tr("Forward"),NULL)  ,0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	horizontalGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|106","N150|20|0",tr("Backwards"),NULL),1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	horizontalGroupBoxLayout->addWidget(horizontalResetPushbutton=new myQPushButton(tcpServer,"N150|20|115","N150|20|0",tr("Reset backwards"),NULL),2,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	horizontalResetLabel=new QLabel(tr("Reset backwards"));horizontalResetLabel->setMargin(5);horizontalGroupBoxLayout->addWidget(horizontalResetLabel,3,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	horizontalResetLabel->setMargin(5);horizontalResetLabel->setMinimumWidth(100);
	//2nd column
	horizontalGroupBoxLayout->addWidget(horizontalActualPosLabel=  new QLabel(tr("Actual position")),0,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	horizontalGroupBoxLayout->addWidget(horizontalTargetPosLabel=  new QLabel(tr("Target position")),1,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	horizontalGroupBoxLayout->addWidget(horizontalRefSpeedLabel=   new QLabel(tr("Reference speed")),2,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	horizontalGroupBoxLayout->addWidget(horizontalActualSpeedLabel=new QLabel(tr("Actual speed   ")),3,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	horizontalActualPosLabel		->setMargin(5); horizontalActualPosLabel		->setMinimumWidth(100);
	horizontalTargetPosLabel		->setMargin(5);	horizontalTargetPosLabel		->setMinimumWidth(100);
	horizontalRefSpeedLabel			->setMargin(5);	horizontalRefSpeedLabel			->setMinimumWidth(100);
	horizontalActualSpeedLabel	->setMargin(5);	horizontalActualSpeedLabel	->setMinimumWidth(100);
	//3rd column
	horizontalActualPosLabel  =new QLabel(tr("99999.9 mm"));horizontalGroupBoxLayout->addWidget(horizontalActualPosLabel  ,0,2,1,1,Qt::AlignLeft|Qt::AlignCenter);   horizontalActualPosLabel  ->setMinimumWidth(100);
	horizontalTargetPosLabel  =new QLabel(tr("99999.9 mm"));horizontalGroupBoxLayout->addWidget(horizontalTargetPosLabel  ,1,2,1,1,Qt::AlignLeft|Qt::AlignCenter);   horizontalTargetPosLabel  ->setMinimumWidth(100);
	horizontalRefSpeedLabel   =new QLabel(tr("99999.9 m/min"));horizontalGroupBoxLayout->addWidget(horizontalRefSpeedLabel   ,2,2,1,1,Qt::AlignLeft|Qt::AlignCenter);horizontalRefSpeedLabel   ->setMinimumWidth(100);
	horizontalActualSpeedLabel=new QLabel(tr("99999.9 m/min"));horizontalGroupBoxLayout->addWidget(horizontalActualSpeedLabel,3,2,1,1,Qt::AlignLeft|Qt::AlignCenter);horizontalActualSpeedLabel->setMinimumWidth(100);
	//4th column
	horizontalGroupBoxLayout->addWidget(horizontalOnTargetLabel  =new QLabel(tr("On target")),1,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	horizontalGroupBoxLayout->addWidget(horizontalFwLabel        =new QLabel(tr("Forward")),2,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	horizontalGroupBoxLayout->addWidget(horizontalBwLabel        =new QLabel(tr("Backwards")),3,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	horizontalOnTargetLabel			->setMargin(5);	horizontalOnTargetLabel->setMinimumWidth(100);	
	horizontalFwLabel      			->setMargin(5);	horizontalFwLabel      ->setMinimumWidth(100);
	horizontalBwLabel      			->setMargin(5);	horizontalBwLabel      ->setMinimumWidth(100);	
														
	//verticalGroupBox
	QGroupBox *verticalGroupBox=new QGroupBox(tr("Vertical"));leftButtonsLayout->addWidget(verticalGroupBox);
	QGridLayout *verticalGroupBoxLayout=new QGridLayout();verticalGroupBox->setLayout(verticalGroupBoxLayout);
	//1st column
	verticalGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|103","N150|20|0",tr("Up"),NULL),0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
  verticalGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|104","N150|20|0",tr("Down"),NULL),1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	verticalGroupBoxLayout->addWidget(verticalResetPushbutton=new myQPushButton(tcpServer,"N150|20|116","N150|20|0",tr("Reset down"),NULL),2,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	verticalResetLabel=new QLabel(tr("Reset down"));verticalResetLabel->setMargin(5);verticalGroupBoxLayout->addWidget(verticalResetLabel,3,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	verticalResetLabel->setMinimumWidth(100);
	//2nd column
	verticalGroupBoxLayout->addWidget(verticalActualPosLabel  =new QLabel(tr("Actual position")),0,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	verticalGroupBoxLayout->addWidget(verticalTargetPosLabel  =new QLabel(tr("Target position")),1,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	verticalGroupBoxLayout->addWidget(verticalRefSpeedLabel   =new QLabel(tr("Reference speed")),2,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	verticalGroupBoxLayout->addWidget(verticalActualSpeedLabel=new QLabel(tr("Actual speed   ")),3,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	verticalActualPosLabel  	->setMargin(5);	verticalActualPosLabel  ->setMinimumWidth(100);
	verticalTargetPosLabel  	->setMargin(5);	verticalTargetPosLabel  ->setMinimumWidth(100);
	verticalRefSpeedLabel   	->setMargin(5);	verticalRefSpeedLabel   ->setMinimumWidth(100);
	verticalActualSpeedLabel	->setMargin(5);	verticalActualSpeedLabel->setMinimumWidth(100);
  //3rd column
	verticalActualPosLabel  =new QLabel(tr("99999.9 mm"));   verticalGroupBoxLayout->addWidget(verticalActualPosLabel  ,0,2,1,1,Qt::AlignLeft|Qt::AlignCenter);   verticalActualPosLabel  ->setMinimumWidth(100);
	verticalTargetPosLabel  =new QLabel(tr("99999.9 mm"));   verticalGroupBoxLayout->addWidget(verticalTargetPosLabel  ,1,2,1,1,Qt::AlignLeft|Qt::AlignCenter);   verticalTargetPosLabel  ->setMinimumWidth(100);
	verticalRefSpeedLabel   =new QLabel(tr("99999.9 m/min"));verticalGroupBoxLayout->addWidget(verticalRefSpeedLabel   ,2,2,1,1,Qt::AlignLeft|Qt::AlignCenter);   verticalRefSpeedLabel   ->setMinimumWidth(100);
	verticalActualSpeedLabel=new QLabel(tr("99999.9 m/min"));verticalGroupBoxLayout->addWidget(verticalActualSpeedLabel,3,2,1,1,Qt::AlignLeft|Qt::AlignCenter);   verticalActualSpeedLabel->setMinimumWidth(100);
	//4th column
	verticalGroupBoxLayout->addWidget(verticalOnTargetLabel  =new QLabel(tr("On target")),1,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	verticalGroupBoxLayout->addWidget(verticalUpLabel        =new QLabel(tr("Up")),2,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	verticalGroupBoxLayout->addWidget(verticalDownLabel        =new QLabel(tr("Down")),3,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	verticalOnTargetLabel					 ->setMargin(5); verticalOnTargetLabel->setMinimumWidth(100);	
	verticalUpLabel      					 ->setMargin(5); verticalUpLabel      ->setMinimumWidth(100);     	
	verticalDownLabel    					 ->setMargin(5); verticalDownLabel    ->setMinimumWidth(100);   	

	//hooks 
	QGroupBox *hooksGroupBox=new QGroupBox(tr("Hooks"));leftButtonsLayout->addWidget(hooksGroupBox);
	QGridLayout *hooksGroupBoxLayout=new QGridLayout();hooksGroupBox->setLayout(hooksGroupBoxLayout);
	//1st column
	hooksGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|107","N150|20|0",tr("Left"),NULL),0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	hooksGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|108","N150|20|0",tr("Right"),NULL),1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	hooksGroupBoxLayout->addWidget(hooksResetPushButton=new myQPushButton(tcpServer,"N150|20|118","N150|20|0",tr("Reset right"),NULL),2,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	hooksResetLabel=new QLabel(tr("Reset right"));hooksResetLabel->setMargin(5);hooksGroupBoxLayout->addWidget(hooksResetLabel,3,0,1,1,Qt::AlignHCenter|Qt::AlignCenter);
	hooksResetLabel->setMargin(5);hooksResetLabel->setMinimumWidth(100);
	hooksGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|109","N150|20|0",tr("Up"),NULL),4,0,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	//2nd column
	hooksGroupBoxLayout->addWidget(hooksActualPosLabel 	=new QLabel(tr("Actual position")),0,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	hooksGroupBoxLayout->addWidget(hooksTargetPosLabel 	=new QLabel(tr("Target position")),1,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	hooksGroupBoxLayout->addWidget(hooksRefSpeedLabel  	=new QLabel(tr("Reference speed")),2,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	hooksGroupBoxLayout->addWidget(hooksActualSpeedLabel=new QLabel(tr("Actual speed   ")),3,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	hooksGroupBoxLayout->addWidget(hooksPositionLabel=new QLabel(tr("...")),4,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	hooksActualPosLabel 			->setMargin(5);	 hooksActualPosLabel   ->setMinimumWidth(100);	
	hooksTargetPosLabel 			->setMargin(5);	 hooksTargetPosLabel 	 ->setMinimumWidth(100);	
	hooksRefSpeedLabel  			->setMargin(5);	 hooksRefSpeedLabel  	 ->setMinimumWidth(100);	
	hooksActualSpeedLabel 		->setMargin(5);	 hooksActualSpeedLabel ->setMinimumWidth(100);	
	hooksPositionLabel				->setMargin(5);	 hooksPositionLabel		 ->setMinimumWidth(100);	
	//3rd column
	hooksActualPosLabel  =new QLabel(tr("99999.9 mm"));   hooksGroupBoxLayout->addWidget(hooksActualPosLabel  ,0,2,1,1,Qt::AlignLeft|Qt::AlignCenter);   hooksActualPosLabel  ->setMinimumWidth(100);
	hooksTargetPosLabel  =new QLabel(tr("99999.9 mm"));   hooksGroupBoxLayout->addWidget(hooksTargetPosLabel  ,1,2,1,1,Qt::AlignLeft|Qt::AlignCenter);   hooksTargetPosLabel  ->setMinimumWidth(100);
	hooksRefSpeedLabel   =new QLabel(tr("99999.9 m/min"));hooksGroupBoxLayout->addWidget(hooksRefSpeedLabel   ,2,2,1,1,Qt::AlignLeft|Qt::AlignCenter);   hooksRefSpeedLabel   ->setMinimumWidth(100);
	hooksActualSpeedLabel=new QLabel(tr("99999.9 m/min"));hooksGroupBoxLayout->addWidget(hooksActualSpeedLabel,3,2,1,1,Qt::AlignLeft|Qt::AlignCenter);   hooksActualSpeedLabel->setMinimumWidth(100);
	hooksGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|110","N150|20|0",tr("Down"),NULL),4,2,1,1,Qt::AlignLeft|Qt::AlignCenter); 
	//4th column
	hooksGroupBoxLayout->addWidget(hooksOnCenterLabel  =new QLabel(tr("On center")),0,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	hooksGroupBoxLayout->addWidget(hooksOnTargetLabel  =new QLabel(tr("On target")),1,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	hooksGroupBoxLayout->addWidget(hooksLeftLabel        =new QLabel(tr("Left")),2,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	hooksGroupBoxLayout->addWidget(hooksRightLabel        =new QLabel(tr("Right")),3,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	hooksOnCenterLabel		->setMargin(5);	hooksOnCenterLabel->setMinimumWidth(100);
	hooksOnTargetLabel		->setMargin(5);	hooksOnTargetLabel->setMinimumWidth(100);
	hooksLeftLabel    		->setMargin(5);	hooksLeftLabel    ->setMinimumWidth(100);
	hooksRightLabel   		->setMargin(5);	hooksRightLabel   ->setMinimumWidth(100);

  //Anchors
	QGroupBox *anchorsGroupBox=new QGroupBox(tr("Anchors"));leftButtonsLayout->addWidget(anchorsGroupBox);
	QGridLayout *anchorsGroupBoxLayout=new QGridLayout();anchorsGroupBox->setLayout(anchorsGroupBoxLayout);
	//1st column
	anchorsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|111","N150|20|0",tr("Left out"),NULL),0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	anchorsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|113","N150|20|0",tr("Left in"),NULL),1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	//2nd column
	anchorsGroupBoxLayout->addWidget(leftMgOutLabel=new QLabel(tr("MG out")),0,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	anchorsGroupBoxLayout->addWidget(leftMgInLabel=new QLabel(tr("MG in")),1,1,1,1,Qt::AlignLeft|Qt::AlignCenter);
	leftMgOutLabel	->setMargin(5); leftMgOutLabel->setMinimumWidth(100);
	leftMgInLabel		->setMargin(5); leftMgInLabel	->setMinimumWidth(100);
	//3rd column
	anchorsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|112","N150|20|0",tr("Right out"),NULL),0,2,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	anchorsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|114","N150|20|0",tr("Right in"),NULL),1,2,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	//4th column
	anchorsGroupBoxLayout->addWidget(rightMgOutLabel=new QLabel(tr("MG out")),0,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	rightMgOutLabel->setMargin(5);rightMgOutLabel->setMinimumWidth(100);
	anchorsGroupBoxLayout->addWidget(rightMgInLabel=new QLabel(tr("MG in")),1,3,1,1,Qt::AlignLeft|Qt::AlignCenter);
	rightMgInLabel->setMargin(5);rightMgInLabel->setMinimumWidth(100);

	//Conveyors
	QGroupBox *conveyorsGroupBox=new QGroupBox(tr("Conveyors"));leftButtonsLayout->addWidget(conveyorsGroupBox);
	QGridLayout *conveyorsGroupBoxLayout=new QGridLayout();conveyorsGroupBox->setLayout(conveyorsGroupBoxLayout);
	//1st column
	conveyorsGroupBoxLayout->addWidget(leftConveyorStopLabel=new QLabel(tr("Left stop")),0,0,1,1,Qt::AlignCenter|Qt::AlignCenter);
	leftConveyorStopLabel->setMargin(5);leftConveyorStopLabel->setMinimumWidth(100);
	conveyorsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|123","N150|20|0",tr("Left"),NULL),1,0,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	//2nd column
	conveyorsGroupBoxLayout->addWidget(leftConveyorBrakeLabel=new QLabel(tr("Left brake")),0,1,1,1,Qt::AlignCenter|Qt::AlignCenter);
	leftConveyorBrakeLabel->setMargin(5);leftConveyorBrakeLabel->setMinimumWidth(100);
	conveyorsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|152","N150|20|0",tr("From 25"),NULL),1,1,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	//3rd column
	conveyorsGroupBoxLayout->addWidget(rightConveyorBrakeLabel=new QLabel(tr("Right brake")),0,2,1,1,Qt::AlignCenter|Qt::AlignCenter);
	rightConveyorBrakeLabel->setMargin(5);rightConveyorBrakeLabel->setMinimumWidth(100);
	conveyorsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|151","N150|20|0",tr("From 24"),NULL),1,2,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	//4th column
	conveyorsGroupBoxLayout->addWidget(rightConveyorStopLabel=new QLabel(tr("Right stop")),0,3,1,1,Qt::AlignCenter|Qt::AlignCenter);
	rightConveyorStopLabel->setMargin(5);rightConveyorStopLabel->setMinimumWidth(100);
	conveyorsGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|122","N150|20|0",tr("Right"),NULL),1,3,1,1,Qt::AlignHCenter|Qt::AlignCenter); 

	//PLC mode
	QGroupBox *plcModeGroupBox=new QGroupBox(tr("PLC mode"));leftButtonsLayout->addWidget(plcModeGroupBox);
	QGridLayout *plcModeGroupBoxLayout=new QGridLayout();plcModeGroupBox->setLayout(plcModeGroupBoxLayout);
	//1st column
	plcModeGroupBoxLayout->addWidget(autoPushbutton=new myQPushButton(tcpServer,"N150|20|100","N150|20|0",tr("Auto"),NULL),0,0,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	//2nd column
	//plcModeGroupBoxLayout->addWidget(manualPushbutton=new myQPushButton(tcpServer,"N150|20|101","N150|20|0",tr("Manual"),NULL),0,1,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	//3rd column
	plcModeGroupBoxLayout->addWidget(maitenancePushbutton=new myQPushButton(tcpServer,"N150|20|102","N150|20|0",tr("Maintenance"),NULL),0,2,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	//4th column
	myQPushButton *resetAlarmsPushButton;
	plcModeGroupBoxLayout->addWidget(resetAlarmsPushButton=new myQPushButton(tcpServer,"N150|20|119","N150|20|0",tr("Reset Alarms"),NULL),0,3,1,1,Qt::AlignHCenter|Qt::AlignCenter); 
	//5th column
	plcModeGroupBoxLayout->addWidget(stopLabel_0_24=new QLabel(tr("....")),0,4,1,1,Qt::AlignLeft|Qt::AlignCenter);
	plcModeGroupBoxLayout->addWidget(new myQPushButton(tcpServer,"N150|20|127","N150|20|0",tr("Reset Stop"),NULL),0,5,1,1,Qt::AlignLeft|Qt::AlignCenter);
	stopLabel_0_24->setMargin(5);

	leftButtonsLayout->stretch(5);
	//////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////Right Buttons Layout///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
	//Mission
	QGroupBox *missionGroupBox=new QGroupBox(tr("Mission"));rightButtonsLayout->addWidget(missionGroupBox);
	QGridLayout *missionGroupBoxLayout=new QGridLayout();missionGroupBox->setLayout(missionGroupBoxLayout);
	//1st column
	missionGroupBoxLayout->addWidget(loadPosLabel=new QLabel(tr("Load position")),0,0,1,1,Qt::AlignLeft|Qt::AlignCenter);
	loadPosLabel->setMargin(5);loadPosLabel->setMinimumWidth(100);
	missionGroupBoxLayout->addWidget(unloadPosLabel=new QLabel(tr("Unload position")),1,0,1,1,Qt::AlignLeft|Qt::AlignCenter);
	unloadPosLabel->setMargin(5);unloadPosLabel->setMinimumWidth(100);
	missionGroupBoxLayout->addWidget(pcsStatusLabel=new QLabel(tr("PC Status")),2,0,1,1,Qt::AlignLeft|Qt::AlignCenter);
	pcsStatusLabel->setMargin(5);pcsStatusLabel->setMinimumWidth(100);
	missionGroupBoxLayout->addWidget(plcStatusLabel=new QLabel(tr("PLC Status")),3,0,1,1,Qt::AlignLeft|Qt::AlignCenter);
	plcStatusLabel->setMargin(5);plcStatusLabel->setMinimumWidth(100);
	//2nd column
	loadPosLabel    =new QLabel(tr("99999.9 mm"));   missionGroupBoxLayout->addWidget(loadPosLabel  ,0,1,1,1,Qt::AlignLeft|Qt::AlignCenter);   loadPosLabel  ->setMinimumWidth(100);
	unloadPosLabel  =new QLabel(tr("99999.9 mm"));   missionGroupBoxLayout->addWidget(unloadPosLabel,1,1,1,1,Qt::AlignLeft|Qt::AlignCenter);   unloadPosLabel->setMinimumWidth(100);
	pcsStatusLabel  =new QLabel(tr("99999.9 m/min"));missionGroupBoxLayout->addWidget(pcsStatusLabel,2,1,1,1,Qt::AlignLeft|Qt::AlignCenter);   pcsStatusLabel->setMinimumWidth(100);
	plcStatusLabel  =new QLabel(tr("99999.9 m/min"));missionGroupBoxLayout->addWidget(plcStatusLabel,3,1,1,1,Qt::AlignLeft|Qt::AlignCenter);   plcStatusLabel->setMinimumWidth(100);

	//PLC status
	QGroupBox *plcStatusGroupBox=new QGroupBox(tr("PLC status"));rightButtonsLayout->addWidget(plcStatusGroupBox);
	QGridLayout *plcStatusGroupBoxLayout=new QGridLayout();plcStatusGroupBox->setLayout(plcStatusGroupBoxLayout);
	//1st column
	plcInOutLabel  =new QLabel(tr("PLC I/O"));   plcStatusGroupBoxLayout->addWidget(plcInOutLabel  ,0,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   plcInOutLabel  ->setMinimumWidth(100);
	plcInOutLabel->setMargin(5);plcInOutLabel->setMinimumWidth(300);
	thermalOverloadLabel  =new QLabel(tr("Thermal Overload"));   plcStatusGroupBoxLayout->addWidget(thermalOverloadLabel  ,1,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   thermalOverloadLabel  ->setMinimumWidth(100);
	thermalOverloadLabel->setMargin(5); thermalOverloadLabel->setMinimumWidth(300);
	readyMissionLabel  =new QLabel(tr("Ready for mission"));   plcStatusGroupBoxLayout->addWidget(readyMissionLabel  ,2,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   readyMissionLabel  ->setMinimumWidth(100);
	readyMissionLabel->setMargin(5);readyMissionLabel->setMinimumWidth(300);
	homePositionLabel  =new QLabel(tr("Home position"));    plcStatusGroupBoxLayout->addWidget(homePositionLabel  ,3,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   homePositionLabel  ->setMinimumWidth(100);
	homePositionLabel->setMargin(5);homePositionLabel->setMinimumWidth(300);
	rightButtonsLayout->addStretch(5);

	//oil
	QGroupBox *oilGroupBox=new QGroupBox(tr("Oil"));rightButtonsLayout->addWidget(oilGroupBox);
	QVBoxLayout *oilGroupBoxLayout=new QVBoxLayout();oilGroupBox->setLayout(oilGroupBoxLayout);
	QPushButton *addOilPushButton=new QPushButton(tr("Add oil"),NULL);
	oilGroupBoxLayout->addWidget(addOilPushButton,Qt::AlignLeft|Qt::AlignCenter);
	connect(addOilPushButton,SIGNAL(clicked()),this,SLOT(addOilPushButtonSlot()));
	lowOilPressLabel  =new QLabel(tr("low OilPressLabel"));   oilGroupBoxLayout->addWidget(lowOilPressLabel,Qt::AlignLeft|Qt::AlignCenter);   lowOilPressLabel  ->setMinimumWidth(100);lowOilPressLabel->setMargin(5);
	pressPumpLabel  =new QLabel(tr("OilPressure"));   oilGroupBoxLayout->addWidget(pressPumpLabel,Qt::AlignLeft|Qt::AlignCenter);   pressPumpLabel  ->setMinimumWidth(100);pressPumpLabel->setMargin(5);
	//Mission step
	QGroupBox *missionStepGroupBox=new QGroupBox(tr("Mission step"));rightButtonsLayout->addWidget(missionStepGroupBox);
	QGridLayout *missionStepGroupBoxLayout=new QGridLayout();missionStepGroupBox->setLayout(missionStepGroupBoxLayout);
	//1st column
	step1Label  =new QLabel(tr("Step 1 move to load position"));   missionStepGroupBoxLayout->addWidget(step1Label ,0,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   step1Label  ->setMinimumWidth(100);
	step2Label  =new QLabel(tr("Step 2 hooks carriage move to cell"));   missionStepGroupBoxLayout->addWidget(step2Label ,1,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   step2Label  ->setMinimumWidth(100);
	step3Label  =new QLabel(tr("Step 3 hooks carriage up"));   missionStepGroupBoxLayout->addWidget(step3Label ,2,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   step3Label  ->setMinimumWidth(100);
	step4Label  =new QLabel(tr("Step 4 hooks carriage transfer cassette to crane"));   missionStepGroupBoxLayout->addWidget(step4Label  ,3,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   step4Label  ->setMinimumWidth(100);
	step5Label  =new QLabel(tr("Step 5 move to unload position"));   missionStepGroupBoxLayout->addWidget(step5Label  ,4,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   step5Label  ->setMinimumWidth(100);
	step6Label  =new QLabel(tr("Step 6 hooks carriage push cassette to cell "));   missionStepGroupBoxLayout->addWidget(step6Label  ,5,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   step6Label  ->setMinimumWidth(100);
	step7Label  =new QLabel(tr("Step 7 hooks carriage down"));   missionStepGroupBoxLayout->addWidget(step7Label  ,6,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   step7Label  ->setMinimumWidth(100);
	step8Label  =new QLabel(tr("Step 8 hooks carriage return to center"));   missionStepGroupBoxLayout->addWidget(step8Label  ,7,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   step8Label  ->setMinimumWidth(100);
	step9Label  =new QLabel(tr("Step 9 end mission"));   missionStepGroupBoxLayout->addWidget(step9Label  ,8,0,1,1,Qt::AlignLeft|Qt::AlignCenter);   step9Label  ->setMinimumWidth(100);

	step1Label->setMargin(5); 	step1Label->setMinimumWidth(300);
	step2Label->setMargin(5);		step2Label->setMinimumWidth(300);
	step3Label->setMargin(5);		step3Label->setMinimumWidth(300);
	step4Label->setMargin(5);		step4Label->setMinimumWidth(300);
	step5Label->setMargin(5);		step5Label->setMinimumWidth(300);
	step6Label->setMargin(5);		step6Label->setMinimumWidth(300);
	step7Label->setMargin(5);		step7Label->setMinimumWidth(300);
	step8Label->setMargin(5);		step8Label->setMinimumWidth(300);
	step9Label->setMargin(5);		step9Label->setMinimumWidth(300);

	myQPushButton *continueMissionBtn=new myQPushButton(tcpServer,"","N150|20|0",tr("Continue mission"),NULL);
	rightButtonsLayout->addWidget(continueMissionBtn);
	connect(continueMissionBtn,SIGNAL(clicked()),this,SLOT(continueMissionBtnSlot()));
	myQPushButton *closeMissionBtn=new myQPushButton(tcpServer,"","N150|20|0",tr("Close mission"),NULL);
	rightButtonsLayout->addWidget(closeMissionBtn);
	connect(closeMissionBtn,SIGNAL(clicked()),this,SLOT(closeMissionBtnSlot()));
	//
	QPushButton *manualMissionBtn=new QPushButton(tr("Manual mission"));
	rightButtonsLayout->addWidget(manualMissionBtn);
	connect(manualMissionBtn,SIGNAL(clicked()),this,SLOT(manualMissionBtnSlot()));
	/*mainSplitter->addWidget(btnsWidget);*/
	mainSplitter->addWidget(listWidget);

	QList<int> sizes;sizes<<750<<500<<50;
	mainSplitter->setSizes(sizes);
	sizes.clear();sizes<<700<<900<<300;
	upperSplitter->setSizes(sizes);
//
	QScrollArea *topArea=new QScrollArea();
	topMainLayout->addWidget(topArea);
	QWidget *topAreaWidget=new QWidget();
 	topAreaWidget->setLayout(mainLayout);
	topArea->setWidget(topAreaWidget);

	connect(&alarmTimer,SIGNAL(timeout()),this,SLOT(alarmTimerSlot()));
	alarmTimer.start(5000);
}
void packetCraneManualClass::plcIntMemory150Changed(int index,int value)
{
	QVector<bool> bitsVector=deprecatedCalculateBits(value);
	if (index==0)loadPosLabel  ->setText(tr("%1").arg(value));
	if (index==1)unloadPosLabel->setText(tr("%1").arg(value));
	if (index==3)pcsStatusLabel->setText(tr("%1").arg(value));
	if (index==2)plcStatusLabel->setText(tr("%1").arg(value));
	if (index==10||index==11||index==12)
		conveyorsManual->packetCranePlcIntMemory150Changed(index,value);
}
void packetCraneManualClass::plcIntMemory152Changed(int index,int value)
{	
	QVector<bool> bitsVector=deprecatedCalculateBits(value);
	if (index==1)
	{
		if (bitsVector[6])
			plcInOutLabel->setStyleSheet("QLabel{background-color:green}");
		else
			plcInOutLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[5])
			thermalOverloadLabel->setStyleSheet("QLabel{background-color:green}");
		else
			thermalOverloadLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[4])
		{
			vLog("info","crane plc Rready for mission:","true");
			readyMissionLabel->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			vLog("info","crane plc ready for mission:","false");
			readyMissionLabel->setStyleSheet("QLabel{background-color:lightgray}");
		}
		if (bitsVector[7])
		{
			vLog("info","crane plc step:","Step 1 move to load position");
			step1Label->setStyleSheet("QLabel{background-color:green}");
		}
		else
			step1Label->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[8])
		{
			vLog("info","crane plc step:","Step 2 hooks carriage move to cell");
			step2Label->setStyleSheet("QLabel{background-color:green}");
		}
		else
			step2Label->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[9])
		{
			vLog("info","crane plc step:","Step 3 hooks carriage up");
			step3Label->setStyleSheet("QLabel{background-color:green}");
		}
		else
			step3Label->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[10])
		{
			vLog("info","plc step:","Step 4 hooks carriage transfer cassette to crane");
			step4Label->setStyleSheet("QLabel{background-color:green}");
		}
		else
			step4Label->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[11])
		{
			vLog("info","crane plc step:","Step 5 move to unload position");
			step5Label->setStyleSheet("QLabel{background-color:green}");
		}
		else
			step5Label->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[12])
		{
			vLog("info","crane plc step:","Step 6 hooks carriage push cassette to cell");
			step6Label->setStyleSheet("QLabel{background-color:green}");
		}
		else
			step6Label->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[13])
		{
			vLog("info","crane plc step:","Step 7 hooks carriage down");
			step7Label->setStyleSheet("QLabel{background-color:green}");
		}
		else
			step7Label->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[14])
		{
			vLog("info","crane plc step:","Step 8 hooks carriage return to center");
			step8Label->setStyleSheet("QLabel{background-color:green}");
		}
		else
			step8Label->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[15])
		{
			vLog("info","crane plc step:","Step 9 end mission");
			step9Label->setStyleSheet("QLabel{background-color:green}");
		}
		else
			step9Label->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[16])
			horizontalFwLabel->setStyleSheet("QLabel{background-color:green}");
		else
			horizontalFwLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[17])
			horizontalBwLabel->setStyleSheet("QLabel{background-color:green}");
		else
			horizontalBwLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[18])
			verticalUpLabel->setStyleSheet("QLabel{background-color:green}");
		else
			verticalUpLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[19])
			verticalDownLabel->setStyleSheet("QLabel{background-color:green}");
		else
			verticalDownLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[20])
			hooksLeftLabel->setStyleSheet("QLabel{background-color:green}");
		else
			hooksLeftLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[21])
			hooksRightLabel->setStyleSheet("QLabel{background-color:green}");
		else
			hooksRightLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[22])
		{
			vLog("info","crane plc home position:","true");
			homePositionLabel->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			vLog("info","crane plc home position:","false");
			homePositionLabel->setStyleSheet("QLabel{background-color:lightgray}");
		}
		if (bitsVector[23])
			horizontalOnTargetLabel->setStyleSheet("QLabel{background-color:green}");
		else
			horizontalOnTargetLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[24])
			verticalOnTargetLabel->setStyleSheet("QLabel{background-color:green}");
		else
		  verticalOnTargetLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[25])
			hooksOnTargetLabel->setStyleSheet("QLabel{background-color:green}");
		else
		  hooksOnTargetLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[26])
		{
			vLog("info","crane plc mode:","auto");
			autoPushbutton->setStyleSheet("QPushButton{background-color:green}");
		}
		else
		  autoPushbutton->setStyleSheet("QPushButton{background-color:lightgray}");
		//if (bitsVector[27])
		//{
		//	vLog("info","crane plc mode:","manual");
		//	manualPushbutton->setStyleSheet("QPushButton{background-color:green}");
		//}
		//else
		//  manualPushbutton->setStyleSheet("QPushButton{background-color:lightgray}");
		if (bitsVector[28])
		{
			vLog("info","crane plc mode:","maitenance");
			maitenancePushbutton->setStyleSheet("QPushButton{background-color:green}");
		}
		else
		  maitenancePushbutton->setStyleSheet("QPushButton{background-color:lightgray}");
		if (bitsVector[29])
		{
			pressPumpLabel->setText(tr("Oil pump on"));
			pressPumpLabel->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			pressPumpLabel->setText(tr("Oil pump off"));
			pressPumpLabel->setStyleSheet("");
		}
		if (bitsVector[30])
			hooksOnCenterLabel->setStyleSheet("QLabel{background-color:green}");
		else
		  hooksOnCenterLabel->setStyleSheet("QLabel{background-color:lightgray}");

		//if (bitsVector[26])
		//	autoPushbutton->setStyleSheet("QPushButton{text-align:center;color:green}");
		//else
		//	autoPushbutton->setStyleSheet("QPushButton{text-align:center;color:black}");
		//if (bitsVector[27])
		//	manualPushbutton->setStyleSheet("QPushButton{text-align:center;color:blue}");
		//else
		//	manualPushbutton->setStyleSheet("QPushButton{text-align:center;color:black}");
		//if (bitsVector[28])
		//	maitenancePushbutton->setStyleSheet("QPushButton{text-align:center;color:blue}");
		//else
		//  maitenancePushbutton->setStyleSheet("QPushButton{text-align:center;color:black}");
	}	
	if (index==4)
	{	
	  alarmsIndex4Value=value;
		refreshAlarmsList();
	}
	else if (index==5)
	{
		alarmsIndex5Value=value;
		refreshAlarmsList();
	}
	else if (index==8)
	{
		warningsIndex8Value=value;
		refreshWarningList();
	}
	else if (index==0)
	{
		conveyorsManual->packetCranePlcIntMemory152Changed(index,value);
		warningsIndex0Value=value;
		refreshWarningList();
		if (bitsVector[0])
			hooksResetLabel->setStyleSheet("QLabel{background-color:green}");
		else 
		  hooksResetLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[2])
			horizontalResetLabel->setStyleSheet("QLabel{background-color:green}");
		else 
		  horizontalResetLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[3])
			verticalResetLabel->setStyleSheet("QLabel{background-color:green}");
		else 
		  verticalResetLabel->setStyleSheet("QLabel{background-color:lightgray}");
		hooksPositionLabel->setText("................");
		hooksPositionLabel->setStyleSheet("QLabel{background-color:lightgray}");
		hooksPositionLabel->setMargin(5);
		if (bitsVector[4])
			rightMgInLabel->setStyleSheet("QLabel{background-color:green}");
		else 
		  rightMgInLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[5])
			rightMgOutLabel->setStyleSheet("QLabel{background-color:green}");
		else 
		  rightMgOutLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[6])
			leftMgInLabel->setStyleSheet("QLabel{background-color:green}");
		else 
		  leftMgInLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[7])
			leftMgOutLabel->setStyleSheet("QLabel{background-color:green}");
		else 
		  leftMgOutLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[8]) 
		{
			hooksPositionLabel->setText(tr("Hooks are up"));
			hooksPositionLabel->setStyleSheet("QLabel{background-color:green}");
		}
		if (bitsVector[9])
		{
			hooksPositionLabel->setText(tr("Hooks are down"));
			hooksPositionLabel->setStyleSheet("QLabel{background-color:green}");
		}
		if (bitsVector[10])
			leftConveyorStopLabel->setStyleSheet("QLabel{background-color:green}");
		else 
		  leftConveyorStopLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[11])
			leftConveyorBrakeLabel->setStyleSheet("QLabel{background-color:green}");
		else 
		  leftConveyorBrakeLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[12])
			rightConveyorStopLabel->setStyleSheet("QLabel{background-color:green}");
		else 
		  rightConveyorStopLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[13])
			rightConveyorBrakeLabel->setStyleSheet("QLabel{background-color:green}");
		else  
		  rightConveyorBrakeLabel->setStyleSheet("QLabel{background-color:lightgray}");
		if (bitsVector[14])
		{
			lowOilPressLabel->setText(tr("Oil pressure ok"));
			lowOilPressLabel->setStyleSheet("QLabel{background-color:green}");
		}
		else
		{
			lowOilPressLabel->setText(tr("Low oil pressure"));
			lowOilPressLabel->setStyleSheet("QLabel{background-color:red}");
		}
	  if (bitsVector[24])
		{ 
			vLog("info","crane plc:","active stop");
			stopLabel_0_24->setText("  Active stop  ");
			stopLabel_0_24->setStyleSheet("QLabel{text-align:center;color:black; background-color:red;}");
		}
		else
		{
			vLog("info","crane plc:","no active stop");
			stopLabel_0_24->setText("  No active stop");
			stopLabel_0_24->setStyleSheet("QLabel{text-align:center;color:gray}");
		} 
	}
	else if (index==1)
	{
		conveyorsManual->packetCranePlcIntMemory152Changed(index,value);
		warningsIndex1Value=value;
		refreshWarningList();
		if (bitsVector[0])
		{
			horizontalResetPushbutton->setText("Reseting...");
		  horizontalResetPushbutton->setStyleSheet("QPushButton{text-align:center;color:blue}");
		}
		else
		{
			horizontalResetPushbutton->setText("Reset");
			horizontalResetPushbutton->setStyleSheet("QPushButton{text-align:center;color:black}");
		}
		if (bitsVector[1])
		{
			verticalResetPushbutton->setStyleSheet("QPushButton{text-align:center;color:blue}");
			verticalResetPushbutton->setText("Reseting...");
		}
		else
		{
		  verticalResetPushbutton->setText("Reset");	
			verticalResetPushbutton->setStyleSheet("QPushButton{text-align:center;color:black}");
		}
		if (bitsVector[3])
		{
			hooksResetPushButton->setStyleSheet("QPushButton{text-align:center;color:blue}");
			hooksResetPushButton->setText("Reseting...");
		}
		else
		{
			hooksResetPushButton->setStyleSheet("QPushButton{text-align:center;color:black}");
		  hooksResetPushButton->setText("Reset");	
		}
	}
}
void packetCraneManualClass::alarmTimerSlot()
{
	//if (alarmsSignalListWidget->count()>0)
	//	QSound(QDir::current().absolutePath()+"//failed.wav").play();
}
void packetCraneManualClass::refreshAlarmsList()
{
	QVector<bool> bitsIndex4Vector=deprecatedCalculateBits(alarmsIndex4Value);
	alarmsSignalListWidget->clear();
	for (int i=0;i<32;i++)
	{
		if (bitsIndex4Vector[i])
		{
			alarmsSignalListWidget->addItem(alarmsIndex4StringList[i]);
			alarmsSignalListWidget->item(alarmsSignalListWidget->count()-1)->setBackground(QBrush(QColor(255,0,0)));
			alarmsSignalListWidget->item(alarmsSignalListWidget->count()-1)->setForeground(QBrush(QColor(0,0,0)));
			apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('craneError','N152[4].%1','%2','%3',%4)").arg(i).arg(alarmsIndex4StringList[i]).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		}
	}
	QVector<bool> bitsIndex5Vector=deprecatedCalculateBits(alarmsIndex5Value);
	for (int i=0;i<32;i++)
	{
		if (bitsIndex5Vector[i])
		{
			alarmsSignalListWidget->addItem(alarmsIndex5StringList[i]);
			alarmsSignalListWidget->item(alarmsSignalListWidget->count()-1)->setBackground(QBrush(QColor(255,0,0)));
			alarmsSignalListWidget->item(alarmsSignalListWidget->count()-1)->setForeground(QBrush(QColor(0,0,0)));
			apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('craneError','N152[5].%1','%2','%3',%4)").arg(i).arg(alarmsIndex5StringList[i]).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		}
	}
}
void packetCraneManualClass::refreshWarningList()
{
	warningsSignalListWidget->clear();
	QVector<bool> bitsIndex8Vector=deprecatedCalculateBits(warningsIndex8Value);
	for (int i=0;i<32;i++)
	{
		if (bitsIndex8Vector[i])
		{ 
			warningsSignalListWidget->addItem(warningsIndex8StringList[i]);
			warningsSignalListWidget->item(warningsSignalListWidget->count()-1)->setBackground(QBrush(QColor(0,128,192)));
			warningsSignalListWidget->item(warningsSignalListWidget->count()-1)->setForeground(QBrush(QColor(0,0,100)));
			apexExecQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('craneWarning','N152[8].%1','%2','%3',%4)").arg(i).arg(warningsIndex8StringList[i]).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		}  
	}
}
void packetCraneManualClass::showAlarmsHistoryBtnSlot()
{
	QVector<QVariant> bitsVector=apexExecQuery("select top 100 bit from logErrorPlc where type='craneError' order by tid desc");
	QVector<QVariant> descriptionVector=apexExecQuery("select top 100 description from logErrorPlc where type='craneError' order by tid desc");
	QVector<QVariant> dateVector=apexExecQuery("select top 100 date from logErrorPlc where type='craneError' order by tid desc");
	QDialog dlg(this);
	QVBoxLayout *mainLayout=new QVBoxLayout();dlg.setLayout(mainLayout);
	QTableWidget *errorsTableWidget=new QTableWidget();mainLayout->addWidget(errorsTableWidget);
	errorsTableWidget->setAlternatingRowColors(true);
	errorsTableWidget->horizontalHeader()->setStretchLastSection(true);
	errorsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	errorsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	errorsTableWidget->setColumnCount(2);
	QStringList ch;ch<<tr("Date")<<tr("Description");
	errorsTableWidget->setHorizontalHeaderLabels(ch);
	errorsTableWidget->setRowCount(bitsVector.count());
	for (int i=0;i<bitsVector.count();i++)
	{
		errorsTableWidget->setItem(i,0,new QTableWidgetItem(dateVector[i].toString(),Qt::DisplayRole));
		errorsTableWidget->setItem(i,1,new QTableWidgetItem(bitsVector[i].toString()+":"+descriptionVector[i].toString(),Qt::DisplayRole));
	}
	errorsTableWidget->resizeColumnsToContents();
	dlg.resize(600,900);
	dlg.exec();
}
void packetCraneManualClass::showWarningsHistoryBtnSlot()
{
	QVector<QVariant> bitsVector=apexExecQuery("select top 100 bit from logErrorPlc where type='craneWarning' order by tid desc");
	QVector<QVariant> descriptionVector=apexExecQuery("select top 100 description from logErrorPlc where type='craneWarning' order by tid desc");
	QVector<QVariant> dateVector=apexExecQuery("select top 100 date from logErrorPlc where type='craneWarning' order by tid desc");
	QDialog dlg(this);
	QVBoxLayout *mainLayout=new QVBoxLayout();dlg.setLayout(mainLayout);
	QTableWidget *errorsTableWidget=new QTableWidget();mainLayout->addWidget(errorsTableWidget);
	errorsTableWidget->setAlternatingRowColors(true);
	errorsTableWidget->horizontalHeader()->setStretchLastSection(true);
	errorsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	errorsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	errorsTableWidget->setColumnCount(2);
	QStringList ch;ch<<tr("Date")<<tr("Description");
	errorsTableWidget->setHorizontalHeaderLabels(ch);
	errorsTableWidget->setRowCount(bitsVector.count());
	for (int i=0;i<bitsVector.count();i++)
	{
		errorsTableWidget->setItem(i,0,new QTableWidgetItem(dateVector[i].toString(),Qt::DisplayRole));
		errorsTableWidget->setItem(i,1,new QTableWidgetItem(bitsVector[i].toString()+":"+descriptionVector[i].toString(),Qt::DisplayRole));
	}
	errorsTableWidget->resizeColumnsToContents();
	dlg.resize(600,900);
	dlg.exec();
}

void packetCraneManualClass::plcFloatMemoryChanged(int index,double value)
{
	if (index== 0)horizontalActualPosLabel->setText(tr("%1 mm").arg(value,8,'f',1));
	if (index== 4)horizontalTargetPosLabel->setText(tr("%1 mm").arg(value,8,'f',1));
	if (index== 8)horizontalRefSpeedLabel->setText(tr("%1 m/min").arg(value,8,'f',1));
	if (index==11)horizontalActualSpeedLabel->setText(tr("%1 m/min").arg(value,8,'f',1));

	if (index== 1) verticalActualPosLabel->setText(tr("%1 mm").arg(value,8,'f',1));
	if (index== 5) verticalTargetPosLabel->setText(tr("%1 mm").arg(value,8,'f',1));
	if (index== 9) verticalRefSpeedLabel->setText(tr("%1 m/min").arg(value,8,'f',1));
	if (index== 12)verticalActualSpeedLabel->setText(tr("%1 m/min").arg(value,8,'f',1));

	if (index== 2)  hooksActualPosLabel->setText(tr("%1 mm").arg(value,8,'f',1));
	if (index== 6)  hooksTargetPosLabel->setText(tr("%1 mm").arg(value,8,'f',1));
	if (index== 10) hooksRefSpeedLabel->setText(tr("%1 m/min").arg(value,8,'f',1));
	if (index== 13) hooksActualSpeedLabel->setText(tr("%1 m/min").arg(value,8,'f',1));
}
void packetCraneManualClass::continueMissionBtnSlot()
{
	QString passStr=QInputDialog::getText(this,tr("Password neaded for this operation"),tr("Verify that the cassette is on the crane.\nPlease input the password"),QLineEdit::Password,"");
	if(checkStrongPass(passStr))
		tcpServer->sendString(QString("N150|20|126"));	
	else
	{
		if (!passStr.isEmpty())
			QMessageBox::warning(0,tr("Warning"),tr("The password that you typed is incorrect"));
	}
}
void packetCraneManualClass::closeMissionBtnSlot()
{
	QString passStr=QInputDialog::getText(this,tr("Password neaded for this operation"),tr("Verify that the cassette is on the target position and that the mission finished.\nPlease input the password"),QLineEdit::Password,"");
	if(checkStrongPass(passStr))
		tcpServer->sendString(QString("N150|20|125"));	
	else
	{
		if (!passStr.isEmpty())
			QMessageBox::warning(0,tr("Warning"),tr("The password that you typed is incorrect"));
	}
}
void packetCraneManualClass::addOilPushButtonSlot()
{ 
	QString passStr=QInputDialog::getText(this,tr("Password neaded for this operation"),tr("Password needed to add oil"),QLineEdit::Password,"");
	if(checkStrongPass(passStr))
	{
		QDialog *dlg=new QDialog();
		QVBoxLayout *mainLayout=new QVBoxLayout();dlg->setLayout(mainLayout);
		myQPushButton *addOilPushButton=new myQPushButton(tcpServer,"N150|20|144","N150|20|0",tr("Add oil"),NULL);
		mainLayout->addWidget(addOilPushButton);
		QPushButton *okPushButton=new QPushButton(tr("Ok"));mainLayout->addWidget(okPushButton);
		connect(okPushButton,SIGNAL(clicked()),dlg,SLOT(accept()));
		dlg->exec();
		delete dlg;
	}	
	else
	{
		if (!passStr.isEmpty())
			QMessageBox::warning(0,tr("Warning"),tr("The password that you typed is incorrect"));
	}
}
void packetCraneManualClass::hideButtons()
{
	for (int i=0;i<areaButtonsVector.count();i++)
		areaButtonsVector[i]->hide();
}
void packetCraneManualClass::showButtons()
{
	for (int i=0;i<areaButtonsVector.count();i++)
		areaButtonsVector[i]->show();
}
void packetCraneManualClass::onShowConveyorsManualsSlot()
{
	conveyorsManual->show();
}
void packetCraneManualClass::manualMissionBtnSlot()
{
	if (manualCassetteMovement)
	{
		manualCassetteMovement->updateFromDb();
		manualCassetteMovement->show();
	}
}
void packetCraneManualClass::setManualCassetteMovementClass(manualCassetteMovementClass *manualCassetteMovement_)
{
	manualCassetteMovement=manualCassetteMovement_;
}
#include "plcM.h"
#include "plcNetworkStatus.h"
#include <QPainter>
#include "../common/execDBQuery.h"
float plcSetXVelocityForOpenGl;
float plcSetYVelocityForOpenGl;
float plcSetZVelocityForOpenGl;

plcManualClass::plcManualClass(QWidget *p, plcWidgetClass *plcWidget_, tcpServerClass *cranePlcTcpServer_, tcpServerClass *conveyorsPlcTcpServer_) :QWidget()
{
		p->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{font-size:13px;color:#ffffff;} QPushButton,QPushButton:disabled,QCheckBox,QCheckBox:disabled,QRadioButton,QRadioButton:disabled{font-size:13px;color:#ffffff;} QGroupBox::title{font-size:13px;}");
		plcWidget = plcWidget_;
		cranePlcTcpServer = cranePlcTcpServer_;
		conveyorsPlcTcpServer = conveyorsPlcTcpServer_;
		//
		QHBoxLayout *topLayout = new QHBoxLayout();
		p->setLayout(topLayout);
		QScrollArea *mainScrollArea = new QScrollArea();
		mainScrollArea->setBackgroundRole(QPalette::Window);
		topLayout->addWidget(mainScrollArea);
		QWidget *mainScrollAreaWidget = new QWidget();
		mainScrollAreaWidget->setObjectName("plcManualContent");
		mainScrollAreaWidget->setStyleSheet("QWidget#plcManualContent{background-color:#0b1220;}");
		//
		QVBoxLayout *mainLayout = new QVBoxLayout(); mainLayout->setMargin(2); mainScrollAreaWidget->setLayout(mainLayout);
		QHBoxLayout *upLayout = new QHBoxLayout(); mainLayout->addLayout(upLayout);
		QVBoxLayout *layout0 = new QVBoxLayout(); upLayout->addLayout(layout0);
		QVBoxLayout *layout1 = new QVBoxLayout(); upLayout->addLayout(layout1);
		QVBoxLayout *layout2 = new QVBoxLayout(); upLayout->addLayout(layout2);
		QVBoxLayout *layout3 = new QVBoxLayout(); upLayout->addLayout(layout3);
		QVBoxLayout *layout4 = new QVBoxLayout(); upLayout->addLayout(layout4);
		QHBoxLayout *downLayout = new QHBoxLayout(); mainLayout->addLayout(downLayout);
		int columnWidth = 250;
		//////////////////////////////////////////////////////////////////////////////////////////////layout0
		QGridLayout *fromToLayout = new QGridLayout(); layout0->addLayout(fromToLayout);
		fromToLayout->addWidget(new QLabel(tr("From PC position")), 0, 0);
		fromPcLabel = new QLabel("...");
		fromPcLabel->setMinimumWidth(50);
		fromPcLabel->setStyleSheet("QLabel{color:#7dd3fc;text-align:center;font-size:16px;}");
		fromToLayout->addWidget(fromPcLabel, 0, 1);

		fromToLayout->addWidget(new QLabel(tr("To PC position")), 0, 2);
		toPcLabel = new QLabel("...");
		toPcLabel->setMinimumWidth(50);
		toPcLabel->setStyleSheet("QLabel{color:#7dd3fc;text-align:center;font-size:16px;}");
		fromToLayout->addWidget(toPcLabel, 0, 3);

		fromToLayout->addWidget(new QLabel(tr("From Plc pos")), 1, 0);
		fromPlcLabel = new QLabel("...");
		fromPlcLabel->setMinimumWidth(50);
		fromPlcLabel->setStyleSheet("QLabel{color:#7dd3fc;text-align:center;font-size:16px;}");
		fromToLayout->addWidget(fromPlcLabel, 1, 1);

		fromToLayout->addWidget(new QLabel(tr("To Plc pos")), 1, 2);
		toPlcLabel = new QLabel("...");
		toPlcLabel->setMinimumWidth(50);
		toPlcLabel->setStyleSheet("QLabel{color:#7dd3fc;text-align:center;font-size:16px;}");
		fromToLayout->addWidget(toPlcLabel, 1, 3);

		fromToPlcStatus = new QLabel("...");
		fromToPlcStatus->setStyleSheet("QLabel{color:#7dd3fc;text-align:center;font-size:13px;}");
		fromToLayout->addWidget(fromToPlcStatus, 2, 0, 1, 4, Qt::AlignHCenter);
		//state group
		QGroupBox *stateGroupBox = new QGroupBox(tr("PLC states")); layout0->addWidget(stateGroupBox);
		QVBoxLayout *stateGroupBoxLayout = new QVBoxLayout(); stateGroupBox->setLayout(stateGroupBoxLayout);
		abordMissionPushButton = new myQPushButton(cranePlcTcpServer, "N20|2|-1", "N20|2|0", tr("   Abord Mission  "), this); stateGroupBoxLayout->addWidget(abordMissionPushButton, Qt::AlignHCenter);
		continueMissionPushButton = new QPushButton(tr("   Continue Mission  "), this); stateGroupBoxLayout->addWidget(continueMissionPushButton, Qt::AlignHCenter);
		connect(continueMissionPushButton, SIGNAL(clicked(bool)), this, SLOT(continueMissionPushButtonSlot(bool)));
		stateLabel_15_13 = new QLabel(tr("       Executing mission")); stateLabel_15_13->setMinimumWidth(100); stateGroupBoxLayout->addWidget(stateLabel_15_13, Qt::AlignHCenter); stateLabel_15_13->setMinimumHeight(25);
		stateLabel_15_14 = new QLabel(tr("       With basket in unload mission")); stateLabel_15_14->setMinimumWidth(100); stateGroupBoxLayout->addWidget(stateLabel_15_14, Qt::AlignHCenter); stateLabel_15_14->setMinimumHeight(25);
		stateLabel_15_15 = new QLabel(tr("       Mission finished, going up ")); stateLabel_15_15->setMinimumWidth(100); stateGroupBoxLayout->addWidget(stateLabel_15_15, Qt::AlignHCenter); stateLabel_15_15->setMinimumHeight(25);
		//plcZonesGroupBox
		QGroupBox *plcZonesGroupBox = new QGroupBox(tr("Plc zones")); layout0->addWidget(plcZonesGroupBox);
		QVBoxLayout *plcZonesGroupBoxLayout = new QVBoxLayout(); plcZonesGroupBox->setLayout(plcZonesGroupBoxLayout);
		plcStatusLabel_14_14 = new QLabel(tr("  In maintenance zone (y=1900-3400)")); plcZonesGroupBoxLayout->addWidget(plcStatusLabel_14_14); plcStatusLabel_14_14->setMinimumWidth(250); plcStatusLabel_14_14->setMinimumHeight(30);
		plcStatusLabel_14_15 = new QLabel(tr("  In stacker zone"));    plcZonesGroupBoxLayout->addWidget(plcStatusLabel_14_15); plcStatusLabel_14_15->setMinimumWidth(250); plcStatusLabel_14_15->setMinimumHeight(30);
		plcStatusLabel_15_00 = new QLabel(tr("  In ageing oven zone")); plcZonesGroupBoxLayout->addWidget(plcStatusLabel_15_00); plcStatusLabel_15_00->setMinimumWidth(250); plcStatusLabel_15_00->setMinimumHeight(30);
		//targets group
		QGroupBox *targetsGroupBox = new QGroupBox(tr("Targets")); layout0->addWidget(targetsGroupBox); targetsGroupBox->setMinimumWidth(columnWidth);
		QVBoxLayout *targetsGroupBoxLayout = new QVBoxLayout(); targetsGroupBox->setLayout(targetsGroupBoxLayout);

		loadTargetZLabel = new QLabel("..."); targetsGroupBoxLayout->addWidget(loadTargetZLabel);
		loadTargetXLabel = new QLabel("..."); targetsGroupBoxLayout->addWidget(loadTargetXLabel);
		loadTargetYLabel = new QLabel("..."); targetsGroupBoxLayout->addWidget(loadTargetYLabel);

		unloadTargetZLabel = new QLabel("..."); targetsGroupBoxLayout->addWidget(unloadTargetZLabel);
		unloadTargetXLabel = new QLabel("..."); targetsGroupBoxLayout->addWidget(unloadTargetXLabel);
		unloadTargetYLabel = new QLabel("..."); targetsGroupBoxLayout->addWidget(unloadTargetYLabel);

		stepTargetZLabel = new QLabel("..."); targetsGroupBoxLayout->addWidget(stepTargetZLabel);
		stepTargetXLabel = new QLabel("..."); targetsGroupBoxLayout->addWidget(stepTargetXLabel);
		stepTargetYLabel = new QLabel("..."); targetsGroupBoxLayout->addWidget(stepTargetYLabel);
		//reals pos group
		QGroupBox *realGroupBox = new QGroupBox(tr("Position")); layout0->addWidget(realGroupBox); realGroupBox->setMinimumWidth(columnWidth);
		QVBoxLayout *realGroupBoxLayout = new QVBoxLayout(); realGroupBox->setLayout(realGroupBoxLayout);

		realPositionZLabel = new QLabel("..."); realGroupBoxLayout->addWidget(realPositionZLabel);
		realPositionXLabel = new QLabel("..."); realGroupBoxLayout->addWidget(realPositionXLabel);
		realPositionYLabel = new QLabel("..."); realGroupBoxLayout->addWidget(realPositionYLabel);
		//velocity group
		QGroupBox *velocityGroupBox = new QGroupBox(tr("Velocity")); layout0->addWidget(velocityGroupBox); velocityGroupBox->setMinimumWidth(columnWidth);
		QVBoxLayout *velocityGroupBoxLayout = new QVBoxLayout(); velocityGroupBox->setLayout(velocityGroupBoxLayout);

		setSpeedZLabel = new QLabel("..."); velocityGroupBoxLayout->addWidget(setSpeedZLabel);
		setSpeedXLabel = new QLabel("..."); velocityGroupBoxLayout->addWidget(setSpeedXLabel);
		setSpeedYLabel = new QLabel("..."); velocityGroupBoxLayout->addWidget(setSpeedYLabel);

		realSpeedZLabel = new QLabel("..."); velocityGroupBoxLayout->addWidget(realSpeedZLabel);
		realSpeedXLabel = new QLabel("..."); velocityGroupBoxLayout->addWidget(realSpeedXLabel);
		realSpeedYLabel = new QLabel("..."); velocityGroupBoxLayout->addWidget(realSpeedYLabel);
		layout0->addStretch(1);
		//
		///////////////////////////////////////////////////////////////////////////////////////layout1
		//autoRadios
		QGroupBox *radioButtonsGroupBox = new QGroupBox(tr("PLC Mode")); layout1->addWidget(radioButtonsGroupBox);
		QVBoxLayout *radioButtonsMainLayout = new QVBoxLayout(); radioButtonsGroupBox->setLayout(radioButtonsMainLayout);
		QHBoxLayout *radioButtonsLayout = new QHBoxLayout(); radioButtonsMainLayout->addLayout(radioButtonsLayout);

		autoRadioButton = new myQRadioButton(cranePlcTcpServer, "", "N20|3|0", tr("Auto"), this); radioButtonsLayout->addWidget(autoRadioButton);
		connect(autoRadioButton, SIGNAL(clicked(bool)), this, SLOT(autoRadioButtonSlot(bool)));//connect(stepRadioButton,SIGNAL(released()),this,SLOT(buttonReleasedSlot()));connect(stepRadioButton,SIGNAL(mouseLeftSignal()),this,SLOT(buttonReleasedSlot()));

		stepRadioButton = new myQRadioButton(cranePlcTcpServer, "N20|3|3", "N20|3|0", tr("Step"), this); radioButtonsLayout->addWidget(stepRadioButton);
		connect(stepRadioButton, SIGNAL(clicked(bool)), this, SLOT(setpRadioButtonSlot(bool)));

		maintenanceRadioButton = new myQRadioButton(cranePlcTcpServer, "N20|3|1", "N20|3|0", tr("Maintenance"), this); radioButtonsLayout->addWidget(maintenanceRadioButton);
		connect(maintenanceRadioButton, SIGNAL(clicked(bool)), this, SLOT(maintenanceRadioButtonSlot(bool)));

		QHBoxLayout *makeStepLayout = new QHBoxLayout(); radioButtonsMainLayout->addLayout(makeStepLayout);
		makeStepBtn = new myQPushButton(cranePlcTcpServer, "N20|3|4", "N20|3|0", tr("Make step"), this);
		makeStepBtn->setDisabled(true, false);
		makeStepLayout->addWidget(makeStepBtn);
		//

		recallStopPushButton = new myQPushButton(cranePlcTcpServer, "N20|3|23", "N20|3|0", tr("Stop recall"), this); layout1->addWidget(recallStopPushButton);

		stopLabel = new QLabel("STOPPED");
		stopLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
		stopLabel->setMinimumHeight(30);
		layout1->addWidget(stopLabel);

		okLabel = new QLabel("...");
		okLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
		okLabel->setFont(QFont("Arial", 12));
		okLabel->setMinimumHeight(30);
		layout1->addWidget(okLabel);

		CollisionLabel = new QLabel("...");
		CollisionLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
		CollisionLabel->setFont(QFont("Arial", 12));
		CollisionLabel->setMinimumHeight(30);
		layout1->addWidget(CollisionLabel);

		makeResetLabel = new QLabel("...");
		makeResetLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
		makeResetLabel->setFont(QFont("Arial", 12));
		makeResetLabel->setMinimumHeight(30);
		layout1->addWidget(makeResetLabel);

		alarm_13_1Label = new QLabel("...");
		alarm_13_1Label->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
		alarm_13_1Label->setFont(QFont("Arial", 12));
		alarm_13_1Label->setMinimumHeight(50);
		layout1->addWidget(alarm_13_1Label);

		alarm_13_2Label = new QLabel("...");
		alarm_13_2Label->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
		alarm_13_2Label->setFont(QFont("Arial", 11));
		alarm_13_2Label->setMinimumHeight(50);
		layout1->addWidget(alarm_13_2Label);
		//commands group
		QGroupBox *commandsGroupBox = new QGroupBox(tr("PLC commands")); layout1->addWidget(commandsGroupBox);
		QVBoxLayout *commandsGroupBoxLayout = new QVBoxLayout(); commandsGroupBox->setLayout(commandsGroupBoxLayout);
		command10PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|10", "N20|3|0", tr("Move axis Z up "), this); commandsGroupBoxLayout->addWidget(command10PushButton);
		command11PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|11", "N20|3|0", tr("Move axis Z down"), this); commandsGroupBoxLayout->addWidget(command11PushButton);
		command12PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|12", "N20|3|0", tr("Move axis X East"), this); commandsGroupBoxLayout->addWidget(command12PushButton);
		command13PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|13", "N20|3|0", tr("Move axis X West"), this); commandsGroupBoxLayout->addWidget(command13PushButton);
		command14PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|14", "N20|3|0", tr("Move axis Y North"), this); commandsGroupBoxLayout->addWidget(command14PushButton);
		command15PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|15", "N20|3|0", tr("Move axis Y South"), this); commandsGroupBoxLayout->addWidget(command15PushButton);
		command16PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|16", "N20|3|0", tr("Basket pins out"), this); commandsGroupBoxLayout->addWidget(command16PushButton);
		command17PushButton = new myQPushButton(cranePlcTcpServer, "", "N20|3|0", tr("Basket pins in"), this); commandsGroupBoxLayout->addWidget(command17PushButton);
		command18PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|18", "N20|3|0", tr("Lock basket in upper position"), this); commandsGroupBoxLayout->addWidget(command18PushButton);
		command19PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|19", "N20|3|0", tr("Unlock basket in upper position"), this); commandsGroupBoxLayout->addWidget(command19PushButton);
		connect(command17PushButton, SIGNAL(clicked(bool)), this, SLOT(pinsInSlot(bool)));
		//reset group
		QGroupBox *resetGroupBox = new QGroupBox(tr("PLC reset")); layout1->addWidget(resetGroupBox);
		QGridLayout *resetGroupBoxLayout = new QGridLayout(); resetGroupBox->setLayout(resetGroupBoxLayout);
		command20PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|20", "N20|3|0", tr("Reset Z axis "), this); resetGroupBoxLayout->addWidget(command20PushButton, 0, 0);
		command21PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|21", "N20|3|0", tr("Reset X axis "), this); resetGroupBoxLayout->addWidget(command21PushButton, 1, 0);
		command22PushButton = new myQPushButton(cranePlcTcpServer, "N20|3|22", "N20|3|0", tr("Reset Y axis "), this); resetGroupBoxLayout->addWidget(command22PushButton, 2, 0);
		runFasterInMaintenancePushButton = new myQPushButton(cranePlcTcpServer, "N20|3|5", "N20|3|0", tr("Speed up"), this); resetGroupBoxLayout->addWidget(runFasterInMaintenancePushButton, 3, 0, 1, 2);

		resetingZLabel = new QLabel(tr("   On Z reset position  ")); resetingZLabel->setMinimumWidth(100); resetGroupBoxLayout->addWidget(resetingZLabel, 0, 1, 1, 1, Qt::AlignHCenter);
		resetingXLabel = new QLabel(tr("   On X reset position  ")); resetingXLabel->setMinimumWidth(100); resetGroupBoxLayout->addWidget(resetingXLabel, 1, 1, 1, 1, Qt::AlignHCenter);
		resetingYLabel = new QLabel(tr("   On Y reset position  ")); resetingYLabel->setMinimumWidth(100); resetGroupBoxLayout->addWidget(resetingYLabel, 2, 1, 1, 1, Qt::AlignHCenter);

		layout1->addStretch(1);
		///////////////////////////////////////////////////////////////////////////////////////layout2
		//signals group
		QGroupBox *receivedSignalsGroupBox = new QGroupBox(tr("Receiving signals")); layout2->addWidget(receivedSignalsGroupBox);
		QVBoxLayout *receivedSignalsGroupBoxLayout = new QVBoxLayout(); receivedSignalsGroupBox->setLayout(receivedSignalsGroupBoxLayout);
		signal_13_03 = new QLabel(tr("  Extrusion line  stacker\n  empty basket permit crane  movements 40x")); signal_13_03->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_03, Qt::AlignHCenter | Qt::AlignCenter); signal_13_03->setMinimumHeight(28);
		signal_13_04 = new QLabel(tr("  Extrusion line  stacker\n  empty basket empty basket request 40x")); signal_13_04->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_04, Qt::AlignHCenter | Qt::AlignCenter); signal_13_04->setMinimumHeight(28);
		signal_13_05 = new QLabel(tr("  Extrusion line  stacker\n  full basket permit crane movements 41x")); signal_13_05->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_05, Qt::AlignHCenter | Qt::AlignCenter); signal_13_05->setMinimumHeight(28);
		signal_13_06 = new QLabel(tr("  Extrusion line  stacker\n  full basket ready  for ageing oven 41x")); signal_13_06->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_06, Qt::AlignHCenter | Qt::AlignCenter); signal_13_06->setMinimumHeight(28);
		signal_13_07 = new QLabel(tr("  Ageing oven (loading side)\n  permit crane movements 10x->15x")); signal_13_07->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_07, Qt::AlignHCenter | Qt::AlignCenter); signal_13_07->setMinimumHeight(28);
		signal_13_08 = new QLabel(tr("  Ageing oven (unloading side)\n  permit crane movements 16x")); signal_13_08->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_08, Qt::AlignHCenter | Qt::AlignCenter); signal_13_08->setMinimumHeight(28);
		signal_13_09 = new QLabel(tr("  Destacker pack 2 (13)\n  permit crane movements 52x")); signal_13_09->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_09, Qt::AlignHCenter | Qt::AlignCenter); signal_13_09->setMinimumHeight(28);
		signal_13_10 = new QLabel(tr("  Destacker pack 2 (13)\n  full basket request 52x")); signal_13_10->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_10, Qt::AlignHCenter | Qt::AlignCenter); signal_13_10->setMinimumHeight(28);
		signal_13_11 = new QLabel(tr("  Destacker pack 1 (15)\n  permit crane movements 51x")); signal_13_11->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_11, Qt::AlignHCenter | Qt::AlignCenter); signal_13_11->setMinimumHeight(28);
		signal_13_12 = new QLabel(tr("  Destacker pack 1 (15)\n  full basket request 51x")); signal_13_12->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_12, Qt::AlignHCenter | Qt::AlignCenter); signal_13_12->setMinimumHeight(28);
		signal_13_13 = new QLabel(tr("  Chain (18)\n  permit crane movements 50x")); signal_13_13->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_13, Qt::AlignHCenter | Qt::AlignCenter); signal_13_13->setMinimumHeight(28);
		signal_13_14 = new QLabel(tr("  Chain (18)\n  chain is empty with no job 50x")); signal_13_14->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_14, Qt::AlignHCenter | Qt::AlignCenter); signal_13_14->setMinimumHeight(28);
		signal_13_15 = new QLabel(tr("  Chain (18)\n  basket in position to be removed")); signal_13_15->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_13_15, Qt::AlignHCenter | Qt::AlignCenter); signal_13_15->setMinimumHeight(28);

		signal_14_00 = new QLabel(tr("  Crane 50x to  extrusion line stacker\n  (empty basket) permit chain movements 40x")); signal_14_00->setMinimumWidth(280);		receivedSignalsGroupBoxLayout->addWidget(signal_14_00, Qt::AlignHCenter | Qt::AlignCenter); signal_14_00->setMinimumHeight(28);

		QGroupBox *transmittedSignalsGroupBox = new QGroupBox(tr("Transmitting signals")); layout2->addWidget(transmittedSignalsGroupBox);
		QVBoxLayout *transmittedSignalsGroupBoxLayout = new QVBoxLayout(); transmittedSignalsGroupBox->setLayout(transmittedSignalsGroupBoxLayout);
		signal_14_01 = new QLabel(tr("  Extrusion line  stacker\n  empty basket delivered 40x")); signal_14_01->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_01, Qt::AlignHCenter | Qt::AlignCenter); signal_14_01->setMinimumHeight(28);
		signal_14_02 = new QLabel(tr("  Extrusion line  stacker\n  full basket permit chain movements 41x")); signal_14_02->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_02, Qt::AlignHCenter | Qt::AlignCenter); signal_14_02->setMinimumHeight(28);
		signal_14_03 = new QLabel(tr("  Extrusion line  stacker\n  full basket removed 41x")); signal_14_03->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_03, Qt::AlignHCenter | Qt::AlignCenter); signal_14_03->setMinimumHeight(28);
		signal_14_04 = new QLabel(tr("  Ageing oven (loading side)\n  permit  movements 10x->15x)")); signal_14_04->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_04, Qt::AlignHCenter | Qt::AlignCenter); signal_14_04->setMinimumHeight(28);
		signal_14_05 = new QLabel(tr("  Ageing oven (unloading side)\n  permit  movements 16x)")); signal_14_05->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_05, Qt::AlignHCenter | Qt::AlignCenter); signal_14_05->setMinimumHeight(28);
		signal_14_06 = new QLabel(tr("  Destacker pack 2(13)\n  permit  movements  52x")); signal_14_06->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_06, Qt::AlignHCenter | Qt::AlignCenter); signal_14_06->setMinimumHeight(28);
		signal_14_07 = new QLabel(tr("  Destacker pack 2(13)\n  full basket  delivered  52x")); signal_14_07->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_07, Qt::AlignHCenter | Qt::AlignCenter); signal_14_07->setMinimumHeight(28);
		signal_14_08 = new QLabel(tr("  Destacker pack 1(15)\n  permit  movements  51x")); signal_14_08->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_08, Qt::AlignHCenter | Qt::AlignCenter); signal_14_08->setMinimumHeight(28);
		signal_14_09 = new QLabel(tr("  Destacker pack 1(15)\n  full basket delivered 51x")); signal_14_09->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_09, Qt::AlignHCenter | Qt::AlignCenter); signal_14_09->setMinimumHeight(28);
		signal_14_10 = new QLabel(tr("  Chain (18)\n  permit movements 50x")); signal_14_10->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_10, Qt::AlignHCenter | Qt::AlignCenter); signal_14_10->setMinimumHeight(28);
		signal_14_11 = new QLabel(tr("  Chain (18)\n  basket delivered 50x")); signal_14_11->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_11, Qt::AlignHCenter | Qt::AlignCenter); signal_14_11->setMinimumHeight(28);
		signal_14_12 = new QLabel(tr("  Chain (18)\n  basket removed 50x")); signal_14_12->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_12, Qt::AlignHCenter | Qt::AlignCenter); signal_14_12->setMinimumHeight(28);
		signal_14_13 = new QLabel(tr("  Chain (18) basket is coming\n  from basket crane to  go out 50x")); signal_14_13->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_14_13, Qt::AlignHCenter | Qt::AlignCenter); signal_14_13->setMinimumHeight(28);
		signal_16_12 = new QLabel(tr("  Chain (18) scrap basket\n  delivered 50x")); signal_16_12->setMinimumWidth(280);		transmittedSignalsGroupBoxLayout->addWidget(signal_16_12, Qt::AlignHCenter | Qt::AlignCenter); signal_16_12->setMinimumHeight(28);
		layout2->addStretch(1);

		///////////////////////////////////////////////////////////////////////////////////////layout3
		//magnetic group
		QGroupBox *magneticSwitchesGroupBox = new QGroupBox(tr("Magnetic switches")); layout3->addWidget(magneticSwitchesGroupBox);
		QVBoxLayout *magneticSwitchesGroupBoxpBoxLayout = new QVBoxLayout(); magneticSwitchesGroupBox->setLayout(magneticSwitchesGroupBoxpBoxLayout);
		switch_15_04 = new QLabel(tr("  Load pins out load is locked")); switch_15_04->setMinimumWidth(280);		switch_15_04->setMinimumHeight(28); magneticSwitchesGroupBoxpBoxLayout->addWidget(switch_15_04, Qt::AlignHCenter | Qt::AlignCenter);
		switch_15_05 = new QLabel(tr("  Load pins out load is unlocked")); switch_15_05->setMinimumWidth(280);		switch_15_05->setMinimumHeight(28); magneticSwitchesGroupBoxpBoxLayout->addWidget(switch_15_05, Qt::AlignHCenter | Qt::AlignCenter);
		switch_15_06 = new QLabel(tr("  Left pins out basket is locked")); switch_15_06->setMinimumWidth(280);		switch_15_06->setMinimumHeight(28); magneticSwitchesGroupBoxpBoxLayout->addWidget(switch_15_06, Qt::AlignHCenter | Qt::AlignCenter);
		switch_15_07 = new QLabel(tr("  Right pins out basket is locked")); switch_15_07->setMinimumWidth(280);		switch_15_07->setMinimumHeight(28); magneticSwitchesGroupBoxpBoxLayout->addWidget(switch_15_07, Qt::AlignHCenter | Qt::AlignCenter);
		switch_15_08 = new QLabel(tr("  Left pins in basket is unlocked")); switch_15_08->setMinimumWidth(280);		switch_15_08->setMinimumHeight(28); magneticSwitchesGroupBoxpBoxLayout->addWidget(switch_15_08, Qt::AlignHCenter | Qt::AlignCenter);
		switch_15_09 = new QLabel(tr("  Right pins in basket is unlocked")); switch_15_09->setMinimumWidth(280);		switch_15_09->setMinimumHeight(28); magneticSwitchesGroupBoxpBoxLayout->addWidget(switch_15_09, Qt::AlignHCenter | Qt::AlignCenter);

		QGroupBox *valveGroupBox = new QGroupBox(tr("Pneumatic valves")); layout3->addWidget(valveGroupBox);
		QVBoxLayout *valveGroupBoxLayout = new QVBoxLayout(); valveGroupBox->setLayout(valveGroupBoxLayout);
		valve_16_00 = new QLabel(tr("  Lock load in upper position")); valve_16_00->setMinimumWidth(250); valveGroupBoxLayout->addWidget(valve_16_00, Qt::AlignHCenter); valve_16_00->setMinimumHeight(30);
		valve_16_01 = new QLabel(tr("  Unock load in upper position")); valve_16_01->setMinimumWidth(250); valveGroupBoxLayout->addWidget(valve_16_01, Qt::AlignHCenter); valve_16_01->setMinimumHeight(30);
		valve_16_02 = new QLabel(tr("  Lock basket")); valve_16_02->setMinimumWidth(250); valveGroupBoxLayout->addWidget(valve_16_02, Qt::AlignHCenter); valve_16_02->setMinimumHeight(30);
		valve_16_03 = new QLabel(tr("  Unlock basket")); valve_16_03->setMinimumWidth(250); valveGroupBoxLayout->addWidget(valve_16_03, Qt::AlignHCenter); valve_16_03->setMinimumHeight(30);
		layout2->addStretch(1);

		///////////////////////////////////////////////////////////////////////////////////////layout4
		//alarms group
		QGroupBox *alarmsGroupBox = new QGroupBox(tr("Alarms")); layout4->addWidget(alarmsGroupBox);
		QVBoxLayout *alarmsGroupBoxLayout = new QVBoxLayout(); alarmsGroupBox->setLayout(alarmsGroupBoxLayout);
		alarmLabel_16_05 = new QLabel(tr("  Overweight basket")); alarmLabel_16_05->setMinimumWidth(175); alarmLabel_16_05->setMinimumHeight(30); alarmsGroupBoxLayout->addWidget(alarmLabel_16_05, Qt::AlignHCenter); alarmLabel_16_05->setMinimumHeight(25);
		alarmLabel_16_06 = new QLabel(tr("  Axis Z overspeed")); alarmLabel_16_06->setMinimumWidth(175); alarmLabel_16_06->setMinimumHeight(30); alarmsGroupBoxLayout->addWidget(alarmLabel_16_06, Qt::AlignHCenter); alarmLabel_16_06->setMinimumHeight(25);
		alarmLabel_16_07 = new QLabel(tr("  Axis Z underspeed")); alarmLabel_16_07->setMinimumWidth(175); alarmLabel_16_07->setMinimumHeight(30); alarmsGroupBoxLayout->addWidget(alarmLabel_16_07, Qt::AlignHCenter); alarmLabel_16_07->setMinimumHeight(25);
		alarmLabel_16_08 = new QLabel(tr("  Axis X overspeed")); alarmLabel_16_08->setMinimumWidth(175); alarmLabel_16_08->setMinimumHeight(30); alarmsGroupBoxLayout->addWidget(alarmLabel_16_08, Qt::AlignHCenter); alarmLabel_16_08->setMinimumHeight(25);
		alarmLabel_16_09 = new QLabel(tr("  Axis X underspeed")); alarmLabel_16_09->setMinimumWidth(175); alarmLabel_16_09->setMinimumHeight(30); alarmsGroupBoxLayout->addWidget(alarmLabel_16_09, Qt::AlignHCenter); alarmLabel_16_09->setMinimumHeight(25);
		alarmLabel_16_10 = new QLabel(tr("  Axis Y overspeed")); alarmLabel_16_10->setMinimumWidth(175); alarmLabel_16_10->setMinimumHeight(30); alarmsGroupBoxLayout->addWidget(alarmLabel_16_10, Qt::AlignHCenter); alarmLabel_16_10->setMinimumHeight(25);
		alarmLabel_16_11 = new QLabel(tr("  Axis Y underspeed")); alarmLabel_16_11->setMinimumWidth(175); alarmLabel_16_11->setMinimumHeight(30); alarmsGroupBoxLayout->addWidget(alarmLabel_16_11, Qt::AlignHCenter); alarmLabel_16_11->setMinimumHeight(25);
		showAlarmsHistoryBtn = new QPushButton(tr("Show alarms history"));						   showAlarmsHistoryBtn->setMinimumWidth(175); showAlarmsHistoryBtn->setMinimumHeight(30); alarmsGroupBoxLayout->addWidget(showAlarmsHistoryBtn, Qt::AlignHCenter); showAlarmsHistoryBtn->setMinimumHeight(25);
		connect(showAlarmsHistoryBtn, SIGNAL(clicked()), this, SLOT(showAlarmsHistoryBtnSlot()));

		//plcStatusGroupBox
		QGroupBox *plcStatusGroupBox = new QGroupBox(tr("Plc status")); layout4->addWidget(plcStatusGroupBox);
		QVBoxLayout *plcStatusGroupBoxLayout = new QVBoxLayout(); plcStatusGroupBox->setLayout(plcStatusGroupBoxLayout);
		plcStatusLabel_21_00 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_00); plcStatusLabel_21_00->setMinimumWidth(250); plcStatusLabel_21_00->setMinimumHeight(35);
		plcStatusLabel_21_01 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_01); plcStatusLabel_21_01->setMinimumWidth(250); plcStatusLabel_21_01->setMinimumHeight(35);
		plcStatusLabel_21_02 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_02); plcStatusLabel_21_02->setMinimumWidth(250); plcStatusLabel_21_02->setMinimumHeight(35);
		plcStatusLabel_21_03 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_03); plcStatusLabel_21_03->setMinimumWidth(250); plcStatusLabel_21_03->setMinimumHeight(35);
		plcStatusLabel_21_04 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_04); plcStatusLabel_21_04->setMinimumWidth(250); plcStatusLabel_21_04->setMinimumHeight(35);
		plcStatusLabel_21_05 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_05); plcStatusLabel_21_05->setMinimumWidth(250); plcStatusLabel_21_05->setMinimumHeight(35);
		plcStatusLabel_21_06 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_06); plcStatusLabel_21_06->setMinimumWidth(250); plcStatusLabel_21_06->setMinimumHeight(35);
		plcStatusLabel_21_07 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_07); plcStatusLabel_21_07->setMinimumWidth(250); plcStatusLabel_21_07->setMinimumHeight(35);
		plcStatusLabel_21_08 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_08); plcStatusLabel_21_08->setMinimumWidth(250); plcStatusLabel_21_08->setMinimumHeight(35);
		plcStatusLabel_21_09 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_09); plcStatusLabel_21_09->setMinimumWidth(250); plcStatusLabel_21_09->setMinimumHeight(35);
		plcStatusLabel_21_10 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_10); plcStatusLabel_21_10->setMinimumWidth(250); plcStatusLabel_21_10->setMinimumHeight(35);
		plcStatusLabel_21_11 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_11); plcStatusLabel_21_11->setMinimumWidth(250); plcStatusLabel_21_11->setMinimumHeight(35);
		plcStatusLabel_21_12 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_12); plcStatusLabel_21_12->setMinimumWidth(250); plcStatusLabel_21_12->setMinimumHeight(35);
		plcStatusLabel_21_13 = new QLabel(tr("")); plcStatusGroupBoxLayout->addWidget(plcStatusLabel_21_13); plcStatusLabel_21_13->setMinimumWidth(250); plcStatusLabel_21_13->setMinimumHeight(35);

		layout3->addStretch(1);
		/////////////////////////////////////////////////////////////////////////////////////////////
		//downLayout
		QImage image(":/plcPos.png");
		// Annotate the existing 1281 x 401 layout without covering its axis labels.
		if (!image.isNull())
		{
			image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
			QPainter painter(&image);
			painter.setRenderHint(QPainter::TextAntialiasing);
			painter.scale(image.width() / 1281.0, image.height() / 401.0);
			painter.setPen(QColor("#ffff00"));
			QFont directionFont("Arial");
			directionFont.setPixelSize(18);
			directionFont.setBold(true);
			painter.setFont(directionFont);
			painter.drawText(QRect(1046, 256, 26, 24), Qt::AlignCenter, tr("N"));
			painter.drawText(QRect(1013, 361, 26, 24), Qt::AlignCenter, tr("S"));
			painter.drawText(QRect(1098, 303, 26, 24), Qt::AlignCenter, tr("E"));
			painter.drawText(QRect(980, 288, 26, 24), Qt::AlignCenter, tr("W"));
			directionFont.setPixelSize(16);
			painter.setFont(directionFont);
			painter.drawText(QRect(900, 348, 40, 24), Qt::AlignCenter, tr("up"));
			painter.drawText(QRect(937, 263, 60, 24), Qt::AlignCenter, tr("down"));
		}
		QLabel *imageLabel = new QLabel;
		imageLabel->setMinimumWidth(1281);
		imageLabel->setMinimumHeight(401);
		imageLabel->setPixmap(QPixmap::fromImage(image));
		imageLabel->setAlignment(Qt::AlignCenter);
		downLayout->addWidget(imageLabel);
		mainScrollArea->setWidget(mainScrollAreaWidget);

		// QLabel otherwise derives its text role from the inherited background role.
		// A Dark background selects Light, which QSS tints from each status color.
		foreach (QLabel *label, mainScrollAreaWidget->findChildren<QLabel*>())
			label->setForegroundRole(QPalette::WindowText);
}
plcManualClass::~plcManualClass()
{
}
void plcManualClass::showAlarmsHistoryBtnSlot()
{
#ifdef EMULATE
		QVector<QVariant> bitsVector = execDBQuery("select bit from logErrorPlc where type='craneError' order by tid desc limit 100",MAIN_DB_CONNECTION);
		QVector<QVariant> descriptionVector = execDBQuery("select description from logErrorPlc where type='craneError' order by tid desc limit 100",MAIN_DB_CONNECTION);
		QVector<QVariant> dateVector = execDBQuery("select date from logErrorPlc where type='craneError' order by tid desc limit 100",MAIN_DB_CONNECTION);
#else
		QVector<QVariant> bitsVector = execDBQuery("select top 100 bit from logErrorPlc where type='craneError' order by tid desc",MAIN_DB_CONNECTION);
		QVector<QVariant> descriptionVector = execDBQuery("select top 100 description from logErrorPlc where type='craneError' order by tid desc",MAIN_DB_CONNECTION);
		QVector<QVariant> dateVector = execDBQuery("select top 100 date from logErrorPlc where type='craneError' order by tid desc",MAIN_DB_CONNECTION);
#endif
		QDialog dlg(this);
		QVBoxLayout *mainLayout = new QVBoxLayout(); dlg.setLayout(mainLayout);
		QTableWidget *errorsTableWidget = new QTableWidget(); mainLayout->addWidget(errorsTableWidget);
		errorsTableWidget->setAlternatingRowColors(true);
		errorsTableWidget->horizontalHeader()->setStretchLastSection(true);
		errorsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
		errorsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
		errorsTableWidget->setColumnCount(2);
		QStringList ch; ch << tr("Date") << tr("Description");
		errorsTableWidget->setHorizontalHeaderLabels(ch);
		errorsTableWidget->setRowCount(bitsVector.count());
		for (int i = 0; i<bitsVector.count(); i++)
		{
				errorsTableWidget->setItem(i, 0, new QTableWidgetItem(dateVector[i].toString(), Qt::DisplayRole));
				errorsTableWidget->setItem(i, 1, new QTableWidgetItem(bitsVector[i].toString() + ":" + descriptionVector[i].toString(), Qt::DisplayRole));
		}
		errorsTableWidget->resizeColumnsToContents();
		dlg.resize(600, 900);
		dlg.exec();
}

void plcManualClass::plcStatusChanged(int index, int value)
{
	QVector<bool> bitsVector = deprecatedCalculateBits(value);
	//
	if (index == 0)		fromPcLabel->setText(QString().number(value));
		
	if (index == 1)		toPcLabel->setText(QString().number(value));
		
	if (index == 10) {
		fromPlcLabel->setText(QString().number(value)); 
		plcWidget->setFromPosLabelText(tr("  From %1").arg(value)); 
	}
	if (index == 11) {
		toPlcLabel->setText(QString().number(value)); 
		plcWidget->setToPosLabelText(tr("  To %1").arg(value)); 
	}
	if (index == 12) {
		N20_12 = value;
		if (N20_12 == 0) { 
			fromToPlcStatus->setText(tr("Plc not in a mission")); 
			plcWidget->setStatus0LabelText(tr("  Not in a mission"), "QLabel{background-color:white}"); 
		}
		if (N20_12 == 1) {
			fromToPlcStatus->setText(tr("Executing mission")); 
			plcWidget->setStatus0LabelText(tr("  Executing mission"), "QLabel{background-color:white}"); 
		}
		if (N20_12 == 2) { 
			fromToPlcStatus->setText(tr("Mission finished")); 
			plcWidget->setStatus0LabelText(tr("  Mission finished"), "QLabel{background-color:white}"); 
		}
		if (N20_12 == 3) { 
			fromToPlcStatus->setText(tr("Mission Not finished, overweight basket")); 
			plcWidget->setStatus0LabelText(tr("  Overweight basket"), "QLabel{background-color:white}"); 
		}
	}
	if (index == 13)
	{
		if (bitsVector[0]) {
			okLabel->setText(tr("System Ok"));
			okLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setStatus1LabelText(tr("  Plc"), "QLabel{background-color:white}");
		}
		else {
			okLabel->setText(tr("System Not Ok (recall stop)"));
			okLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setStatus1LabelText(tr("  Plc not ok"), "QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}

		if (!bitsVector[16]) {
			CollisionLabel->setText(tr("Collision Ok"));
			CollisionLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setStatus2LabelText(tr("  No Collision"), "QLabel{background-color:white}");
		}
		else {
			CollisionLabel->setText(tr("Collision Not Ok (recall stop)"));
			CollisionLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setStatus2LabelText(tr("  Basket Collision Alarm"), "QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}

		if (bitsVector[1]) {
			alarm_13_1Label->setText(tr("Alarm of basket crane\ninverter U1 or U2"));
			alarm_13_1Label->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setError0LabelText(tr("  Alarm of basket crane\n  inverter U1 or U2"), "QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else {
			alarm_13_1Label->setText(tr("Basket crane\ninverter U1 and U2 Ok"));
			alarm_13_1Label->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setError0LabelText(tr("  Basket crane\n  inverter U1 and U2"), "QLabel{background-color:white}");
		}

		if (bitsVector[2]) {
			alarm_13_2Label->setText(tr("Alarm of basket crane\nmotors(X,Y or Z axis)"));
			alarm_13_2Label->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setError1LabelText(tr(" Alarm basket crane\n motors (X,Y or Z axis)"), "QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else {
			alarm_13_2Label->setText(tr("Basket crane motors\n(X,Y and Z axis) Ok"));
			alarm_13_2Label->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setError1LabelText(tr("  Basket crane motors\n  (X,Y and Z axis)"), "QLabel{background-color:white}");
		}

		//////signals
		if (bitsVector[3])   signal_13_03->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_03->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[4])	 signal_13_04->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_04->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[5])   signal_13_05->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_05->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[6])   signal_13_06->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_06->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[7])   signal_13_07->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_07->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[8])   signal_13_08->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_08->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[9])   signal_13_09->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_09->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[10])  signal_13_10->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_10->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[11])  signal_13_11->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_11->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[12])  signal_13_12->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_12->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[13])  signal_13_13->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_13->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[14])  signal_13_14->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_14->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[15])  signal_13_15->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_13_15->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
	}

	if (index == 14)
	{
		if (bitsVector[0])   signal_14_00->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_00->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[1])   signal_14_01->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_01->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[2])   signal_14_02->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_02->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[3])   signal_14_03->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_03->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[4])   signal_14_04->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_04->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[5])   signal_14_05->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_05->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[6])   signal_14_06->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_06->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[7])   signal_14_07->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_07->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[8])   signal_14_08->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_08->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[9])   signal_14_09->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_09->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[10])  signal_14_10->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_10->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[11])  signal_14_11->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_11->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[12])  signal_14_12->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_12->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[13])  signal_14_13->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else	signal_14_13->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");

		if (bitsVector[14])  plcStatusLabel_14_14->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");  else  plcStatusLabel_14_14->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[15])  plcStatusLabel_14_15->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");    else  plcStatusLabel_14_15->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
	}

	if (index == 15)
	{
		if (bitsVector[0])	plcStatusLabel_15_00->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}"); else plcStatusLabel_15_00->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");

		if (bitsVector[1])	resetingZLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else resetingZLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[2])	resetingXLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else resetingXLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[3])	resetingYLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else resetingYLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");

		if (bitsVector[4])	switch_15_04->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else switch_15_04->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[5])	switch_15_05->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else switch_15_05->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[6])	switch_15_06->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else switch_15_06->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[7])	switch_15_07->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else switch_15_07->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[8])	switch_15_08->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else switch_15_08->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[9])	switch_15_09->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else switch_15_09->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");

		if (bitsVector[13])	stateLabel_15_13->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else stateLabel_15_13->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[14])	stateLabel_15_14->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else stateLabel_15_14->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[15])	stateLabel_15_15->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else stateLabel_15_15->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		//

		plcModeEnum = NO_MODE;
		if (bitsVector[10] == true)
		{
			plcModeEnum = MAINTENANCE_MODE;
			maintenanceRadioButton->blockSignals(true);
			maintenanceRadioButton->setChecked(true);
			maintenanceRadioButton->blockSignals(false);
			abordMissionPushButton->setEnabled(true);
			command10PushButton->setEnabled(true);
			command11PushButton->setEnabled(true);
			command12PushButton->setEnabled(true);
			command13PushButton->setEnabled(true);
			command14PushButton->setEnabled(true);
			command15PushButton->setEnabled(true);
			command16PushButton->setEnabled(true);
			command17PushButton->setEnabled(true);
			command18PushButton->setEnabled(true);
			command19PushButton->setEnabled(true);
			command20PushButton->setEnabled(true);
			command21PushButton->setEnabled(true);
			command22PushButton->setEnabled(true);
		}
		else
		{
			//abordMissionPushButton->setEnabled(false,false);
			//command10PushButton->setEnabled(false);
			//command11PushButton->setEnabled(false);
			//command12PushButton->setEnabled(false);
			//command13PushButton->setEnabled(false);
			//command14PushButton->setEnabled(false);
			//command15PushButton->setEnabled(false);
			//command16PushButton->setEnabled(false);
			//command17PushButton->setEnabled(false);
			//command18PushButton->setEnabled(false);
			//command19PushButton->setEnabled(false);
			//command20PushButton->setEnabled(false);
			//command21PushButton->setEnabled(false);
			//command22PushButton->setEnabled(false);
		}
		if (bitsVector[11] == true)
		{
			plcModeEnum = AUTO_MODE;
			continueMissionPushButton->setEnabled(true);
			autoRadioButton->blockSignals(true);
			autoRadioButton->setChecked(true);
			autoRadioButton->blockSignals(false);
		}
		else
		{
			continueMissionPushButton->setEnabled(false);
		}
		//makeStepBtn->setEnabled(false);
		if (bitsVector[12] == true)
		{
			plcModeEnum = STEP_MODE;
			stepRadioButton->blockSignals(true);
			stepRadioButton->setChecked(true);
			stepRadioButton->blockSignals(false);
			makeStepBtn->setEnabled(true);
		}
	}

	if (index == 16)
	{
		if (bitsVector[4])
		{
			stopLabel->setText(tr("STOPPED"));
			stopLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setStopLabelText(tr("  STOPPED"), "QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
			execDBQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('craneError','N22[16].%1','%2','%3',%4)").arg(4).arg("Stop").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION);
		}
		else
		{
			stopLabel->setText(tr("No active stop"));
			stopLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setStopLabelText(tr("  No active stop"), "QLabel{background-color:white}");
		}
		if (bitsVector[12])signal_16_12->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else signal_16_12->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[0])valve_16_00->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else valve_16_00->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[1])valve_16_01->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else valve_16_01->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[2])valve_16_02->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else valve_16_02->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");
		if (bitsVector[3])valve_16_03->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}"); else valve_16_03->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#475569;color:#ffffff;padding:2px 4px;}");

		if (bitsVector[5]){ alarmLabel_16_05->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}"); execDBQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('craneError','N22[16].%1','%2','%3',%4)").arg(5).arg("Overweight basket").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION); }
		else alarmLabel_16_05->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		if (bitsVector[6]){ alarmLabel_16_06->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}"); execDBQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('craneError','N22[16].%1','%2','%3',%4)").arg(6).arg("Axis Z overspeed ").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION); }
		else alarmLabel_16_06->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		if (bitsVector[7]){ alarmLabel_16_07->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}"); execDBQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('craneError','N22[16].%1','%2','%3',%4)").arg(7).arg("Axis Z underspeed").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION); }
		else alarmLabel_16_07->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		if (bitsVector[8]){ alarmLabel_16_08->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}"); execDBQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('craneError','N22[16].%1','%2','%3',%4)").arg(8).arg("Axis X overspeed ").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION); }
		else alarmLabel_16_08->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		if (bitsVector[9]){ alarmLabel_16_09->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}"); execDBQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('craneError','N22[16].%1','%2','%3',%4)").arg(9).arg("Axis X underspeed").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION); }
		else alarmLabel_16_09->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		if (bitsVector[10]){ alarmLabel_16_10->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}"); execDBQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('craneError','N22[16].%1','%2','%3',%4)").arg(10).arg("Axis Y overspeed ").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION); }
		else alarmLabel_16_10->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		if (bitsVector[11]){ alarmLabel_16_11->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}"); execDBQuery(QString("insert into logErrorPlc (type,bit,description,date,sec) values('craneError','N22[16].%1','%2','%3',%4)").arg(11).arg("Axis Y underspeed").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()), MAIN_DB_CONNECTION); }
		else alarmLabel_16_11->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		if (bitsVector[5])
			plcWidget->setError1LabelText(tr(" Overweight basket\n   press 'Stop recall'."), "QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		else
			plcWidget->setError1LabelText(tr(" Not overweight basket"), "QLabel{background-color:white}");

		if (bitsVector[15])
		{
			makeResetLabel->setText(tr("Reset all axis"));
			makeResetLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setMakeResetLabelText(tr("  Reset all axis"), "QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			makeResetLabel->setText(tr("No reset needed"));
			makeResetLabel->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
			plcWidget->setMakeResetLabelText(tr("  No reset needed"), "QLabel{background-color:white}");
		}
	}

	if (index == 21)
	{
		if (bitsVector[0])
		{
			plcStatusLabel_21_00->setText(tr("  1769-HSC/B BC_axisZ_axisX\n  module Ok"));
			plcStatusLabel_21_00->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_00->setText(tr("  1769-HSC/B BC_axisZ_axisX\n  module fault"));
			plcStatusLabel_21_00->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[1])
		{
			plcStatusLabel_21_01->setText(tr("  1769-HSC/B BC_axisY_PC_axisZ\n  module Ok"));
			plcStatusLabel_21_01->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_01->setText(tr("  1769-HSC/B BC_axisY_PC_axisZ\n  module fault"));
			plcStatusLabel_21_01->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[2])
		{
			plcStatusLabel_21_02->setText(tr("  1769-HSC/B PC_axisX_axisY\n  module Ok"));
			plcStatusLabel_21_02->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_02->setText(tr("  1769-HSC/B PC_axisX_axisY\n  module fault"));
			plcStatusLabel_21_02->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[3])
		{
			plcStatusLabel_21_03->setText(tr("  1769-IQ32T/A DIG_Input_Mod_400\n  module Ok"));
			plcStatusLabel_21_03->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_03->setText(tr("  1769-IQ32T/A DIG_Input_Mod_400\n  module fault"));
			plcStatusLabel_21_03->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[4])
		{
			plcStatusLabel_21_04->setText(tr("  1769-IQ32T/A DIG_Input_Mod_500\n  module Ok"));
			plcStatusLabel_21_04->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_04->setText(tr("   1769-IQ32T/A DIG_Input_Mod_500\n  module fault"));
			plcStatusLabel_21_04->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[5])
		{
			plcStatusLabel_21_05->setText(tr("  1769-IF4I/A ANL_Input_600\n  module Ok"));
			plcStatusLabel_21_05->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_05->setText(tr("  1769-IF4I/A ANL_Input_600\n  module fault"));
			plcStatusLabel_21_05->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[6])
		{
			plcStatusLabel_21_06->setText(tr("  1769-OF4/A ANL_Output_700\n  module Ok"));
			plcStatusLabel_21_06->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_06->setText(tr("  1769-OF4/A ANL_Output_700\n  module fault"));
			plcStatusLabel_21_06->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[7])
		{
			plcStatusLabel_21_07->setText(tr("  1769-OW16/A DIG_Output_800\n  module Ok"));
			plcStatusLabel_21_07->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_07->setText(tr("  1769-OW16/A DIG_Output_800\n  module fault"));
			plcStatusLabel_21_07->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[8])
		{
			plcStatusLabel_21_08->setText(tr("  1769-OW16/A DIG_Output_900\n  module Ok"));
			plcStatusLabel_21_08->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_08->setText(tr("  1769-OW16/A DIG_Output_900\n  module fault"));
			plcStatusLabel_21_08->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[9])
		{
			plcStatusLabel_21_09->setText(tr("  1769-OW16/A DIG_Output_1000\n  module Ok"));
			plcStatusLabel_21_09->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_09->setText(tr("  1769-OW16/A DIG_Output_1000\n  module fault"));
			plcStatusLabel_21_09->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[10])
		{
			plcStatusLabel_21_10->setText(tr("  1769-OW16/A DIG_Output_1100\n  module Ok"));
			plcStatusLabel_21_10->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_10->setText(tr("  1769-OW16/A DIG_Output_1100\n  module fault"));
			plcStatusLabel_21_10->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[11])
		{
			plcStatusLabel_21_11->setText(tr("  1769-OW16/A DIG_Output_1200\n  module Ok"));
			plcStatusLabel_21_11->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_11->setText(tr("  1769-OW16/A DIG_Output_1200\n  module fault"));
			plcStatusLabel_21_11->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (bitsVector[12])
		{
			plcStatusLabel_21_12->setText(tr("  Communication with pack lgx Ok"));
			plcStatusLabel_21_12->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_12->setText(tr("  Communication with pack lgx fault"));
			plcStatusLabel_21_12->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#b91c1c;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		/////
		if (!bitsVector[13])
		{
			plcStatusLabel_21_13->setText(tr("  PLC Battery Ok"));
			plcStatusLabel_21_13->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#166534;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
		else
		{
			plcStatusLabel_21_13->setText(tr("  PLC Low Battery minor fault"));
			plcStatusLabel_21_13->setStyleSheet("QLabel,QLabel:enabled,QLabel:disabled{background-color:#92400e;color:#ffffff;font-weight:600;padding:2px 4px;}");
		}
	}
}

void plcManualClass::plcFloatValuesChanged(int index, float value)
{
	if (index == 0)loadTargetZLabel->setText(QString("Load target Z :%1mm").arg(value, 8, 'f', 2));
	if (index == 1)loadTargetXLabel->setText(QString("Load target X :%1mm").arg(value, 8, 'f', 2));
	if (index == 2)loadTargetYLabel->setText(QString("Load target Y :%1mm").arg(value, 8, 'f', 2));

	if (index == 3)unloadTargetZLabel->setText(QString("Unload target Z :%1mm").arg(value, 8, 'f', 2));
	if (index == 4)unloadTargetXLabel->setText(QString("Unload target X :%1mm").arg(value, 8, 'f', 2));
	if (index == 5)unloadTargetYLabel->setText(QString("Unload target Y :%1mm").arg(value, 8, 'f', 2));

	if (index == 6)stepTargetZLabel->setText(QString("Current step target Z :%1mm").arg(value, 8, 'f', 2));
	if (index == 7)stepTargetXLabel->setText(QString("Current step target X :%1mm").arg(value, 8, 'f', 2));
	if (index == 8)stepTargetYLabel->setText(QString("Current step target Y :%1mm").arg(value, 8, 'f', 2));

	if (index == 9)realPositionZLabel->setText(QString("Z Position :%1mm").arg(value, 8, 'f', 2));
	if (index == 10)realPositionXLabel->setText(QString("X Position :%1mm").arg(value, 8, 'f', 2));
	if (index == 11)realPositionYLabel->setText(QString("Y Position :%1mm").arg(value, 8, 'f', 2));

	if (index == 12)setSpeedZLabel->setText(QString("Z Set speed :%1m/min").arg(value, 8, 'f', 2));
	if (index == 13)setSpeedXLabel->setText(QString("X Set speed :%1m/min").arg(value, 8, 'f', 2));
	if (index == 14)setSpeedYLabel->setText(QString("Y Set speed :%1m/min").arg(value, 8, 'f', 2));

	if (index == 15){ realSpeedZLabel->setText(QString("Z real speed :%1m/min").arg(value, 8, 'f', 2)); plcSetZVelocityForOpenGl = value; }
	if (index == 16){ realSpeedXLabel->setText(QString("X real speed :%1m/min").arg(value, 8, 'f', 2)); plcSetXVelocityForOpenGl = value; }
	if (index == 17){ realSpeedYLabel->setText(QString("Y real speed :%1m/min").arg(value, 8, 'f', 2)); plcSetYVelocityForOpenGl = value; }
}

////////////////////////////////////////////////////////////////
void plcManualClass::buttonReleasedSlot()
{
		setPlcNetworkActive(PLC_NETWORK_CRANE, false);
		cranePlcTcpServer->sendString(QString("N20|3|0"));
		setPlcNetworkIdle(PLC_NETWORK_CRANE);
}

void plcManualClass::continueMissionPushButtonSlot(bool checked)
{
	QMessageBox msgBox(this);
	msgBox.setWindowIcon(QIcon(":/logo_16x16.png"));
	msgBox.setInformativeText("Contineu mission?");
	msgBox.setText("Important!\nDo you confirm that the basket is safe on the pins?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	if (msgBox.exec() == QMessageBox::Yes)
	{
		setPlcNetworkActive(PLC_NETWORK_CRANE, false);
		cranePlcTcpServer->sendString(QString("N20|3|25"));
		cranePlcTcpServer->sendString(QString("N20|3|0"));
		setPlcNetworkIdle(PLC_NETWORK_CRANE);
	}
}

void plcManualClass::autoRadioButtonSlot(bool checked)
{
		QMessageBox msgBox(this);
		msgBox.setWindowIcon(QIcon(":/logo_16x16.png"));
		msgBox.setInformativeText("Close the software?");
		msgBox.setText("Important!\nIf you made any manual movements while in maintenance mode that resulted in changing the positions of baskets it is very important to close the software and make the appropriate changes to the database.\nIf you don't do so, a serious accident WILL occur.");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::Yes);
		if (msgBox.exec() == QMessageBox::Yes)
				EXIT(tr("user selection"), tr("user selected to exit software to modify the database."));
		{
				QFile file("userSelections.txt");
				file.open(QIODevice::Append | QIODevice::Text);
				QTextStream out(&file);
				out << QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss\t");
				out << QString(" user selected did not make any manual movements while in maintenance mode that resulted in changing the positions of baskets") << "\n";
				file.close();
		}
		cranePlcTcpServer->sendString(QString("N20|3|2"));
		if (plcModeEnum == AUTO_MODE)//let the plc do it
				autoRadioButton->setChecked(true);
		else if (plcModeEnum == STEP_MODE)
				stepRadioButton->setChecked(true);
		else if (plcModeEnum == MAINTENANCE_MODE)
				maintenanceRadioButton->setChecked(true);
}
void plcManualClass::setpRadioButtonSlot(bool checked)
{
		//cranePlcTcpServer->sendString(QString("N20|3|3"));
		//cranePlcTcpServer->sendString(QString("N20|3|0"));
		if (plcModeEnum == AUTO_MODE)//let the plc do it
				autoRadioButton->setChecked(true);
		else if (plcModeEnum == STEP_MODE)
				stepRadioButton->setChecked(true);
		else if (plcModeEnum == MAINTENANCE_MODE)
				maintenanceRadioButton->setChecked(true);
}
void plcManualClass::maintenanceRadioButtonSlot(bool checked)
{
		//cranePlcTcpServer->sendString(QString("N20|3|1"));
		//cranePlcTcpServer->sendString(QString("N20|3|0"));
		if (plcModeEnum == AUTO_MODE)//let the plc do it
				autoRadioButton->setChecked(true);
		else if (plcModeEnum == STEP_MODE)
				stepRadioButton->setChecked(true);
		else if (plcModeEnum == MAINTENANCE_MODE)
				maintenanceRadioButton->setChecked(true);
}
void plcManualClass::pinsInSlot(bool checked)
{
		QMessageBox msgBox(this);
		msgBox.setWindowIcon(QIcon(":/logo_16x16.png"));
		msgBox.setInformativeText("Do you want to proceed?");
		msgBox.setText("You selected to retrieve the pins that hold the basket on the air.\nMake sure that the basket is safe.\nIf you proceed the basket WILL fall. ");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		if (msgBox.exec() == QMessageBox::Yes)
		{
				QString passStr = QInputDialog::getText(this, tr("Password is neaded to proceed."), tr("Please input the password"), QLineEdit::Password, "");
				if (passStr == "notSafe")
				{
						setPlcNetworkActive(PLC_NETWORK_CRANE, false);
						cranePlcTcpServer->sendString("N20|3|17");
						setPlcNetworkIdle(PLC_NETWORK_CRANE);
				}
		}
}
//void plcManualClass::command10PushButtonSlot(){cranePlcTcpServer->sendString(QString("N20|3|10"));}
//void plcManualClass::command11PushButtonSlot(){cranePlcTcpServer->sendString(QString("N20|3|11"));}
//void plcManualClass::command12PushButtonSlot(){cranePlcTcpServer->sendString(QString("N20|3|12"));}
//void plcManualClass::command13PushButtonSlot(){cranePlcTcpServer->sendString(QString("N20|3|13"));}
//void plcManualClass::command14PushButtonSlot(){cranePlcTcpServer->sendString(QString("N20|3|14"));}
//void plcManualClass::command15PushButtonSlot(){cranePlcTcpServer->sendString(QString("N20|3|15"));}
//void plcManualClass::command16PushButtonSlot(){cranePlcTcpServer->sendString(QString("N20|3|16"));}
//void plcManualClass::command17PushButtonSlot(){cranePlcTcpServer->sendString(QString("N20|3|17"));}
//void plcManualClass::command18PushButtonSlot(){cranePlcTcpServer->sendString(QString("N20|3|18"));}
//void plcManualClass::command19PushButtonSlot(){cranePlcTcpServer->sendString(QString("N20|3|19"));}
//																																									
//void plcManualClass::command20PushButtonSlot() {cranePlcTcpServer->sendString(QString("N20|3|20"));}
//void plcManualClass::command21PushButtonSlot() {cranePlcTcpServer->sendString(QString("N20|3|21"));}
//void plcManualClass::command22PushButtonSlot() {cranePlcTcpServer->sendString(QString("N20|3|22"));}
//void plcManualClass::recallStopPushButtonSlot(){cranePlcTcpServer->sendString(QString("N20|3|23"));}
//void plcManualClass::makeStepPushButtonSlot()  {cranePlcTcpServer->sendString(QString("N20|3|3"));}

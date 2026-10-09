#include "co.h"
commandsGroupClass::commandsGroupClass(tcpServerClass *tcpServer_,QWidget *p):QGroupBox(tr("Commands"),p)
{
	tcpServer=tcpServer_;
	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);
	//auto
	autoPushButton=new myQPushButton(tcpServer,"N11|0|26","N11|0|0",tr("Auto"),NULL);mainLayout->addWidget(autoPushButton);
	autoPushButton->setToolTip(tr("Sends the value 26 to PLC memory N11.0"));
	autoPushButton->setMinimumHeight(40);
	autoPushButton->setPalette(QPalette(Qt::lightGray));
	//manual
	manualPushButton=new myQPushButton(tcpServer,"N11|0|27","N11|0|0",tr("Manual"),NULL);mainLayout->addWidget(manualPushButton);
	manualPushButton->setToolTip(tr("Sends the value 27 to PLC memory N11.0"));
	manualPushButton->setMinimumHeight(40);
	manualPushButton->setPalette(QPalette(Qt::lightGray));
	//reset
	resetPushButton=new myQPushButton(tcpServer,"N11|0|28","N11|0|0",tr("Reset"),NULL);mainLayout->addWidget(resetPushButton);
	resetPushButton->setToolTip(tr("Sends the value 28 to PLC memory N11.0"));
	resetPushButton->setMinimumHeight(40);
	resetPushButton->setStyleSheet("QPushButton{color:#770000;}");
	resetPushButton->setPalette(QPalette(Qt::lightGray));
	//reset
	resetFultsPushButton=new myQPushButton(tcpServer,"N11|0|25","N11|0|0",tr("Reset faults"),NULL);mainLayout->addWidget(resetFultsPushButton);
	resetFultsPushButton->setToolTip(tr("Sends the value 25 to PLC memory N11.0"));
	resetFultsPushButton->setMinimumHeight(40);
	resetFultsPushButton->setStyleSheet("QPushButton{color:#770000;}");
	resetFultsPushButton->setPalette(QPalette(Qt::lightGray));

	myQPushButton *loadOvenPushButton=new myQPushButton(tcpServer,"","",tr("Load oven"),NULL);mainLayout->addWidget(loadOvenPushButton);
	loadOvenPushButton->setToolTip(tr("Sends the value 7 to PLC memory N11.0"));
	loadOvenPushButton->setMinimumHeight(40);
	connect(loadOvenPushButton,SIGNAL(clicked()),this,SLOT(loadOvenPushButtonSlot()));

	myQPushButton *unloadOvenPushButton=new myQPushButton(tcpServer,"","",tr("Unload oven"),NULL);mainLayout->addWidget(unloadOvenPushButton);
	unloadOvenPushButton->setToolTip(tr("Sends the value 7 to PLC memory N11.0"));
	unloadOvenPushButton->setMinimumHeight(40);
	connect(unloadOvenPushButton,SIGNAL(clicked()),this,SLOT(unloadOvenPushButtonSlot()));

	////////////////////////
	//Code added by Vahid
	////////////////////////
	myQPushButton *loadOven2PushButton = new myQPushButton(tcpServer, "", "", tr("Load oven 2"), NULL); mainLayout->addWidget(loadOven2PushButton);
	loadOven2PushButton->setToolTip(tr("Sends the value 64 to PLC memory N11.0"));
	loadOven2PushButton->setMinimumHeight(40);
	connect(loadOven2PushButton, SIGNAL(clicked()), this, SLOT(loadOven2PushButtonSlot()));

	myQPushButton *unloadOven2PushButton = new myQPushButton(tcpServer, "", "", tr("Unload oven 2"), NULL); mainLayout->addWidget(unloadOven2PushButton);
	unloadOven2PushButton->setToolTip(tr("Sends the value 65 to PLC memory N11.0"));
	unloadOven2PushButton->setMinimumHeight(40);
	connect(unloadOven2PushButton, SIGNAL(clicked()), this, SLOT(unloadOven2PushButtonSlot()));
	////////////////////////

	stopLabel=new QLabel(tr("Stop __active"));mainLayout->addWidget(stopLabel,0,Qt::AlignHCenter);
	stopLabel->setMinimumHeight(40);stopLabel->setMinimumWidth(120);
}
void commandsGroupClass::loadOvenPushButtonSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Quation"));
	msgBox.setText(tr("This will start the conveyors loading oven cicle."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N11|0|7"));
	tcpServer->sendString(QString("N11|0|0"));
}
void commandsGroupClass::unloadOvenPushButtonSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Quation"));
	msgBox.setText(tr("This will start the conveyors unloading oven cicle."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N11|0|8"));
	tcpServer->sendString(QString("N11|0|0"));
}

/////////////////////////
//Code added by Vahid
/////////////////////////
void commandsGroupClass::loadOven2PushButtonSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Question"));
	msgBox.setText(tr("This will start the conveyors loading oven 2 cycle."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec() == QMessageBox::No)
		return;
	tcpServer->sendString(QString("N11|0|64"));
	tcpServer->sendString(QString("N11|0|0"));
}
void commandsGroupClass::unloadOven2PushButtonSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Question"));
	msgBox.setText(tr("This will start the conveyors unloading oven 2 cycle."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec() == QMessageBox::No)
		return;
	tcpServer->sendString(QString("N11|0|65"));
	tcpServer->sendString(QString("N11|0|0"));
}
/////////////////////////

void commandsGroupClass::plcStatusChanged(int index,int value)
{
	if (index==23)
	{
		QVector<bool> bitsVector=deprecatedCalculateBits(value);
		if (bitsVector[0]==true)
		{
			autoPushButton->setStyleSheet("QPushButton{color:#0000aa;}");
			autoPushButton->setPalette(QPalette(Qt::green));
			manualPushButton->setStyleSheet("QPushButton{color:#000000;}");
			manualPushButton->setPalette(QPalette(Qt::lightGray));
		}
		else
		{
			autoPushButton->setStyleSheet("QPushButton{color:#000000;}");
			manualPushButton->setPalette(QPalette(Qt::lightGray));
			manualPushButton->setStyleSheet("QPushButton{color:#0000aa;}");
			manualPushButton->setPalette(QPalette(Qt::red));
		}
	}
}
void commandsGroupClass::activateStop(bool a)
{
	if (a)
	{
		stopLabel->setText(tr("        Stop active"));
		stopLabel->setStyleSheet("QLabel{background-color:red}");
	}
	else
	{
		stopLabel->setText(tr("     Stop inactive"));
		stopLabel->setStyleSheet("");
	}
}

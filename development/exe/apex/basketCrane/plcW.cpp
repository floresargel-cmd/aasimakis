#include "plcW.h"
plcWidgetClass::plcWidgetClass(QWidget *p):QWidget()
{
	p->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px}  QCheckBox{font-size:%1px}").arg(12));
	QVBoxLayout *mainLayout=new QVBoxLayout();
	showPlcManualsBtn=new QPushButton(tr("PLC"));	showPlcManualsBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);showPlcManualsBtn->setMinimumHeight(25);mainLayout->addWidget(showPlcManualsBtn);connect(showPlcManualsBtn,SIGNAL(clicked()),this,SLOT(onShowPlcManualsBtnSlot()));
	fromPosLabel=new QLabel("...");  fromPosLabel->setStyleSheet("QLabel{color:#000099;background-color:transparent;background:white;}");fromPosLabel->setMinimumHeight(30);mainLayout->addWidget(fromPosLabel);
	toPosLabel=new QLabel("...");		 toPosLabel->setStyleSheet("QLabel{color:#000099;background-color:transparent;background:white;}");toPosLabel->setMinimumHeight(30);mainLayout->addWidget(toPosLabel);
	stopLabel=new QLabel("...");		 stopLabel->setStyleSheet("QLabel{color:#ffffff;background-color:transparent;background:white;}");stopLabel->setMinimumHeight(30);mainLayout->addWidget(stopLabel); 
	status0Label=new QLabel("...");  status0Label->setStyleSheet("QLabel{color:#ffffff;background-color:transparent;background:white;}");status0Label->setMinimumHeight(30);mainLayout->addWidget(status0Label); 
	status1Label=new QLabel("...");  status1Label->setStyleSheet("QLabel{color:#ffffff;background-color:transparent;background:white;}");status1Label->setMinimumHeight(30);mainLayout->addWidget(status1Label); 
	status2Label = new QLabel("...");  status2Label->setStyleSheet("QLabel{color:#ffffff;background-color:transparent;background:white;}"); status2Label->setMinimumHeight(30); mainLayout->addWidget(status2Label);
	error0Label = new QLabel("...");   error0Label->setStyleSheet("QLabel{color:#ffffff;background-color:transparent;background:white;}"); error0Label->setMinimumHeight(30); mainLayout->addWidget(error0Label);
	error1Label=new QLabel("...");   error1Label->setStyleSheet("QLabel{color:#ffffff;background-color:transparent;background:white;}"); error1Label->setMinimumHeight(30); mainLayout->addWidget(error1Label);  
	makeResetLabel=new QLabel("...");   makeResetLabel->setStyleSheet("QLabel{color:#ffffff;background-color:transparent;background:white;}"); makeResetLabel->setMinimumHeight(25); mainLayout->addWidget(makeResetLabel);  
	mainLayout->addStretch(1);
	p->setLayout(mainLayout);
	////
	plcDialog=new plcDialogClass(this);
	cranePlcTcpServer=plcDialog->getCranePlcTcpServer();
	conveyorsPlcTcpServer=plcDialog->getConveyorsPlcTcpServer();
}
plcWidgetClass::~plcWidgetClass()
{
	delete plcDialog;
}
void plcWidgetClass::startExternalSoft()
{
	plcDialog->startExternalSoft();
}
tcpServerClass* plcWidgetClass::getCranePlcTcpServer()
{
	return cranePlcTcpServer;
}
tcpServerClass* plcWidgetClass::getConveyorsPlcTcpServer()
{
	return conveyorsPlcTcpServer;
}
#ifdef TUX_PLC_COMMS
tuxipClass *plcWidgetClass::getCrainTuxClass()
{
	return plcDialog->getCrainTuxClass();
}
tuxipClass *plcWidgetClass::getConveyorTuxClass()
{
	return plcDialog->getConveyorTuxClass();
}
tuxipClass *plcWidgetClass::getNewOvenTuxClass()
{
	return plcDialog->getNewOvenTuxClass();
}
void plcWidgetClass::showTuxServerCommsDialog()
{
	plcDialog->showTuxServerCommsDialog();
}
void plcWidgetClass::addTables()
{
	plcDialog->addTables();
}
#endif
void plcWidgetClass::onShowPlcManualsBtnSlot()
{
	passwordClass *pass=new passwordClass(this);
	pass->exec();
	if (pass->getPasswordIsCorrect())
		plcDialog->show();
	delete pass;
}
void plcWidgetClass::setFromPosLabelText(QString str)
{
	fromPosLabel->setText(str);
}
void plcWidgetClass::setToPosLabelText(QString str)
{
	toPosLabel->setText(str);
}
void plcWidgetClass::setStopLabelText(QString str,QString style)
{
	stopLabel->setText(str);
	stopLabel->setStyleSheet(style);
}
void plcWidgetClass::setStatus0LabelText(QString str,QString style)
{
	status0Label->setText(str);
	status0Label->setStyleSheet(style);
}
void plcWidgetClass::setStatus1LabelText(QString str,QString style)
{
	status1Label->setText(str);
	status1Label->setStyleSheet(style);
}
void plcWidgetClass::setStatus2LabelText(QString str, QString style)
{
	status2Label->setText(str);
	status2Label->setStyleSheet(style);
}
void plcWidgetClass::setError0LabelText(QString str, QString style)
{
	error0Label->setText(str);
	error0Label->setStyleSheet(style);
}
void plcWidgetClass::setError1LabelText(QString str,QString style)
{
	error1Label->setText(str);
	error1Label->setStyleSheet(style);
}
void plcWidgetClass::setMakeResetLabelText(QString str,QString style)
{
	makeResetLabel->setText(str);
	makeResetLabel->setStyleSheet(style);
}

void plcWidgetClass::signalFromCranePlc(QString table,int index,QString value)
{
	if (table=="N20")
		plcDialog->plcStatusChanged(index,value.toInt());
	if (table=="F21")
		plcDialog->plcFloatValuesChanged(index,value.toFloat());
	if (table=="N23")
		plcDialog->plcStatusChanged(index,value.toFloat());
}
void plcWidgetClass::signalFromConveyorsPlc(QString table,int index,QString value)
{
}

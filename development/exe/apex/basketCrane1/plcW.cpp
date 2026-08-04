#include "plcW.h"

static QString readablePlcLabelStyle(QString style)
{
	if (style.contains("background-color:white", Qt::CaseInsensitive))
		style.replace("background-color:white", "background-color:#f8fafc;color:#334155", Qt::CaseInsensitive);
	else if (style.contains("background-color:red", Qt::CaseInsensitive))
		style.replace("background-color:red", "background-color:#b91c1c;color:#ffffff", Qt::CaseInsensitive);

	style.replace("QLabel{", "QLabel{padding-left:6px;border-bottom:1px solid #64748b;");
	return style;
}

plcWidgetClass::plcWidgetClass(QWidget *p):QWidget()
{
	p->setObjectName("plcStatusPanel");
	p->setAttribute(Qt::WA_StyledBackground, true);
	p->setStyleSheet(QString(
		"QWidget#plcStatusPanel{background:#0b1220;border:1px solid #475569;} "
		"QWidget#plcStatusPanel QLabel{font-size:%1px;} "
		"QWidget#plcStatusPanel QPushButton{font-size:%1px;} "
		"QWidget#plcStatusPanel QCheckBox{font-size:%1px;}").arg(12));
	QVBoxLayout *mainLayout=new QVBoxLayout();
	mainLayout->setContentsMargins(1, 1, 1, 1);
	mainLayout->setSpacing(0);

	showPlcManualsBtn = new QPushButton(tr("PLC"));
	showPlcManualsBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	showPlcManualsBtn->setMinimumHeight(25); 
	mainLayout->addWidget(showPlcManualsBtn);
	connect(showPlcManualsBtn, SIGNAL(clicked()), this, SLOT(onShowPlcManualsBtnSlot()));

	fromPosLabel = new QLabel("...");
	fromPosLabel->setStyleSheet("QLabel{color:#000099;background:#ffffff;padding-left:6px;border-bottom:1px solid #64748b;}");
	fromPosLabel->setMinimumHeight(30); 
	mainLayout->addWidget(fromPosLabel);

	toPosLabel=new QLabel("...");		 
	toPosLabel->setStyleSheet("QLabel{color:#000099;background:#ffffff;padding-left:6px;border-bottom:1px solid #64748b;}");
	toPosLabel->setMinimumHeight(30);
	mainLayout->addWidget(toPosLabel);

	stopLabel=new QLabel("...");		 stopLabel->setStyleSheet("QLabel{color:#334155;background:#f8fafc;padding-left:6px;border-bottom:1px solid #64748b;}");stopLabel->setMinimumHeight(30);mainLayout->addWidget(stopLabel);
	status0Label=new QLabel("...");  status0Label->setStyleSheet("QLabel{color:#334155;background:#f8fafc;padding-left:6px;border-bottom:1px solid #64748b;}");status0Label->setMinimumHeight(30);mainLayout->addWidget(status0Label);
	status1Label=new QLabel("...");  status1Label->setStyleSheet("QLabel{color:#334155;background:#f8fafc;padding-left:6px;border-bottom:1px solid #64748b;}");status1Label->setMinimumHeight(30);mainLayout->addWidget(status1Label);
	
	status2Label = new QLabel("...");  
	status2Label->setStyleSheet("QLabel{color:#334155;background:#f8fafc;padding-left:6px;border-bottom:1px solid #64748b;}");
	status2Label->setMinimumHeight(30); 
	mainLayout->addWidget(status2Label);
	
	error0Label = new QLabel("...");   
	error0Label->setStyleSheet("QLabel{color:#334155;background:#f8fafc;padding-left:6px;border-bottom:1px solid #64748b;}");
	error0Label->setMinimumHeight(30); 
	mainLayout->addWidget(error0Label);

	error1Label=new QLabel("...");   
	error1Label->setStyleSheet("QLabel{color:#334155;background:#f8fafc;padding-left:6px;border-bottom:1px solid #64748b;}");
	error1Label->setMinimumHeight(30); 
	mainLayout->addWidget(error1Label);  
	
	makeResetLabel=new QLabel("...");   
	makeResetLabel->setStyleSheet("QLabel{color:#334155;background:#f8fafc;padding-left:6px;border-bottom:1px solid #64748b;}");
	makeResetLabel->setMinimumHeight(25); 
	mainLayout->addWidget(makeResetLabel);  

	mainLayout->addStretch(1);
	p->setLayout(mainLayout);

	// PLC dialog
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
tuxipClass *plcWidgetClass::getCraneTuxClass()
{
	return plcDialog->getCraneTuxClass();
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
	stopLabel->setStyleSheet(readablePlcLabelStyle(style));
}
void plcWidgetClass::setStatus0LabelText(QString str,QString style)
{
	status0Label->setText(str);
	status0Label->setStyleSheet(readablePlcLabelStyle(style));
}

void plcWidgetClass::setStatus1LabelText(QString str,QString style)
{
	status1Label->setText(str);
	status1Label->setStyleSheet(readablePlcLabelStyle(style));
}

void plcWidgetClass::setStatus2LabelText(QString str, QString style)
{
	status2Label->setText(str);
	status2Label->setStyleSheet(readablePlcLabelStyle(style));
}

void plcWidgetClass::setError0LabelText(QString str, QString style)
{
	error0Label->setText(str);
	error0Label->setStyleSheet(readablePlcLabelStyle(style));
}

void plcWidgetClass::setError1LabelText(QString str,QString style)
{
	error1Label->setText(str);
	error1Label->setStyleSheet(readablePlcLabelStyle(style));
}

void plcWidgetClass::setMakeResetLabelText(QString str,QString style)
{
	makeResetLabel->setText(str);
	makeResetLabel->setStyleSheet(readablePlcLabelStyle(style));
}

void plcWidgetClass::signalFromCranePlc(QString table, int index, QString value)
{
	if (table == "N20")
		plcDialog->plcStatusChanged(index, value.toInt());

	if (table == "F21")
		plcDialog->plcFloatValuesChanged(index, value.toFloat());

	if (table == "N23")
		plcDialog->plcStatusChanged(index, value.toFloat());
}

void plcWidgetClass::signalFromConveyorsPlc(QString table, int index, QString value)
{
}

#include "pch.h"
#include "newSbD.h"
newSbPosClass::newSbPosClass(tuxipClass *tuxip_,int plcId_):QWidget()
{
	tuxip=tuxip_;
	plcId=plcId_;
	globalPosId=craneId+plcId;
	QHBoxLayout *mainL=new QHBoxLayout();mainL->setMargin(2);QWidget::setLayout(mainL);
	mainL->addWidget(new QLabel(tr("Position %1:").arg(plcId)));
	mainL->addWidget(new QLabel("Sb:"));
	sbSpin=new QSpinBox();
	sbSpin->setMinimum(0);sbSpin->setMaximum(INT_MAX);
	mainL->addWidget(sbSpin);
	connect(sbSpin,SIGNAL(valueChanged(int)),this,SLOT(sbSpinBoxValueChangedSlot(int)));

	mainL->addWidget(new QLabel("Ticket:"));
	ticketSpin=new QSpinBox();
	ticketSpin->setMinimum(0);ticketSpin->setMaximum(INT_MAX);
	mainL->addWidget(ticketSpin);
	connect(ticketSpin,SIGNAL(valueChanged(int)),this,SLOT(ticketSpinBoxValueChangedSlot(int)));

	saveBtn=new QPushButton(QIcon(":/ok.png"),tr("Save"));mainL->addWidget(saveBtn);
	connect(saveBtn,SIGNAL(clicked()),this,SLOT(saveBtnClickedSlot()));

	mainL->addWidget(checkLabel=new QLabel("Ok"));checkLabel->setFixedWidth(500);
	checkLabel->setStyleSheet("QLabel{color:blue;}");
}
void newSbPosClass::checkGui()
{
	saveBtn->setEnabled(true);
	checkLabel->setText(tr("Ok"));
	checkLabel->setStyleSheet("QLabel{color:blue;}");
	//sb
	int posOfSb=getPosIdOfSbGlobal(sbSpin->value(),mDb);
	if ((sbSpin->value()!=0)&&(posOfSb!=0)&&(posOfSb!=globalPosId))
	{
		QString posOfSbDesc=getDescriptionOfPosIdGlobal(posOfSb,mDb);
		checkLabel->setText(tr("SB %1 is on position \"%2\" (id:%3)").arg(sbSpin->value()).arg(posOfSbDesc).arg(posOfSb));
		checkLabel->setStyleSheet("QLabel{color:red;}");
		saveBtn->setEnabled(false);
	}
	//ticket
	int posOfTicket=getPosIdOfTicketGlobal(ticketSpin->value(),mDb);
	if ((ticketSpin->value()!=0)&&(posOfTicket!=0)&&(posOfTicket!=globalPosId))
	{
		QString posOfTicketDesc=getDescriptionOfPosIdGlobal(posOfTicket,mDb);
		checkLabel->setText(tr("Ticket %1 is on position \"%2\" (id:%3)").arg(ticketSpin->value()).arg(posOfTicketDesc).arg(posOfTicket));
		checkLabel->setStyleSheet("QLabel{color:red;}");
		saveBtn->setEnabled(false);
	}
}
void newSbPosClass::sbSpinBoxValueChangedSlot(int value)
{
	if (sbSpin->value()==0)
		ticketSpin->setValue(0);
	checkGui();
}
void newSbPosClass::ticketSpinBoxValueChangedSlot(int value)
{
	checkGui();
}
void newSbPosClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==sbWarehouseIp)
	{
		if ( (table=="posSb")&&(index==plcId) )
			sbSpin->setValue(value.toInt());
		if ( (table=="posTicket")&&(index==plcId) )
			ticketSpin->setValue(value.toInt());
	}
}
void newSbPosClass::saveBtnClickedSlot()
{
	setCassetteSbTicketInPosG(globalPosId,0,sbSpin->value(),ticketSpin->value(),"user",false,false,true,true,mDb);
	tuxip->writeInteger("posSb",plcId,sbSpin->value());
	tuxip->writeInteger("posTicket",plcId,ticketSpin->value());
}
//////////////////////////////////////////////
newSbDlgClass::newSbDlgClass(tuxipClass *tuxip,QWidget *p):QDialog(p)
{
	setWindowIcon(QIcon(":/assimakis.png"));
	setWindowTitle(tr("New sb"));
	QVBoxLayout *mainL=new QVBoxLayout();mainL->setMargin(2);QDialog::setLayout(mainL);
	newSbPosV<<new newSbPosClass(tuxip,26);mainL->addWidget(newSbPosV.last());
	newSbPosV<<new newSbPosClass(tuxip,25);mainL->addWidget(newSbPosV.last());
	newSbPosV<<new newSbPosClass(tuxip,24);mainL->addWidget(newSbPosV.last());
	newSbPosV<<new newSbPosClass(tuxip,21);mainL->addWidget(newSbPosV.last());
	mainL->addWidget(new QLabel("Bottom to top:"));
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Take SB in"),"",tuxip,"pcCom",10,21,0,"","rConveyorState",21,4,"","","","","",buttonStyle,buttonStyleActive,tr("Take SB in"),this);
	mainL->addWidget(pushButtonsVector.last());
	mainL->addStretch(1);
}
newSbDlgClass::~newSbDlgClass()
{
}
void newSbDlgClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	for (int i=0;i<newSbPosV.count();i++)
		newSbPosV[i]->dataInPlcChanged(ip,table,index,value);
	for (int i=0;i<pushButtonsVector.count();i++)
		pushButtonsVector[i]->updateValue(table,index,value.toInt());
}

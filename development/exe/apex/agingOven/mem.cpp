#include "mem.h"
plcMemoryClass::plcMemoryClass(tcpServerClass *tcpServer_,int plcIndex_,int x,int y,QString title,QString toolTip,QWidget *p):QWidget(p)
{
	tcpServer=tcpServer_;
	plcIndex=plcIndex_;
	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);
	plcValue=0;

	QLabel *titleLabel=new QLabel(title);
	mainLayout->addWidget(titleLabel);
	titleLabel->setStyleSheet("QLabel{color:#ddddee;}");
	titleLabel->setCursor(Qt::PointingHandCursor);
	titleLabel->setAlignment(Qt::AlignBottom|Qt::AlignRight);

	valueLabel=new QLabel("99999");
	mainLayout->addWidget(valueLabel,1);
	valueLabel->setStyleSheet("QLabel{color:#ff0000;}");
	valueLabel->setCursor(Qt::PointingHandCursor);
	valueLabel->setAlignment(Qt::AlignLeft);
	valueLabel->setFont(QFont("arial",12));

	setToolTip("<b><span style='color:blue'>"+tr("Position :%1").arg(plcIndex)+tr("</b><br/><span style='color:black'>This is the number of the basket in PLC memory. By left clicking in this you can change this value.<br/><b>Caution!</b><br/> Change this value only if something went wrong and you are absolutely sure of what you are doing."));
	setGeometry(x,y,85,35);
}
bool plcMemoryClass::plcValueChanged(QString ip,int index,int value)
{
	if (tcpServer->getTuxipClass()->getIp()==ip)
	{
		if (plcIndex==index)
		{
			setPlcValue(value);
			return true;
		}
	}
	return false;
}
void plcMemoryClass::setPlcValue(int value)
{
	plcValue=value;
	if (value!=0)
		valueLabel->setText(QString().number(plcValue));
	else
		valueLabel->setText("...");
}
void plcMemoryClass::writeValueToPlc(int value)
{
#ifndef EMULATE
	tcpServer->sendString("N11"+QString("|%1|%2").arg(plcIndex).arg(value));
#else
	setPlcValue(value);
#endif
}
void plcMemoryClass::mouseReleaseEvent(QMouseEvent *ev)
{
	bool ok;
	int value=QInputDialog::getInt(this,tr("Change PLC memory"),tr("Define the number of the basket in PLC memory for %1 position.").arg(plcIndex),plcValue,0,100000,1,&ok);
	if (ok)
		writeValueToPlc(value);
}

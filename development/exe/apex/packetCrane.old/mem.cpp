#include "mem.h"
plcMemoryClass::plcMemoryClass(tcpServerClass *tcpServer_,int plcIndex_,int x,int y,QString title,QString toolTip,QWidget *p):QWidget(p)
{
	tcpServer=tcpServer_;
	plcIndex=plcIndex_;
	setGeometry(x,y,85,35);
	setCursor(Qt::PointingHandCursor);
	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);
	plcValue=0;
																																											 
	QLabel *titleLabel=new QLabel(title);																								 
	mainLayout->addWidget(titleLabel);																									 
	titleLabel->setStyleSheet("QLabel{color:#ddddee;}");																 
	titleLabel->setAlignment(Qt::AlignBottom|Qt::AlignRight);	
	titleLabel->setCursor(Qt::PointingHandCursor);
																																											 
	valueLabel=new QLabel("99999");																											 
	mainLayout->addWidget(valueLabel,1);																								 
	valueLabel->setStyleSheet("QLabel{color:#ff0000;}");																 
	valueLabel->setAlignment(Qt::AlignLeft);																						 
	valueLabel->setCursor(Qt::PointingHandCursor);

	setToolTip("<b><span style='color:blue'>"+tr("Position :%1").arg(plcIndex)+tr("</b><br/><span style='color:black'>This is the number of the basket in PLC memory. By left clicking in this you can change this value.<br/><b>Caution!</b><br/> Change this value only if something went wrong and you are absolutely sure of what you are doing."));
}
bool plcMemoryClass::plcValueChanged(int index,int value)
{
	if (plcIndex==index)
	{
		setPlcValue(value);
		return true;
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
	tcpServer->sendString("N150"+QString("|%1|%2").arg(plcIndex).arg(value));
}
void plcMemoryClass::mouseReleaseEvent(QMouseEvent *ev)
{
	bool ok;
	int value=QInputDialog::getInt(this,tr("Change PLC memory"),tr("Define the number of the basket in PLC memory for %1 position.").arg(plcIndex),plcValue,-780,780,1,&ok);
	if (ok)
		writeValueToPlc(value);
}

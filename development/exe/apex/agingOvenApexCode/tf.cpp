#include "tf.h"
thirtyFeetClass::thirtyFeetClass(tcpServerClass *tcpServer_,int truePlcCommand_,int falsePlcCommand_,int plcIndex_,int plcBit_,int x,int y,QWidget *p):QCheckBox(tr("30f"),p)
{
	tcpServer=tcpServer_;
	truePlcCommand=truePlcCommand_;
	falsePlcCommand=falsePlcCommand_;
	plcIndex=plcIndex_;
	plcBit=plcBit_;
	//
	setGeometry(x,y,40,20);
	connect(&clearCommandTimer, SIGNAL(timeout()), this, SLOT(onClearCommandTimerSlot()));
}
void thirtyFeetClass::nextCheckState()
{
	QCheckBox::nextCheckState();
	if (checkState()==Qt::Checked)
		tcpServer->sendString(QString("N11|0|%1").arg(truePlcCommand));
	else
		tcpServer->sendString(QString("N11|0|%1").arg(falsePlcCommand));
	clearCommandTimer.start(100);
}
void thirtyFeetClass::onClearCommandTimerSlot()
{
	tcpServer->sendString(QString("N11|0|0"));
	clearCommandTimer.stop();
}
void thirtyFeetClass::plcValueChanged(int index,int bit,bool value)
{
	if ( (index==plcIndex)&&(plcBit==bit) )
	{
		if (value==true)
			setCheckState(Qt::Checked);
		else
			setCheckState(Qt::Unchecked);
	}
}
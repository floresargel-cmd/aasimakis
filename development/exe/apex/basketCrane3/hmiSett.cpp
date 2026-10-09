#include "pch.h"
#include "hmiSett.h"
hmiSettingsClass::hmiSettingsClass(tuxipClass *tux,QWidget *p):QDialog(p)
{
	tuxip=tux;
	QDialog::setWindowTitle(tr("Settings"));
	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);
	QVBoxLayout *layout0=new QVBoxLayout();mainLayout->addLayout(layout0);

	QGroupBox *encoderGroupBox=new QGroupBox(tr("Encoders"));layout0->addWidget(encoderGroupBox);
	QGridLayout *encoderGroupBoxLayout=new QGridLayout();encoderGroupBox->setLayout(encoderGroupBoxLayout);

	int row=0;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Position[mm]")),0,1);
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Reset")),0,2);
	row++;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("X")),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",0,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset X"),":/reset.png",tuxip,"CR3_PCcomDint",22,103,0,"","CR3_PCcomDint",24,11,"","","","","",buttonStyle,buttonStyleActive,tr("Reset X"),this);
	encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
	row++;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Z")),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",12,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Z"),":/reset.png",tuxip,"CR3_PCcomDint",22,101,0,"","CR3_PCcomDint",24,12,"","","","","",buttonStyle,buttonStyleActive,tr("Reset Z"),this);
	encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
}
void hmiSettingsClass::reject()
{
	QDialog::reject();
}
void hmiSettingsClass::accept()
{
	QDialog::accept();
}
int hmiSettingsClass::exec()
{
	if (getPassword(tr("Password is needed to open this window.")),tr("Please give the password"),this)
		return QDialog::exec();
	return 0;
}
void hmiSettingsClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane3Ip)
	{
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<floatPlcMemoryVector.count();i++)
			floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toFloat());
	}
}

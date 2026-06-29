#include "pch.h"
#include "hmiSettingsPackingFork.h"
hmiSettingsPackingForkClass::hmiSettingsPackingForkClass(tuxipClass *tux,QWidget *p):QDialog(p)
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
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"fc1ToPcF",0,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset X"),":/reset.png",tuxip,"fc1ToPcI",22,101,0,"","fc1ToPcI",13,1,"","","","","",buttonStyle,buttonStyleActive,tr("Reset X"),this);
	encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
	row++;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Y")),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"fc1ToPcF",11,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Y"),":/reset.png",tuxip,"fc1ToPcI",22,103,0,"","fc1ToPcI",13,15,"","","","","",buttonStyle,buttonStyleActive,tr("Reset Y"),this);
	encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
	row++;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Z")),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"fc1ToPcF",6,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Z"),":/reset.png",tuxip,"fc1ToPcI",22,102,0,"","fc1ToPcI",13,0,"","","","","",buttonStyle,buttonStyleActive,tr("Reset Z"),this);
	encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
	row++;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("R")),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"fc1ToPcF",15,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset R"),":/reset.png",tuxip,"fc1ToPcI",22,104,0,"","fc1ToPcI",13,16,"","","","","",buttonStyle,buttonStyleActive,tr("Reset R"),this);
	encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
}
void hmiSettingsPackingForkClass::reject()
{
	QDialog::reject();
}
void hmiSettingsPackingForkClass::accept()
{
	QDialog::accept();
}
int hmiSettingsPackingForkClass::exec()
{
	if (getPassword(tr("Password is needed to open this window."),tr("Please give the password for settings"),passStrong,this))
		return QDialog::exec();
	return 0;
}
void hmiSettingsPackingForkClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane3Ip)
	{
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<floatPlcMemoryVector.count();i++)
			floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toFloat());
	}
}
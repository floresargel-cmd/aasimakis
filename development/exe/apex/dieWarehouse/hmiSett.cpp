#include "pch.h"
#include "hmiSett.h"
hmiSettingsClass::hmiSettingsClass(tuxipClass *tux,QWidget *p):QDialog(p)
{
	tuxip=tux;
	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);
	QVBoxLayout *layout0=new QVBoxLayout();mainLayout->addLayout(layout0);

	QGroupBox *encoderGroupBox=new QGroupBox(tr("Encoders"));layout0->addWidget(encoderGroupBox);
	QGridLayout *encoderGroupBoxLayout=new QGridLayout();encoderGroupBox->setLayout(encoderGroupBoxLayout);

	int row=0;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Position[mm]")),0,1);
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Reset")),0,2);
	row++;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("X")),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",0,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset X"),":/reset.png",tuxip,"bRobotToPcI",22,100,0,"","bRobotToPcI",-1,-1,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Reset X"),this);
	encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
	row++;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Z")),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",1,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Z"),":/reset.png",tuxip,"bRobotToPcI",22,101,0,"","bRobotToPcI",-1,-1,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Reset Z"),this);
	encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
	row++;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Hooks A")),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",4,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset hooks A"),":/reset.png",tuxip,"bRobotToPcI",22,102,0,"","bRobotToPcI",24,3,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Reset hooks A"),this);
	encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
	row++;	
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Hooks B")),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,dieIp,"bRobotToPcF",5,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset hooks B"),":/reset.png",tuxip,"bRobotToPcI",22,103,0,"","bRobotToPcI",24,3,"","","","","",BUTTON_STYLE,BUTTON_STYLE_ACTIVE,tr("Reset hooks B"),this);
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
	if (adminMode)
		return QDialog::exec();
	passwordClass *pass=new passwordClass(this);
	pass->exec();
	if (pass->getPasswordIsCorrect())
		return QDialog::exec();
	return 0;
}
void hmiSettingsClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==dieIp)
	{
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<floatPlcMemoryVector.count();i++)
			floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toFloat());
	}
}
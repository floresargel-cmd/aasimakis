#include "pch.h"
#include "hmiB.h"
hmiDlgBasicClass::hmiDlgBasicClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip_,bool useTranslation,QWidget *p):QDialog(p)
{
	setWindowTitle(tr("Basket crane basic HMI"));
	resize(800,600);
	tuxip=tuxip_;
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);
	setLayout(mainLayout);
	QScrollArea *mainScrollArea=new QScrollArea();mainScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	mainScrollArea->setWidgetResizable(true);
	QWidget *mainScrollAreaWidget=new QWidget();
	QVBoxLayout *mainScrollAreaLayout=new QVBoxLayout();mainScrollAreaLayout->setMargin(2);mainScrollAreaWidget->setLayout(mainScrollAreaLayout);
	mainScrollArea->setWidget(mainScrollAreaWidget);
	mainLayout->addWidget(mainScrollArea);

	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Alarms"),"",tuxip,"bRobotToPcI",22,33,0,"","bRobotToPcI",24,2,"","","","","",BUTTON_STYLE_ACTIVE,BUTTON_STYLE_RED,tr("No Alarms"),this);mainScrollAreaLayout->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auxiliary on"),":/ok.png",tuxip,"bRobotToPcI",22,104,0,"","bRobotToPcI",24,27,"","","","","",BUTTON_STYLE_RED,BUTTON_STYLE_ACTIVE,tr("Auxiliary off"),this);mainScrollAreaLayout->addWidget(pushButtonsVector.last());
	bitWarningsVector=new bitLogVectorClass("bRobotToPcI","plcWarnings.txt",bitClass::TYPE_SHOW_HIDE,STYLE_O,STYLE_GRAY,"","","","",useTranslation,bitLogHisoryClass::typeMsSql,this);mainScrollAreaLayout->addWidget(bitWarningsVector);
	bitErrorsVector=new bitLogVectorClass("bRobotToPcI","plcAlarms.txt",bitClass::TYPE_SHOW_HIDE,STYLE_R,STYLE_GRAY,"","","","",useTranslation,bitLogHisoryClass::typeMsSql,this);mainScrollAreaLayout->addWidget(bitErrorsVector);
}
hmiDlgBasicClass::~hmiDlgBasicClass()
{
}
int hmiDlgBasicClass::exec()
{
	return QDialog::exec();
}
void hmiDlgBasicClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==dieIp)
	{
		if("bRobotToPcI"==table)
		{
			if ((index==21)||(index==23)||(index==24))
			{
				for (int i=0;i<pushButtonsVector.count();i++)
					pushButtonsVector[i]->updateValue(table,index,value.toInt());
			}
			bitErrorsVector->updateValue(table,index,value.toInt());
			bitWarningsVector->updateValue(table,index,value.toInt());
		}
	}
}
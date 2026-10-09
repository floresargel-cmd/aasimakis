#include "pch.h"
#include "hmiB.h"
hmiDlgBasicClass::hmiDlgBasicClass(tuxipClass *sbTuxip_,QWidget *p):QDialog(p)
{
	setWindowTitle(tr("Cassette crane basic HMI"));
	resize(800,600);
	sbTuxip=sbTuxip_;
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);
	setLayout(mainLayout);
	QScrollArea *mainScrollArea=new QScrollArea();mainScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	mainScrollArea->setWidgetResizable(true);
	QWidget *mainScrollAreaWidget=new QWidget();
	QVBoxLayout *mainScrollAreaLayout=new QVBoxLayout();mainScrollAreaLayout->setMargin(2);mainScrollAreaWidget->setLayout(mainScrollAreaLayout);
	mainScrollArea->setWidget(mainScrollAreaWidget);
	mainLayout->addWidget(mainScrollArea);

	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset crane alarms "),"",sbTuxip,"bRobotToPcI",22,23,0,"","bRobotToPcI",24,2,"","","","","",buttonStyleActive,buttonStyleRed,tr("No crane alarms"),this);mainScrollAreaLayout->addWidget(pushButtonsVector.last());
	bitWarningsVector=new bitLogVectorClass("bRobotToPcI","sbWarnings.txt",bitClass::TYPE_SHOW_HIDE,styleLabelO,styleLabelGr,"","","","",false,bitLogHisoryClass::typeMySql,this);mainScrollAreaLayout->addWidget(bitWarningsVector);
	bitErrorsVector=new bitLogVectorClass("bRobotToPcI","sbAlarms.txt",bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,"","","","",false,bitLogHisoryClass::typeMySql,this);mainScrollAreaLayout->addWidget(bitErrorsVector);
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
	for (int i=0;i<pushButtonsVector.count();i++)
		pushButtonsVector[i]->updateValue(table,index,value.toInt());
	bitErrorsVector->updateValue(table,index,value.toInt());
	bitWarningsVector->updateValue(table,index,value.toInt());
}

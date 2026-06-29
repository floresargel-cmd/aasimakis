#include "pch.h"
#include "hmiSett.h"
extern 	allPossClass *globalAllPosPtr;
hmiSettingsClass::hmiSettingsClass(tuxipClass *tux,QWidget *p):QDialog(p)
{
	tuxip=tux;
	QDialog::setWindowTitle(tr("Settings dialog"));
	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);
	QVBoxLayout *layout0=new QVBoxLayout();mainLayout->addLayout(layout0);

	QGroupBox *encoderGroupBox=new QGroupBox(tr("Reset encoders"));layout0->addWidget(encoderGroupBox);
	QGridLayout *encoderGroupBoxLayout=new QGridLayout();encoderGroupBox->setLayout(encoderGroupBoxLayout);

	int row=0;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Position[mm]")),0,1);
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Reset")),0,2);
	row++;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Z")),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",6,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Z"),":/reset.png",tuxip,"bRobotToPcI",22,101,0,"","bRobotToPcI",27,10,"","","","","",buttonStyle,buttonStyleActive,tr("Reset Z"),this);
	encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
	row++;
	encoderGroupBoxLayout->addWidget(new QLabel(tr("Hooks")),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",18,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset hooks"),":/reset.png",tuxip,"bRobotToPcI",22,102,0,"","bRobotToPcI",27,11,"","","","","",buttonStyle,buttonStyleActive,tr("Reset hooks"),this);
	encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
	//row++;
	//encoderGroupBoxLayout->addWidget(new QLabel(tr("Drawer left")),row,0);
	//floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",4,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	//encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset drawer left"),":/reset.png",tuxip,"bRobotToPcI",22,102,0,"","bRobotToPcI",24,3,"","","","","",buttonStyle,buttonStyleActive,tr("Reset drawer left"),this);
	//encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);
	//row++;	
	//encoderGroupBoxLayout->addWidget(new QLabel(tr("Drawer right")),row,0);
	//floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcF",5,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);
	//encoderGroupBoxLayout->addWidget(floatPlcMemoryVector.last(),row,1);
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset drawer right"),":/reset.png",tuxip,"bRobotToPcI",22,103,0,"","bRobotToPcI",24,3,"","","","","",buttonStyle,buttonStyleActive,tr("Reset drawer right"),this);
	//encoderGroupBoxLayout->addWidget(pushButtonsVector.last(),row,2);

	//QGroupBox *cleanGroupBox=new QGroupBox(tr("Clean"));layout0->addWidget(cleanGroupBox);
	//QGridLayout *cleanGroupBoxL=new QGridLayout();cleanGroupBox->setLayout(cleanGroupBoxL);
	//row=0;
	//intPlcMemoryVector<<new intPlcMemoryClass(tuxip,sbWarehouseIp,"bRobotToPcI",9,1,500,tr("Missions:"),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,"",this);cleanGroupBoxL->addWidget(intPlcMemoryVector.last(),row,0);
	//pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Clean"),":/reset.png",tuxip,"bRobotToPcI",22,34,0,"","bRobotToPcI",23,20,"","","","","",buttonStyle,buttonStyleActive,tr("Clean"),this);
	//cleanGroupBoxL->addWidget(pushButtonsVector.last(),row,1);

	QGroupBox *correctionsGroupBox=new QGroupBox(tr("Corrections"));layout0->addWidget(correctionsGroupBox);
	QGridLayout *correctionsGroupBoxL=new QGridLayout();correctionsGroupBox->setLayout(correctionsGroupBoxL);
	row=0;
	//correctionsGroupBoxL->addWidget(correcitonsLabel=new QLabel(),row,0);
	QPushButton *applyCorrectionsBtn=new QPushButton(tr("Apply corrections"));
	connect(applyCorrectionsBtn,SIGNAL(clicked()),this,SLOT(applyCorrectionsBtnClickedSlot()));
	correctionsGroupBoxL->addWidget(applyCorrectionsBtn,row,1);
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
	if (getPassword(tr("Password is needed to open this window."),tr("Please give the password"),passWeak,this))
	{
		updateCorrections();
		return QDialog::exec();
	}
	return 0;
}
void hmiSettingsClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==sbWarehouseIp)
	{
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<floatPlcMemoryVector.count();i++)
			floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toFloat());
		for (int i=0;i<intPlcMemoryVector.count();i++)
			intPlcMemoryVector[i]->updateValue(ip,table,index,value.toInt());
	}
}
void hmiSettingsClass::updateCorrections()
{
	//QVector<QPointF> minusCellsPositions=globalAllPosPtr->getMinusCellsPositions();
	//QVector<QPointF> plusCellsPositions=globalAllPosPtr->getPlusCellsPositions();
	//int noOfDifferent=0;
	//QVector<float> leftX=tuxip->readFloatValues("LcellHz",1,-minCassetteId);
	//QVector<float> leftZ=tuxip->readFloatValues("LcellVt",1,-minCassetteId);
	//QVector<float> rightX=tuxip->readFloatValues("RcellHz",1,maxCassetteId);
	//QVector<float> rightZ=tuxip->readFloatValues("RcellVt",1,maxCassetteId);
	//for (int i=0;i<minusCellsPositions.count();i++)
	//{
	//	if (	(!ufEqual(leftX[i],minusCellsPositions[i].x(),0.1f))  ||  (!ufEqual(leftZ[i],minusCellsPositions[i].y(),0.1f))  )
	//		noOfDifferent++;
	//}
	//for (int i=0;i<plusCellsPositions.count();i++)
	//{
	//	if (	(!ufEqual(rightX[i],plusCellsPositions[i].x(),0.1f))  ||  (!ufEqual(rightZ[i],plusCellsPositions[i].y(),0.1f))  )
	//		noOfDifferent++;
	//}
	//correcitonsLabel->setText(tr("%1 changed").arg(noOfDifferent));
}
void hmiSettingsClass::applyCorrectionsBtnClickedSlot()
{
	QVector<float> LcellHz;
	QVector<float> LcellVt;
	QVector<float> RcellHz;
	QVector<float> RcellVt;
	QVector<QPointF> minusCellsPositions=globalAllPosPtr->getMinusCellsPositions();
	QVector<QPointF> plusCellsPositions=globalAllPosPtr->getPlusCellsPositions();
	for (int i=0;i<minusCellsPositions.count();i++)
	{
		RcellHz<<minusCellsPositions[i].x();
		RcellVt<<minusCellsPositions[i].y();
	}
	for (int i=0;i<plusCellsPositions.count();i++)
	{
		LcellHz<<plusCellsPositions[i].x();
		LcellVt<<plusCellsPositions[i].y();
	}
	tuxip->writeFloats("LcellHz",0,LcellHz);
	tuxip->writeFloats("LcellVt",0,LcellVt);
	tuxip->writeFloats("RcellHz",0,RcellHz);
	tuxip->writeFloats("RcellVt",0,RcellVt);
	updateCorrections();
}

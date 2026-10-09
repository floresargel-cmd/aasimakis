#include "pch.h"
#include "destW.h"
destackerWidgetClass::destackerWidgetClass(tuxipClass *tuxip,QWidget *p):QWidget(p)
{
	setWindowTitle(tr("Destacker HMI"));
	resize(600,900);
	QVBoxLayout *mainL=new QVBoxLayout();setLayout(mainL);mainL->setMargin(2);
	QGroupBox *pincersG=new QGroupBox(tr("Pincers"));mainL->addWidget(pincersG);
	QGridLayout *pincersGL=new QGridLayout();pincersGL->setMargin(2);pincersG->setLayout(pincersGL);
	for (int i=0;i<8;i++)
	{
		bitVector<<new bitClass("CR3_PCcomDint",51,i,bitClass::TYPE_CHANGE_COLOR,tr("Ok"),tr("Not ok"),styleLabelG,styleLabelGr,"","","","",this);
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Disable"),"",tuxip,"CR3_PCcomDint",50,i+1,0,"","CR3_PCcomDint",51,8+i,"","","","","",buttonStyleActive,buttonStyle,tr("Enable"),this);
		pincersGL->addWidget(new QLabel(tr("Pincer %1").arg(i+1)),0,7-i);
		pincersGL->addWidget(bitVector.last(),1,7-i);
		pincersGL->addWidget(pushButtonsVector.last(),2,7-i);
	}
	QGroupBox *modeG=new QGroupBox(tr("Mode"));mainL->addWidget(modeG);
	QGridLayout *modeL=new QGridLayout();modeL->setMargin(2);modeG->setLayout(modeL);
	int row=0;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Basket 1 to Station 1"),"",tuxip,"CR3_PCcomDint",50,11,0,"","CR3_PCcomDint",51,17,"","","","","",buttonStyleO,buttonStyleActive,tr("Basket 1 to Station 2"),this);modeL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Basket 2 to Station 2"),"",tuxip,"CR3_PCcomDint",50,12,0,"","CR3_PCcomDint",51,18,"","","","","",buttonStyleO,buttonStyleActive,tr("Basket 2 to Station 1"),this);modeL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Basket 1 Auto Start"),"",tuxip,"CR3_PCcomDint",50,13,0,"","CR3_PCcomDint",51,19,"","","","","",buttonStyle,buttonStyleActive,tr("Basket 1 Manual Start"),this);modeL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Basket 2 Auto Start"),"",tuxip,"CR3_PCcomDint",50,14,0,"","CR3_PCcomDint",51,20,"","","","","",buttonStyle,buttonStyleActive,tr("Basket 2 Manual Start"),this);modeL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Basket 1 On Hooks"),"",tuxip,"CR3_PCcomDint",50,15,0,"","CR3_PCcomDint",51,21,"","","","","",buttonStyle,buttonStyleActive,tr("Basket 1 Out Of Hooks"),this);modeL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Basket 2 On Hooks"),"",tuxip,"CR3_PCcomDint",50,16,0,"","CR3_PCcomDint",51,22,"","","","","",buttonStyle,buttonStyleActive,tr("Basket 2 Out Of Hooks"),this);modeL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Basket 1 Single Layer"),"",tuxip,"CR3_PCcomDint",50,17,0,"","CR3_PCcomDint",51,23,"","","","","",buttonStyleO,buttonStyleActive,tr("Basket 1 Double Layer"),this);modeL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Basket 2 Single Layer"),"",tuxip,"CR3_PCcomDint",50,18,0,"","CR3_PCcomDint",51,24,"","","","","",buttonStyleO,buttonStyleActive,tr("Basket 2 Double Layer"),this);modeL->addWidget(pushButtonsVector.last(),row,1);

	QGroupBox *positionG=new QGroupBox(tr("Position"));mainL->addWidget(positionG);
	QVBoxLayout *positionGL=new QVBoxLayout();positionGL->setMargin(2);positionG->setLayout(positionGL);
	QGridLayout *encodersL=new QGridLayout();encodersL->setMargin(2);positionGL->addLayout(encodersL);
	row=0;
	encodersL->addWidget(new QLabel("Position[mm]"),row,1);
	encodersL->addWidget(new QLabel("Target[mm]"),row,2);
	row++;
	encodersL->addWidget(new QLabel("Horizontal"),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",20,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);encodersL->addWidget(floatPlcMemoryVector.last(),row,1);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",21,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);encodersL->addWidget(floatPlcMemoryVector.last(),row,2);
	row++;
	encodersL->addWidget(new QLabel("Vertical"),row,0);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",22,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);encodersL->addWidget(floatPlcMemoryVector.last(),row,1);
	floatPlcMemoryVector<<new floatPlcMemoryClass(tuxip,crane3Ip,"CR3_PCcomReal",23,0,100000.,tr(""),"","","","",100,plcMemoryClass::TYPE_USER_CANNOT_CHANGE_VALUE,1,"",this);encodersL->addWidget(floatPlcMemoryVector.last(),row,2);
	row++;
	bitLogVectors<<new bitLogVectorClass("CR3_PCcomDint",QString("destackerPositions.txt"),bitClass::TYPE_SHOW_HIDE,styleLabelB,styleLabelGr,"","Position","","",true,bitLogHisoryClass::typeMsSql,this);positionGL->addWidget(bitLogVectors.last(),0);
	//
	bitLogVectors<<new bitLogVectorClass("CR3_PCcomDint",QString("destackerAlarms.txt"),bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,"","Errors","","",true,bitLogHisoryClass::typeMsSql,this);mainL->addWidget(bitLogVectors.last(),1);
}
destackerWidgetClass::~destackerWidgetClass()
{
}
void destackerWidgetClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane3Ip)
	{
		for (int i=0;i<bitVector.count();i++)
			bitVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<floatPlcMemoryVector.count();i++)
			floatPlcMemoryVector[i]->updateValue(ip,table,index,value.toInt());
		for (int i=0;i<bitLogVectors.count();i++)
			bitLogVectors[i]->updateValue(table,index,value.toInt());
	}
}

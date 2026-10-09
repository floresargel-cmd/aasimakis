#include "convDlg.h"
conveyorsManualClass::conveyorsManualClass(tcpServerClass *tcpServer_,QWidget *p):QDialog(p)
{
	//this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px}  QCheckBox{font-size:%1px}").arg(12));
	tcpServer=tcpServer_;

	QVBoxLayout *mainLayout=new QVBoxLayout();
  setLayout(mainLayout);

	QImage image(":/conveyorsLayout.png");
	imageLabel=new QLabel;
	imageLabel->setMinimumWidth(1600);
	imageLabel->setMinimumHeight(598);
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setAlignment(Qt::AlignCenter);

	mainLayout->addWidget(imageLabel);

	c25Break=new packetProximitySwitchClass(0,23,410,300,tr("Brake"),imageLabel);
	c24Break=new packetProximitySwitchClass(0,22,1130,300,tr("Brake"),imageLabel);

	conveyorPsLeftStop=new packetProximitySwitchClass(0,10,550,300,tr("Stop ps"),imageLabel);
  conveyorPsRightStop=new packetProximitySwitchClass(0,12,1050,300,tr("Stop ps"),imageLabel,-45,-10);
	conveyorPsLeftBrake=new packetProximitySwitchClass(0,11,665,300,tr("Brake ps"),imageLabel,-50,-10);
  conveyorPsRightBreak=new packetProximitySwitchClass(0,13,940,300,tr("Brake ps"),imageLabel);

	cranePsPositionLeft=new packetProximitySwitchClass(0,20,60,300,tr("Crane ps"),imageLabel);
	forkPsPositionLeft=new packetProximitySwitchClass(0,21,60,400,tr("Fork ps"),imageLabel);
	cranePsPositionRight=new packetProximitySwitchClass(0,18,1490,300,tr("Crane ps"),imageLabel);
	forkPsPositionRight=new packetProximitySwitchClass(0,19,1490,400,tr("Fork ps"),imageLabel);
  //
  casseteNumberOnRobotMem=new plcMemoryClass(tcpServer,10,800-25,400-20,"","",imageLabel);
	casseteNumberOnC25Mem=new plcMemoryClass(tcpServer,11,300-25,400-20,"","",imageLabel);
	casseteNumberOnC24Mem=new plcMemoryClass(tcpServer,12,1300-25,400-20,"","",imageLabel);

	conveyorPhotoLeftAnticollision=new packetProximitySwitchClass (1,31, 575,380,tr("Collision"),imageLabel,10,-6,"QLabel{color:#990000;}");
  conveyorPhotoRightAnticollision=new packetProximitySwitchClass(1,31,1050,380,tr("Collision"),imageLabel,10,-6,"QLabel{color:#990000;}");
	int W=60, H=30;
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|130","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveLeft.png")); buttonsVector.last()->setGeometry(1205,400,W,H);
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|131","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveRight.png"));   buttonsVector.last()->setGeometry(1345,400,W,H);
	//
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|132","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveLeft.png"));   buttonsVector.last()->setGeometry(205,400,W,H);
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|133","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveRight.png"));	buttonsVector.last()->setGeometry(345,400,W,H);
	//
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|136","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveLeft.png"));		buttonsVector.last()->setGeometry(520,350,W-30,H);
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|137","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveRight.png")); buttonsVector.last()->setGeometry(520,300, W-30,H);
	//
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|134","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveLeft.png")); buttonsVector.last()->setGeometry(1060,350,W-30,H);
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|135","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveRight.png"));buttonsVector.last()->setGeometry(1060,300,W-30,H);
	//
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|138","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveLeft.png"));  buttonsVector.last()->setGeometry(705,400,W,H);
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|139","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveRight.png")); buttonsVector.last()->setGeometry(845,400,W,H);
	//
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|140","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveUp.png"));   buttonsVector.last()->setGeometry(20,320,W,H);
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|141","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveDown.png"));	buttonsVector.last()->setGeometry(20,360,W,H);
	//
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|142","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveUp.png"));   buttonsVector.last()->setGeometry(1530,320,W,H);
	buttonsVector<<new myQPushButton(tcpServer,"N150|20|143","N150|20|0",tr(""),imageLabel); buttonsVector.last()->setIcon(QIcon(":/arrowMoveDown.png"));	buttonsVector.last()->setGeometry(1530,360,W,H);

	c24Cm=new conveyorMovmentClass(0,25,":/arrowLeft.png",0,26,":/arrowRight.png",1285,390,imageLabel);
	c25Cm=new conveyorMovmentClass(0,27,":/arrowLeft.png",0,28,":/arrowRight.png",285,390,imageLabel);
	rCm=  new conveyorMovmentClass(0,29,":/arrowLeft.png",0,30,":/arrowRight.png",780,390,imageLabel);
}
conveyorsManualClass::~conveyorsManualClass()
{
	delete imageLabel;
}
void conveyorsManualClass::packetCranePlcIntMemory150Changed(int index, int value)
{
	QVector<bool> bitsVector=deprecatedCalculateBits(value);
	if(index==10||index==11||index==12)
	{
		casseteNumberOnRobotMem->plcValueChanged(index, value);
		casseteNumberOnC25Mem->  plcValueChanged(index, value);
		casseteNumberOnC24Mem->  plcValueChanged(index, value);
	}
}
void conveyorsManualClass::packetCranePlcIntMemory152Changed(int index, int value)
{
	if ((index==0)||(index==1))
	{
		QVector<bool> bitsVector=deprecatedCalculateBits(value);
		c25Break->plcValueChanged(index,23,bitsVector[23]);
		c24Break->plcValueChanged(index,22,bitsVector[22]);
		conveyorPsLeftStop  ->plcValueChanged(index,10,bitsVector[10]);
		conveyorPsRightStop ->plcValueChanged(index,12,bitsVector[12]);
		conveyorPsLeftBrake ->plcValueChanged(index,11,bitsVector[11]);
		conveyorPsRightBreak->plcValueChanged(index,13,bitsVector[13]);
		cranePsPositionLeft ->plcValueChanged(index,20,bitsVector[20]);
		forkPsPositionLeft  ->plcValueChanged(index,21,bitsVector[21]);
		cranePsPositionRight->plcValueChanged(index,18,bitsVector[18]);
		forkPsPositionRight ->plcValueChanged(index,19,bitsVector[19]);
		conveyorPhotoLeftAnticollision->plcValueChanged(index,31,bitsVector[31]);
		conveyorPhotoRightAnticollision->plcValueChanged(index,31,bitsVector[31]);
		for (int i=0;i<bitsVector.count();i++)
		{
			c24Cm->plcValueChanged(index,i,bitsVector[i]);
			c25Cm->plcValueChanged(index,i,bitsVector[i]);
			rCm->plcValueChanged(index,i,bitsVector[i]);
		}
	}
}

void conveyorsManualClass::packetCranePlcFloatMemoryChanged(int index, double value)
{

}

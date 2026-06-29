#include "sp.h"
layerClass::layerClass(tcpServerClass *tcpServer_,int layer_,QWidget *p):QWidget(p)
{
	tcpServer=tcpServer_;
	layer=layer_;
	isOnComb=sp1=sp2=sp3=sp4=sp5=sp6=sp7=sp8=false;

	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);
	activeRadioButton=new QRadioButton(tr("Layer %1").arg(layer));mainLayout->addWidget(activeRadioButton);
	activeRadioButton->setChecked(false);
	connect(activeRadioButton,SIGNAL(clicked(bool)),this,SLOT(activeRadioButtonSlot(bool)));
	mainLayout->addWidget(sp1Label=new QLabel(""));
	mainLayout->addWidget(sp2Label=new QLabel(""));
	mainLayout->addWidget(sp3Label=new QLabel(""));
	mainLayout->addWidget(sp4Label=new QLabel(""));
	mainLayout->addWidget(sp5Label=new QLabel(""));
	mainLayout->addWidget(sp6Label=new QLabel(""));
	mainLayout->addWidget(sp7Label=new QLabel(""));
	mainLayout->addWidget(sp8Label=new QLabel(""));
}
void layerClass::setSpacersType(bool sp1_,bool sp2_,bool sp3_,bool sp4_,bool sp5_,bool sp6_,bool sp7_,bool sp8_,bool isOnComb_)
{
	isOnComb=isOnComb_;
	sp1=sp1_;
	sp2=sp2_;
	sp3=sp3_;
	sp4=sp4_;
	sp5=sp5_;
	sp6=sp6_;
	sp7=sp7_;
	sp8=sp8_;
	sp1Label->setPixmap(QPixmap::fromImage(QImage(sp1?(isOnComb?":/spacer.comb.png":":/spacer.normal.png"):":/spacer.no.png")));
	sp2Label->setPixmap(QPixmap::fromImage(QImage(sp2?(isOnComb?":/spacer.comb.png":":/spacer.normal.png"):":/spacer.no.png")));
	sp3Label->setPixmap(QPixmap::fromImage(QImage(sp3?(isOnComb?":/spacer.comb.png":":/spacer.normal.png"):":/spacer.no.png")));
	sp4Label->setPixmap(QPixmap::fromImage(QImage(sp4?(isOnComb?":/spacer.comb.png":":/spacer.normal.png"):":/spacer.no.png")));
	sp5Label->setPixmap(QPixmap::fromImage(QImage(sp5?(isOnComb?":/spacer.comb.png":":/spacer.normal.png"):":/spacer.no.png")));
	sp6Label->setPixmap(QPixmap::fromImage(QImage(sp6?(isOnComb?":/spacer.comb.png":":/spacer.normal.png"):":/spacer.no.png")));
	sp7Label->setPixmap(QPixmap::fromImage(QImage(sp7?(isOnComb?":/spacer.comb.png":":/spacer.normal.png"):":/spacer.no.png")));
	sp8Label->setPixmap(QPixmap::fromImage(QImage(sp8?(isOnComb?":/spacer.comb.png":":/spacer.normal.png"):":/spacer.no.png")));
}
void layerClass::setActive(bool active)
{
	activeRadioButton->setChecked(active);
}
bool layerClass::getIsOnComb()
{
	return isOnComb;
}
bool layerClass::getExistsSpacer(int index)
{
	if (index==1)return sp1;
	if (index==2)return sp2;
	if (index==3)return sp3;
	if (index==4)return sp4;
	if (index==5)return sp5;
	if (index==6)return sp6;
	if (index==7)return sp7;
	if (index==8)return sp8;
	return false;
}
bool layerClass::getActive()
{
	return activeRadioButton->isChecked();
}
void layerClass::activeRadioButtonSlot(bool v)
{
	emit activeRadioButtonClickedSignal(this,v);
}
void layerClass::sendWordToPlc()
{
	int word=0;
	word+=
			(getIsOnComb()  ?1.:0.0)*1	+
			(getExistsSpacer(1)?1.:0.0)*2	+
			(getExistsSpacer(2)?1.:0.0)*4	+
			(getExistsSpacer(3)?1.:0.0)*8	+
			(getExistsSpacer(4)?1.:0.0)*16 +
			(getExistsSpacer(5)?1.:0.0)*32 +
			(getExistsSpacer(6)?1.:0.0)*64 +
			(getExistsSpacer(7)?1.:0.0)*128+
			(getExistsSpacer(8)?1.:0.0)*256;
	tcpServer->sendString(QString("N163|%1|%2").arg(layer-1).arg(word));
}
void layerClass::sendZeroWordToPlc()
{
	tcpServer->sendString(QString("N163|%1|0").arg(layer-1));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
spacersClass::spacersClass(tcpServerClass *tcpServer_,QWidget *p):QWidget(p)
{
	//setMaximumHeight(300);
	tcpServer=tcpServer_;
	basket=0;
	plcWordsVector.resize(MAX_LAYERS_IN_A_BASKET);
	blockTcpSignals=false;
	//mainWidget->setMaximumHeight(300);
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	QHBoxLayout *buttonsLayout=new QHBoxLayout();mainLayout->addLayout(buttonsLayout);
	buttonsLayout->addWidget(sendDataPushButton=new QPushButton(tr("Send data")));
	connect(sendDataPushButton,SIGNAL(clicked()),this, SLOT(sendDataPushButtonSlot()));
	buttonsLayout->addWidget(new myQPushButton(tcpServer,"N100|30|229","N100|30|0",tr("Auto"),NULL));
	buttonsLayout->addWidget(statusLabel=new QLabel(tr("......................")));
	buttonsLayout->addWidget(new myQPushButton(tcpServer,"N100|30|230","N100|30|0",tr("Manual"),NULL));
	QScrollArea *scrollArea=new QScrollArea();mainLayout->addWidget(scrollArea);
	QWidget *scrollAreaWidget=new QWidget;scrollAreaWidget->setMinimumWidth(400);
	QVBoxLayout *scrollAreaWidgetLayout=new QVBoxLayout();scrollAreaWidget->setLayout(scrollAreaWidgetLayout);
	//spacers
	for (int i=0;i<MAX_LAYERS_IN_A_BASKET;i++)
	{
		layerClass *l=new layerClass(tcpServer,i+1,this);
		connect(l,SIGNAL(activeRadioButtonClickedSignal(layerClass*,bool)),this,SLOT(layerActiveRadioButtonClickedSlot(layerClass*,bool)));
		layersVector<<l;
	}
	for (int i=layersVector.count()-1;i>=0;i--)
		scrollAreaWidgetLayout->addWidget(layersVector[i]);
	//btns
	scrollAreaWidgetLayout->addStretch(1);
	scrollArea->setWidget(scrollAreaWidget);
}
void spacersClass::setBasket(int b)
{
	basket=b;
	loadFromDataBase();
}
void spacersClass::layerActiveRadioButtonClickedSlot(layerClass* layer,bool v)
{
	for (int i=0;i<layersVector.count();i++)
	{
		if (v)
		{
			if (layersVector[i]!=layer)
				layersVector[i]->setActive(false);
		}
		//else
		//	layersVector[i]->setActive(true);
	}
}
void spacersClass::loadFromDataBase()
{
	QVector<QVariant> layersDbVector=apexExecQuery(QString("select layer  from basketSpacers where (sp1=1 or sp2=1 or sp3=1 or sp4=1 or sp5=1 or sp6=1 or sp7=1 or sp8=1) and basket=%1 order by layer").arg(basket));
	QVector<QVariant> inCombDbVector=apexExecQuery(QString("select inComb from basketSpacers where (sp1=1 or sp2=1 or sp3=1 or sp4=1 or sp5=1 or sp6=1 or sp7=1 or sp8=1) and basket=%1 order by layer").arg(basket));
	QVector<QVariant> sp1DbVector=apexExecQuery(QString("select sp1 from basketSpacers where (sp1=1 or sp2=1 or sp3=1 or sp4=1 or sp5=1 or sp6=1 or sp7=1 or sp8=1) and basket=%1 order by layer").arg(basket));
	QVector<QVariant> sp2DbVector=apexExecQuery(QString("select sp2 from basketSpacers where (sp1=1 or sp2=1 or sp3=1 or sp4=1 or sp5=1 or sp6=1 or sp7=1 or sp8=1) and basket=%1 order by layer").arg(basket));
	QVector<QVariant> sp3DbVector=apexExecQuery(QString("select sp3 from basketSpacers where (sp1=1 or sp2=1 or sp3=1 or sp4=1 or sp5=1 or sp6=1 or sp7=1 or sp8=1) and basket=%1 order by layer").arg(basket));
	QVector<QVariant> sp4DbVector=apexExecQuery(QString("select sp4 from basketSpacers where (sp1=1 or sp2=1 or sp3=1 or sp4=1 or sp5=1 or sp6=1 or sp7=1 or sp8=1) and basket=%1 order by layer").arg(basket));
	QVector<QVariant> sp5DbVector=apexExecQuery(QString("select sp5 from basketSpacers where (sp1=1 or sp2=1 or sp3=1 or sp4=1 or sp5=1 or sp6=1 or sp7=1 or sp8=1) and basket=%1 order by layer").arg(basket));
	QVector<QVariant> sp6DbVector=apexExecQuery(QString("select sp6 from basketSpacers where (sp1=1 or sp2=1 or sp3=1 or sp4=1 or sp5=1 or sp6=1 or sp7=1 or sp8=1) and basket=%1 order by layer").arg(basket));
	QVector<QVariant> sp7DbVector=apexExecQuery(QString("select sp7 from basketSpacers where (sp1=1 or sp2=1 or sp3=1 or sp4=1 or sp5=1 or sp6=1 or sp7=1 or sp8=1) and basket=%1 order by layer").arg(basket));
	QVector<QVariant> sp8DbVector=apexExecQuery(QString("select sp8 from basketSpacers where (sp1=1 or sp2=1 or sp3=1 or sp4=1 or sp5=1 or sp6=1 or sp7=1 or sp8=1) and basket=%1 order by layer").arg(basket));
	for (int i=0;i<MAX_LAYERS_IN_A_BASKET;i++)
	{
		layersVector[i]->hide();
		layersVector[i]->setActive(false);
	}
	for (int i=0;i<layersDbVector.count();i++)
	{
		layersVector[i]->setSpacersType(sp1DbVector[i].toBool(),sp2DbVector[i].toBool(),sp3DbVector[i].toBool(),sp4DbVector[i].toBool(),sp5DbVector[i].toBool(),sp6DbVector[i].toBool(),sp7DbVector[i].toBool(),sp8DbVector[i].toBool(),inCombDbVector[i].toBool());
		layersVector[i]->show();
		layersVector[i]->setActive(false);
	}
	if (layersVector.count()>0)
		layersVector.last()->setActive(true);
	for (int i=0;i<layersVector.count();i++)
		layersVector[i]->sendWordToPlc();
}
void spacersClass::setAutoStatus(bool st)
{
	statusLabel->setText(st?tr("Auto mode   "):tr("Manual mode"));
}
void spacersClass::setOkToSendData(bool st)
{
	sendDataPushButton->setEnabled(st);
}
void spacersClass::spacerMemoryChanged(int index,int value)
{
	if (blockTcpSignals)
		return;
	if (index<MAX_LAYERS_IN_A_BASKET)
	{
		plcWordsVector[index]=value;
		for (int i=layersVector.count()-1;i>=0;i--)
		{
			if (plcWordsVector[i]!=0)
			{
				layersVector[i]->setActive(true);
				layerActiveRadioButtonClickedSlot(layersVector[i],true);
				break;
			}
		}
	}
}
void spacersClass::sendDataPushButtonSlot()
{
	blockTcpSignals=true;
	int index=0;
	for (index=0;index<layersVector.count();index++)
	{
		layersVector[index]->sendWordToPlc();
		if (layersVector[index]->getActive())
			break;
	}
	for (int i=index+1;i<layersVector.count();i++)
	{
		layersVector[i]->sendZeroWordToPlc();
	}
	blockTcpSignals=false;
}
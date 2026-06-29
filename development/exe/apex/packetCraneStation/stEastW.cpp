#include "pch.h"
#include "stEastW.h" 
extern QSplashScreen *splash;
stationEastWidgetClass::stationEastWidgetClass(tuxipClass	*tuxip_,QWidget *p):QWidget(p)
{
	splash->showMessage(tr("Creating Station Widget..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxip=tuxip_;
	station1Cassette=station2Cassette=0;
	QVBoxLayout *mainLayout=new QVBoxLayout();this->setLayout(mainLayout);
	QSplitter *mainSplitter=new QSplitter(Qt::Horizontal,this);mainLayout->addWidget(mainSplitter);
	QWidget *stationWidget1=new QWidget();QVBoxLayout *stationWidgetL1=new QVBoxLayout();stationWidget1->setLayout(stationWidgetL1);
	QWidget *stationWidget2=new QWidget();QVBoxLayout *stationWidgetL2=new QVBoxLayout();stationWidget2->setLayout(stationWidgetL2);
	mainSplitter->addWidget(stationWidget2);
	mainSplitter->addWidget(stationWidget1);

	//station 1	
	stationWidgetL1->addWidget(new QLabel(tr("Station 1")));
	stationCassetteLabel1=new QLabel("");stationWidgetL1->addWidget(stationCassetteLabel1);
	QHBoxLayout *return1L=new QHBoxLayout();stationWidgetL1->addLayout(return1L);
	return1L->addWidget(returnEmptyCassetteBtn1=new QPushButton(QIcon(":/arrowUp.png"),"Return EMPTY"));returnEmptyCassetteBtn1->setMinimumSize(QSize(128,128));returnEmptyCassetteBtn1->setStyleSheet("QPushButton{font-size:30px}");
	connect(returnEmptyCassetteBtn1,SIGNAL(clicked()),this,SLOT(returnEmptyCassetteBtn1ClickedSlot()));
	return1L->addStretch(1);
	return1L->addWidget(returnFullCassetteBtn1=new QPushButton(QIcon(":/arrowUp.png"),"Return FULL"));returnFullCassetteBtn1->setMinimumSize(QSize(128,128));returnFullCassetteBtn1->setStyleSheet("QPushButton{font-size:30px}");
	connect(returnFullCassetteBtn1,SIGNAL(clicked()),this,SLOT(returnFullCassetteBtn1ClickedSlot()));
	stationExportsList1=new exportsWidgetClass("exportsStation1",this);stationWidgetL1->addWidget(stationExportsList1);
	//station 2
	stationWidgetL2->addWidget(new QLabel(tr("Station 2")));
	stationCassetteLabel2=new QLabel("");stationWidgetL2->addWidget(stationCassetteLabel2);
	QHBoxLayout *return2L=new QHBoxLayout();stationWidgetL2->addLayout(return2L);
	return2L->addWidget(returnEmptyCassetteBtn2=new QPushButton(QIcon(":/arrowUp.png"),"Return EMPTY"));returnEmptyCassetteBtn2->setMinimumSize(QSize(128,128));returnEmptyCassetteBtn2->setStyleSheet("QPushButton{font-size:30px}");
	connect(returnEmptyCassetteBtn2,SIGNAL(clicked()),this,SLOT(returnEmptyCassetteBtn2ClickedSlot()));
	return2L->addStretch(1);
	return2L->addWidget(returnFullCassetteBtn2=new QPushButton(QIcon(":/arrowUp.png"),"Return FULL"));returnFullCassetteBtn2->setMinimumSize(QSize(128,128));returnFullCassetteBtn2->setStyleSheet("QPushButton{font-size:30px}");
	connect(returnFullCassetteBtn2,SIGNAL(clicked()),this,SLOT(returnFullCassetteBtn2ClickedSlot()));
	stationExportsList2=new exportsWidgetClass("exportsStation2",this);stationWidgetL2->addWidget(stationExportsList2);

	QTimer *updateGuiTimer=new QTimer(this);
	connect(updateGuiTimer,SIGNAL(timeout()),this,SLOT(updateGuiTimerSlot()));
	updateGuiTimer->start(10000);	
	updateGuiTimerSlot();
}
void stationEastWidgetClass::updateGuiTimerSlot()
{
	stationExportsList1->updateGui();
	stationExportsList2->updateGui();
}
void stationEastWidgetClass::returnEmptyCassetteBtn1ClickedSlot()
{
	execQuery(QString("update positions set packetId=%1 where posId=%2 and cassette<>0").arg(0).arg(stationBackExport1PlcId),MAIN_DB_CONNECTION);
	tuxip->writeInteger("pcCom",9,3540);
	tuxip->writeInteger("pcCom",9,0);
}
void stationEastWidgetClass::returnFullCassetteBtn1ClickedSlot()
{
	tuxip->writeInteger("pcCom",9,3540);
	tuxip->writeInteger("pcCom",9,0);
}
void stationEastWidgetClass::returnEmptyCassetteBtn2ClickedSlot()
{
	execQuery(QString("update positions set packetId=%1 where posId=%2 and cassette<>0").arg(0).arg(stationBackExport2PlcId),MAIN_DB_CONNECTION);
	tuxip->writeInteger("pcCom",9,3140);
	tuxip->writeInteger("pcCom",9,0);
}
void stationEastWidgetClass::returnFullCassetteBtn2ClickedSlot()
{
	tuxip->writeInteger("pcCom",9,3140);
	tuxip->writeInteger("pcCom",9,0);
}
void stationEastWidgetClass::updateGui()
{
	int packet1=getFirst(QString("select packetId from positions where cassette=%1").arg(station1Cassette),MAIN_DB_CONNECTION).toInt();
	int packet2=getFirst(QString("select packetId from positions where cassette=%1").arg(station2Cassette),MAIN_DB_CONNECTION).toInt();
	stationCassetteLabel1->setText(tr("Cassette %1(%2)").arg(station1Cassette).arg(packet1));
	stationCassetteLabel2->setText(tr("Cassette %1(%2)").arg(station2Cassette).arg(packet2));
}
void stationEastWidgetClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	for (int i=0;i<pushButtonsVector.count();i++)
		pushButtonsVector[i]->updateValue(table,index,value.toInt());
	if ((table=="ConveyorBasket")&&(index==35))
	{
		station1Cassette=value.toInt();
		updateGui();
	}
	if ((table=="ConveyorBasket")&&(index==31))
	{
		station2Cassette=value.toInt();
		updateGui();
	}
}
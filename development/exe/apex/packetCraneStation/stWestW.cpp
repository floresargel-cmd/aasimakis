#include "pch.h"
#include "stWestW.h"
extern QSplashScreen *splash;
stationWestWidgetClass::stationWestWidgetClass(tuxipClass	*tuxip_,QWidget *p):QWidget(p)
{
	splash->showMessage(tr("Creating Station Widget..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	tuxip=tuxip_;
	QVBoxLayout *mainL=new QVBoxLayout();this->setLayout(mainL);

	//station 1
	mainL->addWidget(stationCassetteLabel=new QLabel(""));
	QHBoxLayout *returnL=new QHBoxLayout();mainL->addLayout(returnL);
	returnL->addWidget(returnEmptyCassetteBtn=new QPushButton(QIcon(":/arrowUp.png"),"Return EMPTY"));returnEmptyCassetteBtn->setMinimumSize(QSize(128,128));returnEmptyCassetteBtn->setStyleSheet("QPushButton{font-size:30px}");
	connect(returnEmptyCassetteBtn,SIGNAL(clicked()),this,SLOT(returnEmptyCassetteBtnClickedSlot()));
	returnL->addWidget(returnFullCassetteBtn=new QPushButton(QIcon(":/arrowUp.png"),"Return FULL"));returnFullCassetteBtn->setMinimumSize(QSize(128,128));returnFullCassetteBtn->setStyleSheet("QPushButton{font-size:30px}");
	connect(returnFullCassetteBtn,SIGNAL(clicked()),this,SLOT(returnFullCassetteBtnnClickedSlot()));

	stationExportsList=new exportsWidgetClass("exportsPacking",this);mainL->addWidget(stationExportsList);

	QTimer *updateGuiTimer=new QTimer(this);
	connect(updateGuiTimer,SIGNAL(timeout()),this,SLOT(updateGuiTimerSlot()));
	updateGuiTimer->start(10000);
	updateGuiTimerSlot();
}
void stationWestWidgetClass::updateGuiTimerSlot()
{
	stationExportsList->updateGui();
}
void stationWestWidgetClass::returnEmptyCassetteBtnClickedSlot()
{
	tuxip->writeInteger("ConveyorData",packingBackExport2PlcId-POS_ID_CRANE,0);
	tuxip->writeInteger("pcCom",30,4142);
	tuxip->writeInteger("pcCom",30,0);
//	execQuery(QString("update positions set packetId=%1 where posId=%2 and cassette<>0").arg(0).arg(packingBackExport2PlcId),MAIN_DB_CONNECTION);
}
void stationWestWidgetClass::returnFullCassetteBtnnClickedSlot()
{
	tuxip->writeInteger("pcCom",30,41420);
	tuxip->writeInteger("pcCom",30,0);
}
void stationWestWidgetClass::updateGui()
{
	int packet=getFirst(QString("select packetId from positions where cassette=%1").arg(stationCassette),MAIN_DB_CONNECTION).toInt();
	stationCassetteLabel->setText(tr("Cassette %1(%2)").arg(stationCassette).arg(packet));
}
void stationWestWidgetClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	for (int i=0;i<pushButtonsVector.count();i++)
		pushButtonsVector[i]->updateValue(table,index,value.toInt());
	if ((table=="ConveyorBasket")&&(index==41))
	{
		stationCassette=value.toInt();
		updateGui();
	}
}

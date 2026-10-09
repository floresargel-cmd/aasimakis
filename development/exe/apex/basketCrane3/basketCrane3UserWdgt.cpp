#include "pch.h"
#include "basketCrane3UserWdgt.h"
void basketCrane3UserWdgtEnableBtn(QPushButton *btn,bool enable,bool active)
{
	btn->setStyleSheet(active?buttonStyleActive:buttonStyle);
	btn->setEnabled(enable);
}
basketCrane3UserWdgtClass::basketCrane3UserWdgtClass(convPositionClass *conveyor1Pos_,QWidget *p):QWidget(p)
{
	conveyor1Pos=conveyor1Pos_;
	QVBoxLayout *mainL=new QVBoxLayout();mainL->setMargin(2);QWidget::setLayout(mainL);
	mainL->addWidget(setDestinationNoneBtn			=new QPushButton(QIcon(":/return.png"),tr("None")));connect(setDestinationNoneBtn,SIGNAL(clicked()),this,SLOT(setDestinationNoneBtnClickedSlot()));
	mainL->addWidget(setDestinationCrane2Btn		=new QPushButton(QIcon(":/return.png"),tr("To Press")));connect(setDestinationCrane2Btn,SIGNAL(clicked()),this,SLOT(setDestinationCrane2BtnClickedSlot()));
	mainL->addWidget(setDestinationCrane3Btn		=new QPushButton(QIcon(":/return.png"),tr("To Packing Destacker")));connect(setDestinationCrane3Btn,SIGNAL(clicked()),this,SLOT(setDestinationCrane3BtnClickedSlot()));
	mainL->addWidget(setDestinationAnodizingBtn	=new QPushButton(QIcon(":/return.png"),tr("Out")));connect(setDestinationAnodizingBtn,SIGNAL(clicked()),this,SLOT(setDestinationAnodizingBtnClickedSlot()));
	mainL->addStretch(1);
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerTriggeredSlot()));
	mainTimer->start(1000);
}
void basketCrane3UserWdgtClass::updateGui()
{
	basketCrane3UserWdgtEnableBtn(setDestinationNoneBtn			,true,(conveyor1Pos->getBasketDestination()==destinationStruct::none));
	basketCrane3UserWdgtEnableBtn(setDestinationCrane2Btn		,conveyor1Pos->getHasBasket(),(conveyor1Pos->getBasketDestination()==destinationStruct::inFromOut));
	basketCrane3UserWdgtEnableBtn(setDestinationCrane3Btn		,conveyor1Pos->getHasBasket(),(conveyor1Pos->getBasketDestination()==destinationStruct::packingDestackerPos));
	basketCrane3UserWdgtEnableBtn(setDestinationAnodizingBtn,conveyor1Pos->getHasBasket(),(conveyor1Pos->getBasketDestination()==destinationStruct::anodizing));
}
void basketCrane3UserWdgtClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
}
void basketCrane3UserWdgtClass::mainTimerTriggeredSlot()
{
	updateGui();
}
void basketCrane3UserWdgtClass::setDestinationNoneBtnClickedSlot()
{
	conveyor1Pos->setBasketDestination(destinationStruct::none);
	updateGui();
}
void basketCrane3UserWdgtClass::setDestinationCrane2BtnClickedSlot()
{
	conveyor1Pos->setBasketDestination(destinationStruct::inFromOut);
	updateGui();
}
void basketCrane3UserWdgtClass::setDestinationCrane3BtnClickedSlot()
{
	conveyor1Pos->setBasketDestination(destinationStruct::packingDestackerPos);
	updateGui();
}
void basketCrane3UserWdgtClass::setDestinationAnodizingBtnClickedSlot()
{
	conveyor1Pos->setBasketDestination(destinationStruct::anodizing);
	updateGui();
}

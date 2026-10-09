#include "pch.h"
#include "basketCrane.h"
basketCraneClass::basketCraneClass(tuxipClass *tuxipConnection_,QWidget *p):QWidget(p)
{
	tuxipConnection=tuxipConnection_;
	QVBoxLayout *mainL=new QVBoxLayout();mainL->setMargin(2);QWidget::setLayout(mainL);
	//
	mainL->addWidget(new QLabel(tr("Basket number:")));
	basketSpinBox=new QSpinBox();mainL->addWidget(basketSpinBox);
	basketSpinBox->setMinimum(0);
	basketSpinBox->setMaximum(INT_MAX);
	basketSpinBox->setValue(0);
	connect(basketSpinBox,SIGNAL(valueChanged(int)),this,SLOT(basketSpinBoxChangedSlot(int)));
	mainL->addWidget(basketInfoLabel=new QLabel(tr("")));
	basketInfoLabel->setMinimumWidth(125);
	okBtn=new QPushButton(tr("Ok"),this);
	connect(okBtn,SIGNAL(clicked()),this,SLOT(okButtonClicledSlot()));
	mainL->addWidget(okBtn);

	//destinations
	mainL->addWidget(new QLabel(tr("Destination:")));
	QVBoxLayout *destL=new QVBoxLayout();destL->setMargin(2);mainL->addLayout(destL);
	QVector<destinationStruct::plcValueEnum> enumsV;enumsV<<destinationStruct::none<<destinationStruct::anodizing<<destinationStruct::inFromOut<<destinationStruct::packingDestackerPos;
	for (int i=0;i<enumsV.count();i++)
	{
		QPushButton *btn=new QPushButton(basketCrane2And3DestinationsG->getDestinationStructOfPlcValue(enumsV[i]).text,this);
		btn->setProperty("plcValueEnum",enumsV[i]);destL->addWidget(btn);connect(btn,SIGNAL(clicked()),this,SLOT(destinationButtonClickedSlot()));
		destinationBtnsV<<btn;
	}
	mainL->addStretch(1);
}
basketCraneClass::~basketCraneClass()
{
}
void basketCraneClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane2Ip)
	{
		if ((table=="bConveyorBasket")&&(index==1))
		{
			basketSpinBox->setValue(value.toInt());
		}
		if ((table=="bConveyorData")&&(index==1))
		{
			for (int i=0;i<destinationBtnsV.count();i++)
				destinationBtnsV[i]->setStyleSheet(destinationBtnsV[i]->property("plcValueEnum").toInt()==value.toInt()?buttonStyleActive:buttonStyle);
		}
	}
}
void basketCraneClass::updateDestinationBtns(int destination)
{

}
QString	getDescriptionOfPosWithBasket(int b,QVector<int> ignorePos,QVector<int> ignoreIndex)
{
	if (b!=0)
	{
		QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select description,posNumber,posIndex from positions where basketTableId=%1").arg(b),bDb);
		if (dataVV.count()==1)
		{
			for (int i=0;i<ignorePos.count();i++)
			{
				if ( (ignorePos[i]==dataVV[0][1].toInt())&&(ignoreIndex[i]==dataVV[0][2].toInt()) )
					return QString();
			}
			return QString("%1 (%2.%3)").arg(dataVV[0][0].toString().trimmed()).arg(dataVV[0][1].toInt()).arg(dataVV[0][2].toInt()).trimmed();
		}
	}
	return QString();
}
#define exit2PlcId 1230
void basketCraneClass::basketSpinBoxChangedSlot(int b)
{
	QString desc=getDescriptionOfPosWithBasket(b,QVector<int>()<<exit2PlcId,QVector<int>()<<1);
	if ((b!=0)&&(!desc.isEmpty()))
	{
		basketInfoLabel->setText(tr("Basket %1 is on position %2)").arg(b).arg(desc));
		basketInfoLabel->setStyleSheet("QLabel{color:red;}");
		okBtn->setEnabled(false);
	}
	else
	{
		basketInfoLabel->setText(tr(""));
		basketInfoLabel->setStyleSheet("QLabel{color:blue;}");
		okBtn->setEnabled(true);
	}
}
void basketCraneClass::destinationButtonClickedSlot()
{
	QPushButton *btn=qobject_cast<QPushButton*>(sender());
	if (btn)
		tuxipConnection->writeInteger("bConveyorData",1,btn->property("plcValueEnum").toInt());
}
void basketCraneClass::okButtonClicledSlot()
{
	tuxipConnection->writeInteger("bConveyorBasket",1,basketSpinBox->value());
}

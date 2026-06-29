#include "pch.h"
#include "ps.h"
packetProximitySwitchClass::packetProximitySwitchClass(int plcIndex_,int plcBit_,int x,int y,QString title,QWidget *p,int dx,int dy,QString styleOn_):QWidget(p)
{
	plcIndex=plcIndex_;
	plcBit=plcBit_;
	imageLabel=new QLabel(p);
	QImage image(":/proximitySwitchOff.png");
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setGeometry(x,y,8,8);

	titleLabel=new QLabel(title,p);
	titleLabel->setStyleSheet("QLabel{color:#999999;}");
	titleLabel->setGeometry(x+dx,y+dy,50,20);
	styleOn=styleOn_;

	//setToolTip(tr("%1 proximity switch").arg(plcIndex)+tr("this is the proximity switch coresponding to N11.%1.%2 bit").arg(plcIndex).arg(plcBit));
	setGeometry(x,y,55,20);
}
void packetProximitySwitchClass::plcValueChanged(int index,int bit,bool value)
{
	if ((plcIndex==index)&&(plcBit==bit))
	{
		if (value==true)
		{
			QImage image(":/proximitySwitchOn.png");
			titleLabel->setStyleSheet(styleOn);
			imageLabel->setPixmap(QPixmap::fromImage(image));
		}
		else
		{
			QImage image(":/proximitySwitchOff.png");
			titleLabel->setStyleSheet("QLabel{color:#999999;}");
			imageLabel->setPixmap(QPixmap::fromImage(image));
		}
	}
}

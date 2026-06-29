#include "ph.h"
photoSensorClass::photoSensorClass(tcpServerClass *tcpServer_,int plcIndex_,int plcBit_,int typeEnum_,int x_,int y_,QString title,QWidget *p):QWidget(p)
{
	tcpServer=tcpServer_;
	plcIndex=plcIndex_;
	plcBit=plcBit_;
	typeEnum=typeEnum_;
	x=x_;
	y=y_;
	imageLabel=new QLabel(p);
	titleLabel=new QLabel(title,p);
	titleLabel->setStyleSheet("QLabel{color:#14f75a;}");
	titleLabel->setGeometry(x+6,y-6,50,20);
	setToolTip(tr("%1 photosensor").arg(plcIndex)+tr("this is the photosensor coresponding to N11.%1.%2 bit").arg(plcIndex).arg(plcBit));
	if (typeEnum==TYPE_SINGLE)
		setGeometry(x,y,20,72);
	else
		setGeometry(x,y,20,140);
	plcValueChanged(plcIndex,plcBit,true);
}
void photoSensorClass::plcValueChanged(int index,int bit,bool value)
{
	if ((plcIndex==index)&&(plcBit==bit))
	{
		if (value==true)
		{
			if (typeEnum==TYPE_SINGLE)
			{
				QImage image(":/photoSensorSingleActive.png");
				imageLabel->setPixmap(QPixmap::fromImage(image));
				imageLabel->setGeometry(x,y,4,72);
			}
			else
			{
				QImage image(":/photoSensorDoubleActive.png");
				imageLabel->setPixmap(QPixmap::fromImage(image));
				imageLabel->setGeometry(x,y,4,140);
			}
			titleLabel->setStyleSheet("QLabel{color:#14f75a;}");
		}
		else
		{
			QImage image(":/photoSensorInactive.png");
			imageLabel->setPixmap(QPixmap::fromImage(image));
			imageLabel->setGeometry(x,y,4,9);
			titleLabel->setStyleSheet("QLabel{color:#14f75a;}");
		}
	}
}

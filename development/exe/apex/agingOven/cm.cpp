#include "cm.h"
conveyorMovmentClass::conveyorMovmentClass(tcpServerClass *tcpServer_,int plcIndex1_,int plcBit1_,QString image1Str,int plcIndex2_,int plcBit2_,QString image2Str,int x,int y,QWidget *p):QLabel(p)
{
	tcpServer=tcpServer_;
	plcIndex1=plcIndex1_;
	plcBit1=plcBit1_;

	plcIndex2=plcIndex2_;
	plcBit2=plcBit2_;

	image1=new QImage(image1Str);
	image2=new QImage(image2Str);
	setGeometry(x,y,50,50);
	firstBitUpdated=secondBitUpdated=false;
	bit1Value=bit2Value=false;
}
void conveyorMovmentClass::updateImages()
{
	if (bit1Value)
			setPixmap(QPixmap::fromImage(*image1));
	else if (bit2Value)
			setPixmap(QPixmap::fromImage(*image2));
	else
		setPixmap(QPixmap(0,0));
}
void conveyorMovmentClass::plcValueChanged(int index,int bit,bool value)
{
	if ((plcIndex1==index)&&(plcBit1==bit))
	{
		bit1Value=value;
		firstBitUpdated=true;
	}
	if ((plcIndex2==index)&&(plcBit2==bit))
	{
		bit2Value=value;
		secondBitUpdated=true;
	}
	if (firstBitUpdated&&secondBitUpdated)
	{
		firstBitUpdated=secondBitUpdated=false;
		updateImages();
	}
}

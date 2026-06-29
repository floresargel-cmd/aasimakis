#include "movement.h"
conveyorMovementClass::conveyorMovementClass(int plcIndex1_,int plcBit1_,QString image1Str,int plcIndex2_,int plcBit2_,QString image2Str,int x,int y,QWidget *p):QLabel(p)
{
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
void conveyorMovementClass::updateImages()
{
	if (bit1Value)
			setPixmap(QPixmap::fromImage(*image1));
	else if (bit2Value)
			setPixmap(QPixmap::fromImage(*image2));
	else
		setPixmap(QPixmap());
}
void conveyorMovementClass::plcValueChanged(int plcIndex,int plcValue)
{
	if (plcIndex1==plcIndex)
	{
		bit1Value=HAVECOMMONBIT(plcValue,BITS[plcBit1]);
		firstBitUpdated=true;
	}
	if (plcIndex1==plcIndex)
	{
		bit2Value=HAVECOMMONBIT(plcValue,BITS[plcBit2]);
		secondBitUpdated=true;
	}
	if (firstBitUpdated&&secondBitUpdated)
	{
		firstBitUpdated=secondBitUpdated=false;
		updateImages();
	}
}

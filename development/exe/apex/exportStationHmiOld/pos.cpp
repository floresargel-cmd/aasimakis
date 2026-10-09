#include "pos.h"
positionClass::positionClass(int plcPositionId_,int noSBplcIndex_,int fullSBplcIndex_,int emptySBplcIndex_,int noSBplcBitIndex_,int fullSBplcBitIndex_,int emptySBplcBitIndex_,int noSBPlcCommand_,int fullSBPlcCommand_,int emptySBPlcCommand_):QWidget()
{
	//
	plcPositionId=plcPositionId_;

	noSBplcIndex=noSBplcIndex_;
	fullSBplcIndex=fullSBplcIndex_;
	emptySBplcIndex=emptySBplcIndex_;

	noSBplcBitIndex=noSBplcBitIndex_;
	fullSBplcBitIndex=fullSBplcBitIndex_;
	emptySBplcBitIndex=emptySBplcBitIndex_;

	noSBPlcCommand=noSBPlcCommand_;
	fullSBPlcCommand=fullSBPlcCommand_;
	emptySBPlcCommand=emptySBPlcCommand_;
	//
	QHBoxLayout *mainLayout=new QHBoxLayout();
	mainLayout->setContentsMargins(0,0,0,0);
	srtLabel=new QLabel(QString().number(plcPositionId));mainLayout->addWidget(srtLabel);srtLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
	imgLabel=new QLabel();
	imgLabel->setMinimumWidth(50);
	imgLabel->setMinimumHeight(41);
	mainLayout->addWidget(imgLabel);
	setLayout(mainLayout);
	statusEnum=STATUS_NO_SB;
}
int positionClass::getStatusEnum()
{
	return statusEnum;
}
int positionClass::getPositionId()
{
	return plcPositionId;
}
void positionClass::setPlcNoFullEmptyStatusBitToTrue(int plcIndex,int plcBitIndex)
{
	if ((noSBplcIndex==plcIndex)&&(noSBplcBitIndex==plcBitIndex))
	{
		statusEnum=STATUS_NO_SB;
		imgLabel->setPixmap(QPixmap(":/noSb.png"));
	}
	else if ((fullSBplcIndex==plcIndex)&&(fullSBplcBitIndex==plcBitIndex))
	{
		statusEnum=STATUS_FULL_SB;
		imgLabel->setPixmap(QPixmap(":/fullSb.png"));
	}
	else if ((emptySBplcIndex==plcIndex)&&(emptySBplcBitIndex==plcBitIndex))
	{
		statusEnum=STATUS_EMPTY_SB;
		imgLabel->setPixmap(QPixmap(":/emptySb.png"));
	}
}
bool positionClass::getHasSB()
{
	if (statusEnum==STATUS_NO_SB)
		return false;
	return true;
}

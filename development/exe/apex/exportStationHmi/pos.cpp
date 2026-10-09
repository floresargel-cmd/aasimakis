#include "pos.h"
positionClass::positionClass(int plcPositionId_,int noSBplcIndex_,int fullSBplcIndex_,int emptySBplcIndex_,int noSBplcBitIndex_,int fullSBplcBitIndex_,int emptySBplcBitIndex_,int noSBPlcCommand_,int fullSBPlcCommand_,int emptySBPlcCommand_,QWidget *p):QLabel(p)
{
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

	QImage noSBImage(":/noSb.png");
	QPainter noSBPainter(&noSBImage);
	noSBPainter.setPen(Qt::blue);noSBPainter.setFont(QFont("Arial",10));
	noSBPainter.drawText(noSBImage.rect(),Qt::AlignCenter,QString("%1").arg(plcPositionId));
	noSBPixMap=QPixmap::fromImage(noSBImage);

	QImage fullSBImage(":/fullSb.png");
	QPainter fullSBPainter(&fullSBImage);
	fullSBPainter.setPen(Qt::blue);fullSBPainter.setFont(QFont("Arial",10));
	fullSBPainter.drawText(fullSBImage.rect(),Qt::AlignCenter,QString("%1").arg(plcPositionId));
	fullSBPixMap=QPixmap::fromImage(fullSBImage);

	QImage emptySBImage(":/emptySb.png");
	QPainter emptySBPainter(&emptySBImage);
	emptySBPainter.setPen(Qt::blue);emptySBPainter.setFont(QFont("Arial",10));
	emptySBPainter.drawText(emptySBImage.rect(),Qt::AlignCenter,QString("%1").arg(plcPositionId));
	emptySBPixMap=QPixmap::fromImage(emptySBImage);
	//
	//QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);setLayout(mainLayout);
	////srtLabel=new QLabel(QString().number(plcPositionId));mainLayout->addWidget(srtLabel);srtLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
	//imgLabel=new QLabel();
	//imgLabel->setPixmap(noSBPixMap);mainLayout->addWidget(imgLabel);
	//mainLayout->addWidget(new QLabel(QString("%1").arg(plcPositionId)));
	////QLabel::setMinimumWidth(50);
	//QLabel::setMinimumHeight(41);
	//
	QLabel::setMinimumSize(30,25);
	//
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
		QLabel::setPixmap(noSBPixMap);
	}
	else if ((fullSBplcIndex==plcIndex)&&(fullSBplcBitIndex==plcBitIndex))
	{
		statusEnum=STATUS_FULL_SB;
		QLabel::setPixmap(fullSBPixMap);
	}
	else if ((emptySBplcIndex==plcIndex)&&(emptySBplcBitIndex==plcBitIndex))
	{
		statusEnum=STATUS_EMPTY_SB;
		QLabel::setPixmap(emptySBPixMap);
	}
}
bool positionClass::getHasSB()
{
	if (statusEnum==STATUS_NO_SB)
		return false;
	return true;
}

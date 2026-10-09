#include "pch.h"
#include "manCellLabel.h"
cellLabelClass::cellLabelClass(int cellNumber_,int row_,int col_,craneClass *crane_):QWidget()
{
	//QWidget::hide();
	crane=crane_;
	setMinimumWidth(35);
	cellNumber=cellNumber_;
	row=row_;
	col=col_;
	setCursor(Qt::PointingHandCursor);
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	packetLabel=new QLabel(QString("%1").arg(cellNumber));
	mainLayout->addWidget(packetLabel,Qt::AlignCenter);
}
cellLabelClass::~cellLabelClass()
{
}
void cellLabelClass::mouseReleaseEvent(QMouseEvent *ev)
{
	cellContClass *cellCont=new cellContClass(cellNumber,crane,this);
	cellCont->exec();
	delete cellCont;
}
void cellLabelClass::updateGuiFromData(int packetId,bool isOnSb)
{
	////////packetId=GETONE(QString("select packetId from positions where posId=%1").arg(cellNumber)).toInt();
	////////isOnSb=false;
	//////////setTextFormat(Qt::RichText);
	////////if (packetId!=0)
	////////	isOnSb=GETONE(QString("select sb from packets where packetId=%1").arg(packetId)).toInt()==PACKET_WITHOUT_SB?false:true;
	if (packetId!=0)
		packetLabel->setText(QString("%1").arg(packetId));
	else
		packetLabel->setText(QString("%1").arg(""));
	if (isOnSb)
		packetLabel->setStyleSheet("QWidget:{color:blue;}");
	if ((col%2)==0)
			setStyleSheet("QWidget{background-color:#bbbbdd;}");
	if ((row%2)==0)
		setStyleSheet("QWidget{background-color:#bbddbb;}");
	if ( ((col%2)==0)&&((row%2)==0) )
		setStyleSheet("QWidget{background-color:#bbdddd;}");

}

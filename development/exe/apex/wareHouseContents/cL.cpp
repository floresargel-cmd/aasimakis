#include "cL.h"
cellLabelClass::cellLabelClass(int cellNumber_,int row,int col):QWidget()
{
	setMinimumWidth(35);
	cellNumber=cellNumber_;
	setCursor(Qt::PointingHandCursor);
	packetId=GETONE(QString("select packetId from positions where posId=%1").arg(cellNumber)).toInt();
	isOnSb=false;
	//setTextFormat(Qt::RichText);
	if (packetId!=0)
		isOnSb=GETONE(QString("select sb from packets where packetId=%1").arg(packetId)).toInt()==PACKET_WITHOUT_SB?false:true;
	//if (isOnSb)
	//	setText(QString("<font color=gray>%1</font>\n<font color=blue>%2</font>").arg(cellNumber).arg(packetId));
	//else
	//	setText(QString("<font color=gray>%1</font>\n<font color=black>%2</font>").arg(cellNumber).arg(packetId));
	////
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	//cell
	//QLabel *cellLabel=new QLabel(QString("%1").arg(cellNumber));
	//cellLabel->setStyleSheet("QLabel:{color:gray;}");
	//cellLabel->setPalette(QPalette(Qt::gray));
	//mainLayout->addWidget(cellLabel);
	//packet
	QLabel *packetLabel;
	if (packetId!=0)
		packetLabel=new QLabel(QString("%1").arg(packetId));
	else
		packetLabel=new QLabel(QString("%1").arg(""));
	if (isOnSb)
		packetLabel->setStyleSheet("QWidget:{color:blue;}");
	if ((col%2)==0)
			setStyleSheet("QWidget{background-color:#bbbbdd;}");
	if ((row%2)==0)
		setStyleSheet("QWidget{background-color:#bbddbb;}");
	if ( ((col%2)==0)&&((row%2)==0) )
		setStyleSheet("QWidget{background-color:#bbdddd;}");
	mainLayout->addWidget(packetLabel,Qt::AlignCenter);
}
void cellLabelClass::mouseReleaseEvent(QMouseEvent *ev)
{
	cellContClass *cellCont=new cellContClass(cellNumber,this);
	cellCont->exec();
	delete cellCont;
}
void cellLabelClass::writeToPdf(QPainter *painter,double x,double y,float cellWidth,float cellHeight)
{
	//HPDF_Page_SetRGBFill (*page,0.0,0.2,0.0);
	//HPDF_Page_Rectangle(*page,x,y-PDF_CELL_H,PDF_CELL_W,PDF_CELL_H);
	//HPDF_Page_Stroke(*page);
	painter->setPen(QPen(QColor(200,200,200)));
	painter->drawRect(QRectF(x,y,cellWidth,cellHeight));
	if (packetId!=0)
	{
	//	if (isOnSb)
	//		HPDF_Page_SetRGBFill (*page,0.0,0.0,1.0);
	//	HPDF_Page_BeginText(*page);
	//	HPDF_Page_SetFontAndSize(*page,font,10);
	//	HPDF_Page_TextRect(*page,x,y,x+PDF_CELL_W,y-PDF_CELL_H,(QString("%1").arg(packetId)).toLocal8Bit().data(),HPDF_TALIGN_CENTER,NULL);
		painter->setPen(QPen(QColor(100,100,255)));
		painter->drawText(QRectF(x,y,cellWidth,cellHeight),Qt::AlignCenter|Qt::TextWordWrap,QString("%1").arg(packetId));
	//	HPDF_Page_EndText(*page);
	}
}

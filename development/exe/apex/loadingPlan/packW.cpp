#include "packW.h"
packetWidgetClass::packetWidgetClass(QWidget *p):QLabel(p)
{
	packet=NULL;
	parentColumn=NULL;
	initialParentColumn=NULL;
	mouseOver=false;
}
packetWidgetClass::packetWidgetClass(packetClass *packet_,QString soNum_,QString soItem_,QString alloy_,QString temper_,QString datePacked_,QString status_,packetColumnClass *parentColumn_,QColor color_,int cassette_,QWidget *p):QLabel(p)
{
	soNum=soNum_;
	soItem=soItem_;
	alloy=alloy_;
	temper=temper_;
	datePacked=datePacked_;
	status=status_;
	mouseOver=false;
	color=color_;
	cassette=cassette_;
	setCursor(Qt::PointingHandCursor);
	packet				=packet_;
	parentColumn=parentColumn_;
	setGeometry(0,0,(packet->width+2*woodWidth)*PACKETSCALE,(packet->height+2*woodWidth)*PACKETSCALE);
	//setMinimumWidth(packet->width*PACKETSCALE);
	//setMinimumHeight(packet->height*PACKETSCALE);
	//setMaximumWidth(packet->width*PACKETSCALE);
	//setMaximumHeight(packet->height*PACKETSCALE);
	initialParentColumn=parentColumn;
	setParent(initialParentColumn);

	profileImgFile=getImageOfProfile(packet->profile);
	QImage profileImg;
	bool l=profileImg.load(profileImgFile);
	double xScale=(packet->width*PACKETSCALE)/profileImg.width();
	double yScale=(packet->height*PACKETSCALE)/profileImg.height();
	double scale=xScale;
	if (yScale<scale)scale=yScale;
	//int imgWidth=scale*;
	//int imgHeight=scale*profileImg.height();


	profileImg=profileImg.scaled(profileImg.width()*scale,profileImg.height()*scale,Qt::KeepAspectRatio,Qt::SmoothTransformation);
	profilePixMap=new QPixmap(profileImg.size());
	profilePixMap->fill(Qt::transparent);
	QPainter profilePainter;
	profilePainter.begin(profilePixMap);
	profilePainter.setCompositionMode(QPainter::CompositionMode_Source);
	profilePainter.drawPixmap(0, 0, QPixmap::fromImage(profileImg));
	profilePainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	profilePainter.fillRect(profilePixMap->rect(), color);
	profilePainter.end();

	backGroundPixMap=new QPixmap(QSize(width(),height()));
	backGroundPixMap->fill(Qt::transparent);
	QPainter backgroundPainter;
	backgroundPainter.begin(backGroundPixMap);
	backgroundPainter.setCompositionMode(QPainter::CompositionMode_Source);
	backgroundPainter.fillRect(backGroundPixMap->rect(),QColor(255,255,255));
	backgroundPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	backgroundPainter.fillRect(backGroundPixMap->rect(), color);
	backgroundPainter.end();
	setTipText();
}

void packetWidgetClass::setInitialParentColumn(packetColumnClass *initialParentColumn_)
{
	initialParentColumn=initialParentColumn_;
}
void packetWidgetClass::setParentColumn(packetColumnClass *parentColumn_)
{
	parentColumn=parentColumn_;
	setParent(parentColumn);
}
void packetWidgetClass::calculateGeometry()
{
	setGeometry(0,0,(packet->width+2*woodWidth)*PACKETSCALE,(packet->height+2*woodWidth)*PACKETSCALE);
	//setMinimumWidth(packet->width*PACKETSCALE);
	//setMinimumHeight(packet->height*PACKETSCALE);
	//setMaximumWidth(packet->width*PACKETSCALE);
	//setMaximumHeight(packet->height*PACKETSCALE);
}

void packetWidgetClass::removeFromParent()
{
	parentColumn->removePacket(this);
}
void packetWidgetClass::setTipText()
{
	QString tableTxt;
	tableTxt+=QString("<table><tr><td>%1</td><td><b>%2</b></td><td>%3</td><td><b>%4</b></td></tr>").arg(tr("Ticket")).arg(packet->code).arg(tr("Die")).arg(packet->profile);
	tableTxt+=QString("<tr><td align=\"center\" colspan=\"4\"><img src=\"%1\"</td></tr>").arg(profileImgFile);
	tableTxt+=QString("<tr><td>%1</td><td><b>%2mm </b></td><td>%3</td><td><b>%4</b></td></tr>").arg(tr("Width" )).arg(packet->width               ).arg(tr("SoNum")).arg(soNum);
	tableTxt+=QString("<tr><td>%1</td><td><b>%2mm </b></td><td>%3</td><td><b>%4</b></td></tr>").arg(tr("Height")).arg(packet->height              ).arg(tr("SoItem")).arg(soItem);
	tableTxt+=QString("<tr><td>%1</td><td><b>%2m  </b></td><td>%3</td><td><b>%4</b></td></tr>").arg(tr("Length")).arg(packet->length              ).arg(tr("Alloy")).arg(alloy);
	tableTxt+=QString("<tr><td>%1</td><td><b>%2Kgr</b></td><td>%3</td><td><b>%4</b></td></tr>").arg(tr("Weight")).arg(packet->weight              ).arg(tr("Temper")).arg(temper);
	tableTxt+=QString("<tr><td>%1</td><td><b>%2   </b></td><td>%3</td><td><b>%4</b></td></tr>").arg(tr("Drop"  )).arg(packet->orderId             ).arg(tr("Date")).arg(datePacked);
	tableTxt+=QString("<tr><td>%1</td><td><b>%2   </b></td><td>%3</td><td><b>%4</b></td></tr>").arg(tr("Top "  )).arg(packet->topPlacementPriority).arg(tr("Status")).arg(status);
	tableTxt+=QString("</table>");
	QString tltp;
	tltp+=QString("<span style=\"background-color:white;\">%1</span>").arg(tableTxt);
	setStyleSheet("QToolTip {background-color: #ffffff;border:1px solid gray; }");
	setToolTip(tltp);
}

void packetWidgetClass::mouseReleaseEvent(QMouseEvent *ev)
{	
	if (getManualExportOrder()==0)
	{
		(*minAvailableManualOutOrder)++;
		setManualExportOrder(*minAvailableManualOutOrder);
	}
	update();
	return QLabel::mouseReleaseEvent(ev);
}
void packetWidgetClass::mousePressEvent(QMouseEvent *ev)
{	
	update();
	return QWidget::mousePressEvent(ev);
}
void packetWidgetClass::enterEvent (QEvent *ev)
{
	mouseOver=true;
	update();
	return QLabel::enterEvent(ev);
}
void packetWidgetClass::leaveEvent(QEvent *ev)
{
	mouseOver=false;
	update();
	return QLabel::leaveEvent(ev);
}
double packetWidgetClass::getYCG()
{
	return packet->getYCG();
}

static QPen normalPen(QColor(50,50,50), 1, Qt::SolidLine);
static QPen mouseOverPen(Qt::white, 2, Qt::SolidLine);
QPixmap packetWidgetClass::paint(bool forDrag)
{
	QPixmap ansPixMap(width(),height());
	int w=width();
	int h=height();
	QPainter painter(&ansPixMap);
	painter.drawPixmap(backGroundPixMap->rect(),*backGroundPixMap);
	painter.setFont(QFont("helvetica",7));
	painter.setPen(normalPen);
	//painter.setBrush(QBrush(color));
	painter.drawRect(0,0,w-1,h-1);
	if (mouseOver)
	{
		painter.setPen(mouseOverPen);
		painter.drawRect(1,1,w-2,h-2);
		painter.setPen(normalPen);
	}
	//painter.drawImage(
	//	QRectF(woodWidth*PACKETSCALE+0.5*(packet->width*PACKETSCALE-profileImg.width()),woodWidth*PACKETSCALE+0.5*(packet->height*PACKETSCALE-profileImg.height()),profileImg.width(),profileImg.height()),
	//	profileImg.createMaskFromColor(QRgb((255,255,255))));
	painter.drawPixmap(
		QRectF(woodWidth*PACKETSCALE+0.5*(packet->width*PACKETSCALE-profilePixMap->width()),woodWidth*PACKETSCALE+0.5*(packet->height*PACKETSCALE-profilePixMap->height()),profilePixMap->width(),profilePixMap->height()),
		*profilePixMap,profilePixMap->rect());
	//QFontMetrics fm(painter.font());
	int m=3;int txtH=woodWidth*PACKETSCALE;//fm.height();
	if ((3.*double(txtH))>double(h))
		txtH=double(h)/3.;
	painter.drawText(m,h-txtH,w-2*m,txtH,Qt::AlignLeft|Qt::AlignVCenter,QVariant(packet->weight).toString()+"Kgr");
	painter.drawText(m,h-txtH,w-2*m,txtH,Qt::AlignRight|Qt::AlignVCenter,QVariant(packet->code).toString());
	painter.setFont(QFont("helvetica",8));
	if (packet->length>6001)
		painter.setPen(QPen(QColor(255,0,0), 1, Qt::SolidLine));
	else if (packet->length>5001)
		painter.setPen(QPen(QColor(0,255,0), 1, Qt::SolidLine));
	else if (packet->length>4001)
		painter.setPen(QPen(QColor(0,0,255), 1, Qt::SolidLine));
	else
		painter.setPen(QPen(QColor(255,0,0), 1, Qt::SolidLine));
	painter.drawText(m,h-txtH,w-2*m,txtH,Qt::AlignHCenter|Qt::AlignVCenter,QString("%1m").arg(packet->length));
	if (packet->manualExportOrder!=0)
	{
		painter.setFont(QFont("helvetica",22));
		if (cassette!=0)
			painter.setPen(QPen(QColor(0,0,255), 1, Qt::SolidLine));
		else
			painter.setPen(QPen(QColor(200,0,0), 1, Qt::SolidLine));
		painter.drawText(0,0,w,h,Qt::AlignCenter|Qt::AlignVCenter,QVariant(packet->manualExportOrder).toString());
	}
	if (forDrag)
	{
		painter.drawPixmap(0, 0, QPixmap::fromImage(QImage(":/dragArrow.png")));
	}
	painter.end();
	return ansPixMap;
}
void packetWidgetClass::paintEvent(QPaintEvent *ev)
{
	QPainter thisPainter(this);
	thisPainter.drawPixmap(QRect(0,0,width(),height()),paint(false));
	thisPainter.end();
}
void packetWidgetClass::appendYourSelfToInitialParentColumn()
{
	if (parentColumn)
		parentColumn->removePacket(this);
	if (initialParentColumn)
		initialParentColumn->appendPacket(this);
}
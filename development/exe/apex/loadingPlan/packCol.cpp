#include "packCol.h"
int guiTextHeight=15;
packetColumnClass::packetColumnClass(int horAlignment_,int perpAlignment_,QString labelString_,int maxHeight_,QWidget *p):QWidget(p)
{
	guiX=0;
	guiY=0;
	loadingPlan=NULL;
	horAlignment=horAlignment_;
	perpAlignment=perpAlignment_;
	labelString=labelString_;
	setAcceptDrops(true);
	selectedPacket=NULL;
	maxHeight=maxHeight_;
	calculate();
	isDragOver=false;
}
void packetColumnClass::setLoadingPlan(loadingPlanClass *lp)
{
	loadingPlan=lp;
}
void packetColumnClass::mouseReleaseEvent(QMouseEvent *ev)
{
	return;
}
void packetColumnClass::enterEvent (QEvent *ev)
{
}
void packetColumnClass::leaveEvent(QEvent *ev)
{
}
int packetColumnClass::getOrderIdOfFirstPacket()
{
	if (packetsVector.size()>0)
		return packetsVector[0]->getOrderId();
	else
		return -1;
}
void packetColumnClass::insertPacket(int index,packetWidgetClass* packet)
{
	packetsVector.insert(index,packet);
	if (loadingPlan)
		loadingPlan->calculate();
	calculate();
}
void packetColumnClass::appendPacket(packetWidgetClass* packet)
{
	packetsVector.append(packet);
	if (loadingPlan)
		loadingPlan->calculate();
	calculate();
}
void packetColumnClass::removePacket(packetWidgetClass* packet)
{
	packetsVector.remove(packetsVector.indexOf(packet));
	if (loadingPlan)
		loadingPlan->calculate();
	calculate();
}
void packetColumnClass::calculate()
{
	totalWeight=0;
	totalHeight=0;
	double Ymom=0.;
	YCG=0.;
	double h=0.;
	for (int i=0;i<packetsVector.size();i++)
	{
		packetsVector[i]->calculateGeometry();
		totalWeight+=packetsVector[i]->getWeight();
		totalHeight+=packetsVector[i]->getHeight();
		Ymom+=(h+packetsVector[i]->getYCG())*packetsVector[i]->getWeight();
		h+=packetsVector[i]->getHeight();
	}
	if (totalWeight>0.1)
		YCG=Ymom/totalWeight;
	calculateGui();
}
void packetColumnClass::calculateGui()
{
	int W=0.5*MAXCOLUMNGUIWIDTH;
	for (int i=0;i<packetsVector.size();i++)
	{
		if (W<packetsVector[i]->width())
			W=packetsVector[i]->width();
	}
	W+=4;
	int H=guiTextHeight;
	for (int i=0;i<packetsVector.size();i++)
	{
		packetsVector[i]->setParentColumn(this);
		packetsVector[i]->setParent(this);
		packetsVector[i]->show();
		H+=packetsVector[i]->height();
	}
	H+=guiTextHeight;
	H+=2;
	if (perpAlignment==Qt::AlignBottom)
		H=PACKETSCALE*maxHeight+2.*guiTextHeight;
	if (perpAlignment==Qt::AlignTop)
	{
		H=guiTextHeight;
		for (int i=0;i<packetsVector.size();i++)
		{
			if (horAlignment==Qt::AlignLeft)
				packetsVector[i]->move(2,H);
			else if (horAlignment==Qt::AlignRight)
				packetsVector[i]->move(W-packetsVector[i]->width()-2,H);
			else if (horAlignment==Qt::AlignHCenter)
				packetsVector[i]->move(0.5*(W-packetsVector[i]->width()),H);
			H+=packetsVector[i]->height();
		}
		H+=guiTextHeight;
	}
	if (perpAlignment==Qt::AlignBottom)
	{
		int hh=H-guiTextHeight;
		for (int i=0;i<packetsVector.size();i++)
		{
			hh-=packetsVector[i]->height();
			if (horAlignment==Qt::AlignLeft)
				packetsVector[i]->move(2,hh);
			else if (horAlignment==Qt::AlignRight)
				packetsVector[i]->move(W-packetsVector[i]->width()-2,hh);
			else if (horAlignment==Qt::AlignHCenter)
				packetsVector[i]->move(0.5*(W-packetsVector[i]->width()),hh);
		}
	}
	setMinimumWidth(W);
	setMaximumWidth(W);
	setMinimumHeight(H);
	setMaximumHeight(H);
}
void packetColumnClass::paintEvent(QPaintEvent *ev)
{
	QPainter painter(this);
	painter.setFont(QFont("helvetica",7));
	if (!isDragOver)
		painter.setPen(QPen(Qt::white, 1, Qt::DotLine));
	else
		painter.setPen(QPen(QColor(100,150,100),2, Qt::SolidLine));
	painter.drawRect(1,1,width()-2,height()-2);
	painter.drawText(0,0,width(),guiTextHeight,Qt::AlignHCenter|Qt::AlignVCenter,labelString);
	//painter.drawText(0,0,MAXCOLUMNGUIWIDTH,guiTextHeight,Qt::AlignHCenter|Qt::AlignVCenter,labelString);
	if (perpAlignment==Qt::AlignBottom)
	{
		painter.setPen(QPen(Qt::red, 1, Qt::DashDotDotLine));
		painter.drawLine(2,guiTextHeight,width()-4,guiTextHeight);
		if (totalHeight>maxHeight)
			painter.setPen(QPen(Qt::red));
		else
			painter.setPen(QPen(Qt::white));
		painter.drawText(2,height()-guiTextHeight,width()-4,guiTextHeight,Qt::AlignLeft|Qt::AlignVCenter,QVariant(totalHeight*0.001).toString()+" m");
		painter.setPen(QPen(Qt::white));
	}
	painter.drawText(2,height()-guiTextHeight,width()-4,guiTextHeight,Qt::AlignRight|Qt::AlignRight,QVariant(totalWeight).toString()+"Kgr");
}
/////////////////////////////
packetWidgetClass *packetColumnClass::getMouseOverItem()
{
	for (int i=0;i<packetsVector.size();i++)
	{
		if (packetsVector[i]->isMouseOver())
		{
			return packetsVector[i];
		}
	}
	return NULL;
}
void packetColumnClass::setMaxHeight(int h)
{
	maxHeight=h;
}
void packetColumnClass::mousePressEvent(QMouseEvent *event)
{
	if (event->button()==Qt::LeftButton)
		selectedPacket=getMouseOverItem();
    QWidget::mousePressEvent(event);
}
void packetColumnClass::mouseMoveEvent(QMouseEvent *event)
{
    if ( (event->buttons()&Qt::LeftButton) && selectedPacket )
	{
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            performDrag();
    }
    QWidget::mouseMoveEvent(event);
}

void packetColumnClass::performDrag()
{
  if (selectedPacket)
	{
		QMimeData *mimeData = new QMimeData;
    QDrag *drag = new QDrag(selectedPacket);
		//drag->setHotSpot(QPoint(0.5*selectedPacket->getPixMap()->width(),0.5*selectedPacket->getPixMap()->height()));
		drag->setDragCursor(selectedPacket->paint(true),Qt::MoveAction);
		drag->setMimeData(mimeData);
		drag->exec(Qt::MoveAction);
  }
}
void packetColumnClass::dragEnterEvent(QDragEnterEvent *event)
{
	packetWidgetClass *source=(packetWidgetClass*)(event->source());
	if (source)
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
  }
}
void packetColumnClass::dragLeaveEvent(QDragLeaveEvent * event)
{
	isDragOver=false;
	event->accept();
	update();
}

void packetColumnClass::dragMoveEvent(QDragMoveEvent *event)
{
	packetWidgetClass *source=(packetWidgetClass*)(event->source());
	if (source)
	{
		setCursor(Qt::DragCopyCursor);
    event->setDropAction(Qt::MoveAction);
		event->accept();
		isDragOver=true;
		update();
	}
}
void packetColumnClass::dropEvent(QDropEvent *event)
{
	packetWidgetClass *source=(packetWidgetClass*)(event->source());
	if (source)
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
		bool inserted=false;
		for (int i=0;i<packetsVector.size();i++)
		{
			if ((event->pos().y()>packetsVector[i]->y())&&(event->pos().y()<(packetsVector[i]->y()+packetsVector[i]->height())))
			{
				inserted=true;
				source->removeFromParent();
				insertPacket(i,source);
				break;
			}
		}
		if (!inserted)
		{
			source->removeFromParent();
			appendPacket(source);
		}
	}
	selectedPacket=NULL;
	isDragOver=false;
}
void packetColumnClass::setGuiX(int xx)
{
	guiX=xx;
	move(guiX,guiY);
}
void packetColumnClass::setGuiY(int yy)
{
	guiY=yy;
	move(guiX,guiY);
}
//
void packetColumnClass::addPacketToManualExportList(int packetInt)
{
	//QString stmnt;
	//QString packetStr=QVariant(packetInt).toString();
	////cassete
	//stmnt=QString("SELECT position FROM "+positionsTable+" where packet="+packetStr);
	//QVector<QVariant> posArray=execQuery(stmnt,mainConnectionName);
	//if (posArray.size()!=1)
	//	EXIT(("Error"),tr("The packet: %1 is not in the warehouse").arg(packetInt));
	//int casseteInt=posArray[0].toInt();
	//QString casseteStr=QVariant(casseteInt).toString();
	////check if exists in manualExportsTable
	//stmnt=QString("SELECT cassete FROM "+manualExportsTable+" where cassete="+casseteStr);
	//QVector<QVariant> casseteOnExportTableArray=execQuery(stmnt,mainConnectionName);
	//if (casseteOnExportTableArray.size()!=0)
	//	return;
	////get next orderOut
	//stmnt=QString("SELECT top 1 outOrder FROM "+manualExportsTable+" order by outOrder DESC");
	//QVector<QVariant> orderArray=execQuery(stmnt,mainConnectionName);
	//int lastOrderOut=0;
	//if (orderArray.size()>0)
	//	lastOrderOut=orderArray[0].toInt();
	//QString orderOutStr=QVariant(lastOrderOut+1).toString();
	////insert to export table
	//stmnt=QString("INSERT INTO ")+manualExportsTable+" (cassete,outOrder,packet) VALUES("+casseteStr+","+orderOutStr+","+packetStr+")";
	//execQuery(stmnt,mainConnectionName);
}
void packetColumnClass::exportPacketsToManual()
{
	for (int i=0;i<packetsVector.size();i++)
	{
		addPacketToManualExportList(packetsVector[i]->getCode());
	}
}

//optim
void packetColumnClass::clearPackets()
{
	while (packetsVector.size()>0)
	{
		packetsVector[0]->appendYourSelfToInitialParentColumn();
	}
	packetsVector.clear();
	selectedPacket=NULL;
	totalWeight=totalHeight=YCG=0;
	calculate();
}
void packetColumnClass::clearManualExportOrder()
{
	for (int i=0;i<packetsVector.size();i++)
	{
		packetsVector[i]->setManualExportOrder(0);
	}
}

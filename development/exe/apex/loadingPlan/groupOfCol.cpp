#include "groupOfCol.h"
groupOfColumnsClass::groupOfColumnsClass(QScrollArea *scrollArea,QVector<packetColumnClass*> packetColumnsVector_,loadingPlanClass *loadingPlan_,int groupType_,bool drawCG_,QWidget *p):QWidget(p)
{
	groupType=groupType_;
	loadingPlan=loadingPlan_;
	packetColumnsVector=packetColumnsVector_;
	maxWeight=0;
	drawCG=drawCG_;
	totalWeight=0;
	XCG=0;
	YCG=0;
	int x=0;int y=0;
	for (int i=0;i<packetColumnsVector.size();i++)
	{
		packetColumnsVector[i]->setParent(this);
		packetColumnsVector[i]->setLoadingPlan(loadingPlan);
		packetColumnsVector[i]->setGuiX(x);
		packetColumnsVector[i]->setGuiY(y);
		x+=MAXCOLUMNGUIWIDTH;
	}
	this->setParent(scrollArea);
	scrollArea->setWidget(this);
	//
	calculate();
	connect(&mainTimer, SIGNAL(timeout()), this, SLOT(onMainTimer()));
	mainTimer.start(50);
}
void groupOfColumnsClass::setMaxHeight(int h)
{
	maxHeight=h;
	for (int i=0;i<packetColumnsVector.size();i++)
	{
		packetColumnsVector[i]->setMaxHeight(maxHeight);
	}
}
void groupOfColumnsClass::setMaxWeight(int m)
{
	maxWeight=m;
	calculate();
}
void groupOfColumnsClass::calculate()
{
	totalWeight=0;
	double Xmom=0.;
	double Ymom=0.;
	XCG=0;	YCG=0;
	int X=0;
	for (int i=0;i<packetColumnsVector.size();i++)
	{
		packetColumnsVector[i]->calculate();
		packetColumnsVector[i]->setGuiX(X);
		X+=packetColumnsVector[i]->width();
		double w=packetColumnsVector[i]->getWeight();
		totalWeight+=w;
		if (i==0)
			Xmom+=-w*2.*MAXCOLUMNWIDTH;
		if (i==1)
			Xmom+=-w*1.*MAXCOLUMNWIDTH;
		if (i==3)
			Xmom+=w*1.*MAXCOLUMNWIDTH;
		if (i==4)
			Xmom+=w*2.*MAXCOLUMNWIDTH;
		Ymom+=packetColumnsVector[i]->getWeight()*packetColumnsVector[i]->getYCG();
	}
	if (totalWeight>0.1)
	{
		XCG=Xmom/totalWeight;
		YCG=Ymom/totalWeight;
	}
	calculateGeometry();
	update();
}

void groupOfColumnsClass::onMainTimer()
{

}
QString toString(double v,int pres=0)
{
	v=((int)(v*pow(10.,pres)))/pow(10.,pres);
	return QVariant(v).toString();
}
void groupOfColumnsClass::calculateGeometry()
{
	int H=0;
	int W=0;
	for (int i=0;i<packetColumnsVector.size();i++)
	{
		if (packetColumnsVector[i]->height()>H)
			H=packetColumnsVector[i]->height();
		W+=packetColumnsVector[i]->width();
	}
	if (W==0)
		W=MAXCOLUMNGUIWIDTH;
	H+=15;
	//int W=packetColumnsVector.size()*MAXCOLUMNGUIWIDTH;
	if ((W!=width())||(H!=height()))
	{
		setGeometry(0,0,W,H);
//		setMinimumWidth(W);
//		setMinimumHeight(H);
//		setMaximumWidth(W);
//		setMaximumHeight(H);
	}
}
void groupOfColumnsClass::paintEvent(QPaintEvent *ev)
{
	QPainter painter(this);
	painter.setPen(QPen (Qt::white, 1, Qt::DotLine));
	painter.drawRect(1,1,width()-2,height()-2);
	QString label;
	if (drawCG)
	{
		label+="X:"+toString(XCG)+"mm ";
		label+="Y:"+toString(YCG)+"mm ";
	}
	if (maxWeight>0)
		label+=toString(totalWeight)+tr(" out of ")+QString().number(maxWeight)+" Kgr";
	else
		label+=toString(totalWeight)+" Kgr";
	painter.drawText(2,height()-15,width()-4,15,Qt::AlignRight|Qt::AlignRight,label);
}
//optim
void groupOfColumnsClass::appendPacket(packetWidgetClass *p,int column)
{
	packetColumnsVector[column]->appendPacket(p);
}
void groupOfColumnsClass::clearPackets()
{
	for (int i=0;i<packetColumnsVector.size();i++)
	{
		packetColumnsVector[i]->clearPackets();
	}
}
void groupOfColumnsClass::clearManualExportOrder()
{
	for (int i=0;i<packetColumnsVector.size();i++)
	{
		packetColumnsVector[i]->clearManualExportOrder();
	}
}
void groupOfColumnsClass::exportLoading(int loadingId)
{
	//QString stmnt="INSERT INTO "+loadingsTableName+" (packet,col,row,loadingid,status,station) VALUES (:packet,:col,:row,"+QString().number(loadingId)+",1,"+QString().number(-1)+")";
	//QSqlQuery query(QSqlDatabase::database(mainConnectionName));
	//query.prepare(stmnt);
	//for (int i=0;i<packetColumnsVector.count();i++)
	//{
	//	for (int j=0;j<packetColumnsVector[i]->getNoOfPackets();j++)
	//	{
	//		packetWidgetClass *p=packetColumnsVector[i]->getPacket(j);
	//		query.bindValue(":packet",p->getCode());
	//		if (groupType==FOR_GROUP)
	//			query.bindValue(":col",i+1);
	//		else
	//			query.bindValue(":col",i+6);
	//		query.bindValue(":row",j+1);
	//		if (!(query.exec()))
	//			EXIT(("Error in database"),query.lastError().text());
	//	}
	//}
}
int groupOfColumnsClass::getNoOfPackets()
{
	int no=0;
	for (int i=0;i<packetColumnsVector.size();i++)
	{
		no+=packetColumnsVector[i]->getNoOfPackets();
	}
	return no;
}
int groupOfColumnsClass::getWeight()
{
	int w=0;
	for (int i=0;i<packetColumnsVector.size();i++)
	{
		w+=packetColumnsVector[i]->getWeight();
	}
	return w;
}
//
//void groupOfColumnsClass::exportPacketsToManual()
//{
//	packetColumnsVector[2]->exportPacketsToManual();
//	packetColumnsVector[1]->exportPacketsToManual();
//	packetColumnsVector[3]->exportPacketsToManual();
//	packetColumnsVector[0]->exportPacketsToManual();
//	packetColumnsVector[4]->exportPacketsToManual();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////
///// pdf  ///// pdf  ///// pdf  ///// pdf  ///// pdf  ///// pdf  ///// pdf  ///// pdf  ///// pdf  
/////////////////////////////////////////////////////////////////////////////////////////////////
void groupOfColumnsClass::exportToPdf(QPainter *painter,int activeLoading,QString date)
{
	float scale=46;
	int W=297*scale;
	int H=208*scale;
	int txtDh=250;
	float packetScale=11.;//screen->pdf
	painter->drawText(QRect(0,0,W,txtDh),Qt::AlignCenter,tr("Loading:%1 %2 %3").arg(activeLoading).arg(groupType==FOR_GROUP?tr("For group"):tr("Aft group")).arg(date));
	painter->drawText(QRect(0,H-3*txtDh,W,txtDh),Qt::AlignCenter,tr("%1 packets   %2kgr X:%3m Y:%4m").arg(getNoOfPackets()).arg(getWeight()).arg(getXCG()/1000.).arg(getYCG()/1000.));
	int x,y,w,h;
	painter->setPen(QPen(QBrush(QColor(0,0,0)),1));
	int totalWidth=0;
	int totalHeight=0;
	for (int i=0;i<packetColumnsVector.size();i++)
	{
		totalWidth+=packetColumnsVector[i]->width();
		if (totalHeight<packetColumnsVector[i]->height())
			totalHeight=packetColumnsVector[i]->height();
	}
	int Xo=0.5*(W-totalWidth*packetScale);
	int Yo=txtDh+(H-5*txtDh)-totalHeight*packetScale;
	int X=Xo;
	for (int i=0;i<packetColumnsVector.size();i++)
	{
		QString colTitle=QString("%1 Kgr %2m").arg(packetColumnsVector[i]->getWeight()).arg(double(packetColumnsVector[i]->getHeight())/1000.);
		painter->drawText(QRect(X,H-4*txtDh,(packetColumnsVector[i]->width()*packetScale),txtDh),Qt::AlignCenter,colTitle);
		for (int j=0;j<packetColumnsVector[i]->getNoOfPackets();j++)
		{
			packetWidgetClass* p=packetColumnsVector[i]->getPacket(j);
			w=p->width()*packetScale;
			h=p->height()*packetScale;
			x=X+p->x()*packetScale;
			y=Yo+p->y()*packetScale;
			QImage img=QImage(p->getProfileImgFile()).scaled(QSize(w-2,h-txtDh-2),Qt::KeepAspectRatio);
			painter->drawImage(QPoint(x+0.5*(w-img.width()),y+txtDh+0.5*(h-txtDh-img.height())),img);
			painter->drawRect(x,y,w,h);
			painter->drawText(QRect(x,y,w,txtDh),Qt::AlignCenter,QString("%1-%2").arg(p->getCode()).arg(p->getOrderId()));
		}
		X+=packetColumnsVector[i]->width()*packetScale;
	}
}
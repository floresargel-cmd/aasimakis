#include "optim.h"
QVector<packetClass*>	allPacketsVector;
QVector<int>			allOrdersVector;
int						firstOrder;
QVector<double>			optimGenVector;
QVector<double>			optimObjVector;
//
//#define NO_OF_ISLANDS 25
//#define MAX_GENERATIONS 100
//#define NO_OF_PACKETS_POP_MULTIPLIER 10

#define CROSSOVER 0.25
#define MUTATION 0.25
#define MIGRATION 0.25

#define NO_OF_ISLANDS 25
#define MAX_GENERATIONS 500
#define NO_OF_PACKETS_POP_MULTIPLIER 25
#define MAX_OBJECTIVE 100000



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////plotClass///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//plotClass::plotClass(QWidget *parent):QwtPlot(parent)
//{
//    // Disable polygon clipping
//    QwtPainter::setDeviceClipping(false);
//    // We don't need the cache here
//    canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
//    canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
//	alignScales();
//	// Assign a title
//    insertLegend(new QwtLegend(), QwtPlot::BottomLegend);
//    // Insert new curves
//	int fontSize=9;
//  	QwtText curvelabelTxt(G("στόχος"));
//	curvelabelTxt.setFont(QFont("helvetica",fontSize));
//	progressCurve = new QwtPlotCurve(curvelabelTxt);
//    progressCurve->attach(this);
//    // Set curve styles
//	progressCurve->setStyle(QwtPlotCurve::Lines);
//    progressCurve->setPen(QPen(Qt::gray));
//	//
//    QwtSymbol sym;
//    sym.setStyle(QwtSymbol::Ellipse);
//	sym.setBrush(QBrush(Qt::white));
//    sym.setPen(QColor(Qt::darkGray));
//    sym.setSize(5);
//	progressCurve->setSymbol(sym);
//    // Axis 
//	QwtText xlabelTxt(G("δοκιμές"));
//	xlabelTxt.setFont(QFont("helvetica",fontSize));
//    setAxisTitle(QwtPlot::xBottom,xlabelTxt);
//	setAxisFont(QwtPlot::xBottom,QFont("helvetica",fontSize));
//	
//	QwtText ylabelTxt(G("στόχος"));
//	ylabelTxt.setFont(QFont("helvetica",fontSize));
//    setAxisTitle(QwtPlot::yLeft,ylabelTxt);
//	setAxisFont(QwtPlot::yLeft,QFont("helvetica",fontSize));
//	//title
//	//QwtText titlelabelTxt(G("στόχος"));
//	//titlelabelTxt.setFont(QFont("helvetica",fontSize));
// //   setTitle(titlelabelTxt);
//}
//void plotClass::calculateFitCurve(int gen,double obj)
//{
//	obj=fabs(obj);
//	//if (obj>MAX_OBJECTIVE)
//	//	return;
//	if ( (optimGenVector.count()>0)&&(optimGenVector.last()==gen) )
//	{
//		if (optimObjVector.last()>obj)
//			optimObjVector.last()=obj;
//	}
//	else
//	{
//		optimGenVector.append(gen);
//		optimObjVector.append(obj);
//	}
//	if (optimGenVector.count()>MAX_GENERATIONS*0.10)
//	{
//		optimGenVector.remove(0,1);
//		optimObjVector.remove(0,1);
//	}
//	int no=optimGenVector.count();
//
//	double *fx=new double[no];
//	double *fy=new double[no];
//
//	for (int i=0;i<no;i++)
//	{
//		fx[i]=optimGenVector[i];
//		fy[i]=(MAX_OBJECTIVE-optimObjVector[i])/1000.;
//	}
//	setAxisScale(QwtPlot::xBottom,fx[0],gen);
//	setAxisScale(QwtPlot::yLeft,fy[0],100);
//	progressCurve->setRawData(fx,fy,no);
//}
//
//void plotClass::alignScales()
//{
//    canvas()->setFrameStyle(QFrame::Box | QFrame::Plain );
//    canvas()->setLineWidth(1);
//    for ( int i = 0; i < QwtPlot::axisCnt; i++ )
//    {
//        QwtScaleWidget *scaleWidget = (QwtScaleWidget *)axisWidget(i);
//        if (scaleWidget)
//            scaleWidget->setMargin(0);
//        QwtScaleDraw *scaleDraw = (QwtScaleDraw *)axisScaleDraw(i);
//        if ( scaleDraw )
//            scaleDraw->enableComponent(QwtAbstractScaleDraw::Backbone, false);
//    }
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////optimColumn/////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
optimColumn::optimColumn(int id_,double maxHeight_)
{
	id=id_;
	if ((id==0)||(id==5))xPos=-2.;
	if ((id==1)||(id==6))xPos=-1.;
	if ((id==2)||(id==7))xPos=0.;
	if ((id==3)||(id==8))xPos=1.;
	if ((id==4)||(id==9))xPos=2.;
	yCenter=0.;
	totalWeight=0.;
	yMoment=0.;
	maxHeight=maxHeight_;
}
void optimColumn::clearPackets()
{
	packetsVector.clear();
}
void optimColumn::appendPacket(packetClass* p)
{
	packetsVector.append(p);
}
bool optimColumn::hasOrder(int ord)
{
	if (ordersVector.indexOf(ord)!=-1)
		return true;
	return false;
}
int optimColumn::getNoOfPacketsWithOrder(int ord)
{
	int ans=0;
	for (int i=0;i<packetsVector.count();i++)
	{
		if (packetsVector[i]->orderId==ord)
			ans++;
	}
	return ans;
}
int optimColumn::getHeightOfOrder(int ord)
{
	int index=ordersVector.indexOf(ord);
	if (index!=-1)
		return ordersHeightVector[index];
	return 0;
}
void optimColumn::optimizePackets()
{
	bool flag=true;
	for(int i=1;(i<=packetsVector.count())&&flag;i++)
	{
		flag=false;
		for (int j=0;j<(packetsVector.count()-1); j++)
		{
			if (packetsVector[j+1]->orderId<packetsVector[j]->orderId)
			{
				packetClass *p=packetsVector[j];
				packetsVector[j]=packetsVector[j+1];
				packetsVector[j+1]=p;
				flag=true;
			}
			if (packetsVector[j+1]->orderId==packetsVector[j]->orderId)
			{
				if (packetsVector[j+1]->width>packetsVector[j]->width)
				{
					packetClass *p=packetsVector[j];
					packetsVector[j]=packetsVector[j+1];
					packetsVector[j+1]=p;
					flag=true;
				}
			}
		}	
	}
	//orders vectors
	ordersVector.clear();
	ordersHeightVector.clear();
	double h=0.;
	for (int i=0;i<packetsVector.size();i++)
	{
		h+=packetsVector[i]->height;
		int index=ordersVector.indexOf(packetsVector[i]->orderId);
		if (index==-1)
		{
			ordersVector.append(packetsVector[i]->orderId);
			ordersHeightVector.append(0);
			index=ordersHeightVector.count()-1;
		}
		ordersHeightVector[index]=h;
	}
}
void optimColumn::calculate()
{
	height=0;
	totalWeight=0;
	yMoment=0.;
	yCenter=0.;
	for (int i=0;i<packetsVector.size();i++)
	{
		totalWeight+=packetsVector[i]->weight;
		yMoment+=(height+packetsVector[i]->getYCG())*packetsVector[i]->weight;
		height+=packetsVector[i]->height;
	}
	if (totalWeight>0.1)
		yCenter=yMoment/totalWeight;
}
double optimColumn::calculateObjective()
{
	//maxHeightObj
	double maxHeightObj=0.;
	if (height>maxHeight)
		maxHeightObj=1000000.;
	double obj=maxHeightObj;
	return obj;
}
void optimColumn::definePacketPos(QVector<int> *packetsPosArray)
{
	for (int i=1;i<packetsVector.size();i++)
	{
		int index=allPacketsVector.indexOf(packetsVector[i]);
		(*packetsPosArray)[index]=id;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////optimizerProgressClass//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
optimizerProgressClass::optimizerProgressClass()
{
	setModal(true);
	setWindowTitle(tr("Progress"));
	setWindowIcon(QIcon(":/logo_16x16.png"));

	progressBar=new QProgressBar();
	targetBar=new QProgressBar();targetBar->setMaximum(MAX_OBJECTIVE);
	QPushButton *okBtn=new QPushButton("OK");
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));

	QGridLayout *mainLayout=new QGridLayout();
	mainLayout->addWidget(new QLabel(tr("Search:")),0,0);
	mainLayout->addWidget(progressBar,0,1);
	mainLayout->addWidget(new QLabel(tr("target:")),1,0);
	mainLayout->addWidget(targetBar  ,1,1);
	mainLayout->addWidget(okBtn,2,0,1,2);
	mainLayout->setRowStretch(3,1);
	mainLayout->setColumnStretch(1,1);
	setLayout(mainLayout);
}
void optimizerProgressClass::setMaxProgress(int m)
{
	progressBar->setMaximum(m);
}
void optimizerProgressClass::setProgress(int p,int obj)
{
	targetBar->setValue(MAX_OBJECTIVE-obj);
	progressBar->setValue(p);
}
void optimizerProgressClass::accept()
{
	emit done();
	hide();
}
void optimizerProgressClass::reject()
{
	emit done();
	hide();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////optimizerClass//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
optimizerClass			*optim;
void startOptim();
QString activeStyle="color:#FF0000;font:bold 12px;";
optimizerClass::optimizerClass(loadingPlanClass *loadingPlan_,QWidget *p):QWidget(p)
{
	optim=this;
	optimizerProgress=new optimizerProgressClass();
	optimizerProgress->setGeometry(250,250,700,40);
	loadingPlan=loadingPlan_;
	//
	QHBoxLayout *mainLayout=new QHBoxLayout();
	//row=0
	startOptimPushButton=new QPushButton(tr("Start"));
	mainLayout->addWidget(startOptimPushButton);
	clearManualExportOrderPushButton=new QPushButton(tr("Clear manual export order"));
	mainLayout->addWidget(clearManualExportOrderPushButton);
	
	exportLoadingPushButton=new QPushButton(tr("Export loading to crane"));
	mainLayout->addWidget(exportLoadingPushButton);
	
	//printLabelsPushButton=new QPushButton(QIcon(":/arrow.png"),tr("Print packet labels"));
	//mainLayout->addWidget(printLabelsPushButton);

	printPushButton=new QPushButton(tr("Print loading"));
	mainLayout->addWidget(printPushButton);
	mainLayout->addStretch(1);

	//startLoadingPushButton=new QPushButton(QIcon(":/arrow.png"),tr("Load"));
	//mainLayout->addWidget(startLoadingPushButton,5,0);
	//row=1
	//QLabel *generationsLabelLabel=new QLabel(tr("generations"));mainLayout->addWidget(generationsLabelLabel,6,0);generationsLabelLabel->hide();
	//QLabel *ojectiveLabelLabel=new QLabel(tr("ojectiveLabel"));mainLayout->addWidget(ojectiveLabelLabel,7,0);ojectiveLabelLabel->hide();
	//QLabel *colObjLabelLabel=new QLabel(tr("colObj"));mainLayout->addWidget(colObjLabelLabel,8,0);colObjLabelLabel->hide();
	//QLabel *xObjLabelLabel=new QLabel(tr("xObj"));mainLayout->addWidget(xObjLabelLabel,9,0);xObjLabelLabel->hide();
	//QLabel *yObjLabelLabel=new QLabel(tr("yObj"));mainLayout->addWidget(yObjLabelLabel,10,0);yObjLabelLabel->hide();
	//QLabel *firstOrderObjLabelLabel=new QLabel(tr("firstOrderObj"));mainLayout->addWidget(firstOrderObjLabelLabel,11,0);firstOrderObjLabelLabel->hide();
	//QLabel *orderInOneGroupObjLabelLabel=new QLabel(tr("orderInOneGroupObj"));mainLayout->addWidget(orderInOneGroupObjLabelLabel,12,0);orderInOneGroupObjLabelLabel->hide();
	//QLabel *piramidObjLabelLabel=new QLabel(tr("piramidObj"));mainLayout->addWidget(piramidObjLabelLabel,13,0);piramidObjLabelLabel->hide();
	//QLabel *dWObjLabelLabel=new QLabel(tr("dWObj"));mainLayout->addWidget(dWObjLabelLabel,14,0);dWObjLabelLabel->hide();
	////row=2
	//generationsLabel=new QLabel();mainLayout->addWidget(generationsLabel,6,1);generationsLabel->hide();
	//ojectiveLabel=new QLabel();mainLayout->addWidget(ojectiveLabel,7,1);ojectiveLabel->hide();
	//colObjLabel=new QLabel();mainLayout->addWidget(colObjLabel,8,1);generationsLabel->hide();
	//xObjLabel=new QLabel();mainLayout->addWidget(xObjLabel,9,1);colObjLabel->hide();
	//yObjLabel=new QLabel();mainLayout->addWidget(yObjLabel,10,1);xObjLabel->hide();
	//firstOrderObjLabel=new QLabel();mainLayout->addWidget(firstOrderObjLabel,11,1);yObjLabel->hide();
	//orderInOneGroupObjLabel=new QLabel();mainLayout->addWidget(orderInOneGroupObjLabel,12,1);orderInOneGroupObjLabel->hide();
	//piramidObjLabel=new QLabel();mainLayout->addWidget(piramidObjLabel,13,1);piramidObjLabel->hide();
	//dWObjLabel=new QLabel();mainLayout->addWidget(dWObjLabel,14,1);dWObjLabel->hide();
	//row=3
	//plot=new plotClass(this);
	//mainLayout->addWidget(plot,3,0,1,10);
	//
	setLayout(mainLayout);
	bool ans=true;
	ans=connect(startOptimPushButton,SIGNAL(clicked()),this,SLOT(onStartOptimPushButtonClicked()));
	ans=connect(exportLoadingPushButton,SIGNAL(clicked()),this,SLOT(onExportLoadingPushButtonClicked()));
	ans=connect(printPushButton,SIGNAL(clicked()),this,SLOT(onExportToPrintPushButtonClicked()));
	//ans=connect(printLabelsPushButton,SIGNAL(clicked()),this,SLOT(onPrintLabelsPushButtonClicked()));
	//ans=connect(startLoadingPushButton,SIGNAL(clicked()),this,SLOT(onStartLoadingPushButtonClicked()));

	ans=connect(clearManualExportOrderPushButton,SIGNAL(clicked()),this,SLOT(onClearExportOrderPushButtonClicked()));
	ans=connect(optimizerProgress,SIGNAL(done()),this,SLOT(onDoneFromProgressBar()));
 	connect(&mainTimer, SIGNAL(timeout()),this,SLOT(onMainTimer()));
	mainTimer.start(1*1000);
}
void optimizerClass::setMaxProgress(int m)
{
	optimizerProgress->setMaxProgress(m);
}
void optimizerClass::setProgress(int p,int obj)
{
	optimizerProgress->setProgress(p,obj);
}
void optimizerClass::onDoneFromProgressBar()
{
	stopGaul=true;
}
bool optimizerClass::continueGaul()
{
	return !stopGaul;
}
void optimizerClass::onExportLoadingPushButtonClicked()
{
	QMessageBox::warning(0,tr("Warning"),tr("Not available"));
	loadingPlan->exportLoading();
}
void optimizerClass::onExportToPrintPushButtonClicked()
{
	loadingPlan->exportToPdf();
}
//void optimizerClass::onPrintLabelsPushButtonClicked()
//{
//	loadingPlan->printLabels();
//}
//void optimizerClass::onStartLoadingPushButtonClicked()
//{
//	loadingPlan->startManualLoading();
//}
void optimizerClass::onClearExportOrderPushButtonClicked()
{
	(*minAvailableManualOutOrder)=0;
	loadingPlan->clearManualExportOrder();
}
void optimizerClass::onStartOptimPushButtonClicked()
{
	stopGaul=false;
	optimizerProgress->setProgress(0,MAX_OBJECTIVE);
	optimizerProgress->show();
	optimGenVector.clear();
	optimObjVector.clear();
	allPacketsVector.fill(0,loadingPlan->getAllPackets()->getNoOfPackets());
	allOrdersVector.clear();
	firstOrder=1000;
	for (int i=0;i<loadingPlan->getAllPackets()->getNoOfPackets();i++)
	{
		allPacketsVector[i]=loadingPlan->getAllPackets()->getPacket(i)->getPacket();
		if (allOrdersVector.indexOf(allPacketsVector[i]->orderId)==-1)
			allOrdersVector.append(allPacketsVector[i]->orderId);
		if (allPacketsVector[i]->orderId<firstOrder)
			firstOrder=allPacketsVector[i]->orderId;
	}
	packetColumns.append(optimColumn(0,loadingPlan->getForGroupOfOrders()->getMaxHeight()));
	packetColumns.append(optimColumn(1,loadingPlan->getForGroupOfOrders()->getMaxHeight()));
	packetColumns.append(optimColumn(2,loadingPlan->getForGroupOfOrders()->getMaxHeight()));
	packetColumns.append(optimColumn(3,loadingPlan->getForGroupOfOrders()->getMaxHeight()));
	packetColumns.append(optimColumn(4,loadingPlan->getForGroupOfOrders()->getMaxHeight()));

	packetColumns.append(optimColumn(5,loadingPlan->getAftGroupOfOrders()->getMaxHeight()));
	packetColumns.append(optimColumn(6,loadingPlan->getAftGroupOfOrders()->getMaxHeight()));
	packetColumns.append(optimColumn(7,loadingPlan->getAftGroupOfOrders()->getMaxHeight()));
	packetColumns.append(optimColumn(8,loadingPlan->getAftGroupOfOrders()->getMaxHeight()));
	packetColumns.append(optimColumn(9,loadingPlan->getAftGroupOfOrders()->getMaxHeight()));
	startOptim();
}
void optimizerClass::newSolution(QVector<int> packets,int generation,double bestEver,bool forceUpdate)
{
	//generationsLabel->setText(QString().number(generation));
	//ojectiveLabel->setText(QString().number(bestEver));
	if (stopGaul)
	{
		return;
	}
	if ((generation<(0.00*optimizerProgress->getMax()))&&(!forceUpdate))
	{
		qApp->processEvents();
		return;
	}
	loadingPlan->clearPackets();
	for (int i=0;i<packetColumns.count();i++)
	{
		for (int j=0;j<packetColumns[i].getPacketsNo();j++)
		{
			int index=allPacketsVector.indexOf(packetColumns[i].getPacket(j));
			loadingPlan->loadPacket(index,i);
		}		
	}
	loadingPlan->calculate();
	qApp->processEvents();
}
void optimizerClass::newGeneration(int generation,double bestEver)
{
	//plot->calculateFitCurve(generation,bestEver);
	//plot->replot();
	//generationsLabel->setText(QString().number(generation));
	//ojectiveLabel->setText(QString().number(bestEver));
	setProgress(generation,fabs(bestEver));
	qApp->processEvents();
}

void optimizerClass::onMainTimer()
{

}
int dhPiramidObjective(int h0,int h1,int acceptable)
{
	int d=h0-h1;
	if (d>acceptable)
		return d;
	if (d>0)
		return 0.1*d;
	return 0;
}
int dhFlatObjective(int h0,int h1,int acceptable)
{
	int d=abs(h0-h1);
	if (d>acceptable)
		return d;
	return 0.1*d;
}
double optimizerClass::calculateObjective(int* packetsPos,bool updateQui)
{
	if (stopGaul)
		return -DBL_MAX;
	for (int i=0;i<packetColumns.size();i++)
	{
		packetColumns[i].clearPackets();
	}
	for (int i=0;i<allPacketsVector.size();i++)
	{
		packetColumns[packetsPos[i]].appendPacket(allPacketsVector[i]);
	}
	//colObj
	double colObj=0.;
		for (int i=0;i<packetColumns.size();i++)
		{
			packetColumns[i].optimizePackets();
			packetColumns[i].calculate();
			colObj+=packetColumns[i].calculateObjective();
		}
	//yObj
	double yObj=0.;
		for (int i=0;i<packetColumns.size();i++)
		{
			yObj+=fabs(packetColumns[i].getYmoment());
		}
	//xObj
	double xObj=0.;
		for (int i=0;i<packetColumns.size();i++)
		{
			xObj+=fabs(packetColumns[i].getXmoment());
		}
	//firstOrderObj
	double firstOrderObj=0.;
	if (allOrdersVector.count()>1)
	{
		//for (int i=0;i<5;i++)
		//{
		//	int n=packetColumns[i].getNoOfPacketsWithOrder(firstOrder);
		//	firstOrderObj-=n*10000.;
		//}
		for (int i=5;i<10;i++)
		{
			int n=packetColumns[i].getNoOfPacketsWithOrder(firstOrder);
			firstOrderObj+=n*1000.;
		}
	}
	//orderInOneGroupObj
	double orderInOneGroupObj=0.;
	if (allOrdersVector.count()>1)
	{
		for (int j=0;j<allOrdersVector.size();j++)
		{
			int ord=allOrdersVector[j];
			int noOfColsFor=0;
			for (int i=0;i<5;i++)
			{
				if (packetColumns[i].hasOrder(ord))
				{
					noOfColsFor++;
				}
			}
			int noOfColsAft=0;
			for (int i=5;i<10;i++)
			{
				if (packetColumns[i].hasOrder(ord))
				{
					noOfColsAft++;
				}
			}
			if (noOfColsFor>noOfColsAft)
				orderInOneGroupObj+=noOfColsAft*10000;
			else
				orderInOneGroupObj+=noOfColsFor*10000;
			//orderInOneGroupObj+=10000.;
		}
	}
	//piramidObj
	double piramidObj=0.;
		for (int i=0;i<allOrdersVector.size();i++)
		{
			int ord=allOrdersVector[i];
			int h0=packetColumns[0].getHeightOfOrder(ord);
			int h1=packetColumns[1].getHeightOfOrder(ord);
			int h2=packetColumns[2].getHeightOfOrder(ord);
			int h3=packetColumns[3].getHeightOfOrder(ord);
			int h4=packetColumns[4].getHeightOfOrder(ord);
								 
			int h5=packetColumns[5].getHeightOfOrder(ord);
			int h6=packetColumns[6].getHeightOfOrder(ord);
			int h7=packetColumns[7].getHeightOfOrder(ord);
			int h8=packetColumns[8].getHeightOfOrder(ord);
			int h9=packetColumns[9].getHeightOfOrder(ord);
			//piramid
			int dh=100;
			piramidObj+=dhPiramidObjective(h0,h1,dh);
			piramidObj+=dhPiramidObjective(h1,h2,dh);
			piramidObj+=dhPiramidObjective(h3,h2,dh);
			piramidObj+=dhPiramidObjective(h4,h3,dh);
										
			piramidObj+=dhPiramidObjective(h5,h6,dh);
			piramidObj+=dhPiramidObjective(h6,h7,dh);
			piramidObj+=dhPiramidObjective(h8,h7,dh);
			piramidObj+=dhPiramidObjective(h9,h8,dh);
			//empty row
			if ((h0>0)||(h1>0)||(h2>0)||(h3>0)||(h4>0))
			{
				if (h0==0)piramidObj+=1000.;
				if (h1==0)piramidObj+=1000.;
				if (h2==0)piramidObj+=1000.;
				if (h3==0)piramidObj+=1000.;
				if (h4==0)piramidObj+=1000.;
			}
			if ((h5>0)||(h6>0)||(h7>0)||(h8>0)||(h9>0))
			{
				if (h5==0)piramidObj+=1000.;
				if (h6==0)piramidObj+=1000.;
				if (h7==0)piramidObj+=1000.;
				if (h8==0)piramidObj+=1000.;
				if (h9==0)piramidObj+=1000.;
			}
			//flat
			dh=250;
			piramidObj+=dhFlatObjective(h0,h1,dh);
			piramidObj+=dhFlatObjective(h1,h2,dh);
			piramidObj+=dhFlatObjective(h3,h2,dh);
			piramidObj+=dhFlatObjective(h4,h3,dh);
										  
			piramidObj+=dhFlatObjective(h5,h6,dh);
			piramidObj+=dhFlatObjective(h6,h7,dh);
			piramidObj+=dhFlatObjective(h8,h7,dh);
			piramidObj+=dhFlatObjective(h9,h8,dh);
		}
	//dWObj
	double dWObj=0.;
		int weigthFor=packetColumns[0].getTotalWight()+packetColumns[1].getTotalWight()+packetColumns[2].getTotalWight()+packetColumns[3].getTotalWight()+packetColumns[4].getTotalWight();
		int weigthAft=packetColumns[5].getTotalWight()+packetColumns[6].getTotalWight()+packetColumns[7].getTotalWight()+packetColumns[8].getTotalWight()+packetColumns[9].getTotalWight();
		int maxWeightFor=loadingPlan->getForGroupOfOrders()->getMaxWeight();
		int maxWeightAft=loadingPlan->getAftGroupOfOrders()->getMaxWeight();
		if (weigthFor>maxWeightFor)
			dWObj+=10*(weigthFor-maxWeightFor);
		if (weigthAft>maxWeightAft)
			dWObj+=10*(weigthAft-maxWeightAft);
		//if (abs(weigthFor-weigthAft)>5000)
		//	dWObj+=abs(weigthFor-weigthAft)*0.1;
		yObj/=(weigthFor+weigthAft);
		yObj/=100.;
		xObj/=(weigthFor+weigthAft);
		xObj*=10.;
		//dWObj+=abs(abs(maxWeightFor-weigthFor)-abs(maxWeightAft-weigthAft));
	//all
	double obj=colObj+yObj+xObj+orderInOneGroupObj+firstOrderObj+piramidObj+dWObj;
	//updateQui
	if (updateQui)
	{
		//colObjLabel->setText(QString().number(colObj));
		//xObjLabel->setText(QString().number(xObj));
		//yObjLabel->setText(QString().number(yObj));
		//firstOrderObjLabel->setText(QString().number(firstOrderObj));
		//orderInOneGroupObjLabel->setText(QString().number(orderInOneGroupObj));
		//piramidObjLabel->setText(QString().number(piramidObj));
		//dWObjLabel->setText(QString().number(dWObj));
	}
	return obj;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////winGaul/////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" 
{
#include "gaul.h" 
}
double bestEver;
int generationsNo;
QVector<int> bestEverSolution;
boolean score(population *pop, entity *entity)
{
	entity->fitness=-optim->calculateObjective(&((int*)entity->chromosome[0])[0],false);
	if (bestEver<entity->fitness)
	{
		optim->calculateObjective(&((int*)entity->chromosome[0])[0],true);
		bestEver=entity->fitness;
		bestEverSolution.clear();
		bestEverSolution.fill(0,allPacketsVector.count());
		for (int i=0;i<allPacketsVector.size();i++)
			bestEverSolution[i]=((int*)entity->chromosome[0])[i];
		optim->newSolution(bestEverSolution,generationsNo,bestEver,false);
	}
	return optim->continueGaul();
}
boolean iterationCallback(int iteration, entity *solution)
{
	return optim->continueGaul();
}
boolean generationCallback(int generation, population *pop)
{
	optim->newGeneration(generation,bestEver);
	generationsNo=generation;
	return optim->continueGaul();
}
boolean seed(population *pop, entity *adam)
{
/* Checks. */
	if (!pop) die("Null pointer to population structure passed.");
	if (!adam) die("Null pointer to entity structure passed.");
/* Seeding. */
	for (int i=0;i<allPacketsVector.size();i++)
	{
		((int*)adam->chromosome[0])[i]=random_int_range(0,10);
	}
	return TRUE;
}
void mutate(population *pop, entity *father, entity *son)
{
	int r=random_int_range(1,3);
	if (r==1)
	{
		for (int i=1;i<allPacketsVector.size();i++)
		{
			((int*)son->chromosome[0])[i]=((int*)father->chromosome[0])[i-1];
		}
		((int*)son->chromosome[0])[0]=((int*)father->chromosome[0])[allPacketsVector.size()-1];
	}
	else if (r==2)
	{
		for (int i=0;i<allPacketsVector.size()-1;i++)
		{
			((int*)son->chromosome[0])[i]=((int*)father->chromosome[0])[i+1];
		}
		((int*)son->chromosome[0])[allPacketsVector.size()-1]=((int*)father->chromosome[0])[0];
	}
	else
	{
		for (int i=0;i<allPacketsVector.size()-1;i++)
		{
			((int*)son->chromosome[0])[i]=random_int_range(0,10);
		}
	}
}
void startOptim()
{
	bestEver=-DBL_MAX;
	generationsNo=0;
	bestEverSolution.clear();
	bestEverSolution.fill(0,allPacketsVector.count());
	////
	optim->setMaxProgress(MAX_GENERATIONS);
	population		*pop[NO_OF_ISLANDS];			/* Population of solutions. */
	//entity			*solution;			/* Optimised solution. */
	random_seed(QDateTime::currentDateTime().toTime_t());
	for (int i=0;i<NO_OF_ISLANDS;i++)
	{
		pop[i] = ga_genesis_integer
		(
		   allPacketsVector.count()*NO_OF_PACKETS_POP_MULTIPLIER,	/* const int              population_size */
		   1,						/* const int              num_chromo */
		   allPacketsVector.count(),			/* const int              len_chromo */
		   generationCallback,		/* GAgeneration_hook      generation_hook */
		   iterationCallback,		/* GAiteration_hook       iteration_hook */
		   NULL,					/* GAdata_destructor      data_destructor */
		   NULL,					/* GAdata_ref_incrementor data_ref_incrementor */
		   score,					/* GAevaluate             evaluate */
		   seed,					/* GAseed                 seed */
		   NULL,					/* GAadapt                adapt */
		   ga_select_one_bestof2,	/* GAselect_one           select_one */
		   ga_select_two_bestof2,	/* GAselect_two           select_two */
		   mutate,					/* GAmutate	mutate */
		   ga_crossover_integer_doublepoints,	/* GAcrossover            crossover */
		   NULL,			/* GAreplace              replace */
		   NULL			/* vpointer	User data */
		);
		ga_population_set_parameters(
		   pop[i],				/* population      *pop */
		   GA_SCHEME_DARWIN,		/* const ga_scheme_type     scheme */
		   GA_ELITISM_PARENTS_SURVIVE,		/* const ga_elitism_type   elitism */
		   CROSSOVER,				/* double  crossover */
		   MUTATION,				/* double  mutation */
		   MIGRATION      			/* double  migration */
			);
	}
	ga_evolution_archipelago(
		NO_OF_ISLANDS,
		pop,				/* population	*pop */
		MAX_GENERATIONS		/* const int	max_generations */
		);
	for (int i=0;i<NO_OF_ISLANDS;i++)
	{
		ga_extinction(pop[i]);
	}
}

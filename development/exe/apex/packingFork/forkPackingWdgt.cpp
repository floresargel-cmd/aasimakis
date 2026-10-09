#include "pch.h"
#include "forkPackingWdgt.h"
forkPackingWdgtClass::forkPackingWdgtClass(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,QWidget *p):QWidget(p)
{
	craneTuxip=craneTuxip_;
	forkTuxip=forkTuxip_;
	QVBoxLayout *mainL=new QVBoxLayout();mainL->setMargin(2);QWidget::setLayout(mainL);
	splash->showMessage(tr("Initializing fork Widget..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	layoutGraphicsView=new myQGraphicsViewClass(true,this);mainL->addWidget(layoutGraphicsView);
	layoutScene=new QGraphicsScene(this);

	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr("Zoom extents"),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(zoomExtentsBtnClickedSlot()));
#ifdef packetWareHouseExe
	//QPushButton *hmiBtn=new QPushButton(QIcon(":/gear.png"),tr("HMI"),layoutGraphicsView);
	//hmiBtn->move(155,5);
	//hmiBtn->setIconSize(QSize(16,16));
	//connect(hmiBtn,SIGNAL(clicked()),this,SLOT(hmiBtnClickedSlot()));
#endif
	gItemClass *layoutGraphicsItem=new gItemClass(":/dxfs/packingFork/layout.dxf",QPen(QColor(240,240,240)),false,false,QColor(),this);
	layoutGraphicsView->setScene(layoutScene);
	layoutGraphicsItem->setZValue(-10.);layoutScene->addItem(layoutGraphicsItem);
	layoutGraphicsView->scale(0.05,0.05);
#ifdef packingForkExe
	initializeChildren(craneTuxip,forkTuxip);
#endif
}
allPosPackingForkClass* forkPackingWdgtClass::getAllPosPtr()
{
	return allPos;
}
forkPackingClass* forkPackingWdgtClass::getForkPtr()
{
	return fork;
}
void forkPackingWdgtClass::initializeChildren(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_)
{
	craneTuxip=craneTuxip_;
	forkTuxip=forkTuxip_;
	splash->showMessage(tr("Creating postions..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	allPos=new allPosPackingForkClass(craneTuxip,forkTuxip,layoutScene,this);
	splash->showMessage(tr("Creating Fork Packing..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	fork=new forkPackingClass(craneTuxip,forkTuxip,allPos,layoutScene,this);
	hmiDlg=new hmiDlgForkPackingClass(forkTuxip,fork);
	allPos->setForkClass(fork);
	connect(fork,SIGNAL(returnEmptyCassetteFromNorthPackingConveyor()),this,SLOT(returnEmptyCassetteFromNorthConveyorSlot()));
}
void forkPackingWdgtClass::showHmiDlg()
{
	hmiDlg->show();
}
void forkPackingWdgtClass::hmiBtnClickedSlot()
{
	showHmiDlg();
}
void forkPackingWdgtClass::zoomExtentsBtnClickedSlot()
{
	layoutScene->setSceneRect(layoutScene->itemsBoundingRect());
	layoutGraphicsView->fitInView(layoutGraphicsView->sceneRect(),Qt::KeepAspectRatio);
}
void forkPackingWdgtClass::returnEmptyCassetteFromNorthConveyorSlot()
{
	returnEmptyCassetteFromNorthConveyor();
}
void forkPackingWdgtClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane3Ip)
	{
		if (allPos)allPos->dataInPlcChanged(ip,table,index,value);
		if (fork)fork->dataInPlcChanged(ip,table,index,value);
		if (hmiDlg)hmiDlg->dataInPlcChanged(ip,table,index,value);
	}
}
bool forkPackingWdgtClass::getIsForkIdle()
{
	return fork->getIsIdle();
}
bool forkPackingWdgtClass::getIsAboutToBeIdle()
{
	return fork->getIsAboutToBeIdle();
}
bool forkPackingWdgtClass::sendMissionToFork(int from,int to)
{
	return fork->sendMission(from,to);
}
void forkPackingWdgtClass::returnEmptyCassetteFromNorthConveyor()
{
	craneTuxip->writeInteger("pcCom",30,4542);
	craneTuxip->writeInteger("pcCom",30,0);
}
void forkPackingWdgtClass::returnFullCassetteFromNorthConveyor()
{
	craneTuxip->writeInteger("pcCom",30,45420);
	craneTuxip->writeInteger("pcCom",30,0);
}

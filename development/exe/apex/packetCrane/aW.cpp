#include "pch.h"
#include "aW.h"
extern QSplashScreen *splash;
allWidgetClass::allWidgetClass(QWidget *parent):QWidget(parent)
{
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);QWidget::setLayout(mainLayout);
	layoutGraphicsView=new myQGraphicsViewClass(true,this);mainLayout->addWidget(layoutGraphicsView);
	layoutGraphicsView->scale(0.015,0.015);
	QGraphicsScene *layoutScene=new QGraphicsScene(this);
	layoutGraphicsView->setScene(layoutScene);

	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr("Zoom extents"),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoomSlot()));

	dxfClass *layoutGraphicsItem=new dxfClass(":dxfs/layout.dxf",QPen(QColor(200,200,200)),false,false,QColor(),this);
	layoutGraphicsItem->setZValue(-10.);layoutScene->addItem(layoutGraphicsItem);
	//
	crane=new craneClass(layoutScene,this);
	fork=new forkClass(layoutScene,this);
	allPosisions=new allPosisionsClass(layoutScene,this);
	splash->showMessage(tr("Loading from database..."), Qt::AlignRight | Qt::AlignBottom, QColor(150, 150, 200));
	allPosisions->setConveyorsPositions(crane->getCranePosition(),crane->getConveyorPosition24(),crane->getConveyorPosition25(),crane->getConveyorStationImport()
		,crane->getStationExit1(),crane->getStationImport(),crane->getStationExit2()
		,crane->getStationBackExit1(),crane->getStationBackImport(),crane->getStationBackExit2()
		,crane->getPackingExit1(),crane->getPackingImport(),crane->getPackingExit2()
		,crane->getPackingBackExit1(),crane->getPackingBackImport(),crane->getPackingBackExit2()
	);
	crane->setAllPositionsClass(allPosisions);
	fork->setPosition24(crane->getConveyorPosition24());
	fork->setPosition25(crane->getConveyorPosition25());
	//
	forkPackingWdgt=new forkPackingWdgtClass(nullptr,nullptr,parent);
	forkPackingWdgtDlg=new QDialog(this);QVBoxLayout *forkPackingWdgtDlgL=new QVBoxLayout();forkPackingWdgtDlg->resize(800,1300);forkPackingWdgtDlg->setWindowTitle(tr("Packing Fork"));forkPackingWdgtDlgL->setMargin(2);forkPackingWdgtDlg->setLayout(forkPackingWdgtDlgL);
	forkPackingWdgtDlgL->addWidget(forkPackingWdgt);
	//
	splash->showMessage(tr("Loading from manager..."), Qt::AlignRight | Qt::AlignBottom, QColor(150, 150, 200));
	manager=new managerClass(layoutScene,allPosisions,crane,crane->getCranePosition(),crane->getConveyorPosition24(),crane->getConveyorPosition25(),fork,fork->getForkPosition()
#ifdef keepPacketCraneOldFunctionality
		,fork->getPositionSb()
#endif
		,crane->getStationExit1(),crane->getStationImport(),crane->getStationExit2()
		,crane->getPackingExit1(),crane->getPackingImport(),crane->getPackingExit2(),crane->getPackingBackExit1()
#ifdef keepPacketCraneOldFunctionality
		,fork->getPositionPa()
#endif
		,fork->getPositionIm(),forkPackingWdgt,this);
#ifdef keepPacketCraneOldFunctionality
	fork->setPositionSbExit(manager->getPositionSbExit());
	fork->setPositionPacketExit(manager->getPositionPacketExit());
	fork->setPositionStoreExit(manager->getPositionStoreExit());
#endif

	splash->showMessage(tr("Loading from crane..."), Qt::AlignRight | Qt::AlignBottom, QColor(150, 150, 200));
	crane->loadFromDatabase();
	splash->showMessage(tr("Loading from fork..."), Qt::AlignRight | Qt::AlignBottom, QColor(150, 150, 200));
	fork->loadFromDatabase();
	manualMissionFromPos=NULL;
	connect(allPosisions,SIGNAL(mousePressedOnPacketPosSignal(packetPositionClass*,QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedOnPacketPosSlot(packetPositionClass*,QGraphicsSceneMouseEvent*)));
	connect(crane->getConveyorPosition24(),SIGNAL(mouseReleasedSignal(packetPositionClass*,QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedOnPacketPosSlot(packetPositionClass*,QGraphicsSceneMouseEvent*)));
	connect(crane->getConveyorPosition25(),SIGNAL(mouseReleasedSignal(packetPositionClass*,QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedOnPacketPosSlot(packetPositionClass*,QGraphicsSceneMouseEvent*)));
	connect(fork->getPositionIm(),SIGNAL(mouseReleasedSignal(packetPositionClass*,QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedOnPacketPosSlot(packetPositionClass*,QGraphicsSceneMouseEvent*)));
#ifdef keepPacketCraneOldFunctionality
	connect(fork->getPositionSb(),SIGNAL(mouseReleasedSignal(packetPositionClass*,QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedOnPacketPosSlot(packetPositionClass*,QGraphicsSceneMouseEvent*)));
	connect(fork->getPositionPa(),SIGNAL(mouseReleasedSignal(packetPositionClass*,QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedOnPacketPosSlot(packetPositionClass*,QGraphicsSceneMouseEvent*)));
	connect(manager->getPositionSbExit(),SIGNAL(mouseReleasedSignal(packetPositionClass*,QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedOnPacketPosSlot(packetPositionClass*,QGraphicsSceneMouseEvent*)));
	connect(manager->getPositionPacketExit(),SIGNAL(mouseReleasedSignal(packetPositionClass*,QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedOnPacketPosSlot(packetPositionClass*,QGraphicsSceneMouseEvent*)));
	connect(manager->getPositionStoreExit(),SIGNAL(mouseReleasedSignal(packetPositionClass*,QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedOnPacketPosSlot(packetPositionClass*,QGraphicsSceneMouseEvent*)));
#endif
	splash->showMessage(tr("Loading gui..."), Qt::AlignRight | Qt::AlignBottom, QColor(150, 150, 200));
}
allWidgetClass::~allWidgetClass()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
{
}
void allWidgetClass::resetGraphicsViewZoomSlot()
{
	layoutGraphicsView->scene()->setSceneRect(layoutGraphicsView->scene()->itemsBoundingRect());
	layoutGraphicsView->fitInView(layoutGraphicsView->sceneRect(),Qt::KeepAspectRatio);
}
#ifdef keepPacketCraneOldFunctionality
void allWidgetClass::enableAutoExportsMissions(bool enable)
{
	manager->enableAutoExportsMissions(enable);
}
#endif
void allWidgetClass::enableAutoImportsMissions(bool enable)
{
	manager->enableAutoImportsMissions(enable);
}
void allWidgetClass::enableAutoExportsStation1Missions(bool enable)
{
	manager->enableAutoExportsStation1Missions(enable);
}
void allWidgetClass::enableAutoExportsStation2Missions(bool enable)
{
	manager->enableAutoExportsStation2Missions(enable);
}
void allWidgetClass::enableAutoImportsStationMissions(bool enable)
{
	manager->enableAutoImportsStationMissions(enable);
}
void allWidgetClass::enableAutoWestStationMissions(bool enable)
{
	manager->enableAutoPackingMissions(enable);
}
void allWidgetClass::mousePressedOnPacketPosSlot(packetPositionClass *packetPos,QGraphicsSceneMouseEvent *ev)
{
	if (packetPos->getIsLocked())
		return;
	if (!manualMissionFromPos)
	{
		if (  ((packetPos->getPacketId()>0)&&(packetPos->getPosId()!=POS_ID_FORK)
#ifdef keepPacketCraneOldFunctionality
			&&(packetPos->getPosId()!=POS_ID_SBEXIT)&&(packetPos->getPosId()!=POS_ID_PACKETEXIT)
#endif
			&&(packetPos->getPosId()!=POS_ID_CRANE)
#ifdef keepPacketCraneOldFunctionality
			&&(packetPos->getPosId()!=POS_ID_STOREEXIT)
#endif
			)  
			||
			 (packetPos->getPacketId()<MAX_CELL_POS_ID) 
			)
		{
			manualMissionFromPos=packetPos;
			manualMissionFromPos->setIsSelected(true);
		}
	}
	else
	{
		if ((manualMissionFromPos==packetPos)||(packetPos->getPacketId()>0))
		{
			manualMissionFromPos->setIsSelected(false);
			manualMissionFromPos=NULL;
			return;
		}
		//if (plc->isConectionsEstablished()&&(!manager->getAreAutoMissionsEnabled())&&fork->isIdle()&&crane->isIdle())
		{
			if (  
#ifdef tempForMove
				( (manualMissionFromPos->getPosId()<MAX_CELL_POS_ID) && (packetPos->getPosId()<MAX_CELL_POS_ID) )
				||
#endif
				( (manualMissionFromPos->getPosId()<MAX_CELL_POS_ID) && ((packetPos->getPosId()==POS_ID_WESTCONV)||(packetPos->getPosId()==POS_ID_EASTCONV)) )
				||
				(((manualMissionFromPos->getPosId()==POS_ID_WESTCONV)||(manualMissionFromPos->getPosId()==POS_ID_EASTCONV)) &&  (packetPos->getPosId()<MAX_CELL_POS_ID) )
				)
				crane->newMission(manualMissionFromPos->getPosId(),packetPos->getPosId(),false,true);
			else if (
				  (  
#ifdef keepPacketCraneOldFunctionality
						(manualMissionFromPos->getPosId()==POS_ID_NEWSB)||(manualMissionFromPos->getPosId()==POS_ID_NEWPACKET)||
#endif
						(manualMissionFromPos->getPosId()==POS_ID_SB_PACKET_IMPORT)||(manualMissionFromPos->getPosId()==POS_ID_WESTCONV)||(manualMissionFromPos->getPosId()==POS_ID_EASTCONV)  )
					&&
				 (  
#ifdef keepPacketCraneOldFunctionality
					 (packetPos->getPosId()==POS_ID_SBEXIT)||(packetPos->getPosId()==POS_ID_PACKETEXIT)||(packetPos->getPosId()==POS_ID_STOREEXIT)||
#endif
					 (packetPos->getPosId()==POS_ID_WESTCONV)||(packetPos->getPosId()==POS_ID_EASTCONV)  )
				)
				fork->newMission(manualMissionFromPos,packetPos,false);
		}
		manualMissionFromPos->setIsSelected(false);
		manualMissionFromPos=NULL;
	}
}
craneClass* allWidgetClass::getCraneClass()
{
	return crane;
}
forkClass* allWidgetClass::getForkClass()
{
	return fork;
}
forkPackingWdgtClass* allWidgetClass::getForkPackingWdgtClass()
{
	return forkPackingWdgt;
}
allPosisionsClass* allWidgetClass::getAllPosClass()
{
	return allPosisions;
}
void allWidgetClass::startTimers()
{
	manager->startTimers();
}
#ifdef keepPacketCraneOldFunctionality
void allWidgetClass::setExportsDestination(managerClass::exportsDestinationEnum e)
{
	manager->setExportsDestination(e);
}
#endif
void allWidgetClass::setPlcClass(plcClass *plc_)
{
	plc=plc_;
	manager->setPlcClass(plc);
	allPosisions->setPlcClass(plc);
	crane->setPlcClass(plc);
	fork->setPlcClass(plc);
	forkPackingWdgt->initializeChildren(plc->getCraneTuxClass(),plc->getCrane3TuxClass());
}
float allWidgetClass::getPacketCraneX()
{
	return crane->getPlcX();
}
float allWidgetClass::getPacketCraneY()
{
	return crane->getPlcY();
}
float allWidgetClass::getPacketCraneZ()
{
	return crane->getPlcZ();
}
float allWidgetClass::getForkCraneX()
{
	return fork->getPlcX();
}
float allWidgetClass::getForkCraneY()
{
	return fork->getPlcY();
}
float allWidgetClass::getForkCraneZ()
{
	return fork->getPlcZ();
}
void allWidgetClass::signalFromCranePlc(QString table,int index,QString value)
{
	if (table=="N150")
	{
		if (index==2)
			crane->plcStatusChanged(value.toInt());
		else if (index==10)
			crane->plcCassetteOnCraneChanged(value.toInt());
	}
	crane->dataInPlcChanged(packetCraneIp,table,index,value);
	if (table=="N152")
	{
		if (index==0)
		{
			QVector<bool> v=deprecatedCalculateBits(value.toInt());
			if (v[31])
				crane->plcCraneIsCommingOutWithCassette();
		}
	}
	else if (table=="F151")
	{
		if (index==0)
			crane->setPlcX(value.toFloat()/1000.);
		else if (index==1)
			crane->setPlcZ(value.toFloat()/1000.);
		else if (index==2)
			crane->setPlcY(value.toFloat()/1000.);
	}
}
void allWidgetClass::signalFromForkPlc(QString table,int index,QString value)
{
	if (table=="N160")
	{
		if (index==2)
			fork->plcStatusChanged(value.toInt());
	}
	else if (table=="F161")
	{
		if (index==0)
			fork->setPlcY(value.toFloat()/1000.);
		else if (index==1)
			fork->setPlcZ(-value.toFloat()/1000.);
		else if (index==2)
			fork->setPlcX(value.toFloat()/1000.);
	}
}
void allWidgetClass::signalFromCrane3Plc(QString table,int index,QVariant value)
{
	forkPackingWdgt->dataInPlcChanged(crane3Ip,table,index,value);
	crane->dataInPlcChanged(crane3Ip,table,index,value);
}
int allWidgetClass::getType1EmptyCassettes()
{
	return allPosisions->getType1EmptyCassettes();
}
int allWidgetClass::getType2EmptyCassettes()
{
	return allPosisions->getType2EmptyCassettes();
}
int allWidgetClass::getType3EmptyCassettes()
{
	return allPosisions->getType3EmptyCassettes();
}
void allWidgetClass::showForkPackingWdgt()
{
	forkPackingWdgtDlg->exec();
}
void allWidgetClass::newScanForInOutStation(int ticket)
{
	forkPackingWdgtDlg->exec();
}

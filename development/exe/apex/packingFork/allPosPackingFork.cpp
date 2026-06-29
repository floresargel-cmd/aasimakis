#include "pch.h"
#include "allPosPackingFork.h"
allPosPackingForkClass::allPosPackingForkClass(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,QGraphicsScene *layoutScene,QWidget *p):QWidget{p}
{
	craneTuxip=craneTuxip_;
	forkTuxip=forkTuxip_;
	setVisible(false);
	gItemClass *posItem=new gItemClass(":dxfs/packingFork/packetRect.dxf",QPen(QColor(200,200,250)),false,false,QColor(),this);
	gItemClass *mouseOverItem=new gItemClass(":dxfs/packingFork/packetRect.dxf",QPen(QColor(255,100,100)),true,true,QColor(255,100,100,150),this);
	gItemClass *packetItem=new gItemClass(":dxfs/packingFork/packet.dxf",QPen(QColor(150,150,100)),false,false,QColor(),this);packetItem->setFillBrush(QBrush(QColor(150,150,100,50)));
	gItemClass *posFrom=new gItemClass(":dxfs/packingFork/posFrom.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);
	gItemClass *posTo=new gItemClass(":dxfs/packingFork/posTo.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);
	manualSelectedFromPos=nullptr;
	splash->showMessage(tr("Loading position data..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	posV<<new posPackingForkClass(positionInputOutputPosId,craneTuxip,forkTuxip,inputOutputPlcId,515.f,8770.f,2000.,0.,tr("Input/Output"),layoutScene,posItem,mouseOverItem,packetItem,posFrom,posTo,this);
	posV<<new posPackingForkClass(0,craneTuxip,forkTuxip,packetFormingStation1PlcId,1172.f,2385.f,2000.,0.,tr("Forming Packet Station 1"),layoutScene,posItem,mouseOverItem,packetItem,posFrom,posTo,this);
	posV<<new posPackingForkClass(0,craneTuxip,forkTuxip,packetFormingStation2PlcId,10683.f,2385.f,2000.,0.,tr("Forming Packet Station 2"),layoutScene,posItem,mouseOverItem,packetItem,posFrom,posTo,this);
	posV<<new posPackingForkClass(0,craneTuxip,forkTuxip,strappingOldInPlcId,515.f,6530.f,2000.,0.,tr("Strapping Old In"),layoutScene,posItem,mouseOverItem,packetItem,posFrom,posTo,this);
	posV<<new posPackingForkClass(packingOldStrappingExitPosId,craneTuxip,forkTuxip,strappingOldOutPlcId,11280.f,6530.f,2000.,0.,tr("Strapping Old Out"),layoutScene,posItem,mouseOverItem,packetItem,posFrom,posTo,this);
	posV<<new posPackingForkClass(0,craneTuxip,forkTuxip,strappingNewInPlcId,9240.f,9600.f,2000.,0.,tr("Strapping New In"),layoutScene,posItem,mouseOverItem,packetItem,posFrom,posTo,this);
	posV<<new posPackingForkClass(packingNewStrappingExitPosId,craneTuxip,forkTuxip,strappingNewOutPlcId,20100.f,9600.f,2000.,0.,tr("Strapping New Out"),layoutScene,posItem,mouseOverItem,packetItem,posFrom,posTo,this);
	posV<<new posPackingForkClass(packingBackExport1PlcId,craneTuxip,forkTuxip,warehouseConveyorPlcId,23570.f,1737.f,2000.,90.,tr("Warehouse Conveyor"),layoutScene,posItem,mouseOverItem,packetItem,posFrom,posTo,this);

	for (int i=0;i<posV.count();i++)
		connect(posV[i],SIGNAL(posMouseReleasedSignal(posPackingForkClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posMouseReleasedSlot(posPackingForkClass*,QGraphicsSceneMouseEvent*)));

	QList <QGraphicsView*> gv=layoutScene->views();
	if (gv.count()!=0)
		connect(gv[0],SIGNAL(keyPressedSignal(QKeyEvent*)),this,SLOT(graphicsViewKeyPressedSlot(QKeyEvent*)));

	safeToRotateEllipseItem=layoutScene->addEllipse(0,0,10,10,QPen(QColor(100,200,100)),Qt::NoBrush);
	rotatedLimitsPolygon=layoutScene->addPolygon(QPolygonF(1),Qt::NoPen,QBrush(QColor(100,200,100,75)));

	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerTriggeredSlot()));
	mainTimer->start(5000);
}
void allPosPackingForkClass::mainTimerTriggeredSlot()
{
}
void allPosPackingForkClass::setForkClass(forkPackingClass *f)
{
	fork=f;
}
posPackingForkClass *allPosPackingForkClass::getPosWithPlcId(int plcId)
{
	for (int i=0;i<posV.count();i++)
	{
		if (posV[i]->getPlcId()==plcId)
			return posV[i];
	}
	return NULL;
}
void allPosPackingForkClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane3Ip)
	{
		for (int i=0;i<posV.count();i++)
			posV[i]->dataInPlcChanged(ip,table,index,value);
		if (table=="fc1ToPcF")
		{
			if ((index>=30)&&(index<=33))
			{
				float x0=forkTuxip->readFloatValue("fc1ToPcF",30);
				float dx=forkTuxip->readFloatValue("fc1ToPcF",31);
				float y0=forkTuxip->readFloatValue("fc1ToPcF",32);
				float dy=forkTuxip->readFloatValue("fc1ToPcF",33);
				safeToRotateEllipseItem->setRect(-y0-dy,-x0-dx,2*dy,2*dx);
			}
			if (index==45)
			{
				QPolygonF polyF;
				polyF<<QPointF(-forkTuxip->readFloatValue("fc1ToPcF",37),-forkTuxip->readFloatValue("fc1ToPcF",36));
				polyF<<QPointF(-forkTuxip->readFloatValue("fc1ToPcF",35),-forkTuxip->readFloatValue("fc1ToPcF",34));
				polyF<<QPointF(-forkTuxip->readFloatValue("fc1ToPcF",39),-forkTuxip->readFloatValue("fc1ToPcF",38));
				polyF<<QPointF(-forkTuxip->readFloatValue("fc1ToPcF",41),-forkTuxip->readFloatValue("fc1ToPcF",40));
				polyF<<QPointF(-forkTuxip->readFloatValue("fc1ToPcF",45),-forkTuxip->readFloatValue("fc1ToPcF",44));
				polyF<<QPointF(-forkTuxip->readFloatValue("fc1ToPcF",45),-forkTuxip->readFloatValue("fc1ToPcF",36));
				polyF<<QPointF(-forkTuxip->readFloatValue("fc1ToPcF",37),-forkTuxip->readFloatValue("fc1ToPcF",36));
				rotatedLimitsPolygon->setPolygon(polyF);
			}
		}
	}
}
void allPosPackingForkClass::graphicsViewKeyPressedSlot(QKeyEvent *ev)
{
	if (ev->key()==Qt::Key_Escape)
	{
		if (manualSelectedFromPos)
		{
			manualSelectedFromPos->setIsFrom(false);
			manualSelectedFromPos=nullptr;
		}
	}
}
void allPosPackingForkClass::posMouseReleasedSlot(posPackingForkClass *p,QGraphicsSceneMouseEvent *ev)
{
#ifndef packingForkExe
	return;
#endif
	if (!fork->getIsIdle())
			return;
	if (!manualSelectedFromPos)
	{
		if (p->getPlcPacketTicket()>0)
		{
			manualSelectedFromPos=p;
			manualSelectedFromPos->setIsFrom(true);
		}
	}
	else
	{
		if ((manualSelectedFromPos==p)||(p->getPlcPacketTicket()>0))
		{
			manualSelectedFromPos->setIsFrom(false);
			manualSelectedFromPos=nullptr;
			return;
		}
		fork->sendMission(manualSelectedFromPos,p);
		manualSelectedFromPos->setIsFrom(false);
		manualSelectedFromPos=nullptr;
	}

}

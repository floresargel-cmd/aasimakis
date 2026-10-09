#include "pch.h"
#include "cPos.h"
convPositionClass::convPositionClass(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,QGraphicsScene *myScene_,
		int globalId_,QString description_,QString plcBasketNumberIp_,QString plcBasketNumberTable_,int plcBasketNumberIndex_,
		dxfClass *mouseOverGraphicsItem_,dxfClass *noBasketGraphicsItem_,dxfClass *emptyBasketGraphicsItem_,dxfClass *fullBasketGraphicsItem_,
		float dx,float dy,
		allConvPositionsClass *all_):QWidget((QWidget*)all_)
{
	craneTuxip=craneTuxip_;
	forkTuxip=forkTuxip_;
	myScene=myScene_;
	setVisible(false);
	globalId=globalId_;
	description=description_;
	gXPos=dx;
	gYPos=dy;
	angle=0.f;
	rotateXoffset=0.f;
	rotateYoffset=0.f;
	inMaintenanceMode=false;
	plcBasketNumberIp=plcBasketNumberIp_;
	plcBasketNumberTable=plcBasketNumberTable_;
	plcBasketNumberIndex=plcBasketNumberIndex_;
	allConvPositions=all_;

	mouseOverGraphicsItem=new dxfClass(mouseOverGraphicsItem_,gXPos,gYPos,this);
	noBasketGraphicsItem=new dxfClass(noBasketGraphicsItem_,gXPos,gYPos,this);
	emptyBasketGraphicsItem=new dxfClass(emptyBasketGraphicsItem_,gXPos,gYPos,this);
	fullBasketGraphicsItem=new dxfClass(fullBasketGraphicsItem_,gXPos,gYPos,this);
	fullBasketGraphicsItem->setVisible(false);
	myScene->addItem(noBasketGraphicsItem);
	myScene->addItem(emptyBasketGraphicsItem);
	myScene->addItem(fullBasketGraphicsItem);
	myScene->addItem(mouseOverGraphicsItem);
	connect(mouseOverGraphicsItem,SIGNAL(hoverEnterSignal()),this,SLOT(hoverEnterSlot()));
	connect(mouseOverGraphicsItem,SIGNAL(hoverLeaveSignal()),this,SLOT(hoverLeaveSlot()));
	connect(mouseOverGraphicsItem,SIGNAL(mousePressedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedSlot(QGraphicsSceneMouseEvent*)));
	QTransform  txtTransformation;
	txtTransformation.scale(1,-1);
	txtTransformation.translate(gXPos,-gYPos);
	graphicsText=new myGraphicsTextItem(QString("%1").arg(globalId),mouseOverGraphicsItem->boundingRect(),QFont("Arial",200),QColor(50,50,150),myScene);
	graphicsText->setTransform(txtTransformation);
	basketNumber=0;
	updateGui();
	posDlg=new cPosDlgClass(globalId,plcBasketNumberIndex,basketNumber,description,allConvPositions,!((globalId>=packingExport2PlcId)&&(globalId<=packingBackExport1PlcId)));
	setMaintenanceMode(false);
}
convPositionClass::~convPositionClass()
{
}
void convPositionClass::addMoveClass(moveClass* m)
{
	movesVector<<m;
	m->setMaintenanceMode(inMaintenanceMode);
}
int convPositionClass::getGlobalId()
{
	return globalId;
}
float convPositionClass::getGX()
{
	return gXPos;
}
float convPositionClass::getGY()
{
	return gYPos;
}
void convPositionClass::addProximitySwitch(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset,float width,QString description)
{
	gProximitySwitchClass *ps=new gProximitySwitchClass(plcIpAdress_,plcTable_,plcIndex_,plcBit_,txt,offset,myScene,width,description);
	QTransform  transformation;
	transformation.translate(gXPos,gYPos);
	ps->setTransform(transformation);
	proximitySwitchVector<<ps;
}
void convPositionClass::addButton(QString plcIpAdress_,QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,QString plcActiveTable_,int plcActiveIndex_,int plcActiveNumber_,float offset,QString label,int type)
{
	buttonClass *btn=new buttonClass(craneTuxip,plcIpAdress_,plcCommandTable_,plcCommandIndex_,plcCommandNumber_,plcActiveTable_,plcActiveIndex_,plcActiveNumber_,offset,myScene,label,type);
	QTransform  transformation;
	transformation.translate(gXPos,gYPos);
	btn->setTransform(transformation);
	buttonsVector<<btn;
}
void convPositionClass::setMaintenanceMode(bool m)
{
	inMaintenanceMode=m;
	for (int i=0;i<buttonsVector.count();i++)
		buttonsVector[i]->setMaintenanceMode(inMaintenanceMode);
	for (int i=0;i<movesVector.count();i++)
		movesVector[i]->setMaintenanceMode(inMaintenanceMode);
	updateGui();
}
void convPositionClass::myRotate(float rotateXoffset_,float rotateYoffset_,float angle_)
{
	rotateXoffset=rotateXoffset_;
	rotateYoffset=rotateYoffset_;
	angle=angle_;
	QTransform  transformation;
	transformation.rotate(angle);
	transformation.translate(rotateXoffset+gXPos*qCos(-uPi*angle/180)-gYPos*qSin(-uPi*angle/180),rotateYoffset+gXPos*qSin(-uPi*angle/180)+gYPos*qCos(-uPi*angle/180));
	for (int i=0;i<proximitySwitchVector.count();i++)
		proximitySwitchVector[i]->setTransform(transformation);
	for (int i=0;i<buttonsVector.count();i++)
		buttonsVector[i]->setTransform(transformation);
	mouseOverGraphicsItem->setTransform(transformation);
	noBasketGraphicsItem->setTransform(transformation);
	emptyBasketGraphicsItem->setTransform(transformation);
	fullBasketGraphicsItem->setTransform(transformation);
}
void convPositionClass::setPos(float x,float y,bool transformText)
{
	gXPos=x;
	gYPos=y;
	if (transformText)
	{
		QTransform  txtTransformation;
		txtTransformation.scale(1,-1);
		txtTransformation.translate(gXPos,-gYPos);
		graphicsText->setTransform(txtTransformation);
	}
	myRotate(rotateXoffset,rotateYoffset,angle);
}
void convPositionClass::hoverEnterSlot()
{
}
void convPositionClass::hoverLeaveSlot()
{
}
void convPositionClass::setBasketId(int b,bool user,QString logInfo)
{
	craneTuxip->writeInteger(plcBasketNumberTable,plcBasketNumberIndex,b);
}
void convPositionClass::mousePressedSlot(QGraphicsSceneMouseEvent *ev)
{
	if (ev->button()==Qt::LeftButton)
	{
		posDlg->setBasketNum(basketNumber);
		if (posDlg->exec())
			setBasketId(posDlg->getBasketNum(),true,"user");
	}
	if ((globalId>=packingExport2PlcId)&&(globalId<=packingBackExport1PlcId))
	{
		if (ev->button()==Qt::RightButton)
		{
			cellContentsClass *cellContents=new cellContentsClass(globalId,this);
			if (cellContents->exec())
			{
				setBasketId(cellContents->getBasketNum(),true,"user");
				craneTuxip->writeInteger("ConveyorData",plcBasketNumberIndex,cellContents->getPacketId());
				if (globalId==packingBackExport1PlcId)
					forkTuxip->writeInteger("packetId",8,cellContents->getPacketId());
				updateGui();
			}
		}
	}
}
bool convPositionClass::getHasBasket()
{
	return basketNumber!=0;
}
int convPositionClass::getBasket()
{
	return basketNumber;
}
QString convPositionClass::getDescription()
{
	return description;
}
void convPositionClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((plcBasketNumberIp==ip)&&(plcBasketNumberTable==table)&&(plcBasketNumberIndex==index))
	{
		basketNumber=value.toInt();
		updateGui();
	}
	for (int i=0;i<proximitySwitchVector.count();i++)
		proximitySwitchVector[i]->dataInPlcChanged(ip,table,index,value);
	for (int i=0;i<buttonsVector.count();i++)
		buttonsVector[i]->dataInPlcChanged(ip,table,index,value);
	posDlg->dataInPlcChanged(ip,table,index,value);
}

void convPositionClass::updateGui()
{
	QString toolTipTxt;
	if (basketNumber!=0)
	{
		graphicsText->setText(QString("%1").arg(basketNumber),QColor(100,100,255));
		toolTipTxt+="<font style='color:#333333'>"+tr("Basket number:")+"</font><font style='color:#000077'>"+QString("%1").arg(basketNumber)+"</font><br>";
		noBasketGraphicsItem->setVisible(false);
		bool hasContents=getFirst(QString("select packetId from positions where cassette=%1").arg(basketNumber),MAIN_DB_CONNECTION).toInt()>0;
		if (hasContents)
		{
			emptyBasketGraphicsItem->setVisible(false);
			fullBasketGraphicsItem->setVisible(true);
		}
		else
		{
			emptyBasketGraphicsItem->setVisible(true);
			fullBasketGraphicsItem->setVisible(false);
		}
	}
	else
	{
		graphicsText->setText(QString("%1").arg(globalId),QColor(225,225,225));
		noBasketGraphicsItem->setVisible(true);
		emptyBasketGraphicsItem->setVisible(false);
		fullBasketGraphicsItem->setVisible(false);
		toolTipTxt+=tr("Empty conveyor");
	}
	toolTipTxt+="<br>"+tr("Position:%1(%2)").arg(globalId).arg(description);
	toolTipTxt+="<br>"+tr("Plc basket:%1[%2]").arg(plcBasketNumberTable).arg(plcBasketNumberIndex);
	mouseOverGraphicsItem->setToolTip(toolTipTxt);
}

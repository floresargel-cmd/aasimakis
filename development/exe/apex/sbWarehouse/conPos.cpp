#include "pch.h"
#include "conPos.h"
convPositionClass::convPositionClass(posClass *parrentPos_,QGraphicsScene *myScene_,float plcX_,float plcZ_,
	gItemClass *posTopMouseOver_,gItemClass *conveyorItem_,gItemClass *cassetteTop_,gItemClass *sbTop_,gItemClass *profileTop_,
	QWidget *p):QWidget(p)
{
	parrentPos=parrentPos_;
	setVisible(false);
	myScene=myScene_;
	setVisible(false);
	gXPos=plcX_;
	gYPos=plcZ_;
	angle=0.f;
	rotateXoffset=0.f;
	rotateYoffset=0.f;

	mouseOver=new gItemClass(posTopMouseOver_,gXPos,gYPos);
	conveyor=new gItemClass(conveyorItem_,gXPos,gYPos);
	cassette=new gItemClass(cassetteTop_,gXPos,gYPos);
	sb=new gItemClass(sbTop_,gXPos,gYPos);
	profiles=new gItemClass(profileTop_,gXPos,gYPos);
	myScene->addItem(conveyor);
	myScene->addItem(cassette);cassette->setVisible(false);
	myScene->addItem(sb);sb->setVisible(false);
	myScene->addItem(profiles);profiles->setVisible(false);
	myScene->addItem(mouseOver);
	connect(mouseOver,SIGNAL(hoverEnterSignal()),this,SLOT(hoverEnterSlot()));
	connect(mouseOver,SIGNAL(hoverLeaveSignal()),this,SLOT(hoverLeaveSlot()));
	connect(mouseOver,SIGNAL(mouseReleasedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mouseReleasedSlot(QGraphicsSceneMouseEvent*)));
	QTransform  txtTransformation;
	txtTransformation.scale(1,-1);
	txtTransformation.translate(gXPos,-gYPos);
	posGraphicsText=new myGraphicsTextItem(QString("%1").arg(parrentPos->getPlcIndex()),mouseOver->boundingRect(),QFont("Arial",250),QPen(QColor(150,50,50)),myScene);
	posGraphicsText->setTransform(txtTransformation);
	posGraphicsText->setPos(300,+300.);
	graphicsText=new myGraphicsTextItem(QString("%1").arg(parrentPos->getPosId()),mouseOver->boundingRect(),QFont("Arial",250),QPen(QColor(50,50,150)),myScene);
	graphicsText->setTransform(txtTransformation);
	updateGui();
}
convPositionClass::~convPositionClass()
{
}
void convPositionClass::addMoveClass(moveClass* m)
{
	movesVector<<m;
}
int convPositionClass::getGlobalId()
{
	return parrentPos->getPosId();
}
float convPositionClass::getGX()
{
	return gXPos;
}
float convPositionClass::getGY()
{
	return gYPos;
}
void convPositionClass::addProximitySwitch(int bit,float offset,QString txt,QPen activePen)
{
	addProximitySwitch(parrentPos->getTuxIp()->getIp(),"rConveyorState",parrentPos->getPlcIndex(),bit,txt,offset,activePen);
}
void convPositionClass::addProximitySwitch(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset,QPen activePen)
{
	gProximitySwitchClass *ps=new gProximitySwitchClass(plcIpAdress_,plcTable_,plcIndex_,plcBit_,txt,offset,myScene,activePen);
	QTransform  transformation;
	transformation.translate(gXPos,gYPos);
	ps->setTransform(transformation);
	proximitySwitchVector<<ps;
}
void convPositionClass::addButton(QString plcIpAdress_,QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,QString plcActiveTable_,int plcActiveIndex_,int plcActiveBit_,float offset,QString label,int type)
{
	buttonClass *btn=new buttonClass(parrentPos->getTuxIp(),plcIpAdress_,plcCommandTable_,plcCommandIndex_,plcCommandNumber_,plcActiveTable_,plcActiveIndex_,plcActiveBit_,offset,myScene,label,type);
	QTransform  transformation;
	transformation.translate(gXPos,gYPos);
	btn->setTransform(transformation);
	buttonsVector<<btn;
	btn->setMaintenanceMode(true);
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
	mouseOver->setTransform(transformation);
	conveyor->setTransform(transformation);
	cassette->setTransform(transformation);
	sb->setTransform(transformation);
	profiles->setTransform(transformation);
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
void convPositionClass::setBasketId(int b)
{
	parrentPos->getTuxIp()->writeInteger("posCassette",parrentPos->getPlcIndex(),b);
}
void convPositionClass::mouseReleasedSlot(QGraphicsSceneMouseEvent *ev)
{
	parrentPos->showPositionDlg(false);
	updateGui();
}
bool convPositionClass::getHasCassette()
{
	return parrentPos->getHasCassette();
}
int convPositionClass::getCassette()
{
	return parrentPos->getCassetteNumber();
}
QString convPositionClass::getDescription()
{
	return parrentPos->getDescription();
}
void convPositionClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((parrentPos->getTuxIp()->getIp()==ip)&&(("posCassette"==table)||("posSb"==table)||("posTicket"==table))&&(parrentPos->getPlcIndex()==index))
	{
		updateGui();
	}
	for (int i=0;i<proximitySwitchVector.count();i++)
		proximitySwitchVector[i]->dataInPlcChanged(ip,table,index,value);
	for (int i=0;i<buttonsVector.count();i++)
		buttonsVector[i]->dataInPlcChanged(ip,table,index,value);
}

void convPositionClass::updateGui()
{
	QString toolTipTxt;
	if (parrentPos->getCassetteNumber()!=0)
	{
		graphicsText->setText(QString("c%1").arg(parrentPos->getCassetteNumber()));
		toolTipTxt+="<font style='color:#333333'>"+tr("Cassette number:")+"</font><font style='color:#000077'>"+QString("%1").arg(parrentPos->getCassetteNumber())+"</font><br>";
		cassette->setVisible(true);
	}
	else
	{
		cassette->setVisible(false);
		profiles->setVisible(false);
		graphicsText->setText("");
	}
	profiles->setVisible(parrentPos->getHasTicket());
	sb->setVisible(parrentPos->getHasSb());
	toolTipTxt+="<br>"+tr("Position:%1(%2)").arg(parrentPos->getPosId()).arg(parrentPos->getDescription());
	toolTipTxt+="<br>"+tr("Plc basket:%1[%2]").arg("posCassette").arg(parrentPos->getPlcIndex());
	mouseOver->setToolTip(toolTipTxt);
}

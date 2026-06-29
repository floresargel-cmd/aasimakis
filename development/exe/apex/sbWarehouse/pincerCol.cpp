#include "pch.h"
#include "pincerCol.h"
pincerColumnsClass::pincerColumnsClass(tuxipClass *tuxip_,QString title_,QGraphicsScene *myScene,QPointF bottomPos,QWidget *p):QWidget(p)
{
	tuxip=tuxip_;
	posIsLockedFromPlc=false;
	columnTitle=title_;
	graphicsItemText=new myGraphicsTextItem(columnTitle,QRectF(-500.,0.,1000,200),QFont("Arial",100),QColor(100,125,100),myScene);
	graphicsItemText->setText(columnTitle);
	graphicsItemText->setZValue(10);
	QTransform  txtTransformation;
	txtTransformation.scale(1.,-1.);
	txtTransformation.translate(bottomPos.x(),-bottomPos.y());
	graphicsItemText->setTransform(txtTransformation);
}
void pincerColumnsClass::updateFromParentPos()
{
	for(int i=0;i<posVector.count();i++)
		posVector[i]->updateVisual();
}
void pincerColumnsClass::positionChangedSlot(int id)
{
	for(int i=0;i<posVector.count();i++)
	{
		posVector[i]->updateVisual();
	}
}
void pincerColumnsClass::addPositionClass(pincerPositionClass *p)
{
	if (posVector.count()==0)
		posVector<<p;
	else
	{
		posVector.last()->setAbovePosition(p);
		p->setBelowPosition(posVector.last());
		posVector<<p;
	}
	connect(posVector.last(),SIGNAL(changedSignal(int)),this,SLOT(positionChangedSlot(int)));
}
void pincerColumnsClass::enablePositionsToSelectFrom()
{ 
	for(int i=0;i<posVector.count();i++)
		posVector[i]->addMode(pincerPositionClass::ENABLED_FOR_USER_TO_SELECTE_FROM);
} 
bool pincerColumnsClass::getHasMode(int m)
{ 
	for(int i=0;i<posVector.count();i++)
	{
		if (posVector[i]->getHasMode(m))
			return true;
	}
	return false;
} 
void pincerColumnsClass::addMode(int m)
{
	bool isForUserToselectTo=HAVECOMMONBIT(m,pincerPositionClass::SHOWN_FOR_USER_TO_SELECTE_TO);
	bool hasSelected=false;
	if (isForUserToselectTo)
		hasSelected=getHasMode(pincerPositionClass::ENABLED_FOR_USER_TO_SELECTE_FROM);
	for(int i=0;i<posVector.count();i++)
	{
		if (isForUserToselectTo)
		{
			if (!hasSelected)
				posVector[i]->addMode(m);
		}
		else
			posVector[i]->addMode(m);
	}
}
void pincerColumnsClass::removeMode(int m)
{
	for(int i=0;i<posVector.count();i++)
		posVector[i]->removeMode(m);
}

void pincerColumnsClass::enablePositionsToSelectTo()
{	
	for(int i=0;i<posVector.count();i++)
		posVector[i]->addMode(pincerPositionClass::SHOWN_FOR_USER_TO_SELECTE_TO);
}
void pincerColumnsClass::setUserMaintenanseMode(bool m)
{	
	for(int i=0;i<posVector.count();i++)
		posVector[i]->setUserMaintenanseMode(m);
}
pincerPositionClass *pincerColumnsClass::getPosWithId(int p)
{
	pincerPositionClass *ans=NULL;
	for(int i=0;i<posVector.count();i++)
	{
		if (p==posVector[i]->getPosId())
		{
			ans=posVector[i];
			break;
		} 
	}
	return ans;
}
QVector<pincerPositionClass*> pincerColumnsClass::getAllPos()
{
	return posVector;
}
void pincerColumnsClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	for(int i=0;i<posVector.count();i++)
		posVector[i]->dataInPlcChanged(ip,table,index,value);
} 

#include "ovS.h"
ovenStackClass::ovenStackClass(QGraphicsScene *myScene_,double xOffset_,double yOffset_,QPainterPath basketPath_,bool canTake30Feet_,QWidget *p):QWidget(p)
{
	needsToBeCalculated=0;
	basketPath=basketPath_;
	myScene=myScene_;
	xOffset=xOffset_;
	yOffset=yOffset_;
	canTake30Feet=canTake30Feet_;
	numberOfBaskets=0;
}
ovenStackClass::~ovenStackClass()
{
}
//
void ovenStackClass::setNeedsToBeCalculated(int needsToBeCalculated_)
{
	if (needsToBeCalculated!=needsToBeCalculated_)
	{
		needsToBeCalculated=needsToBeCalculated_;
		for (int i=0;i<profileGroupsVector.count();i++)
			profileGroupsVector[i]->setNeedsToBeCalculated(needsToBeCalculated);
	}
}
void ovenStackClass::loadFromBasketCraneDatabase(int posPlcId)
{
	int noOfBaskets=GETONE(QString("select count(basketTableId) from positions where basketTableId<>0 and posNumber=%1").arg(posPlcId),BASKET_CRANE_DB_CONNECTION).toInt();
	for (int i=0;i<noOfBaskets;i++)
	{
		myScene->addPath(basketPath.translated(xOffset,yOffset+750.*numberOfBaskets),QPen(QColor(100,50,50)));
		numberOfBaskets++;
	}
}
bool ovenStackClass::canTakeProfilesGroup(QVector<basketProfileGroupClass*> groupsVector)
{
	if (!canTake30Feet)
	{
		for (int i=0;i<groupsVector.count();i++)
		{
			if (groupsVector[i]->getLength()>7365)
				return false;
		}
	}
	return true;
}
int ovenStackClass::getNumberOfBaskets()
{
	return numberOfBaskets;
}
bool ovenStackClass::addBasket(QVector<basketProfileGroupClass*> groupsVector)
{
	if (!canTakeProfilesGroup(groupsVector))
		return false;
	if (numberOfBaskets==4)
		return false;
	myScene->addPath(basketPath.translated(xOffset,yOffset+750.*numberOfBaskets),QPen(QColor(150,150,150)));
	profileGroupsVector<<groupsVector;
	for (int i=0;i<groupsVector.count();i++)
	{
		groupsVector[i]->setTransform(QTransform(1.,0.,0.,1.,xOffset,yOffset+750.*numberOfBaskets));
		myScene->addItem(groupsVector[i]);
		connect(groupsVector[i],SIGNAL(mouseEnterSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));
		connect(groupsVector[i],SIGNAL(mouseLeaveSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));
		connect(groupsVector[i],SIGNAL(productionSequenceChangedSignal()),this,SLOT(productionSequenceChangedSlot()));
	}
	numberOfBaskets++;
	return true;
}
void ovenStackClass::mouseEnterProfileGroupSlot(profileProductionClass *p)
{
	emit mouseEnterProfileGroupSignal(p);
}
void ovenStackClass::mouseLeaveProfileGroupSlot(profileProductionClass *p)
{
	emit mouseLeaveProfileGroupSignal(p);
}
void ovenStackClass::productionSequenceChangedSlot()
{
	emit productionSequenceChangedSignal();
}
void ovenStackClass::setMouseOverProfileProducton(profileProductionClass* p)
{
	for (int i=0;i<profileGroupsVector.count();i++)
		profileGroupsVector[i]->setMouseOverProfileProducton(p);
}

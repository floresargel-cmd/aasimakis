#include "ovL.h"
ovenLayoutClass::ovenLayoutClass(QGraphicsScene *myScene,double xOffset,double yOffset,QPainterPath basketPath,QWidget *p):QWidget(p)
{
	needsToBeCalculated=0;
	myScene->addRect(xOffset-1500.,yOffset,3000.,6500,QPen(QColor(0,0,150),15),QBrush(QColor(225,225,225)));
	stack1=new ovenStackClass(myScene,xOffset-750.,yOffset,							basketPath,false,p);connect(stack1,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack1,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));connect(stack1,SIGNAL(productionSequenceChangedSignal()),this,SLOT(productionSequenceChangedSlot()));
	stack2=new ovenStackClass(myScene,xOffset+750.,yOffset,							basketPath,false,p);connect(stack2,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack2,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));connect(stack2,SIGNAL(productionSequenceChangedSignal()),this,SLOT(productionSequenceChangedSlot()));
	stack3=new ovenStackClass(myScene,xOffset-750.,yOffset+750.*4+300.,	basketPath,false,p);connect(stack3,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack3,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));connect(stack3,SIGNAL(productionSequenceChangedSignal()),this,SLOT(productionSequenceChangedSlot()));
	stack4=new ovenStackClass(myScene,xOffset+750.,yOffset+750.*4+300.,	basketPath,false,p);connect(stack4,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack4,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));connect(stack4,SIGNAL(productionSequenceChangedSignal()),this,SLOT(productionSequenceChangedSlot()));
	stack5=new ovenStackClass(myScene,xOffset     ,yOffset						,	basketPath,true, p);connect(stack5,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack5,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));connect(stack5,SIGNAL(productionSequenceChangedSignal()),this,SLOT(productionSequenceChangedSlot()));
	stack6=new ovenStackClass(myScene,xOffset     ,yOffset+750.*4+300.,	basketPath,true, p);connect(stack6,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack6,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));connect(stack6,SIGNAL(productionSequenceChangedSignal()),this,SLOT(productionSequenceChangedSlot()));
}
ovenLayoutClass::~ovenLayoutClass()
{
}
//
void ovenLayoutClass::setNeedsToBeCalculated(int needsToBeCalculated_)
{
	if (needsToBeCalculated!=needsToBeCalculated_)
	{
		needsToBeCalculated=needsToBeCalculated_;
		stack1->setNeedsToBeCalculated(needsToBeCalculated);
		stack2->setNeedsToBeCalculated(needsToBeCalculated);
		stack3->setNeedsToBeCalculated(needsToBeCalculated);
		stack4->setNeedsToBeCalculated(needsToBeCalculated);
		stack5->setNeedsToBeCalculated(needsToBeCalculated);
		stack6->setNeedsToBeCalculated(needsToBeCalculated);
	}
}
void ovenLayoutClass::loadFromBasketCraneDatabase()
{
	stack1->loadFromBasketCraneDatabase(100);//connect(stack1,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack1,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));
	stack2->loadFromBasketCraneDatabase(110);//connect(stack2,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack2,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));
	stack3->loadFromBasketCraneDatabase(120);//connect(stack3,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack3,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));
	stack4->loadFromBasketCraneDatabase(130);//connect(stack4,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack4,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));
	stack5->loadFromBasketCraneDatabase(140);//connect(stack5,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack5,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));
	stack6->loadFromBasketCraneDatabase(150);//connect(stack6,SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));connect(stack6,SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));
}
bool ovenLayoutClass::addBasket(QVector<basketProfileGroupClass*> profileGroupsVector)
{
	if (stack6->getNumberOfBaskets()==0)
	{
		if (stack4->addBasket(profileGroupsVector))
			return true;
		if (stack3->addBasket(profileGroupsVector))
			return true;
	}
	if (stack5->getNumberOfBaskets()==0)
	{
		if (stack2->addBasket(profileGroupsVector))
			return true;
		if (stack1->addBasket(profileGroupsVector))
			return true;
	}
	if ((stack3->getNumberOfBaskets()==0)&&(stack4->getNumberOfBaskets()==0)&&(stack1->getNumberOfBaskets()<3)&&(stack2->getNumberOfBaskets()<3))
	{
		if (stack6->addBasket(profileGroupsVector))
			return true;
	}
	if ((stack1->getNumberOfBaskets()==0)&&(stack2->getNumberOfBaskets()==0))
	{
		if (stack5->addBasket(profileGroupsVector))
			return true;
	}
	return false;
}
void ovenLayoutClass::mouseEnterProfileGroupSlot(profileProductionClass* p)
{
	emit mouseEnterProfileGroupSignal(p);
}
void ovenLayoutClass::mouseLeaveProfileGroupSlot(profileProductionClass* p)
{
	emit mouseLeaveProfileGroupSignal(p);
}
void ovenLayoutClass::productionSequenceChangedSlot()
{
	emit productionSequenceChangedSignal();
}
void ovenLayoutClass::setMouseOverProfileProducton(profileProductionClass* p)
{
	stack1->setMouseOverProfileProducton(p);
	stack2->setMouseOverProfileProducton(p);
	stack3->setMouseOverProfileProducton(p);
	stack4->setMouseOverProfileProducton(p);
	stack5->setMouseOverProfileProducton(p);
	stack6->setMouseOverProfileProducton(p);
}


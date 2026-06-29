#include "bs.h"
basketClass::basketClass(QWidget *p):QWidget(p)
{
	totalWeight=0.;
	setAutoFillBackground(true);
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);

	myScene=new QGraphicsScene();//list of objects
	myQGraphicsView=new dxfQGraphicsViewClass(this,5,0.35);//viewer 
	myQGraphicsView->setMinimumSize(600,350);
	myQGraphicsView->scale(0.4,0.4);
	myQGraphicsView->setScene(myScene);//setScene
	QBrush b=QBrush(Qt::SolidPattern);
	b.setColor(QColor(230,230,230));
	myQGraphicsView->setBackgroundBrush(b);
	mainLayout->addWidget(myQGraphicsView);
	basketGraphics=new dxfGroupOfGraphicsItemClass(QDir::current().absolutePath()+"//basketSide.dxf",0.0,true,QColor(75,50,50));
	myScene->addItem(basketGraphics);
	basketGraphics->setFlag(QGraphicsItem::ItemIsMovable,false);
}
basketClass::~basketClass()
{
	delete myScene;
}
void basketClass::setNeedsToBeCalculated(bool needsToBeCalculated_)
{
	if (needsToBeCalculated!=needsToBeCalculated_)
	{
		needsToBeCalculated=needsToBeCalculated_;
		if (needsToBeCalculated)
		{
			QBrush b=QBrush(Qt::BDiagPattern);
			b.setColor(QColor(150,150,150));
			myQGraphicsView->setBackgroundBrush(b);
		}
		else
		{
			QBrush b=QBrush(Qt::SolidPattern);
			b.setColor(QColor(230,230,230));
			myQGraphicsView->setBackgroundBrush(b);
		}
	}
}
double basketClass::getTotalHeight()
{
	if (layersVector.count()==0)
		return FIRST_LAYER_HEIGTH;
	else
		return layersVector.last()->getTotalHeight();
}
void basketClass::calculateWeight()
{
	totalWeight=0.0;
	for (int i=0;i<layersVector.count();i++)
		totalWeight+=layersVector[i]->getTotalWeight();
}
bool basketClass::addEmptyLayerForProfiles(profileProductionClass* p,bool forceOnComb)
{
	double currentHeight=getTotalHeight();
	if (p->getIsOnComb()||forceOnComb||((layersVector.count()>0)&&layersVector.last()->getIsOnComb()))
	{
		int comb=getCombWithHeightMoreThan(currentHeight/*+p->getProfileInterLayerSpaceing()*/);
		if (comb>MAX_COMBS)
			return false;
		double layerHeight=getHeightOfComb(comb);
		if ((layerHeight+p->getProfileHeight())<MAX_HEIGHT_OF_LAYERS_IN_A_BASKET)
			layersVector<<new profileLayerClass(layersVector.count(),comb,layerHeight,myScene);
		else
			return false;
	}
	else
	{
		if ((currentHeight+SPACER_THIKNESS+p->getProfileHeight())<MAX_HEIGHT_OF_LAYERS_IN_A_BASKET)
			layersVector<<new profileLayerClass(layersVector.count(),0,currentHeight,myScene);
		else
			return false;
	}
	return true;
}
int basketClass::addProfiles(profileProductionClass* p,int number)
{
	if ((p->getWeightOfOneProfile()+totalWeight)>MAX_WEIGHT_OF_BASKET)
		return 0;
	int totalProfilesThatWarePutIntoTheLayer=0;
	if ((layersVector.count()==0)||(p->getIsToBePutOnNextLayer())||(p->getIsOnComb()))
	{
		
		if (!addEmptyLayerForProfiles(p,false))
			return totalProfilesThatWarePutIntoTheLayer;
	}
	int profilesThaCanBePutBecauseOfMaxWeightLimitation=(MAX_WEIGHT_OF_BASKET-totalWeight)/p->getWeightOfOneProfile();
	int profilesLeftToPutIntoTheLayer=number<profilesThaCanBePutBecauseOfMaxWeightLimitation?number:profilesThaCanBePutBecauseOfMaxWeightLimitation;
	while (profilesLeftToPutIntoTheLayer>0)
	{
		int profilesThatWarePutIntoTheLayer=layersVector.last()->addProfiles(p,profilesLeftToPutIntoTheLayer);
		if (profilesThatWarePutIntoTheLayer==0)
		{
			if (!addEmptyLayerForProfiles(p,!layersVector.last()->getCanSupportLayer()))
				break;
		}
		profilesLeftToPutIntoTheLayer-=profilesThatWarePutIntoTheLayer;
		totalProfilesThatWarePutIntoTheLayer+=profilesThatWarePutIntoTheLayer;
	}
	calculateWeight();
	return totalProfilesThatWarePutIntoTheLayer;
}

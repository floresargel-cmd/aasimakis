#include "pl.h"
profileLayerClass::profileLayerClass(int number_,int combNumber_,double baseHeight_,QGraphicsScene *myScene_):QWidget()
{
	number=number_;
	combNumber=combNumber_;
	baseHeight=baseHeight_;
	myScene=myScene_;
	lastProfile=NULL;
	lastProfileDx=0.;
	lastProfileDy=0.;
	spacers.resize(8);
	flatSurfase=0.;
	maxProfileHeight=0.;
	//
	totalHeight=totalWidth=totalWeight=0;
	//
	myScene->addRect(-0.5*SPACER_WIDTH,baseHeight+1.5,SPACER_WIDTH-3.,SPACER_THIKNESS-3.,QPen(QColor(255,255,255),0.0,Qt::SolidLine));
	myScene->addLine(-620.,baseHeight,620,baseHeight,QPen(QColor(247,164,9),0.1,Qt::SolidLine));
	myScene->addLine(-620.,baseHeight+SPACER_THIKNESS,620,baseHeight+SPACER_THIKNESS,QPen(QColor(247,164,9),0.1,Qt::SolidLine));
	//spacers
	myScene->addRect(-600.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine));
	myScene->addRect(-435.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine));
	myScene->addRect(-270.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine));
	myScene->addRect(-105.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine));
	myScene->addRect(  60.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine));
	myScene->addRect( 225.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine));
	myScene->addRect( 390.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine));
	myScene->addRect( 550.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine));
}
profileLayerClass::~profileLayerClass()
{
}
//
int profileLayerClass::getNumber()
{
	return number;
}
int profileLayerClass::getCombNumber()
{
	return combNumber;
}
bool profileLayerClass::getIsOnComb()
{
	return combNumber>0?true:false;
}
void profileLayerClass::addSpacers(QVector<bool> sp)
{
	for (int i=0;i<8;i++)
	{
		if (sp[i])
		{
			spacers[i]=sp[i];
				   if (i==0)myScene->addRect(-600.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(255,255,255))));
			else if (i==1)myScene->addRect(-435.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(255,255,255))));
			else if (i==2)myScene->addRect(-270.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(255,255,255))));
			else if (i==3)myScene->addRect(-105.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(255,255,255))));
			else if (i==4)myScene->addRect(  60.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(255,255,255))));
			else if (i==5)myScene->addRect( 225.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(255,255,255))));
			else if (i==6)myScene->addRect( 390.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(255,255,255))));
			else if (i==7)myScene->addRect( 550.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(255,255,255))));
		}
	}
}
double profileLayerClass::getTotalHeight()
{
	return baseHeight+SPACER_THIKNESS+totalHeight;
}
bool profileLayerClass::getCanSupportLayer()
{
	return true;
}
double profileLayerClass::getTotalWeight()
{
	return totalWeight+spacers.count(true)*SPACER_WEIGHT;
}
int profileLayerClass::addProfiles(profileProductionClass* p,int numberOfProfiles)
{
	int profilesThatWarePutIntoTheLayer=0;
	if ((baseHeight+SPACER_THIKNESS+p->getProfileHeight())>MAX_HEIGHT_OF_LAYERS_IN_A_BASKET)
		return 0;
	addSpacers(p->getSpacers());
	for (int i=0;i<numberOfProfiles;i++)
	{
		if ((totalWidth+p->getProfileWidth())>MAX_LAYER_WIDTH)
			break;
		double dx=totalWidth-0.5*MAX_LAYER_WIDTH-p->getProfilePath().boundingRect().left();
		double dy=baseHeight+SPACER_THIKNESS+p->getProfilePath().boundingRect().height()-p->getProfilePath().boundingRect().bottom();
		double offSet=0.;
		if (profiles.count()>0)
		{
			if (i==0)
				offSet=getMinXDistanceOfPaths(lastProfile->getProfilePath().translated(lastProfileDx,lastProfileDy),p->getProfilePath().translated(dx,dy));
			else
				offSet=p->getOffsetToTouchTheNextProfileInTheLayer();
		}
		if (profiles.count()>0)
		{
			double sp=p->getProfileSpaceingInLayer();
			if (lastProfile&&(lastProfile->getProfileSpaceingInLayer()>sp))
				sp=lastProfile->getProfileSpaceingInLayer();
			offSet-=sp;
		}
		lastProfile=p;
		dx-=offSet;
		lastProfileDx=dx;
		lastProfileDy=dy;
		myScene->addPath(p->getProfilePath().translated(dx,dy),QPen(p->getColor(),0.0,Qt::SolidLine));
		profilesThatWarePutIntoTheLayer++;
		totalWidth+=p->getProfileWidth()-offSet;
		totalHeight=MAX(totalHeight,p->getProfileHeight());
		if (p->getProfileHeight()>(maxProfileHeight+1.))
		{
			flatSurfase=0.;
			maxProfileHeight=p->getProfileHeight();
		}
		if (fabs(p->getProfileHeight()-maxProfileHeight)<1.)
			flatSurfase+=p->getProfileWidth();
		profiles<<p->getProfileCode();
	}
	totalWeight+=profilesThatWarePutIntoTheLayer*p->getWeightOfOneProfile();
	return profilesThatWarePutIntoTheLayer;
}

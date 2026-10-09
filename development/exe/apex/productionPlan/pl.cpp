#include "pl.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////profileGroupClass//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
layerProfileGroupClass::layerProfileGroupClass()
{
	noOfProfiles=0;
	profileProduction=NULL;
}
layerProfileGroupClass::layerProfileGroupClass(profileProductionClass* profileProduction_,double baseHeight)
{
	noOfProfiles=0;
	profileProduction=profileProduction_;
}
layerProfileGroupClass::~layerProfileGroupClass()
{
	double lala=0.;
}
int layerProfileGroupClass::getTableId()
{
	if (profileProduction)
		return profileProduction->getProfilesEpicsTableId();
	return NULL;
}
void layerProfileGroupClass::addProfile(QPainterPath p)
{
	noOfProfiles++;
	path.addRect(p.boundingRect());
}
QPainterPath layerProfileGroupClass::getPath()
{
	return path;
}
int layerProfileGroupClass::getNumberOfProfiles()
{
	return noOfProfiles;
}
profileProductionClass* layerProfileGroupClass::getProfileProduction()
{
	return profileProduction;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////basketClass////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	wordFromStackerPlc=0;
	//
	totalHeight=totalWidth=totalWeight=maxProfileLength=0;
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
	double textSpacing=50;
	QGraphicsSimpleTextItem *pItem=new QGraphicsSimpleTextItem("");pItem->setTransform(QTransform::fromTranslate(-0.5*SPACER_WIDTH-110.,baseHeight+40));pItem->setFont(QFont("arial",20));
	profileQuandityTextItemVector<<pItem;pItem->setTransform(QTransform(1.,0.,0.,-1.,0.,0.),true);myScene->addItem(pItem);
//	textItemsVector<<pItem;
	for (int i=0;i<10;i++)
	{
		pItem=new QGraphicsSimpleTextItem("");pItem->setTransform(QTransform::fromTranslate(0.5*SPACER_WIDTH+20.+(i+1)*textSpacing,baseHeight+40));pItem->setFont(QFont("arial",20));
		profileQuandityTextItemVector<<pItem;pItem->setTransform(QTransform(1.,0.,0.,-1.,0.,0.),true);myScene->addItem(pItem);
//		textItemsVector<<pItem;
	}
}
profileLayerClass::~profileLayerClass()
{
	//qDeleteAll(textItemsVector);
	//textItemsVector.clear();
	qDeleteAll(profileQuandityTextItemVector);
	profileQuandityTextItemVector.clear();
	qDeleteAll(profileGroupsVector);
	profileGroupsVector.clear();
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
void profileLayerClass::addSpacers(QVector<bool> sp)
{
	for (int i=0;i<8;i++)
	{
		if (sp[i])
		{
			spacers[i]=sp[i];
				   if (i==0)myScene->addRect(-600.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(75,50,50))));
			else if (i==1)myScene->addRect(-435.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(75,50,50))));
			else if (i==2)myScene->addRect(-270.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(75,50,50))));
			else if (i==3)myScene->addRect(-105.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(75,50,50))));
			else if (i==4)myScene->addRect(  60.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(75,50,50))));
			else if (i==5)myScene->addRect( 225.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(75,50,50))));
			else if (i==6)myScene->addRect( 390.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(75,50,50))));
			else if (i==7)myScene->addRect( 550.,baseHeight+1.5,50.,16.,QPen(QColor(200,200,200),0.0,Qt::SolidLine),QBrush((combNumber>0?QColor(150,150,175):QColor(75,50,50))));
		}
	}
}
#define USE_DATA_FROM_PLC
QVector<bool> profileLayerClass::getSpacers()
{
#ifdef USE_DATA_FROM_PLC
	QVector<bool> ans;
	ans<<bitsFromStackerPlc[1]<<bitsFromStackerPlc[2]<<bitsFromStackerPlc[3]<<bitsFromStackerPlc[4]<<bitsFromStackerPlc[5]<<bitsFromStackerPlc[6]<<bitsFromStackerPlc[7]<<bitsFromStackerPlc[8];
	return ans;
#else
	return spacers;
#endif
}
bool profileLayerClass::getIsOnCombForDb()
{
#ifdef USE_DATA_FROM_PLC
	return bitsFromStackerPlc[0];
#else
	return nComb();
#endif
}
bool profileLayerClass::getIsOnComb()
{
	return combNumber>0?true:false;
}
void profileLayerClass::setSpacerWordFromStackerPlc(int v)
{
	wordFromStackerPlc=v;
	bitsFromStackerPlc=deprecatedCalculateBits(wordFromStackerPlc);
}
int profileLayerClass::getSpacersWord()//from database
{
	int ans=0;
	ans+=
		  (spacers[0]?1.:0.0)*1	 +
		  (spacers[1]?1.:0.0)*2	 +
		  (spacers[2]?1.:0.0)*4	 +
		  (spacers[3]?1.:0.0)*8	 +
		  (spacers[4]?1.:0.0)*16 +
		  (spacers[5]?1.:0.0)*32 +
		  (spacers[6]?1.:0.0)*64 +
		  (spacers[7]?1.:0.0)*128;
	return ans;
}

QVector<int> profileLayerClass::getProfilesEpicsTableId()
{
	return profilesEpicsTableId;
}
QVector<int> profileLayerClass::getProfilesCodesTableId()
{
	return profiles;
}
QVector<int> profileLayerClass::getProfilesMultiplicity()
{
	return profilesMultiplicity;
}
QVector<QColor> profileLayerClass::getProfilesColors()
{
	return profilesColors;
}
double profileLayerClass::getTotalHeight()
{
	return baseHeight+SPACER_THIKNESS+totalHeight;
}
double profileLayerClass::getMaxProfileLength()
{
	return maxProfileLength;
}
bool profileLayerClass::getCanSupportLayer()
{
	return (!((profiles.count()>0)&&(flatSurfase<(0.01*LAYER_MIN_SUPORT_LENGTH)*MAX_LAYER_WIDTH)))?true:false;
}
double profileLayerClass::getTotalWeight()
{
	return totalWeight+spacers.count(true)*SPACER_WEIGHT;
}
void profileLayerClass::addToGroup(profileProductionClass* profileProduction,QPainterPath p)
{
	bool found=false;
	for (int i=0;i<profileGroupsVector.count();i++)
	{
		if (profileGroupsVector[i]->getTableId()==profileProduction->getProfilesEpicsTableId())
		{
			profileGroupsVector[i]->addProfile(p);
			found=true;
			break;
		}
	}
	if (!found)
	{
		layerProfileGroupClass *g=new layerProfileGroupClass(profileProduction,baseHeight);
		g->addProfile(p);
		profileGroupsVector<<g;
	}
}
QVector<layerProfileGroupClass*> profileLayerClass::getProfileGroups()
{
	return profileGroupsVector;
}
int profileLayerClass::addProfiles(profileProductionClass* p,int numberOfProfiles)
{
	int profilesThatWarePutIntoTheLayer=0;
	if ((baseHeight+SPACER_THIKNESS+p->getProfileHeight())>MAX_HEIGHT_OF_LAYERS_IN_A_BASKET)
		return 0;
	for (int i=0;i<numberOfProfiles;i++)
	{
		if ((totalWidth+p->getProfileWidth()+p->getProfileSpaceingInLayer())>MAX_LAYER_WIDTH)
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
		addToGroup(p,p->getProfilePath().translated(dx,dy));
		profilesThatWarePutIntoTheLayer++;
		totalWidth+=p->getProfileWidth()-offSet;
		totalHeight=MAX(totalHeight,p->getProfileHeight());
		maxProfileLength=MAX(maxProfileLength,p->getProfileLength());
		if (p->getProfileHeight()>(maxProfileHeight+1.))
		{
			flatSurfase=0.;
			maxProfileHeight=p->getProfileHeight();
		}
		if (fabs(p->getProfileHeight()-maxProfileHeight)<1.)
			flatSurfase+=p->getProfileWidth();
		profiles<<p->getProfileCode();
		profilesEpicsTableId<<p->getProfilesEpicsTableId();
		profilesColors<<p->getColor();
		profilesMultiplicity<<p->getTotalProfileMultiplicity();
	}
	totalWeight+=profilesThatWarePutIntoTheLayer*p->getWeightOfOneProfile();
	if (profilesThatWarePutIntoTheLayer>0)
		addSpacers(p->getSpacers());
	//calculate quandity labels
	QVector<int> uProfiles;
	QVector<int> uProfilesEpicsTableId;
	QVector<QColor> uProfilesColors;
	QVector<int> uProfilesMultiplicity;
	for (int i=0;i<profilesEpicsTableId.count();i++)
	{
		if (!uProfilesEpicsTableId.contains(profilesEpicsTableId[i]))
		{
			uProfiles<<profiles[i];
			uProfilesEpicsTableId<<profilesEpicsTableId[i];
			uProfilesColors<<profilesColors[i];
			uProfilesMultiplicity<<profilesMultiplicity[i];
		}
	}
	for (int i=0;(i<uProfiles.count())&&(i<profileQuandityTextItemVector.count());i++)
	{
		profileQuandityTextItemVector[i]->setText(QString("%1").arg(uProfilesMultiplicity[i]*profilesEpicsTableId.count(uProfilesEpicsTableId[i])));
		profileQuandityTextItemVector[i]->setBrush(QBrush(uProfilesColors[i]));
	}
	return profilesThatWarePutIntoTheLayer;
}
bool profileLayerClass::hasProfile(int code)
{
	if (profiles.indexOf(code)>=0)
		return true;
	else
		return false;
}
int profileLayerClass::getFirstProfile()
{
	if (profiles.count()>=0)
		return profiles[0];
	return 0;
}

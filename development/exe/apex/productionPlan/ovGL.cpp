#include "ovGL.h"
ovenGroupLayoutClass::ovenGroupLayoutClass(QWidget *p):QDialog(p)
{
	resize(1310,350);
	needsToBeCalculated=false;
	QScrollArea *mainScrollArea=new QScrollArea();
	mainScrollArea->setAlignment(Qt::AlignCenter);
	mainScrollArea->setBackgroundRole(QPalette::Dark);
	QHBoxLayout *topLayout=new QHBoxLayout();
	topLayout->setMargin(0);
	QHBoxLayout *mainLayout=new QHBoxLayout();
	setLayout(mainLayout);

	myScene=new QGraphicsScene();//list of objects
	myQGraphicsView=new dxfQGraphicsViewClass(this,1,0.05);//viewer 
	myQGraphicsView->setMinimumSize(800,350);
	myQGraphicsView->scale(0.05,0.05);
	myQGraphicsView->setScene(myScene);//setScene
	QBrush b=QBrush(Qt::SolidPattern);
	b.setColor(QColor(230,230,230));
	myQGraphicsView->setBackgroundBrush(b);
	mainLayout->addWidget(myQGraphicsView);

	dxfGroupOfGraphicsItemClass *basket=new dxfGroupOfGraphicsItemClass(":/dxfs/basketSide.dxf",0.0,true,QColor(75,50,50));
	basketPath=basket->getPath();
	//actions
	QAction *f5Action=new QAction("",this);
	addAction(f5Action);
	f5Action->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F5));
	connect(f5Action,SIGNAL(triggered()),this,SLOT(f5Slot()));
}
ovenGroupLayoutClass::~ovenGroupLayoutClass()
{
	delete myQGraphicsView;
}
void ovenGroupLayoutClass::f5Slot()
{
	emit f5Signal();
}
void ovenGroupLayoutClass::productionSequenceChangedSlot()
{
	setNeedsToBeCalculated(true);
	emit productionSequenceChangedSignal();
}
void ovenGroupLayoutClass::clearLayout()
{
	myScene->clear();
}
void ovenGroupLayoutClass::setNeedsToBeCalculated(int needsToBeCalculated_)
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
		for (int i=0;i<ovenStacksVector.count();i++)
			ovenStacksVector[i]->setNeedsToBeCalculated(needsToBeCalculated);
	}
}
//
void ovenGroupLayoutClass::recalculate(QVector<basketClass*> basketsVector)
{
	ovenStacksVector.clear();
	//current positions in oven entranse
	ovenStacksVector<<new ovenLayoutClass(myScene,3200.*ovenStacksVector.count(),0.,basketPath,this);
	ovenStacksVector.last()->loadFromBasketCraneDatabase();
	connect(ovenStacksVector.last(),SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));
	connect(ovenStacksVector.last(),SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));
	connect(ovenStacksVector.last(),SIGNAL(productionSequenceChangedSignal()),this,SLOT(productionSequenceChangedSlot()));
	//
	for (int i=0;i<basketsVector.count();i++)
		addBasket(basketsVector[i]->getProfilesGroupsVector());
	setNeedsToBeCalculated(false);
}
void ovenGroupLayoutClass::addBasket(QVector<basketProfileGroupClass*> profileGroupsVector)
{
	if (ovenStacksVector.count()==0)
	{
		ovenStacksVector<<new ovenLayoutClass(myScene,3200.*ovenStacksVector.count(),0.,basketPath,this);
		connect(ovenStacksVector.last(),SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));
		connect(ovenStacksVector.last(),SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));
		connect(ovenStacksVector.last(),SIGNAL(productionSequenceChangedSignal()),this,SLOT(productionSequenceChangedSlot()));
	}
	if (!ovenStacksVector.last()->addBasket(profileGroupsVector))
	{
		ovenStacksVector<<new ovenLayoutClass(myScene,3200.*ovenStacksVector.count(),0.,basketPath,this);
		connect(ovenStacksVector.last(),SIGNAL(mouseEnterProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseEnterProfileGroupSlot(profileProductionClass*)));
		connect(ovenStacksVector.last(),SIGNAL(mouseLeaveProfileGroupSignal(profileProductionClass*)),this,SLOT(mouseLeaveProfileGroupSlot(profileProductionClass*)));
		connect(ovenStacksVector.last(),SIGNAL(productionSequenceChangedSignal()),this,SLOT(productionSequenceChangedSlot()));
		ovenStacksVector.last()->addBasket(profileGroupsVector);
	}
}
void ovenGroupLayoutClass::mouseEnterProfileGroupSlot(profileProductionClass* p)
{
	if (!needsToBeCalculated)
	{
		for (int i=0;i<ovenStacksVector.count();i++)
			ovenStacksVector[i]->setMouseOverProfileProducton(p);
		myScene->update();
	}
}
void ovenGroupLayoutClass::mouseLeaveProfileGroupSlot(profileProductionClass* p)
{
	if (!needsToBeCalculated)
	{
		for (int i=0;i<ovenStacksVector.count();i++)
			ovenStacksVector[i]->setMouseOverProfileProducton(NULL);
		myScene->update();
	}
}

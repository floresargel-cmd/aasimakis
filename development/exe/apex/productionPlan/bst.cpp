#include "bst.h"
basketStackingClass::basketStackingClass(QWidget *p):QWidget(p)
{
	tcpServer=NULL;
	stackerPlcWordsVector.resize(21);
	stackerPlcCombsWord=0;
	mainScrollArea=new QScrollArea();
	mainScrollArea->setAlignment(Qt::AlignCenter);
	mainScrollArea->setBackgroundRole(QPalette::Dark);
	QHBoxLayout *topLayout=new QHBoxLayout();
	topLayout->setMargin(0);
	setLayout(topLayout);
	topLayout->addWidget(mainScrollArea);
	mainScrollAreaWidget=new QWidget();
	mainLayout=new QHBoxLayout();
	mainScrollAreaWidget->setLayout(mainLayout);
	mainScrollArea->setWidgetResizable(true);
	mainScrollArea->setWidget(mainScrollAreaWidget);
	setNeedsToBeCalculated(1);
}
basketStackingClass::~basketStackingClass()
{
	clearAllBaskets();
}
//
QVector<basketClass*> basketStackingClass::getBasketsVector()
{
	return basketsVector;
}
void basketStackingClass::setTcpServer(tcpServerClass *tcpServer_)
{
	tcpServer=tcpServer_;
	for (int i=0;i<basketsVector.count();i++)
		basketsVector[i]->setTcpServer(tcpServer);
}
void basketStackingClass::stackerPlcMemoryChanged(QString table,int index,QString value)
{
	if ((table=="XOUT")&&(index>=1)&&(index<=20))
		stackerPlcWordsVector[index]=value.toInt();
	if ((table=="XOUT")&&(index==16))
		stackerPlcCombsWord=value.toInt();
	if (basketsVector.count()>0)
	{
		basketsVector[0]->setStackerPlcWords(stackerPlcWordsVector);
		basketsVector[0]->setStackerPlcWords(stackerPlcWordsVector);
	}
}
basketClass* basketStackingClass::getFirstVisible()
{
	for (int i=0;i<basketsVector.count();i++)
	{
		if (!basketsVector[i]->visibleRegion().isEmpty())
			return basketsVector[i];
	}
	return NULL;
}
void basketStackingClass::setVisibleBasketWithProfile(int profile)
{
	for (int i=0;i<basketsVector.count();i++)
	{
		if (basketsVector[i]->hasProfile(profile))
		{

			mainScrollArea->ensureWidgetVisible(basketsVector[i]);
			blockSignals(true);
			basketClassSelected(basketsVector[i],true);
			blockSignals(false);
			break;
		}
	}
}
void basketStackingClass::basketClassSelected(basketClass *b,bool selected)
{
	for (int i=0;i<basketsVector.count();i++)
	{
		if (selected&&(basketsVector[i]==b))
		{
			basketsVector[i]->setIsSelected(true);
			emit basketSelectedSignal(basketsVector[i]);
		}
		else
			basketsVector[i]->setIsSelected(false);
	}
}
////
void basketStackingClass::calculateLayout(profileProductionLayoutClass* profileProductionLayout)
{
	if (needsToBeCalculated)
	{
		LAYER_MIN_SUPORT_LENGTH=GETONE(QString("select value from settings where name='namelayerMinSuportLength'")).toInt();
		QVector<profileProductionClass*> ppv=profileProductionLayout->getProfileProductionVector();
		QProgressDialog progress(tr("Calculating baskets..."),tr("Cancel"),0,ppv.count(), this);
    progress.setWindowModality(Qt::WindowModal);
		progress.setMinimumWidth(600);
		progress.show();

		clearAllBaskets();
		appendBasket(new basketClass(basketsVector.count(),basketsVector.count()==0?true:false,this));
		basketsVector.last()->setMinimumWidth(1900);
		int addedBaskets=1;
		for (int i=0;i<ppv.count();i++)
		{
			if ((ppv[i]->getIsToBePutOnNextBasket()&&(basketsVector.last()->getTotalWeight()>0.0))||((i>0)&&(ppv[i]->getTemperType()!=ppv[i-1]->getTemperType())))
			{
				appendBasket(new basketClass(basketsVector.count(),basketsVector.count()==0?true:false,this));
				addedBaskets++;
			}
			int profilesLeftToPutIntoTheBasket=ppv[i]->getNumberOfProfilesNotInABasket()/(ppv[i]->getMultiplicityOfProfiles()*(ppv[i]->getIsDoubleLength()?2.:1.));
			while (profilesLeftToPutIntoTheBasket>0)
			{
				int profilesThatWarePutIntoTheBasket=basketsVector.last()->addProfiles(ppv[i],profilesLeftToPutIntoTheBasket);
				if (profilesThatWarePutIntoTheBasket==0)
				{
					appendBasket(new basketClass(basketsVector.count(),basketsVector.count()==0?true:false,this));
					addedBaskets++;
				}
				profilesLeftToPutIntoTheBasket-=profilesThatWarePutIntoTheBasket;
			}
			if (progress.wasCanceled())
				break;
			progress.setValue(i);
			apexExecQuery(QString("update epicsProduction2 set baskets=%1 where soNum='%2'").arg(addedBaskets).arg(ppv[i]->getSoNum()));
			QApplication::processEvents();
		}
		progress.setValue(ppv.count());
		setNeedsToBeCalculated(progress.wasCanceled(),1);
		for (int i=0;i<basketsVector.count();i++)
		{
			basketsVector[i]->calculateGroups();
		}
	}
	if (basketsVector.count()>0)
	{
		basketsVector[0]->sendDatabaseSpacerDataToPlc(1);//the first basket writes from index 1 to 20
		basketsVector[0]->setStackerPlcWords(stackerPlcWordsVector);
		basketsVector[0]->setStackerCombsWord(stackerPlcCombsWord);
	}
	if (basketsVector.count()>1)
		basketsVector[1]->sendDatabaseSpacerDataToPlc(21);//the second basket writes from index 21 to 40
}
void basketStackingClass::setNeedsToBeCalculated(int c,int force)
{
	if (force||(needsToBeCalculated!=c))
	{
		needsToBeCalculated=c;
		for (int i=0;i<basketsVector.count();i++)
		{
			basketsVector[i]->setNeedsToBeCalculated(needsToBeCalculated);
			basketsVector[i]->setPrivileges(needsToBeCalculated?0:PRIVILEGETYPE);
		}
	}
}
void basketStackingClass::clearAllBaskets()
{
	qDeleteAll(basketsVector);
	// line below comment by gene
	//basketsVector.clear();
	//
	// line below uncomment by gene
	for (int i=0;i<basketsVector.count();i++)
	{
		basketClass *b=basketsVector[i];
		mainLayout->removeWidget(b);
		basketsVector.remove(i);
		//delete b;
		i--;
	}
	//

}
void basketStackingClass::appendBasket(basketClass* b)
{
	connect(b,SIGNAL(selectedSignal(basketClass*,bool)),this,SLOT(basketClassSelected(basketClass*,bool)));
	b->setTcpServer(tcpServer);
	basketsVector<<b;
	mainLayout->addWidget(b);
}

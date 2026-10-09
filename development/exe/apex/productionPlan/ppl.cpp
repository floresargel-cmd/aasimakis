#include "ppl.h"
profileProductionLayoutClass::profileProductionLayoutClass():QWidget()
{
	mainScrollArea=new QScrollArea();
	mainScrollArea->setAlignment(Qt::AlignCenter);
	mainScrollArea->setBackgroundRole(QPalette::Dark);
	QHBoxLayout *topLayout=new QHBoxLayout();
	topLayout->setMargin(0);

	setLayout(topLayout);
	topLayout->addWidget(mainScrollArea);
	QWidget *mainScrollAreaWidget=new QWidget();
	mainLayout=new QHBoxLayout();
	mainScrollAreaWidget->setLayout(mainLayout);
	mainScrollArea->setWidgetResizable(true);
	mainScrollArea->setWidget(mainScrollAreaWidget);
	// Set the inner widget for scrolling
	mainScrollArea->setWidget(mainScrollAreaWidget);

	// Optional: Set a smaller initial size for this widget
	//setFixedSize(420, 3000); // Makes the top-level QWidget small initially

	setNeedsToBeLoadedFromDatabase(true);
	moreProductionAvailable=NULL;
}
profileProductionLayoutClass::~profileProductionLayoutClass()
{
	clearAllProfileProduction();
}
////
void profileProductionLayoutClass::loadFromDatabase()
{
	if (needsToBeLoadedFromDatabase)
	{
		globalColorIndex=-1;
		clearAllProfileProduction();
		QVector<QVariant> productionProfilesTId=apexExecQuery							(QString("select tid from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")									.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionProfilesProductionOrder=apexExecQuery	(QString("select productionOrder from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")			.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionProfiles=apexExecQuery								(QString("select die from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")									.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionProfileHSpacing=apexExecQuery					(QString("select profileSpacing from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")				.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionInComb=apexExecQuery									(QString("select inComb from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")								.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionTotalQuantity=apexExecQuery						(QString("select totalQuantity from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")				.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionQuantityInBaskets=apexExecQuery				(QString("select quantityInBaskets from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")		.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionLinearDencity=apexExecQuery						(QString("select linearDencity from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")				.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionLength=apexExecQuery									(QString("select length from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")								.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionDoubleLength=apexExecQuery						(QString("select doubleLength from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")					.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionStatus=apexExecQuery									(QString("select status from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")								.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionOnNextBasket=apexExecQuery						(QString("select onNextBasket from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")					.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionOnNextLayer=apexExecQuery							(QString("select onNextLayer from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")					.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionTemper=apexExecQuery									(QString("select temper from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")								.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionSoNum=apexExecQuery										(QString("select soNum from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")								.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionScheduleDate=apexExecQuery						(QString("select scheduleDate from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")					.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionScheduleShift=apexExecQuery						(QString("select scheduleShift from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")				.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionIsVirtual=apexExecQuery								(QString("select isVirtual from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")						.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionBilletsNo=apexExecQuery								(QString("select billetsNo from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")						.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionBilletLength=apexExecQuery						(QString("select billetLength from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")					.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionBilletButtLength=apexExecQuery				(QString("select billetButtLength from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")			.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionScrapFrontLength=apexExecQuery				(QString("select scrapFrontLength from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")			.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionScrapRearLength=apexExecQuery					(QString("select scrapRearLength from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")			.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionRunOutLength=apexExecQuery						(QString("select runOutLength from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")					.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));
		QVector<QVariant> productionDieMultiplicity=apexExecQuery					(QString("select dieMultiplicity from epicsProduction2 where (status=%1 and quantityInBaskets<totalQuantity) and pressNum=%2 order by productionOrder")			.arg(PRODUCTION_STATUS_SCHEDULED).arg(globalPressNumber));

		//check if the dxf files exist
		QStringList dxfFilesNotThere;
		for (int i=0;i<productionProfiles.count();i++)
		{
			QString profileFName=dxfDirPath+QString("%1//%2.dxf").arg((int)(floor(productionProfiles[i].toInt()/100)*100)).arg(productionProfiles[i].toInt());
			if (!(QFile(profileFName).exists()))
			{
				if (!dxfFilesNotThere.contains(profileFName))
					dxfFilesNotThere<<profileFName;
			}
		}
		if (dxfFilesNotThere.count() > 0)
		{
			QString msg;
			for (int i = 0;i < dxfFilesNotThere.count();i++)
				msg += dxfFilesNotThere[i] + "\n";

			//comment by gene EXIT("error", QString("Could not locate dxf file for the following profiles:\n%1").arg(msg));
		}

		//

		QVector<QVariant> productionTemperDescription=execQuery(QString("select RTRIM(temper) from temperTypes "),BASKET_CRANE_DB_CONNECTION);
		QVector<QVariant> productionTemperType=execQuery(QString("select temperType from temperTypes "),BASKET_CRANE_DB_CONNECTION);
		QProgressDialog progress(tr("Loading planned production from database..."),tr("Cancel"),0,productionProfilesTId.count(), this);
		progress.setWindowModality(Qt::WindowModal);
		progress.setMinimumWidth(600);
		progress.show();
		for (int i=0;(i<MAX_NO_OF_PROFILES_TO_LOAD)&&(i<productionProfiles.count());i++)
		{
			//if this is the first time that I read this profile insert the inComb and interProfileSpace
			progress.setLabelText(QString("Loading planned production from database. profile:%1").arg(productionProfiles[i].toInt()));
			apexExecQuery(QString("if not exists (select die from profileCode where die=%1) insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,%2,0,1,%3,0,0,0,0,0,0,0,0)").arg(productionProfiles[i].toString()).arg(productionProfileHSpacing[i].toString()).arg(productionInComb[i].toString()));
			progress.setValue(i);
			QApplication::processEvents();
			appendProfileProduction(new profileProductionClass(productionProfilesTId[i].toInt(),i==0?true:false,productionProfilesProductionOrder[i].toInt(),productionProfiles[i].toInt(),productionTotalQuantity[i].toInt(),productionQuantityInBaskets[i].toInt(),productionLinearDencity[i].toDouble(),productionLength[i].toInt(),productionDoubleLength[i].toInt(),productionStatus[i].toInt(),productionOnNextBasket[i].toInt(),productionOnNextLayer[i].toInt(),productionTemper[i].toString().trimmed(),productionSoNum[i].toString(),productionScheduleDate[i].toString(),productionScheduleShift[i].toInt(),productionIsVirtual[i].toBool()
				,productionBilletsNo[i].toInt(),productionBilletLength[i].toDouble(),productionBilletButtLength[i].toDouble(),productionScrapFrontLength[i].toDouble(),productionScrapRearLength[i].toDouble(),productionRunOutLength[i].toDouble(),productionDieMultiplicity[i].toInt(),productionTemperDescription,productionTemperType));
			if (progress.wasCanceled())
				break;
		}
		if (MAX_NO_OF_PROFILES_TO_LOAD<productionProfiles.count())
			mainLayout->addWidget(moreProductionAvailable=new QLabel(tr("Showing only %1 of %2 available scheduled profiles\npress Ctrl+O to change the maximum number of profiles to load").arg(MAX_NO_OF_PROFILES_TO_LOAD).arg(productionProfiles.count())));
		progress.setValue(productionProfilesTId.count());
		setNeedsToBeLoadedFromDatabase(progress.wasCanceled(),true);
	}
}
profileProductionClass* profileProductionLayoutClass::getFirstVisible()
{
	for (int i=0;i<profileProductionVector.count();i++)
	{
		if (!profileProductionVector[i]->visibleRegion().isEmpty())
			return profileProductionVector[i];
	}
	return NULL;
}
void profileProductionLayoutClass::setVisibleProfile(int profile)
{
	for (int i=0;i<profileProductionVector.count();i++)
	{
		if (profileProductionVector[i]->getProfileCode()==profile)
		{
			mainScrollArea->ensureWidgetVisible(profileProductionVector[i]);
			blockSignals(true);
			profileProductionClassSelected(profileProductionVector[i],true);
			blockSignals(false);
		}
	}
}

void profileProductionLayoutClass::clearAllProfileProduction()
{
	//profileProductionVector.clear();
	qDeleteAll(profileProductionVector);
	// comment below out gene
	//profileProductionVector.clear();
	//
	//
	//
	for (int i=0;i<profileProductionVector.count();i++)
	{
		profileProductionClass *p=profileProductionVector[i];
		mainLayout->removeWidget(p);
		profileProductionVector.remove(i);
		//put comments  back if it fails Gene ie //delete p
		//delete p;
		//
	i--;
	}
	// uncomment for by gene
	if (moreProductionAvailable)
	{
		mainLayout->removeWidget(moreProductionAvailable);
		delete moreProductionAvailable;
		moreProductionAvailable=NULL;
	}
}
void profileProductionLayoutClass::appendProfileProduction(profileProductionClass* p)
{
	profileProductionVector<<p;
	connect(p,SIGNAL(selectedSignal(profileProductionClass*,bool)),this,SLOT(profileProductionClassSelected(profileProductionClass*,bool)));
	connect(p,SIGNAL(productionChangedSignal(bool)),this,SLOT(productionChangedSlot(bool)));
	mainLayout->addWidget(p);
}
void profileProductionLayoutClass::productionChangedSlot(bool needsReloadFromDatabase)
{
	emit productionChangedSignal(needsReloadFromDatabase);
}
QVector<profileProductionClass*> profileProductionLayoutClass::getProfileProductionVector()
{
	return profileProductionVector;
}
profileProductionClass *profileProductionLayoutClass::getSelectedProfileProduction()
{
	profileProductionClass * ans=NULL;
	for (int i=0;i<profileProductionVector.count();i++)
	{
		if (profileProductionVector[i]->getIsSelected())
		{
			ans=profileProductionVector[i];
			break;
		}
	}
	return ans;
}

void profileProductionLayoutClass::moveProfileLeft()
{
	int index=-1;
	for (int i=0;i<profileProductionVector.count();i++)
	{
		if (profileProductionVector[i]->getIsSelected())
		{
			index=i;
			break;
		}
	}
	if ((index>=0)&&(index!=0))
	{
		if (index==1)
			needsToBeLoadedFromDatabase=true;
		profileProductionClass* slected=profileProductionVector[index];
		int leftProductionOrder=profileProductionVector[index-1]->getProductionOrder();
		int rightProductionOrder=profileProductionVector[index]->getProductionOrder();
		profileProductionVector[index-1]->setProductionOrder(rightProductionOrder);
		profileProductionVector[index]->setProductionOrder(leftProductionOrder);
		profileProductionVector[index]=profileProductionVector[index-1];
		profileProductionVector[index-1]=slected;
		mainScrollArea->ensureWidgetVisible(slected);
		//
		for (int i=0;i<profileProductionVector.count();i++)
			mainLayout->removeWidget(profileProductionVector[i]);
		for (int i=0;i<profileProductionVector.count();i++)
			mainLayout->addWidget(profileProductionVector[i]);
		emit productionChangedSignal(false);
	}
}
void profileProductionLayoutClass::moveProfileRight()
{
	int index=-1;
	for (int i=0;i<profileProductionVector.count();i++)
	{
		if (profileProductionVector[i]->getIsSelected())
		{
			index=i;
			break;
		}
	}
	if ((index>=0)&&(index!=(profileProductionVector.count()-1)))
	{
		if (index==0)
			needsToBeLoadedFromDatabase=true;
		profileProductionClass* slected=profileProductionVector[index];
		if (slected->getProductionStatus()!=PRODUCTION_STATUS_SCHEDULED)
			return;
		int leftProductionOrder=profileProductionVector[index]->getProductionOrder();
		int rightProductionOrder=profileProductionVector[index+1]->getProductionOrder();
		profileProductionVector[index+1]->setProductionOrder(leftProductionOrder);
		profileProductionVector[index]->setProductionOrder(rightProductionOrder);
		profileProductionVector[index]=profileProductionVector[index+1];
		profileProductionVector[index+1]=slected;
		mainScrollArea->ensureWidgetVisible(slected);
		//
		for (int i=0;i<profileProductionVector.count();i++)
			mainLayout->removeWidget(profileProductionVector[i]);
		for (int i=0;i<profileProductionVector.count();i++)
			mainLayout->addWidget(profileProductionVector[i]);
		emit productionChangedSignal(false);
	}
}
void profileProductionLayoutClass::deleteProfile()
{
	int index=-1;
	for (int i=0;i<profileProductionVector.count();i++)
	{
		if (profileProductionVector[i]->getIsSelected())
		{
			index=i;
			break;
		}
	}
	if (index>=0)
	{
		profileProductionClass* slected=profileProductionVector[index];
		apexExecQuery(QString("delete from epicsProduction2 where tid=%1").arg(slected->getProfilesEpicsTableId()));
		LOG(QString("user deleted production with for die:%1 and tid%2").arg(slected->getProfileCode()).arg(slected->getProfilesEpicsTableId()));
	}
}

void profileProductionLayoutClass::profileProductionClassSelected(profileProductionClass *pp,bool selected)
{
	for (int i=0;i<profileProductionVector.count();i++)
	{
		if (selected&&(profileProductionVector[i]==pp))
		{
			profileProductionVector[i]->setIsSelected(true);
			emit productionProfileSelectedSignal(profileProductionVector[i]);
		}
		else
			profileProductionVector[i]->setIsSelected(false);
	}
}
bool profileProductionLayoutClass::getNeedsToBeLoadedFromDatabase()
{
	return needsToBeLoadedFromDatabase;
}
void profileProductionLayoutClass::setNeedsToBeLoadedFromDatabase(int c,int force)
{
	if (force||(needsToBeLoadedFromDatabase!=c))
	{
		needsToBeLoadedFromDatabase=c;
		for (int i=0;i<profileProductionVector.count();i++)
		{
			profileProductionVector[i]->setNeedsToBeLoadedFromDatabase(needsToBeLoadedFromDatabase);
			profileProductionVector[i]->setPrivileges(needsToBeLoadedFromDatabase?0:PRIVILEGETYPE);
		}
	}
}

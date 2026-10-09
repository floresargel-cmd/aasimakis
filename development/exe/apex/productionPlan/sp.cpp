#include "sp.h"
defineSpacersClass::defineSpacersClass(int basketDBTableId_,basketClass *b):QDialog(b)
{
	setWindowTitle(tr("Define basket spacers.."));
	setWindowIcon(QIcon(":/logo_16x16.png"));
	this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px} QCheckBox{font-size:%1px} QSpinBox{font-size:%1px}").arg(20));
	//
	basketDBTableId=basketDBTableId_;
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	//basketNumberLayout
	QHBoxLayout *basketNumberLayout=new QHBoxLayout();mainLayout->addLayout(basketNumberLayout);
	QHBoxLayout *midLayout=new QHBoxLayout();mainLayout->addLayout(midLayout);
	basketNumberLayout->addWidget(new QLabel(tr("Basket number:")));
	basketNumberSpinBox=new QSpinBox(this);basketNumberLayout->addWidget(basketNumberSpinBox);
	basketNumberSpinBox->blockSignals(true);
	basketNumberSpinBox->setMinimum(1);
	basketNumberSpinBox->setMaximum(INT_MAX);
	basketNumberSpinBox->setValue(basketDBTableId);
	basketNumberSpinBox->blockSignals(false);
	layersLayout=new QVBoxLayout();midLayout->addLayout(layersLayout);
	profilesQuandityLayout=new QGridLayout();midLayout->addLayout(profilesQuandityLayout);
	//
	confirmBtn=new QPushButton(tr("Confirm"),this);
	connect(confirmBtn,SIGNAL(clicked()),this,SLOT(confirmBtnSlot()));
	mainLayout->addWidget(confirmBtn);
	mainLayout->addStretch(1);
	updateSpacersFromBasket(b);
}
defineSpacersClass::~defineSpacersClass()
{
}
/////
void defineSpacersClass::updateSpacersFromBasket(basketClass *b)
{
	int combsWord=b->getStackerPlcCombsWord();
	QVector<bool> combsVector=deprecatedCalculateBits(combsWord);
	for (int i=0;i<MAX_LAYERS_IN_A_BASKET;i++)
	{
		layerOfSpacersClass *l;
#ifdef USE_SPACERS_INFO_FROM_PLC
			l=new layerOfSpacersClass(i,combsVector[i],b->getStackerPlcWord(i));
#else
			l=new layerOfSpacersClass(i,b->getDatabaseWord(i));
#endif
		layerOfSpacersClassVector<<l;
	}
	//fill layout
	for (int i=(layerOfSpacersClassVector.count()-1);i>=0;i--)
		layersLayout->addWidget(layerOfSpacersClassVector[i]);
	//calculate
	layerOfSpacersClassVector[0]->setNextLayerClass(layerOfSpacersClassVector[1]);
	for (int i=1;i<layerOfSpacersClassVector.count()-1;i++)
	{
		layerOfSpacersClassVector[i]->setNextLayerClass(layerOfSpacersClassVector[i+1]);
		layerOfSpacersClassVector[i]->setPreviousLayerClass(layerOfSpacersClassVector[i-1]);
	}
	layerOfSpacersClassVector.last()->setPreviousLayerClass(layerOfSpacersClassVector[layerOfSpacersClassVector.count()-2]);
	layerOfSpacersClassVector[0]->validate();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	QVector<int> tableIds;
	QVector<int> codesIds;
	QVector<QColor> colors;
	QVector<int> profilesMultiplicity;
	for (int i=0;i<b->getNumberOfLayers();i++)
	{
		tableIds<<b->getLayer(i)->getProfilesEpicsTableId();
		codesIds<<b->getLayer(i)->getProfilesCodesTableId();
		colors<<b->getLayer(i)->getProfilesColors();
		profilesMultiplicity<<b->getLayer(i)->getProfilesMultiplicity();
	}
	QVector<int> uniqeTableIds;
	QVector<int> uniqeCodesIds;
	QVector<QColor> uniqeColors;
	QVector<int> uniqeProfilesMultiplicity;
	for (int i=0;i<tableIds.count();i++)
	{
		if (!uniqeTableIds.contains(tableIds[i]))
		{
			uniqeTableIds<<tableIds[i];
			uniqeCodesIds<<codesIds[i];
			uniqeColors<<colors[i];
			uniqeProfilesMultiplicity<<profilesMultiplicity[i];
		}
	}
	//
	profilesQuandityLayout->setRowStretch(0,1);
	profilesQuandityLayout->addWidget(new QLabel(tr("Die")),1,0);
	profilesQuandityLayout->addWidget(new QLabel(tr("Quantity")),1,1);
	int a=profileCodesSpinBoxVector.count();
	//for (int i=uniqeTableIds.count()-1;i>=0;i--)
	for (int i=0;i<uniqeTableIds.count();i++)
	{
		QSpinBox *pSpin=new QSpinBox();
		pSpin->setMinimum(0);
		pSpin->setMaximum(INT_MAX);
		pSpin->setValue(uniqeCodesIds[i]);
		QPalette pSpinPalette(pSpin->palette());
		pSpinPalette.setColor(QPalette::Active,QPalette::Base,uniqeColors[i]);
		pSpinPalette.setColor(QPalette::Active,QPalette::Text,QColor(255,255,255));
		pSpin->setPalette(pSpinPalette);

		profileCodesSpinBoxVector<<pSpin;
		epicsTableIdVector<<uniqeTableIds[i];
		profilesQuandityLayout->addWidget(pSpin,uniqeTableIds.count()-i+2,0);

		QSpinBox *qSpin=new QSpinBox();
		qSpin->setMinimum(0);
		qSpin->setMaximum(INT_MAX);
		int no=uniqeProfilesMultiplicity[i]*tableIds.count(uniqeTableIds[i]);
		profileQuantityThatIsNeededAccordingToTheLayout<<no;
		int noFromEpics=GETFIRST(QString("select top 1 quantity from PEBaskets where soNum=(select soNum from epicsProduction2 where tid=%1) and processed=0 and basketID=%2 order by tid").arg(uniqeTableIds[i]).arg(basketNumberSpinBox->value())).toInt();
		if (noFromEpics!=0)
			no=noFromEpics;
		qSpin->setValue(no);
		QPalette qSpinPalette(qSpin->palette());
		qSpinPalette.setColor(QPalette::Active,QPalette::Base,uniqeColors[i]);
		qSpinPalette.setColor(QPalette::Active,QPalette::Text,QColor(255,255,255));
		qSpin->setPalette(pSpinPalette);

		profileQuantitySpinBoxVector<<qSpin;
		profilesQuandityLayout->addWidget(qSpin,uniqeTableIds.count()-i+2 ,1);
	}
	//add more                         //don't do it. i do not know the tid only the die, if there is an other die with the same code in the table it will also change
	//for (int j=uniqeTableIds.count();j<uniqeTableIds.count()+5;j++)
	//{
	//	QSpinBox *pSpin=new QSpinBox();
	//	pSpin->setMinimum(0);
	//	pSpin->setMaximum(INT_MAX);
	//	pSpin->setValue(0);
	//	profileCodesSpinBoxVector<<pSpin;
	//	profilesQuandityLayout->addWidget(pSpin,j+1,0);

	//	QSpinBox *qSpin=new QSpinBox();
	//	qSpin->setMinimum(0);
	//	qSpin->setMaximum(INT_MAX);
	//	qSpin->setValue(0);
	//	profileQuantitySpinBoxVector<<qSpin;
	//	profilesQuandityLayout->addWidget(qSpin,j+1,1);
	//}
}
int defineSpacersClass::getBasketNumber()
{
	return basketNumberSpinBox->value();
}
//int defineSpacersClass::getActiveLayers()
//{
//	int ans=0;
//	for (int i=0;i<layerOfSpacersClassVector.count();i++)
//	{
//		if (layerOfSpacersClassVector[i]->isActive())
//			ans++;
//	}
//	return ans;
//}
//layerOfSpacersClass* defineSpacersClass::getLayer(int index)
//{
//	if (index<layerOfSpacersClassVector.count())
//		return layerOfSpacersClassVector[index];
//	return NULL;
//}
void defineSpacersClass::confirmBtnSlot()
{
	//spacers
	apexExecQuery(QString("delete from basketSpacers where basket=%1").arg(basketNumberSpinBox->value()));
	for (int i=0;i<layerOfSpacersClassVector.count();i++)
	{
		if (layerOfSpacersClassVector[i]->isActive())
		{
			apexExecQuery(QString("insert into basketSpacers (basket,layer,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11)").arg(basketNumberSpinBox->value()).arg(i+1).arg(layerOfSpacersClassVector[i]->getIsOnComb()).arg(layerOfSpacersClassVector[i]->getSp1()).arg(layerOfSpacersClassVector[i]->getSp2()).arg(layerOfSpacersClassVector[i]->getSp3()).arg(layerOfSpacersClassVector[i]->getSp4()).arg(layerOfSpacersClassVector[i]->getSp5()).arg(layerOfSpacersClassVector[i]->getSp6()).arg(layerOfSpacersClassVector[i]->getSp7()).arg(layerOfSpacersClassVector[i]->getSp8()));
		}
	}
	//profiles
	for (int i=0;i<profileCodesSpinBoxVector.count();i++)
	{
		if ( (profileCodesSpinBoxVector[i]->value()>0)&&(profileQuantitySpinBoxVector[i]->value()>0))
		{
			if (i<epicsTableIdVector.count())
			{
				int tid=epicsTableIdVector[i];
				int n=profileQuantitySpinBoxVector[i]->value();
				int noOfProfilesThatWhereAddedToTheBasket=profileQuantitySpinBoxVector[i]->value();
				if (i<(epicsTableIdVector.count()-1))//if not the last then the noOfProfilesThatWhereAddedToTheBasket=max quantity
				{
					//noOfProfilesThatWhereAddedToTheBasket=profileQuantityThatIsNeededAccordingToTheLayout[i];
				}
				apexExecQuery(QString("update epicsProduction2 set quantityInBaskets=((select quantityInBaskets from epicsProduction2 where tid=%1)+%2) where tid=%1").arg(epicsTableIdVector[i]).arg(noOfProfilesThatWhereAddedToTheBasket));
				// commentout by gene apexExecQuery(QString("update PEBaskets set processed=1 where soNum=(select soNum from epicsProduction2 where tid=%1) and basketID=%2").arg(epicsTableIdVector[i]).arg(basketNumberSpinBox->value()));
			}
			//else
//				execQuery(QString("update epicsProduction2 set quantityInBaskets=((select quantityInBaskets from epicsProduction2 where die=%1)+%2) where die=%1").arg(profileCodesSpinBoxVector[i]->value()).arg(profileQuantitySpinBoxVector[i]->value()));
		}
	}
	QDialog::accept();
}

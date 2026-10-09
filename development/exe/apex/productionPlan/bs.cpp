#include "bs.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////basketProfileGroupClass//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
basketProfileGroupClass::basketProfileGroupClass(profileProductionClass* profileProduction_)
{
	signed int lala=INT_MIN;
	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setAcceptedMouseButtons(Qt::LeftButton);
	//
	needsToBeCalculated=0;
	noOfProfiles=0;
	length=0.;
	isMouseOver=isSelected=0;
	profileProduction=profileProduction_;
}
basketProfileGroupClass::~basketProfileGroupClass()
{
	//qDeleteAll(layerProfilesVector);
	//layerProfilesVector.clear();
}
void basketProfileGroupClass::mousePressEvent(QGraphicsSceneMouseEvent *)
{
	if (!needsToBeCalculated)
		setCursor(Qt::ClosedHandCursor);
}
void basketProfileGroupClass::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
	if (needsToBeCalculated||(QLineF(ev->screenPos(),ev->buttonDownScreenPos(Qt::LeftButton)).length()<QApplication::startDragDistance()))
		return;
	QDrag *drag = new QDrag(ev->widget());
	QMimeData *mime=new QMimeData;
	mime->setText(QString("%1").arg(getTableId()));
	drag->setMimeData(mime);
	double scale=2.;
	QPixmap pixmap(scale*profileProduction->getProfileGraphics()->boundingRect().width(),scale*profileProduction->getProfileGraphics()->boundingRect().height()+20);
  QPainter painter(&pixmap);
	painter.fillRect(pixmap.rect(),QBrush(QColor(230,230,240)));
	painter.setPen(QPen(profileProduction->getColor()));
	painter.setTransform(QTransform(scale,0.,0.,-scale,-scale*profileProduction->getProfileGraphics()->boundingRect().left(),scale*profileProduction->getProfileGraphics()->boundingRect().bottom()));
	painter.drawPath(profileProduction->getProfilePath());

	painter.resetTransform();
	painter.setPen(QPen(QColor(0,0,100)));
	painter.setFont(QFont("Arial",12));
	painter.drawText(0,scale*profileProduction->getProfileGraphics()->boundingRect().height(),scale*profileProduction->getProfileGraphics()->boundingRect().width(),20,Qt::AlignCenter,QString("%1").arg(profileProduction->getProfileCode()));
  painter.end();
  drag->setPixmap(pixmap);
	drag->exec();
	setCursor(Qt::OpenHandCursor);
}
void basketProfileGroupClass::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
	setCursor(Qt::OpenHandCursor);
}
void basketProfileGroupClass::dragEnterEvent(QGraphicsSceneDragDropEvent *ev)
{
	if (ev->mimeData()->hasText()&&(ev->mimeData()->text().toInt()!=getTableId()))
	{
		ev->setAccepted(true);
		isDragging = true;
		update();
	}
	else
		ev->setAccepted(false);
}
void basketProfileGroupClass::dragLeaveEvent(QGraphicsSceneDragDropEvent *ev)
{
	Q_UNUSED(ev);
	isDragging = false;
	update();
}
void basketProfileGroupClass::dropEvent(QGraphicsSceneDragDropEvent *ev)
{
	isDragging = false;
	QString a=ev->mimeData()->text();
	if (ev->mimeData()->hasText())
	{
		QString msg=tr("Do you want to insert the production %1(die %2) before production %3(die %4)?").arg(GETONE(QString("select soNum from epicsProduction2 where tid=%1").arg(ev->mimeData()->text())).toString()).arg(GETONE(QString("select die from epicsProduction2 where tid=%1").arg(ev->mimeData()->text())).toString()).arg(GETONE(QString("select soNum from epicsProduction2 where tid=%1").arg(getTableId())).toString()).arg(profileProduction->getProfileCode());
		if (QMessageBox::question(NULL,tr("Change profile sequence?"),msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes)==QMessageBox::Yes)
		{
			apexExecQuery(QString(
			"declare @tidOfThis int;set @tidOfThis=%1; "
			"declare @tidOfDropped int;set @tidOfDropped=%2; "
			"declare @productionOrderOfThis int;set @productionOrderOfThis=(select productionOrder from [productionPlan].[dbo].[epicsProduction2] where tid=@tidOfThis); "
			"update [productionPlan].[dbo].[epicsProduction2] set productionOrder=@productionOrderOfThis where tid=@tidOfDropped; "
			"DECLARE @tid int; "
			"DECLARE @incrementOrder int; "
			"set @incrementOrder=@productionOrderOfThis; "
			"DECLARE crs CURSOR READ_ONLY "
			"FOR "
			"	SELECT tid FROM [productionPlan].[dbo].[epicsProduction2] where productionOrder>@productionOrderOfThis or tid=@tidOfThis order by productionOrder "
			"		OPEN crs "
			"		FETCH NEXT FROM crs INTO @tid "
			"		WHILE @@FETCH_STATUS = 0 "
			"		BEGIN "
			"			set @incrementOrder=@incrementOrder+1; "
			"			update [productionPlan].[dbo].[epicsProduction2] set productionOrder=@incrementOrder where tid=@tid; "
			"			FETCH NEXT FROM crs INTO @tid "
			"		END "
			"CLOSE crs; "
			"DEALLOCATE crs; "
			).arg(getTableId()).arg(ev->mimeData()->text())
			);
			emit productionSequenceChangedSignal();
		}
	}
	update();
}
////
int basketProfileGroupClass::getTableId()
{
	if (profileProduction)
		return profileProduction->getProfilesEpicsTableId();
	return 0;
}
double basketProfileGroupClass::getLength()
{
	return length;
}
void basketProfileGroupClass::setNeedsToBeCalculated(int needsToBeCalculated_)
{
	if (needsToBeCalculated!=needsToBeCalculated_)
	{
		needsToBeCalculated=needsToBeCalculated_;
	}
}
void basketProfileGroupClass::addProfile(layerProfileGroupClass *layerProfileGroup)
{
	length=layerProfileGroup->getProfileProduction()->getProfileLength();
	layerProfilesVector<<layerProfileGroup;
	noOfProfiles+=layerProfileGroup->getNumberOfProfiles();
	path.addPath(layerProfileGroup->getPath());
}
QRectF basketProfileGroupClass::boundingRect()const
{
	return path.boundingRect();
}
void basketProfileGroupClass::paint(QPainter *painter,const QStyleOptionGraphicsItem *opt,QWidget *w)
{
	if (profileProduction)
	{
		if (isMouseOver||isSelected)
			painter->setPen(QPen(profileProduction->getColor().darker(200)));
		else
			painter->setPen(QPen(profileProduction->getColor()));
		painter->drawPath(path);
		if (profileProduction->getProfileLength()>7365)
		{
			painter->setPen(QPen(QColor(200,0,0),50));
			painter->drawRect(path.boundingRect());
		}
	}
}
void basketProfileGroupClass::setMouseOverProfileProducton(profileProductionClass* p)
{
	if (profileProduction)
	{
		if (p)
		{
			if (p->getProfilesEpicsTableId()==profileProduction->getProfilesEpicsTableId())
				isMouseOver=true;
		}
		else
			isMouseOver=false;
	}
}
profileProductionClass *basketProfileGroupClass::getProfileProduction()
{
	return profileProduction;
}
QPainterPath basketProfileGroupClass::getProfilesPath()
{
	return path;
	//return QGraphicsRectItem(path.boundingRect()).shape();
}

void basketProfileGroupClass::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
	if (!needsToBeCalculated)
		emit mouseEnterSignal(profileProduction);
	QGraphicsObject::hoverEnterEvent(ev);
}
void basketProfileGroupClass::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
	if (!needsToBeCalculated)
		emit mouseLeaveSignal(profileProduction);
	QGraphicsObject::hoverLeaveEvent(ev);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////basketClass//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
basketClass::basketClass(int basketNumber_,bool firstToComeOutOfTheStacker_,QWidget *p):QWidget(p)
{
	setAutoFillBackground(true);
	basketNumber=basketNumber_;
	firstToComeOutOfTheStacker=firstToComeOutOfTheStacker_;
	isSelected=false;
	totalWeight=0.;
	tcpServer=NULL;
	maxWeightLimitReached=false;
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	basketReadyBtn=NULL;
	if (firstToComeOutOfTheStacker&&HAVE_PLC_COMMS)
	{
		basketReadyBtn=new QPushButton(QString("Ready"));
		connect(basketReadyBtn,SIGNAL(clicked()),this,SLOT(basketReadyBtnSlot()));
		mainLayout->addWidget(basketReadyBtn);
	}
	myScene=new QGraphicsScene(this);//list of objects
	myQGraphicsView=new dxfQGraphicsViewClass(this,5,0.35);//viewer
	myQGraphicsView->setMinimumSize(600,350);
//	myQGraphicsView->setMinimumSize(600, 100);
	myQGraphicsView->scale(0.4,0.4);
	myQGraphicsView->setScene(myScene);//setScene
	QBrush b=QBrush(Qt::SolidPattern);
	b.setColor(QColor(230,230,230));
	myQGraphicsView->setBackgroundBrush(b);
	mainLayout->addWidget(myQGraphicsView);
	basketGraphics=new dxfGroupOfGraphicsItemClass(":/dxfs/basketSide.dxf",0.0,true,QColor(75,50,50));
	myScene->addItem(basketGraphics);
	basketGraphics->setFlag(QGraphicsItem::ItemIsMovable,false);
	//

	//
	QGridLayout *dataLayout=new QGridLayout();mainLayout->addLayout(dataLayout);
	//id
	dataLayout->addWidget(new QLabel(tr("No:%1").arg(basketNumber+1)),0,0);
	dataLayout->addWidget(totalWeightLabel=new QLabel(tr("...")),0,1);
	//
	setPrivileges(PRIVILEGETYPE);
}
basketClass::~basketClass()
{
	delete basketGraphics;
	qDeleteAll(layersVector);
	layersVector.clear();
	qDeleteAll(profilesGroupsVector);
	profilesGroupsVector.clear();
	//return;
	//delete basketGraphics;
	//for (int i=0;i<profilesGroupsVector.count();i++)
	//	delete profilesGroupsVector[i];
	//profilesGroupsVector.clear();
	//for (int i=0;i<layersVector.count();i++)
	//	delete layersVector[i];
	//layersVector.clear();
	//stackerPlcWordsVector.clear();
}
//
void basketClass::enterEvent(QEvent *ev)
{
	setBackgroundRole(QPalette::Midlight);
}
void basketClass::leaveEvent(QEvent *ev)
{
	if (isSelected)
		setBackgroundRole(QPalette::Highlight);
	else
	{
		if (maxProfileLength>7365)
			setBackgroundRole(QPalette::Light);
		else
			setBackgroundRole(QPalette::Mid);
	}
}
void basketClass::mousePressEvent(QMouseEvent *ev)
{
	if (isSelected)
		emit selectedSignal(this,false);
	else
		emit selectedSignal(this,true);
}
void basketClass::setIsSelected(bool s)
{
	isSelected=s;
	if (isSelected)
		setBackgroundRole(QPalette::Highlight);
	else
	{
		if (maxProfileLength>7365)
			setBackgroundRole(QPalette::Light);
		else
			setBackgroundRole(QPalette::Mid);
	}
}
bool basketClass::getIsSelected()
{
	return isSelected;
}
int basketClass::getFirstProfileCode()
{
	if (layersVector.count()>0)
		return layersVector[0]->getFirstProfile();
	return 0;
}
/////
void basketClass::setTcpServer(tcpServerClass *tcpServer_)
{
	tcpServer=tcpServer_;
}
void basketClass::sendDatabaseSpacerDataToPlc(int firstIndex)//only when i have recalculate
{
	if (!tcpServer)
		return;
	if (HAVE_PLC_COMMS)
	{
		QVector<bool> combBitsArray;combBitsArray.resize(32);combBitsArray.fill(false);
		for (int i=0;i<=15;i++)
		{
			if (i<layersVector.count())
			{
				tcpServer->sendString(QString("%1|%2|%3").arg(STACKER_TABLE_WRITE).arg(firstIndex+i).arg(layersVector[i]->getSpacersWord()));
				combBitsArray[i]=layersVector[i]->getIsOnComb();
			}
			else
				tcpServer->sendString(QString("%1|%2|%3").arg(STACKER_TABLE_WRITE).arg(firstIndex+i).arg(0));
		}
		tcpServer->sendString(QString("%1|%2|%3").arg(STACKER_TABLE_WRITE).arg(firstIndex+15).arg(deprecatedCalculateInt(combBitsArray)));
	}
}
void basketClass::setStackerPlcWords(QVector<int> v)
{
	if (HAVE_PLC_COMMS)
		stackerPlcWordsVector=v;
}
void basketClass::setStackerCombsWord(int v)
{
	if (HAVE_PLC_COMMS)
		stackerPlcCombsWord=v;
}
int basketClass::getStackerPlcWord(int index)
{
	if (index<(stackerPlcWordsVector.count()+1))
		return stackerPlcWordsVector[index+1];
	return 0;
}
int basketClass::getStackerPlcCombsWord()
{
	return stackerPlcCombsWord;
}
int basketClass::getDatabaseWord(int index)
{
	if (index<layersVector.count())
		return layersVector[index]->getSpacersWord();
	return 0;
}
int basketClass::getCombNumberOfLayer(int index)
{
	if (index<layersVector.count())
		return layersVector[index]->getCombNumber();
	return 0;
}
void basketClass::calculate()
{
	totalWeight=maxProfileLength=0.0;
	for (int i=0;i<layersVector.count();i++)
	{
		totalWeight+=layersVector[i]->getTotalWeight();
		maxProfileLength=MAX(maxProfileLength,layersVector[i]->getMaxProfileLength());
	}
	totalWeightLabel->setText(QString("%1Kgr %2mm max").arg(totalWeight,8,'f',1).arg(maxProfileLength/*>7365*/,8,'f',1));
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
			if (maxWeightLimitReached)
			{
				QBrush b=QBrush(QImage(":overWeight.png"));
				//b.setColor(QColor(255,200,200));
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
}
void basketClass::setPrivileges(int privilegeType)
{
	if (basketReadyBtn)
	{
		if (!(privilegeType&PRIVILEGE_TYPE_STACKER))
			basketReadyBtn->setEnabled(false);
		else
			basketReadyBtn->setEnabled(true);
	}
}
//int basketClass::getBasketNumber()
//{
//	return basketNumber;
//}
void basketClass::basketReadyBtnSlot()
{
	defineSpacersClass *defineSpacers=new defineSpacersClass(GETONE("select basketTableId from positions where pid=101",BASKET_CRANE_DB_CONNECTION).toInt(),this);
	if (defineSpacers->exec()==QDialog::Accepted)
	{
		apexExecQuery(QString("update users set needsUpdate=1"));
	}
	delete defineSpacers;
}
double basketClass::getTotalHeight()
{
	if (layersVector.count()==0)
		return FIRST_LAYER_HEIGTH;
	else
		return layersVector.last()->getTotalHeight();
}
double basketClass::getTotalWeight()
{
	return totalWeight;
}
int basketClass::getMaxUsedComb()
{
	int ans=0;
	for (int i=0;i<layersVector.count();i++)
	{
		if (ans<layersVector[i]->getCombNumber())
			ans=layersVector[i]->getCombNumber();
	}
	return ans;
}
int basketClass::getNumberOfLayers()
{
	return layersVector.count();
}
profileLayerClass* basketClass::getLayer(int index)
{
	return layersVector[index];
}
bool basketClass::hasProfile(int code)
{
	for (int i=0;i<layersVector.count();i++)
	{
		if (layersVector[i]->hasProfile(code))
			return true;
	}
	return false;
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
		{
			layersVector<<new profileLayerClass(layersVector.count(),comb,layerHeight,myScene);
		}
		else
			return false;
	}
	else
	{
		if ((currentHeight+SPACER_THIKNESS+p->getProfileHeight())<MAX_HEIGHT_OF_LAYERS_IN_A_BASKET)
		{
			layersVector<<new profileLayerClass(layersVector.count(),0,currentHeight,myScene);
		}
		else
			return false;
	}
	return true;
}
int basketClass::addProfiles(profileProductionClass* p,int number)
{
	if ((p->getWeightOfOneProfile()+totalWeight)>MAX_WEIGHT_OF_BASKET)
	{
		maxWeightLimitReached=true;
		calculate();
		return 0;
	}
	int totalProfilesThatWarePutIntoTheLayer=0;
	if ((layersVector.count()==0)||(p->getIsToBePutOnNextLayer())||   (p->getIsOnComb()&&((layersVector.count()>0)&&(!layersVector.last()->getIsOnComb())))  )
	{

		if (!addEmptyLayerForProfiles(p,false))
			return totalProfilesThatWarePutIntoTheLayer;
	}
	//int profilesThaCanBePutBecauseOfMaxWeightLimitation=(MAX_WEIGHT_OF_BASKET-totalWeight)/p->getWeightOfOneProfile();
	int profilesLeftToPutIntoTheLayer=number;//<profilesThaCanBePutBecauseOfMaxWeightLimitation?number:profilesThaCanBePutBecauseOfMaxWeightLimitation;
	while (profilesLeftToPutIntoTheLayer>0)
	{
		calculate();
		int profilesThaCanBePutBecauseOfMaxWeightLimitation=(MAX_WEIGHT_OF_BASKET-totalWeight-(p->getSpacers().count(true)*SPACER_WEIGHT))/p->getWeightOfOneProfile();
		if (profilesThaCanBePutBecauseOfMaxWeightLimitation<profilesLeftToPutIntoTheLayer)
			maxWeightLimitReached=true;
		int profilesToPutIntoTheLayer=profilesLeftToPutIntoTheLayer;
		if (profilesToPutIntoTheLayer>profilesThaCanBePutBecauseOfMaxWeightLimitation)
			profilesToPutIntoTheLayer=profilesThaCanBePutBecauseOfMaxWeightLimitation;
		int profilesThatWarePutIntoTheLayer=layersVector.last()->addProfiles(p,profilesToPutIntoTheLayer);
		if (profilesThatWarePutIntoTheLayer==0)
		{
			if ( (MAX_WEIGHT_OF_BASKET-totalWeight-(p->getSpacers().count(true)*SPACER_WEIGHT))<p->getWeightOfOneProfile() )
			{
				maxWeightLimitReached=true;
				break;
			}
			if (!addEmptyLayerForProfiles(p,!layersVector.last()->getCanSupportLayer()))
			{
				maxWeightLimitReached=false;//it is because of max height
				break;
			}
		}
		profilesLeftToPutIntoTheLayer-=profilesThatWarePutIntoTheLayer;
		totalProfilesThatWarePutIntoTheLayer+=profilesThatWarePutIntoTheLayer;
	}
	calculate();
	if (maxProfileLength>7365)
		setBackgroundRole(QPalette::Light);
	else
		setBackgroundRole(QPalette::Mid);
	return totalProfilesThatWarePutIntoTheLayer;
}
void basketClass::calculateGroups()
{
	for (int i=0;i<layersVector.count();i++)
	{
		QVector<layerProfileGroupClass*> layerGroups=layersVector[i]->getProfileGroups();
		for (int j=0;j<layerGroups.count();j++)
		{
			bool found=false;
			for (int k=0;k<profilesGroupsVector.count();k++)
			{
				if (profilesGroupsVector[k]->getTableId()==layerGroups[j]->getTableId())
				{
					profilesGroupsVector[k]->addProfile(layerGroups[j]);
					found=true;
					break;
				}
			}
			if (!found)
			{
				basketProfileGroupClass *b=new basketProfileGroupClass(layerGroups[j]->getProfileProduction());
				//myScene->addItem((QGraphicsItem*)b);
				b->addProfile(layerGroups[j]);
				profilesGroupsVector<<b;
			}
		}
	}
}
QVector<basketProfileGroupClass*> basketClass::getProfilesGroupsVector()
{
	return profilesGroupsVector;
}

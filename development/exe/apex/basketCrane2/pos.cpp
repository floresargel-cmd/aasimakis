#include "pch.h"
#include "pos.h"
double textScale=15.;
double exportTextScale=25.;
posClass::posClass(allPossClass *allPos_,int posNumber_,int posIndex_,float plcXPos_,float plcYPos_,float plcZPos_,float gXOffset_,float gYOffset_,
	QGraphicsScene *scene_,gItemClass *posItem_,gItemClass *mouseOverItem_,gItemClass *basketItem_,gItemClass *profilesItem_,gItemClass *lockedItem_,gItemClass *fromItem_,gItemClass *toItem_
	,bool handleMousePressed,QWidget *p)
{
	tuxip=NULL;
	plcIndex=-1;
	setVisible(false);
	allPos=allPos_;
	posNumber=posNumber_;
	posIndex=posIndex_;
	plcXPos=plcXPos_;
	plcYPos=plcYPos_;
	plcZPos=plcZPos_;
	gXOffset=gXOffset_;
	gYOffset=gYOffset_;
	abovePos=belowPos=NULL;
	displayMode=displayToSelectFrom;
	scene=scene_;
	isFrom=isTo=false;
	basketNum=-1;
	isCranePlcPos=(posNumber==toRotatingSouthPlcId)||(posNumber==toRotatingNorthPlcId)||(posNumber==rotatingNorthPlcId)||(posNumber==rotatingSouthPlcId)
		||(posNumber==exit1PlcId)||(posNumber==exit2PlcId)||(posNumber==stackerWaitingPlcId);
	isCranePos=!((posNumber==rotatingNorthPlcId)||(posNumber==rotatingSouthPlcId)||(posNumber==exit1PlcId)||(posNumber==exit2PlcId)
		||(posNumber==stackerWaitingPlcId)||(posNumber==stackerWorkingingPlcId));
	fromItem=new gItemClass(fromItem_,0.,0.,this);scene->addItem(fromItem);
	toItem=new gItemClass(toItem_,0.,0.,this);scene->addItem(toItem);
	basketItem=new gItemClass(basketItem_,0.,0.,this);scene->addItem(basketItem);
	profilesItem=new gItemClass(profilesItem_,0.,0.,this);scene->addItem(profilesItem);
	lockedItem=new gItemClass(lockedItem_,0.,0.,this);scene->addItem(lockedItem);
	mouseOverItem=new gItemClass(mouseOverItem_,0.,0.,this);scene->addItem(mouseOverItem);mouseOverItem->setCursor(Qt::ClosedHandCursor);
	posItem=new gItemClass(posItem_,0.,0.,this);scene->addItem(posItem);

	posItemText=new QGraphicsSimpleTextItem();
	scene->addItem(posItemText);
	posItemText->setZValue(10);
	txtTransformation.scale(textScale,-textScale);
	posItemText->setTransform(txtTransformation,false);

	txtTransformation.reset();
	txtTransformation.scale(textScale,-textScale);
	profileTxtItem=new QGraphicsSimpleTextItem();
	scene->addItem(profileTxtItem);
	profileTxtItem->setZValue(10);
	profileTxtItem->setTransform(txtTransformation,false);
	setPlcPos(plcXPos,plcYPos,plcZPos);
	if (handleMousePressed)
		connect(mouseOverItem,SIGNAL(mousePressedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedSlot(QGraphicsSceneMouseEvent*)));
}
posClass::~posClass()
{
}
bool posClass::getIsFreeToMove()
{
	for (int i=0;i<obstaclePosV.count();i++)
	{
		if (obstaclePosV[i]->getHasBasket())
			return false;
	}
	if (
		((abovePos)&&(abovePos->getHasBasket()))
		||
		((belowPos)&&(!belowPos->getHasBasket()))
		)
		return false;
	return true;
}
void posClass::setDisplayMode(int m)
{
	if (isCranePos)
	{
		displayMode=m;
		updateItemsVisibility();
	}
}
void posClass::setAboveBelowPos(posClass *a,posClass *b)
{
	abovePos=a;
	belowPos=b;
}
void posClass::setObstaclePos(QVector<posClass*> obstaclePosV_)
{
	obstaclePosV=obstaclePosV_;
}
void posClass::setPlcPos(float x,float y,float z)
{
	float bWidth=1500.f;
	plcXPos=x;
	plcYPos=y;
	plcZPos=z;
	gXPos=gXOffset+plcXPos+posIndex*bWidth;
	gYPos=plcYPos+gYOffset;
	posItem->setPos(gXPos,gYPos);
	mouseOverItem->setPos(gXPos,gYPos);
	basketItem->setPos(gXPos,gYPos);
	profilesItem->setPos(gXPos,gYPos);
	lockedItem->setPos(gXPos,gYPos);
	fromItem->setPos(gXPos,gYPos);
	toItem->setPos(gXPos,gYPos);
	updateTxt();
}
void posClass::setPlcAdress(tuxipClass *tuxip_,QString plcTable_,int index_)
{
	tuxip=tuxip_;
	plcTable=plcTable_;
	plcIndex=index_;
}
void posClass::updateTxt()
{
	if (basketNum!=0)
	{
		QString txt=tr("%1:%2:%3").arg(basketNum).arg(getHoneyCombDestinationEnumClassStringG(honeyCombDestinationEnumOfTopProfile)).arg(getExportCrane2EnumOneCharacterClassStringG(exportEnum));
		posItemText->setText(txt);
		posItemText->setPos(gXPos-0.5*posItemText->boundingRect().width()*textScale,gYPos+0.5*posItem->boundingRect().height());
		posItemText->setVisible(true);
	}
	else
	{
		posItemText->setText(tr("%1").arg(posNumber+posIndex));
		posItemText->setVisible(false);
	}
	posItemText->setBrush(getExportCrane2EnumColorG(exportEnum));
	//profileTxt
	QString profileTxt;
	for (int i=0;i<contentsV.count();i++)
		profileTxt.append(tr("%1:%2:%3\n").arg(contentsV[i].profile).arg(contentsV[i].quantity).arg(getHoneyCombDestinationEnumClassStringG(contentsV[i].honeyCombDestinationEnum)));
	profileTxtItem->setText(profileTxt);
	profileTxtItem->setPos(gXPos-0.5*profileTxtItem->boundingRect().width()*textScale,gYPos+0.25*posItem->boundingRect().height());
	profileTxtItem->setVisible(contentsV.count());
}
void tempLogSetDestinations(int basket,int destination)
{
	QString logFileName=QString("tempLogSetDestinations.txt");
	QFile file(logFileName);
	if (!file.open(QIODevice::Append|QIODevice::Text))
		exit(0);
	QTextStream out(&file);
	out<<QString("%1:%2:%3\n").arg(timeForLog).arg(basket).arg(destination);
	file.close();
}

void posClass::setBasketId(int b,bool user,QString logInfo,int doNotCheckPosition,int doNotCheckIndex)
{
	if ((basketNum!=0)&&(b!=0)&&(user))
	{
		if ((basketNum!=b)&&(posNumber!=doNotCheckPosition))
			uExit(errorStr,QString("trying to declare basket %1 in position %2.%3, position %2.%3 allready has basket %4 declared").arg(b).arg(posNumber).arg(posIndex).arg(basketNum));
	}
	{
		QString posOfBasket=getDescriptionOfPosWithBasket(b,QVector<int>()<<doNotCheckPosition<<posNumber,QVector<int>()<<doNotCheckIndex<<posIndex);
		if (posOfBasket.isEmpty())
		{
			execQuery(QString("update positions set basketTableId=%1 where posNumber=%2 and posIndex=%3").arg(b).arg(posNumber).arg(posIndex),bDb);
			execQuery(QString("insert into c2logPos (pos,basket,info,time) values(%1,%2,'%3','%4')").arg(getPosIndexDisplayString()).arg(b).arg(logInfo).arg(timeForLog),bDb);
			if (tuxip)
			{
				tuxip->writeInteger(plcTable,plcIndex,b);
				if (posNumber==toRotatingNorthPlcId)
				{
					bool isForDestackerForSb=(getFirst(QString("select count(tId) from %1 where basket=%2").arg(crane2ExportsDestackerForSb).arg(b),bDb).toInt()!=0);
					bool isForAnodizing=(getFirst(QString("select count(tId) from %1 where basket=%2").arg(crane2ExportsAnodizing).arg(b),bDb).toInt()!=0);
					bool isForPackingDestacker1=(getFirst(QString("select count(tId) from %1 where basket=%2").arg(crane2ExportsPackingDestacker1Position).arg(b),bDb).toInt()!=0);
					bool isForPackingDestacker2=(getFirst(QString("select count(tId) from %1 where basket=%2").arg(crane2ExportsPackingDestacker2Position).arg(b),bDb).toInt()!=0);
					int dest=0;
					if (isForDestackerForSb)
					{
						if (getMaximumLength()<7821)
							dest=destinationStruct::destackerForSb;
						else
							dest=destinationStruct::anodizingOverLength;
					}
					if (isForAnodizing)
					{
						if (getMaximumLength()<7821)
							dest=destinationStruct::anodizing;
						else
							dest=destinationStruct::anodizingOverLength;
					}
					if (isForPackingDestacker1)
					{
						if (getMaximumLength()<=packingMaximumPacketLength)
							dest=destinationStruct::packingDestackerPos;
						else
							dest=destinationStruct::anodizingOverLength;
					}
					if (isForPackingDestacker2)
					{
						if (getMaximumLength()<=packingMaximumPacketLength)
							dest=destinationStruct::packingDestackerPos;
						else
							dest=destinationStruct::anodizingOverLength;
					}
					if (isForPackingDestacker1&&(dest==destinationStruct::packingDestackerPos))
						execQuery(QString("insert into %1 (basket,priority) select %2,isnull((select max(priority) from %1),0)+1 where not exists (select 1 from %1 where basket=%2)").arg(crane3ExportsDestackerPositions1).arg(b),bDb);
					if (isForPackingDestacker2&&(dest==destinationStruct::packingDestackerPos))
						execQuery(QString("insert into %1 (basket,priority) select %2,isnull((select max(priority) from %1),0)+1 where not exists (select 1 from %1 where basket=%2)").arg(crane3ExportsDestackerPositions2).arg(b),bDb);
					if (isForDestackerForSb||isForAnodizing||isForPackingDestacker1||isForPackingDestacker2)
					{
						tuxip->writeInteger("bConveyorData",plcIndex,dest);
						tempLogSetDestinations(b,dest);
					}
				}
			}
			loadFromDatabase();
		}
	}
	emit posChangedSignal();
}
void posClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)//for plc driver positions:isPlcPos==true
{
	if ((tuxip)&&/*(!tuxip->getHasError())&&*/(tuxip->getIp()==ip)&&(index==plcIndex))
	{
		if (table==plcTable)
		{
			setBasketId(value.toInt());
			execQuery(QString("update positions set basketTableId=%1 where posNumber=%2 and posIndex=%3").arg(value.toInt()).arg(posNumber).arg(posIndex),bDb);
			execQuery(QString("insert into c2logPos (pos,basket,info,time) values(%1,%2,'%3','%4')").arg(getPosIndexDisplayString()).arg(value.toInt()).arg("plc").arg(timeForLog),bDb);
			loadFromDatabase();
		}
		if (table=="bConveyorData")
			plcDestinationEnum=destinationStruct::plcValueEnum(value.toInt());
	}
	//if ((table=="toPcI")&&(index==24))
	//{
	//	if (posNumber==toRotatingNorthPlcId)
	//		isPlcReadyToGetBasket=getValueOfBit(value.toInt(),8);
	//}
	if ((table=="BConveyorState")&&(index==6))
	{
		if (posNumber==toRotatingNorthPlcId)
			isPlcReadyToGetBasket=getValueOfBit(value.toInt(),12);
	}
}
//int posClass::getBasketForInFromOut()
//{
//	if (getHasBasket())
//	{
//		if (plcDestination==destinationStruct::inFromOut)
//			return getBasketNumber();
//	}
//	return 0;
//}
//int posClass::getBasketForInFromDestacker()
//{
//	if (getHasBasket())
//	{
//		if (plcDestination==destinationStruct::inFromDestacker)
//			return getBasketNumber();
//	}
//	return 0;
//}
int posClass::getPositionNumber()
{
	return posNumber;
}
int posClass::getPositionIndex()
{
	return posIndex;
}
QString posClass::getPosIndexDisplayString()
{
	return QString("%1.%2").arg(posNumber).arg(posIndex);
}
void posClass::loadFromDatabaseSlot()
{
	loadFromDatabase();
}
void posClass::loadFromDatabase()
{
	QVector<QVector<QVariant>> posVV=execTableQuery(QString("select posNumber,posIndex,description,basketTableId,locked from positions where posNumber=%1 and posIndex=%3").arg(posNumber).arg(posIndex),bDb);
	QVector<QVector<QVariant>> contentsVV;
	if (posVV.count()==1)
	{
		int b=posVV[0][3].toInt();
		if (b!=0)
			contentsVV=execTableQuery(QString("%1 and RackDetail.RackId=%2 order by StopTime desc").arg(selectContentsFromEpicsStrG).arg(b),eDb);
		else
		{
			contentsVV.clear();
//			plcDestinationEnum=destinationStruct::plcValueEnum::none;
		}
	}
	updateFromData(posVV,contentsVV);
}
void posClass::loadFromAllData(QVector<QVector<QVariant>> allPosVV,QVector<QVector<QVariant>> allContentsVV)
{
	QVector<QVector<QVariant>> myPosVV;
	QVector<QVector<QVariant>> myContentsVV;
	for (int i=0;i<allPosVV.count();i++)
	{
		if (  (allPosVV[i][0].toInt()==posNumber)&&(allPosVV[i][1].toInt()==posIndex)  )
			myPosVV<<allPosVV[i];
	}
	if (myPosVV.count()==1)
	{
		int b=myPosVV[0][3].toInt();
		for (int i=0;i<allContentsVV.count();i++)
		{
			if (allContentsVV[i][0].toInt()==b)
				myContentsVV<<allContentsVV[i];
		}
	}
	updateFromData(myPosVV,myContentsVV);
}
void posClass::updateFromData(QVector<QVector<QVariant>> posVV,QVector<QVector<QVariant>> contentsVV)
{
	contentsV.clear();
	for (int i=0;i<contentsVV.count();i++)
		contentsV<<craneContentsDataStruct(contentsVV[i]);
	honeyCombDestinationEnumOfTopProfile=honeyCombDestinationEnumClass::none;
	if (posVV.count()==1)
	{
		basketNum=posVV[0][3].toInt();
		description=posVV[0][2].toString().trimmed();
		isLocked=(posVV[0][4].toInt()==2);
	}
	if (contentsV.count())
	{
		craneContentsDataStruct topProfileContentsData=getTopContentsDataStruct();
		honeyCombDestinationEnumOfTopProfile=getHoneyCombDestinationEnumClassG(topProfileContentsData,eDb);
		profileTxtItem->setBrush(QBrush(getHoneyCombDestinationEnumColorG(honeyCombDestinationEnumOfTopProfile)));
	}
	updateGui();
}
void posClass::updateItemsVisibility()
{
	if ((posNumber+posIndex)==1221)
		double a=0.;
	fromItem->setVisible(isFrom);
	toItem->setVisible(isTo);
	bool hasObstacle=false;
	for (int i=0;i<obstaclePosV.count();i++)
	{
		if (obstaclePosV[i]->getHasBasket())
			hasObstacle=true;
	}
	if (displayMode==displayNone)
	{
		posItem->setVisible(false);
		mouseOverItem->setVisible(false);
		basketItem->setVisible(false);
		profilesItem->setVisible(false);
		lockedItem->setVisible(false);
		posItemText->setVisible(false);
		profileTxtItem->setVisible(false);
	}
	if (displayMode==displayNormal)
	{
		posItem->setVisible(false);
		mouseOverItem->setVisible(false);
		posItemText->setVisible(true);
		profileTxtItem->setVisible(true);
	}
	if (displayMode==displayToModify)
	{
		bool canBeSelected=false;
		if (
			(isCranePos)
			&&
			((!abovePos)||(!abovePos->getHasBasket()))
			&&
			((!belowPos)||(belowPos->getHasBasket()))
			)
			canBeSelected=true;
		posItem->setVisible(canBeSelected);
		mouseOverItem->setVisible(canBeSelected);
		posItemText->setVisible(true);
		profileTxtItem->setVisible(true);
	}
	if (displayMode==displayToSelectFrom)
	{
		bool canBeSelected=false;
		if (
			(isCranePos)
			&&
			((getHasBasket())&&(getIsFreeToMove()))
			&&
			((!abovePos)||(!abovePos->getHasBasket()))
			&&
			((!belowPos)||((belowPos->getHasBasket())&&(!belowPos->getIsFrom())))
			&&
			( (posNumber!=660)&&(posNumber!=680))
			)
		{
			canBeSelected=true;
		}
		posItem->setVisible(canBeSelected);
		mouseOverItem->setVisible(canBeSelected);
		posItemText->setVisible(true);
		profileTxtItem->setVisible(true);
	}
	if (displayMode==displayToSelectTo)
	{
		bool canBeSelected=false;
		if (
			(isCranePos)
			&&
			((!getHasBasket())&&(getIsFreeToMove()) )
			&&
			((!abovePos)||(!abovePos->getHasBasket()))
			&&
			((!belowPos)||(((belowPos->getHasBasket())&&(!belowPos->getIsFrom()))))
			&&
			( (posNumber!=660)&&(posNumber!=680))
			)
		{
			canBeSelected=true;
		}
		mouseOverItem->setVisible(canBeSelected);
		posItem->setVisible(canBeSelected);
		basketItem->setVisible(getHasBasket());
		posItemText->setVisible(true);
		profileTxtItem->setVisible(true);
	}
	if (displayMode==displayToExport)
	{
		bool canBeSelected=false;
		if (
			(isCranePos)
			&&
			(getHasBasket())
			)
		{
			canBeSelected=true;
		}
		mouseOverItem->setVisible(canBeSelected);
		posItem->setVisible(canBeSelected);
		posItemText->setVisible(true);
		profileTxtItem->setVisible(true);
	}
}
void posClass::updateGui()
{
	QString toolTipTxt=QString("%1<br>Position:%2<br>x:%3mm,y:%4mm,z:%5mm<br>").arg(description).arg(posNumber+posIndex).arg(plcXPos).arg(plcYPos).arg(plcZPos);
	updateItemsVisibility();
	basketItem->setVisible(basketNum!=0);
	profilesItem->setVisible(contentsV.count());
	lockedItem->setVisible(isLocked);
	if (basketNum!=0)
	{//			contentsVV=execTableQuery(QString("select basketnum,dienum,Pc,PcLen,temper from basket_details where basketnum=%1 and Pc<>0").arg(b),aDb);
		toolTipTxt+="<html><body bgcolor=\"#E6E6FA\"><font style='color:#333333'>"+tr("Basket number:")+"</font><font style='color:#000077'>"+QString("%1").arg(basketNum)+"</font><br>";
		toolTipTxt+=QString("Destination %1<br>").arg(getHoneyCombDestinationEnumClassStringG(honeyCombDestinationEnumOfTopProfile));
		if (contentsV.count())
		{
			toolTipTxt+=QString(QString("<table><tr>"));
			for (int i=(contentsV.count()-1);i>=0;i--)
			{
				toolTipTxt+=QString(QString("<td>"))+
					QString("%1<br>").arg(contentsV[i].profile)+
					tr("pieces:%1<br>").arg(contentsV[i].quantity)+
					tr("length:%1mm<br>").arg(contentsV[i].length)+
					tr("temper:%1<br>").arg(contentsV[i].temper)+
					tr("So:%1<br>").arg(contentsV[i].soNumber)+
					tr("So Line:%1<br>").arg(contentsV[i].soLine)+
					tr("CurrentFinCode:%1<br>").arg(contentsV[i].CurrentFinCode)+
					tr("StdPract:%1<br>").arg(contentsV[i].StdPract)+
					tr("CustShip:%1<br>").arg(contentsV[i].CustShip)+
					tr("NextDeptNum:%1<br>").arg(contentsV[i].NextDeptNum)
					;
				toolTipTxt+=QString(QString("</td>"));
			}
			toolTipTxt+=QString(QString("</tr></table><br><br>"));
		}
		else
		{
			toolTipTxt+=QString(tr("The basket is empty.<br>"));
		}
	}
	else
	{
		toolTipTxt+=QString("<font style='color:#000077'>")+tr("No basket")+"</font><br>";
	}
	toolTipTxt+=QString(QString("</body></html>"));
	posItem->setToolTip(toolTipTxt);
	basketItem->setToolTip(toolTipTxt);
	updateTxt();
	scene->update();
}

void posClass::mousePressedSlot(QGraphicsSceneMouseEvent *ev)
{
	if ((ev->modifiers()==Qt::ShiftModifier)&&(ev->button()==Qt::LeftButton))
	{
		loadFromDatabase();
		return;
	}
	if ((ev->button()==Qt::LeftButton)||(ev->button()==Qt::RightButton))
	{
		if (ev->modifiers()==Qt::ControlModifier)
		{
			showPositionDlgSlotSlot();
			emit userChangedBasketSignal(this);
		}
		else
			emit posMouseReleasedSignal(this,ev);
	}
}
void posClass::showPositionDlgSlotSlot()
{
	showPositionDlg(0);
}
void posClass::showPositionDlg(int activeTab,int type,bool checkPass)
{
	if (!isCranePlcPos)
	{
		posDlgClass *posDlg=new posDlgClass(allPos,this,"user",this);
		if (posDlg->myExec(false))
		{
			bool isActive=getFirst(QString("select count(tId) from c2missions where (fromPosNumber=%1 and fromPosIndex=%2) or (toPosNumber=%1 and toPosIndex=%2)").arg(posNumber).arg(posIndex),bDb).toInt()!=0;
			if (!isActive)
				setBasketId(posDlg->getBasketNum(),true,"user");
			loadFromDatabase();
		}
		delete posDlg;
	}
	else
	{
		QMessageBox::information(this,tr("Information"),tr("This is a PLC controlled position.\nUse the conveyors program to access the position's data."));
	}
}
void posClass::setIsFrom(bool s)
{
	isFrom=s;
	updateItemsVisibility();
}
bool posClass::getIsFrom()
{
	return isFrom;
}
void posClass::setIsTo(bool s)
{
	isTo=s;
	updateItemsVisibility();
}
int posClass::getIdForPlc()
{
	return posNumber+posIndex;
}
//
int posClass::getBasketNumber()
{
	return basketNum;
}
bool posClass::getHasBasket()
{
	return basketNum!=0;
}
bool posClass::getHasBasketCommingIn()
{
	if (!getHasBasket())
		return false;
	if (getDestination()==destinationStruct::plcValueEnum::anodizing)
		return false;
	if (getDestination()==destinationStruct::plcValueEnum::destackerForSb)
		return false;
	if (getDestination()==destinationStruct::plcValueEnum::anodizingOverLength)
		return false;
	if (getDestination()==destinationStruct::plcValueEnum::packingDestackerPos)
		return false;
	return true;
}
bool posClass::getHasFullBasket()
{
	return (basketNum!=0)&&(contentsV.count()!=0);
}
bool posClass::getIsLocked()
{
	return isLocked;
}
bool posClass::getIsPlcReadyToGetBasket()
{
	return isPlcReadyToGetBasket;
}
bool posClass::getIsPlcReadyToSendBasket()
{
	return isPlcReadyToSendBasket;
}
float posClass::getPlcX()
{
	return plcXPos;
}
float posClass::getPlcY()
{
	return plcYPos;
}
float posClass::getPlcZ()
{
	return plcZPos;
}
QString posClass::getDescription()
{
	return description;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool posClass::getCanGetBasketInPos(posClass *p)
{
	if (getHasBasket())
		return false;
	if (!getIsFreeToMove())
		return false;
	if (abovePos&&abovePos->getHasBasket())
		return false;
	if (belowPos&&(!belowPos->getHasBasket()))
		return false;
	return true;
}
int posClass::getMaximumLength()
{
	int maxLength=0;
	for (int i=0;i<contentsV.count();i++)
	{
		if (contentsV[i].length>maxLength)
			maxLength=contentsV[i].length;
	}
	return maxLength;
}
//void posClass::setDestination(destinationStruct::plcValueEnum dEnun)
//{
//	plcDestination Enum=dEnun;
//	updateGui();
//}
destinationStruct::plcValueEnum posClass::getDestination()
{
	return plcDestinationEnum;
}
QVector<int> posClass::getProfiles()
{
	QVector<int> ans;
	for (int i=0;i<contentsV.count();i++)
		ans<<contentsV[i].profile;
	return ans;
}
craneContentsDataStruct posClass::getTopContentsDataStruct()
{
	if (contentsV.count())
		return contentsV.last();
	return craneContentsDataStruct();
}
craneContentsDataStruct posClass::getBottomContentsDataStruct()
{
	if (contentsV.count())
		return contentsV.first();
	return craneContentsDataStruct();
}
int posClass::getTopProfile()
{
	if (contentsV.count())
		return contentsV.last().profile;
	return -1;
}
int posClass::getBottomProfile()
{
	if (contentsV.count())
		return contentsV[0].profile;
	return -1;
}
bool posClass::getIsProfileStacked(int p)
{
	QVector<int> profiles=getProfiles();
	if (!profiles.contains(p))
		return true;
	if (p==getTopProfile())
		return true;
	return false;
}
honeyCombDestinationEnumClass posClass::getHoneyCombDestinationEnum()
{
	return honeyCombDestinationEnumOfTopProfile;
}
void posClass::setExportEnun(exportCrane2EnumClass exportEnum_)
{
	exportEnum=exportEnum_;
	updateGui();
}
exportCrane2EnumClass posClass::getExportEnun()
{
	return exportEnum;
}
QVector<posClass*> posClass::getPosThatPreventYouToMove()
{
	QVector<posClass*> ans;
	if (abovePos)
	{
		if (abovePos->getHasBasket())
		{
			if (!ans.contains(abovePos))
				ans<<abovePos;
		}
		ans<<abovePos->getPosThatPreventYouToMove();
	}
	for (int i=0;i<obstaclePosV.count();i++)
	{
		if (obstaclePosV[i]->getHasBasket())
		{
			if (!ans.contains(obstaclePosV[i]))
				ans<<obstaclePosV[i];
		}
		ans<<obstaclePosV[i]->getPosThatPreventYouToMove();
	}
	return ans;
}

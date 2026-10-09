#include "pch.h"
#include "pos.h"
#include "allPos.h"
double textScale=15.;//constexpr??
double exportTextScale=25.;//constexpr??
posClass::posClass(allPosClass *allPos_,int posNumber_,int posIndex_,float plcXPos_,float plcZPos_,float gXOffset_,float gYOffset_,
	QGraphicsScene *scene_,gItemClass *posItem_,gItemClass *mouseOverItem_,gItemClass *basketItem_,gItemClass *profilesItem_,gItemClass *lockedItem_,gItemClass *fromItem_,gItemClass *toItem_
	,bool handleMousePressed,QWidget *p):QWidget(p)
{
	tuxip=nullptr;
	plcIndex=-1;
	setVisible(false);
	allPos=allPos_;
	posNumber=posNumber_;
	posIndex=posIndex_;
	plcXPos=plcXPos_;
	plcZPos=plcZPos_;
	gXOffset=gXOffset_;
	gYOffset=gYOffset_;
	displayMode=displayToSelectFrom;
	scene=scene_;
	isFrom=isTo=false;
	basketNum=-1;
	plcDestination=0;
	isCranePos=true;
	isCranePlcPos=false;
	fromItem=new gItemClass(fromItem_,0.,0.,this);
	scene->addItem(fromItem);
	toItem=new gItemClass(toItem_,0.,0.,this);
	scene->addItem(toItem);
	basketItem=new gItemClass(basketItem_,0.,0.,this);
	scene->addItem(basketItem);
	profilesItem=new gItemClass(profilesItem_,0.,0.,this);
	scene->addItem(profilesItem);
	lockedItem=new gItemClass(lockedItem_,0.,0.,this);
	scene->addItem(lockedItem);
	posItem=new gItemClass(posItem_,0.,0.,this);
	scene->addItem(posItem);
	mouseOverItem=new gItemClass(mouseOverItem_,0.,0.,this);
	scene->addItem(mouseOverItem);

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
	setPlcPos(plcXPos,plcZPos);
	if (handleMousePressed)
		connect(mouseOverItem,SIGNAL(mousePressedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedSlot(QGraphicsSceneMouseEvent*)));
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
posClass* posClass::getAbovePos()
{
	return abovePos;
}
posClass* posClass::getBelowPos()
{
	return belowPos;
}
void posClass::setPlcPos(float x,float z)
{
	float bHeight=750.f;//const ?? What meaning does it have? Localy used.
	plcXPos=x;
	plcZPos=z;
	gXPos=plcXPos+gXOffset;
	gYPos=plcZPos+(posIndex-1)*bHeight+gYOffset;
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
		QString txt=tr("%1:%2:%3").arg(basketNum).arg(getHoneyCombDestinationEnumClassStringG(honeyCombDestinationEnumOfTopProfile)).arg(getExportCrane3EnumOneCharacterClassStringG(exportEnum));
		posItemText->setText(txt);
		posItemText->setPos(gXPos-0.5*posItemText->boundingRect().width()*textScale,gYPos+posItem->boundingRect().height());
		posItemText->setVisible(true);
	}
	else
	{
		posItemText->setText(tr("%1").arg(posNumber+posIndex));
		posItemText->setVisible(false);
	}
	posItemText->setBrush(getExportCrane3EnumColorG(exportEnum));
	//profileTxt
	QString profileTxt;
	for (int i=0;i<contentsV.count();i++)
		profileTxt.append(tr("%1:%2:%3\n").arg(contentsV[i].profile).arg(contentsV[i].quantity).arg(getHoneyCombDestinationEnumClassStringG(contentsV[i].honeyCombDestinationEnum)));
	profileTxtItem->setText(profileTxt);
	profileTxtItem->setPos(gXPos-0.5*profileTxtItem->boundingRect().width()*textScale,gYPos+0.75*posItem->boundingRect().height());
	profileTxtItem->setVisible(contentsV.count());
}
void posClass::setBasketId(int basketID,bool user,QString logInfo,int doNotCheckPosition,int doNotCheckIndex)
{
	if ((basketNum!=0)&&(basketID!=0)&&(!user))
	{
		if (posNumber!=doNotCheckPosition)
			uExit(errorStr,QString("trying to declare basket %1 in position %2.%3, position %2.%3 allready has basket %4 declared").arg(basketID).arg(posNumber).arg(posIndex).arg(basketNum));
	}
	{
		QString posOfBasket=getDescriptionOfPosWithBasket(basketID,QVector<int>()<<doNotCheckPosition<<posNumber,QVector<int>()<<doNotCheckIndex<<posIndex);
		if (posOfBasket.isEmpty())
		{
			execQuery(QString("update positions set basketTableId=%1 where posNumber=%2 and posIndex=%3").arg(basketID).arg(posNumber).arg(posIndex),bDb);
			execQuery(QString("insert into c3logPos (pos,basket,info,time) values(%1,%2,'%3','%4')").arg(getPosIndexDisplayString()).arg(basketID).arg(logInfo).arg(timeForLog),bDb);
			if (tuxip)
			{
				tuxip->writeInteger(plcTable,plcIndex,basketID);
			}
			loadFromDatabase();
		}
	}
	emit posChangedSignal();
}
void posClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((tuxip)&&(!tuxip->getHasError())&&(tuxip->getIp()==ip)&&(index==plcIndex))
	{
		if (table==plcTable)
		{
			execQuery(QString("update positions set basketTableId=%1 where posNumber=%2 and posIndex=%3").arg(value.toInt()).arg(posNumber).arg(posIndex),bDb);
			execQuery(QString("insert into c3logPos (pos,basket,info,time) values(%1,%2,'%3','%4')").arg(getPosIndexDisplayString()).arg(value.toInt()).arg("plc").arg(timeForLog),bDb);
			loadFromDatabase();
		}
		if (table=="bConveyorData")
			plcDestination=value.toInt();
	}
	if ((table=="CR3_PCcomDint")&&(index==21))
	{
		if (posNumber==destackerWorkingPos1PlcId)
			isPlcReadyToGetBasket=isPlcReadyToSendBasket=getValueOfBit(value.toInt(),24);
		if (posNumber==destackerWorkingPos2PlcId)
			isPlcReadyToGetBasket=isPlcReadyToSendBasket=getValueOfBit(value.toInt(),25);
	}
	if ((table=="CR3_PCcomDint")&&(index==23))
	{
		if (posNumber==crane2FromToPlcId)
			isPlcReadyToGetBasket=getValueOfBit(value.toInt(),5);
	}
}
int posClass::getPositionNumber() const
{
	return posNumber;
}
int posClass::getPositionIndex() const
{
	return posIndex;
}
QString posClass::getPosIndexDisplayString() const
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
	if (posVV.count()==1)
	{
		QVector<QVector<QVariant>> contentsVV;
		int b=posVV[0][3].toInt();
		if (b!=0)
			contentsVV=execTableQuery(QString("%1 and RackDetail.RackId=%2 order by StopTime desc").arg(selectContentsFromEpicsStrG).arg(b),eDb);
		else
		{
			contentsVV.clear();
		}
		updateFromData(posVV,contentsVV);
	}
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
		if (honeyCombDestinationEnumOfTopProfile==honeyCombDestinationEnumClass::none)
			profileTxtItem->setBrush(QBrush(QColor(100,100,100)));
		if (honeyCombDestinationEnumOfTopProfile==honeyCombDestinationEnumClass::anodizing)
			profileTxtItem->setBrush(QBrush(QColor(100,100,150)));
		if (honeyCombDestinationEnumOfTopProfile==honeyCombDestinationEnumClass::HCA)
			profileTxtItem->setBrush(QBrush(QColor(100,150,150)));
		if (honeyCombDestinationEnumOfTopProfile==honeyCombDestinationEnumClass::HCB)
			profileTxtItem->setBrush(QBrush(QColor(150,150,100)));
	}
	if (posNumber==destackerWorkingPos1PlcId)
	{
		if (tuxip->readIntegerValue("CR3_PCcomDint",56)==-1)
			tuxip->writeInteger("CR3_PCcomDint",56,getMaximumContentsLengh());
	}
	if (posNumber==destackerWorkingPos2PlcId)
	{
		if (tuxip->readIntegerValue("CR3_PCcomDint",57)==-1)
			tuxip->writeInteger("CR3_PCcomDint",57,getMaximumContentsLengh());
	}
	updateGui();
}
void posClass::updateItemsVisibility()
{
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
		posItemText->setVisible(false);
		basketItem->setVisible(basketNum);
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
}
void setIsDestackerExportBasket(bool v)
{
}
void setIsPackingExportBasket(bool v)
{
}
void posClass::updateGui()
{
	QString toolTipTxt=QString("%1<br>Position:%2<br>x:%3mm,z:%4mm<br>").arg(description).arg(posNumber+posIndex).arg(plcXPos).arg(plcZPos);
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
					tr("So Line:%1<br>").arg(contentsV[i].soLine)
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
			bool isActive=getFirst(QString("select count(tId) from c3missions where (fromPosNumber=%1 and fromPosIndex=%2) or (toPosNumber=%1 and toPosIndex=%2)").arg(posNumber).arg(posIndex),bDb).toInt()!=0;
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
int posClass::getIdForPlc() const
{
	return posNumber+posIndex;
}
int posClass::getBasketNumber() const
{
	return basketNum;
}
bool posClass::getHasBasket() const
{
	return basketNum!=0;
}
bool posClass::getHasFullBasket()
{
	return (basketNum!=0)&&(contentsV.count()!=0);
}
bool posClass::getHasEmptyBasket()
{
	return (basketNum!=0)&&(contentsV.count()==0);
}
bool posClass::getIsLocked() const
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
float posClass::getPlcX() const
{
	return plcXPos;
}
float posClass::getPlcZ() const
{
	return plcZPos;
}
QString posClass::getDescription() const
{
	return QString("%1 %2").arg(description).arg(posNumber+posIndex);
}

bool posClass::getCanGetBasketInPos(posClass *p)
{
	if (p->getMaximumLength()>packingMaximumPacketLength)
		return false;
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
QVector<craneContentsDataStruct> posClass::getContentsDataStructs()
{
	return contentsV;
}
bool posClass::getHasSameContentsWithPos(posClass *p)
{
	QVector<craneContentsDataStruct> otherContents=p->getContentsDataStructs();
	if (contentsV.count()!=otherContents.count())
		return false;
	for (int i=0;i<contentsV.count();i++)
	{
		if (!contentsV[i].hasSameSoSoLine(otherContents[i]))
			return false;
	}
	return true;
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
int posClass::getMaximumContentsLengh()
{
	int max=0;
	for (int i=0;i<contentsV.count();i++)
	{
		if (max<contentsV[i].length)
			max=contentsV[i].length;
	}
	return max;
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
void posClass::setExportEnun(exportCrane3EnumClass exportEnum_)
{
	exportEnum=exportEnum_;
	updateGui();
}
exportCrane3EnumClass posClass::getExportEnun()
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

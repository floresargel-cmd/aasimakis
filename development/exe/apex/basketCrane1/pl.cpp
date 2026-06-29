#include "pl.h"

positionListClass::positionListClass(int mainPosId_, int typeEnum_, int positionEnumForEpics, GLfloat openGlX, GLfloat openGlY) :QObject()
{
	LOG("pl.cpp", "positionListClass::positionListClass()");
	mainPosId = mainPosId_;
	typeEnumeration = typeEnum_;
	isLockedToRecieve = false;
	int noOfPos = GETONE(QString("select count(posIndex) from positions where posNumber=%1").arg(mainPosId), MAIN_DB_CONNECTION).toInt();
	description = GETONE(QString("select description from positions where posNumber=%1 and posIndex=1").arg(mainPosId), MAIN_DB_CONNECTION).toString();

	GLfloat dz = 0.750f;
	GLfloat z = -dz;

	for (int i = 0; i < noOfPos; i++)
	{
		basketPositionClass *t = new basketPositionClass(this, description, mainPosId, i + 1, positionEnumForEpics, openGlX, openGlY, z += dz);
		basketPositions.append(t);
		connect(t, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	}
	LOG("pl.cpp", "loadFromDatabase()");
	loadFromDatabase();
	LOG("pl.cpp", "... done loadFromDatabase()");
}

positionListClass::~positionListClass()
{
	for (int i = 0; i <	basketPositions.count(); i++)
		delete basketPositions[i];
}

//
void positionListClass::setLockRecieve(bool l)
{
	isLockedToRecieve = l;
}

void positionListClass::addObstructingPositionList(positionListClass *list)
{
	obstructingPositionListsArray.append(list);
}

void positionListClass::addBehindPositionLists(positionListClass *list)
{
	behindPositionListsArray.append(list);
}

void positionListClass::addConcurrentPositionLists(positionListClass *list)
{
	concurrentPositionListsArray.append(list);
}

void positionListClass::positionSelectedSlot(basketPositionClass* b)
{
	emit positionSelectedSignal(b);
}

void positionListClass::drawAllPosWithBasket(bool forPicking)
{
	for (int i = 0; i < basketPositions.count(); i++)
	{
		if (basketPositions[i]->hasBasket())
			basketPositions[i]->draw(forPicking, false, false);
	}
}

void positionListClass::drawFreeToMoveBasket()//for Picking only
{
	basketPositionClass *freeBasketPos=getFreeToMoveBasketPosition();
	if (freeBasketPos)
		freeBasketPos->draw(true,false,false);
}

void positionListClass::drawFreeToRecievePos(bool forPicking, basketPositionClass *incomingBasketPos)
{
	basketPositionClass *freeToRecievePos = getFreeToRecievePos(incomingBasketPos);
	// @@@@ -- blowing up here, nullptr on the error return path?
	if (freeToRecievePos == nullptr) {
		LOG("positionListClass", "WARNING: NULLPTR - the called function may return a NULL now due to missing data...");
		// return; 
	}
	if (freeToRecievePos)
		freeToRecievePos->draw(forPicking, false, false);
}

void positionListClass::drawLockedToRecieve()
{
	if (isLockedToRecieve)
		basketPositions.last()->drawLockedToRecieve();
}
void positionListClass::mouseOver(QColor pickedColor)
{
	for (int i = 0; i < basketPositions.count(); i++)
		basketPositions[i]->mouseOver(pickedColor);
}
void positionListClass::mouseOverHighlitingMode(QColor pickedColor)
{
	for (int i = 0; i < basketPositions.count(); i++)
		basketPositions[i]->mouseOverHighlitingMode(pickedColor);
}

void positionListClass::mouseClicked(QColor pickedColor, bool rightClick)
{
	for (int i = 0; i < basketPositions.count(); i++)
		basketPositions[i]->mouseClicked(pickedColor, rightClick);
}

int positionListClass::getNumberOfBaskets()
{
	int numberOfBaskets = 0;
	for (int i = 0; i < basketPositions.count(); i++)
	{
		if (basketPositions[i]->hasBasket())
			numberOfBaskets++;
	}
	return numberOfBaskets;
}

bool positionListClass::isTheBottomBasket(int b)
{
	if (basketPositions[0]->getBasketTableId() == b)
		return true;
	return false;
}

bool positionListClass::hasLockedPositionByCrane()
{
	for (int i = 0; i < basketPositions.count(); i++)
	{
		if (basketPositions[i]->getIsLockedByCrane())
			return true;
	}
	return false;
}

void positionListClass::setNextBasketForExport(int basketId)//gui only
{
	for (int i = 0; i < basketPositions.count(); i++)
		basketPositions[i]->setNextBasketForExport(basketId);
}

void positionListClass::setNextBasketForOmavDestacker(int basketId)//gui only
{
	for (int i = 0; i < basketPositions.count(); i++)
		basketPositions[i]->setNextBasketForOmavDestacker(basketId);
}

void positionListClass::setNextBasketForAssimakisDestacker(int basketId)//gui only
{
	for (int i = 0; i < basketPositions.count(); i++)
		basketPositions[i]->setNextBasketForAssimakisDestacker(basketId);
}

int positionListClass::getNumberOfPositions()
{
	return basketPositions.count();
}

basketPositionClass* positionListClass::getPosition(int index)
{
	// LOG("pl.cpp", "positionListClass::getPosition(" + QString::number(index) + ")");
	if (index < basketPositions.count()) {
		// LOG("pl.cpp", "positionListClass::getPosition(int " + QString::number(index) + ") returning position with id " + QString::number(basketPositions[index]->getNumber()) + ", Desc: " + QString(basketPositions[index]->getDescription()));
		return basketPositions[index];
	}
	else {
		LOG("pl.cpp", "Unable to find a position for index: " + QString::number(index) );
		// EXIT(tr("Error"), tr("positionListClass::getPosition(int index)"));  
	}

	return NULL;
}

QVector<basketPositionClass*> positionListClass::getBasketPositions()
{
	QVector<basketPositionClass*> ans;

	for (int i = 0; i < basketPositions.count(); i++)
	{
		if (basketPositions[i]->getBasketTableId())
			ans << basketPositions[i];
		else
			break;
	}
	return ans;
}

basketPositionClass* positionListClass::getBaskePositiontWithBasketId(int id)
{
	if (id > 0)
	{
		for (int i = 0; i < basketPositions.count(); i++)
		{
			if (basketPositions[i]->getBasketTableId() == id)
				return basketPositions[i];
		}
	}
	return NULL;
}

basketPositionClass* positionListClass::getPositionWithAnagedBasket()
{
	for (int i = basketPositions.count() - 1; i >= 0; i--)
	{
		if (basketPositions[i]->hasBasket() && (!basketPositions[i]->getAreContentsAged()))
			return basketPositions[i];
	}
	return NULL;
}

basketPositionClass* positionListClass::getTopBasketPosition()
{
	for (int i = basketPositions.count() - 1; i >= 0; i--)
	{
		if (basketPositions[i]->hasBasket())
			return basketPositions[i];
	}
	return NULL;
}

bool positionListClass::hasBaskePositiontWithBasketId(int id)
{
	for (int i = 0; i<basketPositions.count(); i++)
	{
		if (basketPositions[i]->getBasketTableId() == id)
			return true;
	}
	return false;
}

basketPositionClass* positionListClass::getHighlightedPos()
{
	for (int i = basketPositions.count() - 1; i >= 0; i--)
	{
		if (basketPositions[i]->getIsHighlighted())
			return basketPositions[i];
	}
	return NULL;
}

basketPositionClass* positionListClass::getFreeToMoveBasketPosition()
{
	if (isSomethingObstructingThePositionsWay())
		return NULL;

	for (int i = basketPositions.count() - 1; i >= 0; i--)
	{
		if ((basketPositions[i]->getLocked() == true) || (basketPositions[i]->getIsLockedByCrane() == true))
			return NULL;

		if (basketPositions[i]->hasBasket())
			return basketPositions[i];
	}
	return NULL;
}

int positionListClass::getTopBasketTableId()
{
	for (int i = basketPositions.count() - 1; i >= 0; i--)
	{
		if (basketPositions[i]->hasBasket())
			return basketPositions[i]->getBasketTableId();
	}
	return 0;
}

int positionListClass::getMainPosId()
{
	return mainPosId;
}

bool positionListClass::hasEmptyBasket()
{
	for (int i = 0; i < basketPositions.count(); i++)
	{
		if ((basketPositions[i]->hasBasket()) && (!basketPositions[i]->hasContents()))
			return true;
	}
	return false;
}

bool positionListClass::canTakeFullBasket()
{
	if (hasEmptyBasket())
		return false;

	for (int i = 0; i < obstructingPositionListsArray.count(); i++)
	{
		if (obstructingPositionListsArray[i]->hasEmptyBasket())
			return false;
	}

	//for (int i=0;i<behindPositionListsArray.count();i++)
	//{
	//	if (behindPositionListsArray[i]->hasEmptyBasket())
	//		return false;
	//}

	return true;
}

bool positionListClass::hasFullBasket()
{
	for (int i = 0; i < basketPositions.count(); i++)
	{
		if ((basketPositions[i]->hasBasket()) && (basketPositions[i]->hasContents()))
			return true;
	}
	return false;
}

bool positionListClass::canTakeEmptyBasket()
{
	if (hasFullBasket())
		return false;

	//for (int i=0;i<obstructingPositionListsArray.count();i++)
	//{
	//	if (obstructingPositionListsArray[i]->hasFullBasket())
	//		return false;
	//}

	for (int i = 0; i < behindPositionListsArray.count(); i++)
	{
		if (behindPositionListsArray[i]->hasFullBasket())
			return false;
	}
	return true;
}

basketPositionClass* positionListClass::getFreeToRecievePos(basketPositionClass* incomingPos)
{
	if (!incomingPos) // @@@@@ -- This doesn't seem to be catching the null pointer, as the program is still crashing with an access violation when trying to read memory for incomingPos.  Need to investigate further, but for now, adding this check to prevent the crash and log the issue.
		return NULL;

	if( incomingPos == nullptr) {
		LOG("pl.cpp", "WARNING: Vendor code failed above in it's !incomingPos logic (nullptr), positionListClass::getFreeToRecievePos() - incomingPos is NULL, returning NULL");
		return NULL;
	}


	if (isLockedToRecieve)
		return NULL;

	if (isBasketPositionYours(incomingPos))
		return NULL;
	
	basketPositionClass* ans = NULL;
	
	//is same type?
	if (incomingPos && (incomingPos->getTypeEnum() == BASKET_NORMAL) && (!(typeEnumeration&TYPE_NORMAL)))
		return NULL;

	if (incomingPos && (incomingPos->getTypeEnum() == BASKET_SCRAP) && (!(typeEnumeration&TYPE_SCRAP)))
		return NULL;
	
	//contents
	if (incomingPos && (incomingPos->getTypeEnum() == BASKET_NORMAL))
	{
		if ((incomingPos->getContentsEnum() == BASKET_CONTENS_NONE) && (!(typeEnumeration&TYPE_EMPTY)))
			return NULL;

		if ((incomingPos->getContentsEnum() == BASKET_CONTENS_LESS_THAN_20) && (!(typeEnumeration&TYPE_PROFILES20)))
			return NULL;

		if ((incomingPos->getContentsEnum() == BASKET_CONTENS_LESS_THAN_24) && (!(typeEnumeration&TYPE_PROFILES24)))
			return NULL;

		if ((incomingPos->getContentsEnum() == BASKET_CONTENS_LESS_THAN_30) && (!(typeEnumeration&TYPE_PROFILES30)))
			return NULL;

		if (incomingPos->getContentsEnum() == BASKET_CONTENS_MORE_THAN_30)
			return NULL;
	}
	
	//is a basket in the column full/empty and the incoming is empty/full (this is for the empty buffer positions when puting 20 feet)
	if ((mainPosId == 300) || (mainPosId == 310) || (mainPosId == 320) || (mainPosId == 330) || (mainPosId == 340) || (mainPosId == 350) || (mainPosId == 360) || (mainPosId == 370) || (mainPosId == 380) || (mainPosId == 390))  //only in empty buffer possitions
	{
		if ((incomingPos->hasContents()) && (!canTakeFullBasket()))
			return NULL;

		if ((!incomingPos->hasContents()) && (!canTakeEmptyBasket()))
			return NULL;
	}

	// when puting 20 feet in empty buffer go up to the third floor
	if ((mainPosId == 300) || (mainPosId == 310) || (mainPosId == 320) || (mainPosId == 330) || (mainPosId == 340) || (mainPosId == 350) || (mainPosId == 360) || (mainPosId == 370) || (mainPosId == 380) || (mainPosId == 390) ||
		(mainPosId == 800) || (mainPosId == 820) || (mainPosId == 840) || (mainPosId == 860) || (mainPosId == 880))  //only in empty buffer possitions
	{
		if ((incomingPos->hasContents()) && (getNumberOfBaskets() >= 3))
			return NULL;
	}

	//is Something Obstructing the way?
	if (isSomethingObstructingThePositionsWay(incomingPos))
		return NULL;

	//isAConcurrentPositionFull?
	if (isAConcurrentPositionFull())
		return NULL;

	//is recieving mine?
	if (incomingPos)
	{
		for (int i = 0; i < basketPositions.count(); i++)
		{
			if (basketPositions[i] == incomingPos)
			{
				return NULL;
			}
		}
	}

	//is empty?
	for (int i = 0; i < basketPositions.count(); i++)
	{
		if (!basketPositions[i]->hasBasket())
		{
			if (!basketPositions[i]->getLocked())  //if the first empty is locked then return NULL
				ans = basketPositions[i];
			break;
		}
	}

	return ans;
}

bool positionListClass::isBasketPositionYours(basketPositionClass* incomingPos)
{
	//if (incomingPos == nullptr) {
	//	LOG("positionListClass", "WARNING: incomingPos is NULL!  Have to bail, as the program logic can not return succesfully from this.");
	//	EXIT("NULLPTR", "incomingPos in inBasketPositionYours() is NULL, can not continue!");
	//	
	//}
	//LOG("pl.cpp", "positionListClass::isBasketPositionYours() - incomingPos = " + QString::number(incomingPos->getBasketNumber()));  // @@@@@ - AB - COME BACK HERE: this mainPosId is NUKK?  Unable to read memory (isLockedToRevieve and typeEnumberation were also possibly null.
	if (incomingPos == NULL) {
		LOG("pl.cpp", "positionListClass::isBasketPositionYours() - incomingPos is NULL, returning false");	
		return false;
	}

	if (basketPositions.contains(incomingPos)) {
		LOG("pl.cpp", "positionListClass::isBasketPositionYours() - incomingPos is in basketPositions, returning true");	
		return true;
	}

	LOG("pl.cpp", "positionListClass::isBasketPositionYours() - incomingPos is NOT in basketPositions, returning false");
	return false;
}

bool positionListClass::isAConcurrentPositionFull()
{
	LOG("pl.cpp", "positionListClass::isAConcurrentPositionFull() - mainPosId = " + QString::number(mainPosId));
	for (int i = 0; i < concurrentPositionListsArray.count(); i++)
	{
		if (concurrentPositionListsArray[i]->getNumberOfBaskets() >= 1) {
			LOG("pl.cpp", "positionListClass::isAConcurrentPositionFull() - TRUE, because of position list with mainPosId = " + QString::number(concurrentPositionListsArray[i]->getMainPosId()));
			return true;
		}
	}
	LOG("pl.cpp", "positionListClass::isAConcurrentPositionFull() - FALSE");	
	return false;
}

bool positionListClass::isSomethingObstructingThePositionsWay(basketPositionClass* incomingPos)
{
	//if( incomingPos == nullptr) {  // @@@@@ -- AB
	//	LOG("pl.cpp", "WARNING: Vendor code (2nd) failed above in it's !incomingPos logic (nullptr), positionListClass::isSomethingObstructingThePositionsWay() - incomingPos is NULL, returning NULL");
	//	return NULL;
	//}


	LOG("pl.cpp", "INFO: positionListClass::isSomethingObstructingThePositionsWay() - mainPosId = " + QString::number(mainPosId));	
	// LOG("pl.cpp", "INFO: Checking position list for obstructions");
	for (int i = 0; i < obstructingPositionListsArray.count(); i++) {
		LOG("pl,cpp", "INFO: Index=" + QString::number(i));
		if (obstructingPositionListsArray[i] == nullptr) {
			LOG("obstructingPositionListArray", "ERROR: NULLPTR!!!");
		}
	}
	for (int i = 0; i < obstructingPositionListsArray.count(); i++)
	{
		if (!obstructingPositionListsArray[i]->isBasketPositionYours(incomingPos)) //otherwise if on same row and if one 4 this one is obstructing itshelf
		{
			if (obstructingPositionListsArray[i]->getNumberOfBaskets() == 4) {
				LOG("pl.cpp", "INFO: positionListClass::isSomethingObstructingThePositionsWay() - TRUE, because of position list with mainPosId = " + QString::number(obstructingPositionListsArray[i]->getMainPosId()));
				return true;
			}
		}
	}
	LOG("pl.cpp", "INFO: positionListClass::isSomethingObstructingThePositionsWay() - FALSE");
	return false;
}

bool positionListClass::isBasketOnTop(int index)
{
	LOG("pl.cpp", "positionListClass::isBasketOnTop(int index) - index = " + QString::number(index));
	int topIndexWithBasket = getNumberOfBaskets() - 1;
	if (index != topIndexWithBasket) {
		LOG("pl.cpp", "positionListClass::isBasketOnTop = FALSE");	
		return false;
	}
	LOG("pl.cpp", "positionListClass::isBasketOnTop = TRUE");
	return true;
}

//bool positionListClass::canBasketMove(int index)
//{
//	if (!isBasketOnTop(index))
//		return false;
//	if (isSomethingObstructingThePositionsWay())
//		return false;
//	return true;
//}
//bool positionListClass::canDropBasket(int index)
//{
//	if (isSomethingObstructingThePositionsWay())
//		return false;
//	if (isAConcurrentPositionFull())
//		return false;
//	if (getNumberOfBaskets()==basketPositions.count())
//		return false;
//	return true;
//}
//

void positionListClass::loadFromDatabase()
{
	LOG("pl.cpp", "positionListClass::loadFromDatabase() - mainPosId = " + QString::number(mainPosId));
	LOG("pl.cpp", "positionListClass::loadFromDatabase() - number of positions = " + QString::number(basketPositions.count()));
	for (int i = 0; i < basketPositions.count(); i++)
	{
		basketPositions[i]->loadFromDataBase();
	}
}

void positionListClass::calculateBasketContentsTypeFromEpicsDatabase()
{
	LOG("pl.cpp", "positionListClass::calculateBasketContentsTypeFromEpicsDatabase() - mainPosId = " + QString::number(mainPosId));	
	LOG("pl.cpp", "positionListClass::calculateBasketContentsTypeFromEpicsDatabase() - number of positions = " + QString::number(basketPositions.count()));
	for (int i = 0; i < basketPositions.count(); i++)
	{
		basketPositions[i]->calculateBasketContentsTypeFromEpicsDatabase();
	}
}

void positionListClass::loadContentFromDatabase()
{
	LOG("pl.cpp", "positionListClass::loadContentFromDatabase() - mainPosId = " + QString::number(mainPosId));
	LOG("pl.cpp", "positionListClass::loadContentFromDatabase() - number of positions = " + QString::number(basketPositions.count()));
	for (int i = 0; i < basketPositions.count(); i++)
	{
		LOG("pl.cpp", "positionListClass::loadContentFromDatabase() - loading content for position index " + QString::number(i) + " with basket id " + QString::number(basketPositions[i]->getBasketTableId()));
		basketPositions[i]->loadContentFromDatabase();
	}
}

void positionListClass::saveToDatabase()
{
	LOG("pl.cpp", "positionListClass::saveToDatabase() - mainPosId = " + QString::number(mainPosId));
	LOG("pl.cpp", "positionListClass::saveToDatabase() - number of positions = " + QString::number(basketPositions.count()));
	for (int i = 0; i < basketPositions.count(); i++)
	{
		basketPositions[i]->saveToDatabase();
	}
}
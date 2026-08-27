#include "bp.h"
#include "dbTableStatus.h"
#include "configuration/ConfigurationManager.h"
#include "../common/execDBQuery.h"

namespace
{
QGLWidget *basketLabelWidget = NULL;
}

void basketPositionClass::setLabelWidget(QGLWidget *widget)
{
	basketLabelWidget = widget;
}

basketPositionClass::basketPositionClass(positionListClass *parentPositionList_, QString description_, int number_, int index_, int positionEnumForEpics_, GLfloat openGlX_, GLfloat openGlY_, GLfloat openGlZ_) :QObject()
{
	if( this == nullptr) {
		// @@@@@ -- Adding a null pointer check for 'this' in the constructor. This should never happen, but if it does, we log an error message and return without initializing the object to avoid undefined behavior.
		QString mess = "ERROR: basketPositionClass::basketPositionClass() called, but 'this' is null.This should never happen.Returning without initializing the object.";
		LOG("bP.cpp", mess);
		// EXIT("nullptr", mess);
	}

	parentPositionList = parentPositionList_;
	description = description_;
	number = number_;
	index = index_;
	positionEnumForEpics = positionEnumForEpics_;
	openGlX = openGlX_;
	openGlY = openGlY_;
	openGlZ = openGlZ_;
	pickingColor = getColorForPicking();
	isMouseOver = isHighlighted = isSelected = isActive = isConveyorSource = isConveyorTarget = false;
	isLockedByCrane = false;
	isNextBasketForExport = false;
	isNextBasketForOmavDestacker = isNextBasketForAssimakisDestacker = false;
}

//basketPositionClass::basketPositionClass(basketPositionClass *p)
//{
//	if (p)
//	{
//		description=p->description;
//		plcPosition=p->plcPosition;
//		plcIndex=p->plcIndex;
//		openGlX=p->openGlX;
//		openGlY=p->openGlY;
//		openGlZ=p->openGlZ;
//		pickingColor=p->pickingColor;
//		basket=p->basket;
//		isMouseOver=isSelected=isActive=false;
//	}
//	else
//	{
//		plcPosition=plcIndex=0;
//		openGlX=openGlY=openGlZ=0.f;
//	}
//}

basketPositionClass::~basketPositionClass()
{
}

QString basketPositionClass::getDescription()
{
	return description;
}

void basketPositionClass::setBasketTableId(int b, bool contents)
{
	bool basketHasContents = contents;

	if (basket.getTableId() != b)
	{
		QString stmnt = QString("insert into basketHistory (type, position, destination, basketTableId, date, sec) VALUES('%1', '%2', '%3',%4, '%5', '%6')")
			.arg("set").arg((description.trimmed() + " %1 %2").arg(number).arg(index)).arg("-").arg(QString().number(b)).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t());

		setDbTableActive("basketHistory", false);
		execDBQuery(stmnt, MAIN_DB_CONNECTION);
		setDbTableIdle("basketHistory");

		//update epics
		if (b != 0)
		{
			if (positionEnumForEpics == EPICS_POS_BASKET_NEW) {
#ifndef EMULATE
				setDbTableActive("basket_location", false);
				execDBQuery(QString("update basket_location set audittime = CONVERT(datetime,	NULL) where basketnum = %1").arg(b), APEX_DB_CONNECTION);
				setDbTableIdle("basket_location");
#else
				setDbTableActive("basket_location", false);
				execQuery(QString("update basket_location set audittime=0 where basketnum=%1").arg(b), APEX_DB_CONNECTION);
				setDbTableIdle("basket_location");
#endif
			}
			// basketHasContents = hasContents();  // only update if basket has contents.
			if (basketHasContents && (positionEnumForEpics != EPICS_POS_BASKET_NOWHERE) && (positionEnumForEpics != EPICS_POS_BASKET_NEW))
			{
				QVariant time = GETFIRST(QString("select audittime from basket_location where basketnum = %1 and basketlocation = %2").arg(b).arg(positionEnumForEpics), APEX_DB_CONNECTION);
#ifdef EMULATE
				if (time.toInt() == 0) {
					setDbTableActive("basket_location", false);
					execQuery(QString("update basket_location set audittime=datetime('now') where basketnum=%1 and basketlocation=%2").arg(b).arg(positionEnumForEpics), APEX_DB_CONNECTION);
					setDbTableIdle("basket_location");
				}
#endif
#ifndef EMULATE
				if (time.isNull() || positionEnumForEpics == EPICS_POS_BASKET_AFTER_OVEN) {  // force write when after oven pos (cuurently 160) reached regardless
					setDbTableActive("basket_location", false);
					execDBQuery(QString("update basket_location set audittime = CONVERT(datetime, CURRENT_TIMESTAMP) where basketnum = %1 and basketlocation = %2").arg(b).arg(positionEnumForEpics), APEX_DB_CONNECTION);
					setDbTableIdle("basket_location");
				}
#endif
			}
		}
	}
	//

	basket.setTableId(b);
	loadContentFromDatabase();
	saveToDatabase();

/////
//update [PEManager_tmp].[dbo].[basket_location] set audittime=CONVERT(datetime, '12-9-14 13:42:20:123') where basketnum=1 and basketlocation=1
//update [PEManager_tmp].[dbo].[basket_location] set audittime=CONVERT(datetime,NULL) where basketnum=1 and basketlocation=2
//update [PEManager_tmp].[dbo].[basket_location] set audittime=CONVERT(datetime,CURRENT_TIMESTAMP) where basketnum=1 and basketlocation=3
}

void basketPositionClass::setBasketTableIdInMemory(int b)
{
	if (basket.getTableId() == b)
		return;
	basket.setTableId(b);
	loadContentFromDatabase();
}

void basketPositionClass::calculateBasketContentsTypeFromEpicsDatabase()
{
	if (hasBasket()) {
		LOG("bP.cpp", "Start: calculateBasketContentTypeFromEpicsDatabase()" );
		basket.calculateBasketContentsTypeFromEpicsDatabase();
		LOG("bP.cpp", "End: calculateBasketContentTypeFromEpicsDatabase() - basket was: [" + QString::number(basket.getNumber()) + "]");
	}
		
}

void basketPositionClass::loadContentFromDatabase()
{
	// LOG("bP.cpp", "basketPositionClass::loadContentFromDatabase()");
	if (hasBasket())
	{
		calculateBasketContentsTypeFromEpicsDatabase();
		basket.loadContentFromDatabase();
		LOG("bP.cpp", "basketPositionClass::loadContentFromDatabase() - basket has contentsEnum = " + QString::number(basket.getContentsEnum()));
		
	}
	else {
		LOG("bP.cpp", "basketPositionClass::loadContentFromDatabase() - no basket in this position");
	}
}

void basketPositionClass::loadFromDataBase()
{
	// LOG("bP.cpp", "basketPositionClass::loadFromDataBase()");
	int basketId = GETONE(QString("select basketTableId from positions where posNumber = %1 and posIndex = %2").arg(number).arg(index)).toInt();
	// LOG("bP.cpp", "basketPositionClass::loadFromDataBase() - basketId = " + QString::number(basketId));
	basket.setTableId(basketId);
	loadContentFromDatabase();

	int lockedTempEnum = GETONE(QString("select locked from positions where posNumber = %1 and posIndex = %2").arg(number).arg(index)).toInt();
	// LOG("bP.cpp", "basketPositionClass::loadFromDataBase() - locked = " + QString::number(GETONE(QString("select locked from positions where posNumber = %1 and posIndex = %2").arg(number).arg(index)).toInt()));

	if (lockedTempEnum == LOCKED) {
		setLocked(true);
		// LOG("bP.cpp", "basketPositionClass::loadFromDataBase() - position is locked");
	}
	else {
		setLocked(false);
		// LOG("bP.cpp", "basketPositionClass::loadFromDataBase() - position is unlocked");
	}

}

void basketPositionClass::saveToDatabase()
{
	setDbTableActive("positions", false);
	execDBQuery(QString("update positions set basketTableId = %1, locked = %2 where posNumber = %3 and posIndex = %4").arg(basket.getTableId()).arg(lockedEnum).arg(number).arg(index), MAIN_DB_CONNECTION);
	setDbTableIdle("positions");
}

//
void basketPositionClass::setOpenGlX(float x)
{
	openGlX = x;
}

void basketPositionClass::setOpenGlY(float y)
{
	openGlY = y;
}

void basketPositionClass::setOpenGlZ(float z)
{
	openGlZ = z;
}

float basketPositionClass::getOpenGlX()
{
	return openGlX;
}

float basketPositionClass::getOpenGlY()
{
	return openGlY;
}

float basketPositionClass::getOpenGlZ()
{
	return openGlZ;
}

//
int basketPositionClass::getPositionEnumForEpics()
{
	return positionEnumForEpics;
}

bool basketPositionClass::getAreContentsAged()
{
	return basket.getAreContentsAged();
}

bool basketPositionClass::hasNonAgeableTemperMaterial()
{
	return hasBasket() && basket.hasNonAgeableTemperMaterial();
}

bool basketPositionClass::canMoveToPosition(int destinationNumber)
{
	if (!hasNonAgeableTemperMaterial())
		return true;

	switch (destinationNumber)
	{
	case 100:
	case 110:
	case 120:
	case 130:
	case 220:
	case 280:
	case 290:
		return false;
	default:
		return true;
	}
}

int basketPositionClass::getPlcId()
{
	return number + index;
}

int basketPositionClass::getPlcLengthType()
{
	if ((basket.getContentsEnum() == BASKET_CONTENS_NONE) || (basket.getContentsEnum() == BASKET_CONTENS_LESS_THAN_20))
		return 3;
	else if (basket.getContentsEnum() == BASKET_CONTENS_LESS_THAN_24)
		return 1;
	else 
		return 2;
}

int basketPositionClass::getNumber()
{
	return number;
}

int basketPositionClass::getIndex()
{
	return index;
}

bool basketPositionClass::getIsHighlighted()
{
	return isHighlighted;
}

QString basketPositionClass::getToolTipText()
{
	QString ans;
	ans += tr("Position: %1, %2 (%3)\n\n").arg(number + index).arg(lockedEnum == LOCKED ? "locked" : "unlocked").arg(description);
	if (errorString.length() > 0)
		ans+=tr("Error : %1\n\n").arg(errorString);
	
	if (hasBasket())
		ans += tr("%1").arg(basket.getToolTipText());
	
	return ans;
}

positionListClass *basketPositionClass::getParentPositionList()
{
	return parentPositionList;
}

bool basketPositionClass::isFreeToMove()
{
	basketPositionClass *t = parentPositionList->getFreeToMoveBasketPosition();
	if (t && (t == this))
		return true;
	return false;
}

bool basketPositionClass::canRecieve(basketPositionClass* bp)
{
	basketPositionClass *t = parentPositionList->getFreeToRecievePos(bp);
	if (t && (t == this))
		return true;
	return false;
}

bool basketPositionClass::isBasketNormal()
{
	if (basket.getTableId() > 0)
	{
		if (basket.getTypeEnum() == BASKET_NORMAL)
			return true;
	}
	return false;
}

bool basketPositionClass::isBasketForScrap()
{
	if (basket.getTableId() > 0)
	{
		if (basket.getTypeEnum() == BASKET_SCRAP)
			return true;
	}
	return false;
}

bool basketPositionClass::isPosAfterStacker2()
{
	return (basket.getNumber() == 460);
}

void basketPositionClass::setLockedByCrane(bool craneLock)
{
	isLockedByCrane = craneLock;
}

bool basketPositionClass::getIsLockedByCrane()
{
	return isLockedByCrane;
}

int basketPositionClass::getTemperEnum()
{
	return basket.getTemperEnum();
}

void basketPositionClass::setLocked(bool l)
{
	if (l)
	{
		setErrorString("LOCKED");
		lockedEnum = LOCKED;
	}
	else
	{
		lockedEnum = UNLOCKED;
		setErrorString("");
	}
	saveToDatabase();
}

bool basketPositionClass::getLocked()
{
	if (lockedEnum == LOCKED)
		return true;
	return false;
}

void basketPositionClass::setErrorString(QString errorString_)
{
	if (!getLocked())//dont change the locked string
		errorString = errorString_;
}

QString basketPositionClass::getErrorString()
{
	return errorString;
}

//
bool basketPositionClass::getIsActive()
{
	return isActive;
}

void basketPositionClass::setActive(bool a)
{
	isActive = a;
	isMouseOver = false;//bug
	isHighlighted = false;
}

void basketPositionClass::setConveyorSource(bool source)
{
	isConveyorSource = source;
}

void basketPositionClass::setConveyorTarget(bool target)
{
	isConveyorTarget = target;
}

void basketPositionClass::setNextBasketForExport(int basketId)//gui only
{
	if (basket.getTableId() == basketId)
		isNextBasketForExport = true;
	else
		isNextBasketForExport = false;
}

void basketPositionClass::setNextBasketForOmavDestacker(int basketId)//gui only
{
	if (basket.getTableId() == basketId)
		isNextBasketForOmavDestacker = true;
	else
		isNextBasketForOmavDestacker = false;
}

void basketPositionClass::setNextBasketForAssimakisDestacker(int basketId)//gui only
{
	if (basket.getTableId() == basketId)
		isNextBasketForAssimakisDestacker = true;
	else
		isNextBasketForAssimakisDestacker = false;
}

int basketPositionClass::getBasketTableId()
{
	if (this == nullptr)
	{
		// @@@@@ - defensive check to prevent crash when this is null, which was observed in some cases. This should not happen, but if it does, we will log it and return 0 instead of crashing.
		QString mess = "ERROR: basketPositionClass::getBasketTableId() - ERROR: this is nullptr, cannot get basket table id"; // @@@@@ - Blew up here...
		LOG("bP.cpp", mess);
		return 0;
	}
	else {
		QString mess = "INFO: basketPositionClass::getBasketTableId() - this is a good pointer";
		// LOG("bP.cpp", mess);
	}
	
	/* @@@@@
	Exception thrown: read access violation.
	"this" was nullptr.
	*/
	if (hasBasket()) {
		// LOG("bP.cpp", "basketPositionClass::getBasketTableId() - basket in this position, table id = " + QString::number(basket.getTableId()));
		return basket.getTableId();
	}
	// LOG("bP.cpp", "basketPositionClass::getBasketTableId() - no basket in this position, returning 0");
	return 0; 
}

int basketPositionClass::getBasketNumber()
{
	if (hasBasket())
		return basket.getNumber();

	return 0; 
}

bool basketPositionClass::hasBasket()
{
	bool ans = (basket.getTableId() == 0) ? false : true;
	// LOG("bP.cpp", "basketPositionClass::hasBasket() - " + QString(ans ? "true" : "false"));
	return ans;
}

bool basketPositionClass::hasContents()
{
	if (hasBasket())
		return basket.hasContents();

	return false;
}

int basketPositionClass::getTypeEnum()
{
	if (hasBasket())
		return basket.getTypeEnum();

	return false;
}

int basketPositionClass::getContentsEnum()
{
	if (hasBasket())
		return basket.getContentsEnum();
	return BASKET_CONTENS_NONE;
}

QColor basketPositionClass::basketDisplayColor()
{
	if (basket.getTypeEnum() == BASKET_SCRAP)
		return QColor(255, 255, 0);

	if (basket.getTypeEnum() == BASKET_NORMAL)
	{
		const int nonAgeableTempers = basket.getNonAgeableTemperMask();
		if (nonAgeableTempers != basketClass::NON_AGEABLE_TEMPER_NONE)
			return QColor(255, 20, 147); // Bright pink identifies all no-age material (F, T1, T4).
		if (basket.getTemperEnum() == basketClass::BASKET_TEMPER_185)
			return QColor(0, 125, 150);
		if (basket.getTemperEnum() == basketClass::BASKET_TEMPER_230)
			return QColor(255, 170, 39);
		return QColor(255, 255, 255);
	}

	return QColor(0, 0, 0);
}

void basketPositionClass::drawBasket(bool drawTexture)
{
	const QColor color = basketDisplayColor();
	if (color != QColor(0, 0, 0))
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		glColor3ub(color.red(), color.green(), color.blue());
	}
	basket.drawGeometry(drawTexture);
	if (color != QColor(0, 0, 0))
		glPopAttrib();
}

#include "uglyfont.h"
///
void basketPositionClass::draw(bool forPicking, bool isFromPos, bool isToPos)
{
	// LOG("bP.cpp", "basketPositionClass::draw() basket=" + QString::number(basket.getNumber()) + " - forPicking = " + QString(forPicking ? "true" : "false") + ", isFromPos = " + QString(isFromPos ? "true" : "false") + ", isToPos = " + QString(isToPos ? "true" : "false"));
	
	QColor color = QColor(0, 0, 0);
	bool drawTexture = true;

	// LOG("bP.cpp", "Step 1: Determine color and texture settings based on state");
	if (forPicking)
	{
		color = pickingColor;
		drawTexture = false;
	}
	else if (isMouseOver)
		color = QColor(0, 255, 0);
	else if (isHighlighted)
		color = QColor(0, 255, 255);
	else if (isSelected)
		color = QColor(0, 0, 255);
	else if (isActive)
		color = QColor(255, 0, 255);
	else
		color = basketDisplayColor();

	// LOG("bP.cpp", "Step 2: Check for error state and override color if necessary");
	if ((getErrorString().length() > 0) && (!forPicking) && (!isMouseOver))
		color = QColor(255, 0, 0);

	/////////////////////////////////
	// LOG("bP.cpp", "Step 3: Set up OpenGL state for drawing");
	glPushMatrix();
	glTranslatef(openGlX, openGlY, openGlZ);

	// LOG("bP.cpp", "Step 4: Apply color if not default");
	if (color != QColor(0, 0, 0))
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		glColor3ub(color.red(), color.green(), color.blue());
	}

	// LOG("bP.cpp", "Step 5: Draw position based on isFromPos and isToPos flags");
	if (isFromPos)
	{
		// LOG("bP.cpp", "Drawing from position with semi-transparent overlay");
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0f, 0.35f, 0.08f, 0.85f);
		basket.drawNormalBasketGeometry(false);
		//if (basket.getTypeEnum()==BASKET_NORMAL)
		//	basket.drawNormalBasketGeometry(false);
		//else if (basket.getTypeEnum()==BASKET_SCRAP)
		//	basket.drawScrapBasketGeometry(false);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glPopAttrib();
	}
	else if (isToPos)
	{
		// LOG("bP.cpp", "Drawing to position with semi-transparent overlay");
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		//glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.5f, 0.0f, 0.5f, 0.55f);
		drawBaseGeometry(false);
		basket.drawNormalBasketGeometry(false);
		//if (basket.getTypeEnum()==BASKET_NORMAL)
		//	basket.drawNormalBasketGeometry(false);
		//else if (basket.getTypeEnum()==BASKET_SCRAP)
		//	basket.drawScrapBasketGeometry(false);
		glDisable(GL_BLEND);
		//		glDepthMask(GL_TRUE);	
		glPopAttrib();
	}
	// LOG("bP.cpp", "basketPositionClass::draw() - isToPos=" + QString::number(isToPos) + " isFromPos=" + QString::number(isFromPos) );
	if ((!isFromPos) && (!isToPos))
	{
		// LOG("bP.cpp", "Drawing normal position");
		if (hasBasket()) {
			// LOG("bP.cpp", "Drawing basket geometry");
			basket.drawGeometry(drawTexture);
		}
		else {
			// LOG("bP.cpp", "No basket, drawing base geometry");
			drawBaseGeometry(drawTexture);
		}
	}

	if (hasBasket() && (!forPicking) && (!isFromPos) && (!isToPos))
	{
		const BasketCrane::Configuration::VisualizationSettings& visualization =
			BasketCrane::Configuration::ConfigurationManager::Instance().Visualization();
		if (basketLabelWidget)
		{
			if (visualization.ShowBasketNumbers())
			{
				QFont basketLabelFont("Segoe UI", visualization.BasketLabelFontPointSize());
				basketLabelFont.setBold(true);
				glColor3f(1.0f, 1.0f, 0.0f);
				basketLabelWidget->renderText(0.0, 0.0, 0.55, QString("Basket # %1").arg(basket.getNumber()), basketLabelFont);
			}
			if (visualization.ShowLocationLabels())
			{
				QFont locationLabelFont("Segoe UI", visualization.LocationLabelFontPointSize());
				glColor3f(1.0f, 1.0f, 1.0f);
				basketLabelWidget->renderText(0.0, 0.0, 0.30, QString("Location %1").arg(number + index), locationLabelFont);
			}
		}
	}

	if (isConveyorSource && (!isFromPos) && (!isToPos))
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.5f, 1.0f, 0.5f, 0.20f);
		drawBaseGeometry(false);
		glDisable(GL_BLEND);
		glPopAttrib();
	}
	else if (isConveyorTarget && (!isFromPos) && (!isToPos))
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.5f, 0.5f, 1.0f, 0.20f);
		drawBaseGeometry(false);
		glDisable(GL_BLEND);
		glPopAttrib();
	}

	// LOG("bP.cpp", "Step 6: Draw error string if present and not picking");
	if ((getErrorString().length() > 0) && (!forPicking))
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		glPushMatrix();
		if (basket.getTypeEnum() == BASKET_NORMAL)
		{
			glTranslatef(0.5f, -0.01f, 0.01f);
			glScalef(0.1f, 0.1f, 0.1f);
			glRotatef(90., 1.0f, 0.0f, 0.0f);
			glColor3f(1.0f, 1.0f, 1.0f);
			YsDrawUglyFont(errorString.toLatin1(), 0);
		}
		else if (basket.getTypeEnum() == BASKET_SCRAP)
		{
			glTranslatef(2.0f, -0.01f, 0.01f);
			glScalef(0.1f, 0.1f, 0.1f);
			glRotatef(90., 1.0f, 0.0f, 0.0f);
			glColor3f(1.0f, 1.0f, 1.0f);
			YsDrawUglyFont(errorString.toLatin1(), 0);
		}
		// LOG("bP.cpp", "Step 6a: Draw error string - " + errorString);
		glPopAttrib();
		glPopMatrix();
	}

	// Next basket for Destackers
	//if (isNextBasketForOmavDestacker || isNextBasketForAssimakisDestacker)
	// LOG("bP.cpp", "Step 7: Check if this is the next basket for export or destackers - isNextBasketForExport=" + QString(isNextBasketForExport ? "true" : "false") + ", isNextBasketForOmavDestacker=" + QString(isNextBasketForOmavDestacker ? "true" : "false") + ", isNextBasketForAssimakisDestacker=" + QString(isNextBasketForAssimakisDestacker ? "true" : "false"));
	if (isNextBasketForExport)
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		glPushMatrix();
		glTranslatef(3.0f, +0.75f, 1.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		glRotatef(90., 1.0f, 0.0f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		YsDrawUglyFont("1", 0);

		//if (isNextBasketForOmavDestacker)
		//	glColor3f(1.0f, 0.0f, 0.0f);
		//else
		//	glColor3f(0.0f, 0.0f, 1.0f);
		//glLineWidth(2.5f);
		//YsDrawUglyFont(isNextBasketForOmavDestacker ? "1" : "2", 0);
	
		// LOG("bP.cpp", "Step 7a: Draw next basket for export/destacker indicator");
		glLineWidth(1.f);
		glPopAttrib();
		glPopMatrix();
	}

	// LOG("bP.cpp", "Step 8: Check if position is locked and draw locked indicator if necessary - isLocked=" + QString(getLocked() ? "true" : "false"));
	if (color != QColor(0, 0, 0))
		glPopAttrib();

	// LOG("bP.cpp", "Step 9: Draw locked indicator if position is locked");
	glPopMatrix();

	// LOG("bP.cpp", "basketPositionClass::draw() - completed drawing position");
}

void basketPositionClass::drawLockedToRecieve()
{
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
	glPushMatrix();
	glColor3f(0.5f, 0.0f, 0.0f);
	//glTranslatef(openGlX,openGlY,openGlZ);
	//glColor3f(1.0f,0.0f,0.0f);
	//glTranslatef(openGlX-3.2,openGlY-0.7,openGlZ);
	glTranslatef(openGlX, openGlY, openGlZ);
	glScalef(0.5f, 0.5f, 0.5f);
	//glTranslatef(10,2,0.0);
	//drawBaseGeometry(true);
	YsDrawUglyFont(QString("  Locked").toLatin1(), 0);
	glPopAttrib();
	glPopMatrix();
}

extern GLfloat	*triVertexesForEmptyPosArray;
extern GLfloat	*triNormalsForEmptyPosArray;
extern GLfloat	*triTexCoordForEmptyPosArray;
extern GLfloat	*quadVertexesForEmptyPosArray;
extern GLfloat	*quadNormalsForEmptyPosArray;
extern GLfloat	*quadTexCoordForEmptyPosArray;

extern int numberOfTrisForEmptyPos;
extern int numberOfQuadForEmptyPos;
extern GLfloat *animateArrowDz;
extern int animateArrowDzFrame;

void basketPositionClass::drawBaseGeometry(bool drawTexture)
{
	if (drawTexture)
		glBindTexture(GL_TEXTURE_2D, ARROW_TEXTURE);

	glVertexPointer(3, GL_FLOAT, 0, &triVertexesForEmptyPosArray[0]);
	glNormalPointer(GL_FLOAT, 0, &triNormalsForEmptyPosArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &triTexCoordForEmptyPosArray[0]);
	glDrawArrays(GL_TRIANGLES, 0, numberOfTrisForEmptyPos * 3);

	if (drawTexture)
		glBindTexture(GL_TEXTURE_2D, ARROW_TEXTURE);

	glVertexPointer(3, GL_FLOAT, 0, &quadVertexesForEmptyPosArray[0]);
	glNormalPointer(GL_FLOAT, 0, &quadNormalsForEmptyPosArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &quadTexCoordForEmptyPosArray[0]);
	glDrawArrays(GL_QUADS, 0, numberOfQuadForEmptyPos * 4);
}

void basketPositionClass::setSelected(bool s)
{
	isSelected = s;
}

void basketPositionClass::mouseOver(QColor pickedColor)
{
	if (pickedColor == pickingColor)
		isMouseOver = true;
	else
		isMouseOver = false;
}

void basketPositionClass::mouseOverHighlitingMode(QColor pickedColor)
{
	if (pickedColor == pickingColor)
		isHighlighted = true;
	else
		isHighlighted = false;
}

void basketPositionClass::mouseClicked(QColor pickedColor, bool rightClick)
{
	if (pickedColor == pickingColor)
	{
		//if (rightClick)
		//{
		//	if (basket.getTableId()>0)
		//	{
		//		defineBasketContentsClass *defineBasketContents=new defineBasketContentsClass(basket.getTableId(),NULL);
		//		defineBasketContents->exec();
		//		basket.loadContentFromDatabase();
		//	}
		//}
		//else
		//{
		isSelected = true;
		emit positionSelectedSignal(this);
		//}
	}
	else
	{
		isSelected = false;
	}
}

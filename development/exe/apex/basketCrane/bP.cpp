#include "bp.h"
basketPositionClass::basketPositionClass(positionListClass *parrentPositionList_,QString description_,int number_,int index_,int positionEnumForEpics_,GLfloat openGlX_,GLfloat openGlY_,GLfloat openGlZ_):QObject()
{
	parrentPositionList=parrentPositionList_;
	description=description_;
	number=number_; 
	index=index_;
	positionEnumForEpics=positionEnumForEpics_;
	openGlX=openGlX_;
	openGlY=openGlY_;
	openGlZ=openGlZ_;
	pickingColor=getColorForPicking();
	isMouseOver=isHighlighted=isSelected=isActive=false;
	isLockedByCrane=false;
	isNextBasketForOmavDestacker=isNextBasketForAssimakisDestacker=false;
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
void basketPositionClass::setBasketTableId(int b)
{
	if (basket.getTableId()!=b)
	{
		QString stmnt=QString("insert into basketHistory (type,position,destination,basketTableId,date,sec) VALUES('%1','%2','%3',%4,'%5','%6')")
			.arg("set").arg((description.trimmed()+" %1 %2").arg(number).arg(index)).arg("-").arg(QString().number(b)).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t());
		execQuery(stmnt,MAIN_DB_CONNECTION);
		//update epics
		if (b!=0)
		{
			if (positionEnumForEpics==EPICS_POS_BASKET_NEW)
#ifndef EMULATE
				execQuery(QString("update basket_location set audittime=CONVERT(datetime,NULL) where basketnum=%1").arg(b),APEX_DB_CONNECTION);
#else
				execQuery(QString("update basket_location set audittime=0 where basketnum=%1").arg(b),APEX_DB_CONNECTION);
#endif
			if ((positionEnumForEpics!=EPICS_POS_BASKET_NOWHERE)&&(positionEnumForEpics!=EPICS_POS_BASKET_NEW))
			{
				QVariant time=GETFIRST(QString("select audittime from basket_location where basketnum=%1 and basketlocation=%2").arg(b).arg(positionEnumForEpics),APEX_DB_CONNECTION);
#ifdef EMULATE
				if (time.toInt()==0)
					execQuery(QString("update basket_location set audittime=datetime('now') where basketnum=%1 and basketlocation=%2").arg(b).arg(positionEnumForEpics),APEX_DB_CONNECTION);
#endif
#ifndef EMULATE
				if (time.isNull())
					execQuery(QString("update basket_location set audittime=CONVERT(datetime,CURRENT_TIMESTAMP) where basketnum=%1 and basketlocation=%2").arg(b).arg(positionEnumForEpics),APEX_DB_CONNECTION);
#endif
			}
		}
	}
	//
	basket.setTableId(b);
	loadContentFromsDatabase();
	saveToDatabase();
/////
//update [PEManager_tmp].[dbo].[basket_location] set audittime=CONVERT(datetime, '12-9-14 13:42:20:123') where basketnum=1 and basketlocation=1
//update [PEManager_tmp].[dbo].[basket_location] set audittime=CONVERT(datetime,NULL) where basketnum=1 and basketlocation=2
//update [PEManager_tmp].[dbo].[basket_location] set audittime=CONVERT(datetime,CURRENT_TIMESTAMP) where basketnum=1 and basketlocation=3
}
void basketPositionClass::calculateBasketContentsTypeFromEpicsDatabase()
{
	if (hasBasket())
		basket.calculateBasketContentsTypeFromEpicsDatabase();
}
void basketPositionClass::loadContentFromsDatabase()
{
	if (hasBasket())
	{
		calculateBasketContentsTypeFromEpicsDatabase();
		basket.loadContentFromsDatabase();
	}
}
void basketPositionClass::loadFromDataBase()
{
	int basketId=GETONE(QString("select basketTableId from positions where posNumber=%1 and posIndex=%2").arg(number).arg(index)).toInt();
	basket.setTableId(basketId);
	loadContentFromsDatabase();
	int lockedTempEnum=GETONE(QString("select locked from positions where posNumber=%1 and posIndex=%2").arg(number).arg(index)).toInt();
	if (lockedTempEnum==LOCKED)
		setLocked(true);
	else 
		setLocked(false);
}
void basketPositionClass::saveToDatabase()
{
	execQuery(QString("update positions set basketTableId=%1 , locked=%2 where posNumber=%3 and posIndex=%4").arg(basket.getTableId()).arg(lockedEnum).arg(number).arg(index),MAIN_DB_CONNECTION);
}
//
void basketPositionClass::setOpenGlX(float x)
{
	openGlX=x;
}
void basketPositionClass::setOpenGlY(float y)
{
	openGlY=y;
}
void basketPositionClass::setOpenGlZ(float z)
{
	openGlZ=z;
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
int basketPositionClass::getPlcId()
{
	return number+index;
}
int basketPositionClass::getPlcLengthType()
{
	if (  (basket.getContentsEnum()==BASKET_CONTENS_NONE)   ||   (basket.getContentsEnum()==BASKET_CONTENS_LESS_THAN_20)  )
		return 3;
	else  if  (basket.getContentsEnum()==BASKET_CONTENS_LESS_THAN_24)
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
	ans+=tr("Position:%1, %2 (%3)\n\n").arg(number+index).arg(lockedEnum==LOCKED?"locked":"unlocked").arg(description);
	if (errorString.length()>0)
		ans+=tr("Error:%1\n\n").arg(errorString);
		if (hasBasket())
	{
		ans+=tr("%1").arg(basket.getToolTipText());
	}
	return ans;
}
positionListClass *basketPositionClass::getParrentPositionList()
{
	return parrentPositionList;
}
bool basketPositionClass::isFreeToMove()
{
	basketPositionClass *t=parrentPositionList->getFreeToMoveBasketPosition();
	if (t&&(t==this))
		return true;
	return false;
}
bool basketPositionClass::canRecieve(basketPositionClass* bp)
{
	basketPositionClass *t=parrentPositionList->getFreeToRecievePos(bp);
	if (t&&(t==this))
		return true;
	return false;
}
bool basketPositionClass::isBasketNormal()
{
	if (basket.getTableId()>0)
	{
		if (basket.getTypeEnum()==BASKET_NORMAL)
			return true;
	}
	return false;
}
bool basketPositionClass::isBasketForScrap()
{
	if (basket.getTableId()>0)
	{
		if (basket.getTypeEnum()==BASKET_SCRAP)
			return true;
	}
	return false;
}
void basketPositionClass::setLockedByCrane(bool craneLock)
{
	isLockedByCrane=craneLock;
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
		lockedEnum=LOCKED;
	}
	else
	{
		lockedEnum=UNLOCKED;
		setErrorString("");
	}
	saveToDatabase();
}
bool basketPositionClass::getLocked()
{
	if (lockedEnum==LOCKED)
		return true;
	return false;
}
void basketPositionClass::setErrorString(QString errorString_)
{
	if (!getLocked())//dont change the locked string
		errorString=errorString_;
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
	isActive=a;
	isMouseOver=false;//bug
	isHighlighted=false;
}
void basketPositionClass::setNextBasketForOmavDestacker(int basketId)//gui only
{
	if (basket.getTableId()==basketId)
		isNextBasketForOmavDestacker=true;
	else
		isNextBasketForOmavDestacker=false;
}
void basketPositionClass::setNextBasketForAssimakisDestacker(int basketId)//gui only
{
	if (basket.getTableId()==basketId)
		isNextBasketForAssimakisDestacker=true;
	else
		isNextBasketForAssimakisDestacker=false;
}

int basketPositionClass::getBasketTableId()
{
	if (hasBasket())
		return basket.getTableId();
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
	return (basket.getTableId()==0)?false:true;
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
void basketPositionClass::drawBasket(bool drawTexture)
{
	basket.drawGeometry(drawTexture);
}
#include "uglyfont.h"
///
void basketPositionClass::draw(bool forPicking,bool isFromPos,bool isToPos)
{
	QColor color=QColor(0,0,0);
	bool drawTexture=true;
	if (forPicking)
	{
		color=pickingColor;
		drawTexture=false;
	}
	else if (isMouseOver)
		color=QColor(0,255,0);
	else if (isHighlighted)
		color=QColor(0,255,255);
	else if (isSelected)
		color=QColor(0,0,255);
	else if (isActive)
		color=QColor(255,0,255);
	else if (basket.getTypeEnum()==BASKET_SCRAP)
		color=QColor(255,255,0);
	else if (basket.getTypeEnum()==BASKET_NORMAL)
	{
		color=QColor(255,255,255);
		if (basket.getTemperEnum()==basketClass::BASKET_TEMPER_185)
			color=QColor(0,125,150);
		else if (basket.getTemperEnum()==basketClass::BASKET_TEMPER_230)
			color=QColor(255,170,39);
	}
	if ((getErrorString().length()>0)&&(!forPicking)&&(!isMouseOver))
		color=QColor(255,0,0);
	/////////////////////////////////
	glPushMatrix();
	glTranslatef(openGlX,openGlY,openGlZ);
	if (color!=QColor(0,0,0))
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		glColor3ub(color.red(),color.green(),color.blue());
	}
	if (isFromPos)
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
//		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.5f,1.0f,0.5f,0.20f);
		basket.drawNormalBasketGeometry(false);
		//if (basket.getTypeEnum()==BASKET_NORMAL)
		//	basket.drawNormalBasketGeometry(false);
		//else if (basket.getTypeEnum()==BASKET_SCRAP)
		//	basket.drawScrapBasketGeometry(false);
		glDisable(GL_BLEND);
//		glDepthMask(GL_TRUE);	
		glPopAttrib();
	}
	else if (isToPos)
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		//glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.5f,0.5f,1.0f,0.20f);
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
	if ((!isFromPos)&&(!isToPos))
	{
		if (hasBasket())
			basket.drawGeometry(drawTexture);
		else
			drawBaseGeometry(drawTexture);
	}
	///////
	if ((getErrorString().length()>0)&&(!forPicking))
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		glPushMatrix();
		if (basket.getTypeEnum()==BASKET_NORMAL)
		{
			glTranslatef(0.5f,-0.01f,0.01f);
			glScalef(0.1f,0.1f,0.1f);
			glRotatef(90.,1.0f,0.0f,0.0f);
			glColor3f(1.0f,1.0f,1.0f);
			YsDrawUglyFont(errorString.toLatin1(),0);
		}
		else if (basket.getTypeEnum()==BASKET_SCRAP)
		{
			glTranslatef(2.0f,-0.01f,0.01f);
			glScalef(0.1f,0.1f,0.1f);
			glRotatef(90.,1.0f,0.0f,0.0f);
			glColor3f(1.0f,1.0f,1.0f);
			YsDrawUglyFont(errorString.toLatin1(),0);
		}
		glPopAttrib();
		glPopMatrix();
	}
	//isNextBasketForOmavDestacker
	if (isNextBasketForOmavDestacker||isNextBasketForAssimakisDestacker)
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		glPushMatrix();
			glTranslatef(3.0f,+0.75f,1.0f);
			glScalef(0.5f,0.5f,0.5f);
			glRotatef(90.,1.0f,0.0f,0.0f);
			if (isNextBasketForOmavDestacker)
				glColor3f(1.0f,0.0f,0.0f);
			else
				glColor3f(0.0f,0.0f,1.0f);
			glLineWidth(2.5f); 
			YsDrawUglyFont(isNextBasketForOmavDestacker?"1":"2",0);
			glLineWidth(1.f); 
		glPopAttrib();
		glPopMatrix();
	}
	if (color!=QColor(0,0,0))
		glPopAttrib();
	glPopMatrix();
}
void basketPositionClass::drawLockedToRecieve()
{
	glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
	glPushMatrix();
	glColor3f(0.5f,0.0f,0.0f);
	//glTranslatef(openGlX,openGlY,openGlZ);
	//glColor3f(1.0f,0.0f,0.0f);
	//glTranslatef(openGlX-3.2,openGlY-0.7,openGlZ);
	glTranslatef(openGlX,openGlY,openGlZ);
	glScalef(0.5f,0.5f,0.5f);
	//glTranslatef(10,2,0.0);
	//drawBaseGeometry(true);
	YsDrawUglyFont(QString("  Locked").toLatin1(),0);
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
		glBindTexture(GL_TEXTURE_2D,ARROW_TEXTURE);
	glVertexPointer(3,GL_FLOAT,0,&triVertexesForEmptyPosArray[0]);
	glNormalPointer(GL_FLOAT,0,&triNormalsForEmptyPosArray[0]);
	glTexCoordPointer(2,GL_FLOAT,0,&triTexCoordForEmptyPosArray[0]);
	glDrawArrays(GL_TRIANGLES,0,numberOfTrisForEmptyPos*3);
	if (drawTexture)
		glBindTexture(GL_TEXTURE_2D,ARROW_TEXTURE);
	glVertexPointer(3,GL_FLOAT,0,&quadVertexesForEmptyPosArray[0]);
	glNormalPointer(GL_FLOAT,0,&quadNormalsForEmptyPosArray[0]);
	glTexCoordPointer(2,GL_FLOAT,0,&quadTexCoordForEmptyPosArray[0]);
	glDrawArrays(GL_QUADS,0,numberOfQuadForEmptyPos*4);
}
void basketPositionClass::setSelected(bool s)
{
	isSelected=s;
}

void basketPositionClass::mouseOver(QColor pickedColor)
{
	if (pickedColor==pickingColor)
		isMouseOver=true;
	else
		isMouseOver=false;
}
void basketPositionClass::mouseOverHighlitingMode(QColor pickedColor)
{
	if (pickedColor==pickingColor)
		isHighlighted=true;
	else
		isHighlighted=false;
}
void basketPositionClass::mouseClicked(QColor pickedColor,bool rightClick)
{
	if (pickedColor==pickingColor)
	{
		//if (rightClick)
		//{
		//	if (basket.getTableId()>0)
		//	{
		//		defineBasketContentsClass *defineBasketContents=new defineBasketContentsClass(basket.getTableId(),NULL);
		//		defineBasketContents->exec();
		//		basket.loadContentFromsDatabase();
		//	}
		//}
		//else
		//{
			isSelected=true;
			emit positionSelectedSignal(this);
		//}
	}
	else
	{
		isSelected=false;
	}
}

#ifndef BP_H
#define BP_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "cG.h"
#include "op.h"
#include "b.h"

class basketPositionClass;
class positionListClass;

#include "pl.h"

class basketPositionClass:public QObject			
{
	QString							description;
	positionListClass				*parentPositionList;
	int								number;  //300,310, 320 etc
	int								index;   //1,2,3,4
	//
	bool							isNextBasketForExport;			// gui only
	bool							isNextBasketForOmavDestacker;  // gui only
	bool							isNextBasketForAssimakisDestacker; // gui only
	/////////////////////////////////////////
	GLfloat							openGlX;
	GLfloat							openGlY;
	GLfloat							openGlZ;
	QColor							pickingColor;
	//
	basketClass						basket;

	bool							isMouseOver;
	bool							isHighlighted;
	bool							isSelected;
	bool							isActive;
//	bool							isLocked;

	QString							errorString;
	bool							isLockedByCrane;//on movment only

	Q_OBJECT

public:
	//
	int								positionEnumForEpics;
	enum {
		EPICS_POS_BASKET_NOWHERE = 0,
		EPICS_POS_BASKET_BEFORE_OVEN,
		EPICS_POS_BASKET_IN_OVEN,
		EPICS_POS_BASKET_AFTER_OVEN,
		EPICS_POS_BASKET_NEW,
	};

	bool operator == (const basketPositionClass& t)
	{
		if ((description != t.description) || (number != t.number) || (index != t.index))
			return false;

		return true;
	}

	int						lockedEnum;
	enum{
		UNLOCKED = 1,
		LOCKED,
	};
	///////
	basketPositionClass(positionListClass *parentPositionList_, QString description_, int number_, int index_, int positionEnumForEpics_, GLfloat openGlX_, GLfloat openGlY_, GLfloat openGlZ_);
	//basketPositionClass(basketPositionClass *p);
	~basketPositionClass();
	//
	void setNextBasketForExport(int basketId); // gui only
	void setNextBasketForOmavDestacker(int basketId); //gui only
	void setNextBasketForAssimakisDestacker(int basketId); //gui only
	//
	void setOpenGlX(float x);
	void setOpenGlY(float y);
	void setOpenGlZ(float z);
	float getOpenGlX();
	float getOpenGlY();
	float getOpenGlZ();
	//
	void setBasketTableId(int b, bool contents = false);
	void calculateBasketContentsTypeFromEpicsDatabase();
	void loadContentFromDatabase();
	int getBasketTableId();
	int getBasketNumber();
	bool hasBasket();
	bool hasContents();
	bool getIsActive();
	void setActive(bool a);
	int getTypeEnum();
	int getContentsEnum();
	int getTemperEnum();
	void setSelected(bool s);
	void setLocked(bool l);
	bool getLocked();
	void setErrorString(QString errorString_);
	QString getErrorString();
	int getPlcId();
	int getPlcLengthType();
	int getNumber();
	int getIndex();
	positionListClass *getParentPositionList();
	bool isFreeToMove();
	bool getIsHighlighted();
	QString getToolTipText();
	bool canRecieve(basketPositionClass* bp);
	bool isBasketNormal();
	bool isBasketForScrap();
	bool isPosAfterStacker2();
	void setLockedByCrane(bool craneLock);
	bool getIsLockedByCrane();
	QString getDescription();
	//
	int getPositionEnumForEpics();
	bool getAreContentsAged();
	//
	void draw(bool forPicking,bool isFromPos,bool isToPos);
	void mouseOver(QColor pickedColor);
	void mouseOverHighlitingMode(QColor pickedColor);
	void mouseClicked(QColor pickedColor,bool rightClick);
	void drawBaseGeometry(bool drawTexture);
	void drawBasket(bool drawTexture);
	void drawLockedToRecieve();
//
	void loadFromDataBase();
	void saveToDatabase();

signals:
	void openGlChanged();
	void positionSelectedSignal(basketPositionClass *b);
};
#endif

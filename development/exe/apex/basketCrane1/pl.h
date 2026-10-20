#ifndef PL_H
#define PL_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "cG.h"
#include "bp.h"

class positionListClass;

class positionListClass:public QObject			
{
protected:
	QString lastDestinationDiagnostic;
	QString								description;
	int									mainPosId;
	//
	QVector<basketPositionClass*>		basketPositions;
	QVector<positionListClass*>			obstructingPositionListsArray;
	QVector<positionListClass*>			concurrentPositionListsArray;
	QVector<positionListClass*>			behindPositionListsArray;
	bool								isLockedToRecieve;
	bool trackingReconciliationRequired;

	Q_OBJECT

public:
	int						typeEnumeration;
	enum{
		TYPE_NONE			=0x00000000,
		TYPE_NORMAL			=0x00000001,
		TYPE_SCRAP			=0x00000002,
		TYPE_EMPTY			=0x00000004,
		TYPE_PROFILES20		=0x00000008,
		TYPE_PROFILES24		=0x00000010,
		TYPE_PROFILES30		=0x00000020,
	};

public:
	positionListClass(int mainPosId_,int typeEnum_,int positionEnumForEpics,GLfloat openGlX,GLfloat openGlY);
	~positionListClass();

	//
	void drawAllPosWithBasket(bool forPicking=false);//no for picking
	void drawFreeToMoveBasket();//for picking only
	void drawFreeToRecievePos(bool forPicking,basketPositionClass *incomingBasketPos);
	void mouseOver(QColor pickedColor);
	void mouseOverHighlitingMode(QColor pickedColor);
	void mouseClicked(QColor pickedColor,bool rightClick);
	//
	void addObstructingPositionList(positionListClass *list);
	void addBehindPositionLists(positionListClass *list);
	void addConcurrentPositionLists(positionListClass *list);
	//
	void loadFromDatabase();
	void calculateBasketContentsTypeFromEpicsDatabase();
	void loadContentFromDatabase();
	void saveToDatabase();
	//
	void setNextBasketForExport(int basketId); //gui only
	void setNextBasketForOmavDestacker(int basketId); //gui only
	void setNextBasketForAssimakisDestacker(int basketId); //gui only

	//bool canBasketMove(int index);
	//bool canDropBasket(int index);

	bool isBasketOnTop(int index);
	bool isBasketPositionYours(basketPositionClass* incomingPos);
	bool isSomethingObstructingThePositionsWay(basketPositionClass* incomingPos=NULL);
	bool isAConcurrentPositionFull();
	bool canTakeFullBasket();
	bool canTakeEmptyBasket();
	bool hasEmptyBasket();
	bool hasFullBasket();
	int getNumberOfBaskets();
	int getNumberOfPositions();
	int getTopBasketTableId();
	int getMainPosId();
	bool hasLockedPositionByCrane();
	bool isTheBottomBasket(int b);
	basketPositionClass* getFreeToMoveBasketPosition();
	basketPositionClass* getHighlightedPos();
	basketPositionClass* getFreeToRecievePos(basketPositionClass* incomingPos, QString* rejectionReason = NULL);
	basketPositionClass *getPosition(int index);
	// Additional 910 indices are durable tracking records, not crane destinations.
	basketPositionClass* appendStacker2TrackingPosition();
	QVector<basketPositionClass*> getBasketPositions();
	QVector<basketPositionClass*> getAllBasketPositions();
	basketPositionClass* getBaskePositiontWithBasketId(int id);
	basketPositionClass* getPositionWithAnagedBasket();
	basketPositionClass* getTopBasketPosition();
	bool hasBaskePositiontWithBasketId(int id);
	void setLockRecieve(bool l);
	void setTrackingReconciliationRequired(bool required) { trackingReconciliationRequired = required; }
	void drawLockedToRecieve();

public slots:								
	void positionSelectedSlot(basketPositionClass* b);

signals:
	void positionSelectedSignal(basketPositionClass* b);
};
#endif

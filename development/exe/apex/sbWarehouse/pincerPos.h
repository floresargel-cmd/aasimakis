#ifndef	PINCERPOS_H
#define PINCERPOS_H
class pincerPositionClass;
#include "serv.h"
#include "GsW.h"
#include "gItem.h"
#include "gText.h"
#include "gView.h"
#include "posDlg.h"
#include "pos.h"
class pincerPositionClass:public QWidget	
{ 
	tuxipClass											*tuxip;
	posClass												*parrentPos;

	pincerPositionClass							*abovePosition;
	pincerPositionClass							*belowPosition;
	//
	int															permanentMode;
	int															tempMode;

	QGraphicsScene									*myScene;
	gItemClass											*currentGraphicsItem											;
	gItemClass											*emptyBasketGraphicsItem									;
	gItemClass											*fullBasketGraphicsItem										;
	gItemClass											*basketPlcFromWithBasketGraphicsItem			;
	gItemClass											*basketPlcFromWithOutBasketGraphicsItem		;
	gItemClass											*basketPlcToWithBasketGraphicsItem				;
	gItemClass											*basketPlcToWithOutBasketGraphicsItem			;
	gItemClass											*basketUndefinedGraphicsItem							;
	gItemClass											*basketUserSelectedFromPosGraphicsItem		;
	gItemClass											*noBasketGraphicsItem											;
	gItemClass											*posGraphicsItem													;
	myGraphicsTextItem			 				*graphicsItemText;

	QString													tooltipHeader;
	float														plcX;//travel
	float														plcZ;//vetical
public:
	enum
	{//type
		typeNone=0,
		typeConveyor=1,
		typeBuffer=2,
	};
	enum
	{//mode
		PLC_ON_MISSION									=0x00001,
		USER_SELECTED										=0x00002,
		PLC_FROM												=0x00004,
		PLC_TO													=0x00008,
		ENABLED_FOR_USER_TO_SELECTE_FROM=0x00010,
		SHOWN_FOR_USER_TO_SELECTE_TO		=0x00020,
		USER_CAN_DEFINE_BASKET					=0x00040,
		LOCKED_FROM_PLC									=0x00080,
		MAINTENANCE											=0x00100,
	};

Q_OBJECT
public:		
	pincerPositionClass(posClass *parrentPos_,QGraphicsScene *myScene_,float plcX_,float plcZ_,QWidget *p);
	void addMode(int m);
	void removeMode(int m);
	void checkModes();
	void setUserMaintenanseMode(bool m);
	void setAbovePosition(pincerPositionClass* p);
	void setBelowPosition(pincerPositionClass* p);
	bool getIsUserSelected();
	void updateVisual();
	void plcPositionChanged();
	int getPosId();
	int getPlcIndex();
	bool getHasSb();
	bool getHasMode(int m);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setGraphicsItem(gItemClass* g);
	void setPos(float x,float y);
signals:
	void clickedSignal(int posId);
	void changedSignal(int posId);
public slots:
	void mousePressedSlot(QGraphicsSceneMouseEvent *ev);
}; 
#endif
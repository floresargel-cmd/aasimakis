#ifndef AP_H
#define AP_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "GL/GLU.h"
class allPositionsClass;
#include "cG.h"
#include "pl.h"
#include "cr.h"
#include "stacker2Handoff.h"

class allPositionsClass:public QObject			
{
	tcpServerClass												*conveyorsPlcTcpServer;
	tcpServerClass												*newOvenConveyorsPlcTcpServer;
	QTimer														mainTimer;
	QTime														basketAfterStackerReadyTime;
	QTime														basketNeededBeforeStackerTime;
	//QTimer													inOvenUpdateTimeTimer;
	QTimer														autoModeTimer;
//	QTime														inOvenTime;
//	bool														hasBasketInOven;
	QString														timeLeftForOvenStr;

	int															currrnetOven;

	bool														plcIsBeforeStackerPosEmpty;
	bool														plcBeforeStackerPosMadeStep;
	bool														plcIsAfterStackerPosFull;
	bool														plcStackerPosMadeStep;

	bool														plcIsBeforeStacker2PosEmpty;
	Stacker2Handoff stacker2Handoff;
	bool stacker2InputStatusKnown;
	int pendingStacker2OutputBasket;
	int pendingReturn160BottomBasket;
	QString return160TrackingStatus;
	bool														plcIsAfterStacker2PosFull;
	bool														plcStacker2PosMadeStep;

	int															returningBasketFromOut;
	int															returningBasketFromOutValueChanged;
	bool														incomingBasketOn500;
	bool														scrapBasketIsReady;

	bool														takeBasketsFromStackerOut;
	bool														takeBasketsFromStacker2Out;

	//for destructor only
	openGLPartClass												*emptyNormalBasket;
	openGLPartClass												*scrapBasket;
	openGLPartClass												*profile;
	openGLPartClass												*profile24;
	openGLPartClass												*profile30;
	openGLPartClass												*emptyPos;
	//
	int															plcStatus;
	craneClass													*crane;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	QTimer														sendMissionToPlcTimer;

	basketPositionClass											*selectedFromBasketPos;
	basketPositionClass											*selectedToBasketPos;
	QVector<basketPositionClass*>								conveyorSourceMarkers;
	QVector<basketPositionClass*>								conveyorTargetMarkers;

	QVector<positionListClass*>									allPositionList;
	QVector<positionListClass*>									allActivePositionList;
	QVector<positionListClass*>									emptyPositionList;

	positionListClass											*emptyPositionList01;
	positionListClass											*emptyPositionList02;
	positionListClass											*emptyPositionList03;
	positionListClass											*emptyPositionList04;
	positionListClass											*emptyPositionList05;
																																;
	positionListClass											*emptyPositionList06;
	positionListClass											*emptyPositionList07;
	positionListClass											*emptyPositionList08;
	positionListClass											*emptyPositionList09;
	positionListClass											*emptyPositionList10;
//New Empty Buffer
	positionListClass											*emptyPositionList11;
	positionListClass											*emptyPositionList12;
	positionListClass											*emptyPositionList13;
	positionListClass											*emptyPositionList14;
	positionListClass											*emptyPositionList15;

//New aging oven
	positionListClass											*emptyPositionList16;
	positionListClass											*emptyPositionList17;
	positionListClass											*emptyPositionList18;

	QVector<positionListClass*>								buffer24PositionList;
	QVector<positionListClass*>								buffer30PositionList;
	QVector<positionListClass*>								beforeOvenPositions;
	QVector<positionListClass*>								beforeOven2Positions;
	QVector<positionListClass*>								bufferPositionList;

	positionListClass										*buffer250Position30List;
	positionListClass										*buffer270Position30List;
	positionListClass										*buffer210Position30List;
	positionListClass										*buffer220Position30List;
	positionListClass										*buffer280Position30List;

	positionListClass										*buffer240Position24List;
	positionListClass										*buffer230Position24List;
	positionListClass										*buffer200Position24List;
	positionListClass										*buffer260Position24List;
	
	positionListClass										*toDeStacker24Position;
	positionListClass										*toDeStacker30Position;
	positionListClass										*returningFromDestackersPosition;

	positionListClass										*beforeStackerPosition;
	positionListClass										*stackerPosition;
	positionListClass										*afterStackerPosition;

	positionListClass										*beforeStacker2Position;
	positionListClass										*stacker2Position;
	positionListClass										*afterStacker2Position;

	positionListClass										*basketInOutPositionList;

	// positionListClass										*scrapWorkingPosition;
	//
	positionListClass										*beforeOvenPositionList01;
	positionListClass										*beforeOvenPositionList02;
	positionListClass										*beforeOvenPositionList03;
	positionListClass										*beforeOvenPositionList04;
	positionListClass										*beforeOvenPositionList05;
	positionListClass										*beforeOvenPositionList06;

	//Added by Vahid, Before New Aging Oven
	positionListClass										*beforeOvenPositionList07;
	positionListClass										*beforeOvenPositionList08;
	positionListClass										*beforeOvenPositionList09;

	positionListClass										*ovenPositionList01;
	positionListClass										*ovenPositionList02;
	positionListClass										*ovenPositionList03;
	positionListClass										*ovenPositionList04;
	positionListClass										*ovenPositionList05;
	positionListClass										*ovenPositionList06;

	positionListClass										*afterOvenPositionList01;
	positionListClass										*afterOvenPositionList02;  // Cross transfer 2
	positionListClass										*afterOvenPositionList03;
	positionListClass										*afterOvenPositionList04;  // Cross transfer 1
	
	positionListClass										*returningConvPositionList01;
	positionListClass										*returningConvPositionList02;
	positionListClass										*returningConvPositionList03;
	positionListClass										*returningConvPositionList04;  // 160, return from oven
	positionListClass										*returningConvPositionList05;

	positionListClass										*exportPositionListThatWasLastCleared;

	positionListClass										*newOvenInPositionList01;
	positionListClass										*newOvenInPositionList02;
	positionListClass										*newOvenInPositionList03;
	positionListClass										*newOvenOutPositionList01;
	positionListClass										*newOvenOutPositionList02;
	positionListClass										*newOvenOutPositionList03;

////
public:
	int														numberOfPosToBeKeptEmptyInBuffer;
	bool													exportBeforeImports;

	int														autoModeEnum;
	enum {
		AUTO_BEFORE_STACKER									= 0x00000001,
		AUTO_AFTER_STACKER									= 0x00000002,
		AUTO_UNLOAD_OVEN									= 0x00000004,
		AUTO_UNLOAD_OVEN2									= 0x00000008,
		AUTO_EXPORTS										= 0x00000010,
		AUTO_RETURNING_EMPTY								= 0x00000020,
		AUTO_SCRAP											= 0x00000040,
		AUTO_FROM_BUFFER_TO_OVEN							= 0x00000080,
		AUTO_USE_EMPTYBASKETS_POS							= 0x00000100,
		AUTO_EMPTY_EMPTYBASKETS_POS							= 0x00000200,
		AUTO_MOVE_24F_FROM30F_TO_24F_POS					= 0x00000400,
		
		AUTO_BEFORE_STACKER2								= 0x00000800,
		AUTO_AFTER_STACKER2									= 0x00001000,

		AUTO_OVEN1_ENABLE									= 0x00002000,
		AUTO_OVEN2_ENABLE									= 0x00004000,
		AUTO_OVEN1_NOT24									= 0x00008000,
		AUTO_OVEN2_ONLY24									= 0x00010000,

		AUTO_PRESSTOOVEN_ENABLE								= 0x00020000,
	};

	int														nextOvenLoadingCycleTemperEnum;
	int														nextOven2LoadingCycleTemperEnum;
	enum {
		NEXT_OVEN_CYCLE_TEMPER_NULL	= 0,
		NEXT_OVEN_CYCLE_TEMPER_185,
		NEXT_OVEN_CYCLE_TEMPER_230,
	};

	Q_OBJECT

public:
	allPositionsClass(tcpServerClass *conveyorsPlcTcpServer_, tcpServerClass *newOvenConveyorsPlcTcpServer_);
	~allPositionsClass();

	void setPlcX(float x);
	void setPlcY(float y);
	void setPlcZ(float z);
	void draw(bool forPicking,bool forHighliting);
	void mouseOver(QColor pickedColor);
	void mouseOverHighlitingMode(QColor pickedColor);
	void mouseClicked(QColor pickedColor,bool rightClick);
	void clearSelectedPositions();
	bool hasSelectedFromBasketPos();
	basketPositionClass *getPosition(int number,int index);
	basketPositionClass *getPositionByPlcId(int plcId);

	void addToAutoMode(int mode);
	void setTakeBasketsFromStackerOut(bool s);
	void setTakeBasketsFromStacker2Out(bool s);
	void setExportBeforeImports(bool s);
	void setNumberOfPosToBeKeptEmptyInBuffer(int n);
	void removeFromAutoMode(int mode);
	void setCraneClass(craneClass *crane_);
	void plcStatusN20_13Changed(int status);

	void setNextOvenLoadingCycleTemperEnum(int t);
	int getNextOvenLoadingCycleTemperEnum();
	// Added by Vahid
	void setNextOven2LoadingCycleTemperEnum(int t);
	int getNextOven2LoadingCycleTemperEnum();

	positionListClass *getPositionListWithBasketOnFirstPos(int b);
	void moveBaskets(positionListClass* from,positionListClass* to);
	bool isOvenEntranceFull();
	//Added by Vahid
	bool isOven2EntranceFull();

	/////////////////
	int getNumberOf30feetEmptyPositionsInFullBuffer();
	positionListClass* getBasketPositionListFromFullBufferWithBasketId(int id);
	void newBasketInReturningFromDestackerPos(int returningBasketTableId);
	void conveyorsBasketMemoryChanged(int index,int bottomBasket);
	void observeReturn160Report(int bottomBasket);
	void processReturn160Report();
	void setReturn160TrackingStatus(const QString& message);
	void newOvenConveyorsBasketMemoryChanged(int index,int bottomBasket);
	void loadPositionFromDatabase(positionListClass * reloadThisList);
	void timeLeftForOvenTofinishChanged(int minutes);
	void changeConveyorsBasketMemory(int plcPosId,int basket);

	bool getPlcIsBeforeStackerPosEmpty();
	bool getPlcIsAfterStackerPosFull();
	bool getPlcIsBeforeStacker2PosEmpty();
	bool getPlcIsAfterStacker2PosFull();

	basketPositionClass* getFreePositionToRecieve(basketPositionClass* from, bool ignoreBasketsForExport);
	QVector<positionListClass*> getPosListsWithLessBaskets();
	QVector<positionListClass*> get24FPosListsWithLessBaskets();
	QVector<positionListClass*> get30FPosListsWithLessBaskets();
	basketPositionClass* getHighlightedPos();

	int getBasketTableIdOfScrapBasketPos();
	void updateAfterStackerPositionContentsFromDatabase();
	void recalculateBasketContentsFromCache();
	void reloadBasketAssignmentsFromDatabase();
	void setScrapBasketIsReady(bool r);
	//
	void setNextBasketForExport(int basketId);  // gui only
	void setNextBasketForOmavDestacker(int basketId); //gui only
	void setNextBasketForAssimakisDestacker(int basketId); //gui only
	//
	bool isPosBeforeStackerEmpty();
	positionListClass	*getBeforeStackerPosition();
	positionListClass	*getAfterStackerPosition();

	bool isPosBeforeStacker2Empty();
	positionListClass	*getBeforeStacker2Position();
	positionListClass	*getAfterStacker2Position();

	void takeScrapBasket();
	void unlockBasketAfterStacker();
	void unlockBasketReturningFromOven();

private:
	bool transferStacker2Basket(basketPositionClass* source, basketPositionClass* destination, int basketId);
	void processStacker2Handoffs();
	void beforeStacker();
	void afterStacker();
	void beforeStacker2();
	void afterStacker2();
	void bufferToOvenByPress();
	void fromOven();
	void exports();
	void returningEmpty();
	void scrapBasketOut();
	void fromBufferToOven();
	void move24FeetFrom30FeetPosTo24FPos();
	void toEmptyBuffer();					//when the normal buffer is full then take the 20 feet baskets that come from oven there
	void fromEmptyBufferTofullBuffer();		//when idle take the 20 feet baskets from empty baskets buffer to normal buffer
	//////////////////////////////////////////////////////
	// Decide to send basket to Oven 1, Oven 2 or Buffer
	// Added by Vahid
	//////////////////////////////////////////////////////
	basketPositionClass *getEmptyPositionInTheEntranceOfTheOven(basketPositionClass* fromPos);
	basketPositionClass *getEmptyPositionInTheEntranceOfTheOven2(basketPositionClass* fromPos);
	int AgingOvensDecisionMaking(basketPositionClass* fromPos);
	bool BasketCanGoToOven1(basketPositionClass* fromPos);
	bool BasketCanGoToOven2(basketPositionClass* fromPos);
	//////////////////////////////////////////////////////
	// End of code added by Vahid
	//////////////////////////////////////////////////////

	//basketPositionClass *getEmptyPositionInTheBuffer(basketPositionClass* fromPos);
	//

public slots:								
	void mainTimerSlot();
	void inOvenUpdateTimeTimerSlot();
	void autoModeTimerSlot();
	void checkOvenUnloadPositions();
	void openGlObjectChangedSlot();
	void positionSelectedSlot(basketPositionClass* b);
	private:

signals:
	void openGlChangedSignal();
	void return160TrackingStatusChanged(QString message);
};						

#endif

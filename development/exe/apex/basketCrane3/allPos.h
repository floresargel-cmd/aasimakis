#ifndef allpos_h
#define allpos_h

class columnClass;
class allPosClass;
#include "crane.h"
#include "Gb3.h"
#include "gView.h"
#include "gView.h"
#include "hmiGuiWidgetsLib.h"
#include "pos.h"
#include "serv.h"
////////////////////////////////////////////////////columnClass///////////////////////////////////////////////////////
class columnClass
{
public:
	int									posNumber=0;
	QVector<posClass*>	posV;
	columnClass();
	columnClass(int posNumber_,QVector<posClass*> posV_);
	posClass *getTopPositionWithBasket();
};
////////////////////////////////////////////////////columnClass///////////////////////////////////////////////////////
////////////////////////////////////////////////////allPosClass///////////////////////////////////////////////////////
class allPosClass:public QWidget				
{
private:
	tuxipClass															*crane2Tux=nullptr;
	tuxipClass															*crane3Tux=nullptr;
	QVector<posClass*>											posV;
	QVector<columnClass>										bufferV;
	craneClass															*crane;
	posClass 																*manualSelectedFromPos;
	int																			selectMode;	
Q_OBJECT
public:										
	enum{//selectMode
		selectForMission=0,
		selectToModify,
	};
	allPosClass(tuxipClass *crane2Tux_,tuxipClass *crane3Tux_,QGraphicsScene *layoutScene,QWidget *p=nullptr);
	void loadFromDatabase();
	void setSelectMode(int m);
	void setCraneClass(craneClass *c);
	posClass* getPosWithPosIndex(int pos,int index,bool unLockedOnly=true);
	posClass* getPosWithPlcIndex(int plcIndex,bool unLockedOnly=true);
	posClass* getPosWithBasket(int b,bool unLockedOnly=true);
	posClass* getPosInBufferToGetBasketInPos(posClass *pos);
	posClass* getPosWithBasketToMoveInAuto(int b);
	posClass* getPosWithEmptyBasketOnTop();
	QVector<posClass*> getTopPositionsWithBasket();
	void setUpDownPositionsOfColumn(int column);
	void setUpObstacleOfColumn(int column,QVector<posClass*> v);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	int moveBasket(posClass *from,posClass *to);
	void setDisplayMode(int m);
	void clearIsFromPos();
	void clearIsToPos();
	void returnDestacker1PosBasket();
	void returnDestacker2PosBasket();
	void removeBasketFromExportTable(posClass *pos);
	void updateExportEnums();
signals:
	void exportListChangedSignal();
public slots:
	void mainTimerSlot();
	void posMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*);
	void userChangedBasketSlot(posClass *pos);
	void graphicsViewKeyPressedSlot(QKeyEvent *ev);		
	void exportListChangedSlot();
};											
#endif

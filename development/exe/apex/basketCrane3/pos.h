#ifndef POS_H
#define POS_H

class posClass;
#include "Gb3.h"
#include "allPos.h"

#include "gView.h"
#include "hmiGuiWidgetsLib.h"
#include "posDlg.h"
#include "serv.h"
class posClass:public QWidget
{
protected:
	tuxipClass												*tuxip;
	QString														plcTable;
	int																posNumber;
	int																posIndex;
	int																plcDestination;
	QString														description;
	float															plcXPos;
	float															plcZPos;
	float															gXPos;
	float															gYPos;
	float															gXOffset;
	float															gYOffset;
	allPosClass												*allPos;
	QGraphicsScene										*scene;
	gItemClass												*posItem;
	gItemClass												*mouseOverItem;
	gItemClass												*basketItem;
	gItemClass												*profilesItem;
	gItemClass												*lockedItem;
	gItemClass												*fromItem;
	gItemClass												*toItem;
	QTransform												txtTransformation;
	QGraphicsSimpleTextItem 					*posItemText;
	QGraphicsSimpleTextItem						*profileTxtItem;

	posClass													*abovePos=nullptr;
	posClass													*belowPos=nullptr;
	QVector<posClass*>								obstaclePosV;

	int																basketNum;
	exportCrane3EnumClass							exportEnum=exportCrane3EnumClass::none;
	honeyCombDestinationEnumClass			honeyCombDestinationEnumOfTopProfile=honeyCombDestinationEnumClass::none;
	QVector<craneContentsDataStruct>	contentsV;
	bool															isLocked;
	bool															isFrom;
	bool															isTo;
	//plc
	bool															isCranePlcPos;
	bool															isCranePos;
	bool															isPlcReadyToGetBasket=true;
	bool															isPlcReadyToSendBasket=true;
	int																plcIndex;
	int																displayMode;
public:
	QString														debugStr;
public:
	enum{//displayMode
		displayNone=0,
		displayNormal,
		displayToModify,
		displayToSelectFrom,
		displayToSelectTo,
	};
	Q_OBJECT
public:
	posClass(allPosClass *allPos_,int posNumber_,int posIndex_,float plcXPos_,float plcZPos_,float gXOffset_,float gYOffset_,
		QGraphicsScene *scene_,gItemClass *posItem_,gItemClass *mouseOverItem_,gItemClass *basketItem_,gItemClass *profilesItem_,
		gItemClass *lockedItem_,gItemClass *fromItem_,gItemClass *toItem_,bool handleMousePressed,QWidget *p);
	QString getDescription() const;
	int getPositionNumber() const;
	int getPositionIndex() const;
	void setDisplayMode(int m);
	QString getPosIndexDisplayString() const;
	int getBasketNumber() const;
	bool getIsLocked() const;
	bool getIsPlcReadyToGetBasket();
	bool getIsPlcReadyToSendBasket();
	float getPlcX() const;
	float getPlcZ() const;
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setPlcAdress(tuxipClass *tuxip,QString plcTable_,int index_);
	int getIdForPlc() const;
	void setAboveBelowPos(posClass *a,posClass *b);
	void setObstaclePos(QVector<posClass*> obstaclePosV_);
	posClass *getAbovePos();
	posClass *getBelowPos();
	void setPlcPos(float x,float z);
	void updateTxt();
	void setBasketId(int b,bool user=false,QString logInfo="",int doNotCheckPosition=0,int doNotCheckIndex=0);
	bool getHasBasket() const; //returns true if the position is occupied with a (full or empty) basket
	bool getHasFullBasket();
	bool getHasEmptyBasket();
	void loadFromDatabase();
	void loadFromAllData(QVector<QVector<QVariant>> allPosVV,QVector<QVector<QVariant>> allPontentsVV);
	void updateFromData(QVector<QVector<QVariant>> posVV,QVector<QVector<QVariant>> contentsVV);
	void setIsFrom(bool s);
	bool getIsFrom();
	void setIsTo(bool s);
	void updateGui();
	void showPositionDlg(int activeTab,int type=-1,bool checkPass=true);
	void updateItemsVisibility();
	bool getIsFreeToMove();

	bool getCanGetBasketInPos(posClass *p);
	int getMaximumLength();
	QVector<int> getProfiles();
	craneContentsDataStruct getTopContentsDataStruct();
	craneContentsDataStruct getBottomContentsDataStruct();
	QVector<craneContentsDataStruct> getContentsDataStructs();
	bool getHasSameContentsWithPos(posClass *p);
	int getTopProfile();
	int getBottomProfile();
	int getMaximumContentsLengh();
	bool getIsProfileStacked(int p);
	void setExportEnun(exportCrane3EnumClass exportEnum_);
	exportCrane3EnumClass getExportEnun();
	QVector<posClass*> getPosThatPreventYouToMove();
signals:
	void posChangedSignal();
	void basketChangedSignal(int oldBasket,int newBasket);
	void posMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*);
	void userChangedBasketSignal(posClass *pos);
public slots:
	void showPositionDlgSlotSlot();
	void mousePressedSlot(QGraphicsSceneMouseEvent*);
	void loadFromDatabaseSlot();
};
#endif

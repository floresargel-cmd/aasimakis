#ifndef pos_h
#define pos_h
class posClass;
#include "Gb2.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "allPos.h"
#include "posDlg.h"
class posClass : public QWidget				
{
protected:
	tuxipClass												*tuxip;
	QString														plcTable;
	int																posNumber;
	int																posIndex;
	QString														description;
	float															plcXPos;
	float															plcYPos;
	float															plcZPos;
	float															gXPos;
	float															gYPos;
	float															gXOffset;
	float															gYOffset;
	allPossClass											*allPos;
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
	//
	posClass													*abovePos;
	posClass													*belowPos;
	QVector<posClass*>								obstaclePosV;
	//
	int																basketNum;
	exportCrane2EnumClass							exportEnum=exportCrane2EnumClass::none;
	honeyCombDestinationEnumClass			honeyCombDestinationEnumOfTopProfile=honeyCombDestinationEnumClass::none;
	QVector<craneContentsDataStruct>	contentsV;
	bool															isLocked;
	bool															isPlcReadyToGetBasket=true;//only uset to toRotating to get
	bool															isPlcReadyToSendBasket=true;
	bool															isFrom;
	bool															isTo;
	//plc
	bool															isCranePlcPos;//the basket number is controled by plc
	bool															isCranePos;
	int																plcIndex;
	int																displayMode;
	destinationStruct::plcValueEnum		plcDestinationEnum;
public:
	QString														debugStr;
public:
	enum{//displayMode
		displayNone=0,
		displayNormal,
		displayToModify,
		displayToExport,
		displayToSelectFrom,
		displayToSelectTo,
	};
	Q_OBJECT
public:
	posClass(allPossClass *allPos_,int posNumber_,int posIndex_,float plcXPos_,float plcYPos_,float plcZPos_,float gXOffset_,float gYOffset_,
		QGraphicsScene *scene_,gItemClass *posItem_,gItemClass *mouseOverItem_,gItemClass *basketItem_,gItemClass *profilesItem_,gItemClass *lockedItem_,gItemClass *fromItem_,gItemClass *toItem_
		,bool handleMousePressed,QWidget *p);
	~posClass();
	QString getDescription();
	int getPositionNumber();
	int getPositionIndex();
	void setDisplayMode(int m);
	QString getPosIndexDisplayString();
	int getBasketNumber();
	bool getIsLocked();
	bool getIsPlcReadyToGetBasket();
	bool getIsPlcReadyToSendBasket();
	float getPlcX();
	float getPlcY();
	float getPlcZ();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setPlcAdress(tuxipClass *tuxip,QString plcTable_,int index_);
	int getIdForPlc();
	void setAboveBelowPos(posClass *a,posClass *b);
	void setObstaclePos(QVector<posClass*> obstaclePosV_);
	void setPlcPos(float x,float y,float z);
	void updateTxt();
	void setBasketId(int b,bool user=false,QString logInfo="",int doNotCheckPosition=0,int doNotCheckIndex=0);
	bool getHasBasket();
	bool getHasBasketCommingIn();
	bool getHasFullBasket();
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
	//
	bool getCanGetBasketInPos(posClass *p);
	int getMaximumLength();
	//void setDestination(destinationStruct::plcValueEnum dEnun);
	destinationStruct::plcValueEnum getDestination();
	QVector<int> getProfiles();
	craneContentsDataStruct getTopContentsDataStruct();
	craneContentsDataStruct getBottomContentsDataStruct();
	int getTopProfile();
	int getBottomProfile();
	bool getIsProfileStacked(int p);
	honeyCombDestinationEnumClass getHoneyCombDestinationEnum();
	void setExportEnun(exportCrane2EnumClass exportEnum_);
	exportCrane2EnumClass getExportEnun();
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

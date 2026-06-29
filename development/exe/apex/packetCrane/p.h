#ifndef P_H
#define P_H

#include "pcG.h"
#include "../common/apexG.h"
class packetPositionClass;
#include "cellCont.h"
#include "gItem.h"
#include "pass.h"
class packetPositionClass:public QWidget			
{
protected:
	int															posId;
	QString													name;
	int															packetId;
	int															packetDestination;
	int															storagingTypeOfPacket;
	bool														isOnSb;
	bool														isLocked;
	int															acceptableContents;//even or odd number of woods
	int															acceptableLength;
	//
	int															packetNoOfWoods;
	int															packetLength;
	int															packetWidth;
	int															packetHeight;
	//
	QGraphicsSimpleTextItem 				*graphicsItemText;
	dxfClass												*mouseOverItem;
	dxfClass												*lockedItem;
	dxfClass												*sbItem;
	QGraphicsRectItem								*packetItem;
	QString													parentToolTip;
	bool														isSelected;
public:
	bool														hasWrongPacket;
	Q_OBJECT
public:
	packetPositionClass(int posId_,QString name_,int packetId_,bool isOnSb_,int isLocked_,int acceptableContents_,int acceptableLength_,QGraphicsScene *scene,dxfClass *mouseOverItem_,dxfClass *lockedItem_,dxfClass *sbItem_,QWidget *p);
	~packetPositionClass();
	//
	void updateTxt();
	void setGXY(float gXPos,float gYPos);
	void updateGui(QString parentToolTip_);
	void saveToDatabase();
	void loadFromDatabase();
	//void loadContentsFromDatabase();
	int getPosId();
	bool getIsLocked();
	int getPacketTypeForPlc();
	int getPositionIdForForkPlc();
	int getPositionIdForCranePlc();
	void setPacketId(int p,bool check=true);
	void updatePacketIdInPlc(tuxipClass *tuxip);
	int getPacketId(bool loadFromDataBase=true);
	int getHasPacketReadyForWarehouse(bool loadFromDataBase=true);
	int getPacketDestination();
	int getStoragingTypeOfPacket();
	int getPacketPlacingType();
	int getAcceptableContents();
	int getAcceptableLength();
	int getPacketLength();
	bool getIsOnSB();
	bool hasPacket();
	bool getIsPacketDefined();
	int getNumberOfWoods();
	void showDlg();
	void setIsSelected(bool s);
	void setZValue(int v);
signals:
	void packetDataChangedSignal(int cell);
	void cassetteChangedSignal(int cassette);
	void mouseReleasedSignal(packetPositionClass *packetPos,QGraphicsSceneMouseEvent *ev);
public slots:
	void onPosDataChangedSlot(int pos);
	void mousePressedSlot(QGraphicsSceneMouseEvent *ev);
	void cassetteChangedSlot(int cassette);
};
#endif

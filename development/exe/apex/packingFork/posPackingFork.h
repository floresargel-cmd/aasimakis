#ifndef posPackingFork_h
#define posPackingFork_h

class posPackingForkClass;
#include "Gf.h"
#include "allPosPackingFork.h"

#include "../packingFork/gItemP.h"
#include "hmiGuiWidgetsLib.h"
#include "posDlgPackingFork.h"
#include "serv.h"
class posPackingForkClass:public QWidget
{
	tuxipClass										*craneTuxip=nullptr;
	tuxipClass										*forkTuxip=nullptr;
	int														globalId=0;
	int														plcIndex=0;
	QString												description=0;
	float													plcXPos=0.;
	float													plcYPos=0.;
	float													plcZPos=0.;
	float													gXPos=0.;
	float													gYPos=0.;
	float													rotationAngleDegs=0.;
	QGraphicsScene								*scene=nullptr;
	gItemClass										*posItem=nullptr;
	gItemClass										*mouseOverItem=nullptr;
	gItemClass										*profilesItem=nullptr;
	gItemClass										*fromItem=nullptr;
	gItemClass										*toItem=nullptr;
	QGraphicsSimpleTextItem 			*posItemText=nullptr;

	bool													isForPacketId=false;
	bool													isForRackId=false;
	int														plcPacketTicket=0;
	int														plcPacketLength=0;
	int														plcPacketWoods=0;

	int														dbPacketTicket=0;
	int														dbPacketLength=0;
	int														dbPacketWoods=0;
	int														dbPacketWidth=0;
	int														dbPacketHeight=0;
	int														dbPacketWeight=0;

	bool													isFrom=false;
	bool													isTo=false;
	bool													isPlcReadyToGetPacket=true;
	bool													isPlcReadyToSendPacket=true;
public:
	Q_OBJECT
public:
	posPackingForkClass(int globalId_,tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,int plcIndex_,float plcXPos_,float plcYPos_,float plcZPos_,float angleDegs,QString description_,QGraphicsScene *scene_,gItemClass *posItem_,gItemClass *mouseOverItem_,gItemClass *profilesItem_,gItemClass *fromItem_,gItemClass *toItem_,QWidget *p);
	QString getDescription() const;
	int getPlcId() const;
	int getGlobalId() const;
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void checkPlcStatus(int value);
	void setPlcPos(float x,float y,float z);
	void updateGui();
	void loadFromDatabase();
	bool getHasTicket();
	bool getDbHasCassette();
	int getDbCassette();
	int getDbPacketTicket();
	int getPlcPacketTicket();
	bool getNeedsWrapping();
	int getPlcPacketLength();
	int getPlcPacketWoods();
	int getDbPacketWidth();
	bool getIsPacketDefined();
	bool getIsPlcReadyToGetPacket();
	bool getIsPlcReadyToSendPacket();
	void setPacketTicket(int p);
	void setIsFrom(bool s);
	bool getIsFrom();
	void setIsTo(bool s);
	void showPositionDlg();
signals:
	void posMouseReleasedSignal(posPackingForkClass*,QGraphicsSceneMouseEvent*);
	void userChangedBasketSignal(posPackingForkClass *pos);
public slots:
	void showPositionDlgSlot();
	void mousePressedSlot(QGraphicsSceneMouseEvent*);
};
#endif

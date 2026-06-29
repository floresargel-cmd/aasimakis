#ifndef POS_H
#define POS_H
class posClass;
#include "Gsw.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "posDlg.h"
class posClass : public QWidget				
{
public:
	static QGraphicsScene					*scene;
	static QTransform							txtTransformation;
	static double									textScale;
protected:
	int														posId;
	tuxipClass										*tuxip;
	int														plcIndex;
	QString												description;
	int														column;
	int														row;
	float													plcXPos;
	float													plcYPos;
	float													plcZPos;
	float													plcXCorrection;
	float													plcYCorrection;
	float													plcZCorrection;
	float													gXPos;
	float													gYPos;
	bool													isSbPosition;

	gItemClass										*mouseOveItem;
	gItemClass										*posItem;
	gItemClass										*cassetteItem;
	gItemClass										*sbBlueItem;
	gItemClass										*profilesItem;
	gItemClass										*lockedItem;
	gItemClass										*activeFromItem;
	gItemClass										*activeToItem;
	QGraphicsSimpleTextItem 			*graphicsItemText;
	//
	float													xOffset;
	float													yOffset;
	QString												posTollTipStr;
	//
	int														cassetteNum;
	int														sbNumber;
	int														ticket;
	float													ticketWeight;
	bool													isLocked;
	bool													isSelected;
	bool													isActiveFrom;
	bool													isActiveTo;
public:
	enum packetTypeEnum
	{
		none											=0,
		noPacket									=1,
		placedOnCenter						=2,
		placedOnSide							=3,
	};
public:
	Q_OBJECT								
public:
	posClass(int posId_,bool isSbPosition_,int cassetteNum_,QString description_,int column_,int row_,float plcXPos_,float plcYPos_,float plcZPos_,float xCorr_,float yCorr_,float zCorr_,bool isLocked_,int sbNumber_,int ticket_,float ticketWeight_
		,gItemClass *mouseOveItem_,gItemClass *posItem_,gItemClass *cassetteItem_,gItemClass *sbBlueItem_,gItemClass *profilesItem_,gItemClass *lockedItem_,gItemClass *activeFromItem_,gItemClass *activeToItem_,bool handleMousePressed,QWidget *p);
	~posClass();
	int getPosId();
	int getPlcIndex();
	QString getDescription();
	int getColumn();
	int getColumnWithSign();
	int getRow();
	int getCassetteNumber();
	int getTicket();
	bool getIsLocked();
	tuxipClass *getTuxIp();
	float getPlcX();
	float getPlcY();
	float getPlcZ();
	float getCorrectedPlcX();
	float getCorrectedPlcZ();
	void setPlcXCorrection(float dx);
	void setPlcYCorrection(float dy);
	void setPlcZCorrection(float dz);
	float getPlcXCorrection();
	float getPlcYCorrection();
	float getPlcZCorrection();
	void setXOffset(float x);
	void setYOffset(float y);
	void setPlcPos(float x,float y,float z);
	void updateTxt();
	void setCassetteSbTicket(int cassette_,int sbNumber_,int ticket_,QString logInfo,bool isPlcChange,bool isUserChange);
	void loadFromDatabase();
	void setIsSelected(bool s);
	void updateGui();
	void showPositionDlg(bool checkPass=true);
	bool getHasCassette();
	bool getHasSb();
	bool getHasEmptySb();
	bool getHasTicket();
	QPointF getGPos();
	void setIsActiveFrom(bool a);
	void setIsActiveTo(bool a);
	void setGraphicsZ(int z);
	void setPlcAdress(tuxipClass *tuxip_,int plcIndex_=-1);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void sendCassetteSbTicketToPlc();
	void sendCassetteSbTicketToPlc(int cassette_,int sbNumber__,int ticket_);
signals:
	void posChangedSignal();
	void posDlgClosedSignal();
	void posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*);
public slots:
	void showPositionDlgSlotSlot();		
	void mousePressedSlot(QGraphicsSceneMouseEvent*);
};											
#endif

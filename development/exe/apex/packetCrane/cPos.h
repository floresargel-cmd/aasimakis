#ifndef CPOS_H
#define CPOS_H

class convPositionClass;
class allConvPositionsClass;
class cPosDlgClass;
#ifdef stationsExe
#include "Gc.h"
#include "gText.h"
#endif
#ifdef aw1WharehouseExe
#include "../packetWarehouseMy/Gb.h"
#include "../packetWarehouseMy/gText.h"
#endif
#include "serv.h"
#include "move.h"
#include "ps.h"
#include "btn.h"
#include "cPosDlg.h"
#include "cellCont.h"
#include "psc.h"
class convPositionClass : public QWidget
{
protected:
	tuxipClass														*craneTuxip;
	tuxipClass														*forkTuxip;
	tuxipClass														*tuxip;
	QGraphicsScene												*myScene;
	allConvPositionsClass									*allConvPositions;
	//
	int																		globalId;
	QString																description;
	float																	gXPos;
	float																	gYPos;
	float																	angle;
	float																	rotateXoffset;
	float																	rotateYoffset;
	dxfClass															*mouseOverGraphicsItem;
	dxfClass															*noBasketGraphicsItem;
	dxfClass															*emptyBasketGraphicsItem;
	dxfClass															*fullBasketGraphicsItem;
	myGraphicsTextItem										*graphicsText;
	int																		currentBasketDestination;
	//
	QString																plcBasketNumberIp;
	QString																plcBasketNumberTable;
	int																		plcBasketNumberIndex;
	//
	int																		basketNumber;
	QVector<gProximitySwitchClass*>				proximitySwitchVector;
	QVector<buttonClass*>									buttonsVector;
	QVector<moveClass*>										movesVector;	//
	bool																	inMaintenanceMode;
	cPosDlgClass													*posDlg=nullptr;
	Q_OBJECT
public:
	convPositionClass(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,QGraphicsScene *myScene_,
		int globalId_,QString description_,QString plcBasketNumberIp_,QString plcBasketNumberTable_,int plcBasketNumberIndex_,
		dxfClass *mouseOverGraphicsItem,dxfClass *noBasketGraphicsItem_,dxfClass *emptyBasketGraphicsItem_,dxfClass *fullBasketGraphicsItem_,
		float dx,float dy,
		allConvPositionsClass *p);
	~convPositionClass();
	void myRotate(float rotateXoffset_,float rotateYoffset_,float angle_);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setBasketId(int b,bool user,QString logInfo);
	void addMoveClass(moveClass* m);
	void updateGui();
	int getGlobalId();
	bool getHasBasket();
	int getBasket();
	float getGX();
	float getGY();
	QString getDescription();
	void setPos(float x,float y,bool transformText=true);
	void addProximitySwitch(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset,float width=700.f,QString description="Proximity Switch");
	void addButton(QString plcIpAdress_,QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,QString plcActiveTable_,int plcActiveIndex_,int plcActiveNumber_,float offset,QString label,int type=buttonClass::typeNormal);
	void setMaintenanceMode(bool m);
public slots:
	void hoverEnterSlot();
	void hoverLeaveSlot();
	void mousePressedSlot(QGraphicsSceneMouseEvent*);
};
#endif

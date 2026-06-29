#ifndef conPos_H
#define conPos_H

class convPositionClass;
#include "serv.h"
#include "conMove.h"
#include "conPs.h"
#include "conBtn.h"
#include "posDlg.h"
#include "pos.h"
class convPositionClass : public QWidget				
{
protected:
	QGraphicsScene												*myScene;
	posClass															*parrentPos;
	//
	float																	gXPos;
	float																	gYPos;
	float																	angle;
	float																	rotateXoffset;
	float																	rotateYoffset;
	gItemClass														*mouseOver;
	gItemClass														*conveyor;
	gItemClass														*cassette;
	gItemClass														*sb;
	gItemClass														*profiles;
	myGraphicsTextItem										*posGraphicsText;
	myGraphicsTextItem										*graphicsText;
	int																		currentBasketDestination;
	//
	QVector<gProximitySwitchClass*>				proximitySwitchVector;
	QVector<buttonClass*>									buttonsVector;
	QVector<moveClass*>										movesVector;	//
	Q_OBJECT
public:
	convPositionClass(posClass *parrentPos_,QGraphicsScene *myScene_,float plcX_,float plcZ_,
		gItemClass *posTopMouseOver_,gItemClass *conveyorItem_,gItemClass *cassetteTop_,gItemClass *sbTop_,gItemClass *profileTop_,
		QWidget *p);
	~convPositionClass();
	void myRotate(float rotateXoffset_,float rotateYoffset_,float angle_);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setBasketId(int b);
	void addMoveClass(moveClass* m);
	void updateGui();
	int getGlobalId();
	bool getHasCassette();
	int getCassette();
	float getGX();
	float getGY();
	QString getDescription();
	void setPos(float x,float y,bool transformText=true);
	void addProximitySwitch(int bit,float offset,QString txt="Proximimity",QPen activePen=QPen(QColor(100,255,100),100));
	void addProximitySwitch(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset,QPen activePen);
	void addButton(QString plcIpAdress_,QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,QString plcActiveTable_,int plcActiveIndex_,int plcActiveBit_,float offset,QString label,int type=buttonClass::typeNormal);
public slots:
	void hoverEnterSlot();
	void hoverLeaveSlot();
	void mouseReleasedSlot(QGraphicsSceneMouseEvent*);
};											
#endif

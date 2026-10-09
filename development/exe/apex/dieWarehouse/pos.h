#ifndef POS_H
#define POS_H

#include "Gd.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "posDlg.h"
class posClass : public QWidget
{
protected:
	tuxipClass										*tuxip;
	int														plcId;
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
	float													xOffset;
	float													yOffset;

	QGraphicsScene								*scene;
	dxfGroupOfGraphicsItemClass		*mouseOverItem;
	dxfGroupOfGraphicsItemClass		*cellItem;
	dxfGroupOfGraphicsItemClass		*fullItem;
	dxfGroupOfGraphicsItemClass		*emptyItem;
	dxfGroupOfGraphicsItemClass		*lockedItem;
	QTransform										txtTransformation;
	QGraphicsSimpleTextItem 			*graphicsItemText;
	QGraphicsRectItem							*pcItem;
	QString												posTollTipStr;

	double												totalWeight;
	double												totalWidth;
	int														basketNum;
	bool													isLocked;
	bool													isSelected;
	int														type;//cellTypeNormal,cellTypeBig
	Q_OBJECT
public:
	posClass(int type,int plcId,int basketNum,QString description,int column,int row,float plcXPos,float plcYPos,float plcZPos,float xCorr,float yCorr,float zCorr,QGraphicsScene *scene,dxfGroupOfGraphicsItemClass *mouseOver,dxfGroupOfGraphicsItemClass *cell,dxfGroupOfGraphicsItemClass *fullBasket,dxfGroupOfGraphicsItemClass *emptyBasket,dxfGroupOfGraphicsItemClass *locked,QWidget *p);
	~posClass();
	int getPlcId();
	QString getDescription();
	int getColumn();
	int getColumnWithSign();
	int getRow();
	int getBasketNumber();
	bool getIsLocked();
	float getPlcX();
	float getPlcY();
	float getPlcZ();
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
	void setBasketId(int b,bool user=false,QString logInfo="");
	void loadFromDatabase();
	void setIsSelected(bool s);
	void updateGui();
	void showPositionDlg(int activeTab,int type=-1,bool checkPass=true);
	void setIsHighlited(bool h);
	void changeCassetteAndMoveContents();
	bool getCanTakeBasketInPos(int posId);
signals:
	void posChangedSignal();
	void basketChangedSignal(int oldBasket,int newBasket);
	void posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*);
public slots:
	void showPositionDlgSlotSlot();
	void mousePressedSlot(QGraphicsSceneMouseEvent*);
};
#endif

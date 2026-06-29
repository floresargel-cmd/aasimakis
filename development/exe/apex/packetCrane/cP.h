#ifndef CP_H
#define CP_H

#include "pcG.h"
#include "../common/apexG.h"
class cellPositionClass;
class packetPositionClass;
#include "p.h"
#include "gView.h"
class cellPositionClass:public packetPositionClass			
{
	tuxipClass											*tuxip;
protected:
	int															cassette;
	int															storagingTypeOfCassette;
	int															row;
	int															col;
	//
	QGraphicsScene									*scene;
	dxfClass												*cellItem;
	dxfClass												*cassetteItem;
	dxfClass												*fromItem=nullptr;
	dxfClass												*toItem=nullptr;

	float														xOffset;
	float														yOffset;
	float														plcXPos;
	float														plcYPos;
	float														plcZPos;
	float														gXPos;
	float														gYPos;
	//
	bool														isCasseteOnCell;
//
	int															xCorr;
	int															zCorr;
	//plc
	bool														isPlcPos;
	QString													plcIp;
	QString													plcTable;
	int															plcIndex;
	
	QGraphicsSimpleTextItem					*graphicsItemText;
	Q_OBJECT
public:
	cellPositionClass(int posId,QString name,int packetId,bool isOnSb,int cassette_,int acceptableContents,int storagingTypeOfCassette_,int isLocked,int col_,int row_,int xCorr_,int zCorr_,int acceptableLength
		,QGraphicsScene *scene_,dxfClass *cellItem_,dxfClass *mouseOverItem_,dxfClass *lockedItem_,dxfClass *cassetteItem_,dxfClass *sbItem_,dxfClass *fromItem_,dxfClass *toItem_,QWidget *p);
	~cellPositionClass();
	//
	void setPlcPos(float x,float y,float z);
	//	
	void saveToDatabase();
	void loadFromDatabase();
	//void loadContentsFromDatabase();
	void setIsFrom(bool is);
	void setIsTo(bool is);
	bool hasCassette();
	void setCassette(int cas,bool updatePlc,bool isFromPlc=false);
	int getCassette();
	int getStoragingTypeOfCassette();
	int getRow();
	int getColumn();
	void updateGui();
	void setZValue(int v);
	void setPens();
	void setPlcXCorrection(int c);
	void setPlcZCorrection(int c);
	int getXCorr();
	int getZCorr();
	float getCorrectedPlcX();
	float getCorrectedPlcZ();
	void setPlcData(tuxipClass *tuxip_,QString plcIp_,QString plcTable_,int plcIndex_);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
signals:
	void cellDataChangedSignal(int cell);
public slots:
	void onPacketDataChangedSlot(int cell);
	void cassetteChangedSlot(int cassette);
};
#endif

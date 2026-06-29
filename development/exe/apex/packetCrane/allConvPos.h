#ifndef allConvPos_h
#define allConvPos_h

#ifdef stationsExe
#include "Gc.h"
#elif packetWareHouse
#include "pcG.h"
#endif
#include "serv.h"
#include "cPos.h"
#include "move.h"
#include "gView.h"
#include "psc.h"
#include "hmiGuiWidgetsLib.h"
class allConvPositionsClass : public QWidget				
{												
	tuxipClass														*craneTuxip;
	tuxipClass														*forkTuxip;
	myQGraphicsViewClass									*layoutGraphicsView;
	QGraphicsScene												*layoutScene;
	//
	dxfClass															*cMOver;
	dxfClass															*c;
	dxfClass															*cLong;
	dxfClass															*cDouble;
	dxfClass															*cWBasket;
	dxfClass															*cWFull;
	dxfClass															*triGraphicsItem;
	//
	QVector<convPositionClass*>						positionsVector;
	QVector<moveClass*>										movesVector;
	QVector<buttonClass*>									btnsVector;
	QVector<gProximitySwitchClass*>				proxVector;
	QVector<bitClass*>										bitVector;
	QVector<hmiPushButtonWidgetClass*>		pushButtonsVector;
	bitLogVectorClass											*commandsVector;
	bitLogVectorClass											*bitErrorsVector;
	bitLogVectorClass											*bitWarningsVector;
	bool																	inMaintenanceMode;
public:
	enum conveyorsGroupTypeEnum {
		none,
		eastGroup,
		packingGroup,//new triple conveyors in packing area
	};
	conveyorsGroupTypeEnum								conveyorsGroupType;
	//
	Q_OBJECT								
public:										
	allConvPositionsClass(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,conveyorsGroupTypeEnum conveyorsGroupType_,QWidget *p);
	~allConvPositionsClass();
	convPositionClass* getPosWithId(int id);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	QString getPosDescriptionWithBasket(int b,int ignorePlcId=-1);
public slots:								
	void resetGraphicsViewZoom();
};											
#endif

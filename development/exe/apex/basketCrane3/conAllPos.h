#ifndef conAllPos_H
#define conAllPos_H

class proxItemClass;
#include "serv.h"
#include "conPos.h"
#include "conMove.h"
#include "gView.h"
#include "conProxItem.h"
class allConvPositionsClass:public QWidget
{
	tuxipClass														*tuxip;
	myQGraphicsViewClass									*layoutGraphicsView;
	QGraphicsScene												*layoutScene;
	QVector<convPositionClass*>						positionsVector;
	QVector<moveClass*>										movesVector;
	QVector<buttonClass*>									btnsVector;
	QVector<proxItemClass*>								proxVector;
public:
	Q_OBJECT
public:
	allConvPositionsClass(tuxipClass *tuxip_,QWidget *p);
	convPositionClass* getPosWithId(int globalId);
	convPositionClass* getPosWithPlcIndex(int plcId);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:
	void onMainTimerSlot();
	void resetGraphicsViewZoom();
};
#endif

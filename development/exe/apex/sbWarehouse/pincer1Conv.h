#ifndef pincer1Conv_H
#define pincer1Conv_H

class proxItemClass;
#include "../Gs.h"
#include "serv.h"
#include "conPos.h"
#include "conMove.h"
#include "gView.h"
#include "conProxItem.h"
#include "allPos.h"
class pincer1ConvPositionsClass : public QWidget				
{												
	tuxipClass														*tuxip;
	allPossClass													*allPos;
	myQGraphicsViewClass									*layoutGraphicsView;
	QGraphicsScene												*layoutScene;
	QVector<convPositionClass*>						positionsVector;
	QVector<moveClass*>										movesVector;
	QVector<buttonClass*>									btnsVector;
	QVector<conProxItemClass*>						proxVector;
	QVector<hmiPushButtonWidgetClass*>		pushButtonsVector;
	QVector<bitLogVectorClass*>						bitLogVectors;
public:
	Q_OBJECT								
public:										
	pincer1ConvPositionsClass(tuxipClass *tuxip_,allPossClass *allPos_,QWidget *p);
	~pincer1ConvPositionsClass();
	convPositionClass* getPosWithId(int id);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:								
	void onMainTimerSlot();
	void resetGraphicsViewZoom();
};											
#endif

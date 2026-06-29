#ifndef pincer2Conv_H
#define pincer2Conv_H

class proxItemClass;
#include "../Gs.h"
#include "serv.h"
#include "conPos.h"
#include "conMove.h"
#include "gView.h"
#include "conProxItem.h"
#include "allPos.h"
class pincer2ConvPositionsClass : public QWidget				
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
	pincer2ConvPositionsClass(tuxipClass *tuxip_,allPossClass *allPos_,QWidget *p);
	~pincer2ConvPositionsClass();
	convPositionClass* getPosWithId(int id);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:								
	void onMainTimerSlot();
	void resetGraphicsViewZoom();
};											
#endif

#ifndef allPosPackingFork_h
#define allPosPackingFork_h

class allPosPackingForkClass;
#include "forkPacking.h"
#include "Gf.h"

#include "../packingFork/gItemP.h"
#include "hmiGuiWidgetsLib.h"
#include "posPackingFork.h"
#include "serv.h"
class allPosPackingForkClass:public QWidget
{
	QGraphicsEllipseItem					*safeToRotateEllipseItem;
	QGraphicsPolygonItem					*rotatedLimitsPolygon;
private:
	tuxipClass										*craneTuxip=nullptr;
	tuxipClass										*forkTuxip=nullptr;
	forkPackingClass							*fork=nullptr;
	QVector<posPackingForkClass*>	posV;
	posPackingForkClass 					*manualSelectedFromPos=nullptr;
	posPackingForkClass						*inputOutputPos=nullptr;
Q_OBJECT
public:
	allPosPackingForkClass(tuxipClass *craneTuxip_,tuxipClass *forkTuxip_,QGraphicsScene *layoutScene,QWidget *p=nullptr);
	void setForkClass(forkPackingClass *f);
	posPackingForkClass *getPosWithPlcId(int plcId);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
signals:
public slots:
	void mainTimerTriggeredSlot();
	void posMouseReleasedSlot(posPackingForkClass*,QGraphicsSceneMouseEvent*);
	void graphicsViewKeyPressedSlot(QKeyEvent *ev);
};
#endif

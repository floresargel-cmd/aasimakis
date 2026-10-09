#ifndef	ALLPincer1POS_H
#define ALLPincer1POS_H
class allPincer1PosClass;
#include "pincerCol.h"
#include "pincer1.h"
#include "allPos.h"
class allPincer1PosClass : public QWidget
{
	allPossClass												*allPos;
	tuxipClass													*tuxip;
	QTimer															*userSelectedPosTimer;
	tuxipClass													*tuxipConnection;
	pincer1Class												*pincer1;
	QVector<pincerColumnsClass*>				colsVector;
	QVector<pincerPositionClass*>				allV;
	pincerPositionClass									*userSelectedFromPosition;
	int																	plcActiveFromPos;
	int																	plcActiveToPos;
	int																	missionLogFromPos;
	int																	missionLogToPos;
	bool																userMaintenanseMode;
	pincerPositionClass									*pincer2Pos;
Q_OBJECT
public:
	allPincer1PosClass(allPossClass *allPos_,tuxipClass *tuxip_,pincer1Class *pincer1_,QGraphicsScene *layoutScene,QWidget *p);
	~allPincer1PosClass();
	pincerPositionClass *getPosWithId(int p);
	void addMode(int m);
	void removeMode(int m);
	void setUserSelectedFromPos(int p);
	void setPlcOnMission(bool m);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setUserMaintenanseMode(bool m);
	bool getUserMaintenanseMode();
	pincerPositionClass* getPincer2Pos();
signals:
	void positionClickedSignal(int posId);
public slots:
	void positionClickedSlot(int posId);
	void userSelectedPosTimerSlot();
	void updateFromParentPosTimerTimeoutSlot();
};
#endif

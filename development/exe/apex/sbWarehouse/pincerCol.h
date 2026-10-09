#ifndef	POSCOL_H
#define POSCOL_H

#include "pincerPos.h"
class pincerColumnsClass : public QWidget
{
	tuxipClass											*tuxip;
	int															posIsLockedFromPlc;
	QString													columnTitle;
	QLabel													*columnLabel;
	QVector<pincerPositionClass*>		posVector;
	myGraphicsTextItem 							*graphicsItemText;
Q_OBJECT
public:
	pincerColumnsClass(tuxipClass *tuxip_,QString title_,QGraphicsScene *myScene,QPointF bottomPos,QWidget *p);
	void addPositionClass(pincerPositionClass *p);
	pincerPositionClass *getPosWithId(int p);
	QVector<pincerPositionClass*> getAllPos();
	void enablePositionsToSelectFrom();
	void enablePositionsToSelectTo();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setUserMaintenanseMode(bool m);
	void addMode(int m);
	void removeMode(int m);
	bool getHasMode(int m);
	void updateFromParentPos();
public slots:
	void positionChangedSlot(int posId);
};
#endif

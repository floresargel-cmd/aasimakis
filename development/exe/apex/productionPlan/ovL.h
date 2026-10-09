#ifndef OVL_H
#define OVL_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "ppG.h"
#include "bs.h"
#include "ppl.h"
#include "ovS.h"
class ovenLayoutClass : public QWidget
{
	int											needsToBeCalculated;
	ovenStackClass					*stack1;//front left
	ovenStackClass					*stack2;//front right
	ovenStackClass					*stack3;//rear left
	ovenStackClass					*stack4;//rear right
	ovenStackClass					*stack5;//front center
	ovenStackClass					*stack6;//rear center
	//
Q_OBJECT
public:
	ovenLayoutClass(QGraphicsScene *myScene,double xOffset,double yOffset,QPainterPath basketPath,QWidget *p);
	~ovenLayoutClass();
//
	void setNeedsToBeCalculated(int needsToBeCalculated_);
	void loadFromBasketCraneDatabase();
	bool addBasket(QVector<basketProfileGroupClass*> profileGroupsVector);
	void setMouseOverProfileProducton(profileProductionClass* p);
signals:
	void mouseEnterProfileGroupSignal(profileProductionClass*);
	void mouseLeaveProfileGroupSignal(profileProductionClass*);
	void productionSequenceChangedSignal();
public slots:
	void productionSequenceChangedSlot();
	void mouseEnterProfileGroupSlot(profileProductionClass*);
	void mouseLeaveProfileGroupSlot(profileProductionClass*);
};
#endif

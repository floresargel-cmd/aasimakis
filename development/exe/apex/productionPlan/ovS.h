#ifndef OVS_H
#define OVS_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "ppG.h"
#include "bs.h"
#include "ppl.h"

class ovenStackClass : public QWidget		
{
	int																needsToBeCalculated;
	int																numberOfBaskets;
	QPainterPath											basketPath;
	double														xOffset;
	double														yOffset;
	QGraphicsScene										*myScene;
	QVector<basketProfileGroupClass*> profileGroupsVector;
	int																canTake30Feet;
	//
Q_OBJECT								
public:										
	ovenStackClass(QGraphicsScene *myScene_,double xOffset_,double yOffset_,QPainterPath basketPath_,bool canTake30Feet_,QWidget *p);
	~ovenStackClass();
//
	void setNeedsToBeCalculated(int needsToBeCalculated_);
	bool canTakeProfilesGroup(QVector<basketProfileGroupClass*> groupsVector);
	bool addBasket(QVector<basketProfileGroupClass*> profileGroupsVector);
	void loadFromBasketCraneDatabase(int posPlcId);
	int getNumberOfBaskets();
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
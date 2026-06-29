#ifndef OVGL_H
#define OVGL_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "ppG.h"
#include "bs.h"
#include "ppl.h"
#include "ovL.h"
#include "bs.h"
class ovenGroupLayoutClass : public QDialog		
{
	dxfQGraphicsViewClass				*myQGraphicsView;
	QGraphicsScene							*myScene;
	QPainterPath								basketPath;
	QVector<ovenLayoutClass*>		ovenStacksVector;
	int													needsToBeCalculated;
	//
Q_OBJECT								
public:										
	ovenGroupLayoutClass(QWidget *p);
	~ovenGroupLayoutClass();
//
	void setNeedsToBeCalculated(int needsToBeCalculated_);
	void recalculate(QVector<basketClass*> basketsVector);
	void addBasket(QVector<basketProfileGroupClass*> profileGroupsVector);
	void clearLayout();
signals:
	void f5Signal();
	void productionSequenceChangedSignal();
public slots:
	void f5Slot();
	void productionSequenceChangedSlot();
	void mouseEnterProfileGroupSlot(profileProductionClass*);
	void mouseLeaveProfileGroupSlot(profileProductionClass*);
};																	
#endif
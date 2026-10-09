#ifndef BS_H
#define BS_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "pdG.h"
#include "dxfGV.h"
#include "dxf.h"
#include "pl.h"
#include "pp.h"
#include "pl.h"
class basketClass : public QWidget
{
	//
	QGraphicsScene							*myScene;
	dxfQGraphicsViewClass				*myQGraphicsView;
	dxfGroupOfGraphicsItemClass *basketGraphics;
	bool												needsToBeCalculated;
	double											totalWeight;
	//
	QVector<profileLayerClass*> layersVector;
Q_OBJECT
public:
	basketClass(QWidget *p=NULL);
	~basketClass();
	double getTotalHeight();
	void calculateWeight();
	void setNeedsToBeCalculated(bool needsToBeCalculated_);
	bool addEmptyLayerForProfiles(profileProductionClass* p,bool forceOnComb);
	int addProfiles(profileProductionClass*,int number);
};
#endif

#ifndef PL_H
#define PL_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "pdG.h"
#include "dxfGV.h"
#include "dxf.h"
#include "pp.h"
class profileLayerClass : public QWidget		
{
	QGraphicsScene							*myScene;
	//
	int													number;
	int												  combNumber;
	double											baseHeight;//input to constractor
	//
	profileProductionClass*			lastProfile;
	double											lastProfileDx;
	double											lastProfileDy;
	QVector<int>								profiles;
	double											totalHeight;//calculated form the profiles max height
	double											totalWidth;//calculated form the profiles layer width
	double											totalWeight;//calculated form the profiles layer weight
	QVector<bool>								spacers;
	double											flatSurfase;
	double											maxProfileHeight;
Q_OBJECT									
public:										
	profileLayerClass(int number_,int combNumber_,double baseHeight_,QGraphicsScene *myScene_);
	~profileLayerClass();
	//
	double getTotalHeight();
	bool getCanSupportLayer();
	double getTotalWeight();
	int getNumber();
	int getCombNumber();
	bool getIsOnComb();
	int addProfiles(profileProductionClass* p,int numberOfProfiles);
	void addSpacers(QVector<bool> sp);
};																	
#endif
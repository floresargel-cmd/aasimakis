#ifndef PL_H
#define PL_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "ppG.h"
#include "dxfGV.h"
#include "dxf.h"
#include "pp.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////layerProfileGroupClass////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class layerProfileGroupClass
{
	int											noOfProfiles;
	profileProductionClass* profileProduction;
	QPainterPath						path;
public:
	layerProfileGroupClass();
	layerProfileGroupClass(profileProductionClass* profileProduction_,double baseHeight);
	~layerProfileGroupClass();
	int getTableId();
	int getNumberOfProfiles();
	void addProfile(QPainterPath path);
	QPainterPath getPath();
	profileProductionClass* getProfileProduction();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////profileLayerClass////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class profileLayerClass : public QWidget
{
	QGraphicsScene										*myScene;
	//
	int																number;
	int																combNumber;
	double														baseHeight;//input to constractor
	//
	int																wordFromStackerPlc;
	QVector<bool>											bitsFromStackerPlc;
	//
	profileProductionClass*						lastProfile;
	double														lastProfileDx;
	double														lastProfileDy;
	QVector<int>											profiles;
	QVector<int>											profilesEpicsTableId;
	QVector<QColor>										profilesColors;
	QVector<int>											profilesMultiplicity;
	double														totalHeight;//calculated form the profiles max height
	double														totalWidth;//calculated form the profiles layer width
	double														totalWeight;//calculated form the profiles layer weight
	double														maxProfileLength;
	QVector<bool>											spacers;
	double														flatSurfase;
	double														maxProfileHeight;
	QVector<layerProfileGroupClass*>	profileGroupsVector;
	QVector<QGraphicsSimpleTextItem*> profileQuandityTextItemVector;
Q_OBJECT
public:
	profileLayerClass(int number_,int combNumber_,double baseHeight_,QGraphicsScene *myScene_,int basketID_);
	~profileLayerClass();
	//
	int getFirstProfile();
	//
	void addToGroup(profileProductionClass* profileProduction,QPainterPath p);
	double getTotalHeight();
	double getMaxProfileLength();
	bool getCanSupportLayer();
	double getTotalWeight();
	int getNumber();
	int getCombNumber();
	bool getIsOnCombForDb();
	bool getIsOnComb();
	int addProfiles(profileProductionClass* p,int numberOfProfiles);
	QVector<int> getProfilesEpicsTableId();
	QVector<int> getProfilesCodesTableId();
	QVector<QColor> getProfilesColors();
	QVector<int> getProfilesMultiplicity();
	bool hasProfile(int code);
	void addSpacers(QVector<bool> sp);
	int getSpacersWord();
	void setSpacerWordFromStackerPlc(int v);
	QVector<bool> getSpacers();
	QVector<layerProfileGroupClass*> getProfileGroups();
};
#endif

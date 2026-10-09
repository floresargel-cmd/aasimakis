#ifndef PPL_H
#define PPL_H

#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QProgressDialog>
#include <QFile>

#include "ppG.h"
#include "pp.h"
class profileProductionLayoutClass : public QWidget
{
	QHBoxLayout													*mainLayout;
	QScrollArea													*mainScrollArea;
	QVector<profileProductionClass*>		profileProductionVector;
	int																	needsToBeLoadedFromDatabase;
	QLabel															*moreProductionAvailable;
	//
Q_OBJECT
public:
	profileProductionLayoutClass();
	~profileProductionLayoutClass();
//
	void setNeedsToBeLoadedFromDatabase(int c,int force=0);
	bool getNeedsToBeLoadedFromDatabase();
	void loadFromDatabase();
	void clearAllProfileProduction();
	void appendProfileProduction(profileProductionClass* p);
	void moveProfileLeft();
	void moveProfileRight();
	void deleteProfile();
	profileProductionClass *getSelectedProfileProduction();
	QVector<profileProductionClass*> getProfileProductionVector();
	profileProductionClass* getFirstVisible();
	void setVisibleProfile(int profile);
signals:
	void productionChangedSignal(bool);
	void productionProfileSelectedSignal(profileProductionClass*);
public slots:
	void productionChangedSlot(bool);
	void profileProductionClassSelected(profileProductionClass *pp,bool selected);
};
#endif

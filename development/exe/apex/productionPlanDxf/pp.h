#ifndef PP_H
#define PP_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QRadioButton>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>


#include "pdG.h"
#include "dxfGV.h"
#include "dxf.h"
class profileProductionClass : public QWidget		
{
	QColor											color;
	//
	int													profileCode;
	QGraphicsScene							*myScene;
	dxfQGraphicsViewClass				*myQGraphicsView;
	dxfGroupOfGraphicsItemClass *profileGraphics;
	bool												needsToBeLoadedFromDatabase;
	QSpinBox										*lengthSpinBox;
	QCheckBox										*doubleLengthBox;
	QDoubleSpinBox							*angleSpinBox;
	QSpinBox										*interProfileSpaceSpinBox;
	QSpinBox										*multiplicitySpinBox;
	QCheckBox										*inCombCheckBox;
	QCheckBox										*sp1Box;
	QCheckBox										*sp2Box;
	QCheckBox										*sp3Box;
	QCheckBox										*sp4Box;
	QCheckBox										*sp5Box;
	QCheckBox										*sp6Box;
	QCheckBox										*sp7Box;
	QCheckBox										*sp8Box;
	//
	double											offsetToTouchTheNextProfileInTheLayer;
	bool												offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated;
Q_OBJECT								
public:										
	profileProductionClass(int profileCode_);
	~profileProductionClass();
	//
	void calculateSpacers();
	void setSpacers(bool sp1,bool sp2,bool sp3,bool sp4,bool sp5,bool sp6,bool sp7,bool sp8);
	QVector<bool> getSpacers();
	void setIsSelected(bool s);
	bool getIsSelected();
	void setProductionOrder(int po);
	int getProfileCode();
	int getProfilesEpicsTableId();
	int getProductionOrder();
	QPainterPath getProfilePath();
	void setNeedsToBeLoadedFromDatabase(bool needsToBeLoadedFromDatabase);
	void setPrivileges(int privilegeType);
	int getProductionStatus();
	int getNumberOfProfilesNotInABasket();
	int getMultiplicityOfProfiles();
	int getTemperType();
	bool getIsToBePutOnNextBasket();
	bool getIsToBePutOnNextLayer();
	double getProfileHeight();
	//double getProfileInterLayerSpaceing();
	double getProfileSpaceingInLayer();
	double getProfileWidth();
	double getProfileLength();
	int getProfileMultiplicity();
	double getProfileLinearDencity();
	double getWeightOfOneProfile();
	bool getIsOnComb();
	void calculateOffsetToTouchTheNextProfileInTheLayer();
	double getOffsetToTouchTheNextProfileInTheLayer();
	QColor getColor();
signals:
	void selectedSignal(profileProductionClass *pp,bool selected);
	void productionChangedSignal();
public slots:
	void lengthSpinBoxSlot(int v);
	void doubleLengthBoxSlot(int v);
	void angleSpinBoxSlot(double v);
	//void interLayerSpaceSpinBoxSlot(int v);
	void interProfileSpaceSpinBoxSlot(int v);
	void multiplicitySpinBoxSlot(int v);
	void inCombCheckBoxSlot(int v);
	void sp1BoxSlot(int v);
	void sp2BoxSlot(int v);
	void sp3BoxSlot(int v);
	void sp4BoxSlot(int v);
	void sp5BoxSlot(int v);
	void sp6BoxSlot(int v);
	void sp7BoxSlot(int v);
	void sp8BoxSlot(int v);
	void calculateSpacersBtnSlot(bool);
};																	
#endif
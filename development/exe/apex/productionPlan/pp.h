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


#include "ppG.h"
#include "dxfGV.h"
#include "dxf.h"
class profileProductionClass : public QWidget
{
	int													isSelected;
	//
	int													tableId;
	int													productionOrder;
	int													profileCode;
	int													inBasketsProfileQuantity;
	QColor											color;
	int													isVirtualEntryInDatabase;
	QString											soNum;
	//
	QGraphicsScene							*myScene;
	dxfQGraphicsViewClass				*myQGraphicsView;
	dxfGroupOfGraphicsItemClass *profileGraphics;
	int													needsToBeLoadedFromDatabase;
	//
	QRadioButton								*productionStatusScheduledRadioBtn;
	QRadioButton								*productionStatusOnPressRadioBtn;
	QRadioButton								*productionStatusFinishedRadioBtn;
	QCheckBox										*onNextBasketCheckBtn;
	QCheckBox										*onNextLayerCheckBtn;
	QComboBox										*temperComboBox;
	//
	QWidget											*moreWidget;
	QPushButton 								*moreToolButton;
	QSpinBox										*totalQuantitySpinBox;
	QSpinBox										*quantityInBasketsSpinBox;
	QSpinBox										*lengthSpinBox;
	QDoubleSpinBox							*linearDencitySpinBox;
	QCheckBox										*doubleLengthBox;
	QDoubleSpinBox							*angleSpinBox;
	//QSpinBox										*interLayerSpaceSpinBox;
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
	int													offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated;
Q_OBJECT
public:
	profileProductionClass(int tableId_,int isFirstInList,int productionOrder_,int profileCode_,int totalProfileQuantity_,int inBasketsProfileQuantity_,double linearDencity_,int profileLength_,int doubleLength_,int productionStaus_,int onNextBasket_,int onNextLayer_,QString temper_,QString soNum_,QString scheduleDate,int scheduleShift,bool isVirtual,
		int numberOfBilletsProduced,double billetLength,double billetButtLength,double scrapFrontLength,double scrapRearLength,double runOutLength,double dieMultiplicity,QVector<QVariant> temperDescriptionVector,QVector<QVariant> temperTypeVector);
	~profileProductionClass();
//
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	//
	void calculateSpacers();
	void setSpacers(bool sp1,bool sp2,bool sp3,bool sp4,bool sp5,bool sp6,bool sp7,bool sp8);
	QVector<bool> getSpacers();
	void setIsSelected(bool s);
	bool getIsSelected();
	void setProductionOrder(int po);
	int getProfileCode();
	int getProfilesEpicsTableId();
	int getTotalProfileMultiplicity();
	int getProductionOrder();
	QString getSoNum();
	QPainterPath getProfilePath();
	void setNeedsToBeLoadedFromDatabase(int needsToBeLoadedFromDatabase);
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
	bool getIsDoubleLength();
	QColor getColor();
	dxfGroupOfGraphicsItemClass *getProfileGraphics();
signals:
	void selectedSignal(profileProductionClass *pp,bool selected);
	void productionChangedSignal(bool needsReloadFromDatabase);
public slots:
	void productionStatusScheduledRadioBtnSlot(bool checked);
	void productionStatusOnPressRadioBtnSlot(bool checked);
	void productionStatusFinishedRadioBtnSlot(bool checked);
	void onNextBasketCheckBtnSlot(int v);
	void onNextLayerCheckBtnSlot(int v);
	void morePlusToolButtonSlot(bool);
	void temperComboBoxSlot(int index);
	void lengthSpinBoxSlot(int v);
	void linearDencitySpinBoxSlot(double v);
	void doubleLengthBoxSlot(int v);
	void totalQuantitySpinBoxSlot(int v);
	void quantityInBasketsSpinBoxSlot(int v);
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

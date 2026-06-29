#ifndef pincer2_H
#define pincer2_H
class pincer2Class;
#include "Gsw.h"
#include "hmiGuiWidgetsLib.h"
#include "pincer2AllPos.h"
#include "allPos.h"
class pincer2Class : public QWidget
{
	allPossClass													*allPos;
	tuxipClass														*tuxip;
	allPincer2PosClass										*allPincer2Positions;
	bool																	readyForNewMission;
	pincerPositionClass										*myPos;

	gItemClass														*craneXItem;
	gItemClass														*craneZItem;
	//
	myQGraphicsViewClass									*layoutGraphicsView;
	QPushButton														*maintenanceBtn;
	QVector<hmiPushButtonWidgetClass*>		pushButtonsVector;
	bitLogVectorClass											*missionStepsVector;
	bitLogVectorClass											*horizontalPosVector;
	bitLogVectorClass											*bitErrorsVector;
	bitLogVectorClass											*bitWarningsVector;
	QVector<progressClass*>								progressVector;
	QVector<intPlcMemoryClass*>						intPlcMemoryVector;
	QVector<floatPlcMemoryClass*>					floatPlcMemoryVector;
	QVector<bitClass*>										bitVector;
	QVector<activeIconClass*>							activeIconsVector;
	plotClass															*verticalVelocityPlot;

	bitLogVectorClass											*stepsLogVector;
	bitLogVectorClass											*horizontalPositionsLogVector;
	//
	int																		pcStatus;
	int																		plcStatus;
	int																		plcFrom;
	int																		plcTo;
	int																		plcCraneBasket;
	float																	plcX;
	float																	plcZ;
	//
	QCheckBox															*importsCheckBtn;
	QCheckBox															*exportsCheckBtn;
	Q_OBJECT								
public:										
	pincer2Class(allPossClass *allPos_,tuxipClass *tuxip_,QWidget *p);
	~pincer2Class();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void newMission(pincerPositionClass *from,pincerPositionClass *to);
	bool getIsIdle();
public slots:			
	void stopSlot();
	void checkForNewMissionsTimerSlot();
	void resetGraphicsViewZoom();
	void maintenanceBtnClickedSlot();
	//
	void importsCheckBtnStateChangedSlot(int state);
	void exportsCheckBtnStateChangedSlot(int state);
};											
#endif
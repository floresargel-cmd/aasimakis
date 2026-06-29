#ifndef pincer1_H
#define pincer1_H
class pincer1Class;
#include "Gsw.h"
#include "hmiGuiWidgetsLib.h"
#include "pincer1AllPos.h"
#include "allPos.h"
class pincer1Class : public QWidget
{
	allPossClass													*allPos;
	tuxipClass														*tuxip;
	allPincer1PosClass										*allPincer1Positions;
	bool																	readyForNewMission;
	pincerPositionClass										*myPos;

	gItemClass														*craneXItem;
	gItemClass														*craneYItem;
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
	QVector<plotClass*>										plotsVector;
	bitLogVectorClass											*stepsLogVector;
	bitLogVectorClass											*horizontalPositionsLogVector;
	hmiPushButtonWidgetClass							*abordActiveMission;
	hmiPushButtonWidgetClass							*endActiveMission;
	bitLogVectorClass											*bitPositionsVector;
	//
	int																		pcStatus;
	int																		plcStatus;
	int																		plcFrom;
	int																		plcTo;
	int																		plcCraneBasket;
	float																	plcX;
	float																	plcY;
	//
	Q_OBJECT								
public:										
	pincer1Class(allPossClass *allPos_,tuxipClass *tuxip_,QWidget *p);
	~pincer1Class();
	void fillAxisGroup(QVBoxLayout *layout,QString title,int index);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void newMission(pincerPositionClass *from,pincerPositionClass *to);
	bool getIsIdle();
public slots:								
	void stopSlot();
	void resetGraphicsViewZoom();
	void maintenanceBtnClickedSlot();
};											
#endif
#ifndef HMIDLG_H
#define HMIDLG_H

class craneClass;
class craneConveyorWidgetClass;
#include "gView.h"
#include "Gd.h"
#include "serv.h"
#include "proxItem.h"
#include "crConvW.h"
#include "hmiSett.h"
#include "crane.h"
#include "crConv.h"
#include "dataLoging.h"
class hmiDlgClass : public QDialog
{
	tuxipClass													*tuxip;
	craneClass													*crane;
	dxfGroupOfGraphicsItemClass					*hooksItem;
	QVector<progressClass*>							progressVector;
	QVector<floatPlcMemoryClass*>				floatPlcMemoryVector;
	QVector<intPlcMemoryClass*>					intPlcMemoryVector;
	QVector<bitClass*>									bitVector;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
	QVector<proxItemClass*>							proxVector;
	QLabel															*craneStep;
	QLabel															*hooksStep;
	bitLogVectorClass										*bitErrorsVector;
	bitLogVectorClass										*bitWarningsVector;
	QVector<plotClass*>									plotsVector;
	hmiSettingsClass										*settings;
	craneConveyorClass									*craneConveyor;
	dataLogingClass											*dataLoging;
	//lock/unlock
	QTabWidget													*missionsTabWidget;
	bool																areAdvancedButtonsLocked;
	QPushButton													*clearActiveMission;
	hmiPushButtonWidgetClass						*abordActiveMission;
	hmiPushButtonWidgetClass						*endActiveMission;
	Q_OBJECT
public:
	hmiDlgClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip_,bool useTranslation,craneClass *crane_=NULL);
	~hmiDlgClass();
	void setCraneConveyorClass(craneConveyorClass *c);
	int exec();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void lockAdvancedBtns(bool l);
public slots:
	void clearNextMissionBtnClickedSlot();
	void clearActiveMissionBtnClickedSlot();
	void settingsBtnClickedSlot();
	void cameraMissionsTabWidgetTabBarDoubleClickedSlot(int);
	//
	void stopSlot();
};
#endif

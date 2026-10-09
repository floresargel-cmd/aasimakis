#ifndef AUTOWDG_H
#define AUTOWDG_H

#include "Gd.h"
#include "crane.h"
#include "expWdg.h"
class autoWidgetClass : public QWidget
{
	craneClass									*crane;
	QCheckBox										*exportsCheckBtn;
	QLabel											*basketLabel;
	basketContentsClass					*basketContents;
	exportStationWidgetClass		*exportStationWidget;
	QPushButton									*returnBtn;
	workStationClass						*workStation;
	tuxipClass									*tuxip;
	bool												isBasketContentOk;
	int													basketOnWork;
	Q_OBJECT
public:
	autoWidgetClass(craneClass *crane_,workStationClass	*workStation_,tuxipClass *tuxip_,QWidget *p=NULL);
	~autoWidgetClass();
	void enableReturnButton();
public slots:
	void exportsCheckBtnStateChangedSlot(int state);
	void saveAndReturnBtnClickedSlot();
	void basketChangedSlot(int basketOnWork);
	void basketContentsStateChangedSlot(bool isOk);
};
#endif

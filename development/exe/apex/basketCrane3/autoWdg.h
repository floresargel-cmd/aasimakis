#ifndef AUTOWDG_H
#define AUTOWDG_H

#include "Gb3.h"
#include "crane.h"
// autoWidgetClass provides GUI components to manage automatic missions of crane 3
class autoWidgetClass : public QWidget
{
private:
	craneClass									*crane;
	QCheckBox										*destackerWorking1CheckBtn;
	QCheckBox										*destackerWorking2CheckBtn;
	QCheckBox										*crane2CheckBtn;
	QCheckBox										*returnEmptyCheckBtn;
	Q_OBJECT								
public:										
	autoWidgetClass(craneClass *crane_,QWidget *p=NULL);
public slots:
	void destackerWorking1CheckBtnStateChangedSlot(int state);
	void destackerWorking2CheckBtnStateChangedSlot(int state);
	void crane2CheckBtnStateChangedSlot();
	void returnEmptyCheckBtnStateChangedSlot();
};
#endif

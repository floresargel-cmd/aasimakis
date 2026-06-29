#ifndef EXPD_H
#define EXPD_H

#include "pcG.h"
#include "../common/apexG.h"
#include "../common/btns.h"
#include "expW.h"

class exportsDlgClass : public QDialog	
{
	exportsWidgetClass						*exports;
	exportsWidgetClass						*exportsStation1;
	exportsWidgetClass						*exportsStation2;
	exportsWidgetClass						*exportsPacking;
Q_OBJECT
public:		
	exportsDlgClass(QWidget *p=NULL);
	int exec();
	void updateGui();
public slots:
	void mainTabWidgetCurrentChangedSlot(int index);
};																	
#endif
#ifndef AUTOWDG_H
#define AUTOWDG_H
#include "Gsw.h"
#include "crane.h"
class autoWidgetClass : public QWidget
{
	tuxipServerClass						*tuxipServer;
	craneClass									*crane;
	QCheckBox										*autoWestImportsCheckBtn;
	QCheckBox										*autoWestExportsEmptySbCheckBtn;
	QCheckBox										*autoWestExportsFullSbCheckBtn;
	QCheckBox										*autoWestExportsEmptyCassettesCheckBtn;

	QCheckBox										*tripleImportsCheckBtn;
	QCheckBox										*tripleExportsWestCheckBtn;
	QCheckBox										*tripleExportsEastCheckBtn;

	QCheckBox										*doubleImportsCheckBtn;
	QCheckBox										*doubleExportEmptyCassettesCheckBtn;
	QCheckBox										*doubleExportFullSBCheckBtn;
	Q_OBJECT
public:
	autoWidgetClass(tuxipServerClass *tuxipServer_,craneClass *crane_,QWidget *p=NULL);
	~autoWidgetClass();
public slots:
	void autoWestImportsCheckBtnSlot(int state);
	void autoWestExportsEmptySbCheckBtnSlot(int state);
	void autoWestExportsFullSbCheckBtnSlot(int state);
	void autoWestExportsEmptyCassettesCheckBtnSlot(int state);

	void tripleImportsCheckBtnSlot(int state);
	void tripleExportsWestCheckBtnSlot(int state);
	void tripleExportsEastCheckBtnSlot(int state);

	void doubleImportsCheckBtnSlot(int state);
	void doubleExportEmptyCassettesCheckBtnSlot(int state);
	void doubleExportFullSBCheckBtnSlot(int state);
};
#endif

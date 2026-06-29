#ifndef CRANEWDG_H
#define CRANEWDG_H
#include "Gd.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
class hmiDlgClass;
class craneWidgetClass;
class craneConveyorClass;
#include "crane.h"
#include "hmiDlg.h"
class craneWidgetClass : public QWidget
{
	tuxipClass														*tuxip;
	QListWidget														*logGuiList;
	QString																logGuiName;
	QVector<activeIconClass*>							activeIconsVector;
	QLabel																*xPosLabel;
	QLabel																*zPosLabel;
	QLabel																*xPosTargetLabel;
	QLabel																*zPosTargetLabel;
	hmiDlgClass														*hmiDlg;

	QComboBox															*diePressDiameterCombo;
	QComboBox															*diePressTypeCombo;

	Q_OBJECT								
public:										
	craneWidgetClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip_,QWidget *conv,bool useTranslation,craneClass *crane=NULL);
	~craneWidgetClass();
	void setCraneConveyorClass(craneConveyorClass *c);
	void loadFromDataBase();
	void addMessageToGui(QString mess,QString details,QString type);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:
	void loadHmiDlgSlot();
	void popUpList();						
	void diePressAssemblyBtnClickedSlot();
	void diePressDisassemblyBtnSlot();
};											
#endif

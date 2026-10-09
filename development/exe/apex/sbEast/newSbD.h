#ifndef newSbD_H
#define newSbD_H
#include "GsE.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
class newSbPosClass:public QWidget
{
	tuxipClass			*tuxip;
	int							plcId;
	int							globalPosId;
	QSpinBox				*sbSpin;
	QSpinBox				*ticketSpin;
	QLabel					*checkLabel;
	QPushButton			*saveBtn;
	Q_OBJECT
public:
	newSbPosClass(tuxipClass *tuxip_,int plcId_);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void checkGui();
public slots:
	void saveBtnClickedSlot();
	void sbSpinBoxValueChangedSlot(int val);
	void ticketSpinBoxValueChangedSlot(int val);
};
class newSbDlgClass : public QDialog
{
	QVector<newSbPosClass*>								newSbPosV;
	QVector<hmiPushButtonWidgetClass*>		pushButtonsVector;
	Q_OBJECT
public:
	newSbDlgClass(tuxipClass *tuxip,QWidget *p);
	~newSbDlgClass();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
};
#endif

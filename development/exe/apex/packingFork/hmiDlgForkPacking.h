#ifndef hmiDlgForkPacking_h
#define hmiDlgForkPacking_h

class hmiDlgForkPackingClass;
#include "gView.h"
#include "Gf.h"
#include "serv.h"
#include "hmiSettingsPackingFork.h"
#include "forkPacking.h"
class hmiDlgForkPackingClass:public QDialog
{
	tuxipClass													*tuxip;
	forkPackingClass										*fork;
	QVector<progressClass*>							progressVector;
	QVector<floatPlcMemoryClass*>				floatPlcMemoryVector;
	QVector<intPlcMemoryClass*>					intPlcMemoryVector;
	QVector<bitClass*>									bitVector;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
	QLabel															*craneStepLabel;
	QLabel															*hooksStepLabel;
	bitLogVectorClass										*bitErrorsVector;
	bitLogVectorClass										*bitWarningsVector;
	bitLogVectorClass										*bitPositionsVector;
	bitLogVectorClass										*bitSignalsFromVector;
	bitLogVectorClass										*bitSignalsToVector;
	QVector<plotClass*>									plotsVector;
	QVector<bitLogVectorClass*>					bitLogVectorV;
	hmiSettingsPackingForkClass					*settings;
	Q_OBJECT
public:
	hmiDlgForkPackingClass(tuxipClass *tuxip_,forkPackingClass *fork_);
	~hmiDlgForkPackingClass();
	int exec();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void fillAxisGroup(QVBoxLayout *layout,QString title,int index);
public slots:
	void settingsBtnClickedSlot();
	void stopSlot();
};
#endif

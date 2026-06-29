#ifndef hmiSettingsPackingFork_h
#define hmiSettingsPackingFork_h

#include "Gf.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
class hmiSettingsPackingForkClass:public QDialog
{
	tuxipClass													*tuxip;
	QVector<floatPlcMemoryClass*>				floatPlcMemoryVector;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
public:
	Q_OBJECT
public:
	hmiSettingsPackingForkClass(tuxipClass *tux,QWidget *p);
	int exec();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
private slots:
	void reject();
	void accept();
};
#endif

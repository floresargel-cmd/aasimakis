#ifndef ISETT_H
#define ISETT_H
class hmiSettingsClass;
#include "Gsw.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "allPos.h"
class hmiSettingsClass: public QDialog
{
	tuxipClass													*tuxip;
	QVector<intPlcMemoryClass*>					intPlcMemoryVector;
	QVector<floatPlcMemoryClass*>				floatPlcMemoryVector;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
	QLabel															*correcitonsLabel;
public:
	Q_OBJECT
public:
	hmiSettingsClass(tuxipClass *tux,QWidget *p);
	int exec();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void updateCorrections();
private slots:
	void reject();
	void accept();
	void applyCorrectionsBtnClickedSlot();
};
#endif

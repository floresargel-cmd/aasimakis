#ifndef HMIB_H
#define HMIB_H

#include "gView.h"
#include "Gb3.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
class hmiDlgBasicClass : public QDialog
{
	tuxipClass													*tuxip;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
	bitLogVectorClass										*bitWarningsVector;
	bitLogVectorClass										*bitErrorsVector;
	Q_OBJECT
public:
	hmiDlgBasicClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip_,QWidget *p);
	~hmiDlgBasicClass();
	int exec();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
};
#endif

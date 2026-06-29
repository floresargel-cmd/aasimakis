#ifndef HMIB_H
#define HMIB_H

#include "gView.h"
#include "Gd.h"
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
	hmiDlgBasicClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip_,bool useTranslation,QWidget *p);
	~hmiDlgBasicClass();
	int exec();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
};											
#endif

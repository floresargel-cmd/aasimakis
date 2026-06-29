#ifndef HMIB_H
#define HMIB_H

#include "gView.h"
#include "Gsw.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
class hmiDlgBasicClass : public QDialog				
{												
	tuxipClass													*sbTuxip;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
	bitLogVectorClass										*bitWarningsVector;
	bitLogVectorClass										*bitErrorsVector;
	Q_OBJECT								
public:										
	hmiDlgBasicClass(tuxipClass *sbTuxip_,QWidget *p);
	~hmiDlgBasicClass();
	int exec();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
};											
#endif

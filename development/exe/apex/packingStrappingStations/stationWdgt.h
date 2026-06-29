#ifndef stationWdgt_h
#define stationWdgt_h

class stationWdgtClass;
#include "Gstr.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "../packingFork/posPackingFork.h"
class stationWdgtClass:public QWidget				
{												
	tuxipClass													*oldStrappingTux=nullptr;
	tuxipClass													*forkTux=nullptr;
	posPackingForkClass									*posOut=nullptr;
	QVector<floatPlcMemoryClass*>				floatPlcMemoryVector;
	QVector<intPlcMemoryClass*>					intPlcMemoryVector;
	QVector<bitClass*>									bitVector;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
	QVector<bitLogVectorClass*>					bitLogVector;
	Q_OBJECT
public:
public:
	stationWdgtClass(tuxipClass *oldStrappingTux_,tuxipClass *forkTux_,QWidget *p);
	~stationWdgtClass();
	QString getName();
	void definePacketInExit();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:								
	void placeHolder();
};											
#endif

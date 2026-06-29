#ifndef rollersDlg_h
#define rollersDlg_h

class rollersDlgClass;
#include "gView.h"
#include "Gf.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gProxItem.h"
#include "gBtnItem.h"
class rollersDlgClass:public QDialog				
{												
	tuxipClass													*tuxip;
	myQGraphicsViewClass								*layoutGraphicsView;
	QGraphicsScene											*layoutScene;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
	QVector<gProximitySwitchClass*>			gProximitySwitchVector;
	QVector<gBtnItemClass*>							gBtnItemVector;
	QVector<bitLogVectorClass*>					bitLogVectorV;
	Q_OBJECT								
public:										
	rollersDlgClass(tuxipClass *tuxip_,QWidget *p);
	~rollersDlgClass();
	int exec();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:								
	void resetGraphicsViewZoom();
};											
#endif

#ifndef MW_H
#define MW_H

#include "Gs.h"
#include "../packetCrane/expD.h"
#include "../packetCrane/pass.h"
#include "../packetCrane/allConvPos.h"
#include "scaleD.h"
#include "stEastW.h"
#include "stWestW.h"
class mainWindowClass : public QMainWindow
{
	tuxipServerClass										*tuxipServer=nullptr;
	QAction															*showTuxDlgAction=nullptr;
	exportsDlgClass											*exportsDlg=nullptr;
	stationEastWidgetClass							*stationEastWidget=nullptr;
	stationWestWidgetClass							*stationWestWidget=nullptr;
	allConvPositionsClass								*allConvPositions=nullptr;
	QVector<bitClass*>									bitVector;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
	scaleDlgClass												*scaleDlg=nullptr;

Q_OBJECT								
public:										
		mainWindowClass();
		~mainWindowClass();
public slots:
	void mainTimerSlot();
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();									
	void qtAboutSlot();
	void showScaleDlgTriggeredSlot();
};																	
#endif
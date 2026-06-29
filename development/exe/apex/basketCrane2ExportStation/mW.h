#ifndef mW_h
#define mW_h

#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "basketCrane.h"
#include "sbWarehouse.h"
class mainWindowClass : public QMainWindow				
{												
	tuxipServerClass											*tuxipServer;
	tuxipClass														*basketTuxipConnection;
	QAction																*showTuxServerAction;
	basketCraneClass											*basketCrane;
	sbWarehouseClass											*sbWarehouse;
	handShakeClass												*handShakeCrane2;
	bool																	handShakeCrane2CommsOk;
	Q_OBJECT								
public:										
	mainWindowClass();
	~mainWindowClass();
private:									
public slots:								
	void mainTimerSlot();						
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();						
	void qtAboutSlot();
	void handShakeCrane2ChangedSlot(bool s);
};											
#endif

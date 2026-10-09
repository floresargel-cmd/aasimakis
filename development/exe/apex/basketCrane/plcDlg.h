#ifndef PLCDLG_H
#define PLCDLG_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>

#include "cG.h"
class plcDialogClass;
class tcpServerClass;
#include "plcW.h"
#include "plcM.h"
class plcDialogClass : public QWidget
{
	plcWidgetClass				*plcWidget;
#ifdef TUX_PLC_COMMS
	tuxipServerClass			*tuxipServer;
#endif
#ifdef TUX_PLC_COMMS
	tuxipClass            *tuxIpCrane;
	tuxipClass            *tuxIpConveyors;
	tuxipClass            *tuxIpNewOven;
#endif
	tcpServerClass				*cranePlcTcpServer;
	tcpServerClass				*conveyorsPlcTcpServer;
	plcManualClass				*plcManual;
	Q_OBJECT
public:
  plcDialogClass(plcWidgetClass *plcWidget_);
  ~plcDialogClass();
#ifdef TUX_PLC_COMMS
	tuxipServerClass *getTuxServer();
	tuxipClass *getCrainTuxClass();
	tuxipClass *getConveyorTuxClass();
	tuxipClass *getNewOvenTuxClass();
	void showTuxServerCommsDialog();
	void addTables();
#endif
	tcpServerClass* getCranePlcTcpServer();
	tcpServerClass* getConveyorsPlcTcpServer();
	void startExternalSoft();
	void plcStatusChanged(int index,int value);
	void plcFloatValuesChanged(int index,float value);
};
#endif

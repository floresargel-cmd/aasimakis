#ifndef MW_H
#define MW_H

#include <QtGui>
#include <QMainWindow>
#include <QSplitter>

#include "aG.h"
#include "a.h"
#include "co.h"
#include "ac.h"
#include "fl.h"

class mainWindowClass : public QWidget				
{												
	QTimer								mainTimer;
#ifdef TUX_PLC_COMMS
	tuxipServerClass			*tuxipServer;
#endif
	tcpServerClass				*tcpServer;
	tcpServerClass				*newOvenTcpServer;
	//
	areaClass							*area;
	commandsGroupClass		*commandsGroup;
	autoCircleGroupClass	*autoCircleGroup;
	faultsListsClass			*faultsLists;
	QLabel								*autoUnloadOvenLabel;
	Q_OBJECT								
public:										
	mainWindowClass();							
	~mainWindowClass();							
public slots:
//for backwards compatibility
	void signalFromTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void signalFromNewOvenTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();
//for backwards compatibility
	void onMainTimer();
	void signalFromPlcSlot(QString ip,QString table,int index,QString value);
	void qtAboutSlot();
};											
#endif

#ifndef EMULATEMW_H
#define EMULATEMW_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include "cG.h"
#include "emulate.G.h"
#include "emulate.cr.h"
#include "emulate.conv.h"
class plcEmulateWindowClass : public QDialog				
{												
	QTimer									mainTimer;
	tuxipClass							*craneTuxip;
	tuxipClass							*conveyorTuxip;
	craneEmulatorClass			*craneEmulator;
	conveyorsEmulatorClass	*conveyorsEmulator;
    Q_OBJECT								
public:										
	plcEmulateWindowClass(tuxipClass *craneTuxip_,tuxipClass *conveyorTuxip_,QWidget *p);
private:									
	void initializeDatabase();
	void demoFillTables();
	void createTable(QString tableName,QStringList colNames,QStringList colTypes);
public:										
	void tuxIsReady();
	void cranePlcDataChanged(QString table,int index,QString value);
	void conveyorPlcDataChanged(QString table,int index,QString value);
public slots:								
	void onMainTimer();
};											
#endif

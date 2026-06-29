#ifndef A_H
#define A_H

#include <QtGui>
#include <QMainWindow>
#include <QGroupBox>
#include <QScrollArea>
#include "aG.h"
#include "mem.h"
#include "ph.h"
#include "ps.h"
#include "cm.h"
#include "mo.h"
#include "tf.h"
class areaClass : public QScrollArea 	
{
//george2018.11.20
	tcpServerClass									*oldOvenTcpServer;
	tcpServerClass									*newOvenTcpServer;
//george2018.11.20
	QVector<plcMemoryClass*>				plcMemoryVector;
	QVector<photoSensorClass*>			photoSensorsVector;
	QVector<proximitySwitchClass*>	proximitySwitchVector;
 	QVector<conveyorMovmentClass*>	conveyorMovmentVector;
 	QVector<moveConveyorClass*>			moveConveyorVector;
 	QVector<thirtyFeetClass*>				thirtyFeetVector;
	QLabel													*crossTransferPosLabel;
	QLabel													*ovenTimeLeftLabel;
 Q_OBJECT								
public:									
	areaClass(tcpServerClass *oldOvenTcpServer_,tcpServerClass *newOvenTcpServer_,QWidget *p);
	//
	void plcMemoryChanged(QString ip,QString table,int index,int value);
	void plcStatusChanged(int index,int value);
	void hideButtons();
	void showButtons();
	void setCrossTransferPos(int p);

	void saveOvenLoadBasketDataToDatabase(int ovenId,int position,int positionIndex);
	void saveOvenLoadDataToDatabase(int ovenNumber);
	void createOvenExcelReport(int ovenId,bool show);
public slots:								
	void resetCrossTransferPosSlot();
	void createOvenLoadExcelReportSlot();
};											
#endif

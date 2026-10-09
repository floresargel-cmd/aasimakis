#ifndef MPOS_H
#define MPOS_H

#include <QtGui>
#include <QMainWindow>
#include <QList>
#include <QInputDialog>
#include <QLineEdit>
class positionClass;
#include "pos.h"
#include "../common/apexG.h"
class mainPositionClass : public QPushButton
{
	tcpServerClass						*tcpServer;
	QString										title;
	int												plcPosId;//100,110,120....
	positionClass							*selectedFromPosition;
	mainPositionResetClass		*mainPositionReset;
	bool											inMaintenanceMode;
public:
	//
	QVector<positionClass*>		positionsVector;
	Q_OBJECT
public:
	mainPositionClass(tcpServerClass *tcpServer_,QString title_,int plcPosId_,QVector<int>	plcIndexVector,QVector<int>	plcBitVector,QVector<int>	commandsVector,QWidget *p=NULL);
	void setSelectedFromPosition(positionClass *selectedFromPosition_);
	void setPlcStatusForNoEmptyFullToTrue(int plcIndex,int plcBitIndex);
	int getNumberOfSBs();
	int getNumberOfEmptyPos();
	void setInMaintenanceMode(bool m);
	bool defineContents();
private:
	void mouseReleaseEvent(QMouseEvent *e);
public:
	void plcStatusChanged(int index,bool value);
signals:
	void clickedSignal(positionClass*);

};
#endif

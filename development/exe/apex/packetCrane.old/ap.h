#ifndef AP_H
#define AP_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "GL/GLU.h"
class allPosisionsClass;
class craneClass;

#include "pcG.h"
#include "../common/apexG.h"
#include "plc.h"
#include "convP.h"
#include "op.h"
class plcClass;
//#include "cr.h"
class allPosisionsClass:public QWidget			
{
	tcpServerClass														*cranePlcTcpServer;
	QTimer																		mainTimer;
	QTimer																		sendMissionToPlcTimer;
	QVector<cellPositionClass*>								allPositionList;
	conveyorPositionClass											*position24;//west conveyor
	conveyorPositionClass											*position25;//east conveyor
	cellPositionClass													*positionCrane;//crane
	//for ~
	openGLPartClass														*cassetteOGL;
	openGLPartClass														*posSelectionOGL;
	openGLPartClass														*packetCenteredOGL;
	openGLPartClass														*packetLeftOGL;
	openGLPartClass														*packetRightOGL;
	openGLPartClass														*for500x500PlateOGL;
public:
	Q_OBJECT
public:
	allPosisionsClass(QWidget *p);
	~allPosisionsClass();
	void setPlcClass(plcClass *plc);
	cellPositionClass *getCellPosWithPosId(int id);
	cellPositionClass *getEmptyCassetteOnRow(int row,int packetType,int storagingType,QString cellSignFilter);
	cellPositionClass *getEmptyCassetteOnRow(int row,int packetType);//ignore storagingType
	cellPositionClass *getEmptyCassetteForPacketIn(packetPositionClass *p,bool secondTry=false);
	//
	void setConveyorPosition24Class(conveyorPositionClass *c24);
	void setConveyorPosition25Class(conveyorPositionClass *c25);
	void setPositionCraneClass(cellPositionClass *pcr);
	void draw(bool forPicking);
	void mouseOver(QColor pickedColor);
	void mouseClicked(QColor pickedColor);
	QVector<cellPositionClass*> getAllCells();
	//
public slots:								
	void mainTimerSlot();
	void openGlObjectChangedSlot();
	void onCellDataChangedSlot(int cell);
	private:
signals:
	void openGlChangedSignal();
};						

#endif

#ifndef OW_H
#define OW_H


#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>

#include "cG.h"
#include "op.h"
#include "ap.h"
#include "cr.h"

class openGLWidgetClass : public QGLWidget				
{
	tcpServerClass				*cranePlcTcpServer;
	tcpServerClass				*conveyorsPlcTcpServer;
	tcpServerClass				*newOvenConveyorsPlcTcpServer;

	GLfloat						xTranslate;
	GLfloat						yTranslate;

	GLfloat						zoom;
	GLfloat						zoomMin;
	GLfloat						zoomMax;

	GLfloat						zRotate;
	GLfloat						zRotMin;
	GLfloat						zRotMax;

	GLfloat						elevate;
	GLfloat						elevateMin;
	GLfloat						elevateMax;

	QPoint						lastMousePos;
	QPoint						midleMousePos;
	//
	openGLPartClass				*area;
	craneClass					*crane;
	allPositionsClass			*allPositions;
	basketPositionClass			*highlightedPosition;
	QString						toolTipText;

	Q_OBJECT

public:
	openGLWidgetClass(tcpServerClass *cranePlcTcpServer_,tcpServerClass *conveyorsPlcTcpServer_,tcpServerClass *newOvenConveyorsPlcTcpServer_,QWidget *parent=0,QGLWidget *shareWidget=0,GLfloat zoomMin_=0.1,GLfloat zoomMax_=30.0,GLfloat zRotMin_=-180.,GLfloat zRotMax_=180.,GLfloat elevateMin_=0.,GLfloat elevateMax_=90.);
	~openGLWidgetClass();
	//
	float getCraneX();
	float getCraneY();
	float getCraneZ();
	//
	void setAutoMode(bool check);
	int getBasketTableIdOfScrapBasketPos();
	bool isPositionReservedByCrane(int positionNumber, int positionIndex);

#ifdef EMULATE
	bool isOvenEntranceFull();
#endif

	void signalFromCranePlc(QString table,int index,QString value);			
	void signalFromConveyorsPlc(QString table,int index,QString value);			
	void signalFromNewOvenConveyorsPlc(QString table,int index,QString value);			
	void takeScrapBasket();
	void unlockBasketAfterStacker();
	void unlockBasketReturningFromOven();
	void addToAutoMode(int mode);
	void setTakeBasketsFromStackerOut(bool s);
	void setTakeBasketsFromStacker2Out(bool s);
	void removeFromAutoMode(int mode);
	void setExportBeforeImports(bool s);
	void setNumberOfPosToBeKeptEmptyInBuffer(int n);

	//
	int getNextOvenLoadingCycleTemperEnum();
	void setNextOvenLoadingCycleTemperEnum(int t);
	//Added by Vahid
	void setNextOven2LoadingCycleTemperEnum(int t);
	void recalculateBasketContentsFromCache();

protected:
	void paintGL();
	void paintGLMine(bool forPicking=false,bool forHighliting=false);
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void selectItem(QMouseEvent *ev,bool leftClick,bool rightClick,bool forHighliting);
	void wheelEvent(QWheelEvent *ev);
	void initializeGL();

signals:
	void return160TrackingStatusChanged(QString message);
	void scrapBasketPositionChangedSignal(int basketTableId);
	void missionFromAfterStackerPositionFinishedSignal();
	void missionFromAfterStacker2PositionFinishedSignal();
	void clicked();

public slots:								
	void scrapBasketPositionChangedSlot(int basketTableId);
	void missionFromAfterStackerPositionFinishedSlot();
	void missionFromAfterStacker2PositionFinishedSlot();
	void openGlPartChangedPosSlot();
};											
#endif

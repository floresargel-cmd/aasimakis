#ifndef OW_H
#define OW_H


#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>

#include "pcG.h"
#include "../common/apexG.h"
#include "plc.h"
#include "op.h"
#include "ap.h"
#include "cr.h"
#include "fr.h"
#include "man.h"
#include "manCasSel.h"
class openGLWidgetClass : public QGLWidget
{
	plcClass										*plc;

	GLfloat											xTranslate;
	GLfloat											yTranslate;

	GLfloat											zoom;
	GLfloat											zoomMin;
	GLfloat											zoomMax;

	GLfloat											zRotate;
	GLfloat											zRotMin;
	GLfloat											zRotMax;

	GLfloat											elevate;
	GLfloat											elevateMin;
	GLfloat											elevateMax;

	QPoint											lastMousePos;
	QPoint											midleMousePos;
	//
	managerClass								*manager;
	openGLPartClass							*baseOpenGl;
	openGLPartClass							*cellsOpenGl;
	craneClass									*crane;
	forkClass										*fork;
	allPosisionsClass						*allPosisions;
 Q_OBJECT
    public:
	openGLWidgetClass(QWidget *parent=0,QGLWidget *shareWidget=0,GLfloat zoomMin_=0.1,GLfloat zoomMax_=30.0,GLfloat zRotMin_=-180.,GLfloat zRotMax_=180.,GLfloat elevateMin_=0.,GLfloat elevateMax_=90.);
	~openGLWidgetClass();
	void setPlcClass(plcClass *plc_);
	void signalFromCranePlc(QString table,int index,QString value);
	void signalFromForkPlc(QString table,int index,QString value);
	craneClass* getCraneClass();
	forkClass* getForkClass();
	allPosisionsClass* getAllPosClass();
	float getPacketCraneX();
	float getPacketCraneY();
	float getPacketCraneZ();
	float getForkCraneX();
	float getForkCraneY();
	float getForkCraneZ();
	//
	void startTimers();
	void setExportsDestination(managerClass::exportsDestinationEnum e);
	void enableAutoMissions(bool enable);
protected:
	void paintGL();
	void paintGLMine(bool forPicking=false);
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void selectItem(QMouseEvent *ev,bool leftClick);
	void wheelEvent(QWheelEvent *ev);
	void initializeGL();
signals:
	void clicked();
public slots:
	void openGlPartChangedPosSlot();
};
#endif

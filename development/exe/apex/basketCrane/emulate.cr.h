#ifndef EMULATECR_H
#define EMULATECR_H

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QDialog>
#include <QtNetwork>
#include "emulate.G.h"
#include "../common/tcpTux.h"
class positionClass
{
public:
	int									id;
	float								x;
	float								y;
	float								z;
	positionClass &operator=(const positionClass& p)
	{
		id=p.id;
		x=p.x;
		y=p.y;
		z=p.z;
		return *this;
	}
	positionClass(){id=0;x=0.f;y=0.f;z=0.f;};
	positionClass(int id_,float x_,float y_,float z_){id=id_;x=x_;y=y_;z=z_;}
};
class positionsClass
{
	QVector<positionClass>	positions;
public:
	positionsClass(){};
	void append(positionClass p){positions.append(p);}
	positionClass *findPosWithId(int id)
	{
		for (int i=0;i<positions.count();i++)
		{
			if (id==positions[i].id)
				return &positions[i];
		}
		return NULL;
	}
};
class craneEmulatorClass : public QWidget
{
	tuxipClass							*craneTuxip;
	QTimer									mainTimer;
	QTimer									stepTimer;
	QTime										time;
	QDoubleSpinBox					*velocitySpinBox;
	float										velocityMultiplier;

	QSpinBox								*stackerSpinBox;
	QTimer									stackerTimer;

	QSpinBox								*destackerSpinBox;
	QTimer									destackerTimer;
	//
	positionClass						*fromPos;
	positionClass						*toPos;
	int											pcStatus;
	int											myStatus;
	float										x;
	float										y;
	float										z;
	float										xTarget;
	float										yTarget;
	float										zTarget;
	//
	float										fromPosX;
	float										fromPosY;
	float										fromPosZ;
	float										toPosX;
	float										toPosY;
	float										toPosZ;
	int											step;
	bool										fromOvenEntry;
	bool										toOvenEntry;
	bool										fromStackerEntry;
	//
	int											beforeStackerBasket;
	int											onStackerBasket;
	int											afterStackerBasket;
	int											basketToAssimakisW;
	int											basketToAssimakis;
	int											basketToOmavW;
	int											basketToOmav;
	int											inOutPosBasket;
	//
	bool										toStackerEntry;
	enum
	{
		START=1,
		FROM_GO_UP,
		FROM_GO_TO_X_CLEAR_TO_MOVE,//for oven
		FROM_GO_TO_Y_CLEAR_TO_MOVE,
		FROM_GO_FROM_POS_X,
		FROM_GO_FROM_POS_Y,
		FROM_GO_FROM_POS_Z,
		FROM_GO_FROM_POS_Z_UP,
		TO_GO_TO_X_CLEAR_TO_MOVE,//for oven
		TO_GO_TO_Y_CLEAR_TO_MOVE,
		TO_GO_TO_POS_X,
		TO_GO_TO_POS_Y,
		TO_GO_TO_FINAL_X,//for oven
		TO_GO_TO_POS_Z,
		TO_GO_TO_POS_Z_UP,
		END,
	};
	//
	positionsClass					positions;
	Q_OBJECT
public:
   craneEmulatorClass(tuxipClass *craneTuxip_);
private:
	QTimer								animateTimer;
  tuxipClass						*craneTuxipConnection;
public:
	void setPcStatus(int st);
	void setFromPos(int st);
	void setToPos(int st);
	float stepToTarget(float pos,float target,float step);
	void plcDataChanged(QString table,int index,QString value);
	void tuxIsReady();
private slots:
  void mainTimerSlot();
  void onVelocitySpinBoxSlot(double v);
 	void onStackerSpinBoxSlot(int t);//auto full timer interval
  void onDestackerSpinBoxSlot(int t);//auto destacker timer interval
	//
	void onAnimateTimerSlot();
	void onStepTimerSlot();
	void stackerTimerSlot();
	void destackerTimerSlot();
};
#endif

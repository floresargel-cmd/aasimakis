#ifndef CR_H
#define CR_H

#include <QtGui>
#include <QHash>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "GL/GLU.h"
class craneClass;

#include "cG.h"
#include "oP.h"
#include "bp.h"
#include "ap.h"

class craneClass:public QObject			
{
	QTimer													activeMissionTimer;
	QTimer													animateTimer;

	Q_OBJECT

	openGLPartClass											*horizontalOpenGl;
	openGLPartClass											*transverseOpenGl;
	openGLPartClass											*verticalOpenGl;

	bool													isCarryingBasket;

	float													lastPlcXInMeters;
	float													lastPlcYInMeters;
	float													lastPlcZInMeters;

	allPositionsClass										*allPositions;

	tcpServerClass											*cranePlcTcpServer;
	tcpServerClass											*conveyorsPlcTcpServer;
	int														plcStatus;
	bool													hasActiveMission;

	basketPositionClass										*myBasketPos;
	basketPositionClass										*activeFromBasketPos;
	basketPositionClass										*activeToBasketPos;
	basketPositionClass										*lastToBasketPos;

	int														myFromPosInPlc;
	int														myToPosInPlc;
	int														myStatusInPlc;
	int														myLengthTypeInPlc;
	int														my500BasketInPlc;
	int														my510BasketInPlc;
	int														my520BasketInPlc;
	//////
	float													plcXInMeters;
	float													plcYInMeters;
	float													plcZInMeters;

	QVector<float>											plcXVelocityArray;
	QVector<float>											plcYVelocityArray;
	QVector<float>											plcZVelocityArray;

	float						plcXVelocity;
	float						plcYVelocity;
	float						plcZVelocity;
	
	QTime						animateTime;
	QTime						plcXTime;
	QTime						plcYTime;
	QTime						plcZTime;
	
	float						openGlX;
	float						openGlY;
	float						openGlZ;
	//
	//bool						missionFinishedWithOverWeight;

public:
	int							plcModeEnum;
	enum{
		NO_MODE=0,
		MAINTENANCE_MODE,
		AUTO_MODE,
		STEP_MODE,
	};

public:
	craneClass();
	~craneClass();
	//
	void setPlcX(float x);
	void setPlcY(float y);
	void setPlcZ(float z);

	float	getPlcX();
	float	getPlcY();
	float	getPlcZ();	

	void openGlDraw();
	void openGlDrawMissionMarkers();
	void openGlDrawForPicking();
	bool mouseOver(QColor pickedColor);
	bool mouseClicked(QColor pickedColor);
	void setCranePlcTcpServer(tcpServerClass *cranePlcTcpServer_);
	void setConveyorsPlcTcpServer(tcpServerClass *conveyorsPlcTcpServer_);
	void setallPositions(allPositionsClass *all);
	//
	void clearActivePositions();
	void setActivePositions(basketPositionClass* from, basketPositionClass *to);
	bool move(basketPositionClass *from,basketPositionClass *to, QString* rejectionReason = NULL);
	void loadFromDatabase();
	bool getHasActiveMission();
	bool isAssignmentReservedForMission(basketPositionClass* position, int basketId);
	void plcStatusChanged(int status);
	void plcMemoryChanged(int index,int value);
	void plcStatusN20_15Changed(int status);
	void recalculateCarriedBasketContentsFromCache();
	void synchronizeObserverMissionFromPlc();
	//void plcStatusN2016Changed(int status);
	bool closeMission();
	bool abortMission();
	basketPositionClass* getLastToPosition();
	//

public slots:								
	void activeMissionTimerSlot();						
	void animateTimerSlot();						
	
private:
	QHash<QString, QString> automaticMoveErrors;
	void showAutomaticMoveError(basketPositionClass* from, basketPositionClass* to, const QString& reason);

signals:
	void scrapBasketPositionChangedSignal(int basketTableId);
	void missionFromAfterStackerPositionFinishedSignal();
	void missionFromAfterStacker2PositionFinishedSignal();
	void openGlChangedSignal();
};						

#endif

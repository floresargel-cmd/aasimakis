#ifndef BS_H
#define BS_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QGraphicsSceneMouseEvent>

#include "ppG.h"
#include "dxfGV.h"
#include "dxf.h"
#include "sp.h"
#include "pl.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////basketProfileGroupClass////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class basketProfileGroupClass:public QGraphicsObject
{
	int																	isDragging;
	QPointF															draggingStartPos;
	int																	needsToBeCalculated;
	int																	noOfProfiles;
	profileProductionClass*							profileProduction;
	QPainterPath												path;
	int																	isMouseOver;
	int																	isSelected;
	double															length;
	QVector<layerProfileGroupClass*>		layerProfilesVector;
Q_OBJECT								
public:
	basketProfileGroupClass(profileProductionClass* profileProduction_);
	~basketProfileGroupClass();
	QRectF boundingRect()const;
	void paint(QPainter *painter,const QStyleOptionGraphicsItem *opt,QWidget *w);
	//
	void setNeedsToBeCalculated(int needsToBeCalculated_);
	int getTableId();
	double getLength();
	profileProductionClass *getProfileProduction();
	QPainterPath getProfilesPath();
	void addProfile(layerProfileGroupClass *layerProfileGroup);
	void setMouseOverProfileProducton(profileProductionClass* p);


	void hoverEnterEvent(QGraphicsSceneHoverEvent *ev);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev);

  void mousePressEvent(QGraphicsSceneMouseEvent *ev);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);

	void dragEnterEvent(QGraphicsSceneDragDropEvent *ev);
	void dragLeaveEvent(QGraphicsSceneDragDropEvent *ev);
	void dropEvent(QGraphicsSceneDragDropEvent *ev);
signals:
	void mouseEnterSignal(profileProductionClass*);
	void mouseLeaveSignal(profileProductionClass*);
	void productionSequenceChangedSignal();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////basketClass////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class basketClass : public QWidget		
{
	int																		basketNumber;//initially this is minus something
	bool																	firstToComeOutOfTheStacker;
	bool																	isSelected;
	tcpServerClass											*tcpServer;
	bool																	maxWeightLimitReached;
	//
	QGraphicsScene												*myScene;
	dxfQGraphicsViewClass									*myQGraphicsView;
	dxfGroupOfGraphicsItemClass						*basketGraphics;
	QPushButton														*basketReadyBtn;
	bool																	needsToBeCalculated;
	//
	double																totalWeight;
	double																maxProfileLength;
	//
	QLabel																*totalWeightLabel;
	//
	QVector<profileLayerClass*>						layersVector;
	QVector<basketProfileGroupClass*>			profilesGroupsVector;
	QVector<int>													stackerPlcWordsVector;
	int																		stackerPlcCombsWord;
Q_OBJECT								
public:										
	basketClass(int basketNumber_,bool firstToComeOutOfTheStacker_,QWidget *p=NULL);
	~basketClass();
//
	void setIsSelected(bool s);
	bool getIsSelected();
	int getFirstProfileCode();
//
	//int getBasketNumber();
	void setNeedsToBeCalculated(bool needsToBeCalculated_);
	void setPrivileges(int privilegeType);
//
	int getNumberOfLayers();
	profileLayerClass* getLayer(int index);
//
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
//
	double getTotalHeight();
	double getTotalWeight();
	int getMaxUsedComb();
	bool addEmptyLayerForProfiles(profileProductionClass* p,bool forceOnComb);
	int addProfiles(profileProductionClass*,int number);
	bool hasProfile(int code);
	void calculate();
	//
	void setTcpServer(tcpServerClass *tcpServer_);
	void sendDatabaseSpacerDataToPlc(int firstIndex);
	void setStackerPlcWords(QVector<int> v);
	void setStackerCombsWord(int v);
	int getStackerPlcWord(int index);
	int getStackerPlcCombsWord();

	int getDatabaseWord(int index);
	int getCombNumberOfLayer(int index);

	void calculateGroups();
	QVector<basketProfileGroupClass*> getProfilesGroupsVector();
signals:
	void selectedSignal(basketClass *b,bool selected);
public slots:
	void basketReadyBtnSlot();
};																	
#endif
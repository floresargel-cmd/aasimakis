#ifndef BS_H
#define BS_H

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qcoreevent.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qpoint.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qrect.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/kernel/qevent.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpainter.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpainterpath.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsitem.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsitem.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsscene.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicssceneevent.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/styles/qstyleoption.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlabel.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qpushbutton.h"

#include "../../common/tcpTux.h"
#include "dxf.h"
#include "dxfGV.h"
#include "pl.h"
#include "pp.h"

class basketProfileGroupClass : public QGraphicsObject
{
	Q_OBJECT

public:

	explicit basketProfileGroupClass(profileProductionClass* profileProduction_);
	~basketProfileGroupClass();

	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* opt, QWidget* w);

	void setNeedsToBeCalculated(int needsToBeCalculated_);
	int getTableId();

	double getLength();
	profileProductionClass* getProfileProduction();
	QPainterPath getProfilesPath();
	void addProfile(layerProfileGroupClass* layerProfileGroup);
	void setMouseOverProfileProducton(profileProductionClass* p);

	void hoverEnterEvent(QGraphicsSceneHoverEvent* ev);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* ev);

	void mousePressEvent(QGraphicsSceneMouseEvent* ev);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* ev);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* ev);

	void dragEnterEvent(QGraphicsSceneDragDropEvent* ev);
	void dragLeaveEvent(QGraphicsSceneDragDropEvent* ev);
	void dropEvent(QGraphicsSceneDragDropEvent* ev);

signals:
	void mouseEnterSignal(profileProductionClass*);
	void mouseLeaveSignal(profileProductionClass*);
	void productionSequenceChangedSignal();

private:
	int isDragging;
	QPointF draggingStartPos;
	int needsToBeCalculated;
	int noOfProfiles;
	profileProductionClass* profileProduction;
	QPainterPath path;
	int isMouseOver;
	int isSelected;
	double length;
	QVector<layerProfileGroupClass*> layerProfilesVector;
};

class basketClass : public QWidget
{
	Q_OBJECT

public:
	void refreshBackgroundBrush();
	basketClass(int basketNumber_, bool firstToComeOutOfTheStacker_, QWidget* p = NULL);
	~basketClass();
	void resetLastLayerWidth();
	void setIsSelected(bool s);
	bool getIsSelected();
	int getFirstProfileCode();

	void setNeedsToBeCalculated(bool needsToBeCalculated_);
	void setPrivileges(int privilegeType);

	int getNumberOfLayers();
	profileLayerClass* getLayer(int index);

	void enterEvent(QEvent* ev);
	void leaveEvent(QEvent* ev);
	void mousePressEvent(QMouseEvent* ev);

	double getTotalHeight();
	double getTotalWeight();
	int getMaxUsedComb();

	bool addEmptyLayerForProfiles(profileProductionClass* p, bool forceOnComb);
	int addProfiles(profileProductionClass* p, int number);
	bool hasProfile(int code);
	bool hasProfileTableId(int tableId);

	void calculate();
	void calculateGroups();
	QVector<basketProfileGroupClass*> getProfilesGroupsVector();

	void setTcpServer(tcpServerClass* tcpServer_);
	void sendDatabaseSpacerDataToPlc(int firstIndex);
	void setStackerPlcWords(QVector<int> v);
	void setStackerCombsWord(int v);
	int getStackerPlcWord(int index);
	int getStackerPlcCombsWord();

	int getDatabaseWord(int index);
	int getCombNumberOfLayer(int index);

	int getDisplayBasketNumber() const;

signals:
	void selectedSignal(basketClass* b, bool selected);

public slots:
	void basketReadyBtnSlot();

private:
	void addBasketNumberSceneLabel();
	void refreshBasketNumberDisplay();

private:
	int basketNumber;
	int displayBasketNumber;
	bool firstToComeOutOfTheStacker;
	bool isSelected;
	tcpServerClass* tcpServer;
	bool maxWeightLimitReached;

	QGraphicsScene* myScene;
	dxfQGraphicsViewClass* myQGraphicsView;
	dxfGroupOfGraphicsItemClass* basketGraphics;
	QGraphicsSimpleTextItem* basketNumberSceneText;
	QPushButton* basketReadyBtn;
	bool needsToBeCalculated;

	double totalWeight;
	double maxProfileLength;

	QLabel* basketMaxWeightTopLabel;
	QLabel* basketNumberFooterLabel;
	QLabel* totalWeightLabel;

	QVector<profileLayerClass*> layersVector;
	QVector<basketProfileGroupClass*> profilesGroupsVector;
	QVector<int> stackerPlcWordsVector;
	int stackerPlcCombsWord;
};

#endif

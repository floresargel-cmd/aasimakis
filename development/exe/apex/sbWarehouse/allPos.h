#ifndef ALLPOS_H
#define ALLPOS_H

class allPossClass;
#include "Gsw.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "crane.h"
class posCorrectionClass
{
public:
	int				col;
	int				row;
	float     x;
	float     y;
	float     z;
public:
	posCorrectionClass()
	{
		col=row=0;x=y=z=0.f;
	}
	posCorrectionClass(int col_,int row_,float x_,float y_,float z_)
	{
		col=col_;row=row_;x=x_;y=y_;z=z_;
	}
};
class allPossClass : public QWidget				
{
	tuxipClass															*sbTuxip;
	tuxipClass															*autoTuxip;
	QVector<posClass*>											allPosV;
	QVector<posClass*>											plcPosV;
	craneClass															*crane;
	posClass 																*manualSelectedFromPos;
	//
	posClass 																*craneFrontConveyor1;
	posClass 																*craneFrontConveyor2;
	posClass 																*craneFrontConveyor3;
	posClass 																*craneFrontConveyor4;
	posClass 																*craneFrontConveyor5;

	posClass 																*pincer1;
	posClass 																*pincer2;

	posClass 																*tripleExit1;
	posClass 																*tripleExit1Out;
	posClass 																*tripleExit2;
	posClass 																*tripleExit2Out;
	posClass 																*tripleImport;
	posClass 																*tripleImportOut;

	posClass 																*doubleExit;
	posClass 																*doubleExitOut;
	posClass 																*doubleImport;
	posClass 																*doubleImportOut;

	posClass																*oldPincerBlueBuffer_1_1;
	posClass																*oldPincerBlueBuffer_1_2;
	posClass																*oldPincerBlueBuffer_1_3;
	posClass																*oldPincerBlueBuffer_1_4;

	posClass																*oldPincerBlueBuffer_2_1;
	posClass																*oldPincerBlueBuffer_2_2;
	posClass																*oldPincerBlueBuffer_2_3;
	posClass																*oldPincerBlueBuffer_2_4;

	posClass																*oldPincerBeforeDestucker;
	posClass																*oldPincerDestucker;
	posClass																*oldPincerScrap;
	posClass																*oldPincerInOut;
	posClass																*oldPincerSideMoving;
	posClass																*oldPincerIn;

	posClass																*newPincerUserImportBuffer1;
	posClass																*newPincerUserImportBuffer2;
	posClass																*newPincerUserImportBuffer3;
	posClass																*newPincerUserImportBuffer4;

	posClass																*newPincerImportBuffer1;
	posClass																*newPincerImportBuffer2;
	posClass																*newPincerImportBuffer3;
	posClass																*newPincerImportBuffer4;

	posClass																*newPincerUnloadBuffer1;
	posClass																*newPincerUnloadBuffer2;
	posClass																*newPincerUnloadBuffer3;
	posClass																*newPincerUnloadBuffer4;

	int																			noOfRows;
	int																			showType;
	Q_OBJECT								
public:
	enum{//showType
		showCranePos=		0x0000001,
		showPincer1Pos=	0x0000002,
		showPincer2Pos=	0x0000004,
	};
public:
	allPossClass(tuxipClass *sbTuxip_,tuxipClass *autoTuxip_,int showType_,QGraphicsScene *layoutScene,QWidget *p);
	~allPossClass();
	void calculateCorrections();
	void setCraneClass(craneClass *c);
	posClass* getPosWithId(int id,bool unLockedOnly=true,bool exitIfNotFound=true);
	posClass* getPosWithColumnRow(int col,int row);
	posClass* getCellWithCassette(int b,bool unLockedOnly=true);
	posClass* getEmptyCellForCassette(int b);
	int getEmptyIdleCassette();
	void drawForReport(QString pdfFile);
	void drawPageLayout(int maxW,int maxH,int dW,int dH,int noOfColumnsInPage,int firstcol,QString headerText,QString footerText,bool rightSideCells,QPainter *painter);
	void drawColForReport(int col,int dW,int dH,int noOfColumnsInPage,int firstcol,bool rightSideCells,QPainter *painter);
	bool isCassettesActive(int cassettes);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	QVector<posClass*> getAllPos();
	QVector<QPointF> getMinusCellsPositions();
	QVector<QPointF> getPlusCellsPositions();
	int getNumberOfCells();
public slots:
	void readFromDbTimerTimeoutSlot();
	void posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*);		
};											
#endif

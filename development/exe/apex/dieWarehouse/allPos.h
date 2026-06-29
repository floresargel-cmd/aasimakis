#ifndef ALLPOS_H
#define ALLPOS_H

#include "Gd.h"
#include "serv.h"
#include "gView.h"

class allPossClass;
#include "pos.h"
#include "wSt.h"
class craneClass;
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
	tuxipClass															*tuxip;
	QVector<posClass*>											posV;
	workStationClass												*workStation;
	craneClass															*crane;
	//
	posClass 																*manualSelectedFromPos;
	int																			noOfRows;
	Q_OBJECT								
public:										
	allPossClass(tuxipClass *tuxipConnection,QGraphicsScene *layoutScene,QWidget *p=NULL);
	~allPossClass();
	void calculateCorrections();
	workStationClass *getWorkStation();
	QString getPosOfBasket(int basket,int ignorePos);
	void setCraneClass(craneClass *c);
	posClass* getPosWithId(int id,bool unLockedOnly=true);
	posClass* getPosWithColumnRow(int col,int row);
	void drawForReport(QString pdfFile);
	void drawPageLayout(int maxW,int maxH,int dW,int dH,int noOfColumnsInPage,int firstcol,QString headerText,QString footerText,bool rightSideCells,QPainter *painter);
	void drawColForReport(int col,int dW,int dH,int noOfColumnsInPage,int firstcol,bool rightSideCells,QPainter *painter);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	posClass* getCellWithBasket(int b,bool unLockedOnly);
	posClass* getEmptyCellForBasket(int b);
	void highLightPosWithBasket(int b);
public slots:								
	void posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*);		
};											
#endif

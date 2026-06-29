#ifndef BST_H
#define BST_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "ppG.h"
#include "bs.h"
#include "ppl.h"
class basketStackingClass : public QWidget		
{
	tcpServerClass					*tcpServer;
	QHBoxLayout							*mainLayout;
	QScrollArea							*mainScrollArea;
	QWidget									*mainScrollAreaWidget;
	QVector<basketClass*>		basketsVector;
	QVector<int>						stackerPlcWordsVector;
	int											stackerPlcCombsWord;
	//
	int											needsToBeCalculated;
	//
Q_OBJECT								
public:										
	basketStackingClass(QWidget *p);
	~basketStackingClass();
//
	void calculateLayout(profileProductionLayoutClass* profileProductionLayout);
	void setNeedsToBeCalculated(int c,int force=1);
	void clearAllBaskets();
	void appendBasket(basketClass* b);
	basketClass* getFirstVisible();
	void setVisibleBasketWithProfile(int profile);
	void stackerPlcMemoryChanged(QString table,int index,QString value);
	void setTcpServer(tcpServerClass *tcpServer_);
	QVector<basketClass*>		getBasketsVector();
signals:
	void basketSelectedSignal(basketClass*);
public slots:
	void basketClassSelected(basketClass *b,bool selected);
};																	
#endif
#ifndef GC_H
#define GC_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>

#include "../common/apexG.h"
#include "cL.h"
class groupOfCellsClass : public QWidget				
{
	int													fromColumn;
	int													toColumn;
	QVector<cellLabelClass*>		cellsVector;
Q_OBJECT
public:
	groupOfCellsClass(int fromColumn_,int toColumn_,bool positiveCells);
	void writeToPdf(QPainter *painter,float y,float cellWidth,float cellHeight);
private:									
};											
#endif

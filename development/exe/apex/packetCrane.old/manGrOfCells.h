#ifndef MANGROFCELLS_H
#define MANGROFCELLS_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
class cellLabelClass;
#include "pcG.h"
#include "cr.h"
#include "manCellLabel.h"
class groupOfCellsClass : public QWidget
{
	craneClass									*crane;
	int													fromColumn;
	int													toColumn;
	QVector<cellLabelClass*>		cellsVector;
Q_OBJECT
public:
	groupOfCellsClass(int fromColumn_,int toColumn_,bool positiveCells,craneClass *crane_);
	~groupOfCellsClass();
	void updateFromDb();
private:
};
#endif

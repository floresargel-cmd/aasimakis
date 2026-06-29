#ifndef BCONT_H
#define BCONT_H
#include "Gb2.h"
class basketContentsClass:public QWidget
{
	int											basketNum;
	//
	QTableWidget						*tableView;
enum columnsEnum
{
	colProfile=0,
	colQuantity=1,
	colLength=2,
	colTemper=3,
	colSoNumber=4,
	colSoLine=5,
	colCurrentFinCode=6,
	colStdPract=7,
	colCustShip=8,
	colNextDeptNum=9,
	colStopTime=10,
	colDestination=11,
};
public:
	Q_OBJECT
public:
	basketContentsClass(QWidget *w);
	~basketContentsClass();
	void updateTable(int basketNum_);
private:
	void updateTable();
public slots:
};
#endif

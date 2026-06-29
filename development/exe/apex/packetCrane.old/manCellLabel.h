#ifndef CB_H
#define CB_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include "cr.h"
#include "manCellDlg.h"
class cellLabelClass : public QWidget				
{
	craneClass			*crane;
	int							cellNumber;
	int							row;
	int							col;
	int							packetId;
	bool						isOnSb;
	QLabel					*packetLabel;
Q_OBJECT
public:
	cellLabelClass(int cellNumber_,int row_,int col_,craneClass *crane_);
	~cellLabelClass();
	void updateFromDb();
	virtual void	mouseReleaseEvent(QMouseEvent *ev);
};
#endif

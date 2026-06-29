#ifndef CB_H
#define CB_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>

#include "../common/apexG.h"
#include "wContH.h"
#include "cC.h"
class cellLabelClass : public QWidget				
{
	int							cellNumber;
	int							packetId;
	bool						isOnSb;
Q_OBJECT
public:
	cellLabelClass(int cellNumber_,int row,int col);
	void writeToPdf(QPainter *painter,double x,double y,float cellWidth,float cellHeight);
	void clicked();
	virtual void	mouseReleaseEvent(QMouseEvent *ev);
};
#endif

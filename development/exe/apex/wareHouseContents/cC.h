#ifndef CC_H
#define CC_H

#include <QtGui>
#include <QMainWindow>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#define USE_LOCAL_DB
#undef USE_LOCAL_DB
#include "../common/apexG.h"
class cellContClass : public QDialog
{
	int				cellNumber;
	//
	QSpinBox	*packetNumberSpinBox;
Q_OBJECT
public:
	cellContClass(int cellNumber_,QWidget *p);
private:
public slots:
    void okBtnClickedSlot();
};
#endif

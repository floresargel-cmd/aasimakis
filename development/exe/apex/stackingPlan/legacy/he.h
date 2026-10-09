#ifndef HE_H
#define HE_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPushButton>
#include <QTextBrowser>
class basketClass;

#include "ppG.h"
#include "dxfGV.h"
#include "dxf.h"
#include "ls.h"
#include "bs.h"
class helpClass : public QDialog
{
	//
Q_OBJECT
public:
	helpClass(QWidget *p);
	~helpClass();
};
#endif

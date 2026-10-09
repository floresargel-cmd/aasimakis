#ifndef OP_H
#define OP_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPushButton>
class basketClass;

#include "ppG.h"
#include "dxfGV.h"
#include "dxf.h"
#include "ls.h"
#include "bs.h"
class optionsClass : public QDialog
{
	QSpinBox													*pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox;
	QSpinBox													*maxNoOfProfilesToLoadSpinBox;
	QDoubleSpinBox										*angleForArcDrawingSpinBox;
	//
Q_OBJECT
public:
	optionsClass(QWidget *p);
	~optionsClass();
public slots:
	void accept();
	void reject();
};
#endif

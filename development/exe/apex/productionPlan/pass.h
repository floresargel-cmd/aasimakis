#ifndef PASS_H
#define PASS_H

#include <QtGui>
#include <QMainWindow>
#include <QList>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


#include "ppG.h"

class passwordClass: public QDialog
{
	//
	QListWidget							*userList;
	QLineEdit								*passEdit;
public:
public:
	Q_OBJECT
public:
	passwordClass(QWidget *p);
	int exec();
	bool checkPassword();
private slots:
	void reject();
	void accept();
};
#endif

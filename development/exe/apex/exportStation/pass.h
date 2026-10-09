#ifndef PASS_H
#define PASS_H

#include <QtGui>
#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QList.h>

#include "eG.h"
class passwordClass: public QDialog
{
	QString						curentUser;
	//
	QVector<QVariant>	nameArray;
	QVector<QVariant>	passArray;
	//
	QListWidget				*userList;
	QLineEdit					*passEdit;
public:
	Q_OBJECT
public:
	passwordClass(QWidget *p);
	int exec();
	bool hasPrivileges();
	bool checkPassword();
	QString getUser(){return curentUser;}
private slots:
	void reject();
	void accept();
};
#endif

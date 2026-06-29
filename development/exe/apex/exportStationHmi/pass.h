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
#include <QDialog>
#include <QMessageBox>
#include "../common/apexG.h"
class passwordClass: public QDialog
{
	//
	QListWidget							*userList;
	QLineEdit								*passEdit;
	bool										passIsCorrect;
public:
public:
	Q_OBJECT
public:
	passwordClass(QWidget *p);
	int exec();
	bool checkPassword();
	bool getPasswordIsCorrect();
private slots:
	void reject();
	void accept();
};
#endif

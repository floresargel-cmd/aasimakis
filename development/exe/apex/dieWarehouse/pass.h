#ifndef PASS_H
#define PASS_H

#include "Gd.h"
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
	passwordClass(QWidget *p=NULL);
	int exec();
	bool checkPassword();
	bool getPasswordIsCorrect();
private slots:
	void reject();
	void accept();
};
#endif

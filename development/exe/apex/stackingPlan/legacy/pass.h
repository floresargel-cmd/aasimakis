#ifndef PASS_H
#define PASS_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QtGui>
#include <QVBoxLayout>

#include "ppG.h"

class passwordClass : public QDialog
{
	Q_OBJECT

public:
	explicit passwordClass(QWidget* p = 0);
	virtual ~passwordClass();

	int exec();
	bool checkPassword();

public slots:
	void reject();
	void accept();

private:
	QListWidget* userList;
	QLineEdit* passEdit;
};

#endif

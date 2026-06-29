#ifndef TCPTUX_H
#define TCPTUX_H

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QWidget>

#include "serv.h"

extern bool theOneBCToRuleThemAll;

class tcpServerClass : public QWidget
{
	Q_OBJECT

private:
	tuxipClass* tuxip;

public:
	explicit tcpServerClass(tuxipClass* tuxip_, QWidget* parent = nullptr);

	tuxipClass* getTuxipClass();
	void sendString(QString outgoingString);
};

#endif
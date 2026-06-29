#include "tcpTux.h"

#include <QDebug>

// 🔧 DEFINE THE GLOBAL VARIABLE
bool theOneBCToRuleThemAll = true;

// #define SEMILIVE

tcpServerClass::tcpServerClass(tuxipClass* tuxip_, QWidget* parent)
	: QWidget(parent),
	tuxip(tuxip_)
{
	// Do not hide this widget; it may be needed for signals.
}

void tcpServerClass::sendString(QString outgoingString)
{
	qDebug() << "tcpTux.cpp - tcpServerClass::sendString() - outgoingString =" << outgoingString;

#ifdef SEMILIVE
	qDebug() << "tcpTux.cpp - SEMILIVE mode - skipping send";
	return;
#else
	if (!tuxip)
	{
		qWarning() << "tcpTux.cpp - ERROR: tuxip is NULL";
		return;
	}

	if (theOneBCToRuleThemAll)
	{
		tuxip->sendString(outgoingString);
	}
	else
	{
		qDebug() << "tcpTux.cpp - Host not active - skipping send";
	}
#endif
}

tuxipClass* tcpServerClass::getTuxipClass()
{
	return tuxip;
}
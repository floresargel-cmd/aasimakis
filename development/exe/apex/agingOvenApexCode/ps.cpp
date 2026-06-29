#include "ps.h"

proximitySwitchClass::proximitySwitchClass(tcpServerClass *tcpServer_, int plcIndex_, int plcBit_, int x, int y, QString title, QWidget *p) :QWidget(p)
{
	tcpServer = tcpServer_;
	plcIndex = plcIndex_;
	plcBit = plcBit_;
	imageLabel = new QLabel(p);
	QImage image(":/proximitySwitchOff.png");
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setGeometry(x, y, 8, 8);

	QLabel *titleLabel = new QLabel(title, p);
	titleLabel->setStyleSheet("QLabel{color:#14f75a;}");
	titleLabel->setGeometry(x + 10, y - 6, 50, 20);

	setToolTip(tr("%1 proximity switch").arg(plcIndex) + tr("this is the proximity switch coresponding to N11.%1.%2 bit").arg(plcIndex).arg(plcBit));
	setGeometry(x, y, 55, 20);
}

void proximitySwitchClass::plcValueChanged(int index, int bit, bool value)
{
	if ((plcIndex == index) && (plcBit == bit))
	{
		if (value == true)
		{
			QImage image(":/proximitySwitchOn.png");
			imageLabel->setPixmap(QPixmap::fromImage(image));
		}
		else
		{
			QImage image(":/proximitySwitchOff.png");
			imageLabel->setPixmap(QPixmap::fromImage(image));
		}
	}
}

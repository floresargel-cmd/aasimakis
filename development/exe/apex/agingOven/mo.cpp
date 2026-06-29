#include "mo.h"
moveConveyorClass::moveConveyorClass(tcpServerClass *tcpServer_,int typeEnum_,int plcCommand,QString name,int x,int y,QWidget *p,int w,int h):myQPushButton(tcpServer_,QString("N11|0|%1").arg(plcCommand),"N11|0|0","",p)
{
	tcpServer=tcpServer_;
	typeEnum=typeEnum_;
	if (typeEnum==TYPE_LEFT)
		setIcon(QIcon(":/arrowMoveLeft.png"));
	else if (typeEnum==TYPE_RIGHT)
		setIcon(QIcon(":/arrowMoveRight.png"));
	else if (typeEnum==TYPE_UP)
		setIcon(QIcon(":/arrowMoveUp.png"));
	else if (typeEnum==TYPE_DOWN)
		setIcon(QIcon(":/arrowMoveDown.png"));
	
	setIconSize(QSize(25,25));
	setToolTip("<b><span style='color:blue'>"+name+tr("</b><br/><span style='color:black'>This button sends the number %1 to PLC memory N11.0.").arg(plcCommand));
	myQPushButton::setGeometry(x,y,w,h);
}
//void moveConveyorClass::mousePressEvent(QMouseEvent *e)
//{
//	QPushButton::mousePressEvent(e);
//	tcpServer->sendString(QString("N11|0|%1").arg(plcCommand));
//}
//void moveConveyorClass::mouseReleaseEvent(QMouseEvent *e)
//{
//	QPushButton::mouseReleaseEvent(e);
//	tcpServer->sendString(QString("N11|0|0"));
//}
//void moveConveyorClass::leaveEvent(QEvent *ev)
//{
//	QPushButton::leaveEvent(ev);
//}

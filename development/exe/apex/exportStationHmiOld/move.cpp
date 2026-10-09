#include "move.h"
moveConveyorClass::moveConveyorClass(tcpServerClass *tcpServer_,int typeEnum_,QString plcCommandString,QString plcZeroString,QString name,int x,int y,QWidget *p):myQPushButton(tcpServer_,plcCommandString,plcZeroString,"",p)
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
	setToolTip("<b><span style='color:blue'>"+name+tr("</b><br/><span style='color:black'>This button sends<span style='color:red'><br>%1<br><span style='color:black'>to PLC.").arg(plcCommandString));
	setGeometry(x,y,30,30);
}

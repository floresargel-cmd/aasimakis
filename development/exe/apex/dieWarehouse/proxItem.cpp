#include "pch.h"
#include "proxItem.h"
proxItemClass::proxItemClass(QGraphicsScene *scene_,dxfGroupOfGraphicsItemClass *onItem_,dxfGroupOfGraphicsItemClass *offItem_,QString onString_,QString offString_,int plcIndex_,int plcBit_,float xPos,float yPos,QString toolTip,QObject *o):QObject(o)
{
	scene=scene_;
	onItem=new dxfGroupOfGraphicsItemClass(onItem_,xPos,yPos,this);
	offItem=new dxfGroupOfGraphicsItemClass(offItem_,xPos,yPos,this);
	onString=onString_;
	offString=offString_;
	plcIndex=plcIndex_;
	plcBit=plcBit_;
	
	graphicsItemText=new QGraphicsSimpleTextItem();
	scene->addItem(graphicsItemText);
	graphicsItemText->setZValue(10);
	txtTransformation.scale(10.,-10.);
	graphicsItemText->setTransform(txtTransformation,false);
	graphicsItemText->setPos(xPos+75,yPos+75);

	onItem->setToolTip(toolTip);
	offItem->setToolTip(toolTip);
	graphicsItemText->setToolTip(toolTip);
}
proxItemClass::~proxItemClass()
{
}
void proxItemClass::setPos(float x,float y)
{
	onItem->setPos(x,y);
	offItem->setPos(x,y);
	graphicsItemText->setPos(x,y);
}
void proxItemClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (dieIp==ip)
	{
		if("bRobotToPcI"==table)
		{
			if (plcIndex==index)
			{
				bool v=HAVECOMMONBIT(value.toInt(),BITS[plcBit]);
				if (v)
				{
					if (!onItem->scene())
						scene->addItem(onItem);
					if (offItem->scene())
						scene->removeItem(offItem);
					graphicsItemText->setText(onString);
					graphicsItemText->setBrush(QBrush(QColor(75,150,75)));
				}
				else
				{
					if (!offItem->scene())
						scene->addItem(offItem);
					if (onItem->scene())
						scene->removeItem(onItem);			
					graphicsItemText->setText(offString);
					graphicsItemText->setBrush(QBrush(QColor(150,150,150)));
				}
			}
		}
	}
}
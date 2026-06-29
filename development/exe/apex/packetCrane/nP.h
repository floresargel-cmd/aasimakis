#ifndef NP_H
#define NP_H

#include "pcG.h"
#include "../common/apexG.h"
#include "p.h"
#include "cellCont.h"
class newPacketPositionClass:public packetPositionClass			
{
	Q_OBJECT
public:
	newPacketPositionClass(int posId,QString name,QGraphicsScene *scene,dxfClass *mouseOverItem,dxfClass *lockedItem,dxfClass *sbItem,QWidget *p);
	~newPacketPositionClass();
	//
	void saveToDatabase();
	void loadFromDatabase();
	//void loadContentsFromDatabase();
};
#endif

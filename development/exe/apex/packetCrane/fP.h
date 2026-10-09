#ifndef FP_H
#define FP_H

#include "pcG.h"
#include "../common/apexG.h"
#include "p.h"
#include "cellCont.h"
class forkPositionClass:public packetPositionClass
{
	Q_OBJECT
public:
	forkPositionClass(QGraphicsScene *scene,dxfClass *mouseOverItem,dxfClass *lockedItem,dxfClass *sbItem,QWidget *p);
	~forkPositionClass();
	//
	void saveToDatabase();
	void loadFromDatabase();
//	void loadContentsFromDatabase();
};
#endif

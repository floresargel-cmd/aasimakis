#ifndef CONVP_H
#define CONVP_H

#include "pcG.h"
#include "../common/apexG.h"
#include "cp.h"
#include "cellCont.h"
class conveyorPositionClass:public cellPositionClass
{
	Q_OBJECT
	bool								activePacketExport;//locked untill fork takes the packet it has
	bool								activePacketImport;//locked untill fork places packet
	bool								activeCassetteImport;//locked, crane will take it in
public:
	conveyorPositionClass(int posId,QString name,QGraphicsScene *scene,dxfClass *cellItem,dxfClass *mouseOverItem,dxfClass *lockedItem,dxfClass *cassetteItem,dxfClass *sbItem,dxfClass *fromItem,dxfClass *toItem,QWidget *p);
	~conveyorPositionClass();
	//
	bool canTakePacketThatIsOnPos(packetPositionClass *p);
	bool isActive();
	bool hasIdleCassette();
	bool isActiveForPacketExport();
	bool isActiveForPacketImport();
	bool isActiveForCassetteImport();
	void setActivePacketExport(bool a);
	void setActivePacketImport(bool a);
	void setActiveCassetteImport(bool a);
	void saveToDatabase();
	void loadFromDatabase();
	//void loadContentsFromDatabase();
};
#endif

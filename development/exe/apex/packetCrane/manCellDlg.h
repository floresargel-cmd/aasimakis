#ifndef CC_H
#define CC_H

#include "cr.h"
#include "../common/apexG.h"
class cellContClass : public QDialog				
{
	craneClass	*crane;
	int					cellNumber;
	//
Q_OBJECT
public:
	cellContClass(int cellNumber_,craneClass *crane_,QWidget *p);							
private:									
public slots:								
    void takeCassetteToEastConveyorBtnClickedSlot();						
    void takeCassetteToWestConveyorBtnClickedSlot();						
};											
#endif

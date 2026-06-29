#ifndef CB_H
#define CB_H
class cellLabelClass;
#include "cr.h"
#include "manCellDlg.h"
class cellLabelClass : public QWidget				
{
	craneClass			*crane;
public:
	int							cellNumber;
	int							row;
	int							col;
	int							packetId;
	bool						isOnSb;
	QLabel					*packetLabel;
Q_OBJECT
public:
	cellLabelClass(int cellNumber_,int row_,int col_,craneClass *crane_);
	~cellLabelClass();
	void updateGuiFromData(int packetId,bool isOnSb);
	virtual void	mouseReleaseEvent(QMouseEvent *ev);
};
#endif

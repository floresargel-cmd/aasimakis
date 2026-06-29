#ifndef POS_H
#define POS_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include "pRes.h"

#include "../common/apexG.h"
class positionClass:public QLabel 				
{
	//QLabel																*srtLabel;
	QLabel																*imgLabel;
	QPixmap																noSBPixMap;
	QPixmap																fullSBPixMap;
	QPixmap																emptySBPixMap;
	int																		plcPositionId;

	int																		noSBplcIndex;
	int																		fullSBplcIndex;
	int																		emptySBplcIndex;

	int																		noSBplcBitIndex;
	int																		fullSBplcBitIndex;
	int																		emptySBplcBitIndex;

	int																		noSBPlcCommand;
	int																		fullSBPlcCommand;
	int																		emptySBPlcCommand;
	////
	int																		statusEnum;
public:										
	enum{
		STATUS_NO_SB=1,
		STATUS_FULL_SB,
		STATUS_EMPTY_SB,
	};
public:										
	Q_OBJECT								
public:										
	positionClass(int plcPositionId_,int noSBplcIndex_,int fullSBplcIndex_,int emptySBplcIndex_,int noSBplcBitIndex_,int fullSBplcBitIndex_,int emptySBplcBitIndex_,int noSBPlcCommand_,int fullSBPlcCommand_,int emptySBPlcCommand_,QWidget *p);
	void setPlcNoFullEmptyStatusBitToTrue(int plcIndex,int plcBitIndex);
	bool getHasSB();
	int getStatusEnum();
	int getPositionId();
private:
};											
#endif

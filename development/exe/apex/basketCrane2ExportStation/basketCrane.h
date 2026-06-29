#ifndef basketCrane_H
#define basketCrane_H
#include "../Gs.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
class basketCraneClass : public QWidget				
{
	tuxipClass						*tuxipConnection;
	QSpinBox							*basketSpinBox;
	QLabel								*basketInfoLabel;
	QVector<QPushButton*>	destinationBtnsV;
	QPushButton						*okBtn;
	Q_OBJECT								
public:										
	basketCraneClass(tuxipClass *tuxipConnection_,QWidget *p);
	~basketCraneClass();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void updateDestinationBtns(int destination);
private:									
public slots:								
	void basketSpinBoxChangedSlot(int b);
	void destinationButtonClickedSlot();
	void okButtonClicledSlot();
};											
#endif

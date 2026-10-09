#ifndef SP_H
#define SP_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSplitter>
#include <QGroupBox>
#include <QScrollArea>

#include "../common/apexG.h"
#include "../common/btns.h"
#include "move.h"
#include "movement.h"
#include "ps.h"
#include "eMem.h"
class layerClass : public QWidget
{
	QRadioButton					*activeRadioButton;
	QLabel								*sp1Label;
	QLabel								*sp2Label;
	QLabel								*sp3Label;
	QLabel								*sp4Label;
	QLabel								*sp5Label;
	QLabel								*sp6Label;
	QLabel								*sp7Label;
	QLabel								*sp8Label;
	tcpServerClass				*tcpServer;
	int										layer;
	bool									isOnComb;
	bool									sp1;
	bool									sp2;
	bool									sp3;
	bool									sp4;
	bool									sp5;
	bool									sp6;
	bool									sp7;
	bool									sp8;
Q_OBJECT
public:
	layerClass(tcpServerClass *tcpServer_,int layer_,QWidget *p);
	void setSpacersType(bool sp1_,bool sp2_,bool sp3_,bool sp4_,bool sp5_,bool sp6_,bool sp7_,bool sp8_,bool isOnComb_);
	void setActive(bool active);
	bool getActive();
	bool getIsOnComb();
	bool getExistsSpacer(int index);
	void sendWordToPlc();
	void sendZeroWordToPlc();
signals:
	void activeRadioButtonClickedSignal(layerClass *b,bool checked);
public slots:
	void activeRadioButtonSlot(bool v);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class spacersClass : public QWidget
{
	QPushButton											*sendDataPushButton;
	QLabel													*statusLabel;
	tcpServerClass									*tcpServer;
	bool														blockTcpSignals;
	int															basket;
	//
	QVector<layerClass*>						layersVector;
	QVector<int>										plcWordsVector;
Q_OBJECT
public:
  spacersClass(tcpServerClass *tcpServer_,QWidget *p);
	void loadFromDataBase();
	void setBasket(int b);
	void setAutoStatus(bool st);
	void setOkToSendData(bool st);
	void spacerMemoryChanged(int index,int value);

public slots:
	void sendDataPushButtonSlot();
	void layerActiveRadioButtonClickedSlot(layerClass*,bool);
};
#endif

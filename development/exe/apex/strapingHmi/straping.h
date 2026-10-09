#ifndef STRAPING_H
#define STRAPING_H

#include <QtGui>
#include <QMainWindow>
#include <QSplitter>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QScrollArea>

#include "shG.h"
#include "../common/btns.h"
#include "move.h"
#include "ps.h"
#include "dnp.h"
class strapingClass : public QWidget
{
	tcpServerClass									*tcpServer;
	//
	defineNewPacketClass						*defineNewPacketDlg;
	//
	float														sbPosPacketLength;
	float														sbPosPacketWidth;
	float														sbPacketHeight;
	float														packetPosPacketLength;
	float														packetPosPacketWidth;
	float														packetPosPacketHeight;
	float														lastWeight;
	int															noOfWoods;
	//main
	myQPushButton										*autoPushbutton;
	QLabel													*stopLabel_1_18;

	QPushButton											*newPacketInShipingBasketStationButton;
	QPushButton											*newPacketInPacketStationButton;
	QPushButton											*newPacketInImportStationButton;
	QPushButton											*newSBInImportStationButton;
	QPushButton											*changeLengthPushButton;
	myQPushButton										*startStrapingPushButton;
	myQPushButton										*r5ToR6PushButton;
	myQPushButton										*r5ToR62PacketsPushButton;
	myQPushButton										*resetPacketQPushButton;
	myQPushButton										*openR6RollersPushButton;
	myQPushButton										*closeR6RollersPushButton;
	QLabel													*r5Tor6Label;
	QLabel													*readyForStrapping;
	int															plcLength;
	QLabel													*lengthLabel;
	QLabel													*widthLabel;
	QLabel													*heightLabel;

	QLabel													*packetOnR5Label;
	QLabel													*sbOnR5Label;
	QLabel													*packetOnR6Label;
	QLabel													*sbOnR6Label;
	QLabel													*packetOnR8Label;
	//
	QLabel													*ph1Label;//photo sensor on quadruple conveyor
	QLabel													*ls2Label;//limit switch on quadruple conveyor
	QLabel													*checkWoodsPositionLabel;

	QLabel													*noOfWoodsLabel;
	//tension
	QSpinBox												*dieTensionSpinBox;
	QSpinBox												*percendageTensionSpinBox;
	//wood
	QLabel													*woods4HorizontalLength;
	QLabel													*woods4VerticalLengthPacket1;
	QLabel													*woods4VerticalLengthPacket2;
	QLabel													*woods2HorizontalLength;
	QLabel													*woodOffsetLabel;
	QSpinBox												*woodOffsetSpinBox;
	//fork
	myQPushButton										*takePacketQPushButton;
	//alarms
	QLabel													*signal_00_00;
	QLabel													*signal_00_01;
	QLabel													*signal_00_02;
	QLabel													*signal_00_03;
	QLabel													*signal_00_04;
	QLabel													*signal_00_05;
	QLabel													*signal_00_06;
	QLabel													*signal_00_07;
	QLabel													*signal_00_08;
	QLabel													*signal_00_09;
	QLabel													*signal_00_10;
	QLabel													*signal_00_11;
	QLabel													*signal_00_12;
	QLabel													*signal_00_13;
	QLabel													*signal_00_14;
	QLabel													*signal_00_15;
	QLabel													*signal_00_16;
	QLabel													*signal_00_17;
	QLabel													*signal_00_18;
	QLabel													*signal_00_19;
	//
Q_OBJECT
	QVector<moveConveyorClass*>			areaButtonsVector;
public:
	strapingClass(tcpServerClass *tcpServer_,QWidget *p);
	defineNewPacketClass *getDefineNewPacketDlg();
	void plcMemoryN160Changed(int index,int value);
	void plcMemoryN162Changed(int index,int value);
	void plcFloatMemoryChanged(int index,float value);
	void hideButtons();
	void showButtons();
	public slots:
	void changeLengthPushButtonSlot();
	//r5
	void packetOnR5PushButtonOnSlot();
	void sbOnR5PushButtonOnSlot();
	void packetOnR5PushButtonOffSlot();
	void sbOnR5PushButtonOffSlot();
	//r6
	void packetOnR6PushButtonOnSlot();
	void sbOnR6PushButtonOnSlot();
	void packetOnR6PushButtonOffSlot();
	void sbOnR6PushButtonOffSlot();
	//r8
	void packetOnR8PushButtonOnSlot();
	void packetOnR8PushButtonOffSlot();
	//
	void resetPacketSlot();
	void dieTensionSpinBoxSlot(int dieNum);
	void percendageTensionSpinBoxSlot(int percentNum);
	void woodOffsetSpinBoxValueChangedSlot(int off);
	//
	void newPacketInShipingBasketStationButtonSlot();
	void newPacketInPacketStationButtonSlot();
	void newPacketInImportStationButtonSlot();
	void newSBInImportStationButtonSlot();

};
#endif

#ifndef PRES_H
#define PRES_H

#include <QtGui>
#include <QMainWindow>
#include <QList>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>

#include "ehG.h"
#include "../common/apexG.h"
class positionResetClass:public QGroupBox
{
	Q_OBJECT
	int												plcNoBasketCommand;
	int												plcFullBasketCommand;
	int												plcEmptyBasketCommand;
	int												activeCommand;
	QRadioButton							*noBasketRadioButton;
	QRadioButton							*fullBasketRadioButton;
	QRadioButton							*emptyBasketRadioButton;
	QButtonGroup							*buttonGroup;
	bool											isPosEnabled;
	//
	positionResetClass				*abovePosition;
public:
	positionResetClass(QString title,int plcNoBasketCommand_,int plcFullBasketCommand_,int plcEmptyBasketCommand_);
	void setAbovePosition(positionResetClass* a);
	void initialize();
	void disable();
	void enable();
	int getActiveCommand();
	void showAll();
	void hideNoBasket();
	void hideFullBasket();
#ifdef EMULATE
	bool getIsNoBasket();
	bool getIsFullBasket();
	bool getIsEmptyBasket();
#endif
	public slots:								
    void buttonGroupButtonReleased(int id);
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////mainPositionResetClass////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class mainPositionResetClass:public QDialog 				
{
	QTimer										sendToPlcTimer;
	tcpServerClass						*tcpServer;
	positionResetClass				*firstLevelPosition;
	positionResetClass				*secondLevelPosition;
	positionResetClass				*thirdLevelPosition;
public:										
	Q_OBJECT								
public:										
	mainPositionResetClass(tcpServerClass *tcpServer_,QVector<int> plcCommands,QString title,QWidget *p);
	positionResetClass* getFirstLevelPosition();
	positionResetClass* getSecondLevelPosition();
	positionResetClass* getThirdLevelPosition();
	void								showAll();
	void								hideNoBasket();
	void								hideFullBasket();
public slots:								
    void okButtonClickedSlot();
    void cancelButtonClickedSlot();
private:
};											
#endif

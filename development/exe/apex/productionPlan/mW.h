#ifndef MW_H
#define MW_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSplitter>
#include <QInputDialog>
#include <QToolBar>

#include "ppG.h"
#include "pass.h"
#include "bst.h"
#include "ppl.h"
#include "op.h"
#include "he.h"
#include "ovGL.h"
class mainWindowClass : public QMainWindow
{
	QTimer														mainTimer;
	QRadioButton											*press1RadioBtn;
	QRadioButton											*press2RadioBtn;
	QTimer														checkDatabaseTimer;
	QTimer														plcHartBitTimer;
	QVBoxLayout												*mainLayout;
#ifdef TUX_PLC_COMMS
	tuxipServerClass									*tuxipServer;
#endif
	tcpServerClass										*tcpServer;
	QDialog														*tcpDialog;
	//
	basketStackingClass								*basketStacking;
	profileProductionLayoutClass			*profileProductionLayout;
	helpClass													*helpDlg;
	ovenGroupLayoutClass							*oven;
	//
	int																plcHartBitCounter;
	//
Q_OBJECT
public:
	mainWindowClass();
	~mainWindowClass();
public slots:
	void mainTimerSlot();
	void checkDatabaseTimerSlot();
	void plcHartBitTimerSlot();
	void productionChangedSlot(bool needsReloadFromDatabase);
	void productionChangedFromOvenLayoutSlot();
	void f5Slot();
	void optionsSlot();
	void keyLeftSlot();
	void keyRightSlot();
	void keyDelSlot();
	void keyPlusSlot();
	void keyNSlot();
	void keyPSlot();
	void showPlcComsSlot();
	void helpActionSlot();
	void basketSelectedSlot(basketClass* b);
	void productionProfileSelectedSlot(profileProductionClass* p);
	void signalFromPlcSlot(QString table,int index,QString value);
	void signalFromTuxPlcSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();
	void qtAboutSlot();
	void press1RadioBtnClickedSlot(bool checked);
	void press2RadioBtnClickedSlot(bool checked);
};
#endif

#ifndef LOGW_H
#define LOGW_H
#include "serv.h"
#include "Gsw.h"
class logWidgetClass : public QWidget
{
	QString												dtLogDbName;
	QString												plcIp;
	QString												plcTable;
	int														plcIndex;
	int														plcBit;
	QString												machine;
	QString												operation;
	int														durationToShow;
	int														whenOnIsOk;
	//
	QLabel												*activeLabel;
	//
	int														machinesTid;
	QTableWidget									*table;	
	QStringList										reasonsList;
	QSignalMapper									*reasonsComboMapper;
	QVector<QComboBox*>						reasonsComboV;

Q_OBJECT								
public:										
	logWidgetClass(QString dtLogDbName_,QString plcIp_,QString plcTable_,int plcIndex_,int plcBit_,QString machine_,QString operation_,int durationToShow_,bool whenOnIsOk_=true,QWidget *p=0);
	~logWidgetClass();
	//
	QString getDisplayText();
	int getMachinesTid();
	void updateFromDataBase();
	void updateGui();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:	
	void updtateFromDatabaseBtnSlot();
	void updateGuiTimerSlot();
	void reasonsComboMapperSlot(int comboIndex);
	void itemChangedSlot(QTableWidgetItem *item);
};											
#endif

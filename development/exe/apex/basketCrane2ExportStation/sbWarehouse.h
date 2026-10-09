#ifndef sbWarehouse_H
#define sbWarehouse_H
#include "serv.h"
#include "../sbWarehouse/expWdg.h"
#include "hmiGuiWidgetsLib.h"
class sbWarehouseClass : public QWidget
{
	tuxipClass														*tuxipConnection;
	QSpinBox															*sbSpinBox;
	QLabel																*sbInfoLabel;
	QSpinBox															*ticketSpinBox;
	QLabel																*ticketInfoLabel;
	QPushButton														*okBtn;
	exportStationWidgetClass							*exportFront;
	QSerialPort														*serialPort;
	Q_OBJECT
public:
	sbWarehouseClass(tuxipClass *tuxipConnection_,QWidget *p);
	~sbWarehouseClass();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
private:
	void updateGui();
public slots:
	void sbSpinBoxChangedSlot(int b);
	void ticketSpinBoxChangedSlot(int b);
	void okButtonClicledSlot();
	void readyReadSerialPortSlot();
};
#endif

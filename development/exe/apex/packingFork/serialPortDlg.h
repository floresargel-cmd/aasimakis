#ifndef serialPortDlg_H
#define serialPortDlg_H
class serialPortDlgClass;
#include "Gf.h"
#include "serv.h"
class serialPortDlgClass : public QDialog
{
	tuxipClass									*forkTuxip;
	tuxipClass									*oldStrappingTux;
	tuxipClass									*tuxip;
	QSerialPort									*serialPort;
	QTableWidget								*logTable;
	QTimer											*readTimer;
	QString											serialData;
public:
	enum typeEnum
	{
		none=0,
		formingStation1,
		formingStation2,
		strappingOldExit,
		strappingNewExit,
		inputOutputStation,
	};
	typeEnum										type=typeEnum::none;
	Q_OBJECT
public:
	serialPortDlgClass(tuxipClass *forkTuxip_,tuxipClass *oldStrappingTux_,QString comStr,QWidget *p);
	~serialPortDlgClass();
	void setTypeEnum(serialPortDlgClass::typeEnum type_);
	serialPortDlgClass::typeEnum getTypeEnum();
	QString	getName();
	void log(QString str);
	void newScan(long scanId);
public:
public slots:
	void mainTimerTriggeredSlot();
	void readTimerTriggeredSlot();
	void errorOccurredSlot(QSerialPort::SerialPortError serialPortError);
	void readyReadSerialPortSlot();
};
#endif

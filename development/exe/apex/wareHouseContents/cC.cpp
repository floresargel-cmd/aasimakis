#include "cC.h"
cellContClass::cellContClass(int cellNumber_,QWidget *p):QDialog(p)
{
	setMinimumWidth(200);
	cellNumber=cellNumber_;
	setWindowTitle(tr("Cell %1 properies").arg(cellNumber));
	QGridLayout *mainLayout=new QGridLayout();setLayout(mainLayout);
	//cell number
	mainLayout->addWidget(new QLabel(tr("Cell:")),0,0);
	mainLayout->addWidget(new QLabel(tr("%1").arg(cellNumber)),0,1);
	//col number
	mainLayout->addWidget(new QLabel(tr("Column:")),1,0);
	mainLayout->addWidget(new QLabel(tr("%1").arg(GETONE(QString("select col from positions where posId=%1").arg(cellNumber)).toString())),1,1);
	//row number
	mainLayout->addWidget(new QLabel(tr("Row:")),2,0);
	mainLayout->addWidget(new QLabel(tr("%1").arg(GETONE(QString("select row from positions where posId=%1").arg(cellNumber)).toString())),2,1);
	//packet number
	mainLayout->addWidget(new QLabel(tr("Packet:")),3,0);
	packetNumberSpinBox=new QSpinBox();mainLayout->addWidget(packetNumberSpinBox,3,1);
	packetNumberSpinBox->setMinimum(0);
	packetNumberSpinBox->setMaximum(INT_MAX);
	packetNumberSpinBox->setValue(GETONE(QString("select packetId from positions where posId=%1").arg(cellNumber)).toInt());
	//ok
	QPushButton *okButton=new QPushButton(tr("Ok"));
	mainLayout->addWidget(okButton,4,0,1,2);
	connect(okButton,SIGNAL(clicked()),this,SLOT(okBtnClickedSlot()));
}
void cellContClass::okBtnClickedSlot()
{
	int packet=packetNumberSpinBox->value();
	if (packet!=0)
	{
		//
		int packetTableId=GETONEATMOST(QString("select tid from packets where packetId=%1").arg(packet)).toInt();
		if (packetTableId==0)
		{
			QMessageBox::critical(this,"Error",QString("The packet %1 is not defined in packets table.").arg(packetTableId));
			return;
		}
		//
		int positionsTableId=GETONEATMOST(QString("select posId from positions where packetId=%1").arg(packet)).toInt();
		if (positionsTableId!=0)
		{
			QMessageBox::critical(this,"Error",QString("The packet %1 is allready defined in posId=%1 in the positions table.").arg(positionsTableId));
			return;
		}
	}
	execQuery(QString("update positions set packetId=%1 where posId=%2").arg(packet).arg(cellNumber),MAIN_DB_CONNECTION);
}

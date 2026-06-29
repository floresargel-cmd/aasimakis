#include "dnp.h"
defineNewPacketClass::defineNewPacketClass(int stationEnum_,int length,int width,int height,int weight,int noOfWoods,QWidget *p):QDialog(p)
{
	setWindowTitle(tr("Define new packet data"));
	setMinimumWidth(250);
	stationEnum=stationEnum_;
	emptyCassettesInWarehouse=GETONE("select count(tid) from positions where packetId=0 and tid>10 and posId not in (select cassette from positions where tid=7 or tid=8 or tid=9)").toInt();
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	//code
	QHBoxLayout *codeLayout=new QHBoxLayout();mainLayout->addLayout(codeLayout);
	codeLayout->addWidget(new QLabel(tr("Packet number")));
	packetCodeSpinBox=new QSpinBox();codeLayout->addWidget(packetCodeSpinBox);
	packetCodeSpinBox->setMinimum(0);
	packetCodeSpinBox->setMaximum(INT_MAX);
	connect(packetCodeSpinBox,SIGNAL(valueChanged(int)),this,SLOT(packetCodeSpinBoxSlot(int)));
	//length
	QHBoxLayout *lengthLayout=new QHBoxLayout();mainLayout->addLayout(lengthLayout);
	lengthLayout->addWidget(new QLabel(tr("Packet length:")));
	packetLengthSpinBox=new QSpinBox();lengthLayout->addWidget(packetLengthSpinBox);
	packetLengthSpinBox->setMinimum(1828);
	packetLengthSpinBox->setMaximum(9144);//7366
	packetLengthSpinBox->setValue(length);
	connect(packetLengthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(packetLengthSpinBoxSlot(int)));
	if ((stationEnum!=TYPE_SB_STATION)&&(stationEnum!=TYPE_IMPORT_SB_STATION))
	{
		//width
		QHBoxLayout *widthLayout=new QHBoxLayout();mainLayout->addLayout(widthLayout);
		widthLayout->addWidget(new QLabel(tr("Packet width:")));
		packetWidthSpinBox=new QSpinBox();widthLayout->addWidget(packetWidthSpinBox);
		packetWidthSpinBox->setMinimum(1);
		packetWidthSpinBox->setMaximum(630);
		packetWidthSpinBox->setValue(width);
		connect(packetWidthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(packetWidthSpinBoxSlot(int)));
		//height
		QHBoxLayout *heightLayout=new QHBoxLayout();mainLayout->addLayout(heightLayout);
		heightLayout->addWidget(new QLabel(tr("Packet height:")));
		packetHeightSpinBox=new QSpinBox();heightLayout->addWidget(packetHeightSpinBox);
		packetHeightSpinBox->setMinimum(1);
		packetHeightSpinBox->setMaximum(630);
		packetHeightSpinBox->setValue(height);
		connect(packetHeightSpinBox,SIGNAL(valueChanged(int)),this,SLOT(packetHeightSpinBoxSlot(int)));
	}
	//height
	QHBoxLayout *weightLayout=new QHBoxLayout();mainLayout->addLayout(weightLayout);
	weightLayout->addWidget(new QLabel(tr("Packet weight:")));
	packetWeightSpinBox=new QSpinBox();weightLayout->addWidget(packetWeightSpinBox);
	packetWeightSpinBox->setMinimum(1);
	packetWeightSpinBox->setMaximum(900);
	packetWeightSpinBox->setValue(weight);
	//woods
	if ((stationEnum!=TYPE_SB_STATION)&&(stationEnum!=TYPE_IMPORT_SB_STATION))
	{
		QGroupBox *woodsGrouBox=new QGroupBox(tr("Woods"));mainLayout->addWidget(woodsGrouBox);
		QHBoxLayout *woodsGrouBoxLayout=new QHBoxLayout();woodsGrouBox->setLayout(woodsGrouBoxLayout);
		woodsGrouBoxLayout->addWidget(woods2RadioButton=new QRadioButton(tr("2")));connect(woods2RadioButton,SIGNAL(clicked()),this,SLOT(woods2RadioButtonSlot()));
		woodsGrouBoxLayout->addWidget(woods3RadioButton=new QRadioButton(tr("3")));connect(woods3RadioButton,SIGNAL(clicked()),this,SLOT(woods3RadioButtonSlot()));
		woodsGrouBoxLayout->addWidget(woods4RadioButton=new QRadioButton(tr("4")));connect(woods4RadioButton,SIGNAL(clicked()),this,SLOT(woods4RadioButtonSlot()));
		woodsGrouBoxLayout->addWidget(woods5RadioButton=new QRadioButton(tr("5")));connect(woods5RadioButton,SIGNAL(clicked()),this,SLOT(woods5RadioButtonSlot()));
		woodsGrouBoxLayout->addWidget(woods6RadioButton=new QRadioButton(tr("6")));connect(woods6RadioButton,SIGNAL(clicked()),this,SLOT(woods6RadioButtonSlot()));
		woodsGrouBoxLayout->addWidget(woods7RadioButton=new QRadioButton(tr("7")));connect(woods7RadioButton,SIGNAL(clicked()),this,SLOT(woods7RadioButtonSlot()));
		if (noOfWoods==3)woods3RadioButton->setChecked(true);
		else if (noOfWoods==4)woods4RadioButton->setChecked(true);
		else if (noOfWoods==5)woods5RadioButton->setChecked(true);
		else if (noOfWoods==6)woods6RadioButton->setChecked(true);
		else if (noOfWoods==7)woods7RadioButton->setChecked(true);
	}
#ifdef USE_STORAGE_TYPE
	//storagingType
	QGroupBox *storagingTypeGroupBox=new QGroupBox(tr("Storaging type"));mainLayout->addWidget(storagingTypeGroupBox);
	QHBoxLayout *storagingTypeGroupBoxLayout=new QHBoxLayout();storagingTypeGroupBox->setLayout(storagingTypeGroupBoxLayout);
	storagingTypeGroupBoxLayout->addWidget(shortTermStorageRadioButton=new QRadioButton(tr("Short term")));connect(shortTermStorageRadioButton,SIGNAL(clicked()),this,SLOT(shortTermStorageRadioButtonSlot()));
	storagingTypeGroupBoxLayout->addWidget(midTermStorageRadioButton=new QRadioButton(tr("Mid term")));connect(midTermStorageRadioButton,SIGNAL(clicked()),this,SLOT(midTermStorageRadioButtonSlot()));
	storagingTypeGroupBoxLayout->addWidget(longTermStorageRadioButton=new QRadioButton(tr("Long term")));connect(longTermStorageRadioButton,SIGNAL(clicked()),this,SLOT(longTermStorageRadioButtonSlot()));
#endif
	//toWarehouse
	QHBoxLayout *toWarehouseLayout=new QHBoxLayout();mainLayout->addLayout(toWarehouseLayout);
	packetToWareHouseCheckBox=new QCheckBox((emptyCassettesInWarehouse>0?tr("Packet to warehouse (%1 cassettes empty).").arg(emptyCassettesInWarehouse):tr("Warehouse is full.")));toWarehouseLayout->addWidget(packetToWareHouseCheckBox);
	connect(packetToWareHouseCheckBox,SIGNAL(clicked()),this,SLOT(packetToWareHouseCheckBoxSlot()));
	if (emptyCassettesInWarehouse>0)
		packetToWareHouseCheckBox->setChecked(true);
	else
	{
		packetToWareHouseCheckBox->setEnabled(false);
		packetToWareHouseCheckBox->setChecked(false);
	}
	//buttons
	QHBoxLayout *btnsLayout=new QHBoxLayout();mainLayout->addLayout(btnsLayout);
	okPushbutton=new QPushButton(tr("Ok"));btnsLayout->addWidget(okPushbutton);
	connect(okPushbutton,SIGNAL(clicked()),this,SLOT(okPushbuttonSlot()));
	QPushButton *cancelPushButton=new QPushButton(tr("Cancel"));btnsLayout->addWidget(cancelPushButton);
	connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(reject()));
	//
	checkToWareHouse();
	checkEnableOkButton();
}
void defineNewPacketClass::messageFromBarcode(QString msg)
{
	int code=msg.toInt();
	if (code>0)
		packetCodeSpinBox->setValue(code);
}
void defineNewPacketClass::checkToWareHouse()
{
	if (emptyCassettesInWarehouse==0)
		return;
	if (((stationEnum==TYPE_PACKET_STATION)||(stationEnum==TYPE_IMPORT_PACKET_STATION)))
	{
		packetToWareHouseCheckBox->setEnabled(true);
		int a=packetLengthSpinBox->value();
		int b=packetWidthSpinBox->value();
		int c=packetHeightSpinBox->value();
		if ((packetLengthSpinBox->value()>bigLength)||(packetLengthSpinBox->value()<1828)||(packetWidthSpinBox->value()>558)||(packetHeightSpinBox->value()>482))
		{
			packetToWareHouseCheckBox->setChecked(false);
			packetToWareHouseCheckBox->setEnabled(false);
		}		
		//
		//if ( (woods2RadioButton->isChecked()&&(packetLengthSpinBox->value()>3400)) ||   (woods4RadioButton->isChecked()&&(packetLengthSpinBox->value()>6200))  )
		//{
		//		packetToWareHouseCheckBox->setChecked(false);
		//		packetToWareHouseCheckBox->setEnabled(false);
		//}
		int noOfWoods=0;
		if (packetLengthSpinBox->value()>1829)
			noOfWoods=2;
		if (packetLengthSpinBox->value()>3400)
			noOfWoods=3;
		if (packetLengthSpinBox->value()>4900)
			noOfWoods=4;
		if (packetLengthSpinBox->value()>=6200)
			noOfWoods=5;
		if ((noOfWoods==2)&&(!woods2RadioButton->isChecked()))
		{
			packetToWareHouseCheckBox->setChecked(false);
			packetToWareHouseCheckBox->setEnabled(false);
		}
		if ((noOfWoods==3)&&(!woods3RadioButton->isChecked()))
		{
			packetToWareHouseCheckBox->setChecked(false);
			packetToWareHouseCheckBox->setEnabled(false);
		}
		if ((noOfWoods==4)&&(!woods4RadioButton->isChecked()))
		{
			packetToWareHouseCheckBox->setChecked(false);
			packetToWareHouseCheckBox->setEnabled(false);
		}
		if ((noOfWoods==5)&&(!woods5RadioButton->isChecked()))
		{
			packetToWareHouseCheckBox->setChecked(false);
			packetToWareHouseCheckBox->setEnabled(false);
		}


		if ((woods6RadioButton->isChecked())||(woods7RadioButton->isChecked()))
		{
			packetToWareHouseCheckBox->setChecked(false);
			packetToWareHouseCheckBox->setEnabled(false);
		}
	}
	else//TYPE_SB_STATION||TYPE_IMPORT_SB_STATION
	{
		packetToWareHouseCheckBox->setEnabled(true);
		if (packetLengthSpinBox->value()>bigLength)
		{
			packetToWareHouseCheckBox->setChecked(false);
			packetToWareHouseCheckBox->setEnabled(false);
		}		
	}
}
void defineNewPacketClass::packetCodeSpinBoxSlot(int value)
{
#ifdef USE_STORAGE_TYPE
	if ((stationEnum==TYPE_SB_STATION)||(stationEnum==TYPE_IMPORT_SB_STATION))
	{
	QString priorytyStr=GETONEATMOST(QString("SELECT top 1 typecode FROM [epics].[foy].[RackDetail] inner join [epics].[foy].[SO] on [epics].[foy].[RackDetail].sonum = epics.foy.so.sonum  where rackid = %1").arg(value),EPICS_DB_CONNECTION).toString();
		int priorytyInt=GETONEATMOST(QString("select priority from storagePriority where orderType='%1'").arg(priorytyStr)).toInt();
		shortTermStorageRadioButton->setAutoExclusive(false);
		midTermStorageRadioButton->setAutoExclusive(false);
		longTermStorageRadioButton->setAutoExclusive(false);
		shortTermStorageRadioButton->setChecked(false);
		midTermStorageRadioButton->setChecked(false);
		longTermStorageRadioButton->setChecked(false);
		shortTermStorageRadioButton->setAutoExclusive(true);
		midTermStorageRadioButton->setAutoExclusive(true);
		longTermStorageRadioButton->setAutoExclusive(true);
		if (priorytyInt==1)
			shortTermStorageRadioButton->setChecked(true);
		else if (priorytyInt==2)
			midTermStorageRadioButton->setChecked(true);
		else if (priorytyInt==3)
			longTermStorageRadioButton->setChecked(true);
	}
	else
	{
		QString priorytyStr=GETONEATMOST(QString("select top 1	typecode from epics.foy.ticket inner join epics.foy.so on epics.foy.ticket.sonum = epics.foy.so.sonum where ticketnum = %1").arg(value),EPICS_DB_CONNECTION).toString();
		int priorytyInt=GETONEATMOST(QString("select priority from storagePriority where orderType='%1'").arg(priorytyStr)).toInt();
		shortTermStorageRadioButton->setAutoExclusive(false);
		midTermStorageRadioButton->setAutoExclusive(false);
		longTermStorageRadioButton->setAutoExclusive(false);
		shortTermStorageRadioButton->setChecked(false);
		midTermStorageRadioButton->setChecked(false);
		longTermStorageRadioButton->setChecked(false);
		shortTermStorageRadioButton->setAutoExclusive(true);
		midTermStorageRadioButton->setAutoExclusive(true);
		longTermStorageRadioButton->setAutoExclusive(true);
		if (priorytyInt==1)
			shortTermStorageRadioButton->setChecked(true);
		else if (priorytyInt==2)
			midTermStorageRadioButton->setChecked(true);
		else if (priorytyInt==3)
			longTermStorageRadioButton->setChecked(true);
	}
	checkEnableOkButton();
#endif
}
void defineNewPacketClass::packetLengthSpinBoxSlot(int value)
{
	checkToWareHouse();
}
void defineNewPacketClass::packetWidthSpinBoxSlot(int value)
{
	checkToWareHouse();
}
void defineNewPacketClass::packetHeightSpinBoxSlot(int value)
{
	checkToWareHouse();
}
void defineNewPacketClass::packetToWareHouseCheckBoxSlot()
{
	checkEnableOkButton();
}
void defineNewPacketClass::checkEnableOkButton()
{
	bool storageTimeChecked=true;
#ifdef USE_STORAGE_TYPE
	if (packetToWareHouseCheckBox->isChecked())
	{
		if ( (!shortTermStorageRadioButton->isChecked())&&(!midTermStorageRadioButton->isChecked())&&(!longTermStorageRadioButton->isChecked()))
			storageTimeChecked=false;
	}
#endif
	if ((stationEnum!=TYPE_SB_STATION)&&(stationEnum!=TYPE_IMPORT_SB_STATION))
	{
		bool dimentionsChecked=true;
		if ( (packetLengthSpinBox->value()<1828)||(packetWidthSpinBox->value()<=1)||(packetHeightSpinBox->value()<=1))
				dimentionsChecked=false;
		bool noOfWoodsCheckedChecked=false;
		if (woods2RadioButton->isChecked())	noOfWoodsCheckedChecked=true;
		else if (woods3RadioButton->isChecked())	noOfWoodsCheckedChecked=true;
		else if (woods4RadioButton->isChecked())	noOfWoodsCheckedChecked=true;
		else if (woods5RadioButton->isChecked())	noOfWoodsCheckedChecked=true;
		else if (woods6RadioButton->isChecked())	noOfWoodsCheckedChecked=true;
		else if (woods7RadioButton->isChecked())	noOfWoodsCheckedChecked=true;
		if (storageTimeChecked&&noOfWoodsCheckedChecked&&dimentionsChecked)
			okPushbutton->setEnabled(true);
		else
			okPushbutton->setEnabled(false);
	}
	else
	{
		if (storageTimeChecked)
			okPushbutton->setEnabled(true);
		else
			okPushbutton->setEnabled(false);
	}

}
void defineNewPacketClass::woods2RadioButtonSlot()
{
	checkToWareHouse();
	checkEnableOkButton();
}
void defineNewPacketClass::woods3RadioButtonSlot()
{
	checkToWareHouse();
	checkEnableOkButton();
}
void defineNewPacketClass::woods4RadioButtonSlot()
{
	checkToWareHouse();
	checkEnableOkButton();
}
void defineNewPacketClass::woods5RadioButtonSlot()
{
	checkToWareHouse();
	checkEnableOkButton();
}
void defineNewPacketClass::woods6RadioButtonSlot()
{
	checkToWareHouse();
	checkEnableOkButton();
}
void defineNewPacketClass::woods7RadioButtonSlot()
{
	checkToWareHouse();
	checkEnableOkButton();
}
void defineNewPacketClass::shortTermStorageRadioButtonSlot()
{
#ifdef USE_STORAGE_TYPE
	checkEnableOkButton();
#endif
}
void defineNewPacketClass::midTermStorageRadioButtonSlot()
{
#ifdef USE_STORAGE_TYPE
	checkEnableOkButton();
#endif
}
void defineNewPacketClass::longTermStorageRadioButtonSlot()
{
#ifdef USE_STORAGE_TYPE
	checkEnableOkButton();
#endif
}
void defineNewPacketClass::printNumber(int c)
{
	if (c<=0)
		return;
	QPrinter printer(QPrinterInfo::defaultPrinter());
	QPainter painter(&printer);
	painter.setPen(Qt::black);
	painter.setFont(QFont("helvetica",120));
	painter.drawText(100,350,QString("%1").arg(c));
}
void defineNewPacketClass::okPushbuttonSlot()
{
	int packet=packetCodeSpinBox->value();
	int woods=0;
	if ((stationEnum!=TYPE_SB_STATION)&&(stationEnum!=TYPE_IMPORT_SB_STATION))
	{	
		if (woods2RadioButton->isChecked())woods=2;
		else if (woods3RadioButton->isChecked())woods=3;
		else if (woods4RadioButton->isChecked())woods=4;
		else if (woods5RadioButton->isChecked())woods=5;
		else if (woods6RadioButton->isChecked())woods=6;
		else if (woods7RadioButton->isChecked())woods=7;
	}
	int length=packetLengthSpinBox->value();
	int width=0;
	int height=0;
	if ((stationEnum!=TYPE_SB_STATION)&&(stationEnum!=TYPE_IMPORT_SB_STATION))
	{	
		width=packetWidthSpinBox->value();
		height=packetHeightSpinBox->value();
	}
	int weight=packetWeightSpinBox->value();
	int sb=((stationEnum==TYPE_SB_STATION)||(stationEnum==TYPE_IMPORT_SB_STATION))?PACKET_WITH_SB:PACKET_WITHOUT_SB;
	int destination=packetToWareHouseCheckBox->isChecked()?PACKET_DESTINATION_WAREHOUSE:PACKET_DESTINATION_OUT;
	int storagingType=STORAGE_TYPE_SHORT_TERM;
#ifdef USE_STORAGE_TYPE
	if (midTermStorageRadioButton->isChecked())
		storagingType=STORAGE_TYPE_MID_TERM;
	else if (longTermStorageRadioButton->isChecked())
		storagingType=STORAGE_TYPE_LONG_TERM;
#endif
	//posInWareHouse
	int posIdInWareHouse=GETFIRST(QString("select posId from positions where packetId=%1").arg(packet)).toInt();
	if (posIdInWareHouse!=0)
	{
		QMessageBox::critical(this,tr("Error defining packet"),tr("The packet %1 is in the position %2 of the warehouse").arg(packet).arg(posIdInWareHouse));
		return;
	}
	//printNumber(packet);
	//tidNumberInPacketsTable
	int tidNumberInPacketsTable=GETFIRST(QString("select tid from packets where packetId=%1").arg(packet)).toInt();
	if (tidNumberInPacketsTable==0)
	{
		apexExecQuery(QString("insert into packets (packetId,woods,length,width,height,weight,sb,destination,storagingType) values(%1,%2,%3,%4,%5,%6,%7,%8,%9)")
			.arg(packet)
			.arg(woods)
			.arg(length)
			.arg(width)
			.arg(height)
			.arg(weight)
			.arg(sb)
			.arg(destination)
			.arg(storagingType)
			);
	}
	else
	{
		apexExecQuery(QString("update packets set packetId=%1,woods=%2,length=%3,width=%4,height=%5,weight=%6,sb=%7,destination=%8,storagingType=%9 where tid=%10")
			.arg(packet)
			.arg(woods)
			.arg(length)
			.arg(width)
			.arg(height)
			.arg(weight)
			.arg(sb)
			.arg(destination)
			.arg(storagingType)
			.arg(tidNumberInPacketsTable)
			);
	}
	QString expPosId="";
	if (stationEnum==TYPE_SB_STATION)
		expPosId="4001";
	else if (stationEnum==TYPE_PACKET_STATION)
		expPosId="4002";
	else if (stationEnum==TYPE_IMPORT_PACKET_STATION)
		expPosId="4005";
	else if (stationEnum==TYPE_IMPORT_SB_STATION)
		expPosId="4005";
	if (packetLengthSpinBox->value()>=1828)
		apexExecQuery(QString("update positions set packetId=%1 where posId=%2").arg(packet).arg(expPosId));
	QDialog::accept();
}

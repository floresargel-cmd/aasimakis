#include "pch.h"
#include "craneWdg.h"
craneWidgetClass::craneWidgetClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip_,QWidget *conv,bool useTranslation,craneClass *crane):QWidget(crane)
{
	tuxip=tuxip_;
	QVBoxLayout *topLayout=new QVBoxLayout();setLayout(topLayout);topLayout->setMargin(1);
	QSplitter *centralSplitter=new QSplitter(Qt::Vertical);topLayout->addWidget(centralSplitter);
	centralSplitter->setStyleSheet(splitterVerticalStyle);
	QWidget *myWidget=new QWidget();
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(1);myWidget->setLayout(mainLayout);
	QGroupBox *craneGroupBox=new QGroupBox(tr("Crane"));mainLayout->addWidget(craneGroupBox);
	QVBoxLayout *craneGroupBoxLayout=new QVBoxLayout();craneGroupBox->setLayout(craneGroupBoxLayout);
	QGridLayout *posLayout=new QGridLayout();posLayout->setMargin(1);craneGroupBoxLayout->addLayout(posLayout);
	posLayout->addWidget(new QLabel(tr("Position[mm]")),0,0);
	posLayout->addWidget(new QLabel(tr("Target[mm]")),0,1);
	posLayout->addWidget(xPosLabel=new QLabel(tr("")),1,0);xPosLabel->setToolTip(tr("Crane X position in mm"));
	posLayout->addWidget(zPosLabel=new QLabel(tr("")),2,0);zPosLabel->setToolTip(tr("Crane Z position in mm"));
	posLayout->addWidget(xPosTargetLabel=new QLabel(tr("")),1,1);xPosTargetLabel->setToolTip(tr("Crane X target in mm"));
	posLayout->addWidget(zPosTargetLabel=new QLabel(tr("")),2,1);zPosTargetLabel->setToolTip(tr("Crane Z target in mm"));

	activeIconsVector<<new activeIconClass("bRobotToPcI",21,21,QPixmap::fromImage(QImage(":/ok.png")),QPixmap::fromImage(QImage(":/notOk.png")),"","","","",this);
	activeIconsVector.last()->setAlignment(Qt::AlignCenter);
	craneGroupBoxLayout->addWidget(activeIconsVector.last());
	//craneGroupBoxLayout->addStretch(1);

	logGuiName=QString("crane");
	logGuiList=new QListWidget();logGuiList->setMinimumHeight(20);craneGroupBoxLayout->addWidget(logGuiList);
	QPushButton *popUpListBtn=new QPushButton(QIcon(":/popUp.png"),"");
	logGuiList->setCornerWidget(popUpListBtn);
	connect(popUpListBtn,SIGNAL(clicked()),this,SLOT(popUpList()));
	connect(logGuiList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(popUpList()));
	loadFromDataBase();
	QPushButton *loadHmiBtn=new QPushButton(QIcon(":/androidRed.png"),tr("HMI"));craneGroupBoxLayout->addWidget(loadHmiBtn);
	connect(loadHmiBtn,SIGNAL(released()),this,SLOT(loadHmiDlgSlot()));

	QWidget *convWidget=new QWidget();
	QVBoxLayout *convWidgetLayout=new QVBoxLayout();convWidgetLayout->setMargin(1);
	convWidget->setLayout(convWidgetLayout);
	convWidgetLayout->addWidget(conv);
	//
	QWidget *diePressWidget=new QWidget();
	QVBoxLayout *diePressTopLayout=new QVBoxLayout();diePressTopLayout->setMargin(1);
	diePressWidget->setLayout(diePressTopLayout);
	QGroupBox *diePressLGroupBox=new QGroupBox(tr("Die press"));diePressTopLayout->addWidget(diePressLGroupBox);diePressTopLayout->addStretch(1);
	QGridLayout *diePressLGroupBoxL=new QGridLayout();diePressLGroupBox->setLayout(diePressLGroupBoxL);
	int row=0;
	diePressLGroupBoxL->addWidget(new QLabel(tr("Diameter")),row,0);
	diePressLGroupBoxL->addWidget(new QLabel(tr("Width")),row,1);
	row++;
	diePressDiameterCombo=new QComboBox();
	diePressDiameterCombo->addItem(tr("254 mm"));
	diePressDiameterCombo->addItem(tr("304 mm"));
	diePressLGroupBoxL->addWidget(diePressDiameterCombo,row,0);

	diePressTypeCombo=new QComboBox();
	diePressTypeCombo->addItem(tr("178 mm"));
	diePressTypeCombo->addItem(tr("228 mm"));
	diePressLGroupBoxL->addWidget(diePressTypeCombo,row,1);
	row++;
	QPushButton *assemblyBtn=new QPushButton(tr("Assembly"));
	diePressLGroupBoxL->addWidget(assemblyBtn,row,0);
	connect(assemblyBtn,SIGNAL(clicked(bool)),this,SLOT(diePressAssemblyBtnClickedSlot()));
	QPushButton *disassemblyBtn=new QPushButton(tr("Disassembly"));
	diePressLGroupBoxL->addWidget(disassemblyBtn,row,1);
	connect(disassemblyBtn,SIGNAL(clicked(bool)),this,SLOT(diePressDisassemblyBtnSlot()));
	//diePressLGroupBoxL->setRowStretch(row++,1);
	//
	centralSplitter->addWidget(myWidget);
	centralSplitter->addWidget(convWidget);
	centralSplitter->addWidget(diePressWidget);
	centralSplitter->setSizes(QList<int>()<<400<<400<<400);
	hmiDlg=new hmiDlgClass(tuxipServer,tuxip,useTranslation,crane);
}
craneWidgetClass::~craneWidgetClass()
{
}

void craneWidgetClass::diePressAssemblyBtnClickedSlot()
{
	tuxip->writeFloat("bRobotToPcF",50,diePressDiameterCombo->currentIndex()==0?254.:304.);
	tuxip->writeInteger("bRobotToPcI",50,diePressTypeCombo->currentIndex()+1);
	tuxip->writeInteger("bRobotToPcI",22,110);
}
void craneWidgetClass::diePressDisassemblyBtnSlot()
{
	tuxip->writeFloat("bRobotToPcF",50,diePressDiameterCombo->currentIndex()==0?254.:304.);
	tuxip->writeInteger("bRobotToPcI",50,diePressTypeCombo->currentIndex()+1);
	tuxip->writeInteger("bRobotToPcI",22,111);
}
void craneWidgetClass::setCraneConveyorClass(craneConveyorClass *c)
{
	hmiDlg->setCraneConveyorClass(c);
}
void craneWidgetClass::loadFromDataBase()
{
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 100 mess,details,type,time from logGui where messenger='%1' order by tid desc").arg(logGuiName),db);
	for (int i=0;i<dataVV.count();i++)
	{
		QListWidgetItem *item=new QListWidgetItem(dataVV[i][0].toString().trimmed());
		item->setToolTip(QString("%1\n%2\n%3").arg(dataVV[i][2].toString().trimmed()).arg(dataVV[i][1].toString().trimmed()).arg(dataVV[i][3].toString().trimmed()));
		if (dataVV[i][2].toString().trimmed()==errorStr)
			item->setForeground(Qt::red);
		else
			item->setForeground(Qt::blue);
		logGuiList->addItem(item);
	}
}
void craneWidgetClass::addMessageToGui(QString mess,QString details,QString type)
{
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 1 mess,details,type,time from logGui where messenger='%1' order by tid desc").arg(logGuiName),db);
	bool isTheSameWithLast=false;
	if (dataVV.count()>0)
	{
		if (  (mess==dataVV[0][0].toString().trimmed())  &&  (details==dataVV[0][1].toString().trimmed())  &&  (type==dataVV[0][2].toString().trimmed())  )
			isTheSameWithLast=true;
	}
	if (!isTheSameWithLast)
	{
		logGui(mess,details,logGuiName,type);
		QListWidgetItem *item=new QListWidgetItem(mess);
		item->setToolTip(QString("%1\n%2\n%3").arg(type).arg(details).arg(timeForLog));
		if (type==errorStr)
			item->setForeground(Qt::red);
		else
			item->setForeground(Qt::blue);
		logGuiList->insertItem(0,item);
	}
	else
	{
		if (logGuiList->count()>0)
		{
			execQuery(QString("update logGui set time='%1' where messenger='%2' and tid in (select max(tid) from logGui where messenger='%2')").arg(timeForLog).arg(logGuiName),db);
			logGuiList->item(0)->setToolTip(QString("%1\n%2\n%3").arg(type).arg(details).arg(timeForLog));
		}
	}
}
void craneWidgetClass::popUpList()
{
	QDialog *dlg=new QDialog(this);
	dlg->setWindowTitle(tr("Crane log"));
	dlg->resize(1100,600);
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(1);dlg->setLayout(mainLayout);
	QTableWidget *logTable=new QTableWidget();mainLayout->addWidget(logTable);
	logTable->setAlternatingRowColors(true);
	logTable->horizontalHeader()->setStretchLastSection(true);
	logTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	logTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	logTable->setColumnCount(4);
	logTable->setHorizontalHeaderLabels(QStringList()<<tr("Message")<<tr("Description")<<tr("type")<<"Time");
	logTable->setWordWrap (true);
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 300 mess,details,type,time from logGui where messenger='%1' order by time desc").arg(logGuiName),db);
	logTable->setRowCount(dataVV.count());
	for (int i=0;i<dataVV.count();i++)
	{
		logTable->setItem(i,0,new QTableWidgetItem(dataVV[i][0].toString().trimmed()));
		logTable->setItem(i,1,new QTableWidgetItem(dataVV[i][1].toString().trimmed()));
		logTable->setItem(i,2,new QTableWidgetItem(dataVV[i][2].toString().trimmed()));
		logTable->setItem(i,3,new QTableWidgetItem(dataVV[i][3].toString().trimmed()));
		if (dataVV[i][2].toString().trimmed()==errorStr)
		{
			logTable->item(i,0)->setBackground(Qt::red);
			logTable->item(i,1)->setBackground(Qt::red);
			logTable->item(i,2)->setBackground(Qt::red);
			logTable->item(i,3)->setBackground(Qt::red);
		}
	}
	logTable->resizeColumnsToContents();
	dlg->exec();
}
void craneWidgetClass::loadHmiDlgSlot()
{
	hmiDlg->exec();
}
void craneWidgetClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==dieIp)
	{
		if(table=="bRobotToPcI")
		{
			for (int i=0;i<activeIconsVector.count();i++)
				activeIconsVector[i]->updateValue(table,index,value.toInt());
		}
		if (table=="bRobotToPcF")
		{
			if (index==0)xPosLabel->setText(QString("%1").arg(value.toFloat()));
			if (index==1)zPosLabel->setText(QString("%1").arg(value.toFloat()));
			if (index==2)xPosTargetLabel->setText(QString("%1").arg(value.toFloat()));
			if (index==3)zPosTargetLabel->setText(QString("%1").arg(value.toFloat()));
		}
		hmiDlg->dataInPlcChanged(ip,table,index,value);
	}
}

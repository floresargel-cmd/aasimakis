#include "cont.h"
///////////////////////////////////////////////////////////////////////////////
basketContentsClass::basketContentsClass(int stationId_,tcpServerClass *tcpServer_,QWidget *w):QWidget(w)
{
	basketTableId=0;
	stationId=stationId_;
	tcpServer=tcpServer_;
	//
	QVBoxLayout *mainLayout=new QVBoxLayout(this);
	setLayout(mainLayout);
	//
	QHBoxLayout *upLayout=new QHBoxLayout();mainLayout->addLayout(upLayout);
	//midLyayout
	QVBoxLayout *upLeftLayout=new QVBoxLayout();upLayout->addLayout(upLeftLayout);
	QVBoxLayout *upRightLayout=new QVBoxLayout();upLayout->addLayout(upRightLayout);
	tableView=new QTableWidget();
	tableView->setAlternatingRowColors(true);
	tableView->horizontalHeader()->setStretchLastSection(true);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->setColumnCount(6);
	QStringList h;h<<tr("Profile")<<tr("Length")<<tr("Quantity")<<tr("SONum")<<tr("SOItemNum")<<tr("Lot");
	tableView->setHorizontalHeaderLabels(h);
	upLeftLayout->addWidget(tableView);

	returnBasketPushButton=new QToolButton(this);
	returnBasketPushButton->setIcon(QIcon(":return.png"));
	returnBasketPushButton->setText(tr("Return basket"));
	returnBasketPushButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	returnBasketPushButton->setStatusTip(tr("Returns current basket"));
	returnBasketPushButton->setToolTip(tr("Returns current basket"));
	returnBasketPushButton->setIconSize(QSize(120,120));
	connect(returnBasketPushButton,SIGNAL(clicked()),this,SLOT(returnBasketPushButtonSlot()));
	//connect(returnBasketPushButton,SIGNAL(released()),this, SLOT(buttonReleasedSlot()));

	returnBasketPushButton->setEnabled(false);
	upRightLayout->addWidget(returnBasketPushButton);
	//
	connect(&mainTimer, SIGNAL(timeout()), this, SLOT(onMainTimer()));
	mainTimer.start(2000);
}
void basketContentsClass::updateTable()
{
	if (basketTableId==0)
	{
//		apexExecQuery(QString("update  basket_location set audittime=CONVERT(datetime,NULL) where basketnum=%1").arg(basketTableId),APEX_DB_CONNECTION);
		tableView->clearContents();
		tableView->setRowCount(0);
		return;
	}
	///
	QVector<QVariant> profilesArray=apexExecQuery(QString("select dienum from basket_details where basketnum=%1 order by sequence desc").arg(basketTableId),APEX_DB_CONNECTION);
	QVector<QVariant> quantityArray=apexExecQuery(QString("select Pc from basket_details where basketnum=%1 order by sequence desc").arg(basketTableId),APEX_DB_CONNECTION);
	QVector<QVariant> lengthArray=apexExecQuery(QString("select PcLen from basket_details where basketnum=%1 order by sequence desc").arg(basketTableId),APEX_DB_CONNECTION);
	QVector<QVariant> SONumArray=apexExecQuery(QString("select SONum from basket_details where basketnum=%1 order by sequence desc").arg(basketTableId),APEX_DB_CONNECTION);
	QVector<QVariant> SOItemArray=apexExecQuery(QString("select SOItemNum from basket_details where basketnum=%1 order by sequence desc").arg(basketTableId),APEX_DB_CONNECTION);
	QVector<QVariant> LotArray=apexExecQuery(QString("select Lot from basket_details where basketnum=%1 order by sequence desc").arg(basketTableId),APEX_DB_CONNECTION);

	tableView->clearContents();
	tableView->setRowCount(profilesArray.count()+1);
	for (int i=0;i<profilesArray.count();i++)
	{
		QLabel *profileWidget=new QLabel(this);


		QVBoxLayout *profileWidgetLayout=new QVBoxLayout();
		profileWidget->setLayout(profileWidgetLayout);
		QToolButton *profileButton=new QToolButton(this);profileButton->setStyleSheet("QToolButton{font-size:16px;text-align:center;}");


		QString iconFileName=QString("C:\\profiles\\%1\\%2.jpg").arg(int(floor(double(profilesArray[i].toInt())/100.)*100.)).arg(profilesArray[i].toInt());
		//QString str=QFileInfo(iconFileName).absoluteFilePath();
		//QIcon icon=QIcon(str);
		//QString dsd=icon.name();
		//bool isNull=icon.isNull();
		profileButton->setIcon(QIcon(iconFileName));
		profileButton->setIconSize(QSize(75,75));

		profileWidgetLayout->addWidget(profileButton,1,Qt::AlignHCenter);
		profileWidgetLayout->addWidget(new QLabel(profilesArray[i].toString()),1,Qt::AlignHCenter);

		tableView->setCellWidget(i,0,profileWidget);
		tableView->setItem(i,1,new QTableWidgetItem(QString().number(lengthArray[i].toInt())));
		tableView->item(i,1)->setTextAlignment(Qt::AlignCenter);
		tableView->setItem(i,2,new QTableWidgetItem(quantityArray[i].toString()));
		tableView->item(i,2)->setTextAlignment(Qt::AlignCenter);
		tableView->setItem(i,3,new QTableWidgetItem(SONumArray[i].toString()));
		tableView->item(i,3)->setTextAlignment(Qt::AlignCenter);
		tableView->setRowHeight(i,125);
		tableView->setItem(i,4,new QTableWidgetItem(QString().number(SOItemArray[i].toInt())));
		tableView->item(i,4)->setTextAlignment(Qt::AlignCenter);
		tableView->setItem(i,5,new QTableWidgetItem(QString().number(LotArray[i].toInt())));
		tableView->item(i,5)->setTextAlignment(Qt::AlignCenter);
	}
	tableView->setRowHeight(profilesArray.count(),30);
	tableView->resizeColumnsToContents();
	//	tableView->resizeRowsToContents();
	tableView->setColumnWidth(0,120);
	tableView->setColumnWidth(1,120);
	tableView->setColumnWidth(2,120);
	tableView->setColumnWidth(3,120);
	tableView->horizontalHeader()->setStretchLastSection(true);
}
void basketContentsClass::updateData(int basTabbleId)
{
	if (basketTableId!=basTabbleId)
	{
		basketTableId=basTabbleId;
		updateTable();
		emit basketChangedSignal();
	}
}
#ifdef PASS_ACTIVE
void basketContentsClass::lockGui()
{
	tableView->setDisabled(true);
}
void basketContentsClass::unLockGui(QString curentUser_)
{
	curentUser=curentUser_;
	tableView->setEnabled(true);
}
#endif
//////////////////////////////////////////////////////slots
void basketContentsClass::onMainTimer()
{
}
void basketContentsClass::plcStatusChanged(int index,int value)
{
	if (index==0)
	{
		if (HAVECOMMONBIT(value,BITS[29]))
			returnBasketPushButton->setEnabled(true);
		else
			returnBasketPushButton->setEnabled(false);
	}
}
void basketContentsClass::returnBasketPushButtonSlot()
{
	int noOfEmptyPositionsInEmptyBuffer=GETONE("select count(pid) from positions where basketTableId=0 and (posNumber=200 or posNumber=230 or posNumber=300 or posNumber=310 or posNumber=320 or posNumber=330 or posNumber=340 or posNumber=350 or posNumber=360 or posNumber=370 or posNumber=380 or posNumber=390)",MAIN_DB_CONNECTION).toInt();
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Warning"));
	msgBox.setText(tr("You are about to return the basket (%1 possitions for empty baskets left).").arg(noOfEmptyPositionsInEmptyBuffer));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	if (noOfEmptyPositionsInEmptyBuffer==0)
	{
		int ans=QMessageBox::question(this,tr("Empty buffer is full"),tr("The empty buffer in the crane area is full, do you want to send the basket out of the system."),QMessageBox::Yes,QMessageBox::No);
		if (ans==QMessageBox::Yes)
		{
			if (stationId==TYPE_OMAV)
				tcpServer->sendString(QString("N100|30|6"));
			if (stationId==TYPE_ASSIM)
				tcpServer->sendString(QString("N100|30|4"));
			tcpServer->sendString(QString("N100|30|0"));
			//clear from basket_details
			apexExecQuery(QString("delete from basket_details where basketnum=%1").arg(basketTableId),APEX_DB_CONNECTION);
		}
	}
	else
	{
		if (stationId==TYPE_OMAV)
			tcpServer->sendString(QString("N100|30|5"));
		if (stationId==TYPE_ASSIM)
			tcpServer->sendString(QString("N100|30|3"));
		tcpServer->sendString(QString("N100|30|0"));
		//clear from basket_details
		apexExecQuery(QString("delete from basket_details where basketnum=%1").arg(basketTableId),APEX_DB_CONNECTION);
	}
}
//void basketContentsClass::buttonReleasedSlot()
//{
//	tcpServer->sendString(QString("N100|30|0"));
//}

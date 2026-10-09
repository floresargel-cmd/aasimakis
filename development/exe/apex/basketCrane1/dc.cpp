////#include "dc.h"
////defineBasketContentsClass::defineBasketContentsClass(int basketTableId_,QWidget *w):QDialog(w)
////{
////	setMinimumWidth(800);
////	setMinimumHeight(800);
////	basketTableId=basketTableId_;
////	addButnsMapper=new QSignalMapper(this);
////	removeButnsMapper=new QSignalMapper(this);
////	editButnsMapper=new QSignalMapper(this);
////	deleteRecordButnsMapper=new QSignalMapper(this);
////
////	connect(addButnsMapper,SIGNAL(mapped(int)),this,SLOT(addButtonClickedSlot(int)));
////	connect(removeButnsMapper,SIGNAL(mapped(int)),this,SLOT(removeButtonClickedSlot(int)));
////	connect(editButnsMapper,SIGNAL(mapped(int)),this,SLOT(editButtonClickedSlot(int)));
////	connect(deleteRecordButnsMapper,SIGNAL(mapped(int)),this,SLOT(deleteRecordButtonClickedSlot(int)));
////	//
////	QVBoxLayout *mainLayout=new QVBoxLayout(this);
////	QHBoxLayout *upLayout=new QHBoxLayout();mainLayout->addLayout(upLayout);
////	QHBoxLayout *downLayout=new QHBoxLayout();mainLayout->addLayout(downLayout);
////	setLayout(mainLayout);
////	//upLayout
////	basketInfoLabel=new QLabel(tr("basket : %1").arg(GETONE(QString("select number from baskets where pid=%1").arg(basketTableId)).toString()));
////	upLayout->addWidget(basketInfoLabel);
////	//downLayout
////	tableView=new QTableWidget();
////	tableView->setAlternatingRowColors(true);
////	tableView->horizontalHeader()->setStretchLastSection(true);
////	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
////	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
////	tableView->setColumnCount(5);
////	QStringList h;
////	h << tr("Profile") << tr("Length") << tr("Quantity") << tr("Alloy") << tr("Actions");
////	tableView->setHorizontalHeaderLabels(h);
////	downLayout->addWidget(tableView,1);
////
////	QHBoxLayout *btnsLayout=new QHBoxLayout();
////	mainLayout->addLayout(btnsLayout);
////	QPushButton *cancelBtn=new QPushButton(tr("Cancel"));
////	QPushButton *okBtn=new QPushButton(tr("OK"));
////	okBtn->setDefault(true);
////	btnsLayout->addWidget(cancelBtn);
////	btnsLayout->addWidget(okBtn);
////	connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
////	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
////	//
////	updateTable();
////}
////void defineBasketContentsClass::reject()
////{
////	QDialog::reject();
////}
////
////void defineBasketContentsClass::accept()
////{
////	QVector<QVariant> lengthArray=execQuery(QString("select length from basketContents where basketTableId=%1").arg(basketTableId));
////	int contentsType=BASKET_CONTENTS_NONE;
////	for (int i=0;i<lengthArray.count();i++)
////	{
////		if ((lengthArray[i].toDouble()>0)&&(contentsType<BASKET_CONTENS_MAX_LENGTH_24))
////			contentsType=BASKET_CONTENS_MAX_LENGTH_24;
////		if ((lengthArray[i].toDouble()>24.00000001)&&(contentsType<BASKET_CONTENS_MAX_LENGTH_30))
////			contentsType=BASKET_CONTENS_MAX_LENGTH_30;
////		if ((lengthArray[i].toDouble()>30.00000001)&&(contentsType<BASKET_CONTENS_MAX_LENGTH_MORE_THAN_30))
////			contentsType=BASKET_CONTENS_MAX_LENGTH_MORE_THAN_30;
////	}
////	execQuery(QString("update baskets set contentsType=%1 where pid=%2").arg(contentsType).arg(basketTableId));
////	QDialog::accept();
////}
////void defineBasketContentsClass::updateTable()
////{
////	//QVector<QVariant> idsArray=execQuery(QString("select pid from basketContents where basketTableId=%1 order by stackOrder").arg(basketTableId));
////	//QVector<QVariant> profilesArray=execQuery(QString("select profile from basketContents where basketTableId=%1 order by stackOrder").arg(basketTableId));
////	//QVector<QVariant> quantityArray=execQuery(QString("select quantity from basketContents where basketTableId=%1 order by stackOrder").arg(basketTableId));
////	//QVector<QVariant> lengthArray=execQuery(QString("select length from basketContents where basketTableId=%1 order by stackOrder").arg(basketTableId));
////	//QVector<QVariant> alloyArray=execQuery(QString("select alloy from basketContents where basketTableId=%1 order by stackOrder").arg(basketTableId));
////
////	//tableView->clearContents();
////	//tableView->setRowCount(profilesArray.count()+1);
////	//for (int i=0;i<profilesArray.count();i++)
////	//{
////	//	QWidget *profileWidget=new QWidget(this);
////	//	QVBoxLayout *profileWidgetLayout=new QVBoxLayout();
////	//	profileWidget->setLayout(profileWidgetLayout);
////	//	QToolButton *profileButton=new QToolButton(this);profileButton->setStyleSheet("QToolButton{font-size:16px;text-align:center;}");
////
////	//	profileButton->setIcon(QIcon(getPicFileNameFromProfile(profilesArray[i].toInt())));
////	//	profileButton->setStatusTip(tr("Edit record"));
////	//	profileButton->setToolTip(tr("Edit record"));
////	//	profileButton->setIconSize(QSize(75,75));
////	//
////	//	connect(profileButton,SIGNAL(clicked()),editButnsMapper,SLOT(map()));
////	//	editButnsMapper->setMapping(profileButton,idsArray[i].toInt());
////	//	profileWidgetLayout->addWidget(profileButton,1,Qt::AlignHCenter);
////	//	profileWidgetLayout->addWidget(new QLabel(profilesArray[i].toString()),1,Qt::AlignHCenter);
////	//	tableView->setCellWidget(i,0,profileWidget);
////	//
////
////	//	tableView->setItem(i,1,new QTableWidgetItem(QString().number(lengthArray[i].toDouble())));
////	//	tableView->setItem(i,2,new QTableWidgetItem(quantityArray[i].toString()));
////	//	tableView->setItem(i,3,new QTableWidgetItem(getAlloyDescription(alloyArray[i].toInt())));
////	//	//
////	//	QWidget *btnsWidget=new QWidget(this);
////	//	QVBoxLayout *btnsLayout=new QVBoxLayout();
////	//	btnsWidget->setLayout(btnsLayout);
////	//	QHBoxLayout *btnsUpLayout=new QHBoxLayout();btnsLayout->addLayout(btnsUpLayout);
////	//	QHBoxLayout *btnsDownLayout=new QHBoxLayout();btnsLayout->addLayout(btnsDownLayout);
////	//
////	//	QToolButton *addButton=new QToolButton();
////	//	addButton->setIcon(QIcon(":add.png"));
////	//	addButton->setText(tr("Add"));
////	//	addButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
////	//	addButton->setStatusTip(tr("Add quantity"));
////	//	addButton->setToolTip(tr("Add quantity"));
////	//	addButton->setIconSize(QSize(32,32));
////	//	connect(addButton,SIGNAL(clicked()),addButnsMapper,SLOT(map()));
////	//	addButnsMapper->setMapping(addButton,idsArray[i].toInt());
////	//	btnsUpLayout->addWidget(addButton);
////
////	//	QToolButton *removeButton=new QToolButton();
////	//	removeButton->setIcon(QIcon(":remove.png"));
////	//	removeButton->setText(tr("Remove"));
////	//	removeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
////	//	removeButton->setStatusTip(tr("Remove quantity"));
////	//	removeButton->setToolTip(tr("Remove quantity"));
////	//	removeButton->setIconSize(QSize(32,32));
////	//	connect(removeButton,SIGNAL(clicked()),removeButnsMapper,SLOT(map()));
////	//	removeButnsMapper->setMapping(removeButton,idsArray[i].toInt());
////	//	btnsUpLayout->addWidget(removeButton);
////
////	//	QToolButton *deleteRecordButton=new QToolButton();
////	//	deleteRecordButton->setIcon(QIcon(":delete.png"));
////	//	deleteRecordButton->setText(tr("Delete row"));
////	//	deleteRecordButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
////	//	deleteRecordButton->setStatusTip(tr("Delete record"));
////	//	deleteRecordButton->setToolTip(tr("Delete record"));
////	//	deleteRecordButton->setIconSize(QSize(32,32));
////	//	connect(deleteRecordButton,SIGNAL(clicked()),deleteRecordButnsMapper,SLOT(map()));
////	//	deleteRecordButnsMapper->setMapping(deleteRecordButton,idsArray[i].toInt());
////	//	btnsDownLayout->addWidget(deleteRecordButton);
////
////	//	tableView->setRowHeight(i,140);
////	//	tableView->setCellWidget(i,4,btnsWidget);
////	//}
////	//addRecordPushButton=new QPushButton(QIcon(":add.png"),tr("Add a record"),this);
////	//addRecordPushButton->setStatusTip(tr("Adds a record to the profile table"));
////	//addRecordPushButton->setToolTip(tr("Adds a record to the profile table"));
////	//connect(addRecordPushButton,SIGNAL(released()),this,SLOT(addRecordSlot()));
////	//tableView->setRowHeight(profilesArray.count(),40);
////	//tableView->setCellWidget(profilesArray.count(),4,addRecordPushButton);
////	//tableView->resizeColumnsToContents();
////	//tableView->setColumnWidth(0,100);
////	//tableView->horizontalHeader()->setStretchLastSection(true);
////}
////
////void defineBasketContentsClass::addButtonClickedSlot(int id)
////{
////	QString profile=GETONE(QString("select profile from basketContents where pid=%1").arg(id),MAIN_DB_CONNECTION).toString();
////	int no=GETONE(QString("select quantity from basketContents where pid=%1").arg(id),MAIN_DB_CONNECTION).toInt();
////	bool ok=false;
////	int addedNo=QInputDialog::getInt(this,tr("Add quantity of profiles."),tr("Specify the number of %2 type profiles that you added in the basket.").arg(profile.trimmed()),1,0,INT_MAX,1,&ok);
////	if (ok)
////	{
////		execQuery(QString("update basketContents set quantity=%1 where pid=%2").arg(no+addedNo).arg(id),MAIN_DB_CONNECTION);
////		updateTable();
////	}
////}
////void defineBasketContentsClass::removeButtonClickedSlot(int id)
////{
////	QString profile=GETONE(QString("select profile from basketContents where pid=%1").arg(id),MAIN_DB_CONNECTION).toString();
////	int no=GETONE(QString("select quantity from basketContents where pid=%1").arg(id),MAIN_DB_CONNECTION).toInt();
////	bool ok=false;
////	int removedNo=QInputDialog::getInt(this,tr("Remove quantity of profiles."),tr("Specify the number of %2 type profiles that you removed in the basket.").arg(profile.trimmed()),1,0,INT_MAX,1,&ok);
////	if (ok)
////	{
////		execQuery(QString("update basketContents set quantity=%1 where pid=%2").arg(no-removedNo).arg(id),MAIN_DB_CONNECTION);
////		updateTable();
////	}
////}
////void defineBasketContentsClass::editButtonClickedSlot(int id)
////{
////	//QVBoxLayout *mainDlgLayout=new QVBoxLayout();
////	//QHBoxLayout *upLayout=new QHBoxLayout();mainDlgLayout->addLayout(upLayout);
////	//QGridLayout *midLayout=new QGridLayout();mainDlgLayout->addLayout(midLayout);
////	//QHBoxLayout *downLayout=new QHBoxLayout();mainDlgLayout->addLayout(downLayout);
////	//QDialog dlg(this);dlg.setLayout(mainDlgLayout);
////	////up
////	//upLayout->addWidget(new QLabel(tr("Edit the values of the record")));
////	////mid
////	//	//profile
////	//	midLayout->addWidget(new QLabel(tr("Profile")),0,0);
////	//	QLineEdit *profileEdit=new QLineEdit(GETONE(QString("select profile from basketContents where pid=%1").arg(id),MAIN_DB_CONNECTION).toString().trimmed());
////	//	profileEdit->setMaxLength(29);
////	//	midLayout->addWidget(profileEdit,1,0);
////	//	//length
////	//	midLayout->addWidget(new QLabel(tr("Length")),0,1);
////	//	QDoubleSpinBox *lengthSpin=new QDoubleSpinBox(&dlg);
////	//	lengthSpin->setSingleStep(0.1);
////	//	lengthSpin->setRange(1.,30.);
////	//	lengthSpin->setValue(GETONE(QString("select length from basketContents where pid=%1").arg(id),MAIN_DB_CONNECTION).toDouble()/1000.);
////	//	midLayout->addWidget(lengthSpin,1,1);
////	//	//quantity
////	//	midLayout->addWidget(new QLabel(tr("Quantity")),0,2);
////	//	QSpinBox *quantitySpin=new QSpinBox(&dlg);
////	//	quantitySpin->setRange(0,INT_MAX);
////	//	quantitySpin->setValue(GETONE(QString("select quantity from basketContents where pid=%1").arg(id),MAIN_DB_CONNECTION).toInt());
////	//	midLayout->addWidget(quantitySpin,1,2);
////	//	//alloySpin
////	//	midLayout->addWidget(new QLabel(tr("Alloy")),0,4);
////	//	int alloyInt=GETONE(QString("select alloy from basketContents where pid=%1").arg(id),MAIN_DB_CONNECTION).toInt();
////	//	QComboBox *alloyComboBox=new QComboBox(&dlg);
////	//	alloyComboBox->addItems(*alloyDescription);
////	//	int alloyIndex=alloyIdArray->indexOf(alloyInt);
////	//	if (alloyIndex>=0)
////	//		alloyComboBox->setCurrentIndex(alloyIndex);
////	//	midLayout->addWidget(alloyComboBox,1,4);
////	////down
////	//QPushButton *cancelBtn=new QPushButton(tr("Cancel"));
////	//downLayout->addWidget(cancelBtn);
////	//connect(cancelBtn,SIGNAL(clicked()),&dlg,SLOT(reject()));
////	//QPushButton *okBtn=new QPushButton(tr("OK"));
////	//downLayout->addWidget(okBtn);
////	//okBtn->setDefault(true);
////	//connect(okBtn,SIGNAL(clicked()),&dlg,SLOT(accept()));
////	/////
////	//if (dlg.exec()==QDialog::Accepted)
////	//{
////	//	int noOfSameInBasket=GETONEATMOST(QString("select quantity from basketContents where profile='%1' and basketTableId=%2 and length=%3 and alloy=%4 and quantity>0 and pid!=%7")
////	//		.arg(profileEdit->text()).arg(basketTableId).arg(int(lengthSpin->value()*1000.)).arg(alloyIdArray->at(alloyComboBox->currentIndex())).arg(id),MAIN_DB_CONNECTION).toInt();
////	//	if (noOfSameInBasket!=0)
////	//	{
////	//		QMessageBox::information(this,tr("Warning"),tr("Basket %1 allready has a record with the same profile, length, finish, alloy, quality data.").arg(GETONE(QString("select number from baskets where pid=%1").arg(basketTableId)).toString()));
////	//		return;
////	//	}
////	//	execQuery(QString("update basketContents set profile=%1,quantity=%2,length=%3,alloy=%4 where pid=%5")
////	//		.arg(profileEdit->text().toInt()).arg(quantitySpin->value()).arg(lengthSpin->value()).arg(alloyIdArray->at(alloyComboBox->currentIndex())).arg(id),MAIN_DB_CONNECTION);
////	//}
////	//updateTable();
////}
////void defineBasketContentsClass::deleteRecordButtonClickedSlot(int id)
////{
////	QMessageBox msgBox(this);
////	msgBox.setWindowTitle(tr("Warning"));
////	msgBox.setText(tr("You are about to delete the record."));
////	msgBox.setInformativeText("Do you want to proceed?");
////	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
////	msgBox.setDefaultButton(QMessageBox::Yes);
////	if (msgBox.exec()==QMessageBox::Yes)
////	{
////		execQuery(QString("update basketContents set quantity=%1 where pid=%2").arg(0).arg(id),MAIN_DB_CONNECTION);
////		execQuery(QString("delete from basketContents where pid=%1").arg(id),MAIN_DB_CONNECTION);
////		updateTable();
////	}
////}
////void defineBasketContentsClass::addRecordSlot()
////{
////	int stackOrder=GETFIRST(QString("select stackOrder from basketContents order by pid desc"),MAIN_DB_CONNECTION).toInt()+1;
////	execQuery(QString("insert into basketContents (profile,basketTableId,quantity,length,alloy,stackOrder) values(0,%1,1,1,1,%2)").arg(basketTableId).arg(stackOrder),MAIN_DB_CONNECTION);
////	int newId=GETFIRST(QString("select top 1 pid from basketContents order by pid desc"),MAIN_DB_CONNECTION).toInt();
////	editButtonClickedSlot(newId);
////}

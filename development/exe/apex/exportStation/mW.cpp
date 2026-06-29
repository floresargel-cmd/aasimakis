#include "mW.h"
//#define inMyLaptop
#ifdef inMyLaptop
	bool globalInMyLaptop=true;
#else
	bool globalInMyLaptop=false;
#endif
QVector<QProcess*> externalProcessesVector; 
QString plcIp="20.20.20.20";
//
mainWindowClass::mainWindowClass()
{
	tuxipServer=new tuxipServerClass(this);
	tuxipServer->addConnection(plcIp,"conveyors");
	tuxipClass *conveyorTuxipClass=tuxipServer->getTuxipClassWithIp(plcIp);
	tcpServer=new tcpServerClass(conveyorTuxipClass,this);
	connect(conveyorTuxipClass,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(signalFromTuxPlcSlot(QString,QString,int,QVariant)));
#ifdef inMyLaptop
	//MAIN_DB_CONNECTION
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("local");//odbc name
	mainDb.setUserName("sa"); 
	mainDb.setPassword("123");
	//db.setPassword("qwerty1!");
	if (!mainDb.open()) 
		EXIT(tr("Error in database"),mainDb.lastError().text());
	apexExecQuery("USE basketCraneData",MAIN_DB_CONNECTION);
	///APEX_DB_CONNECTION
	QSqlDatabase apexDb=QSqlDatabase::addDatabase("QODBC",APEX_DB_CONNECTION);
	apexDb.setDatabaseName("local");//odbc name
	apexDb.setUserName("sa"); 
	apexDb.setPassword("123");
	if (!apexDb.open()) 
		EXIT(tr("Error in database"),apexDb.lastError().text());
	apexExecQuery("USE PEManager_tmp",APEX_DB_CONNECTION);
#else
	//MAIN_DB_CONNECTION
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
	mainDb.setDatabaseName("apexBasketCraneMainOdbc");//odbc name
	mainDb.setUserName("sa"); 
	mainDb.setPassword("123");
	//db.setPassword("qwerty1!");
	if (!mainDb.open()) 
		EXIT(tr("Error in database"),mainDb.lastError().text());
	apexExecQuery("USE basketCraneData",MAIN_DB_CONNECTION);
	QSqlDatabase apexDb=QSqlDatabase::addDatabase("QODBC",APEX_DB_CONNECTION);
	apexDb.setDatabaseName("apexEpicsOdbc");//odbc name
	apexDb.setUserName("peadm2"); 
	apexDb.setPassword("peadm2");
	//db.setPassword("qwerty1!");
	if (!apexDb.open()) 
		EXIT(tr("Error in database"),apexDb.lastError().text());
	apexExecQuery("USE PEManager_tmp",APEX_DB_CONNECTION);
#endif
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	QWidget *packingWidget=new QWidget();
	setWindowIcon(QIcon(":/assimakis.png"));
	//packing
	QSplitter *mainPackingSplitter=new QSplitter(Qt::Horizontal);

	omavStation=new exportStationClass(TYPE_OMAV,tcpServer,this);
	mainPackingSplitter->addWidget(omavStation);

	assimStation=new exportStationClass(TYPE_ASSIM,tcpServer,this);
	mainPackingSplitter->addWidget(assimStation);

	QVBoxLayout *packingLayout=new QVBoxLayout();
	packingWidget->setLayout(packingLayout);
	//setCentralWidget(mainSplitter);
	packingLayout->addWidget(mainPackingSplitter);

	//import station
	QHBoxLayout *importStationLayout=new QHBoxLayout();
	importStationDefineNewBasketPushButton=new myQPushButton(tcpServer,"","N100|30|0",tr("About to put new basket in import station."),NULL);importStationLayout->addWidget(importStationDefineNewBasketPushButton);
	connect(importStationDefineNewBasketPushButton,SIGNAL(clicked()),this,SLOT(onImportStationDefineNewBasketPushButtonSlot()));
	importStationDefineNewScrapBasketPushButton=new myQPushButton(tcpServer,"","N100|30|0",tr("About to put new scrap basket in import station."),NULL);importStationLayout->addWidget(importStationDefineNewScrapBasketPushButton);
	connect(importStationDefineNewScrapBasketPushButton,SIGNAL(clicked()),this,SLOT(onImportStationDefineNewScrapBasketPushButtonSlot()));
	importStationBasketIsReadyForImportationPushButton=new myQPushButton(tcpServer,"","N100|30|0",tr("New basket in import station ready for import."),NULL);importStationLayout->addWidget(importStationBasketIsReadyForImportationPushButton);
	connect(importStationBasketIsReadyForImportationPushButton,SIGNAL(clicked()),this,SLOT(onImportStationBasketIsReadyForImportationPushButtonSlot()));
	importStationIsWithoutBasketPushButton=new myQPushButton(tcpServer,"","N100|30|0",tr("Import station is without basket."),NULL);importStationLayout->addWidget(importStationIsWithoutBasketPushButton);
	connect(importStationIsWithoutBasketPushButton,SIGNAL(clicked()),this,SLOT(onImportStationIsWithoutBasketPushButtonSlot()));
	mainLayout->addWidget(packingWidget);
	mainLayout->addLayout(importStationLayout);
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimer()));
	mainTimer->start(1000);
	//
	QAction *showServerAction=new QAction("",this);
	addAction(showServerAction);
	showServerAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	conveyorTuxipClass->addTable("N100",plcTableClass::TYPE_INT,0,39,750);
	connect(showServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));
	tuxipServer->setIsAutoUpdatingFromPlc(true);
	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
}
mainWindowClass::~mainWindowClass()
{
}
void mainWindowClass::showServerSlot()
{
	tuxipServer->showDialog();
}
void mainWindowClass::onMainTimer()
{
	setWindowTitle(tr("exports. apex packing area, assimakis sa ","main window title..")+timeVerbose);
}
void mainWindowClass::onImportStationDefineNewBasketPushButtonSlot()
{
	bool ok;
	int value=QInputDialog::getInt(this,tr("Define basket number"),tr("Define the number of the basket you are about to put in the import station.\n the import station will not be able to take any exports from the crane until this import ends or 3 minutes pass."),0,0,100000,1,&ok);
	if (ok)
	{
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("Importtation of basket"));
		msgBox.setText(tr("The import station will not be able to take any exports from the crane until this import ends or 3 minutes pass."));
		msgBox.setInformativeText("Do you want to proceed?");
		msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
		msgBox.setDefaultButton(QMessageBox::Yes);
		if (msgBox.exec()==QMessageBox::No)
			return;
		tcpServer->sendString(QString("N100|30|49"));
		tcpServer->sendString(QString("N100|30|0"));
		tcpServer->sendString(QString("N100|16|%1").arg(value));
	}
}
void mainWindowClass::onImportStationDefineNewScrapBasketPushButtonSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Importtation of scrap basket"));
	msgBox.setText(tr("The import station will not be able to take any exports from the crane until this import ends or 3 minutes pass."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N100|30|49"));
	tcpServer->sendString(QString("N100|30|0"));
	tcpServer->sendString(QString("N100|16|-999"));
}
void mainWindowClass::onImportStationBasketIsReadyForImportationPushButtonSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Important"));
	msgBox.setText(tr("Make sure that the forklift has left the import station area and that no humans are near."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N100|30|74"));
	tcpServer->sendString(QString("N100|30|0"));
}
void mainWindowClass::onImportStationIsWithoutBasketPushButtonSlot()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Important"));
	msgBox.setText(tr("Make sure that the forklift has left the import station area and that the basket is not there."));
	msgBox.setInformativeText("Do you want to proceed?");
	msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if (msgBox.exec()==QMessageBox::No)
		return;
	tcpServer->sendString(QString("N100|30|73"));
	tcpServer->sendString(QString("N100|30|0"));
}
void mainWindowClass::signalFromTuxPlcSlot(QString ip,QString table,int index,QVariant value)
{
	signalFromPlcSlot(table,index,value.toString());
}
void mainWindowClass::signalFromPlcSlot(QString table,int index,QString value)
{
	if (table=="N100")
	{
		if (index==14)
			omavStation->plcValueChanged(index,value.toInt());
		if (index==11)
			assimStation->plcValueChanged(index,value.toInt());
		if (index==13)
			omavStation->plcValueChanged(index,value.toInt());
		if (index==10)
			assimStation->plcValueChanged(index,value.toInt());
		if (index==0)
		{
			assimStation->plcValueChanged(index,value.toInt());
			omavStation->plcValueChanged(index,value.toInt());
			int intValue=value.toInt();
			if (HAVECOMMONBIT(intValue,BITS[26]))importStationDefineNewBasketPushButton->setEnabled(true,false);else importStationDefineNewBasketPushButton->setEnabled(false,false);
			if (HAVECOMMONBIT(intValue,BITS[26]))importStationDefineNewScrapBasketPushButton->setEnabled(true,false);else importStationDefineNewScrapBasketPushButton->setEnabled(false,false);
			if (HAVECOMMONBIT(intValue,BITS[27]))importStationBasketIsReadyForImportationPushButton->setEnabled(true,false);else importStationBasketIsReadyForImportationPushButton->setEnabled(false,false);
		}
	}
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}
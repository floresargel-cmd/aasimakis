#include "mW.h"
QVector<QProcess*> externalProcessesVector; 
mainWindowClass::mainWindowClass()
{
	//MAIN_DB_CONNECTION
	QSqlDatabase mainDb=QSqlDatabase::addDatabase("QODBC",MAIN_DB_CONNECTION);
#ifdef USE_LOCAL_DB
	mainDb.setDatabaseName("local");
#else
	mainDb.setDatabaseName("apexPacketCraneMainOdbc");
#endif
	
	mainDb.setUserName("sa"); 
	mainDb.setPassword("123");
	if (!mainDb.open()) 
		EXIT(tr("Error in database"),mainDb.lastError().text());
	execQuery("USE wareHouseData",MAIN_DB_CONNECTION);
	//
	setWindowIcon(QIcon(":/logo_16x16.png"));
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	QScrollArea *mainScrollArea=new QScrollArea();
	mainScrollArea->setBackgroundRole(QPalette::Light);
	mainLayout->addWidget(mainScrollArea);
	QWidget *mainScrollAreaWidget=new QWidget();
	QHBoxLayout *mainScrollAreaWidgetLayout=new QHBoxLayout();mainScrollAreaWidget->setLayout(mainScrollAreaWidgetLayout);

	QVBoxLayout *eastLayout=new QVBoxLayout();mainScrollAreaWidgetLayout->addLayout(eastLayout);
	QVBoxLayout *spaceLayout=new QVBoxLayout();mainScrollAreaWidgetLayout->addLayout(spaceLayout,1);
	QVBoxLayout *westLayout=new QVBoxLayout();mainScrollAreaWidgetLayout->addLayout(westLayout);

	//
	QHBoxLayout *eastTopLayout=new QHBoxLayout();eastLayout->addLayout(eastTopLayout);
	eastTopLayout->addWidget(new QLabel(tr("East")));
	QPushButton *printEastPushBtn=new QPushButton(QIcon(":/print.png"),"");
	eastTopLayout->addWidget(printEastPushBtn);
	eastTopLayout->addStretch(1);
	connect(printEastPushBtn,SIGNAL(clicked()),this,SLOT(printEastBtnSlot()));
	eastLayout->addWidget(eastGroup0=new groupOfCellsClass( 1,20,true));
	eastLayout->addWidget(eastGroup1=new groupOfCellsClass(21,40,true));
	eastLayout->addWidget(eastGroup2=new groupOfCellsClass(41,60,true));

	QHBoxLayout *westTopLayout=new QHBoxLayout();westLayout->addLayout(westTopLayout);
	westTopLayout->addWidget(new QLabel(tr("West")));
	QPushButton *printWestPushBtn=new QPushButton(QIcon(":/print.png"),"");
	westTopLayout->addWidget(printWestPushBtn);
	westTopLayout->addStretch(1);
	connect(printWestPushBtn,SIGNAL(clicked()),this,SLOT(printWestBtnSlot()));
	westLayout->addWidget(westGroup0=new groupOfCellsClass( 1,20,false));
	westLayout->addWidget(westGroup1=new groupOfCellsClass(21,40,false));
	westLayout->addWidget(westGroup2=new groupOfCellsClass(41,60,false));

	mainScrollArea->setWidget(mainScrollAreaWidget);
	//
	QPushButton *excellPushButton=new QPushButton("Create an excell report",this);
	mainLayout->addWidget(excellPushButton);
	excellPushButton->setIcon(QIcon(":/excel.png"));
	excellPushButton->setIconSize(QSize(32,32));
	connect(excellPushButton,SIGNAL(clicked()),this,SLOT(excellBtnSlot()));
	//
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimer()));
	onMainTimer();
	mainTimer.start(1000);
	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
}

void mainWindowClass::excellBtnSlot()
{
	QString excellFName=QString("Z:\\Inventory\\whereHouseContents.")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString("_hh.mm.ss")+".csv";
	//QString excellFName=QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString("_hh.mm.ss")+".csv";
	QFile file(excellFName);
  if (!file.open(QIODevice::Append|QIODevice::Text))
	{
		QMessageBox::critical(this,"Error",tr("Could not create file:%1").arg(excellFName));
		return;
	}
	QTextStream out(&file);
	out<<tr("packet id,")<<tr("length,")<<tr("width,")<<tr("height,")<<tr("woods,")<<tr("in sb,")<<tr("weight")<<"\n";
	QVector<QVariant> profilesVector=execQuery("SELECT packetId FROM packets where packetId in (select packetId from positions where packetId<>0) order by packetId",MAIN_DB_CONNECTION);
	QVector<QVariant> lengthVector=execQuery("SELECT length FROM packets where packetId in (select packetId from positions where packetId<>0) order by packetId",MAIN_DB_CONNECTION);
	QVector<QVariant> widthVector=execQuery("SELECT width FROM packets where packetId in (select packetId from positions where packetId<>0) order by packetId",MAIN_DB_CONNECTION);
	QVector<QVariant> heightVector=execQuery("SELECT height FROM packets where packetId in (select packetId from positions where packetId<>0) order by packetId",MAIN_DB_CONNECTION);
	QVector<QVariant> woodsVector=execQuery("SELECT woods FROM packets where packetId in (select packetId from positions where packetId<>0) order by packetId",MAIN_DB_CONNECTION);
	QVector<QVariant> sbVector=execQuery("SELECT sb FROM packets where packetId in (select packetId from positions where packetId<>0) order by packetId",MAIN_DB_CONNECTION);
	QVector<QVariant> weightVector=execQuery("SELECT weight FROM packets where packetId in (select packetId from positions where packetId<>0) order by packetId",MAIN_DB_CONNECTION);
	for (int i=0;i<profilesVector.count();i++)
	{
		out<<profilesVector[i].toInt()<<",";
		out<<lengthVector[i].toInt()<<",";
		out<<widthVector[i].toInt()<<",";
		out<<heightVector[i].toInt()<<",";
		out<<woodsVector[i].toInt()<<",";
		out<<(sbVector[i].toInt()==1?"no":"yes")<<",";
		out<<weightVector[i].toInt()<<"\n";
	}
	file.close();
	QDesktopServices::openUrl(excellFName);
}
void mainWindowClass::printEastBtnSlot()
{
	QString pdfFName=QString("eastCellsContents.")+QDateTime::currentDateTime().toString("yyyy.MM.dd")+QTime::currentTime().toString(".hh.mm.ss")+".pdf";
	QString pdfFile=QDir::current().absolutePath()+"//"+pdfFName;

	QPrinter printer(QPrinter::HighResolution);
	float scale=46;
	int W=208*scale;
	int H=297*scale;
	int dh=H/50.;
	int dw=W/22.;
	printer.setOutputFileName(pdfFile);
	printer.setOrientation(QPrinter::Portrait);
#if QT_VERSION > QT_VERSION_CHECK(5, 1, 1)
	printer.setPageSize(QPageSize(QPageSize::A4));
#else
	printer.setPageSize(QPrinter::A4);
#endif
	QPainter painter;
	painter.begin(&printer);
	double y=0;
	painter.setFont(QFont("Arial",12));
	QString title=QString("Warehouse contents-east cells ")+(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm"));
	painter.drawRect(QRect(0,0,W,H));
	painter.drawText(QRectF(0,y,W,2*dh),Qt::AlignCenter|Qt::TextWordWrap,title);
	int dGroup=14*dh;
	y=dh;eastGroup0->writeToPdf(&painter,y,dw,dh);
	y+=(dGroup+2*dh);eastGroup1->writeToPdf(&painter,y,dw,dh);
	y+=(dGroup+2*dh);eastGroup2->writeToPdf(&painter,y,dw,dh);
	QDesktopServices::openUrl(pdfFName);
}
void mainWindowClass::printWestBtnSlot()
{
	QString pdfFName=QString("westCellsContents.")+QDateTime::currentDateTime().toString("yyyy.MM.dd")+QTime::currentTime().toString(".hh.mm.ss")+".pdf";
	QString pdfFile=QDir::current().absolutePath()+"//"+pdfFName;

	QPrinter printer(QPrinter::HighResolution);
	float scale=46;
	int W=208*scale;
	int H=297*scale;
	int dh=H/50.;
	int dw=W/22.;
	printer.setOutputFileName(pdfFile);
	printer.setOrientation(QPrinter::Portrait);
#if QT_VERSION > QT_VERSION_CHECK(5, 1, 1)
	printer.setPageSize(QPageSize(QPageSize::A4));
#else
	printer.setPageSize(QPrinter::A4);
#endif
	QPainter painter;
	painter.begin(&printer);
	double y=0;
	painter.setFont(QFont("Arial",12));
	QString title=QString("Warehouse contents-west cells ")+(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm"));
	painter.drawRect(QRect(0,0,W,H));
	painter.drawText(QRectF(0,y,W,2*dh),Qt::AlignCenter|Qt::TextWordWrap,title);
	int dGroup=14*dh;
	y=dh;westGroup0->writeToPdf(&painter,y,dw,dh);
	y+=(dGroup+2*dh);westGroup1->writeToPdf(&painter,y,dw,dh);
	y+=(dGroup+2*dh);westGroup2->writeToPdf(&painter,y,dw,dh);
	QDesktopServices::openUrl(pdfFName);
}
void mainWindowClass::onMainTimer()
{
	setWindowTitle(tr("warehouse contents ","main window title..")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss"));
}
void mainWindowClass::qtAboutSlot()
{
	QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}
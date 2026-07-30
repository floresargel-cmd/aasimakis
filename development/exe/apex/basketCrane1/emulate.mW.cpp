#include "emulate.mW.h"
#include "configuration/ConfigurationManager.h"

plcEmulateWindowClass::plcEmulateWindowClass(tuxipClass *craneTuxip_,tuxipClass *conveyorTuxip_,QWidget *p):QDialog(p)
{
	craneTuxip=craneTuxip_;
	conveyorTuxip=conveyorTuxip_;
	initializeDatabase();
	setWindowIcon(QIcon(":/logo_16x16.png"));
	//
	QHBoxLayout *mainLayout=new QHBoxLayout();
	setLayout(mainLayout);
	//
	QTabWidget *mainTabWidget=new QTabWidget(this);
	mainLayout->addWidget(mainTabWidget);
	//
	craneEmulator=new craneEmulatorClass(craneTuxip);
	mainTabWidget->addTab(craneEmulator,QIcon(),tr("crane"));

	//conveyorsEmulator=new conveyorsEmulatorClass(conveyorTuxip);
	//mainTabWidget->addTab(conveyorsEmulator,QIcon(),tr("oven conveyors"));
	//

	connect(&mainTimer, SIGNAL(timeout()), this, SLOT(onMainTimer()));
	onMainTimer();

	mainTimer.start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().EmulationMainMilliseconds());
}

void plcEmulateWindowClass::tuxIsReady()
{
	craneEmulator->tuxIsReady();
	conveyorsEmulator->tuxIsReady();
}

void plcEmulateWindowClass::createTable(QString tableName,QStringList colNames,QStringList colTypes)
{
	QVector<QVariant> tableArray = execQuery(QString("SELECT name FROM sqlite_master WHERE name='") + tableName + "'", EMULATE_DB_CONNECTION);
	if (tableArray.count() == 0)//table does not exist 
	{
		QString stmnt = QString("CREATE TABLE ") + tableName + " (";
		for (int i = 0; i < colNames.length(); i++)
		{
			stmnt += colNames[i] + " " + colTypes[i];
			if (i != (colNames.length() - 1))
				stmnt += ",";
		}
		stmnt += ")";
		execQuery(stmnt, EMULATE_DB_CONNECTION);
	}
}

void plcEmulateWindowClass::demoFillTables()
{
	execQuery(QString("insert into positions (description,basket) values('beforeStacker',%1)").arg(GETONE("select basketTableId from positions where posNumber=400 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('onStacker',%1)").arg(GETONE("select basketTableId from positions where posNumber=900 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('afterStacker',%1)").arg(GETONE("select basketTableId from positions where posNumber=410 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);

	execQuery(QString("insert into positions (description,basket) values('beforeStacker2',%1)").arg(GETONE("select basketTableId from positions where posNumber=450 and posIndex=1", MAIN_DB_CONNECTION).toInt()), EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('onStacker2',%1)").arg(GETONE("select basketTableId from positions where posNumber=910 and posIndex=1", MAIN_DB_CONNECTION).toInt()), EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('afterStacker2',%1)").arg(GETONE("select basketTableId from positions where posNumber=460 and posIndex=1", MAIN_DB_CONNECTION).toInt()), EMULATE_DB_CONNECTION);
	//
	//execQuery(QString("insert into positions (description,basket) values('lastReturningFromOven',0)"),EMULATE_DB_CONNECTION);

	execQuery(QString("insert into positions (description,basket) values('beforeOven100',%1)").arg(GETONE("select basketTableId from positions where posNumber=100 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('beforeOven110',%1)").arg(GETONE("select basketTableId from positions where posNumber=110 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('beforeOven120',%1)").arg(GETONE("select basketTableId from positions where posNumber=120 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('beforeOven130',%1)").arg(GETONE("select basketTableId from positions where posNumber=130 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('beforeOven140',%1)").arg(GETONE("select basketTableId from positions where posNumber=140 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('beforeOven150',%1)").arg(GETONE("select basketTableId from positions where posNumber=150 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	//
	execQuery(QString("insert into positions (description,basket) values('oven600',%1)").arg(GETONE("select basketTableId from positions where posNumber=600 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('oven610',%1)").arg(GETONE("select basketTableId from positions where posNumber=610 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('oven620',%1)").arg(GETONE("select basketTableId from positions where posNumber=620 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('oven630',%1)").arg(GETONE("select basketTableId from positions where posNumber=630 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('oven640',%1)").arg(GETONE("select basketTableId from positions where posNumber=640 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('oven650',%1)").arg(GETONE("select basketTableId from positions where posNumber=650 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	//
	execQuery(QString("insert into positions (description,basket) values('afterOven660',%1)").arg(GETONE("select basketTableId from positions where posNumber=660 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('afterOven670',%1)").arg(GETONE("select basketTableId from positions where posNumber=670 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('afterOven680',%1)").arg(GETONE("select basketTableId from positions where posNumber=680 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('afterOven690',%1)").arg(GETONE("select basketTableId from positions where posNumber=690 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);

	execQuery(QString("insert into positions (description,basket) values('returningFromOven700',%1)").arg(GETONE("select basketTableId from positions where posNumber=700 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('returningFromOven710',%1)").arg(GETONE("select basketTableId from positions where posNumber=710 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('returningFromOven720',%1)").arg(GETONE("select basketTableId from positions where posNumber=720 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	// execQuery(QString("insert into positions (description,basket) values('returningFromOven730',%1)").arg(GETONE("select basketTableId from positions where posNumber=730 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);
	execQuery(QString("insert into positions (description,basket) values('returningFromOven160',%1)").arg(GETONE("select basketTableId from positions where posNumber=160 and posIndex=1",MAIN_DB_CONNECTION).toInt()),EMULATE_DB_CONNECTION);

	//execQuery(QString("insert into positions (description,basket) values('toAsimakisStackerW',0)"),EMULATE_DB_CONNECTION);
	//execQuery(QString("insert into positions (description,basket) values('toAsimakisStacker',0)"),EMULATE_DB_CONNECTION);
	//execQuery(QString("insert into positions (description,basket) values('toOmavStackerW',0)"),EMULATE_DB_CONNECTION);
	//execQuery(QString("insert into positions (description,basket) values('toOmavStacker',0)"),EMULATE_DB_CONNECTION);
	//execQuery(QString("insert into positions (description,basket) values('inOut',0)"),EMULATE_DB_CONNECTION);
}
void plcEmulateWindowClass::initializeDatabase()
{	
	//checkCreate tables
	if (execQuery("SELECT name FROM sqlite_master WHERE name ='positions'",EMULATE_DB_CONNECTION).count()==0)
	{
		createTable("positions",QStringList()<<"tableId"<<"description"<<"basket",QStringList()<<"integer primary key autoincrement"<<"char(21)"<<"int");
		demoFillTables();
	}
}

void plcEmulateWindowClass::onMainTimer()
{
	setWindowTitle(tr("basketCraneEmulator ","main window title..")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss"));
}
//////////////////////////////////////////////////
void plcEmulateWindowClass::cranePlcDataChanged(QString table,int index,QString value)
{
	craneEmulator->plcDataChanged(table,index,value);
}
void plcEmulateWindowClass::conveyorPlcDataChanged(QString table,int index,QString value)
{
	conveyorsEmulator->plcDataChanged(table,index,value);
}

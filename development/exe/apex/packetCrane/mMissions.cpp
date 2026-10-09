#include "pch.h"
#include "mMissions.h"
manualMissionsClass::manualMissionsClass(craneClass *cr,forkClass *fr,allPosisionsClass *all,QWidget *p):QDialog(p)
{
	setWindowTitle(tr("State"));
	//resize(1500,900);
	crane=cr;
	fork=fr;
	allPosisions=all;
	QVBoxLayout *mainLayout=new QVBoxLayout();this->setLayout(mainLayout);
	//QTabWidget *mainTabWidget=new QTabWidget();mainLayout->addWidget(mainTabWidget);
	//
	//QWidget *missionsWidget=new QWidget();mainTabWidget->addTab(missionsWidget,tr("State"));
	//QWidget *forkWidget=new QWidget();mainTabWidget->addTab(forkWidget,tr("Fork positions"));
	//QScrollArea *eastScrollArea=new QScrollArea();eastScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	//mainTabWidget->addTab(eastScrollArea,tr("East positions"));
	//QScrollArea *westScrollArea=new QScrollArea();westScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	//mainTabWidget->addTab(westScrollArea,tr("West positions"));
	////cells
	//eastScrollArea->setWidgetResizable(true);
	//QWidget *eastScrollAreaWidget=new QWidget();
	//QGridLayout *eastScrollAreaWidgetLayout=new QGridLayout();eastScrollAreaWidget->setLayout(eastScrollAreaWidgetLayout);
	//eastScrollArea->setWidget(eastScrollAreaWidget);

	//westScrollArea->setWidgetResizable(true);
	//QWidget *westScrollAreaWidget=new QWidget();
	//QGridLayout *westScrollAreaWidgetLayout=new QGridLayout();westScrollAreaWidget->setLayout(westScrollAreaWidgetLayout);
	//westScrollArea->setWidget(westScrollAreaWidget);

	//QVector<cellPositionClass*> allCells=allPosisions->getAllCells();
	//for (int i=0;i<allCells.count();i++)
	//{
	//	if (allCells[i]->getPosId()>0)
	//	{
	//		manualMissionsCellClass *manualMissionsCell=new manualMissionsCellClass(allCells[i]);
	//		cells<<manualMissionsCell;
	//		eastScrollAreaWidgetLayout->addWidget(manualMissionsCell,13-allCells[i]->getRow(),allCells[i]->getColumn());
	//	}
	//	if (allCells[i]->getPosId()<0)
	//	{
	//		manualMissionsCellClass *manualMissionsCell=new manualMissionsCellClass(allCells[i]);
	//		cells<<manualMissionsCell;
	//		westScrollAreaWidgetLayout->addWidget(manualMissionsCell,13-allCells[i]->getRow(),allCells[i]->getColumn());
	//	}
	//}
	//
	//QVBoxLayout *forkWidgetL=new QVBoxLayout();forkWidget->setLayout(forkWidgetL);
	//packetPositions<<new manualMissionsPacketClass(fork->getPositionSb());
	//forkWidgetL->addWidget(packetPositions.last());
	//packetPositions<<new manualMissionsPacketClass(fork->getPositionPa());
	//forkWidgetL->addWidget(packetPositions.last());
	//packetPositions<<new manualMissionsPacketClass(fork->getPositionIm());
	//forkWidgetL->addWidget(packetPositions.last());
	//QPushButton *saveForkPosBtn=new QPushButton(tr("Save"));
	//forkWidgetL->addWidget(saveForkPosBtn);
	//connect(saveForkPosBtn,SIGNAL(clicked()),this,SLOT(saveForkPosBtnSlot()));
	//forkWidgetL->addStretch(1);
//
	//QHBoxLayout *missionsWidgetTL=new QHBoxLayout();missionsWidget->setLayout(missionsWidgetTL);
	//QVBoxLayout *missionsWidgetLeftL=new QVBoxLayout();missionsWidgetTL->addLayout(missionsWidgetLeftL);
	//QVBoxLayout *missionsWidgetRightL=new QVBoxLayout();missionsWidgetTL->addLayout(missionsWidgetRightL);
	//state
	//QHBoxLayout *missionsWidgetTL=new QHBoxLayout();missionsWidget->setLayout(missionsWidgetTL);
	QGroupBox *stateGroupBox=new QGroupBox(tr("State"));mainLayout->addWidget(stateGroupBox);
	QVBoxLayout *stateGroupBoxL=new QVBoxLayout();stateGroupBox->setLayout(stateGroupBoxL);

	eastConveyorActivePacketExportCheckBox=new QCheckBox(tr("East conveyor active packet export"));
	stateGroupBoxL->addWidget(eastConveyorActivePacketExportCheckBox);
	eastConveyorActivePacketImportCheckBox=new QCheckBox(tr("East conveyor active packet import"));
	stateGroupBoxL->addWidget(eastConveyorActivePacketImportCheckBox);
	eastConveyorActiveCassetteImportCheckBox=new QCheckBox(tr("East conveyor active cassete import"));
	stateGroupBoxL->addWidget(eastConveyorActiveCassetteImportCheckBox);

	westConveyorActivePacketExportCheckBox=new QCheckBox(tr("West conveyor active packet export"));
	stateGroupBoxL->addWidget(westConveyorActivePacketExportCheckBox);
	westConveyorActivePacketImportCheckBox=new QCheckBox(tr("West conveyor active packet import"));
	stateGroupBoxL->addWidget(westConveyorActivePacketImportCheckBox);
	westConveyorActiveCassetteImportCheckBox=new QCheckBox(tr("West conveyor active cassete import"));
	stateGroupBoxL->addWidget(westConveyorActiveCassetteImportCheckBox);
	QPushButton *saveStatesBtn=new QPushButton(tr("Save"));
	stateGroupBoxL->addWidget(saveStatesBtn);
	connect(saveStatesBtn,SIGNAL(clicked()),this,SLOT(saveStatesBtnSlot()));
	stateGroupBoxL->addStretch(1);
	////fork
	//QGroupBox *forkGroupBox=new QGroupBox(tr("Fork"));missionsWidgetLeftL->addWidget(forkGroupBox);
	//QGridLayout *forkGroupBoxL=new QGridLayout();forkGroupBox->setLayout(forkGroupBoxL);
	//forkGroupBoxL->addWidget(new QLabel(tr("From")),0,0);
	//forkGroupBoxL->addWidget(new QLabel(tr("To")),0,1);
	//forkFromComboBox=new QComboBox();
	//forkGroupBoxL->addWidget(forkFromComboBox,1,0);
	//forkToComboBox=new QComboBox();
	//forkGroupBoxL->addWidget(forkToComboBox,1,1);
	//QPushButton *forkStartBtn=new QPushButton(tr("Start"));
	//forkGroupBoxL->addWidget(forkStartBtn,2,0,1,2);
	//connect(forkStartBtn,SIGNAL(clicked()),this,SLOT(startForkBtnSlot()));
	//forkFromComboBox->addItem(tr("New SB (6)"),6);
	//forkFromComboBox->addItem(tr("New packet (8)"),8);
	//forkFromComboBox->addItem(tr("Import (9)"),9);
	//forkFromComboBox->addItem(tr("West (24)"),24);
	//forkFromComboBox->addItem(tr("East (25)"),25);

	//forkToComboBox->addItem(tr("SB exit (21)"),21);
	//forkToComboBox->addItem(tr("Packet exit (22)"),22);
	//forkToComboBox->addItem(tr("Wharehouse exit (23)"),23);
	//forkToComboBox->addItem(tr("West (24)"),24);
	//forkToComboBox->addItem(tr("East (25)"),25);
	//forkToComboBox->addItem(tr("New SB (6)"),6);
	//forkToComboBox->addItem(tr("New packet (8)"),8);
	//forkGroupBoxL->setRowStretch(3,1);
	//missionsWidgetLeftL->addStretch(1);
	//
	//manualCassetteMovement=new manualCassetteMovementClass(crane);
	//missionsWidgetRightL->addWidget(manualCassetteMovement);
	//missionsWidgetL->addStretch(1);
	//missionsWidgetTL->addStretch(1);
	//
	QTimer *updateTimer=new QTimer(this);
	connect(updateTimer,SIGNAL(timeout()),this,SLOT(updateTimerSlot()));
	updateTimer->start(10000);
}
int manualMissionsClass::exec()
{
	updateGui();
	return QDialog::exec();
}
void manualMissionsClass::updateTimerSlot()
{
	if (this->isVisible())
		updateGui();
}
void manualMissionsClass::updateGui()
{
	//for (int i=0;i<cells.count();i++)
	//	cells[i]->updateGui();
	//for (int i=0;i<packetPositions.count();i++)
	//	packetPositions[i]->updateGui();
	westConveyorActivePacketExportCheckBox  ->setChecked(getOne(QString("select value from state where name='westConveyorActivePacketExport'"),MAIN_DB_CONNECTION).toInt()==1);
	westConveyorActivePacketImportCheckBox  ->setChecked(getOne(QString("select value from state where name='westConveyorActivePacketImport'"),MAIN_DB_CONNECTION).toInt()==1);
	westConveyorActiveCassetteImportCheckBox->setChecked(getOne(QString("select value from state where name='westConveyorActiveCassetteImport'"),MAIN_DB_CONNECTION).toInt()==1);
	eastConveyorActivePacketExportCheckBox  ->setChecked(getOne(QString("select value from state where name='eastConveyorActivePacketExport'"),MAIN_DB_CONNECTION).toInt()==1);
	eastConveyorActivePacketImportCheckBox  ->setChecked(getOne(QString("select value from state where name='eastConveyorActivePacketImport'"),MAIN_DB_CONNECTION).toInt()==1);
	eastConveyorActiveCassetteImportCheckBox->setChecked(getOne(QString("select value from state where name='eastConveyorActiveCassetteImport'"),MAIN_DB_CONNECTION).toInt()==1);
	//manualCassetteMovement->updateFromDb();
}
//void manualMissionsClass::saveForkPosBtnSlot()
//{
//	//for (int i=0;i<packetPositions.count();i++)
//	//	packetPositions[i]->saveToDataBase();
//}
void manualMissionsClass::saveStatesBtnSlot()
{
	execQuery(QString("update state set value=%1 where name='westConveyorActivePacketExport'").arg(westConveyorActivePacketExportCheckBox->isChecked()?1:0),MAIN_DB_CONNECTION);
	execQuery(QString("update state set value=%1 where name='westConveyorActivePacketImport'").arg(westConveyorActivePacketImportCheckBox->isChecked()?1:0),MAIN_DB_CONNECTION);
	execQuery(QString("update state set value=%1 where name='westConveyorActiveCassetteImport'").arg(westConveyorActiveCassetteImportCheckBox->isChecked()?1:0),MAIN_DB_CONNECTION);
	execQuery(QString("update state set value=%1 where name='eastConveyorActivePacketExport'").arg(eastConveyorActivePacketExportCheckBox->isChecked()?1:0),MAIN_DB_CONNECTION);
	execQuery(QString("update state set value=%1 where name='eastConveyorActivePacketImport'").arg(eastConveyorActivePacketImportCheckBox->isChecked()?1:0),MAIN_DB_CONNECTION);
	execQuery(QString("update state set value=%1 where name='eastConveyorActiveCassetteImport'").arg(eastConveyorActiveCassetteImportCheckBox->isChecked()?1:0),MAIN_DB_CONNECTION);
	updateGui();
}
//void manualMissionsClass::startForkBtnSlot()
//{
//	//fork->newMission(forkFromComboBox->currentData().toInt(),forkToComboBox->currentData().toInt());
//}
//void manualMissionsClass::startCraneBtnSlot()
//{
//}

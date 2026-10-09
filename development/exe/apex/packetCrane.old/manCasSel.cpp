#include "manCasSel.h"
manualCassetteMovementClass::manualCassetteMovementClass(craneClass *crane_):QWidget()
{
	resize(1475,649);
	crane=crane_;
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	QScrollArea *mainScrollArea=new QScrollArea();
	mainScrollArea->setBackgroundRole(QPalette::Light);
	mainLayout->addWidget(mainScrollArea);
	QWidget *mainScrollAreaWidget=new QWidget();
	QHBoxLayout *mainScrollAreaWidgetLayout=new QHBoxLayout();mainScrollAreaWidget->setLayout(mainScrollAreaWidgetLayout);

	QVBoxLayout *eastLayout=new QVBoxLayout();mainScrollAreaWidgetLayout->addLayout(eastLayout);
	QVBoxLayout *spaceLayout=new QVBoxLayout();mainScrollAreaWidgetLayout->addLayout(spaceLayout,1);
	QVBoxLayout *westLayout=new QVBoxLayout();mainScrollAreaWidgetLayout->addLayout(westLayout);

	QHBoxLayout *eastTopLayout=new QHBoxLayout();eastLayout->addLayout(eastTopLayout);
	eastConvLaybel=new QLabel(tr("East conveyor"));
	eastTopLayout->addWidget(eastConvLaybel);
	eastLayout->addWidget(eastGroup0=new groupOfCellsClass( 1,20,true,crane));
	eastLayout->addWidget(eastGroup1=new groupOfCellsClass(21,40,true,crane));
	eastLayout->addWidget(eastGroup2=new groupOfCellsClass(41,60,true,crane));
	returnCassetteOfEastConveyor=new QPushButton(QIcon(":/arrowMoveUp.png"),"");
	eastTopLayout->addWidget(returnCassetteOfEastConveyor);
	eastTopLayout->addStretch(1);
	connect(returnCassetteOfEastConveyor,SIGNAL(clicked()),this,SLOT(returnCassetteOfEastConveyorBtnSlot()));

	QHBoxLayout *westTopLayout=new QHBoxLayout();westLayout->addLayout(westTopLayout);
	westConvLaybel=new QLabel(tr("West conveyor"));
	westTopLayout->addWidget(westConvLaybel);
	westLayout->addWidget(westGroup0=new groupOfCellsClass( 1,20,false,crane));
	westLayout->addWidget(westGroup1=new groupOfCellsClass(21,40,false,crane));
	westLayout->addWidget(westGroup2=new groupOfCellsClass(41,60,false,crane));
	returnCassetteOfWestConveyor=new QPushButton(QIcon(":/arrowMoveUp.png"),"");
	westTopLayout->addWidget(returnCassetteOfWestConveyor);
	westTopLayout->addStretch(1);
	connect(returnCassetteOfWestConveyor,SIGNAL(clicked()),this,SLOT(returnCassetteOfWestConveyorBtnSlot()));

	updateFromDb();
	mainScrollArea->setWidget(mainScrollAreaWidget);
	//
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimer()));
	onMainTimer();
	mainTimer.start(1000);
}
manualCassetteMovementClass::~manualCassetteMovementClass()
{
	delete eastGroup0;
	delete eastGroup1;
	delete eastGroup2;
	delete westGroup0;
	delete westGroup1;
	delete westGroup2;
}
void manualCassetteMovementClass::updateFromDb()
{
	//
	int cassetteOfEast=GETONE(QString("select cassette from positions where posId=3025")).toInt();
	eastConvLaybel->setText(tr("East conveyor %1").arg((cassetteOfEast!=0?QString("cassette %1").arg(cassetteOfEast):"(no cassette)")));
	if(cassetteOfEast==0)
		returnCassetteOfEastConveyor->setEnabled(false);
	else
		returnCassetteOfEastConveyor->setEnabled(true);
	//
	int cassetteOfWest=GETONE(QString("select cassette from positions where posId=3024")).toInt();
	westConvLaybel->setText(tr("West conveyor %1").arg((cassetteOfWest!=0?QString("cassette %1").arg(cassetteOfWest):"(no cassette)")));
	if(cassetteOfWest==0)
		returnCassetteOfWestConveyor->setEnabled(false);
	else
		returnCassetteOfWestConveyor->setEnabled(true);


	eastGroup0->updateFromDb();
	eastGroup1->updateFromDb();
	eastGroup2->updateFromDb();
	westGroup0->updateFromDb();
	westGroup1->updateFromDb();
	westGroup2->updateFromDb();
}
void manualCassetteMovementClass::returnCassetteOfEastConveyorBtnSlot()
{
	int cassetteOnConveyor=GETONE("select cassette from Positions where posId=3025").toInt();
	if (cassetteOnConveyor==0)
	{
		QMessageBox::critical(this,tr("Warning "),tr("Conveyor is without a cassette!"));
		return;
	}
	int craneFromPos=GETONE("select value from state where name='craneFromPos                                      '").toInt();
	int craneToPos=GETONE("select value from state where name='craneToPos                                        '").toInt();
	if ((craneFromPos!=0)||(craneToPos!=0))
	{
		QMessageBox::critical(this,tr("Warning "),tr("Crane is on a mission!"));
		return;
	}
	int forkFromPos=GETONE("select value from state where name='forkFromPos                                       '").toInt();
	int forkToPos=GETONE("select value from state where name='forkToPos                                         '").toInt();
	if ((forkFromPos!=0)||(forkToPos!=0))
	{
		QMessageBox::critical(this,tr("Warning "),tr("Fork lift crane is on a mission!"));
		return;
	}
	//
	int activePacketExport=GETONE("select value from state where name='eastConveyorActivePacketExport                    '").toInt();
	int activePacketImport=GETONE("select value from state where name='eastConveyorActivePacketImport                    '").toInt();
	int activeCassetteImport=GETONE("select value from state where name='eastConveyorActiveCassetteImport                  '").toInt();
	if ((activePacketExport!=0)||(activePacketImport!=0)||(activeCassetteImport!=0))
	{
		QMessageBox::critical(this,tr("Warning "),tr("Conveyor is active!"));
		return;
	}
	//
	crane->newMission(3025,cassetteOnConveyor,false,true);
}
void manualCassetteMovementClass::returnCassetteOfWestConveyorBtnSlot()
{
	int cassetteOnConveyor=GETONE("select cassette from Positions where posId=3024").toInt();
	if (cassetteOnConveyor==0)
	{
		QMessageBox::critical(this,tr("Warning "),tr("Conveyor is without a cassette!"));
		return;
	}
	int craneFromPos=GETONE("select value from state where name='craneFromPos                                      '").toInt();
	int craneToPos=GETONE("select value from state where name='craneToPos                                        '").toInt();
	if ((craneFromPos!=0)||(craneToPos!=0))
	{
		QMessageBox::critical(this,tr("Warning "),tr("Crane is on a mission!"));
		return;
	}
	int forkFromPos=GETONE("select value from state where name='forkFromPos                                       '").toInt();
	int forkToPos=GETONE("select value from state where name='forkToPos                                         '").toInt();
	if ((forkFromPos!=0)||(forkToPos!=0))
	{
		QMessageBox::critical(this,tr("Warning "),tr("Fork lift crane is on a mission!"));
		return;
	}
	//
	int activePacketExport=GETONE("select value from state where name='westConveyorActivePacketExport                    '").toInt();
	int activePacketImport=GETONE("select value from state where name='westConveyorActivePacketImport                    '").toInt();
	int activeCassetteImport=GETONE("select value from state where name='westConveyorActiveCassetteImport                  '").toInt();
	if ((activePacketExport!=0)||(activePacketImport!=0)||(activeCassetteImport!=0))
	{
		QMessageBox::critical(this,tr("Warning "),tr("Conveyor is active!"));
		return;
	}
	//
	crane->newMission(3024,cassetteOnConveyor,false,true);
}
void manualCassetteMovementClass::onMainTimer()
{
	setWindowTitle(tr("manual cassette movements","main window title..")+QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss"));
}

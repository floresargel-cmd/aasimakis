#include "pch.h"
#include "manCellDlg.h"
cellContClass::cellContClass(int cellNumber_,craneClass *crane_,QWidget *p):QDialog(p)
{
	crane=crane_;
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
	//takeCassetteToEastConveyorBtn
	QPushButton *takeCassetteToEastConveyorBtn=new QPushButton(tr("Take to east conveyor"));
	mainLayout->addWidget(takeCassetteToEastConveyorBtn,4,0);
	connect(takeCassetteToEastConveyorBtn,SIGNAL(clicked()),this,SLOT(takeCassetteToEastConveyorBtnClickedSlot()));
	//takeCassetteToWestConveyorBtn
	QPushButton *takeCassetteToWestConveyorBtn=new QPushButton(tr("Take to west conveyor"));
	mainLayout->addWidget(takeCassetteToWestConveyorBtn,4,1);
	connect(takeCassetteToWestConveyorBtn,SIGNAL(clicked()),this,SLOT(takeCassetteToWestConveyorBtnClickedSlot()));
}
void cellContClass::takeCassetteToEastConveyorBtnClickedSlot()
{
	int cassetteOnConveyor=GETONE("select cassette from Positions where posId=3025").toInt();
	if (cassetteOnConveyor!=0)
	{
		QMessageBox::critical(this,tr("Warning "),tr("East conveyor is with a cassette!"));
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
	crane->newMission(cellNumber,3025,false,true);
}
void cellContClass::takeCassetteToWestConveyorBtnClickedSlot()
{
	int cassetteOnConveyor=GETONE("select cassette from Positions where posId=3024").toInt();
	if (cassetteOnConveyor!=0)
	{
		QMessageBox::critical(this,tr("Warning "),tr("West conveyor is with a cassette!"));
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
	crane->newMission(cellNumber,3024,false,true);
}

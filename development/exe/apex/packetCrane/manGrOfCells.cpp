#include "pch.h"
#include "manGrOfCells.h"
groupOfCellsClass::groupOfCellsClass(int fromColumn_,int toColumn_,bool positiveCells,craneClass *crane_)
{
	fromColumn=fromColumn_;
	toColumn=toColumn_;
	crane=crane_;
	//
	QGridLayout *mainLayout=new QGridLayout();setLayout(mainLayout);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	int currentCol=0;
	int currentRow=0;
	for (int c=fromColumn;c<=toColumn;c++)
	{
		currentCol++;
		currentRow=0;
		mainLayout->addWidget(new QLabel(tr("%1").arg(c)),0,currentCol,1,1,Qt::AlignCenter);
		for (int r=13;r>=1;r--)
		{
			if (c==fromColumn)//first time only
				mainLayout->addWidget(new QLabel(tr("%1").arg(r)),13-r+1,0);
			currentRow++;
			cellLabelClass *tCell=new cellLabelClass(GETONE(QString("select posId from positions where col=%1 and row=%2 %3").arg(c).arg(r).arg(positiveCells?"and posId>0":"and posId<0")).toInt(),r,c,crane);
			cellsVector<<tCell;
			mainLayout->addWidget(tCell,currentRow,currentCol);
		}
	}
}
groupOfCellsClass::~groupOfCellsClass()
{
	for (int i=0;i<cellsVector.count();i++)
		delete cellsVector[i];
}
void groupOfCellsClass::updateGuiFromData(QVector<QVector<QVariant>> dataVV)
{
	for (int i=0;i<cellsVector.count();i++)
	{
		if (dataVV[i][0].toInt()==cellsVector[i]->cellNumber)
			cellsVector[i]->updateGuiFromData(dataVV[i][1].toInt(),dataVV[i][2].toInt());
	}
}

#include "gC.h"
groupOfCellsClass::groupOfCellsClass(int fromColumn_,int toColumn_,bool positiveCells)
{
	fromColumn=fromColumn_;
	toColumn=toColumn_;
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
			cellLabelClass *tCell=new cellLabelClass(GETONE(QString("select posId from positions where col=%1 and row=%2 %3").arg(c).arg(r).arg(positiveCells?"and posId>0":"and posId<0")).toInt(),r,c);
			cellsVector<<tCell;
			mainLayout->addWidget(tCell,currentRow,currentCol);
		}
	}
}
void groupOfCellsClass::writeToPdf(QPainter *painter,float y,float cellWidth,float cellHeight)
{
	painter->setFont(QFont("Arial",10));
	painter->setPen(QPen(QColor(255,100,100)));

	for (int r=13;r>=1;r--)
	{
	//	HPDF_Page_BeginText(*page);
	//	HPDF_Page_SetFontAndSize(*page,font,9);
	//	HPDF_Page_TextRect(*page,20,y+r*PDF_CELL_H,30,(y+r-1)*PDF_CELL_H,(QString("%1").arg(r)).toLocal8Bit().data(),HPDF_TALIGN_CENTER,NULL);
	//	HPDF_Page_TextRect(*page,630,y+r*PDF_CELL_H,640,(y+r-1)*PDF_CELL_H,(QString("%1").arg(r)).toLocal8Bit().data(),HPDF_TALIGN_CENTER,NULL);
	//	HPDF_Page_EndText(*page);
		painter->drawText(QRectF(0,y+cellHeight+r*cellHeight,cellWidth,cellHeight),Qt::AlignCenter,QString("%1").arg(r));
		painter->drawText(QRectF(cellWidth*(toColumn-fromColumn+2),y+cellHeight+r*cellHeight,cellWidth,cellHeight),Qt::AlignCenter,QString("%1").arg(r));
	}
	int c=0;
	for (int col=fromColumn;col<=toColumn;col++)
	{
		c++;
		painter->drawText(QRectF(c*cellWidth,y+cellHeight,cellWidth,cellHeight),Qt::AlignCenter,QString("%1").arg(col));
		painter->drawText(QRectF(c*cellWidth,y+15*cellHeight,cellWidth,cellHeight),Qt::AlignCenter,QString("%1").arg(col));
	//	HPDF_Page_BeginText(*page);
	//	HPDF_Page_SetFontAndSize(*page,font,9);
	//	HPDF_Page_TextRect(*page,c*PDF_CELL_W,y+14*PDF_CELL_H,(c+1)*PDF_CELL_W,y+13*PDF_CELL_H,(QString("%1").arg(col)).toLocal8Bit().data(),HPDF_TALIGN_CENTER,NULL);
	//	HPDF_Page_TextRect(*page,c*PDF_CELL_W,y,(c+1)*PDF_CELL_W,y-PDF_CELL_H,(QString("%1").arg(col)).toLocal8Bit().data(),HPDF_TALIGN_CENTER,NULL);
	//	HPDF_Page_EndText(*page);
	}
	painter->setFont(QFont("Arial",6));
	int cRow=0;int cCol=0;
	for (int i=0;i<cellsVector.count();i++)
	{
		if ((i%13)==0)
		{
			cCol++;
			cRow=0;
		}
		cRow++;
		if ((cRow%2)==0)
		{
			//painter->draw
	//		HPDF_Page_SetRGBFill (*page,0.9,0.9,0.99);
	//		HPDF_Page_Rectangle(*page,cCol*PDF_CELL_W,y+(13.-cRow)*PDF_CELL_H,PDF_CELL_W,PDF_CELL_H);
	//		HPDF_Page_FillStroke(*page);
		}
		cellsVector[i]->writeToPdf(painter,cCol*cellWidth,y+cRow*cellHeight+cellHeight,cellWidth,cellHeight);
	}
}

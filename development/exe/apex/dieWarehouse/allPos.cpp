#include "pch.h"
#include "allPos.h"
extern QSplashScreen *splash;
//#define notAllPos
allPossClass::allPossClass(tuxipClass *tuxipConnection,QGraphicsScene *layoutScene,QWidget *p):QWidget(p)
{
	tuxip=tuxipConnection;
	noOfRows=15;
	setVisible(false);
	manualSelectedFromPos=NULL;
	crane=NULL;
	dxfGroupOfGraphicsItemClass *cellGraphicsItem=new dxfGroupOfGraphicsItemClass(":dxfs/cell.dxf",QPen(QColor(200,200,250)),false,false,QColor(),this);

	dxfGroupOfGraphicsItemClass *normalMouseOver=new dxfGroupOfGraphicsItemClass(":dxfs/normalMouseOver.dxf",QPen(QColor(255,100,100)),true,true,QColor(255,100,100,150),this);
	dxfGroupOfGraphicsItemClass *normalEmptyBasket=new dxfGroupOfGraphicsItemClass(":dxfs/normalEmptyBasket.dxf",QPen(QColor(100,200,100)),false,false,QColor(),this);
	dxfGroupOfGraphicsItemClass *normalFullBasket=new dxfGroupOfGraphicsItemClass(":dxfs/normalFullBasket.dxf",QPen(QColor(100,100,200)),false,false,QColor(),this);
	dxfGroupOfGraphicsItemClass *normalLocked=new dxfGroupOfGraphicsItemClass(":dxfs/normalLocked.dxf",QPen(QColor(255,100,100)),false,false,QColor(),this);

	dxfGroupOfGraphicsItemClass *bigMouseOver=new dxfGroupOfGraphicsItemClass(":dxfs/bigMouseOver.dxf",QPen(QColor(255,100,100)),true,true,QColor(255,100,100,150),this);
	dxfGroupOfGraphicsItemClass *bigEmptyBasket=new dxfGroupOfGraphicsItemClass(":dxfs/bigEmptyBasket.dxf",QPen(QColor(100,200,100)),false,false,QColor(),this);
	dxfGroupOfGraphicsItemClass *bigFullBasket=new dxfGroupOfGraphicsItemClass(":dxfs/bigFullBasket.dxf",QPen(QColor(100,100,200)),false,false,QColor(),this);
	dxfGroupOfGraphicsItemClass *bigLocked=new dxfGroupOfGraphicsItemClass(":dxfs/bigLocked.dxf",QPen(QColor(255,100,100)),false,false,QColor(),this);

	splash->showMessage(tr("Loading cell data..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select plcId,basket,description,col,row,dx,dy,dz,type from pos where plcId>-10000 " ),db);//
#ifdef notAllPos
	QMessageBox::critical(0,"Fatal","Not alla pos");
	for (int i=0;i<50;i++)
#else
	for (int i=0;i<dataVV.count();i++)
#endif
	{
		splash->showMessage(tr("Loading cell data (%1%)").arg(100*i/dataVV.count()),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
		int plcId=dataVV[i][0].toInt();
		if (plcId==exitPlcId)
		{
			float plcXPos=0.f+dataVV[i][5].toFloat();
			float plcYPos=3050.f+dataVV[i][6].toFloat();
			float plcZPos=1437.f+dataVV[i][7].toFloat();
			workStation=new workStationClass(cellTypeBig,plcId,dataVV[i][1].toInt(),dataVV[i][2].toString(),dataVV[i][3].toInt(),dataVV[i][4].toInt(),plcXPos,plcYPos,plcZPos,dataVV[i][5].toFloat(),dataVV[i][6].toFloat(),dataVV[i][7].toFloat(),layoutScene,bigMouseOver,cellGraphicsItem,bigFullBasket,bigEmptyBasket,bigLocked,this);
			posV<<workStation;
			connect(posV.last(), SIGNAL(posCtrlMouseReleasedSignal(posClass*, QGraphicsSceneMouseEvent*)), this, SLOT(posCtrlMouseReleasedSlot(posClass*, QGraphicsSceneMouseEvent*)));
		}
		else if (plcId<cranePlcId)
		{
			int col=dataVV[i][3].toInt();
			int row=dataVV[i][4].toInt();
			float	plcXPos=592.f-5.f+(col-1)*491.f+dataVV[i][5].toFloat();
			if (col>=3)
				plcXPos+=208;
			if (col>=25)
				plcXPos+=210-6;
			float dz=370.f;
			if ((col<=2)||((col>=25)&&(col<=51)))
				dz=495.f;
			float plcZPos=(row-1)*dz+dataVV[i][7].toFloat();

			float plcYPos=3050.f+dataVV[i][6].toFloat();
			if (typeOfBasket(plcId)==cellTypeBig)
				posV<<new posClass(cellTypeBig,plcId,dataVV[i][1].toInt(),dataVV[i][2].toString(),col,row,plcXPos,plcYPos,plcZPos,dataVV[i][5].toFloat(),dataVV[i][6].toFloat(),dataVV[i][7].toFloat(),layoutScene,bigMouseOver,cellGraphicsItem,bigFullBasket,bigEmptyBasket,bigLocked,this);
			else
				posV<<new posClass(cellTypeNormal,plcId,dataVV[i][1].toInt(),dataVV[i][2].toString(),col,row,plcXPos,plcYPos,plcZPos,dataVV[i][5].toFloat(),dataVV[i][6].toFloat(),dataVV[i][7].toFloat(),layoutScene,normalMouseOver,cellGraphicsItem,normalFullBasket,normalEmptyBasket,normalLocked,this);
			connect(posV.last(),SIGNAL(posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
		}
	}
	//calculateCorrections();
}
allPossClass::~allPossClass()
{
}
void allPossClass::calculateCorrections()
{
	QVector<posCorrectionClass*> corV;
	int noOfRows=0;
	if (false)//big cells
	{
		noOfRows=13;
		corV<<new posCorrectionClass(  1,       1,    +0.f,    +0.f, +3.f);
		corV<<new posCorrectionClass(  2,       1,    +0.f,    +0.f, +3.f);//
		corV<<new posCorrectionClass(  1,noOfRows,    +0.f,    +0.f, +3.f);
		corV<<new posCorrectionClass(  2,noOfRows,    +0.f,    +0.f, +3.f);//

	}
	else if (false)//small cells
	{
		noOfRows=17;
		corV<<new posCorrectionClass(  3, 1,					+0.f,    +0.f, +3.f);
		corV<<new posCorrectionClass( 24, 1,					-7.f,    +0.f, +3.f);//
		corV<<new posCorrectionClass(  3,noOfRows,    +0.f,    +0.f, +3.f);
		corV<<new posCorrectionClass( 24,noOfRows,   +-5.f,    +0.f, +3.f);//

	}
	else if (false) //big cells
	{
		noOfRows=13;
		corV<<new posCorrectionClass( 25, 1,    +0.f,    +0.f, +3.f);
		corV<<new posCorrectionClass( 31, 1,    -3.f,    +0.f, +3.f);//

		corV<<new posCorrectionClass( 25,noOfRows,    +0.f,    +0.f, +3.f);
		corV<<new posCorrectionClass( 31,noOfRows,    -2.f,    +0.f, +3.f);//
	}
	else if (false)//big cells
	{
		noOfRows=13;
		corV<<new posCorrectionClass( 32, 1,					+0.f,    +0.f, +3.f);
		corV<<new posCorrectionClass( 51, 1,					+0.f,    +0.f, +3.f);//
		corV<<new posCorrectionClass( 32,noOfRows,    +0.f,    +0.f, +3.f);
		corV<<new posCorrectionClass( 51,noOfRows,    +0.f,    +0.f, +3.f);//

	}
	else if (false) //small cells
	{
		noOfRows=17;
		corV<<new posCorrectionClass( 52, 1,					+0.f,    +0.f, +3.f);
		corV<<new posCorrectionClass( 61, 1,					+0.f,    +0.f, +3.f);//

		corV<<new posCorrectionClass( 52,noOfRows,    +0.f,    +0.f, +3.f);
		corV<<new posCorrectionClass( 61,noOfRows,    +0.f,    +0.f, +3.f);//
	}
	//fill bottom and top rows
	for (int i=0;i<posV.count();i++)
	{
		splash->showMessage(tr("Corrections pass 1 (%1)").arg(100*i/posV.count()),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
		if (posV[i]->getPlcId()>=cranePlcId)
			continue;
		if (posV[i]->getRow()==1)
		{
			posCorrectionClass *c=NULL;
			posCorrectionClass *c0=NULL;
			posCorrectionClass *c1=NULL;
			for (int j=0;j<corV.count();j++)
			{
				if (corV[j]->row==1)
				{
					if (corV[j]->col<(posV[i]->getColumnWithSign()))
						c0=corV[j];
					if (corV[j]->col>posV[i]->getColumnWithSign())
					{
						c1=corV[j];
						if (c0)
							break;
					}
					if (corV[j]->col==posV[i]->getColumnWithSign())
						c=corV[j];
				}
			}
			if (c)
			{
				posV[i]->setPlcXCorrection(c->x);
				posV[i]->setPlcYCorrection(c->y);
				posV[i]->setPlcZCorrection(c->z);
				posV[i]->updateGui();
			}
			else if ((c0!=NULL)&&(c1!=NULL))
			{
				posV[i]->setPlcXCorrection(c0->x+(posV[i]->getColumnWithSign()-c0->col)*(c1->x-c0->x)/(c1->col-c0->col));
				posV[i]->setPlcYCorrection(c0->y+(posV[i]->getColumnWithSign()-c0->col)*(c1->y-c0->y)/(c1->col-c0->col));
				posV[i]->setPlcZCorrection(c0->z+(posV[i]->getColumnWithSign()-c0->col)*(c1->z-c0->z)/(c1->col-c0->col));
				posV[i]->updateGui();
			}
		}
		if (posV[i]->getRow()==noOfRows)
		{
			posCorrectionClass *c=NULL;
			posCorrectionClass *c0=NULL;
			posCorrectionClass *c1=NULL;
			for (int j=0;j<corV.count();j++)
			{
				if (corV[j]->row==noOfRows)
				{
					if (corV[j]->col<posV[i]->getColumnWithSign())
						c0=corV[j];
					if (corV[j]->col>posV[i]->getColumnWithSign())
					{
						c1=corV[j];
						if (c0)
							break;
					}
					if (corV[j]->col==posV[i]->getColumnWithSign())
						c=corV[j];
				}
			}
			if ((c0!=NULL)&&(c1!=NULL))
			{
				posV[i]->setPlcXCorrection(c0->x+(posV[i]->getColumnWithSign()-c0->col)*(c1->x-c0->x)/(c1->col-c0->col));
				posV[i]->setPlcYCorrection(c0->y+(posV[i]->getColumnWithSign()-c0->col)*(c1->y-c0->y)/(c1->col-c0->col));
				posV[i]->setPlcZCorrection(c0->z+(posV[i]->getColumnWithSign()-c0->col)*(c1->z-c0->z)/(c1->col-c0->col));
				posV[i]->updateGui();
			}
			if (c)
			{
				posV[i]->setPlcXCorrection(c->x);
				posV[i]->setPlcYCorrection(c->y);
				posV[i]->setPlcZCorrection(c->z);
				posV[i]->updateGui();
			}
		}
	}
	//fill rest
	for (int i=0;i<posV.count();i++)
	{
		splash->showMessage(tr("Corrections pass 2 (%1)").arg(100*i/posV.count()),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
		if (posV[i]->getPlcId()>=cranePlcId)
			continue;
		if ((posV[i]->getRow()==1)||(posV[i]->getRow()==noOfRows))
			continue;
		posClass *r0=getPosWithColumnRow(posV[i]->getColumnWithSign(),1);
		posClass *r1=getPosWithColumnRow(posV[i]->getColumnWithSign(),noOfRows);
		if ((r0!=NULL)&&(r1!=NULL))
		{
			posV[i]->setPlcXCorrection(r0->getPlcXCorrection()+(posV[i]->getRow()-r0->getRow())*(r1->getPlcXCorrection()-r0->getPlcXCorrection())/(r1->getRow()-r0->getRow()));
			posV[i]->setPlcYCorrection(r0->getPlcYCorrection()+(posV[i]->getRow()-r0->getRow())*(r1->getPlcYCorrection()-r0->getPlcYCorrection())/(r1->getRow()-r0->getRow()));
			posV[i]->setPlcZCorrection(r0->getPlcZCorrection()+(posV[i]->getRow()-r0->getRow())*(r1->getPlcZCorrection()-r0->getPlcZCorrection())/(r1->getRow()-r0->getRow()));
			posV[i]->updateGui();
		}
	}
}
posClass* allPossClass::getPosWithColumnRow(int col,int row)
{
	for (int i=0;i<posV.count();i++)
	{
		if ((posV[i]->getColumnWithSign()==col)&&(posV[i]->getRow()==row))
			return posV[i];
	}
	return NULL;
}
void allPossClass::setCraneClass(craneClass *c)
{
	crane=c;
}
void allPossClass::highLightPosWithBasket(int b)
{
	for (int i=0;i<posV.count();i++)
		posV[i]->setIsHighlited(false);
	posClass *h=getCellWithBasket(b,false);
	if (h)
		h->setIsHighlited(true);
}
posClass* allPossClass::getPosWithId(int id,bool unLockedOnly)
{
	posClass* ans=NULL;
	for (int i=0;i<posV.count();i++)
	{
		if (posV[i]->getPlcId()==id)
		{
			if (!unLockedOnly)
				ans=posV[i];
			else if (!posV[i]->getIsLocked())
				ans=posV[i];
			break;
		}
	}
	if (!ans)
	{
		log(QString("getPosWithId id:%1 returning null").arg(id),errorStr);
		uExit("Internal error",QString("getPosWithId id:%1 returning null").arg(id));
	}
	return ans;
}
posClass* allPossClass::getCellWithBasket(int b,bool unLockedOnly)
{
	posClass* ans=NULL;
	for (int i=0;i<posV.count();i++)
	{
		if ((posV[i]->getPlcId()<cranePlcId)&&(posV[i]->getBasketNumber()==b))
		{
			if (!unLockedOnly)
				ans=posV[i];
			else if (!posV[i]->getIsLocked())
				ans=posV[i];
			break;
		}
	}
	return ans;
}

posClass* allPossClass::getEmptyCellForBasket(int b)
{
	posClass* ans=NULL;
	//int type=getOne(QString("select type from baskets where basket=%1").arg(b),db).toInt();

	//int lowRowsPosition=getFirst(QString("select top 1 plcId from pos where basket=0 and locked=0 and row>0 and row<=5 and plcId<%1 and plcId not in (select fromPos from missions) and plcId not in (select toPos from missions) order by col,row %2").arg(craneAPlcId).arg(type==posClass::basketTypeNormal?"asc":"desc"),db).toInt();
	//if (lowRowsPosition!=0)ans=getPosWithId(lowRowsPosition);
	//if (ans)return ans;
	//int midRowsPosition=getFirst(QString("select top 1 plcId from pos where basket=0 and locked=0 and row>5 and row<=10 and plcId<%1 and plcId not in (select fromPos from missions) and plcId not in (select toPos from missions) order by col,row %2").arg(craneAPlcId).arg(type==posClass::basketTypeNormal?"asc":"desc"),db).toInt();
	//if (midRowsPosition!=0)ans=getPosWithId(midRowsPosition);
	//if (ans)return ans;
	//int highRowsPosition=getFirst(QString("select top 1 plcId from pos where basket=0 and locked=0 and row>10 and row<15 and plcId<%1 and plcId not in (select fromPos from missions) and plcId not in (select toPos from missions) order by col,row %2").arg(craneAPlcId).arg(type==posClass::basketTypeNormal?"asc":"desc"),db).toInt();
	//if (highRowsPosition!=0)ans=getPosWithId(highRowsPosition);
	//if (ans)return ans;
	//if (!ans)
	//{
	//	log(QString("getEmptyCellForBasket b:%1 returning null").arg(b),errorStr);
	//	//exit("Internal error",QString("getPosWithId id:%1 returning null").arg(b));
	//}
	return ans;
}
workStationClass *allPossClass::getWorkStation()
{
	return workStation;
}
void allPossClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==dieIp)
	{
	}
}
void allPossClass::drawColForReport(int col,int dW,int dH,int noOfColumnsInPage,int firstcol,bool rightSideCells,QPainter *painter)
{
	for (int i=0;i<posV.count();i++)
	{
		if (   ((!rightSideCells)&&(posV[i]->getPlcId()>0))    ||   ((rightSideCells)&&(posV[i]->getPlcId()<0))   )
		{
			if (posV[i]->getColumn()==col)
			{
				painter->setPen(QPen(QBrush(QColor(150,150,150)),0));
				int drawCol=0;
				if (!rightSideCells)
					drawCol=posV[i]->getColumn()-firstcol-1;
				else
					drawCol=noOfColumnsInPage+firstcol-posV[i]->getColumn();
				painter->drawText(drawCol*dW,dH+noOfRows*dH-posV[i]->getRow()*dH,dW,dH,Qt::AlignBottom|Qt::AlignHCenter,QString("%1").arg(posV[i]->getPlcId()));
				painter->setPen(QPen(QBrush(QColor(0,0,0)),0));
				if (posV[i]->getBasketNumber()>0)
					painter->drawText(drawCol*dW,dH+noOfRows*dH-posV[i]->getRow()*dH,dW,dH,Qt::AlignTop|Qt::AlignHCenter,QString("%1").arg(posV[i]->getBasketNumber()));
			}
		}
	}
}
void allPossClass::drawPageLayout(int maxW,int maxH,int dW,int dH,int noOfColumnsInPage,int firstcol,QString headerText,QString footerText,bool rightSideCells,QPainter *painter)
{
	painter->setPen(QPen(QBrush(QColor(0,0,0)),0));
	int y=dH;
	for (int i=0;i<=noOfRows;i++)
	{
		painter->drawLine(0,y,maxW,y);
		y+=dH;
	}
	y-=dH;
	for (int i=0;i<=noOfColumnsInPage;i++)
	{
		painter->drawLine(i*dW,dH,i*dW,y);
		if (rightSideCells)
			painter->drawText(i*dW,y,dW,0.25*dH,Qt::AlignCenter,QString("%1").arg(noOfColumnsInPage+firstcol-i));
		else
			painter->drawText(i*dW,y,dW,0.25*dH,Qt::AlignCenter,QString("%1").arg(firstcol+i+1));
	}
	painter->drawText(0,0,maxW,dH,Qt::AlignHCenter|Qt::AlignBottom,headerText);
	painter->drawText(0,maxH-0.25*dH,maxW,0.25*dH,Qt::AlignRight|Qt::AlignVCenter,footerText);
}
void allPossClass::drawForReport(QString pdfFile)
{
	int noOfColumnsInPage=22;
	QPrinter printer(QPrinter::HighResolution);
	float scale=46;
	int W=297*scale;
	int H=208*scale;
	int dW=W/noOfColumnsInPage;
	int dH=0.9*H/noOfRows;
	printer.setOutputFileName(pdfFile);
	printer.setOrientation(QPrinter::Landscape);
#if QT_VERSION > QT_VERSION_CHECK(5, 1, 1)
	printer.setPageSize(QPageSize(QPageSize::A4));
#else
	printer.setPageSize(QPrinter::A4);
#endif
	QString footerText=tr("warehouse report, created on:%1").arg(timeVerbose);
	QPainter painter;
	painter.begin(&printer);
	//painter.setTransform(QTransform(scale,0.,0.,scale,0.,0.),false);

	int columnWidthInReport=(280./noOfColumnsInPage)*scale;//280mm of 297
	int colsReported=0;
	//left cells
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,0,tr("Left cells"),footerText,false,&painter);
	for (int i=0;i<=21;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,0,false,&painter);
	printer.newPage();
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,22,tr("Left cells"),footerText,false,&painter);
	for (int i=22;i<=43;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,22,false,&painter);
	printer.newPage();
	//right cells
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,0,tr("Right cells"),footerText,true,&painter);
	for (int i=0;i<22;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,0,true,&painter);
	printer.newPage();
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,22,tr("Right cells"),footerText,true,&painter);
	for (int i=22;i<44;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,22,true,&painter);

	painter.end();
}
void allPossClass::posCtrlMouseReleasedSlot(posClass *p,QGraphicsSceneMouseEvent *ev)
{
	if (!adminMode)
		return;
	if (p->getIsLocked())
		return;
	if (!manualSelectedFromPos)
	{
		if (p->getBasketNumber()!=0)
		{
			manualSelectedFromPos=p;
			manualSelectedFromPos->setIsSelected(true);
		}
	}
	else
	{
		if ((manualSelectedFromPos==p)||(p->getBasketNumber()>0))
		{
			manualSelectedFromPos->setIsSelected(false);
			manualSelectedFromPos=NULL;
			return;
		}
		if (ev->button()==Qt::LeftButton)
			crane->sendManualMission(manualSelectedFromPos,p);
		manualSelectedFromPos->setIsSelected(false);
		manualSelectedFromPos=NULL;
	}
}
QString allPossClass::getPosOfBasket(int basket,int ignorePos)
{
	for (int i=0;i<posV.count();i++)
	{
		if (posV[i]->getBasketNumber()==basket)
		{
			if (posV[i]->getPlcId()!=ignorePos)
				return tr("%1").arg(posV[i]->getPlcId());
			return QString();
		}
	}
	return QString();
}

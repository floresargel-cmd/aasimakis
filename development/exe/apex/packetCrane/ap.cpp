#include "pch.h"
#include "ap.h"
extern QSplashScreen *splash;
allPosisionsClass::allPosisionsClass(QGraphicsScene *scene,QWidget *p):QWidget(p)
{
	QWidget::hide();
	splash->showMessage(tr("Loading cell data..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	cranePlcTcpServer=NULL;
	type1EmptyCassettes=type2EmptyCassettes=type3EmptyCassettes=0;
	positionCrane=position24=position25=positionStataionImport=NULL;
	//initialize
	dxfClass *cellItem=new dxfClass(":dxfs/cell.dxf",QPen(QColor(200,200,200)),false,false,QColor(),this);
	dxfClass *mouseOverItem=new dxfClass(":dxfs/posMouseOver.dxf",QPen(QColor(255,100,100)),true,true,QColor(255,100,100,150),this);
	dxfClass *lockedItem=new dxfClass(":dxfs/posLocked.dxf",QPen(QColor(200,50,50)),false,false,QColor(),this);
	dxfClass *cassetteItem=new dxfClass(":dxfs/cassette.dxf",QPen(QColor(150,150,150)),false,false,QColor(),this);
	dxfClass *sbItem=new dxfClass(":dxfs/sb.dxf",QPen(QColor(50,50,150)),false,false,QColor(),this);
	dxfClass *fromItem=new dxfClass(":dxfs/from.dxf",QPen(QColor(50,50,150)),false,false,QColor(),this);
	dxfClass *toItem=new dxfClass(":dxfs/to.dxf",QPen(QColor(50,150,50)),false,false,QColor(),this);
	int noOfCells=1560;//1560
	execQuery(QString("update packets set packetId=-1 where packetId=0"),MAIN_DB_CONNECTION);//gives multiple rows in the next query
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top %1 posId,posName,positions.packetId,cassette,acceptableContents,positions.storagingType,isLocked,col,row,acceptableLength,xCorr,zCorr,packets.sb from positions left join packets on packets.packetId=positions.packetId where posId<>0 and posid<%2").arg(noOfCells).arg(MAX_CELL_POS_ID),MAIN_DB_CONNECTION);
	QVector<int> ignoreCellsV;ignoreCellsV<<511<<512<<524<<525<<537<<538<<-576<<-577<<-589<<-590<<-602<<-603;
	for (int i=0;i<dataVV.count();i++)
	{
		//if ( (allPosIdsArray[i].toInt()==394)||(allPosIdsArray[i].toInt()==395)||(allPosIdsArray[i].toInt()==407)||(allPosIdsArray[i].toInt()==408)||(allPosIdsArray[i].toInt()==420)||(allPosIdsArray[i].toInt()==421) )
		//	continue;6
		splash->showMessage(tr("Loading cell data (%1%)").arg(100*i/noOfCells),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
		int c=0;
		int posId=dataVV[i][c++].toInt();
		if (posId==1)
			double a=0.;
		QString posName=dataVV[i][c++].toString().trimmed();
		int packetId=dataVV[i][c++].toInt();
		int cassette=dataVV[i][c++].toInt();
		int acceptableContents=dataVV[i][c++].toInt();
		int storagingType=dataVV[i][c++].toInt();
		int isLocked=dataVV[i][c++].toInt();
		int col=dataVV[i][c++].toInt();
		int row=dataVV[i][c++].toInt();
		int acceptableLength=dataVV[i][c++].toInt();
		int xCorr=dataVV[i][c++].toInt();
		int zCorr=dataVV[i][c++].toInt();
		bool isOnSb=(dataVV[i][c++].toInt()==PACKET_WITH_SB);
		if (ignoreCellsV.contains(posId))
			continue;
		allPositionList<<new cellPositionClass(posId,posName,packetId,isOnSb,cassette,acceptableContents,storagingType,isLocked,col,row,xCorr,zCorr,acceptableLength,scene,cellItem,mouseOverItem,lockedItem,cassetteItem,sbItem,fromItem,toItem,p);
		connect(allPositionList.last(),SIGNAL(mouseReleasedSignal(packetPositionClass*,QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedOnPacketPosSlot(packetPositionClass*,QGraphicsSceneMouseEvent*)));
	}
	splash->showMessage(tr("Loading cell data Done"),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	//
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer.start(500);
	getEmptyCassetteForPacketIn(NULL,false,false);

	if (false)
	{
		double bottomCorr[6]={-35.,-33.,-27.,-29.,-38.,-58.};
		double topFromBottomCorr[6]={+20.,-4,+17.,-4.,+12.,-14.};
		QVector<posCorrectionClass*> corV;
		if (false)//new
		{
			corV<<new posCorrectionClass(-60, 1,bottomCorr[0],+0.f);
			corV<<new posCorrectionClass(-30, 1,bottomCorr[1],+0.f); 
			corV<<new posCorrectionClass( -1, 1,bottomCorr[2],+0.f); 
			corV<<new posCorrectionClass( +1, 1,bottomCorr[3],+0.f); 
			corV<<new posCorrectionClass(+30, 1,bottomCorr[4],+0.f); 
			corV<<new posCorrectionClass(+60, 1,bottomCorr[5],+0.f); 

			corV<<new posCorrectionClass(-60,13,topFromBottomCorr[0]+bottomCorr[0],-6.f);	//+20
			corV<<new posCorrectionClass(-30,13,topFromBottomCorr[1]+bottomCorr[1],-6.f);	//-4
			corV<<new posCorrectionClass( -1,13,topFromBottomCorr[2]+bottomCorr[2],-6.f);	//+17
			corV<<new posCorrectionClass( +1,13,topFromBottomCorr[3]+bottomCorr[3],-6.f);	//-4
			corV<<new posCorrectionClass(+30,13,topFromBottomCorr[4]+bottomCorr[4],-6.f);	//+12
			corV<<new posCorrectionClass(+60,13,topFromBottomCorr[5]+bottomCorr[5],-6.f);	//-14
		}
		if (true)//old
		{
			corV<<new posCorrectionClass(-60, 1, +10.f ,+0.f); 
			corV<<new posCorrectionClass(-30, 1,  +2.f ,+0.f); 
			corV<<new posCorrectionClass( -1, 1, -20.f ,+0.f); 
			corV<<new posCorrectionClass( +1, 1, +10.f ,+0.f); 
			corV<<new posCorrectionClass(+30, 1, +15.f ,+0.f); 
			corV<<new posCorrectionClass(+60, 1, +40.f ,+0.f); 

			corV<<new posCorrectionClass(-60,13, +30.f ,+0.f);
			corV<<new posCorrectionClass(-30,13, -2.f  ,+0.f);
			corV<<new posCorrectionClass( -1,13, -3.f  ,+0.f);
			corV<<new posCorrectionClass( +1,13, +6.f  ,+0.f);
			corV<<new posCorrectionClass(+30,13,+27.f  ,+0.f);
			corV<<new posCorrectionClass(+60,13,+26.f  ,+0.f);
		}
	 calculateCorrections(13,corV);
	}
}
void allPosisionsClass::calculateCorrections(int noOfRows,QVector<posCorrectionClass*> corV)	
{
	//fill bottom and top rows
	for (int i=0;i<allPositionList.count();i++)
	{
		int colWithDign=allPositionList[i]->getColumn()*(allPositionList[i]->getPosId()/abs(allPositionList[i]->getPosId()));
		splash->showMessage(tr("Corrections pass 1 (%1)").arg(100*i/allPositionList.count()),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
		//if (allPosV[i]->getPlcId()>=craneAPlcId)
		//	continue;
		if (allPositionList[i]->getRow()==1)
		{
			posCorrectionClass *c=NULL;
			posCorrectionClass *c0=NULL;
			posCorrectionClass *c1=NULL;
			for (int j=0;j<corV.count();j++)
			{
				if (corV[j]->row==1)
				{
					if (corV[j]->col<(colWithDign))
						c0=corV[j];
					if (corV[j]->col>colWithDign)
					{
						c1=corV[j];
						if (c0)
							break;
					}
					if (corV[j]->col==colWithDign)
						c=corV[j];
				}
			}
			if (c)
			{
				allPositionList[i]->setPlcXCorrection(c->x);
				allPositionList[i]->setPlcZCorrection(c->z);
				allPositionList[i]->updateGui();
			}
			else if ((c0!=NULL)&&(c1!=NULL))
			{
				allPositionList[i]->setPlcXCorrection(c0->x+(colWithDign-c0->col)*(c1->x-c0->x)/(c1->col-c0->col));
				allPositionList[i]->setPlcZCorrection(c0->z+(colWithDign-c0->col)*(c1->z-c0->z)/(c1->col-c0->col));
				allPositionList[i]->updateGui();
			}
		}
		if (allPositionList[i]->getRow()==noOfRows)
		{
			posCorrectionClass *c=NULL;
			posCorrectionClass *c0=NULL;
			posCorrectionClass *c1=NULL;
			for (int j=0;j<corV.count();j++)
			{
				if (corV[j]->row==noOfRows)
				{
					if (corV[j]->col<colWithDign)
						c0=corV[j];
					if (corV[j]->col>colWithDign)
					{
						c1=corV[j];
						if (c0)
							break;
					}
					if (corV[j]->col==colWithDign)
						c=corV[j];
				}
			}
			if ((c0!=NULL)&&(c1!=NULL))
			{
				allPositionList[i]->setPlcXCorrection(c0->x+(colWithDign-c0->col)*(c1->x-c0->x)/(c1->col-c0->col));
				allPositionList[i]->setPlcZCorrection(c0->z+(colWithDign-c0->col)*(c1->z-c0->z)/(c1->col-c0->col));
				allPositionList[i]->updateGui();
			}
			if (c)
			{
				allPositionList[i]->setPlcXCorrection(c->x);
				allPositionList[i]->setPlcZCorrection(c->z);
				allPositionList[i]->updateGui();
			}
		}
	}
	//fill rest
	for (int i=0;i<allPositionList.count();i++)
	{
		int colWithDign=allPositionList[i]->getColumn()*(allPositionList[i]->getPosId()/abs(allPositionList[i]->getPosId()));
		splash->showMessage(tr("Corrections pass 2 (%1)").arg(100*i/allPositionList.count()),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
		if (allPositionList[i]->getPosId()>=MAX_CELL_POS_ID)
			continue;
		if ((allPositionList[i]->getRow()==1)||(allPositionList[i]->getRow()==noOfRows))
			continue;
		cellPositionClass *r0=getPosWithColumnRow(colWithDign,1);
		cellPositionClass *r1=getPosWithColumnRow(colWithDign,noOfRows);
		if ((r0!=NULL)&&(r1!=NULL))
		{
			allPositionList[i]->setPlcXCorrection(r0->getXCorr()+(allPositionList[i]->getRow()-r0->getRow())*(r1->getXCorr()-r0->getXCorr())/(r1->getRow()-r0->getRow()));
			allPositionList[i]->setPlcZCorrection(r0->getZCorr()+(allPositionList[i]->getRow()-r0->getRow())*(r1->getZCorr()-r0->getZCorr())/(r1->getRow()-r0->getRow()));
			allPositionList[i]->updateGui();
		}
	}
}
cellPositionClass* allPosisionsClass::getPosWithColumnRow(int col,int row)
{
	for (int i=0;i<allPositionList.count();i++)
	{
		int colWithDign=allPositionList[i]->getColumn()*(allPositionList[i]->getPosId()/abs(allPositionList[i]->getPosId()));
		if ((colWithDign==col)&&(allPositionList[i]->getRow()==row))
			return allPositionList[i];
	}
	return NULL;
}
allPosisionsClass::~allPosisionsClass()
{
}
QVector<cellPositionClass*> allPosisionsClass::getAllCells()
{
	return allPositionList;
}
void allPosisionsClass::mousePressedOnPacketPosSlot(packetPositionClass *packetPos,QGraphicsSceneMouseEvent *ev)
{
	emit mousePressedOnPacketPosSignal(packetPos,ev);
}
void allPosisionsClass::setPlcClass(plcClass *plc)
{
	cranePlcTcpServer=plc->getPacketCraneTcpServer();
	plc->setAllPossitionsClass(this);
}
QVector<QPointF> allPosisionsClass::getMinusCellsPositions()
{
	QVector<QPointF> ans;
	ans.resize(60*13+1);
	for (int i=0;i<=60*13;i++)
	{
		cellPositionClass *p=getCellPosWithPosId(-i);
		if (p&&(p->getPosId()<POS_ID_CRANE)&&(p->getPosId()<0))
			ans[i]=QPointF(p->getCorrectedPlcX(),p->getCorrectedPlcZ());
		else
			ans[i]=QPointF(0.f,0.f);
	}
	return ans;
}
QVector<QPointF> allPosisionsClass::getPlusCellsPositions()
{
	QVector<QPointF> ans;
	ans.resize(60*13+1);
	for (int i=0;i<=60*13;i++)
	{
		cellPositionClass *p=getCellPosWithPosId(i);
		if (p&&(p->getPosId()<POS_ID_CRANE)&&(p->getPosId()>0))
			ans[i]=QPointF(p->getCorrectedPlcX(),p->getCorrectedPlcZ());
		else
			ans[i]=QPointF(0.f,0.f);
	}
	return ans;
}
void allPosisionsClass::setConveyorsPositions(cellPositionClass *pcr,conveyorPositionClass *c24,conveyorPositionClass *c25,conveyorPositionClass *pstationImport
	,conveyorPositionClass *stationExit1_,conveyorPositionClass *stationImport_,conveyorPositionClass *stationExit2_
	,conveyorPositionClass *stationBackExit1_,conveyorPositionClass *stationBackImport_,conveyorPositionClass *stationBackExit2_
	,conveyorPositionClass *packingExit1_,conveyorPositionClass *packingImport_,conveyorPositionClass *packingExit2_
	,conveyorPositionClass *packingBackExit1_,conveyorPositionClass *packingBackImport_,conveyorPositionClass *packingBackExit2_
	)
{
	positionCrane=pcr;
	position24=c24;
	position25=c25;
	positionStataionImport=pstationImport;

	stationExit1			=stationExit1_;
	stationImport			=stationImport_;
	stationExit2			=stationExit2_;
	stationBackExit1	=stationBackExit1_;
	stationBackImport	=stationBackImport_;
	stationBackExit2	=stationBackExit2_;

	packingExit1			=packingExit1_;
	packingImport			=packingImport_;
	packingExit2			=packingExit2_;
	packingBackExit1	=packingBackExit1_;
	packingBackImport	=packingBackImport_;
	packingBackExit2	=packingBackExit2_;
}
cellPositionClass *allPosisionsClass::getCellPosWithPosId(int id)
{
	if ((id<MAX_CELL_POS_ID)&&(id!=0))
	{
		for (int i=0;i<allPositionList.count();i++)
		{
			if (allPositionList[i]->getPosId()==id)
				return allPositionList[i];
		}
	}
	if (position24->getPosId()==id)return position24;
	if (position25->getPosId()==id)return position25;
	if (positionCrane->getPosId()==id)return positionCrane;
	if (positionStataionImport->getPosId()==id)return positionStataionImport;

	if (stationExit1->getPosId()==id)			return	stationExit1;
	if (stationImport->getPosId()==id)		return	stationImport;
	if (stationExit2->getPosId()==id)			return	stationExit2;
	if (stationBackExit1->getPosId()==id)	return	stationBackExit1;
	if (stationBackImport->getPosId()==id)return	stationBackImport;
	if (stationBackExit2->getPosId()==id)	return	stationBackExit2;

	if (packingExit1->getPosId()==id)			return	packingExit1;
	if (packingImport->getPosId()==id)		return	packingImport;
	if (packingExit2->getPosId()==id)			return	packingExit2;
	if (packingBackExit1->getPosId()==id)	return	packingBackExit1;
	if (packingBackImport->getPosId()==id)return	packingBackImport;
	if (packingBackExit2->getPosId()==id)	return	packingBackExit2;



	return NULL;
}
cellPositionClass *allPosisionsClass::getEmptyCassetteOnRow(int row,int packetType,int storagingType,QString cellSignFilter)
{
	packetPositionClass *ans=NULL;
	int posId=GETFIRST(QString("select top 1 posId from positions where posid<%1 and packetId=0 and acceptableContents=%2 and storagingType=%3 and row=%4 and isLocked=%5 and cassette<>0 %6")
																																.arg(MAX_CELL_POS_ID)										.arg(packetType)   .arg(storagingType)    .arg(row).arg(IS_NOT_LOCKED).arg(cellSignFilter)).toInt();
	return getCellPosWithPosId(posId);
}
cellPositionClass *allPosisionsClass::getEmptyCassetteOnRow(int row,int packetType)
{
	packetPositionClass *ans=NULL;
	int posId=GETFIRST(QString("select top 1 posId from positions where posid<%1 and packetId=0 and acceptableContents=%2 and row=%3 and isLocked=%4 and cassette<>0")
																																.arg(MAX_CELL_POS_ID)										.arg(packetType)  .arg(row).arg(IS_NOT_LOCKED)).toInt();
	return getCellPosWithPosId(posId);
}
cellPositionClass *allPosisionsClass::getEmptyCassetteForPacketIn(packetPositionClass *packetPosition,bool secondTry,bool loadFromDatabase)
{
	cellPositionClass *ans=NULL;
	QVector<cellPositionClass*>	emptyCellsV;
	if ( positionStataionImport&&(positionStataionImport->getPacketId(loadFromDatabase)==0)&&(positionStataionImport->getCassette()!=0))
		emptyCellsV<<positionStataionImport;
	for (int i=0;i<allPositionList.count();i++)
	{
		if (  (allPositionList[i]->getPacketId(loadFromDatabase)==0)&&(allPositionList[i]->getCassette()!=0)  )
			emptyCellsV<<allPositionList[i];
	}
	type1EmptyCassettes=0;
	for (int i=0;i<emptyCellsV.count();i++)
	{
		if (emptyCellsV[i]->getAcceptableContents()==CENTER_ONLY_PACKET_PLACING_CASSETTE)
		{
			type1EmptyCassettes++;
			if (!packetPosition)
				continue;
			if  (  (packetPosition->getNumberOfWoods()==2)   ||   (packetPosition->getNumberOfWoods()==4)  )
			{
				if ((!ans)&&(!emptyCellsV[i]->getIsLocked())&&(packetPosition->getPacketLength()<=smallLengthForEvenWoods))
					ans=emptyCellsV[i];
			}
			else
			{
				if ((!ans)&&(!emptyCellsV[i]->getIsLocked())&&(packetPosition->getPacketLength()<=smallLength))
					ans=emptyCellsV[i];
			}
		}
	}
	type2EmptyCassettes=0;
	for (int i=0;i<emptyCellsV.count();i++)
	{
		if (  (emptyCellsV[i]->getAcceptableContents()==LEFT_AND_CENTER_PACKET_PLACING_CASSETTE)  &&   (emptyCellsV[i]->getAcceptableLength()==smallLength))
		{
			type2EmptyCassettes++;
			if ((!packetPosition)||(ans)||(emptyCellsV[i]->getIsLocked()))
				continue;
			if (packetPosition->getPacketLength()<=smallLength)
				ans=emptyCellsV[i];
		}
	}
	type3EmptyCassettes=0;
	for (int i=0;i<emptyCellsV.count();i++)
	{
		if ( (emptyCellsV[i]->getAcceptableContents()==LEFT_AND_CENTER_PACKET_PLACING_CASSETTE)  &&   (emptyCellsV[i]->getAcceptableLength()==bigLength))
		{
			type3EmptyCassettes++;
			if ((!packetPosition)||(ans)||(emptyCellsV[i]->getIsLocked()))
				continue;
			if (packetPosition->getPacketLength()<=bigLength)
				ans=emptyCellsV[i];
		}
	}
	if (!packetPosition)
		return NULL;
	if (ans&&canCellTakePacketInPosG(ans->getPosId(),packetPosition->getPacketId()))
	{
		apexExecQuery(QString("insert into log (type,fromPosId,newValue,oldValue,date,sec) values('storagingType',%1,%2,%3,'%4',%5)").arg(ans->getPosId()).arg(packetPosition->getStoragingTypeOfPacket()).arg(ans->getStoragingTypeOfCassette()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		apexExecQuery(QString("update positions set storagingType=%1 where posId=%2").arg(packetPosition->getStoragingTypeOfPacket()).arg(ans->getPosId()));
//		ans=getEmptyCassetteForPacketIn(p,true);
		if (ans)
			return ans;
		else 
			EXIT(tr("error"),tr("could not find cassette for packet:%1").arg(packetPosition->getPacketId()));
	}
	else
		EXIT(tr("error"),tr("could not find cassette for packet:%1").arg(packetPosition->getPacketId()));
	return NULL;
}
cellPositionClass *allPosisionsClass::getCellPosWithEmptyCassette()
{
	int posId=getFirst(QString("select top 1 posId from positions where isLocked=0 and packetId=0 and cassette<>0 order by acceptableContents,acceptableLength"),MAIN_DB_CONNECTION).toInt();
	return getCellPosWithPosId(posId);
}
int allPosisionsClass::getType1EmptyCassettes()
{
	return type1EmptyCassettes;
}
int allPosisionsClass::getType2EmptyCassettes()
{
	return type2EmptyCassettes;
}
int allPosisionsClass::getType3EmptyCassettes()
{
	return type3EmptyCassettes;
}

void allPosisionsClass::mainTimerSlot()
{

}

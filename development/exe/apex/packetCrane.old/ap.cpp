#include "ap.h"
GLfloat	*quadVertexesForCassetteArray;
GLfloat	*quadNormalsForCassetteArray;
GLfloat	*quadTexCoordForCassetteArray;
int numberOfQuadForCassette;
GLfloat	*quadVertexesForPosSelectionArray;
GLfloat	*quadNormalsForPosSelectionArray;
GLfloat	*quadTexCoordForPosSelectionArray;
int numberOfQuadForPosSelection;
GLfloat	*quadVertexesForCenteredPacketArray;
GLfloat	*quadNormalsForCenteredPacketArray;
GLfloat	*quadTexCoordForCenteredPacketArray;
int numberOfQuadForCenteredPacket;
GLfloat	*quadVertexesForLeftPacketArray;
GLfloat	*quadNormalsForLeftPacketArray;
GLfloat	*quadTexCoordForLeftPacketArray;
int numberOfQuadForLeftPacket;
GLfloat	*quadVertexesForRightPacketArray;
GLfloat	*quadNormalsForRightPacketArray;
GLfloat	*quadTexCoordForRightPacketArray;
int numberOfQuadForRightPacket;
GLfloat	*quadVertexesFor500x500PlateArray;
GLfloat	*quadNormalsFor500x500PlateArray;
GLfloat	*quadTexCoordFor500x500PlateArray;
int numberOfQuadFor500x500Plate;

extern QSplashScreen *splash;
allPosisionsClass::allPosisionsClass(QWidget *p):QWidget(p)
{
	splash->showMessage(tr("Loading cell data..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	cranePlcTcpServer=NULL;
	//crane=NULL;
	cassetteOGL=new openGLPartClass(QString("cassette"),CASSETTE_TEXTURE);
	quadVertexesForCassetteArray=cassetteOGL->getQuadVertexesArray();
	quadNormalsForCassetteArray=cassetteOGL->getQuadNormalsArray();
	quadTexCoordForCassetteArray=cassetteOGL->getQuadTexCoordArray();
	numberOfQuadForCassette=cassetteOGL->getNumberOfQuad();
	//
	posSelectionOGL=new openGLPartClass(QString("posForPicking"),0);
	quadVertexesForPosSelectionArray=posSelectionOGL->getQuadVertexesArray();
	quadNormalsForPosSelectionArray=posSelectionOGL->getQuadNormalsArray();
	quadTexCoordForPosSelectionArray=posSelectionOGL->getQuadTexCoordArray();
	numberOfQuadForPosSelection=posSelectionOGL->getNumberOfQuad();
	//
	packetCenteredOGL=new openGLPartClass(QString("packetCentered"),0);
	quadVertexesForCenteredPacketArray=packetCenteredOGL->getQuadVertexesArray();
	quadNormalsForCenteredPacketArray=packetCenteredOGL->getQuadNormalsArray();
	quadTexCoordForCenteredPacketArray=packetCenteredOGL->getQuadTexCoordArray();
	numberOfQuadForCenteredPacket=packetCenteredOGL->getNumberOfQuad();
	//
	packetLeftOGL=new openGLPartClass(QString("packetLeft"),0);
	quadVertexesForLeftPacketArray=packetLeftOGL->getQuadVertexesArray();
	quadNormalsForLeftPacketArray=packetLeftOGL->getQuadNormalsArray();
	quadTexCoordForLeftPacketArray=packetLeftOGL->getQuadTexCoordArray();
	numberOfQuadForLeftPacket=packetLeftOGL->getNumberOfQuad();
	//
	packetRightOGL=new openGLPartClass(QString("packetRight"),0);
	quadVertexesForRightPacketArray=packetRightOGL->getQuadVertexesArray();
	quadNormalsForRightPacketArray=packetRightOGL->getQuadNormalsArray();
	quadTexCoordForRightPacketArray=packetRightOGL->getQuadTexCoordArray();
	numberOfQuadForRightPacket=packetLeftOGL->getNumberOfQuad();
	//
	for500x500PlateOGL=new openGLPartClass(QString("500x500Plate"),0);
	quadVertexesFor500x500PlateArray=for500x500PlateOGL->getQuadVertexesArray();
	quadNormalsFor500x500PlateArray=for500x500PlateOGL->getQuadNormalsArray();
	quadTexCoordFor500x500PlateArray=for500x500PlateOGL->getQuadTexCoordArray();
	numberOfQuadFor500x500Plate=for500x500PlateOGL->getNumberOfQuad();
	//initialize
	int noOfCells=1560;//1560
	QVector<QVariant> allPosIdsArray=apexExecQuery(QString("select top %1 posId from positions where posId<>0 and posid<%2").arg(noOfCells).arg(MAX_CELL_POS_ID));
	QVector<QVariant> allPacketsArray=apexExecQuery(QString("select top %1 packetId from positions where posId<>0 and posid<%2").arg(noOfCells).arg(MAX_CELL_POS_ID));
	QVector<QVariant> allCassettesArray=apexExecQuery(QString("select top %1 cassette from positions where posId<>0 and posid<%2").arg(noOfCells).arg(MAX_CELL_POS_ID));
	QVector<QVariant> allAcceptableContentsArray=apexExecQuery(QString("select top %1 acceptableContents from positions where posId<>0 and posid<%2").arg(noOfCells).arg(MAX_CELL_POS_ID));
	QVector<QVariant> storagingTypeOfCassetteArray=apexExecQuery(QString("select top %1 storagingType from positions where posId<>0 and posid<%2").arg(noOfCells).arg(MAX_CELL_POS_ID));
	QVector<QVariant> allIslockedContentsArray=apexExecQuery(QString("select top %1 isLocked from positions where posId<>0 and posid<%2").arg(noOfCells).arg(MAX_CELL_POS_ID));
	QVector<QVariant> allColumnsContentsArray=apexExecQuery(QString("select top %1 col from positions where posId<>0 and posid<%2").arg(noOfCells).arg(MAX_CELL_POS_ID));
	QVector<QVariant> allRowsContentsArray=apexExecQuery(QString("select top %1 row from positions where posId<>0 and posid<%2").arg(noOfCells).arg(MAX_CELL_POS_ID));
	allPositionList.resize(noOfCells);
	for (int i=0;i<noOfCells;i++)
	{
		splash->showMessage(tr("Loading cell data (%1%)").arg(100*i/noOfCells),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
		allPositionList[i]=new cellPositionClass(allPosIdsArray[i].toInt(),allPacketsArray[i].toInt(),allCassettesArray[i].toInt(),allAcceptableContentsArray[i].toInt(),storagingTypeOfCassetteArray[i].toInt(),allIslockedContentsArray[i].toInt(),allColumnsContentsArray[i].toInt(),allRowsContentsArray[i].toInt(),p);
		connect(allPositionList[i],SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
		connect(allPositionList[i],SIGNAL(openGlChangedSignal()),this,SLOT(openGlObjectChangedSlot()));
	}
	//
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer.start(500);
}
allPosisionsClass::~allPosisionsClass()
{
	delete cassetteOGL;
	delete posSelectionOGL;
	delete packetCenteredOGL;
	delete packetLeftOGL;
	delete packetRightOGL;
	delete for500x500PlateOGL;
}
QVector<cellPositionClass*> allPosisionsClass::getAllCells()
{
	return allPositionList;
}
void allPosisionsClass::setPlcClass(plcClass *plc)
{
	cranePlcTcpServer=plc->getPacketCraneTcpServer();
}
void allPosisionsClass::setConveyorPosition24Class(conveyorPositionClass *c24)
{
	position24=c24;
}
void allPosisionsClass::setConveyorPosition25Class(conveyorPositionClass *c25)
{
	position25=c25;
}
void allPosisionsClass::setPositionCraneClass(cellPositionClass *pcr)
{
	positionCrane=pcr;
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
cellPositionClass *allPosisionsClass::getEmptyCassetteForPacketIn(packetPositionClass *p,bool secondTry)
{
	QString cellSignFilter;
	cellPositionClass *ans=NULL;
	if (p->getPacketPlacingType()==LEFT_PLACED_PACKET)
	{
		ans=getEmptyCassetteOnRow( 5,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 6,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 7,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 8,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 9,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(10,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(11,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(12,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(13,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
	}
	else
	{
		//first look for the normal ones
		ans=getEmptyCassetteOnRow( 4,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 5,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 6,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 3,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 2,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 1,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 7,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 8,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 9,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(10,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(11,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(12,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(13,CENTER_ONLY_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		//next look for the rest
		ans=getEmptyCassetteOnRow( 4,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 5,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 6,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 3,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 2,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 1,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 7,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 8,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow( 9,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(10,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(11,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(12,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
		ans=getEmptyCassetteOnRow(13,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) return ans;
	}
	///////if not returned a position find a cassette ignoring the storagingType of the packet
	for (int i=0;i<1;i++)
	{
		if (p->getPacketPlacingType()==LEFT_PLACED_PACKET)
		{
			ans=getEmptyCassetteOnRow( 5,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) break;
			ans=getEmptyCassetteOnRow( 6,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) break;
			ans=getEmptyCassetteOnRow( 7,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) break;
			ans=getEmptyCassetteOnRow( 8,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) break;
			ans=getEmptyCassetteOnRow( 9,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) break;
			ans=getEmptyCassetteOnRow(10,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) break;
			ans=getEmptyCassetteOnRow(11,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) break;
			ans=getEmptyCassetteOnRow(12,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) break;
			ans=getEmptyCassetteOnRow(13,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE,p->getStoragingTypeOfPacket(),cellSignFilter);if (ans) break;
		}
		else
		{
			//first look for the normal ones
			ans=getEmptyCassetteOnRow( 4,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 5,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 6,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 3,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 2,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 1,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 7,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 8,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 9,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow(10,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow(11,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow(12,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow(13,CENTER_ONLY_PACKET_PLACING_CASSETTE);if (ans) break;
			//next look for the rest
			ans=getEmptyCassetteOnRow( 4,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 5,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 6,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 3,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 2,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 1,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 7,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 8,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow( 9,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow(10,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow(11,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow(12,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
			ans=getEmptyCassetteOnRow(13,LEFT_AND_CENTER_PACKET_PLACING_CASSETTE);if (ans) break;
		}
	}
	if (ans)
	{
		apexExecQuery(QString("insert into log (type,fromPosId,newValue,oldValue,date,sec) values('storagingType',%1,%2,%3,'%4',%5)").arg(ans->getPosId()).arg(p->getStoragingTypeOfPacket()).arg(ans->getStoragingTypeOfCassette()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
		apexExecQuery(QString("update positions set storagingType=%1 where posId=%2").arg(p->getStoragingTypeOfPacket()).arg(ans->getPosId()));
		ans=getEmptyCassetteForPacketIn(p,true);
		if (ans)
			return ans;
		else 
			EXIT(tr("error"),tr("could not find cassette for packet:%1").arg(p->getPacketId()));
	}
	else
		EXIT(tr("error"),tr("could not find cassette for packet:%1").arg(p->getPacketId()));
	return NULL;
}
void allPosisionsClass::onCellDataChangedSlot(int cell)
{
	for (int i=0;i<allPositionList.count();i++)
	{
		if (allPositionList[i]->getPosId()==cell)
		{
			allPositionList[i]->loadFromDatabase();
			emit openGlChangedSignal();
			break;
		}
	}
	position24->loadFromDatabase();
	position25->loadFromDatabase();
	positionCrane->loadFromDatabase();
}
#include "uglyfont.h"
void allPosisionsClass::draw(bool forPicking)
{
	if (forPicking)
	{
		for (int i=0;i<allPositionList.count();i++)
			allPositionList[i]->drawForPicking();
	}
	else
	{
		for (int i=0;i<allPositionList.count();i++)
			allPositionList[i]->draw();
	}
}
void allPosisionsClass::mouseOver(QColor pickedColor)
{
	for (int i=0;i<allPositionList.count();i++)
		allPositionList[i]->mouseOver(pickedColor);
}
void allPosisionsClass::mouseClicked(QColor pickedColor)
{
	for (int i=0;i<allPositionList.count();i++)
		allPositionList[i]->mouseClicked(pickedColor);
}
void allPosisionsClass::openGlObjectChangedSlot()
{
	emit openGlChangedSignal();
}
void allPosisionsClass::mainTimerSlot()
{

}

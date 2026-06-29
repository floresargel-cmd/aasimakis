#include "pch.h"
#include "man.h"	
managerClass::managerClass(QGraphicsScene *scene,allPosisionsClass *allPosisions_,craneClass *crane_,cellPositionClass *positionCrane_,conveyorPositionClass *position24_
	,conveyorPositionClass *position25_,forkClass *fork_,forkPositionClass *positionForkLift_
#ifdef keepPacketCraneOldFunctionality
	,newPacketPositionClass *positionSb_
#endif
	,conveyorPositionClass *stationExit1_,conveyorPositionClass *stationImport_,conveyorPositionClass *stationExit2_
	,conveyorPositionClass *packingExit1_,conveyorPositionClass *packingImport_,conveyorPositionClass *packingExit2_,conveyorPositionClass *packingBackExit1_
#ifdef keepPacketCraneOldFunctionality
	,newPacketPositionClass *positionPa_
#endif
	,newPacketPositionClass *positionIm_,forkPackingWdgtClass *forkPackingWdgt_,QWidget *p):QWidget(p)
{
	QWidget::hide();
	plc=NULL;
#ifdef keepPacketCraneOldFunctionality
	exportsDestination=exportsDestinationEnum::exportsTo23;
#endif
	allPosisions=allPosisions_;
	crane=crane_;
	positionCrane=positionCrane_;
	position24=position24_;
	position25=position25_;
	fork=fork_;
	positionForkLift=positionForkLift_;
#ifdef keepPacketCraneOldFunctionality
	positionSb=positionSb_;
	positionPa=positionPa_;
#endif
	positionIm=positionIm_;

	stationExit1=stationExit1_;
	stationImport=stationImport_;
	stationExit2=stationExit2_;

	packingExit1=packingExit1_;
	packingImport=packingImport_;
	packingExit2=packingExit2_;
	packingBackExit1=packingBackExit1_;

	forkPackingWdgt=forkPackingWdgt_;

	dxfClass *mouseOverItem=new dxfClass(":dxfs/posMouseOver.dxf",QPen(QColor(175,225,175)),true,false,QColor(100,200,100,150),this);
	dxfClass *lockedItem=new dxfClass(":dxfs/posLocked.dxf",QPen(QColor(200,50,50)),false,false,QColor(),this);
	dxfClass *sbItem=new dxfClass(":dxfs/sb.dxf",QPen(QColor(50,50,150)),false,false,QColor(),this);

#ifdef keepPacketCraneOldFunctionality
	positionSbExit=new packetPositionClass(POS_ID_SBEXIT,tr("SB exit"),0,0,0,0,0,scene,mouseOverItem,lockedItem,sbItem,this);positionSbExit->setGXY(-1500.,-43972.);
	positionPacketExit=new packetPositionClass(POS_ID_PACKETEXIT,tr("Packet exit"),0,0,0,0,0,scene,mouseOverItem,lockedItem,sbItem,this);positionPacketExit->setGXY(-1500.,-33924.);
	positionStoreExit=new packetPositionClass(POS_ID_STOREEXIT,tr("Warehouse exit"),0,0,0,0,0,scene,mouseOverItem,lockedItem,sbItem,this);positionStoreExit->setGXY(-570.,-16300.);
	positionSbExit->loadFromDatabase();
	positionPacketExit->loadFromDatabase();
	positionStoreExit->loadFromDatabase();
	connect(positionSbExit,SIGNAL(packetDataChangedSignal(int)),this,SLOT(onPacketDataChangedSlot(int)));
	connect(positionPacketExit,SIGNAL(packetDataChangedSignal(int)),this,SLOT(onPacketDataChangedSlot(int)));
	connect(positionStoreExit,SIGNAL(packetDataChangedSignal(int)),this,SLOT(onPacketDataChangedSlot(int)));
#endif

	positionInputOutput=new packetPositionClass(positionInputOutputPosId,tr("Input-Output"),0,0,0,0,0,scene,mouseOverItem,lockedItem,sbItem,this);positionInputOutput->setGXY(20000,-25000.);
	positionOldStrappingExit=new packetPositionClass(packingOldStrappingExitPosId,tr("Old Strapping Exit"),0,0,0,0,0,scene,mouseOverItem,lockedItem,sbItem,this);positionOldStrappingExit->setGXY(30000,-21000.);
	positionNewStrappingExit=new packetPositionClass(packingNewStrappingExitPosId,tr("New Strapping Exit"),0,0,0,0,0,scene,mouseOverItem,lockedItem,sbItem,this);positionNewStrappingExit->setGXY(33000,-23000.);
	positionInputOutput->loadFromDatabase();
	positionOldStrappingExit->loadFromDatabase();
	positionNewStrappingExit->loadFromDatabase();
	connect(positionInputOutput,SIGNAL(packetDataChangedSignal(int)),this,SLOT(onPacketDataChangedSlot(int)));
	connect(positionOldStrappingExit,SIGNAL(packetDataChangedSignal(int)),this,SLOT(onPacketDataChangedSlot(int)));
	connect(positionNewStrappingExit,SIGNAL(packetDataChangedSignal(int)),this,SLOT(onPacketDataChangedSlot(int)));
	
//
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	connect(&updateFromDbTimer,SIGNAL(timeout()),this,SLOT(updateFromDbTimerSlot()));
}
managerClass::~managerClass()
{
}
void managerClass::onPacketDataChangedSlot(int cell)
{
#ifdef keepPacketCraneOldFunctionality
	positionSbExit->loadFromDatabase();
	positionPacketExit->loadFromDatabase();
	positionStoreExit->loadFromDatabase();
#endif
	positionInputOutput->loadFromDatabase();
	positionOldStrappingExit->loadFromDatabase();
	positionNewStrappingExit->loadFromDatabase();
}
#ifdef keepPacketCraneOldFunctionality
void managerClass::setExportsDestination(exportsDestinationEnum d)
{
	exportsDestination=d;
}
#endif
void managerClass::setPlcClass(plcClass *plc_)
{
	plc=plc_;
	forkM=plc->getForkLiftManualClass();
	crane->setPlcData(plc->getTuxServer()->getTuxipClassWithIp(packetCraneIp));
}
#ifdef keepPacketCraneOldFunctionality
packetPositionClass *managerClass::getPositionStoreExit()
{
	return positionStoreExit;
}
packetPositionClass *managerClass::getPositionSbExit()
{
	return positionSbExit;
}
packetPositionClass *managerClass::getPositionPacketExit()
{
	return positionPacketExit;
}
#endif
void managerClass::updateFromDbTimerSlot()
{
	if (plc->getTuxServer()->getHasError())
		return;
#ifdef keepPacketCraneOldFunctionality
	positionPa->loadFromDatabase();
	positionSb->loadFromDatabase();
#endif
	positionIm->loadFromDatabase();
	positionInputOutput->loadFromDatabase();
	positionOldStrappingExit->loadFromDatabase();
	positionNewStrappingExit->loadFromDatabase();
	mainTimerSlot();
}
bool managerClass::getAreAutoMissionsEnabled()
{
	return autoMissionsExports||autoMissionsImports||autoMissionsExportsStation1||autoMissionsExportsStation2||autoMissionsStationImports||autoMissionsPacking;
}
void managerClass::enableAutoExportsMissions(bool enable)
{
	autoMissionsExports=enable;
	mainTimerSlot();
}
void managerClass::enableAutoImportsMissions(bool enable)
{
	autoMissionsImports=enable;
	mainTimerSlot();
}
void managerClass::enableAutoExportsStation1Missions(bool enable)
{
	autoMissionsExportsStation1=enable;
	mainTimerSlot();
}
void managerClass::enableAutoExportsStation2Missions(bool enable)
{
	autoMissionsExportsStation2=enable;
	mainTimerSlot();
}
void managerClass::enableAutoImportsStationMissions(bool enable)
{
	autoMissionsStationImports=enable;
	mainTimerSlot();
}
void managerClass::enableAutoPackingMissions(bool enable)
{
	autoMissionsPacking=enable;
	mainTimerSlot();
}
void managerClass::mainTimerSlot()
{
	if ((!plc)||(!plc->isConectionsEstablished()))
		return;
	if (!getAreAutoMissionsEnabled())
		return;
////////////////////////////////packing station/////////////////////////////////////////
	bool craneMissionSend=false;
	if ((!craneMissionSend)&&autoMissionsPacking)//packing station
	{
		if ((forkPackingWdgt->getIsForkIdle()||forkPackingWdgt->getIsAboutToBeIdle())&&packingBackExit1->getHasPacketReadyForWarehouse())
			forkPackingWdgt->returnFullCassetteFromNorthConveyor();
		if ((!craneMissionSend)&&crane->isIdle())//exports
		{
			if (packingExit2->getCassette()==0)
			{
				int packet=getFirst(QString("select top 1 contents from exportsPacking where status='0' order by orderSequence,contentsSequence"),MAIN_DB_CONNECTION).toInt();
				if (packet!=0)
				{
					int fromPosId=getFirst(QString("select posId from positions where packetId=%1 and posId<%2 and isLocked=0").arg(packet).arg(MAX_CELL_POS_ID),MAIN_DB_CONNECTION).toInt();
					apexExecQuery(QString("update exportsPacking set status='2' where contents='%1'").arg(packet));
					if (fromPosId==0)//clear from exports
						apexExecQuery(QString("update exportsPacking set status='2' where contents='%1'").arg(packet));
					craneMissionSend=crane->newMission(allPosisions->getCellPosWithPosId(fromPosId),packingExit2,false,false);
				}
			}
		}
		////////if (forkPackingWdgt->getIsForkIdle())//fork mission
		////////{
		////////	if (packingBackExit1->hasCassette()&&(packingBackExit1->getPacketId(true)==0))
		////////	{
		////////		bool forkMissionSend=false;
		////////		if ((!forkMissionSend)&&positionOldStrappingExit->getHasPacketReadyForWarehouse()&&(canCellTakePacketInPosG(packingBackExit1->getCassette(),positionOldStrappingExit->getPacketId(),pDb)))
		////////			forkMissionSend=forkPackingWdgt->sendMissionToFork(strappingOldOutPlcId,warehouseConveyorPlcId);
		////////		if ((!forkMissionSend)&&positionNewStrappingExit->getHasPacketReadyForWarehouse()&&(canCellTakePacketInPosG(packingBackExit1->getCassette(),positionNewStrappingExit->getPacketId(),pDb)))
		////////			forkPackingWdgt->sendMissionToFork(strappingNewOutPlcId,warehouseConveyorPlcId);
		////////		if ((!forkMissionSend)&&positionInputOutput->getHasPacketReadyForWarehouse()&&(canCellTakePacketInPosG(packingBackExit1->getCassette(),positionInputOutput->getPacketId(),pDb)))
		////////			forkPackingWdgt->sendMissionToFork(inputOutputPlcId,warehouseConveyorPlcId);
		////////		if (!forkMissionSend)
		////////		{
		////////			if( 
		////////				(positionOldStrappingExit->getHasPacketReadyForWarehouse()&&(!canCellTakePacketInPosG(packingBackExit1->getCassette(),positionOldStrappingExit->getPacketId(),pDb)))
		////////				||
		////////				(positionNewStrappingExit->getHasPacketReadyForWarehouse()&&(!canCellTakePacketInPosG(packingBackExit1->getCassette(),positionNewStrappingExit->getPacketId(),pDb)))
		////////				||
		////////				(positionInputOutput->getHasPacketReadyForWarehouse()&&(!canCellTakePacketInPosG(packingBackExit1->getCassette(),positionInputOutput->getPacketId(),pDb)))
		////////				)
		////////				forkPackingWdgt->returnEmptyCassetteFromNorthConveyor();
		////////		}
		////////	}
		////////}
		if ((!craneMissionSend)&&crane->isIdle())//crane mission
		{
			if (!packingExit1->hasCassette())//Bring cassette for new packet
			{
				QVector<packetPositionClass*> positionsWithReadyPackets;
				if (positionOldStrappingExit->getHasPacketReadyForWarehouse(true))positionsWithReadyPackets<<positionOldStrappingExit;
				if (positionNewStrappingExit->getHasPacketReadyForWarehouse(true))positionsWithReadyPackets<<positionNewStrappingExit;
				if (positionInputOutput->getHasPacketReadyForWarehouse(true))positionsWithReadyPackets<<positionInputOutput;
				if (packingBackExit1->hasCassette())
				{
					for (int i=0;i<positionsWithReadyPackets.count();i++)
					{
						if (canCellTakePacketInPosG(packingBackExit1->getCassette(),positionsWithReadyPackets[i]->getPacketId()))
						{
							positionsWithReadyPackets.removeAt(i);
							break;
						}
					}
				}
				for (int i=0;(!craneMissionSend)&&(i<positionsWithReadyPackets.count());i++)
				{
					craneMissionSend=crane->newMission(allPosisions->getEmptyCassetteForPacketIn(positionsWithReadyPackets[i]),packingExit1,false,false);
				}
			}
		}
		if ((!craneMissionSend)&&crane->isIdle())//import
		{
			if (packingImport->hasCassette())
				craneMissionSend=crane->newMission(packingImport->getPosId(),packingImport->getCassette(),false,false);
		}
	}
////////////////////////////////packing station/////////////////////////////////////////
	if (fork->isIdle())
	{
		packetPositionClass *fromPos=NULL;
		packetPositionClass *toPos=NULL;
		if (autoMissionsImports&&   (
#ifdef keepPacketCraneOldFunctionality
			positionPa->hasPacket()||positionSb->hasPacket()||
#endif
			positionIm->hasPacket())    )
		{
			vLog("info","managerClass::mainTimerSlot():","new packet");
#ifdef keepPacketCraneOldFunctionality
			if (positionPa->hasPacket())vLog("info","managerClass::mainTimerSlot():",QString("new packet in pos:%1, packet:%2").arg(positionPa->getPosId()).arg(positionPa->getPacketId()));
			if (positionSb->hasPacket())vLog("info","managerClass::mainTimerSlot():",QString("new packet in pos:%1, packet:%2").arg(positionSb->getPosId()).arg(positionSb->getPacketId()));
#endif
			if (positionIm->hasPacket())vLog("info","managerClass::mainTimerSlot():",QString("new packet in pos:%1, packet:%2").arg(positionIm->getPosId()).arg(positionIm->getPacketId()));
			//new packets
			if (positionIm->hasPacket())fromPos=positionIm;
#ifdef keepPacketCraneOldFunctionality
			if (positionPa->hasPacket())fromPos=positionPa;
			if (positionSb->hasPacket())fromPos=positionSb;
#endif
			vLog("info","managerClass::mainTimerSlot():",QString("from pos:%1").arg(fromPos->getPosId()));
			if (fromPos)
			{
				if (fromPos->getPacketDestination()==PACKET_DESTINATION_OUT)
				{
#ifdef keepPacketCraneOldFunctionality
					vLog("info","managerClass::mainTimerSlot():","fromPos->getPacketDestination()==PACKET_DESTINATION_OUT");
					positionPacketExit->setPacketId(0);
					positionSbExit->setPacketId(0);
					if (positionPa->hasPacket())
						toPos=positionPacketExit;
					if (positionSb->hasPacket())
						toPos=positionSbExit;
#endif
				}
				else// if (!isCraneOff)
				{
					vLog("info","managerClass::mainTimerSlot():","fromPos->getPacketDestination()!=PACKET_DESTINATION_OUT");
					if (position24->canTakePacketThatIsOnPos(fromPos))
					{
						vLog("info","managerClass::mainTimerSlot():","position24->canTakePacketThatIsOnPos(fromPos)");
						toPos=position24;
					}
					else if (position25->canTakePacketThatIsOnPos(fromPos))
					{
						vLog("info","managerClass::mainTimerSlot():","position25->canTakePacketThatIsOnPos(fromPos)");
						toPos=position25;
					}
					else 
						crane->takeCassetteOutForPacketIn(fromPos);
				}
			}
		}
		//exports
		if ((position24->isActiveForPacketExport()||position25->isActiveForPacketExport())&&((!fromPos)||(!toPos)))
		{
			//vLog("info","managerClass::mainTimerSlot():","((position24->isActiveForPacketExport()||position25->isActiveForPacketExport())&&((!fromPos)||(!toPos)))");
			if (position24->isActiveForPacketExport()&&position25->isActiveForPacketExport())
			{
				vLog("info","managerClass::mainTimerSlot():","position24->isActiveForPacketExport()&&position25->isActiveForPacketExport()");
				if (position24->hasCassette()&&position25->hasCassette())
				{
					vLog("info","managerClass::mainTimerSlot():","(position24->hasCassette()&&position25->hasCassette())");
					int packetOn24ContentsSequence=GETFIRST(QString("select contentsSequence from exports where contents=%1").arg(position24->getPacketId())).toInt();
					int packetOn25ContentsSequence=GETFIRST(QString("select contentsSequence from exports where contents=%1").arg(position25->getPacketId())).toInt();
					if (packetOn24ContentsSequence<packetOn25ContentsSequence)
						fromPos=(packetPositionClass*)position24;
					else
						fromPos=(packetPositionClass*)position25;
				}
				else if (position24->hasCassette())
				{
					vLog("info","managerClass::mainTimerSlot():","(position24->hasCassette())");
					fromPos=(packetPositionClass*)position24;
				}
				else if (position25->hasCassette())
				{
					vLog("info","managerClass::mainTimerSlot():","(position25->hasCassette())");
					fromPos=(packetPositionClass*)position25;
				}
			}
			else if (position24->isActiveForPacketExport())
			{
				vLog("info","managerClass::mainTimerSlot():","(position24->isActiveForPacketExport())");
				fromPos=(packetPositionClass*)position24;
			}
			else if (position25->isActiveForPacketExport())
			{
				vLog("info","managerClass::mainTimerSlot():","(position25->isActiveForPacketExport())");
				fromPos=(packetPositionClass*)position25;
			}
			if (fromPos)
			{
				vLog("info","managerClass::mainTimerSlot():exports",QString("fromPos:%1").arg(fromPos->getPosId()));
				int packetId=0;
				if (fromPos->hasPacket())
				{
					vLog("info","managerClass::mainTimerSlot():","(fromPos->hasPacket())");
					packetId=fromPos->getPacketId();
				}
				else
				{
					vLog("info","managerClass::mainTimerSlot():","! (fromPos->hasPacket())");
					int craneToPos=GETONE("select value from state where name='craneToPos'").toInt();
					if (craneToPos==fromPos->getPosId())
					{
						int craneFromPosId=GETONE("select value from state where name='craneFromPos             '").toInt();
						packetId=GETONEATMOST(QString("select packetId from positions where cassette=%1").arg(craneFromPosId)).toInt();
					}
				}
				if (packetId>0)
				{
#ifdef keepPacketCraneOldFunctionality
					int length=GETONE(QString("select length from packets where packetId=%1").arg(packetId)).toInt();
					if (exportsDestination==exportsTo23)
					{
						if (length>2743)
							toPos=positionStoreExit;
						else if (length>=1828)
							toPos=positionPacketExit;
					}
					else if (exportsDestination==exportsTo22)
					{
						toPos=positionPacketExit;
					}
					else if (exportsDestination==exportsTo21)
					{
							if (length>2743)
							toPos=positionSbExit;
						else if (length>=1828)
							toPos=positionPacketExit;
					}
#endif
				}
			}
		}
		if (fromPos&&toPos)
		{
#ifdef keepPacketCraneOldFunctionality
			if ( 
				(toPos->getPosId()==POS_ID_SBEXIT)||(toPos->getPosId()==POS_ID_PACKETEXIT)||(toPos->getPosId()==POS_ID_STOREEXIT) )
				toPos->setPacketId(0);
#endif
			fork->newMission(fromPos,toPos,false);
		}
	}
	if ((!craneMissionSend)&&autoMissionsImports&&crane->isIdle())
	{//imports
		//vLog("info","managerClass::mainTimerSlot():","crane->isIdle()");
		cellPositionClass *fromPos=NULL;
		cellPositionClass *toPos=NULL;
		if ( (position24->hasCassette())&&(position24->isActiveForCassetteImport()) )
			fromPos=position24;
		if ( (position25->hasCassette())&&(position25->isActiveForCassetteImport()) )
			fromPos=position25;
		if (fromPos)
			craneMissionSend=crane->newMission(fromPos->getPosId(),fromPos->getCassette(),false,false);
	}
	if (autoMissionsExports&&crane->isIdle())
	{
#ifdef keepPacketCraneOldFunctionality
		if (autoMissionsExports  &&    ( (!autoMissionsImports)  || ( (
#ifdef keepPacketCraneOldFunctionality
			(!positionPa->hasPacket())&&(!positionSb->hasPacket())&&
#endif
			(!positionIm->hasPacket()))||(fork->isGoingToWarehouse())) )  )//exports: only if no new packet
		{
			if (
				( (exportsDestination==exportsTo23)   &&   forkM->get23CanTakeBasket() /*&& (!fork->isPosActiveTo(POS_ID_STOREEXIT))*/ )//billy jack
				||
				( (exportsDestination==exportsTo22)   &&   forkM->get22CanTakeBasket()/* && (!fork->isPosActiveTo(POS_ID_PACKETEXIT)) */)//billy jack
				||
				( (exportsDestination==exportsTo21)   &&   forkM->get21CanTakeBasket()/* && (!fork->isPosActiveTo(POS_ID_PACKETEXIT)) */)//billy jack
				)
			{
				//vLog("info","managerClass::mainTimerSlot():","((!positionPa->hasPacket())&&(!positionSb->hasPacket())&&(!positionIm->hasPacket()))");
				cellPositionClass *fromPos=NULL;
				cellPositionClass *toPos=NULL;
				if ( (!position24->hasCassette())&&(!position24->isActive()) )
					toPos=position24;
				else if ( (!position25->hasCassette())&&(!position25->isActive()) )
					toPos=position25;

				//int sb=GETONEATMOST(QString("select top 1 type from exports where status='0         ' order by orderSequence,contentsSequence"))=="sb"?2:1;
				int packetNumber=GETONEATMOST(QString("select top 1 contents from exports where status='0' order by orderSequence,contentsSequence")).toInt();
				int posIdToComeOut=0;
				if (packetNumber!=0)
					posIdToComeOut=GETONEATMOST(QString("select posId from positions where packetId=%1 and posId<%2 and isLocked=0").arg(packetNumber).arg(MAX_CELL_POS_ID)).toInt();
				if (posIdToComeOut==0)//clear from exports
					apexExecQuery(QString("update exports set status='2' where contents='%1'").arg(packetNumber));
				if ((posIdToComeOut!=0)&&(posIdToComeOut<MAX_CELL_POS_ID))
					fromPos=allPosisions->getCellPosWithPosId(posIdToComeOut);

				if (fromPos&&(!toPos))
					crane->returnNoNeededCassette();
				if (fromPos&&toPos)
					crane->newMission(fromPos,toPos,false,false);
			}
		}
#endif
	}
	if ((!craneMissionSend)&&crane->isIdle())//station exports
	{
		if (autoMissionsExportsStation1&&crane->isIdle())
		{
			if (stationExit1->getCassette()==0)
			{
				int packet=getFirst(QString("select top 1 contents from exportsStation1 where status='0' order by orderSequence,contentsSequence"),MAIN_DB_CONNECTION).toInt();
				if (packet!=0)
				{
					int fromPosId=getFirst(QString("select posId from positions where packetId=%1 and posId<%2 and isLocked=0").arg(packet).arg(MAX_CELL_POS_ID),MAIN_DB_CONNECTION).toInt();
					apexExecQuery(QString("update exportsStation1 set status='2' where contents='%1'").arg(packet));
					if (fromPosId==0)//clear from exports
						apexExecQuery(QString("update exportsStation1 set status='2' where contents='%1'").arg(packet));
					craneMissionSend=crane->newMission(allPosisions->getCellPosWithPosId(fromPosId),stationExit1,false,false);
				}
			}
		}
		if ((!craneMissionSend)&&autoMissionsExportsStation2&&crane->isIdle())
		{
			if (stationExit2->getCassette()==0)
			{
				int packet=getFirst(QString("select top 1 contents from exportsStation2 where status='0' order by orderSequence,contentsSequence"),MAIN_DB_CONNECTION).toInt();
				if (packet!=0)
				{
					int fromPosId=getFirst(QString("select posId from positions where packetId=%1 and posId<%2 and isLocked=0").arg(packet).arg(MAX_CELL_POS_ID),MAIN_DB_CONNECTION).toInt();
					apexExecQuery(QString("update exportsStation2 set status='2' where contents='%1'").arg(packet));
					if (fromPosId==0)//clear from exports
						apexExecQuery(QString("update exportsStation2 set status='2' where contents='%1'").arg(packet));
					craneMissionSend=crane->newMission(allPosisions->getCellPosWithPosId(fromPosId),stationExit2,false,false);
				}
			}
		}
		if ((!craneMissionSend)&&autoMissionsStationImports&&crane->isIdle())
		{
			if (stationImport->hasCassette())
				craneMissionSend=crane->newMission(stationImport->getPosId(),stationImport->getCassette(),false,false);
		}
	}
}
void managerClass::startTimers()
{
	mainTimer.start(500);
	updateFromDbTimer.start(1000);

}

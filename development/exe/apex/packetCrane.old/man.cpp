#include "man.h"
managerClass::managerClass(allPosisionsClass *allPosisions_,craneClass *crane_,cellPositionClass *positionCrane_,conveyorPositionClass *position24_,conveyorPositionClass *position25_,forkClass *fork_,forkPositionClass *positionForkLift_,newPacketPositionClass *positionSb_,newPacketPositionClass *positionPa_,newPacketPositionClass *positionIm_,QWidget *p):QWidget(p)
{
	plc=NULL;
	exportsDestination=exportsDestinationEnum::exportsTo23;
	allPosisions=allPosisions_;
	crane=crane_;
	positionCrane=positionCrane_;
	position24=position24_;
	position25=position25_;
	fork=fork_;
	positionForkLift=positionForkLift_;
	positionSb=positionSb_;
	positionPa=positionPa_;
	positionIm=positionIm_;
	autoMissionsEnabled=false;
	positionSbExit=new packetPositionClass(POS_ID_SBEXIT,0.0f,0.0f,0.0f,this);
	positionPacketExit=new packetPositionClass(POS_ID_PACKETEXIT,0.0f,0.0f,0.0f,this);
	positionStoreExit=new packetPositionClass(POS_ID_STOREEXIT,0.0f,0.0f,0.0f,this);
//
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	connect(&updateFromDbTimer,SIGNAL(timeout()),this,SLOT(updateFromDbTimerSlot()));
}
managerClass::~managerClass()
{
}
void managerClass::setExportsDestination(exportsDestinationEnum d)
{
	exportsDestination=d;
}
void managerClass::setPlcClass(plcClass *plc_)
{
	plc=plc_;
	forkM=plc->getForkLiftManualClass();
}
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
void managerClass::updateFromDbTimerSlot()
{
#ifdef TUX_PLC_COMMS
	if (plc->getTuxServer()->getHasError())
		return;
#endif
	positionPa->loadFromDatabase();
	positionSb->loadFromDatabase();
	positionIm->loadFromDatabase();
	mainTimerSlot();
}
void managerClass::enableAutoMissions(bool enable)
{
	autoMissionsEnabled=enable;
	mainTimerSlot();
}
void managerClass::mainTimerSlot()
{
	if ((!plc)||(!plc->isConectionsEstablished()))
		return;
	if (!autoMissionsEnabled)
		return;
	if (fork->isIdle())
	{
		packetPositionClass *fromPos=NULL;
		packetPositionClass *toPos=NULL;
		if (positionPa->hasPacket()||positionSb->hasPacket()||positionIm->hasPacket())
		{
			vLog("info","managerClass::mainTimerSlot():","new packet");
			if (positionPa->hasPacket())vLog("info","managerClass::mainTimerSlot():",QString("new packet in pos:%1, packet:%2").arg(positionPa->getPosId()).arg(positionPa->getPacketId()));
			if (positionSb->hasPacket())vLog("info","managerClass::mainTimerSlot():",QString("new packet in pos:%1, packet:%2").arg(positionSb->getPosId()).arg(positionSb->getPacketId()));
			if (positionIm->hasPacket())vLog("info","managerClass::mainTimerSlot():",QString("new packet in pos:%1, packet:%2").arg(positionIm->getPosId()).arg(positionIm->getPacketId()));
			//new packets
			if (positionPa->hasPacket())fromPos=positionPa;
			if (positionSb->hasPacket())fromPos=positionSb;
			if (positionIm->hasPacket())fromPos=positionIm;
			vLog("info","managerClass::mainTimerSlot():",QString("from pos:%1").arg(fromPos->getPosId()));
			if (fromPos)
			{
				if (fromPos->getPacketDestination()==PACKET_DESTINATION_OUT)
				{
					vLog("info","managerClass::mainTimerSlot():","fromPos->getPacketDestination()==PACKET_DESTINATION_OUT");
					positionPacketExit->setPacketId(0);
					positionSbExit->setPacketId(0);
					if (positionPa->hasPacket())
						toPos=positionPacketExit;
					if (positionSb->hasPacket())
						toPos=positionSbExit;
				}
				else
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
				}
			}
		}
		if (fromPos&&toPos)
		{
			if ( (toPos->getPosId()==POS_ID_SBEXIT)||(toPos->getPosId()==POS_ID_PACKETEXIT)||(toPos->getPosId()==POS_ID_STOREEXIT) )
				toPos->setPacketId(0);
			fork->newMission(fromPos,toPos,false);
		}
	}
	if (crane->isIdle())
	{
		{//imports
			//vLog("info","managerClass::mainTimerSlot():","crane->isIdle()");
			cellPositionClass *fromPos=NULL;
			cellPositionClass *toPos=NULL;
			if ( (position24->hasCassette())&&(position24->isActiveForCassetteImport()) )
				fromPos=position24;
			if ( (position25->hasCassette())&&(position25->isActiveForCassetteImport()) )
				fromPos=position25;
			if (fromPos)
				crane->newMission(fromPos->getPosId(),fromPos->getCassette(),false,false);
		}
		if ((!positionPa->hasPacket())&&(!positionSb->hasPacket())&&(!positionIm->hasPacket()))//exports: only if no new packet
		{
			if (
				( (exportsDestination==exportsTo23)   &&   forkM->get23CanTakeBasket() && (!fork->isPosActiveTo(POS_ID_STOREEXIT)) )
				||
				( (exportsDestination==exportsTo22)   &&   forkM->get22CanTakeBasket() && (!fork->isPosActiveTo(POS_ID_PACKETEXIT)) )
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
				int packetNumber=GETONEATMOST(QString("select top 1 contents from exports where status='0         ' order by orderSequence,contentsSequence")).toInt();
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
	}
}
void managerClass::startTimers()
{
	////fork
	//{
	//	int fromPosId=GETONE("select value from state where name='forkFromPos'").toInt();
	//	int toPosId=GETONE("select value from state where name='forkToPos'").toInt();
	//	packetPositionClass *fromPos=NULL;
	//	packetPositionClass *toPos=NULL;
	//	if (fromPosId==POS_ID_NEWSB)fromPos=positionSb;
	//	else if (fromPosId==POS_ID_NEWPACKET)fromPos=positionPa;
	//	else if (fromPosId==POS_ID_SB_PACKET_IMPORT)fromPos=positionIm;
	//	else if (fromPosId==POS_ID_WESTCONV)fromPos=position24;
	//	else if (fromPosId==POS_ID_EASTCONV)fromPos=position25;
	//	if (fromPos)
	//	{
	//		if (toPosId==POS_ID_SBEXIT)toPos=positionSbExit;
	//		else if (toPosId==POS_ID_PACKETEXIT)toPos=positionPacketExit;
	//		else if (toPosId==POS_ID_WESTCONV)toPos=position24;
	//		else if (toPosId==POS_ID_EASTCONV)toPos=position25;
	//		else if (toPosId==POS_ID_STOREEXIT)toPos=positionStoreExit;
	//	}
	//	if ((fromPos!=0)&&(toPos!=0))
	//		fork->newMission(fromPos,toPos,true);
	//}
	////crane
	//{
	//	int fromPosId=GETONE("select value from state where name='craneFromPos             '").toInt();
	//	int toPosId=GETONE("select value from state where name='craneToPos               '").toInt();
	//	if ((fromPosId!=0)&&(toPosId!=0))
	//		crane->newMission(fromPosId,toPosId,true,false);
	//}

	mainTimer.start(500);
	updateFromDbTimer.start(1000);

}

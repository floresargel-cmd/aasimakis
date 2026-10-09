#include "cr.h"
//extern float plcSetXVelocityForOpenGl;
//extern float plcSetYVelocityForOpenGl;
//extern float plcSetZVelocityForOpenGl;
#define activeMissionTimerTime 1500
craneClass::craneClass():QObject()
{
	plcXInMeters=plcYInMeters=plcZInMeters=0.0;
	lastPlcXInMeters=lastPlcYInMeters=lastPlcZInMeters=0.0;
	openGlX=openGlY=openGlZ=0.0f;
	plcXVelocity=plcYVelocity=plcZVelocity=0.0f;
	lastToBasketPos=NULL;
	//missionFinishedWithOverWeight=false;
	plcStatus=myFromPosInPlc=myToPosInPlc=myLengthTypeInPlc=myStatusInPlc=my500BasketInPlc=my510BasketInPlc=my520BasketInPlc=-1;
	activeFromBasketPos=activeToBasketPos=NULL;
	myBasketPos=new basketPositionClass(NULL,"crane",1,1,basketPositionClass::EPICS_POS_BASKET_NOWHERE,0.0f,0.0f,0.0f);
	isCarryingBasket=false;
	hasActiveMission=false;
	horizontalOpenGl=new openGLPartClass(QString(":/foreas.stl"),WHITE_TEXTURE_QUAD,WHITE_TEXTURE_QUAD);
	transverseOpenGl=new openGLPartClass(QString(":/forio.stl"),WHITE_TEXTURE_QUAD,WHITE_TEXTURE_QUAD);
	verticalOpenGl=new openGLPartClass(QString(":/tsimpidia.stl"),WHITE_TEXTURE_QUAD,WHITE_TEXTURE_QUAD);
	connect(&animateTimer, SIGNAL(timeout()), this, SLOT(animateTimerSlot()));
	connect(&activeMissionTimer, SIGNAL(timeout()), this, SLOT(activeMissionTimerSlot()));
	//
	plcXTime.start();
	plcYTime.start();
	plcZTime.start();
	plcModeEnum=NO_MODE;
}
craneClass::~craneClass()
{
	delete myBasketPos;
	delete horizontalOpenGl;
	delete transverseOpenGl;
	delete verticalOpenGl;
}
void craneClass::openGlDraw()
{
	if (myBasketPos->hasBasket())
	{
		glPushMatrix();
		glTranslatef(openGlX-3.5f,openGlY+0.60f,2.75+openGlZ);
		myBasketPos->drawBasket(true);
		glPopMatrix();
	}
	glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
	glColor3ub(255,200,200);
	horizontalOpenGl->draw();
	transverseOpenGl->draw();
	verticalOpenGl->draw();
	glPopAttrib();
	if (activeFromBasketPos)
		activeFromBasketPos->draw(false,true,false);
	if (activeToBasketPos)
		activeToBasketPos->draw(false,false,true);
}
void craneClass::openGlDrawForPicking()
{
	horizontalOpenGl->drawForPicking();
	transverseOpenGl->drawForPicking();
	verticalOpenGl->drawForPicking();
}
//
float avarage(QVector<float> v)
{
	float ans=0;
	for(int i=0;i<v.count();i++)
		ans+=v[i];
	return ans/v.count();
}

bool craneClass::mouseOver(QColor pickedColor)
{
	if (horizontalOpenGl->mouseOver(pickedColor))
		 return true;
	if (transverseOpenGl->mouseOver(pickedColor))
		 return true;
	if (verticalOpenGl->mouseOver(pickedColor))
		 return true;
	return false;
}
bool craneClass::mouseClicked(QColor pickedColor)
{
	if (horizontalOpenGl->mouseClicked(pickedColor))
		 return true;
	if (transverseOpenGl->mouseClicked(pickedColor))
		 return true;
	if (verticalOpenGl->mouseClicked(pickedColor))
		 return true;
	return false;
}
////
float stepToTarget(float pos,float target,float step)
{
	if (fabs(pos-target)>FLT_EPSILON)
	{
		if (pos<target)
			return ((pos+step)>target)?target:(pos+step);
		else
			return ((pos-step)<target)?target:(pos-step);
	}
	else
		return target;
}
void craneClass::setPlcX(float x)
{
	plcXInMeters=x;
	int dt=plcXTime.elapsed();
	if ((dt>0)&&(dt<2000))
	{
		plcXVelocityArray.append(1.0f*fabs(openGlX-x)/float(dt));
		plcXVelocity=avarage(plcXVelocityArray);
		plcXTime.restart();
		if ((plcXVelocity>0)&&(!animateTimer.isActive()))
			animateTimer.start(20);
	}
	else
	{
		openGlX=plcXInMeters;
		horizontalOpenGl->setOpenGlXpos(openGlX);
		transverseOpenGl->setOpenGlXpos(openGlX);
		verticalOpenGl->setOpenGlXpos(openGlX);
		plcXTime.restart();
	}
}
void craneClass::setPlcY(float y)
{
	plcYInMeters=y;
	int dt=plcYTime.elapsed();
	if ((dt>0)&&(dt<2000))
	{
		plcYVelocityArray.append(1.0f*fabs(openGlY-y)/float(dt));
		plcYVelocity=avarage(plcYVelocityArray);
		plcYTime.restart();
		if ((plcYVelocity>0)&&(!animateTimer.isActive()))
			animateTimer.start(20);
	}
	else
	{
		openGlY=plcYInMeters;
		transverseOpenGl->setOpenGlYpos(openGlY);
		verticalOpenGl->setOpenGlYpos(openGlY);
		plcYTime.restart();
	}
}
void craneClass::setPlcZ(float z)
{
	plcZInMeters=z;
	int dt=plcZTime.elapsed();
	if ((dt>0)&&(dt<2000))
	{
		plcZVelocityArray.append(1.0f*fabs(openGlZ-z)/float(dt));
		plcZVelocity=avarage(plcZVelocityArray);
		if ((plcZVelocity>0)&&(!animateTimer.isActive()))
			animateTimer.start(20);
	}
	else
	{
		openGlZ=plcZInMeters;
		verticalOpenGl->setOpenGlZpos(openGlZ);
		plcZTime.restart();
	}
}
float	craneClass::getPlcX()
{
	return (46.698-plcXInMeters)*1000.;
}
float	craneClass::getPlcY()
{
	return plcYInMeters*1000.;
}
float	craneClass::getPlcZ()
{
	return -plcZInMeters*1000.;
}
void craneClass::animateTimerSlot()
{
	float dt=animateTime.elapsed();
	if (plcXVelocity>FLT_EPSILON)
		openGlX=stepToTarget(openGlX,plcXInMeters,plcXVelocity*dt);
	if (plcYVelocity>FLT_EPSILON)
		openGlY=stepToTarget(openGlY,plcYInMeters,plcYVelocity*dt);
	if (plcZVelocity>FLT_EPSILON)
		openGlZ=stepToTarget(openGlZ,plcZInMeters,plcZVelocity*dt);
	animateTime.restart();

	horizontalOpenGl->setOpenGlXpos(openGlX);
	transverseOpenGl->setOpenGlXpos(openGlX);
	verticalOpenGl->setOpenGlXpos(openGlX);
	transverseOpenGl->setOpenGlYpos(openGlY);
	verticalOpenGl->setOpenGlYpos(openGlY);
	verticalOpenGl->setOpenGlZpos(openGlZ);
	if ( (fabs(openGlX-plcXInMeters)<FLT_EPSILON)&&(fabs(openGlY-plcYInMeters)<FLT_EPSILON)&&(fabs(openGlZ-plcZInMeters)<FLT_EPSILON))
	{
		animateTimer.stop();
	}
	emit openGlChangedSignal();
}
void craneClass::setCranePlcTcpServer(tcpServerClass *cranePlcTcpServer_)
{
	cranePlcTcpServer=cranePlcTcpServer_;
}
void craneClass::setConveyorsPlcTcpServer(tcpServerClass *conveyorsPlcTcpServer_)
{
	conveyorsPlcTcpServer=conveyorsPlcTcpServer_;
}

void craneClass::setAllPosisions(allPosisionsClass *all)
{
	allPosisions=all;
}
/////////////////////////////////
bool craneClass::getHasActiveMission()
{
	return hasActiveMission;
}
void craneClass::plcMemoryChanged(int index,int value)
{
	if (index==0)
		myFromPosInPlc=value;
	if (index==1)
		myToPosInPlc=value;
	if (index==2)
		myStatusInPlc=value;
	if (index==4)
		myLengthTypeInPlc=value;
	if (index==18)
		my520BasketInPlc=value;
	if (index==19)
		my510BasketInPlc=value;
	if (index==20)
		my500BasketInPlc=value;
#ifndef EMULATE
	if (activeFromBasketPos&&activeToBasketPos)
	{
		if ((myFromPosInPlc==activeFromBasketPos->getPlcId())&&(myToPosInPlc==activeToBasketPos->getPlcId())&&(myLengthTypeInPlc==activeFromBasketPos->getPlcLengthType()))
			activeMissionTimerSlot();
	}
#endif
}
void craneClass::activeMissionTimerSlot()
{
#ifdef EMULATE
	if (plcStatus==0)
#else
	if ((plcStatus==0)&&(plcModeEnum==AUTO_MODE))
#endif
	{
		if (activeFromBasketPos&&activeToBasketPos)
		{
			if (myFromPosInPlc!=activeFromBasketPos->getPlcId())
				cranePlcTcpServer->sendString(QString("N20|0|%1").arg(activeFromBasketPos->getPlcId()));
			if (myToPosInPlc!=activeToBasketPos->getPlcId())
				cranePlcTcpServer->sendString(QString("N20|1|%1").arg(activeToBasketPos->getPlcId()));
			if (myLengthTypeInPlc!=activeFromBasketPos->getPlcLengthType())
				cranePlcTcpServer->sendString(QString("N20|4|%1").arg(activeFromBasketPos->getPlcLengthType()));
			if ((myFromPosInPlc==activeFromBasketPos->getPlcId())&&(myToPosInPlc==activeToBasketPos->getPlcId())&&(myLengthTypeInPlc==activeFromBasketPos->getPlcLengthType()))
			{
				if (myStatusInPlc!=1)
					cranePlcTcpServer->sendString(QString("N20|2|1"));
			}
#ifndef EMULATE
			int basketTableId=myBasketPos->getBasketTableId()>0?myBasketPos->getBasketTableId():activeFromBasketPos->getBasketTableId();
			int basketNoToPlc=basketTableId;
			if ((activeFromBasketPos->hasBasket())&&(activeFromBasketPos->getTypeEnum()!=BASKET_NORMAL))
				basketNoToPlc=-999;
			else if ((myBasketPos->hasBasket())&&(myBasketPos->getTypeEnum()!=BASKET_NORMAL))
				basketNoToPlc=-999;
			if ((activeToBasketPos->getNumber()==500)&& (my500BasketInPlc!=basketNoToPlc))
					cranePlcTcpServer->sendString(QString("N20|20|%1").arg(basketNoToPlc));
			if ((activeFromBasketPos->getNumber()==500)&&(my500BasketInPlc!=0))
					cranePlcTcpServer->sendString(QString("N20|20|%1").arg(0));
			if ((activeToBasketPos->getNumber()==510)&&(my510BasketInPlc!=basketNoToPlc))
					cranePlcTcpServer->sendString(QString("N20|19|%1").arg(basketNoToPlc));
			if ((activeToBasketPos->getNumber()==520)&&(my520BasketInPlc!=basketNoToPlc))
					cranePlcTcpServer->sendString(QString("N20|18|%1").arg(basketNoToPlc));
#endif
			plcXVelocityArray.clear();
			plcYVelocityArray.clear();
			plcZVelocityArray.clear();
			animateTimer.start(20);
			animateTime.restart();
		}
	}
}
void craneClass::setActrivePositions(basketPositionClass* from,basketPositionClass *to)
{
	if ((from&&to)&&((!activeFromBasketPos)&&(!activeToBasketPos)))
	{
		activeFromBasketPos=from;
		activeToBasketPos=to;
		activeFromBasketPos->setLockedByCrane(true);
		activeToBasketPos->setLockedByCrane(true);
	}
	else if(((!from)&&(!to))&&(activeFromBasketPos&&activeToBasketPos))
	{
		activeFromBasketPos=NULL;
		lastToBasketPos=activeToBasketPos;
		activeToBasketPos=NULL;
		activeFromBasketPos->setLockedByCrane(false);
		activeToBasketPos->setLockedByCrane(false);
	}
}
void craneClass::clearActivePositions()
{
	if (activeFromBasketPos)
	{
		activeFromBasketPos->setLockedByCrane(false);
		activeFromBasketPos=NULL;
	}
	if (activeToBasketPos)
	{
		activeToBasketPos->setLockedByCrane(false);
		activeToBasketPos=NULL;
	}
}
bool craneClass::move(basketPositionClass *from,basketPositionClass *to)
{
	if ((!hasActiveMission)&&(from&&to)&&(plcStatus==0))
	{
		if (  (!from->isFreeToMove())||(!to->canRecieve(from))  )
			return false;
		allPosisions->getBefforeStackerPosition()->getPosition(0)->setErrorString("");
		allPosisions->getAfterStackerPosition()->getPosition(0)->setErrorString("");

		if ((from->getNumber()==400)&&(allPosisions->getPlcIsBeforeStackerPosEmpty()))
		{
			allPosisions->getBefforeStackerPosition()->getPosition(0)->setErrorString("omav plc: this position is empty");
			return false;
		}
		if ((to->getNumber()==400)&&(!allPosisions->getPlcIsBeforeStackerPosEmpty()))
		{
			allPosisions->getBefforeStackerPosition()->getPosition(0)->setErrorString("omav plc: this position is full");
			return false;
		}
		if ((from->getNumber()==410)&&(!allPosisions->getPlcIsAfterStackerPosFull()))
		{
			allPosisions->getAfterStackerPosition()->getPosition(0)->setErrorString("omav plc: this position is empty");
			return false;
		}
		if ((to->getNumber()==410)&&(allPosisions->getPlcIsAfterStackerPosFull()))
		{
			allPosisions->getAfterStackerPosition()->getPosition(0)->setErrorString("omav plc: this position is full");
			return false;
		}
		setActrivePositions(from,to);
		hasActiveMission=true;
		execQuery(QString("update positions set targetNumber=%1 , targetIndex=%2 where posNumber=%3 and posIndex=%4").arg(activeToBasketPos->getNumber()).arg(activeToBasketPos->getIndex()).arg(activeFromBasketPos->getNumber()).arg(activeFromBasketPos->getIndex()),MAIN_DB_CONNECTION);
		QString stmnt=QString("insert into basketHistory (type,position,destination,basketTableId,date,sec) VALUES('%1','%2','%3',%4,'%5','%6')")
			.arg("new").arg((from->getDescription().trimmed()+" %1 %2").arg(from->getNumber()).arg(from->getIndex()))
			.arg((  to->getDescription().trimmed()+" %1 %2").arg(  to->getNumber()).arg(  to->getIndex())).arg(QString().number(from->getBasketTableId())).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t());
		execQuery(stmnt,MAIN_DB_CONNECTION);
		activeMissionTimerSlot();
		activeMissionTimer.start(activeMissionTimerTime);
	}
	return true;
}
void craneClass::loadFromDatabase()
{
	myBasketPos->loadFromDataBase();
	int fromNumber=GETONEATMOST(QString("select posNumber from positions where targetNumber<>0")).toInt();
	int fromIndex=GETONEATMOST(QString("select posIndex from positions where targetIndex<>0")).toInt();
	int toNumber=GETONEATMOST(QString("select targetNumber from positions where targetNumber<>0")).toInt();
	int toIndex=GETONEATMOST(QString("select targetIndex from positions where targetIndex<>0")).toInt();
	if ((fromNumber==0)&&(fromIndex==0)&&(toNumber==0)&&(toIndex==0))
	{
		hasActiveMission=false;
		clearActivePositions();
	}
	else
	{
		//myBasketPos=NULL;
	//	clearActivePositions();
		if ((fromNumber==0)||(fromIndex==0)||(toNumber==0)||(toIndex==0))//all not zero
			exit("error in database","error in positions table");
		basketPositionClass *from=allPosisions->getPosition(fromNumber,fromIndex-1);
		basketPositionClass *to=allPosisions->getPosition(toNumber,toIndex-1);
		setActrivePositions(from,to);
		//if (activeFromBasketPos->getBasketTableId()!=0)
		//{
		//		myBasketPos->setBasketTableId(activeFromBasketPos->getBasketTableId());
		//		activeFromBasketPos->setBasketTableId(0);
		//}
		hasActiveMission=true;
		activeMissionTimerSlot();
		activeMissionTimer.start(activeMissionTimerTime);
	}
}
void craneClass::plcStatusChanged(int status)
{
	if (status==0)
	{
		if (hasActiveMission)//plc aborded
		{
			QMessageBox *msgBox=new QMessageBox();
			msgBox->setWindowIcon(QIcon(":/logo_16x16.png"));
			msgBox->setInformativeText("User information needed.");
			msgBox->setText("Important!\nBecause of manual intervention PLC closed the active mission.\nHow did the mission finished?\nIf the basket remained in its initial position choose 'Yes.'\nIf the basket went to the targeted position choose 'No'.\nIf none of the above happened chose 'Close' to quit the software and manually update the database.");
			msgBox->setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Close);
			msgBox->setDefaultButton(QMessageBox::Yes);
			int ans=msgBox->exec();
			////////////////////////////////////////////////////
			{
				int activeBasket=myBasketPos->getBasketTableId()>0?myBasketPos->getBasketTableId():activeFromBasketPos->getBasketTableId();
				QFile file("userSelections.txt");
				file.open(QIODevice::Append|QIODevice::Text);
				QTextStream out(&file);
				out<<QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss\t");
				if (ans==QMessageBox::Yes)
					out<<QString(" user selected that the mission was aborded, selected yes. basket:%1 from:%2 to:%3").arg(activeBasket).arg(activeFromBasketPos->getPlcId()).arg(activeToBasketPos->getPlcId())<<"\n";
				if (ans==QMessageBox::No)
					out<<QString(" user selected that the mission finished properly, selected no. basket:%1 from:%2 to:%3").arg(activeBasket).arg(activeFromBasketPos->getPlcId()).arg(activeToBasketPos->getPlcId())<<"\n";
				file.close();
			}
			////////////////////////////////////////////////////
			if (ans==QMessageBox::Yes)
				abordMission();
			else if (ans==QMessageBox::No)
				closeMission();
			else if (ans==QMessageBox::Close)
			{
				execQuery(QString("update positions set targetNumber=0 , targetIndex=0 where posNumber=%1 and posIndex=%2").arg(activeFromBasketPos->getNumber()).arg(activeFromBasketPos->getIndex()),MAIN_DB_CONNECTION);
				EXIT(tr("user selection"),tr("user selected to exit software to modify the database."));
			}
			delete msgBox;
		}
		else
		{
			activeMissionTimerSlot();
			activeMissionTimer.start(activeMissionTimerTime);
		}
		plcStatus=status;
	}
	else if (status==1)
	{
		plcStatus=status;
		activeMissionTimer.stop();
	}
	else if (status==2)
	{
		if (plcStatus!=status)//only the first time
		{
			if (closeMission())
			{
				activeMissionTimer.stop();
				plcStatus=status;
			}
		}
		if (!hasActiveMission)
		{
			cranePlcTcpServer->sendString(QString("N20|2|2"));
			plcStatus=status;
		}
	}
	else if (status==3)
	{
		if (plcStatus!=status)//only the first time
		{
			if (abordMission())
			{
				activeMissionTimer.stop();
				plcStatus=status;
			}
		}
		if (!hasActiveMission)
		{
			cranePlcTcpServer->sendString(QString("N20|2|3"));
			plcStatus=status;
		}
	}
}
void craneClass::plcStatusN2015Changed(int status)
{
	QVector<bool> statusBits=deprecatedCalculateBits(status);
	isCarryingBasket=statusBits[14]?true:false;
	if (hasActiveMission)
	{
		if (isCarryingBasket)
		{
			if (activeFromBasketPos->getBasketTableId()!=0)//first time only
			{
				myBasketPos->setBasketTableId(activeFromBasketPos->getBasketTableId());
				activeFromBasketPos->setBasketTableId(0);
			}
		}
	}
	if (statusBits[10])
		plcModeEnum=MAINTENANCE_MODE;
	if (statusBits[12])
		plcModeEnum=STEP_MODE;
	if (statusBits[11])
		plcModeEnum=AUTO_MODE;
}
//void craneClass::plcStatusN2016Changed(int status)
//{
//	QVector<bool> statusBits=calculateBits(status);
//	missionFinishedWithOverWeight=statusBits[5];
//}
basketPositionClass* craneClass::getLastToPosition()
{
	return lastToBasketPos;
}
bool craneClass::closeMission()
{
	if (!hasActiveMission)return false;
	//if (activeToBasketPos&&activeToBasketPos->getPlcId()==401)//if target is before stacker i don't close the mission till i see that he is not asking for empty basket, that he took the basket
	//{
	//	if (allPosisions->isPosBeforeStacketEmpty())
	//		return false;
	//}
	int basketTableId=myBasketPos->getBasketTableId()>0?myBasketPos->getBasketTableId():activeFromBasketPos->getBasketTableId();
#ifdef EMULATE
	if (activeToBasketPos->getNumber()==400)
		cranePlcTcpServer->sendString(QString("401|0|%1").arg(basketTableId));
	if (activeFromBasketPos->getNumber()==410)
		cranePlcTcpServer->sendString(QString("411|0|%1").arg(0));

	int basketNoToPlc=basketTableId;
	if ((activeFromBasketPos->hasBasket())&&(activeFromBasketPos->getTypeEnum()!=BASKET_NORMAL))
		basketNoToPlc=-999;
	else if ((myBasketPos->hasBasket())&&(myBasketPos->getTypeEnum()!=BASKET_NORMAL))
		basketNoToPlc=-999;
	if (activeToBasketPos->getNumber()==500)
		cranePlcTcpServer->sendString(QString("N20|20|%1").arg(basketNoToPlc));
	if (activeFromBasketPos->getNumber()==500)
		cranePlcTcpServer->sendString(QString("N20|20|%1").arg(0));
	if (activeToBasketPos->getNumber()==510)
		cranePlcTcpServer->sendString(QString("N20|19|%1").arg(basketNoToPlc));
	if (activeToBasketPos->getNumber()==520)
		cranePlcTcpServer->sendString(QString("N20|18|%1").arg(basketNoToPlc));

		cranePlcTcpServer->sendString(QString("N20|0|0"));
		cranePlcTcpServer->sendString(QString("N20|1|0"));
		cranePlcTcpServer->sendString(QString("N20|2|0"));
		cranePlcTcpServer->sendString(QString("N20|4|0"));
#endif
	if (activeFromBasketPos->getNumber()==410)
	{
		emit missionFromAfterStackerPositionFinishedSignal();
	}
	if (activeFromBasketPos->getNumber()==420)
	{
		emit scrapBasketPositionChangedSignal(0);
		allPosisions->setScrapBasketIsReady(false);
	}
	if (activeToBasketPos->getNumber()==420)
	{
		emit scrapBasketPositionChangedSignal(basketTableId);
		allPosisions->setScrapBasketIsReady(false);
	}
	if (myBasketPos->getBasketTableId()!=0)
	{
		activeToBasketPos->setBasketTableId(myBasketPos->getBasketTableId());
		myBasketPos->setBasketTableId(0);
	}
	else if (activeFromBasketPos->getBasketTableId()!=0)
	{
		activeToBasketPos->setBasketTableId(activeFromBasketPos->getBasketTableId());
		activeFromBasketPos->setBasketTableId(0);
	}
///notify conveyors plc
	if (
		  (activeToBasketPos->getIndex()==1)
			&&
			( (activeToBasketPos->getNumber()==160)||(activeToBasketPos->getNumber()==100)||(activeToBasketPos->getNumber()==110)||(activeToBasketPos->getNumber()==120)||(activeToBasketPos->getNumber()==130)||(activeToBasketPos->getNumber()==140)||(activeToBasketPos->getNumber()==150))
			)//theseis sto fourno h katw thesh
			allPosisions->changeConveyorsBasketMemory(activeToBasketPos->getPlcId(),basketTableId);
	if (
		  (activeFromBasketPos->getIndex()==1)
			&&
			( (activeFromBasketPos->getNumber()==160)||(activeFromBasketPos->getNumber()==100)||(activeFromBasketPos->getNumber()==110)||(activeFromBasketPos->getNumber()==120)||(activeFromBasketPos->getNumber()==130)||(activeFromBasketPos->getNumber()==140)||(activeFromBasketPos->getNumber()==150))
			)//theseis sto fourno h katw thesh
			allPosisions->changeConveyorsBasketMemory(activeFromBasketPos->getPlcId(),0);
///
	//clear from exports
	if (activeToBasketPos->getNumber()==520)
		execQuery(QString("delete from toAssimakisExports where basketTableId=%1").arg(basketTableId),MAIN_DB_CONNECTION);
	if (activeToBasketPos->getNumber()==510)
		execQuery(QString("delete from toOmavExports where basketTableId=%1").arg(basketTableId),MAIN_DB_CONNECTION);
	//
///lock the position if it is the in-out not to take it in auto mode, it unlockes when new basket is comming from out
	if (activeToBasketPos->getNumber()==500)
		activeToBasketPos->setLocked(true);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	execQuery(QString("update positions set targetNumber=0 , targetIndex=0 where posNumber=%1 and posIndex=%2").arg(activeFromBasketPos->getNumber()).arg(activeFromBasketPos->getIndex()),MAIN_DB_CONNECTION);
	hasActiveMission=false;
	//check that is clear in database
	int fromNumber=GETONEATMOST(QString("select posNumber from positions where targetNumber<>0")).toInt();
	int fromIndex=GETONEATMOST(QString("select posIndex from positions where targetIndex<>0")).toInt();
	if ((fromNumber==0)&&(fromIndex==0))
		cranePlcTcpServer->sendString(QString("N20|2|2"));
	////////////////////////
	QString stmnt=QString("insert into basketHistory (type,position,destination,basketTableId,date,sec) VALUES('%1','%2','%3',%4,'%5','%6')")
		.arg("end").arg((activeFromBasketPos->getDescription().trimmed()+" %1 %2").arg(activeFromBasketPos->getNumber()).arg(activeFromBasketPos->getIndex()))
		.arg((  activeToBasketPos->getDescription().trimmed()+" %1 %2").arg(  activeToBasketPos->getNumber()).arg(  activeToBasketPos->getIndex())).arg(QString().number(activeToBasketPos->getBasketTableId())).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t());
	execQuery(stmnt,MAIN_DB_CONNECTION);
	////////////////////////
	clearActivePositions();
	emit openGlChangedSignal();
#ifdef EMULATE
	cranePlcTcpServer->sendString(QString("N20|20|%1").arg(allPosisions->getPosition(500,0)->getBasketNumber()));
	cranePlcTcpServer->sendString(QString("401|0|%1").arg(allPosisions->getPosition(400,0)->getBasketNumber()));
	cranePlcTcpServer->sendString(QString("411|0|%1").arg(allPosisions->getPosition(410,0)->getBasketNumber()));
#endif
	return true;
}
bool craneClass::abordMission()
{
	if (!hasActiveMission)return false;
	QString fromPosStr=activeFromBasketPos->getDescription().trimmed()+QString("%1 %2").arg(activeFromBasketPos->getNumber()).arg(activeFromBasketPos->getIndex());
	QString toPosStr=activeToBasketPos->getDescription().trimmed()+QString("%1 %2").arg(activeToBasketPos->getNumber()).arg(activeToBasketPos->getIndex());
	int basketTableId=myBasketPos->getBasketTableId()>0?myBasketPos->getBasketTableId():activeFromBasketPos->getBasketTableId();
	//int lengthType=BASKET_CONTENS_NONE;
	//if (activeFromBasketPos->getBasketTableId()>0)
	//	lengthType=activeFromBasketPos->getContentsEnum();
	//else
	//	lengthType=myBasketPos->getContentsEnum();
	//if (!missionFinishedWithOverWeight)
	//{
	//	if (lengthType==BASKET_CONTENS_MAX_LENGTH_24)
	//		execQuery(QString("insert into basket_details (basketnum,dienum,sequence,SONum,SOItemNum,Lot,Pc,PcLen,Wt) VALUES('%1',7777,7777,7777,7777,7777,7777,7777,7777)").arg(basketTableId),APEX_DB_CONNECTION);
	//	else if (lengthType==BASKET_CONTENS_MAX_LENGTH_30)
	//		activeFromBasketPos->setLocked(true);
	//}
	//else
		activeFromBasketPos->setLocked(true);
	//
	if (myBasketPos->getBasketTableId()!=0)
	{
		activeFromBasketPos->setBasketTableId(myBasketPos->getBasketTableId());
		myBasketPos->setBasketTableId(0);
	}
	else if (activeFromBasketPos->getBasketTableId()!=0)
	{
		//nothing
	}
	hasActiveMission=false;
	execQuery(QString("update positions set targetNumber=0 , targetIndex=0 where posNumber=%1 and posIndex=%2").arg(activeFromBasketPos->getNumber()).arg(activeFromBasketPos->getIndex()),MAIN_DB_CONNECTION);
	cranePlcTcpServer->sendString(QString("N20|2|3"));
	clearActivePositions();
	emit openGlChangedSignal();
	//
	QString stmnt=QString("insert into basketHistory (type,position,destination,basketTableId,date,sec) VALUES('lock','%1','',%2,'%3','%4')").arg(fromPosStr).arg(basketTableId).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t());
	execQuery(stmnt,MAIN_DB_CONNECTION);
	stmnt=QString("insert into basketHistory (type,position,destination,basketTableId,date,sec) VALUES('abord','%1','%2',%3,'%4','%5')").arg(fromPosStr).arg(toPosStr).arg(basketTableId).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t());
	execQuery(stmnt,MAIN_DB_CONNECTION);
	return true;
}

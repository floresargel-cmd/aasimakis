#include "cr.h"
#include <QMessageBox>
#include "configuration/ConfigurationManager.h"
#include "dbTableStatus.h"
#include "plcNetworkStatus.h"
#include "../common/execDBQuery.h"

// #define SEMILIVE // @@@@@ - This allows the app to run even if 1 is already present.  Don't allow AUTO mode in this mode, though.

//extern float plcSetXVelocityForOpenGl;
//extern float plcSetYVelocityForOpenGl;
//extern float plcSetZVelocityForOpenGl;

// #define activeMissionTimerTime 1500 // @@@@@ -- Slowing down the active mission timer
#define activeMissionTimerTime 3000 // @@@@@ -- slowing this down from 1.5s to 3s for testing

craneClass::craneClass():QObject()
{
	plcXInMeters = plcYInMeters = plcZInMeters = 0.0;
	lastPlcXInMeters = lastPlcYInMeters = lastPlcZInMeters = 0.0;
	openGlX = openGlY = openGlZ = 0.0f;
	plcXVelocity = plcYVelocity = plcZVelocity = 0.0f;
	lastToBasketPos = NULL;
	//missionFinishedWithOverWeight=false;
	plcStatus = myFromPosInPlc = myToPosInPlc = myLengthTypeInPlc = myStatusInPlc = my500BasketInPlc = my510BasketInPlc = my520BasketInPlc = -1;
	activeFromBasketPos = activeToBasketPos = NULL;
	myBasketPos = new basketPositionClass(NULL, "crane", 1, 1, basketPositionClass::EPICS_POS_BASKET_NOWHERE, 0.0f, 0.0f, 0.0f);

	isCarryingBasket = false;
	hasActiveMission = false;

	horizontalOpenGl = new openGLPartClass(QString(":/foreas.stl"), WHITE_TEXTURE_QUAD, WHITE_TEXTURE_QUAD);
	transverseOpenGl = new openGLPartClass(QString(":/forio.stl"), WHITE_TEXTURE_QUAD, WHITE_TEXTURE_QUAD);
	verticalOpenGl = new openGLPartClass(QString(":/tsimpidia.stl"), WHITE_TEXTURE_QUAD, WHITE_TEXTURE_QUAD);

	connect(&animateTimer, SIGNAL(timeout()), this, SLOT(animateTimerSlot()));
	connect(&activeMissionTimer, SIGNAL(timeout()), this, SLOT(activeMissionTimerSlot()));
	//
	plcXTime.start();
	plcYTime.start();
	plcZTime.start();
	plcModeEnum = NO_MODE;
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
		// The legacy renderer displayed the crane-held basket with the neutral
		// white material tint. Set it explicitly so its appearance no longer
		// depends on whichever OpenGL color was left by the previous draw.
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
		glColor3ub(255, 255, 255);
		glPushMatrix();
		glTranslatef(openGlX - 3.5f, openGlY + 0.60f, 2.75 + openGlZ);
		myBasketPos->drawBasket(true);
		glPopMatrix();
		glPopAttrib();
	}
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
	glColor3ub(255, 200, 200);
	horizontalOpenGl->draw();
	transverseOpenGl->draw();
	verticalOpenGl->draw();
	glPopAttrib();

	}

void craneClass::openGlDrawMissionMarkers()
{
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
			animateTimer.start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().CraneAnimationMilliseconds());
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
		plcYVelocityArray.append(1.0f * fabs(openGlY-y)/float(dt));
		plcYVelocity=avarage(plcYVelocityArray);
		plcYTime.restart();
		if ((plcYVelocity>0)&&(!animateTimer.isActive()))
			animateTimer.start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().CraneAnimationMilliseconds());
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
			animateTimer.start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().CraneAnimationMilliseconds());
	}
	else
	{
		openGlZ=plcZInMeters;
		verticalOpenGl->setOpenGlZpos(openGlZ);
		plcZTime.restart();
	}
}

float craneClass::getPlcX()
{
	return (46.698-plcXInMeters)*1000.;
}
float craneClass::getPlcY()
{
	return plcYInMeters*1000.;
}
float craneClass::getPlcZ()
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

	if ((fabs(openGlX - plcXInMeters) < FLT_EPSILON) && (fabs(openGlY - plcYInMeters) < FLT_EPSILON) && (fabs(openGlZ - plcZInMeters) < FLT_EPSILON))
	{
		animateTimer.stop();
	}

	emit openGlChangedSignal();
}

void craneClass::setCranePlcTcpServer(tcpServerClass *cranePlcTcpServer_)
{
	cranePlcTcpServer = cranePlcTcpServer_;
}

void craneClass::setConveyorsPlcTcpServer(tcpServerClass *conveyorsPlcTcpServer_)
{
	conveyorsPlcTcpServer = conveyorsPlcTcpServer_;
}

void craneClass::setallPositions(allPositionsClass *all)
{
	allPositions = all;
}

/////////////////////////////////
bool craneClass::getHasActiveMission()
{
	return hasActiveMission;
}

bool craneClass::isAssignmentReservedForMission(basketPositionClass* position, int basketId)
{
	// Protect the whole source/destination stack, as well as the carried basket.
	// Endpoints can remain set while a mission is being opened or closed.
	if (position == myBasketPos)
		return true;
	basketPositionClass* reserved[] = { activeFromBasketPos, activeToBasketPos, myBasketPos };
	for (int i = 0; i < 3; ++i)
	{
		if (!reserved[i])
			continue;
		if (position && position->getNumber() == reserved[i]->getNumber())
			return true;
		if (basketId > 0 && basketId == reserved[i]->getBasketTableId())
			return true;
	}
	return false;
}

void craneClass::plcMemoryChanged(int index, int value)
{
	if (index == 0)
		myFromPosInPlc = value;

	if (index == 1)
		myToPosInPlc = value;

	if (index == 2)
		myStatusInPlc = value;

	if (index == 4)
		myLengthTypeInPlc = value;

	if (index == 18)
		my520BasketInPlc = value;

	if (index == 19)
		my510BasketInPlc = value;

	if (index == 20)
		my500BasketInPlc = value;

	if (isDatabaseReadOnlyMode() && (index == 0 || index == 1 || index == 2))
		synchronizeObserverMissionFromPlc();

#ifndef EMULATE
	if (activeFromBasketPos && activeToBasketPos)
	{
		if ((myFromPosInPlc == activeFromBasketPos->getPlcId()) && (myToPosInPlc == activeToBasketPos->getPlcId()) && (myLengthTypeInPlc == activeFromBasketPos->getPlcLengthType()))
			activeMissionTimerSlot();
	}
#endif
}

void craneClass::activeMissionTimerSlot()
{
#ifdef EMULATE
	if (plcStatus==0)
#else
	if ((plcStatus == 0) && (plcModeEnum == AUTO_MODE))
#endif
	{
		if (activeFromBasketPos && activeToBasketPos)
		{
			setPlcNetworkActive(PLC_NETWORK_CRANE, false);
			if (myFromPosInPlc != activeFromBasketPos->getPlcId())
				cranePlcTcpServer->sendString(QString("N20|0|%1").arg(activeFromBasketPos->getPlcId()));

			if (myToPosInPlc != activeToBasketPos->getPlcId())
				cranePlcTcpServer->sendString(QString("N20|1|%1").arg(activeToBasketPos->getPlcId()));

			if (myLengthTypeInPlc != activeFromBasketPos->getPlcLengthType())
				cranePlcTcpServer->sendString(QString("N20|4|%1").arg(activeFromBasketPos->getPlcLengthType()));

			if ((myFromPosInPlc == activeFromBasketPos->getPlcId()) && (myToPosInPlc == activeToBasketPos->getPlcId()) && (myLengthTypeInPlc == activeFromBasketPos->getPlcLengthType()))
			{
				if (myStatusInPlc != 1)
					cranePlcTcpServer->sendString(QString("N20|2|1"));
			}

#ifndef EMULATE
			int basketTableId = myBasketPos->getBasketTableId() > 0 ? myBasketPos->getBasketTableId() : activeFromBasketPos->getBasketTableId();
			int basketNoToPlc = basketTableId;

			if ((activeFromBasketPos->hasBasket()) && (activeFromBasketPos->getTypeEnum() != BASKET_NORMAL))
				basketNoToPlc = -999;
			else if ((myBasketPos->hasBasket()) && (myBasketPos->getTypeEnum() != BASKET_NORMAL))
				basketNoToPlc = -999;

			if ((activeToBasketPos->getNumber() == 500) && (my500BasketInPlc != basketNoToPlc))
				cranePlcTcpServer->sendString(QString("N20|20|%1").arg(basketNoToPlc));

			if ((activeFromBasketPos->getNumber() == 500) && (my500BasketInPlc != 0))
				cranePlcTcpServer->sendString(QString("N20|20|%1").arg(0));

			if ((activeToBasketPos->getNumber() == 510) && (my510BasketInPlc != basketNoToPlc))
				cranePlcTcpServer->sendString(QString("N20|19|%1").arg(basketNoToPlc));

			if ((activeToBasketPos->getNumber() == 520) && (my520BasketInPlc != basketNoToPlc))
				cranePlcTcpServer->sendString(QString("N20|18|%1").arg(basketNoToPlc));
#endif
			setPlcNetworkIdle(PLC_NETWORK_CRANE);

			plcXVelocityArray.clear();
			plcYVelocityArray.clear();
			plcZVelocityArray.clear();
			animateTimer.start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().CraneAnimationMilliseconds());
			animateTime.restart();
		}
	}
}

void craneClass::setActivePositions(basketPositionClass* from, basketPositionClass *to)
{
	if ((from && to) && ((!activeFromBasketPos) && (!activeToBasketPos)))
	{
		activeFromBasketPos = from;
		activeToBasketPos = to;
		activeFromBasketPos->setLockedByCrane(true);
		activeToBasketPos->setLockedByCrane(true);
	}
	else if (((!from) && (!to)) && (activeFromBasketPos && activeToBasketPos))
	{
		lastToBasketPos = activeToBasketPos;
		clearActivePositions();
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

void craneClass::showAutomaticMoveError(basketPositionClass* from, basketPositionClass* to, const QString& reason)
{
	QMessageBox* alert = new QMessageBox(QMessageBox::Warning, tr("Automatic basket move rejected"),
		QString(), QMessageBox::Ok, QApplication::activeWindow());
	alert->setAttribute(Qt::WA_DeleteOnClose);
	alert->setTextFormat(Qt::PlainText);
	alert->setText(tr("Basket %1 could not be moved from position %2/%3 to %4/%5.")
		.arg(from->getBasketTableId()).arg(from->getNumber()).arg(from->getIndex())
		.arg(to->getNumber()).arg(to->getIndex()));
	alert->setInformativeText(tr("The database rejected this move.") + "\n\n" + reason
		+ tr("\n\nThe move was not started. Check the basket contents and destination. Identical automatic retries will not show another popup until a move to this destination succeeds."));
	// Keep processing PLC/status updates while the operator reads the message.
	alert->open();
}
bool craneClass::move(basketPositionClass *from, basketPositionClass *to, QString* rejectionReason)
{
	if (rejectionReason) *rejectionReason = tr("The crane is not ready, or the source or destination is unavailable. Check the position and crane status before trying again.");
	if ((!hasActiveMission) && (!activeFromBasketPos) && (!activeToBasketPos) && (from&&to) && (plcStatus == 0))
	{
		if (!from->isFreeToMove())
			return false;

		// Defense in depth for direct/manual moves. Automatic destination selection
		// applies the same rule in positionListClass::getFreeToRecievePos().
		if (!from->canMoveToPosition(to->getNumber()))
		{
			LOG("CRANE MOVE", QString("Blocked basket %1 with T1/T4/F material from age oven load position %2")
				.arg(from->getBasketTableId()).arg(to->getNumber()));
			return false;
		}

		if (!to->canRecieve(from))
			return false;

		// These database-backed lists can exist without a position row.
		basketPositionClass* beforeStacker = allPositions->getBeforeStackerPosition()->getPosition(0);
		basketPositionClass* afterStacker = allPositions->getAfterStackerPosition()->getPosition(0);
		if (!beforeStacker || !afterStacker)
		{
			LOG("ERROR: CRANE MOVE", "Move rejected: missing position 400/1 or 410/1 in the positions table");
			return false;
		}
		beforeStacker->setErrorString("");
		afterStacker->setErrorString("");

		if ((from->getNumber() == 400) && (allPositions->getPlcIsBeforeStackerPosEmpty()))
		{
			beforeStacker->setErrorString("from -> this position is empty");
			return false;
		}

		if ((to->getNumber() == 400) && (!allPositions->getPlcIsBeforeStackerPosEmpty()))
		{
			beforeStacker->setErrorString("to -> this position is full");
			return false;
		}

		if ((from->getNumber() == 410) && (!allPositions->getPlcIsAfterStackerPosFull()))
		{
			afterStacker->setErrorString("from -> this position is empty");
			return false;
		}

		if ((to->getNumber() == 410) && (allPositions->getPlcIsAfterStackerPosFull()))
		{
			afterStacker->setErrorString("to -> this position is full");
			return false;
		}

		// A database trigger can reject a destination (for example, a basket
		// recorded as full at an empty-basket input). Do not publish a mission
		// or send PLC commands until both its target and history are committed.
		const QString targetStatement = QString("update positions set targetNumber=%1 , targetIndex=%2 where posNumber=%3 and posIndex=%4")
			.arg(to->getNumber()).arg(to->getIndex()).arg(from->getNumber()).arg(from->getIndex());
		const QString fromDescription = (from->getDescription().trimmed() + " %1 %2")
			.arg(from->getNumber()).arg(from->getIndex()).replace("'", "''");
		const QString toDescription = (to->getDescription().trimmed() + " %1 %2")
			.arg(to->getNumber()).arg(to->getIndex()).replace("'", "''");
		const QString historyStatement = QString("insert into basketHistory (type,position,destination,basketTableId,date,sec) VALUES('%1','%2','%3',%4,'%5','%6')")
			.arg("new").arg(fromDescription).arg(toDescription).arg(from->getBasketTableId())
			.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t());
		setDbTableActive("positions", false);
		setDbTableActive("basketHistory", false);
		QString databaseError;
		const bool saved = execDBTransaction(QStringList() << targetStatement << historyStatement, MAIN_DB_CONNECTION, 3, &databaseError);
		setDbTableIdle("positions");
		setDbTableIdle("basketHistory");
		const QString destinationKey = QString("%1/%2").arg(to->getNumber()).arg(to->getIndex());
		if (!saved)
		{
			// Manual callers display their own popup using rejectionReason.
			if (!rejectionReason)
			{
				const QString failureKey = QString("%1/%2/%3|").arg(from->getBasketTableId())
					.arg(from->getNumber()).arg(from->getIndex()) + databaseError;
				if (automaticMoveErrors.value(destinationKey) != failureKey)
				{
					automaticMoveErrors.insert(destinationKey, failureKey);
					showAutomaticMoveError(from, to, databaseError);
				}
			}
			to->setErrorString(tr("Move rejected by database; see application log"));
			if (rejectionReason) *rejectionReason = tr("The database rejected this move.") + "\n\n" + databaseError;
			LOG("ERROR: CRANE MOVE", QString("Move not started: basket=%1 from=%2/%3 to=%4/%5; database transaction failed")
				.arg(from->getBasketTableId()).arg(from->getNumber()).arg(from->getIndex())
				.arg(to->getNumber()).arg(to->getIndex()));
			return false;
		}

		automaticMoveErrors.remove(destinationKey);
		if (rejectionReason) rejectionReason->clear();
		to->setErrorString("");
		setActivePositions(from, to);
		hasActiveMission = true;
		activeMissionTimerSlot();
		activeMissionTimer.start(activeMissionTimerTime);
		return true;
	}
	return false;
}

void craneClass::loadFromDatabase()
{
	LOG("cr.cpp", "craneClass::loadFromDatabase()");
	myBasketPos->loadFromDataBase();

	int fromNumber = GETONEATMOST(QString("select posNumber from positions where targetNumber <> 0")).toInt();
	int fromIndex = GETONEATMOST(QString("select posIndex from positions where targetIndex <> 0")).toInt();
	int toNumber = GETONEATMOST(QString("select targetNumber from positions where targetNumber <> 0")).toInt();
	int toIndex = GETONEATMOST(QString("select targetIndex from positions where targetIndex <> 0")).toInt();
	LOG("cr.cpp", QString("craneClass::loadFromDatabase() fromNumber:%1 fromIndex:%2 toNumber:%3 toIndex:%4").arg(fromNumber).arg(fromIndex).arg(toNumber).arg(toIndex));

	if ((fromNumber == 0) && (fromIndex == 0) && (toNumber == 0) && (toIndex==0))
	{
		hasActiveMission = false;
		clearActivePositions();
		LOG("cr.cpp", "craneClass::loadFromDatabase() no active mission");
	}
	else
	{
		//myBasketPos=NULL;
		//	clearActivePositions();
		if ((fromNumber == 0) || (fromIndex == 0) || (toNumber == 0) || (toIndex == 0)) //all not zero
			exit("error in database", "error : positions table - active mission values, review target values");

		basketPositionClass *from = allPositions->getPosition(fromNumber, fromIndex - 1);
		basketPositionClass *to = allPositions->getPosition(toNumber, toIndex - 1);
		setActivePositions(from, to);
		LOG("cr.cpp", QString("craneClass::loadFromDatabase() active mission from:%1 to:%2").arg(from->getDescription()).arg(to->getDescription()));

		//if (activeFromBasketPos->getBasketTableId()!=0)
		//{
		//		myBasketPos->setBasketTableId(activeFromBasketPos->getBasketTableId());
		//		activeFromBasketPos->setBasketTableId(0);
		//}

		hasActiveMission = true;
		activeMissionTimerSlot();
		activeMissionTimer.start(activeMissionTimerTime);
		LOG("cr.cpp", "craneClass::loadFromDatabase() active mission timer started");
	}
}

void craneClass::plcStatusChanged(int status)
{
	#ifdef SEMILIVE // @@@@@ - avoid doing anything with the PLCs in SEMILIVE mode
		LOG("cr.cpp", QString("craneClass::plcStatusChanged() in SEMILIVE mode, ignoring PLC status change to %1").arg(status));
		return;
	#endif

	// We are LIVE, but check to see if we are the enabled basketcrane before doing anything.
	if (!theOneBCToRuleThemAll)
	{
		LOG("cr.cpp", QString("craneClass::plcStatusChanged() not the enabled basket crane, ignoring PLC status change to %1").arg(status));
		return;
	}

	if (status == 0)
	{
		if (hasActiveMission)  //plc aborted
		{
			QMessageBox *msgBox=new QMessageBox();
			msgBox->setWindowIcon(QIcon(":/logo_16x16.png"));
			msgBox->setInformativeText("User information needed.");
			msgBox->setText("Important!\nBecause of manual intervention PLC closed the active mission.\nHow did the mission finished?\nIf the basket remained in its initial position choose 'Yes.'\nIf the basket went to the targeted position choose 'No'.\nIf none of the above happened chose 'Close' to quit the software and manually update the database.");
			msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Close);
			msgBox->setDefaultButton(QMessageBox::Yes);

			int ans = msgBox->exec();
			////////////////////////////////////////////////////
			{
				int activeBasket = myBasketPos->getBasketTableId() > 0 ? myBasketPos->getBasketTableId() : activeFromBasketPos->getBasketTableId();
				QFile file("userSelections.txt");
				file.open(QIODevice::Append | QIODevice::Text);
				QTextStream out(&file);
				out << QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss\t");

				if (ans == QMessageBox::Yes)
					out << QString(" user selected that the mission was aborted, selected yes. basket:%1 from:%2 to:%3").arg(activeBasket).arg(activeFromBasketPos->getPlcId()).arg(activeToBasketPos->getPlcId()) << "\n";

				if (ans == QMessageBox::No)
					out << QString(" user selected that the mission finished properly, selected no. basket:%1 from:%2 to:%3").arg(activeBasket).arg(activeFromBasketPos->getPlcId()).arg(activeToBasketPos->getPlcId()) << "\n";

				file.close();
			}

			////////////////////////////////////////////////////
			if (ans == QMessageBox::Yes)
				abortMission();
			else if (ans == QMessageBox::No)
				closeMission();
			else if (ans == QMessageBox::Close)
			{
				setDbTableActive("positions", false);
				execDBQuery(QString("update positions set targetNumber = 0, targetIndex = 0 where posNumber = %1 and posIndex = %2").arg(activeFromBasketPos->getNumber()).arg(activeFromBasketPos->getIndex()), MAIN_DB_CONNECTION);
				setDbTableIdle("positions");
				EXIT(tr("user selection"), tr("user selected to exit software to modify the database."));
			}
			delete msgBox;
		}
		else
		{
			activeMissionTimerSlot();
			activeMissionTimer.start(activeMissionTimerTime);
		}

		plcStatus = status;
	}
	else if (status == 1)  // in mission
	{
		// in mission
		plcStatus = status;
		activeMissionTimer.stop();
	}
	else if (status == 2)  // close
	{
		// mission complete
		if (plcStatus != status)//only the first time
		{
			if (closeMission())
			{
				activeMissionTimer.stop();
				plcStatus = status;
			}
		}
		if (!hasActiveMission)
		{
			setPlcNetworkActive(PLC_NETWORK_CRANE, false);
			cranePlcTcpServer->sendString(QString("N20|2|2"));
			setPlcNetworkIdle(PLC_NETWORK_CRANE);
			plcStatus = status;
		}
	}
	else if (status == 3)  // abort
	{
		if (plcStatus != status) //only the first time
		{
			if (abortMission())
			{
				activeMissionTimer.stop();
				plcStatus = status;
			}
		}
		if (!hasActiveMission)
		{
			setPlcNetworkActive(PLC_NETWORK_CRANE, false);
			cranePlcTcpServer->sendString(QString("N20|2|3"));
			setPlcNetworkIdle(PLC_NETWORK_CRANE);
			plcStatus = status;
		}
	}
}

void craneClass::plcStatusN20_15Changed(int status)
{
	QVector<bool> statusBits = deprecatedCalculateBits(status);

	const bool wasCarryingBasket = isCarryingBasket;
	isCarryingBasket = statusBits[14] ? true : false;
	if (isDatabaseReadOnlyMode())
		synchronizeObserverMissionFromPlc();
	if (isCarryingBasket != wasCarryingBasket)
		emit openGlChangedSignal();
	if (hasActiveMission)
	{
		if (isCarryingBasket)
		{
			if (activeFromBasketPos->getBasketTableId() != 0)//first time only
			{
				myBasketPos->setBasketTableId(activeFromBasketPos->getBasketTableId());
				activeFromBasketPos->setBasketTableId(0);
			}
		}
	}
	if (isCarryingBasket && !wasCarryingBasket)
	{
		LOG("BASKET CACHE", QString("PLC carrying flag enabled; refreshing crane basket %1")
			.arg(myBasketPos ? myBasketPos->getBasketTableId() : 0));
		recalculateCarriedBasketContentsFromCache();
	}

	if (statusBits[10])
		plcModeEnum = MAINTENANCE_MODE;

	if (statusBits[12])
		plcModeEnum = STEP_MODE;

	if (statusBits[11])
		plcModeEnum = AUTO_MODE;
}

void craneClass::recalculateCarriedBasketContentsFromCache()
{
	if (!myBasketPos || !myBasketPos->hasBasket())
	{
		LOG("BASKET CACHE", "Crane basket refresh skipped: crane is not holding a basket");
		return;
	}

	const int basketId = myBasketPos->getBasketTableId();
	myBasketPos->calculateBasketContentsTypeFromEpicsDatabase();
	LOG("BASKET CACHE", QString("Crane basket %1 refreshed: contentsEnum=%2")
		.arg(basketId).arg(myBasketPos->getContentsEnum()));
}

void craneClass::synchronizeObserverMissionFromPlc()
{
	if (!isDatabaseReadOnlyMode())
		return;

	if (myStatusInPlc == 1 && myFromPosInPlc > 0 && myToPosInPlc > 0)
	{
		basketPositionClass* from = allPositions ? allPositions->getPositionByPlcId(myFromPosInPlc) : NULL;
		basketPositionClass* to = allPositions ? allPositions->getPositionByPlcId(myToPosInPlc) : NULL;
		if (!from || !to)
		{
			LOG("WARNING: BASKET CACHE", QString("Observer could not resolve PLC mission from=%1 to=%2")
				.arg(myFromPosInPlc).arg(myToPosInPlc));
			return;
		}

		if (activeFromBasketPos != from || activeToBasketPos != to)
		{
			clearActivePositions();
			setActivePositions(from, to);
			LOG("BASKET CACHE", QString("Observer mission synchronized: from=%1 to=%2")
				.arg(myFromPosInPlc).arg(myToPosInPlc));
		}
		hasActiveMission = true;

		if (isCarryingBasket && myBasketPos)
		{
			int basketId = myBasketPos->getBasketTableId();
			if (basketId <= 0 && from->hasBasket())
				basketId = from->getBasketTableId();
			if (basketId <= 0)
				basketId = GETONE("select basketTableId from positions where posNumber=1 and posIndex=1").toInt();
			if (basketId > 0 && !myBasketPos->hasBasket())
			{
				myBasketPos->setBasketTableIdInMemory(basketId);
				LOG("BASKET CACHE", QString("Observer attached basket %1 to crane from PLC position %2")
					.arg(basketId).arg(myFromPosInPlc));
			}
			// Keep the source assignment visible while carrying. The legacy display
			// draws the real source basket beneath its translucent mission marker;
			// clearing it here leaves only the green overlay in observer mode.
			// Continue hiding an early destination assignment until release.
			if (to->hasBasket())
			{
				LOG("BASKET CACHE", QString("Observer hid destination basket %1 at PLC position %2 while crane is carrying")
					.arg(to->getBasketTableId()).arg(myToPosInPlc));
				to->setBasketTableIdInMemory(0);
			}
		}
		else if (!isCarryingBasket && myBasketPos && myBasketPos->hasBasket())
		{
			LOG("BASKET CACHE", QString("Observer released crane basket %1 during PLC mission")
				.arg(myBasketPos->getBasketTableId()));
			myBasketPos->setBasketTableIdInMemory(0);
		}
		return;
	}

	if (!isCarryingBasket)
	{
		if (myBasketPos && myBasketPos->hasBasket())
		{
			LOG("BASKET CACHE", QString("Observer cleared crane basket %1 after PLC mission")
				.arg(myBasketPos->getBasketTableId()));
			myBasketPos->setBasketTableIdInMemory(0);
		}
		clearActivePositions();
		hasActiveMission = false;
	}
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
	if (!hasActiveMission) return false;
	//if (activeToBasketPos && activeToBasketPos->getPlcId()==401)//if target is before stacker i don't close the mission till i see that he is not asking for empty basket, that he took the basket
	//{
	//	if (allPositions->isPosBeforeStacketEmpty())
	//		return false;
	//}
	int basketTableId = myBasketPos->getBasketTableId() > 0 ? myBasketPos->getBasketTableId() : activeFromBasketPos->getBasketTableId();

#ifdef EMULATE
	setPlcNetworkActive(PLC_NETWORK_CRANE, false);
	if (activeToBasketPos->getNumber() == 400)
		cranePlcTcpServer->sendString(QString("401|0|%1").arg(basketTableId));
	if (activeFromBasketPos->getNumber() == 410)
		cranePlcTcpServer->sendString(QString("411|0|%1").arg(0));
	if (activeToBasketPos->getNumber() == 450)
		cranePlcTcpServer->sendString(QString("451|0|%1").arg(basketTableId));
	if (activeFromBasketPos->getNumber() == 460)
		cranePlcTcpServer->sendString(QString("461|0|%1").arg(0));

	int basketNoToPlc = basketTableId;
	if ((activeFromBasketPos->hasBasket()) && (activeFromBasketPos->getTypeEnum() != BASKET_NORMAL))
		basketNoToPlc = -999;
	else if ((myBasketPos->hasBasket()) && (myBasketPos->getTypeEnum() != BASKET_NORMAL))
		basketNoToPlc = -999;

	if (activeToBasketPos->getNumber() == 500)
		cranePlcTcpServer->sendString(QString("N20|20|%1").arg(basketNoToPlc));
	if (activeFromBasketPos->getNumber() == 500)
		cranePlcTcpServer->sendString(QString("N20|20|%1").arg(0));
	if (activeToBasketPos->getNumber() == 510)
		cranePlcTcpServer->sendString(QString("N20|19|%1").arg(basketNoToPlc));
	if (activeToBasketPos->getNumber() == 520)
		cranePlcTcpServer->sendString(QString("N20|18|%1").arg(basketNoToPlc));

	cranePlcTcpServer->sendString(QString("N20|0|0"));
	cranePlcTcpServer->sendString(QString("N20|1|0"));
	cranePlcTcpServer->sendString(QString("N20|2|0"));
	cranePlcTcpServer->sendString(QString("N20|4|0"));
	setPlcNetworkIdle(PLC_NETWORK_CRANE);
#endif

	if (activeFromBasketPos->getNumber() == 410)
	{
		emit missionFromAfterStackerPositionFinishedSignal();
	}

	if (activeFromBasketPos->getNumber() == 420)
	{
		emit scrapBasketPositionChangedSignal(0);
		allPositions->setScrapBasketIsReady(false);
	}

	// Stacker 2 has own conveyor mempry location (N11[39])
	//if (activeToBasketPos->getNumber() == 460)
	//{
	//	emit missionFromAfterStacker2PositionFinishedSignal();
	//}

	if (myBasketPos->getBasketTableId() != 0) {
		activeToBasketPos->setBasketTableId(myBasketPos->getBasketTableId());
		myBasketPos->setBasketTableId(0);
	}
	else if (activeFromBasketPos->getBasketTableId() != 0) {
		activeToBasketPos->setBasketTableId(activeFromBasketPos->getBasketTableId());
		activeFromBasketPos->setBasketTableId(0);
	}

	///notify conveyors plc
	if ((activeToBasketPos->getIndex() == 1) &&
		((activeToBasketPos->getNumber() == 160) || (activeToBasketPos->getNumber() == 100) || (activeToBasketPos->getNumber() == 110) || (activeToBasketPos->getNumber() == 120) ||
			(activeToBasketPos->getNumber() == 130) || (activeToBasketPos->getNumber() == 140) || (activeToBasketPos->getNumber() == 150) ||
			(activeToBasketPos->getNumber() == 400) || // DMS basket-ID handoff; PLC owns clearing N12[5].
			(activeToBasketPos->getNumber() == 450) || (activeToBasketPos->getNumber() == 220) || (activeToBasketPos->getNumber() == 280) || (activeToBasketPos->getNumber() == 290))
		) //theseis sto fourno h katw thesh -- places in the oven lower position
		allPositions->changeConveyorsBasketMemory(activeToBasketPos->getPlcId(), basketTableId);

	if ((activeFromBasketPos->getIndex() == 1) &&
		((activeFromBasketPos->getNumber() == 160) || (activeFromBasketPos->getNumber() == 100) || (activeFromBasketPos->getNumber() == 110) || (activeFromBasketPos->getNumber() == 120) ||
			(activeFromBasketPos->getNumber() == 130) || (activeFromBasketPos->getNumber() == 140) || (activeFromBasketPos->getNumber() == 150) ||
			(activeFromBasketPos->getNumber() == 450) || (activeFromBasketPos->getNumber() == 460) ||
			(activeFromBasketPos->getNumber() == 220) || (activeFromBasketPos->getNumber() == 280) || (activeFromBasketPos->getNumber() == 290))
		) //theseis sto fourno h katw thesh
		allPositions->changeConveyorsBasketMemory(activeFromBasketPos->getPlcId(), 0);

	//clear from exports
	if (activeToBasketPos->getNumber() == 520) {
		setDbTableActive("toAssimakisExports", false);
		execDBQuery(QString("delete from toAssimakisExports where basketTableId=%1").arg(basketTableId), MAIN_DB_CONNECTION);
		setDbTableIdle("toAssimakisExports");
	}
	if (activeToBasketPos->getNumber() == 510) {
		setDbTableActive("toOmavExports", false);
		execDBQuery(QString("delete from toOmavExports where basketTableId=%1").arg(basketTableId), MAIN_DB_CONNECTION);
		setDbTableIdle("toOmavExports");
	}

	//lock the position if it is the in-out not to take it in auto mode, it unlockes when new basket is comming from out
	//if (activeToBasketPos->getNumber() == 500)
	//	activeToBasketPos->setLocked(true);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	setDbTableActive("positions", false);
	execDBQuery(QString("update positions set targetNumber = 0, targetIndex = 0 where posNumber = %1 and posIndex = %2").arg(activeFromBasketPos->getNumber()).arg(activeFromBasketPos->getIndex()), MAIN_DB_CONNECTION);
	setDbTableIdle("positions");
	hasActiveMission = false;

	//check that is clear in database
	int fromNumber = GETONEATMOST(QString("select posNumber from positions where targetNumber <> 0")).toInt();
	int fromIndex = GETONEATMOST(QString("select posIndex from positions where targetIndex <> 0")).toInt();
	if ((fromNumber == 0) && (fromIndex == 0)) {
		setPlcNetworkActive(PLC_NETWORK_CRANE, false);
		cranePlcTcpServer->sendString(QString("N20|2|2"));
		setPlcNetworkIdle(PLC_NETWORK_CRANE);
	}

	////////////////////////
	QString stmnt = QString("insert into basketHistory (type,position,destination,basketTableId,date,sec) VALUES('%1','%2','%3',%4,'%5','%6')")
			.arg("end").arg((activeFromBasketPos->getDescription().trimmed() + " %1 %2").arg(activeFromBasketPos->getNumber()).arg(activeFromBasketPos->getIndex()))
			.arg((activeToBasketPos->getDescription().trimmed() + " %1 %2").arg(activeToBasketPos->getNumber()).arg(activeToBasketPos->getIndex())).arg(QString().number(activeToBasketPos->getBasketTableId()))
			.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t());
	setDbTableActive("basketHistory", false);
	execDBQuery(stmnt, MAIN_DB_CONNECTION);
	setDbTableIdle("basketHistory");

	////////////////////////
	clearActivePositions();
	emit openGlChangedSignal();

#ifdef EMULATE
	setPlcNetworkActive(PLC_NETWORK_CRANE, false);
	// cranePlcTcpServer->sendString(QString("N20|20|%1").arg(allPositions->getPosition(500,0)->getBasketNumber()));
	cranePlcTcpServer->sendString(QString("401|0|%1").arg(allPositions->getPosition(400,0)->getBasketNumber()));
	cranePlcTcpServer->sendString(QString("411|0|%1").arg(allPositions->getPosition(410,0)->getBasketNumber()));
	cranePlcTcpServer->sendString(QString("451|0|%1").arg(allPositions->getPosition(450, 0)->getBasketNumber()));
	cranePlcTcpServer->sendString(QString("461|0|%1").arg(allPositions->getPosition(460, 0)->getBasketNumber()));
	setPlcNetworkIdle(PLC_NETWORK_CRANE);
#endif

	return true;
}

bool craneClass::abortMission()
{
	if (!hasActiveMission)
		return false;

	QString fromPosStr = activeFromBasketPos->getDescription().trimmed() + QString("%1 %2").arg(activeFromBasketPos->getNumber()).arg(activeFromBasketPos->getIndex());
	QString toPosStr = activeToBasketPos->getDescription().trimmed() + QString("%1 %2").arg(activeToBasketPos->getNumber()).arg(activeToBasketPos->getIndex());
	int basketTableId = myBasketPos->getBasketTableId() > 0 ? myBasketPos->getBasketTableId() : activeFromBasketPos->getBasketTableId();

	//int lengthType=BASKET_CONTENTS_NONE;
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
	//	activeFromBasketPos->setLocked(true);
	//

	activeFromBasketPos->setLocked(true);

	if (myBasketPos->getBasketTableId() != 0)
	{
		activeFromBasketPos->setBasketTableId(myBasketPos->getBasketTableId());
		myBasketPos->setBasketTableId(0);
	}
	else if (activeFromBasketPos->getBasketTableId() != 0)
	{
		//nothing
	}

	hasActiveMission = false;
	setDbTableActive("positions", false);
	execDBQuery(QString("update positions set targetNumber = 0, targetIndex = 0 where posNumber = %1 and posIndex = %2").arg(activeFromBasketPos->getNumber()).arg(activeFromBasketPos->getIndex()), MAIN_DB_CONNECTION);
	setDbTableIdle("positions");
	setPlcNetworkActive(PLC_NETWORK_CRANE, false);
	cranePlcTcpServer->sendString(QString("N20|2|3"));
	setPlcNetworkIdle(PLC_NETWORK_CRANE);
	clearActivePositions();
	emit openGlChangedSignal();

	QString stmnt = QString("insert into basketHistory (type,position,destination,basketTableId,date,sec) VALUES('lock','%1','',%2,'%3','%4')").arg(fromPosStr).arg(basketTableId).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t());
	setDbTableActive("basketHistory", false);
	execDBQuery(stmnt, MAIN_DB_CONNECTION);
	setDbTableIdle("basketHistory");

	stmnt = QString("insert into basketHistory (type,position,destination,basketTableId,date,sec) VALUES('abort','%1','%2',%3,'%4','%5')").arg(fromPosStr).arg(toPosStr).arg(basketTableId).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd") + QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t());
	setDbTableActive("basketHistory", false);
	execDBQuery(stmnt, MAIN_DB_CONNECTION);
	setDbTableIdle("basketHistory");

	return true;
}

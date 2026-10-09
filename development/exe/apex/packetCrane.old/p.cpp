#include "p.h"
extern GLfloat	*quadVertexesForCassetteArray;
extern GLfloat	*quadNormalsForCassetteArray;
extern GLfloat	*quadTexCoordForCassetteArray;
extern int numberOfQuadForCassette;

extern GLfloat	*quadVertexesForPosSelectionArray;
extern GLfloat	*quadNormalsForPosSelectionArray;
extern GLfloat	*quadTexCoordForPosSelectionArray;
extern int numberOfQuadForPosSelection;

extern GLfloat	*quadVertexesForCenteredPacketArray;
extern GLfloat	*quadNormalsForCenteredPacketArray;
extern GLfloat	*quadTexCoordForCenteredPacketArray;
extern int numberOfQuadForCenteredPacket;

extern GLfloat	*quadVertexesForLeftPacketArray;
extern GLfloat	*quadNormalsForLeftPacketArray;
extern GLfloat	*quadTexCoordForLeftPacketArray;
extern int numberOfQuadForLeftPacket;

extern GLfloat	*quadVertexesForRightPacketArray;
extern GLfloat	*quadNormalsForRightPacketArray;
extern GLfloat	*quadTexCoordForRightPacketArray;
extern int numberOfQuadForRightPacket;

extern GLfloat	*quadVertexesFor500x500PlateArray;
extern GLfloat	*quadNormalsFor500x500PlateArray;
extern GLfloat	*quadTexCoordFor500x500PlateArray;
extern int numberOfQuadFor500x500Plate;
packetPositionClass::packetPositionClass(int posId_,GLfloat x,GLfloat y,GLfloat z,QWidget *p):QWidget(p)
{
	posId=posId_;
	packetId=packetNoOfWoods=packetLength=packetWidth=packetHeight=packetDestination=0;
	storagingTypeOfPacket=STORAGE_TYPE_SHORT_TERM;
	isOnSb=false;
	pickingColor=getColorForPicking();
	isMouseOver=false;

	openGlX=x;
	openGlY=y;
	openGlZ=z;
}
packetPositionClass::~packetPositionClass()
{
}
void packetPositionClass::setOpenGlX(GLfloat v)
{
	openGlX=v;
}
void packetPositionClass::setOpenGlY(GLfloat v)
{
	openGlY=v;
}
void packetPositionClass::setOpenGlZ(GLfloat v)
{
	openGlZ=v;
}
/////////////////////////////////////////////////////////////////////////////////////
int packetPositionClass::getNumberOfWoods()
{
	return packetNoOfWoods;
}
int packetPositionClass::getPosId()
{
	return posId;
}
bool packetPositionClass::getIsOnSB()
{
	return isOnSb;
}
void packetPositionClass::setPacketId(int p,bool check)
{
	vLog("info","packetPositionClass::setPacketId()",QString("id:%1,packet:%2").arg(posId).arg(p));
	apexExecQuery(QString("insert into log (type,posId,newValue,oldValue,date,sec) values('define packet',%1,%2,%3,'%4',%5)").arg(posId).arg(p).arg(packetId).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	if (check&&(p!=0)&&(packetId!=0)&&(p!=packetId))
	{
		vLog("Error","packetPositionClass::setPacketId()",QString("Attempt to define packetId %1 in position %2. The position allready has packetId %3").arg(p).arg(posId).arg(packetId));
		EXIT(tr("Error"),tr("Attempt to define packetId %1 in position %2. The position allready has packetId %3").arg(p).arg(posId).arg(packetId));
	}
	packetId=p;
	saveToDatabase();
	loadContentsFromDatabase();
}
int packetPositionClass::getPacketTypeForPlc()
{
	if (isOnSb)
		return 9;
	else
	{
		if ((packetLength<2743)&&(posId!=POS_ID_SB_PACKET_IMPORT))
			return 1;
		else
			return packetNoOfWoods;
	}
}
int packetPositionClass::getPositionIdForForkPlc()
{
	if (posId==POS_ID_NEWSB)return 6;
	if (posId==POS_ID_NEWPACKET)return 8;
	if (posId==POS_ID_SB_PACKET_IMPORT)return 9;
	if (posId==POS_ID_WESTCONV)return 24;
	if (posId==POS_ID_EASTCONV)return 25;
	if (posId==POS_ID_SBEXIT)return 21;
	if (posId==POS_ID_PACKETEXIT)return 22;
	if (posId==POS_ID_STOREEXIT)return 23;
	return 0;
}
int packetPositionClass::getPositionIdForCranePlc()
{
	return posId;
}
int packetPositionClass::getPacketId()
{
	return packetId;
}
int packetPositionClass::getPacketDestination()
{
	return packetDestination;
}
int packetPositionClass::getStoragingTypeOfPacket()
{
	return storagingTypeOfPacket;
}
int packetPositionClass::getPacketPlacingType()
{
	if ((packetNoOfWoods==2)||(packetNoOfWoods==4))
		return LEFT_PLACED_PACKET;
	if ((packetNoOfWoods==3)||(packetNoOfWoods==5))
		return CENTER_PLACED_PACKET;
	return -1;
}
bool packetPositionClass::hasPacket()
{
	if (packetId!=0)
		return true;
	else
		return false;
}
void packetPositionClass::saveToDatabase()
{
	apexExecQuery(QString("update positions set packetId=%1 where posId=%2").arg(packetId).arg(posId));
}
void packetPositionClass::loadContentsFromDatabase()
{
	if (packetId!=0)
	{
		QVector<QVector<QVariant>> dd=execTableQuery(QString("select woods,length,width,height,sb,destination,storagingType from packets where packetId=%1").arg(packetId),MAIN_DB_CONNECTION);
		if (dd.count()==1)
		{
			packetNoOfWoods=dd[0][0].toInt();
			packetLength=dd[0][1].toInt();
			packetWidth=dd[0][2].toInt();
			packetHeight=dd[0][3].toInt();
			isOnSb = (dd[0][4].toInt() == PACKET_WITH_SB);
			packetDestination=dd[0][5].toInt();
			storagingTypeOfPacket=dd[0][6].toInt();
		}
		else
			uExit("Error",QString("packet %1 is not defined in packets table").arg(packetId));
	}
	else
	{
		packetNoOfWoods=packetLength=0;
	}
	emit openGlChangedSignal();
}
void packetPositionClass::loadFromDatabase()
{
	packetId=GETONEATMOST(QString("select packetId from positions where posId=%1").arg(posId)).toInt();
	loadContentsFromDatabase();
	emit openGlChangedSignal();
}
/////////////////////////////////////////////////////////////////////////////////////////////
#include "uglyfont.h"
void packetPositionClass::draw(QColor color,bool drawCassette)
{
	if (isMouseOver)
		color=QColor(0,255,0);
	//draw cassette
	glPushMatrix();
	glTranslatef(openGlX,openGlY,openGlZ);
	if (drawCassette)
	{
		if (color!=QColor(0,0,0))
		{
			glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
			glColor3ub(color.red(),color.green(),color.blue());
		}
		glBindTexture(GL_TEXTURE_2D,CASSETTE_TEXTURE);
		glVertexPointer(3,GL_FLOAT,0,&quadVertexesForCassetteArray[0]);
		glNormalPointer(GL_FLOAT,0,&quadNormalsForCassetteArray[0]);
		glTexCoordPointer(2,GL_FLOAT,0,&quadTexCoordForCassetteArray[0]);
		glDrawArrays(GL_QUADS,0,numberOfQuadForCassette*4);
	}
	if (isMouseOver)
	{
		glPushMatrix();
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		glBindTexture(GL_TEXTURE_2D,0);

		glColor3ub(255,255,255);
		glTranslatef(0.f,-3.900f,0.f);
		glVertexPointer(3,GL_FLOAT,0,&quadVertexesFor500x500PlateArray[0]);
		glNormalPointer(GL_FLOAT,0,&quadNormalsFor500x500PlateArray[0]);
		glDrawArrays(GL_QUADS,0,numberOfQuadFor500x500Plate*4);

		glColor3ub(0,0,0);
		glPushMatrix();
		glRotatef(90.,1.0f,0.0f,0.0f);
		glTranslatef(-0.25f,0.4f,+0.01f);
		glScalef(0.1f,0.1f,0.1f);
		YsDrawUglyFont(QString().number(posId).toLatin1(),0);
		glPopMatrix();

		glColor3ub(255,255,255);
		glTranslatef(0.f,+7.800f,0.f);
		glVertexPointer(3,GL_FLOAT,0,&quadVertexesFor500x500PlateArray[0]);
		glNormalPointer(GL_FLOAT,0,&quadNormalsFor500x500PlateArray[0]);
		glDrawArrays(GL_QUADS,0,numberOfQuadFor500x500Plate*4);

		glColor3ub(0,0,0);
		glPushMatrix();
		glRotatef(90.,1.0f,0.0f,0.0f);
		glRotatef(180.,0.0f,1.0f,0.0f);
		glTranslatef(-0.2f,+0.4f,+0.01f);
		glScalef(0.1f,0.1f,0.1f);
		YsDrawUglyFont(QString().number(posId).toLatin1(),0);
		glPopMatrix();

		glPopAttrib();
		glPopMatrix();
	}
	if (packetId!=0)
	{
		if (isOnSb)
		{
			glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
			glColor3ub(0,0,200);
		}
		if (packetNoOfWoods!=4)
		{
			if (isOnSb&&((packetWidth<10)||(packetLength<10)||(packetHeight<10)))
				glScalef(500/1000.,6000/1000.,500/1000.);
			else
				glScalef(packetWidth/1000.,packetLength/1000.,packetHeight/1000.);
			glBindTexture(GL_TEXTURE_2D,PACKET_TEXTURE);
			glVertexPointer(3,GL_FLOAT,0,&quadVertexesForCenteredPacketArray[0]);
			glNormalPointer(GL_FLOAT,0,&quadNormalsForCenteredPacketArray[0]);
			glTexCoordPointer(2,GL_FLOAT,0,&quadTexCoordForCenteredPacketArray[0]);
			glDrawArrays(GL_QUADS,0,numberOfQuadForCenteredPacket*4);
		}
		else
		{
			if (posId<0)
			{
				glTranslatef(0.f,-0.762f,0.f);
				glScalef(packetWidth/1000.,packetLength/1000.,packetHeight/1000.);
				glBindTexture(GL_TEXTURE_2D,PACKET_TEXTURE);
				glVertexPointer(3,GL_FLOAT,0,&quadVertexesForCenteredPacketArray[0]);
				glNormalPointer(GL_FLOAT,0,&quadNormalsForCenteredPacketArray[0]);
				glTexCoordPointer(2,GL_FLOAT,0,&quadTexCoordForCenteredPacketArray[0]);
				glDrawArrays(GL_QUADS,0,numberOfQuadForCenteredPacket*4);
			}
			else
			{
				glTranslatef(0.f,+0.762f,0.f);
				glScalef(packetWidth/1000.,packetLength/1000.,packetHeight/1000.);
				glBindTexture(GL_TEXTURE_2D,PACKET_TEXTURE);
				glVertexPointer(3,GL_FLOAT,0,&quadVertexesForCenteredPacketArray[0]);
				glNormalPointer(GL_FLOAT,0,&quadNormalsForCenteredPacketArray[0]);
				glTexCoordPointer(2,GL_FLOAT,0,&quadTexCoordForCenteredPacketArray[0]);
				glDrawArrays(GL_QUADS,0,numberOfQuadForCenteredPacket*4);
			}
		}
		if (isOnSb)
			glPopAttrib();
	}
	if (color!=QColor(0,0,0))
		glPopAttrib();
	glPopMatrix();
}
void packetPositionClass::drawForPicking()
{
	glPushMatrix();
	glTranslatef(openGlX,openGlY,openGlZ);
	glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
	glColor3ub(pickingColor.red(),pickingColor.green(),pickingColor.blue());

	glVertexPointer(3,GL_FLOAT,0,&quadVertexesForPosSelectionArray[0]);
	glNormalPointer(GL_FLOAT,0,&quadNormalsForPosSelectionArray[0]);
	glTexCoordPointer(2,GL_FLOAT,0,&quadTexCoordForPosSelectionArray[0]);
	glDrawArrays(GL_QUADS,0,numberOfQuadForPosSelection*4);
	glPopAttrib();
	glPopMatrix();
}
void packetPositionClass::mouseOver(QColor pickedColor)
{
	if (pickedColor==pickingColor)
		isMouseOver=true;
	else
		isMouseOver=false;
}

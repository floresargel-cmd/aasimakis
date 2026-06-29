#include "b.h"
#include "dbTableStatus.h"

#include "../common/applogger.h" // @@@@@ -- logger LOG() function is now in its own file, so including that instead of apexG.h to avoid unnecessary dependencies. 
#include "../common/execDBQuery.h"
						      //		  Should be included in tcpTux.h when the logger is moved there.

basketClass::basketClass():QObject()
{
	if (this == nullptr) {
		// @@@@@ -- Adding a null pointer check for 'this' in the constructor. This should never happen, but if it does, we log an error message and return without initializing the object to avoid undefined behavior.
		QString mess = "ERROR: basketClass::basketClass() called, but 'this' is null. This should never happen. Exiting without initializing the object.";
		LOG("b.cpp", mess);
		// EXIT("nullprt", mess);
	}
	tableId = number = 0;
	areContentsAged = false;
	temperEnum = BASKET_TEMPER_NULL;
	maxLengthOfContents = 0.;
}

basketClass::~basketClass()
{
}

int basketClass::getTableId()
{
	// LOG("b.cpp",  "basketClass::getTableId() called, returning " + QString::number(tableId));
	return tableId;
}

int basketClass::getNumber()
{
	// LOG("b.cpp", "basketClass::getNumber() called, returning " + QString::number(number));
	return number;
}
int basketClass::getTypeEnum()
{
	// LOG("b.cpp", "basketClass::getTypeEnum() called, returning " + QString::number(typeEnum));
	return typeEnum;
}
int basketClass::getContentsEnum()
{
	// LOG("b.cpp", "basketClass::getContentsEnum() called, returning " + QString::number(contentsEnum));
	return contentsEnum;
}

int basketClass::getTemperEnum()
{
	// LOG("b.cpp", "basketClass::getTemperEnum() called, returning " + QString::number(temperEnum));
	return temperEnum;
}

bool basketClass::hasContents()
{
	return contentsEnum == BASKET_CONTENS_NONE ? false : true;
}

bool basketClass::getAreContentsAged()
{
	return areContentsAged;
}

QString basketClass::getToolTipText()
{
	return toolTipText;
}

void basketClass::calculateBasketContentsTypeFromEpicsDatabase()
{
	toolTipText.clear();

	if (tableId != 0)
	{
		toolTipText += QString("Basket number: %1\n").arg(tableId);
		contentsEnum = BASKET_CONTENS_NONE;
		temperEnum = BASKET_TEMPER_NULL;
		areContentsAged = false;

		setDbTableActive("basket_details", true);
		QVector<QVariant> profilesArray = execDBQuery(QString("select dienum from basket_details where basketnum=%1 and Pc<>0").arg(tableId), APEX_DB_CONNECTION);
		QVector<QVariant> noOfProfilesArray = execDBQuery(QString("select Pc from basket_details where basketnum=%1 and Pc<>0").arg(tableId), APEX_DB_CONNECTION);
		QVector<QVariant> lengthArray = execDBQuery(QString("select PcLen from basket_details where basketnum=%1 and Pc<>0").arg(tableId), APEX_DB_CONNECTION);
		QVector<QVariant> temperArray = execDBQuery(QString("select temper from basket_details where basketnum=%1 and Pc<>0").arg(tableId), APEX_DB_CONNECTION);
		setDbTableIdle("basket_details");

		maxLengthOfContents=0.;

		for (int i = 0; i < lengthArray.count(); i++)
		{
			double tLength = lengthArray[i].toDouble();
			bool doubleBatched = false;

			if (tLength < 3601)
				doubleBatched = true;

			if (doubleBatched)
				tLength = 2 * tLength + 200.;

			// @@@@@ -- Last call before an access violation on the QVector stuff... AB
			QString pa = "--";
			QString npa = "--";
			QString tpa = "--";

			LOG("b.cpp", "WARNING: Size of loop is based on lengthArray.count(): " + QString::number(lengthArray.count()));
			if (i > profilesArray.count()) {
				LOG("b.cpp", "ERROR: Size of i INDEX exceeds profilesArray count (this will cause an array out of bounds if used): " + QString::number(profilesArray.count()));
			}
			else if (profilesArray.size() == 0) {
				LOG("b.cpp", "ERROR: profilesArray[" + QString::number(i) + "] is 0 Size, therefore any iteration based on i INDEX will exceed the array");
			}
			else if (profilesArray.isEmpty()) {
				LOG("b.cpp", "ERROR: profilesArray is emptytherefore any iteration based on i INDEX will exceed the array");
			}
			else {
				// Assumed good...
				try {
					pa = (profilesArray[i].isValid() && !profilesArray[i].toString().isEmpty()) ? profilesArray[i].toString() : "N/A"; // @@@@@ - QVector ASSERT Failure
				}
				catch (...) {
					LOG("b.cpp", "ERROR: Final Destination - something bad happened in PA b.cpp.");
					if (pa == "--") {
						LOG("b.cpp", "ERROR: pa was never set");
						pa = "N/A";
					}
				}
			}


			if (i > noOfProfilesArray.count()) {
				LOG("b.cpp", "ERROR: Size of i INDEX exceeds noOfProfilesArray vector (this will cause array out of bounds if i INDEX is used): " + QString::number(noOfProfilesArray.count()));
			} else if (noOfProfilesArray.size() == 0) {
				LOG("b.cpp", "ERROR: noOfProfilesArray[" + QString::number(i) + "] is 0 Size, therefore any iteration based on i INDEX will exceed the array");
			}
			else if (noOfProfilesArray.isEmpty()) {
				LOG("b.cpp", "ERROR: profilesArray is empty, therefore any iteration based on i INDEX will exceed the array");
			}
			else {
				// Assumed good...
				try {
					npa = (noOfProfilesArray[i].isValid() && !noOfProfilesArray[i].toString().isEmpty()) ? noOfProfilesArray[i].toString() : "N/A";
				}
				catch (...) {
					LOG("b.cpp", "ERROR: Final Destination - something bad happened in NPA b.cpp.");
					if (npa == "--") {
						LOG("b.cpp", "ERROR: npa was never set");
						npa = "N/A";
					}
				}
			}

			if (i > temperArray.count()) {
				LOG("b.cpp", "ERROR: Size of temperArray exceeds vector (this will cause array out of bounds if i INDEX is used): " + QString::number(temperArray.count()));
			}
			else if (temperArray.size() == 0) {
				LOG("b.cpp", "ERROR: temperArray[" + QString::number(i) + "] is 0 Size, therefore any iteration based on i INDEX will exceed the array");
			}
			else if (temperArray.isEmpty()) {
				LOG("b.cpp", "ERROR: profilesArray is empty, therefore any iteration based on i INDEX will exceed the array");
			}
			else {
				// Assumed good...
				try {
					tpa = (temperArray[i].isValid() && !temperArray[i].toString().isEmpty()) ? temperArray[i].toString() : "N/A"; // @@@@@ - QVector ASSERT Failure here
				}
				catch (...) {
					LOG("b.cpp", "ERROR: Final Destination - something bad happened in TPA b.cpp.");
					if (tpa == "--") {
						LOG("b.cpp", "ERROR: tpa was never set");
						tpa = "N/A";
					}
				}
			}
			
			toolTipText += QString("die:%1, %2 profiles with length %3mm%4, temper:%5\n").arg(pa).arg(npa).arg(tLength).arg((doubleBatched ? " (double)" : "")).arg(tpa);
			LOG("b.cpp", "INFO: ToolTip: [" + toolTipText +"]");

			
			if (tLength > maxLengthOfContents)
				maxLengthOfContents = tLength;
		}

		contentsEnum = BASKET_CONTENS_NONE;
		
		if (lengthArray.count() > 0)
		{
			if (maxLengthOfContents <= 6097)
				contentsEnum = BASKET_CONTENS_LESS_THAN_20;

			if ((maxLengthOfContents > 6097) && (maxLengthOfContents <= 7366))
				contentsEnum = BASKET_CONTENS_LESS_THAN_24;

			if ((maxLengthOfContents > 7366) && (maxLengthOfContents <= 9170))
				contentsEnum = BASKET_CONTENS_LESS_THAN_30;

			if (maxLengthOfContents > 9170)
				contentsEnum = BASKET_CONTENS_MORE_THAN_30;
		}
			//if ((lengthArray[i].toDouble()>0)&&(contentsEnum<BASKET_CONTENTS_LESS_THAN_20))
			//{
			//	contentsEnum=BASKET_CONTENTS_LESS_THAN_20;
			//	maxLengthOfContents=lengthArray[i].toDouble();
			//}
			//if ((lengthArray[i].toDouble()>6096)&&(contentsEnum<BASKET_CONTENTS_LESS_THAN_24))
			//{
			//	contentsEnum=BASKET_CONTENTS_LESS_THAN_24;
			//	maxLengthOfContents=lengthArray[i].toDouble();
			//}
			//if ((lengthArray[i].toDouble()>(7366))&&(contentsEnum<BASKET_CONTENTS_LESS_THAN_30))//24 feet +2*25.4 mm
			//{
			//	contentsEnum=BASKET_CONTENTS_LESS_THAN_30;
			//	maxLengthOfContents=lengthArray[i].toDouble();
			//}
			//if ((lengthArray[i].toDouble()>(9170))&&(contentsEnum<BASKET_CONTENTS_MORE_THAN_30))//30 feet +25.4 mm
			//{
			//	contentsEnum=BASKET_CONTENTS_MORE_THAN_30;
			//	maxLengthOfContents=lengthArray[i].toDouble();
			//}

		if (contentsEnum == BASKET_CONTENS_LESS_THAN_20)
			toolTipText += tr("Contents length less than 20 feet\n");
		else if (contentsEnum == BASKET_CONTENS_LESS_THAN_24)
			toolTipText += tr("Contents length less than 24 feet\n");
		else if (contentsEnum == BASKET_CONTENS_LESS_THAN_30)
			toolTipText += tr("Contents length more than 24 feet\n");

		QVariant time = GETFIRST(QString("select audittime from basket_location where basketnum = %1 and basketlocation = %2").arg(tableId).arg(3), APEX_DB_CONNECTION);
#ifdef EMULATE
		if ((!time.isNull()) && (time.toString().length() > 1))
#else
		if (!time.isNull())
#endif		
		{
			toolTipText += QString("Aged on: ") + time.toDateTime().toString("yyyy/MMMM/dd hh:mm") + QString("\n");
			areContentsAged = true;
		}
		else
		{
			toolTipText += QString("Not aged\n");
			areContentsAged = false;
		}

#ifdef EMULATE
		setDbTableActive("basket_details", true);
		QVector<QVariant> temperStrV=execQuery(QString("select temper from basket_details where basketnum = %1 and temper <> ''").arg(tableId),APEX_DB_CONNECTION);
		setDbTableIdle("basket_details");
#else
		setDbTableActive("basket_details", true);
		QVector<QVariant> temperStrV= execDBQuery(QString("select temper from basket_details where basketnum = %1 and temper <> ''").arg(tableId), APEX_DB_CONNECTION);
		setDbTableIdle("basket_details");
#endif

		if (temperStrV.count() == 0)
			temperEnum = BASKET_TEMPER_NULL;
		else
		{
			temperEnum = BASKET_TEMPER_230;
			QString validTemper;
			for (int i = 0; i < temperStrV.count(); i++)
			{
				QString qstr = QString("select temperType from temperTypes where temper='%1'").arg(temperStrV[i].toString());
				qstr.toStdString();
				int tTemper = GETFIRST(QString("select temperType from temperTypes where temper='%1'").arg(temperStrV[i].toString())).toInt();
				if (temperEnum>tTemper)
				{
					temperEnum = tTemper;
					validTemper = temperStrV[i].toString();
				}
			}
			toolTipText += QString("Temper into account:%1\n").arg(validTemper);
		}
	}
	else
	{
		contentsEnum = BASKET_CONTENS_NONE;
		temperEnum = BASKET_TEMPER_NULL;
		areContentsAged = false;
	}
}

void basketClass::loadContentFromDatabase()
{
	if (tableId != 0)
	{
		number = GETONE(QString("select number from baskets where pid=%1").arg(tableId)).toInt();
		typeEnum = GETONE(QString("select type from baskets where pid=%1").arg(tableId)).toInt();
	}
	else
	{
		typeEnum = BASKET_NONE;
		contentsEnum = BASKET_CONTENS_NONE;
		tableId = number = 0;
		temperEnum = BASKET_TEMPER_NULL;
	}
}

void basketClass::setTableId(int id_)
{
	tableId = id_;
	loadContentFromDatabase();
}

void basketClass::setTypeEnum(int t)
{
	typeEnum = t;
}

#include "uglyfont.h"

///
void basketClass::drawGeometry(bool drawTexture)
{
	// LOG("b.cpp", "basketClass::drawGeometry() called with drawTexture=" + QString(drawTexture ? "true" : "false") + ", typeEnum=" + QString::number(typeEnum) + ", contentsEnum=" + QString::number(contentsEnum) + ", number=" + QString::number(number) + ", profileIds count=" + QString::number(profileIds.count()));	
	if (typeEnum == BASKET_NORMAL)
	{
		// LOG("b.cpp", "Drawing normal basket geometry for basket number " + QString::number(number));
		drawNormalBasketGeometry(drawTexture);
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		glPushMatrix();
		glScalef(0.075f,0.075f,0.075f);
		glTranslatef(1.0f,-0.25f,0.25f);
		glRotatef(90.,1.0f,0.0f,0.0f);
		glColor3f(0.0f,0.0f,0.75f);

		// LOG("b.cpp", "Drawing basket number " + QString::number(number) + " on front face");
		YsDrawUglyFont(QString().number(number).toLatin1(),0);

		glTranslatef(0.0f,0.f,-20.f);
		glRotatef(180.,0.0f,1.0f,0.0f);

		YsDrawUglyFont(QString().number(number).toLatin1(),0);

		if (contentsEnum == BASKET_CONTENS_LESS_THAN_24) {
			// LOG("b.cpp", "Contents enum is less than 24, translating for second profile ID");	
			glTranslatef(-0.6f, 1.2f, 0.13f);
		}
		else {
			// LOG("b.cpp", "Contents enum is 24 or more, translating for second profile ID");
			glTranslatef(-1.5f, 1.2f, 0.13f);
		}

		glScalef(0.05f,0.05f,0.05f);
		glRotatef(90.,1.0f,0.0f,0.0f);
		glRotatef(-90.,0.0f,1.0f,0.0f);
		glColor3f(0.75f,0.75f,0.75f);

		for (int i = 0; i < profileIds.count(); i++) {
			// LOG("b.cpp", "Drawing profile ID " + QString::number(profileIds[i].toInt()) + " on basket number " + QString::number(number));
			YsDrawUglyFont(QString().number(profileIds[i].toInt()).toLatin1(), 0);
		}

		glPopMatrix();
		glPopAttrib();
	}
	else if (typeEnum == BASKET_SCRAP)
	{
		// LOG("b.cpp", "Drawing scrap basket geometry for basket number " + QString::number(number));
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		//glColor3f(1.0f,1.0f,0.0f);
		drawScrapBasketGeometry(drawTexture);

		glPopAttrib();
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		glPushMatrix();
		glScalef(0.075f,0.075f,0.075f);
		glTranslatef(20.0f,-0.25f,0.25f);
		glRotatef(90.,1.0f,0.0f,0.0f);
		glColor3f(0.0f,0.0f,0.75f);

		// LOG("b.cpp", "Drawing basket number " + QString::number(number) + " on front face of scrap basket");
		YsDrawUglyFont(QString().number(number).toLatin1(),0);

		glPopMatrix();
		glPopAttrib();
	}

	if (hasContents())
	{
		// LOG("b.cpp", "Basket number " + QString::number(number) + " has contents, drawing profiles geometry with max length of contents " + QString::number(maxLengthOfContents) + " mm");
		glPushMatrix();
		float scaleX=maxLengthOfContents/1000.;
		glScalef(scaleX,1.0f,1.0f);
		glTranslatef((3.-0.001*maxLengthOfContents*.5)/scaleX,0.f,0.f);
		//glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);

		drawProfilesGeometry(drawTexture);
		// LOG("b.cpp", "Finished drawing profiles geometry for basket number " + QString::number(number) + " with max length of contents " + QString::number(maxLengthOfContents) + " mm");
		//glPopAttrib();
		//if (contentsEnum==BASKET_CONTENTS_LESS_THAN_24)
		//	drawProfilesGeometry(drawTexture);
		//else if (contentsEnum==BASKET_CONTENTS_LESS_THAN_30)
		//	drawProfiles30Geometry(drawTexture);
		//else if (contentsEnum==BASKET_CONTENS_SCRAP)
		//	drawScrapGeometry(drawTexture);
		glPopMatrix();
		// LOG("b.cpp", "INFO: Finished drawing geometry for basket number " + QString::number(number));
	}
	//
}

extern	GLfloat	*triVertexesForNormalBasketArray;
extern	GLfloat	*triNormalsForNormalBasketArray;
extern	GLfloat	*triTexCoordForNormalBasketArray;
extern	GLfloat	*quadVertexesForNormalBasketArray;
extern	GLfloat	*quadNormalsForNormalBasketArray;
extern	GLfloat	*quadTexCoordForNormalBasketArray;
extern int numberOfTrisForNormalBasket;
extern int numberOfQuadForNormalBasket;

void basketClass::drawNormalBasketGeometry(bool drawTexture)
{
	if (drawTexture){ 
		// LOG("b.cpp", "Drawing normal basket geometry with texture for basket number " + QString::number(number));
		glBindTexture(GL_TEXTURE_2D,WHITE_TEXTURE_TRIA);
	}

	// LOG("b.cpp", "Drawing normal basket geometry for basket number " + QString::number(number) + " with " + QString::number(numberOfTrisForNormalBasket) + " triangles and " + QString::number(numberOfQuadForNormalBasket) + " quads");
	glVertexPointer(3,GL_FLOAT,0,&triVertexesForNormalBasketArray[0]);
	glNormalPointer(GL_FLOAT,0,&triNormalsForNormalBasketArray[0]);
	glTexCoordPointer(2,GL_FLOAT,0,&triTexCoordForNormalBasketArray[0]);
	glDrawArrays(GL_TRIANGLES,0,numberOfTrisForNormalBasket*3);

	if (drawTexture) {
		// LOG("b.cpp", "Drawing normal basket geometry with texture for basket number " + QString::number(number) + ", switching to quad texture");	
		glBindTexture(GL_TEXTURE_2D, WHITE_TEXTURE_QUAD);
	}

	// LOG("b.cpp", "Drawing normal basket geometry for basket number " + QString::number(number) + " with quads, using " + QString::number(numberOfQuadForNormalBasket) + " quads");
	glVertexPointer(3,GL_FLOAT,0,&quadVertexesForNormalBasketArray[0]);
	glNormalPointer(GL_FLOAT,0,&quadNormalsForNormalBasketArray[0]);
	glTexCoordPointer(2,GL_FLOAT,0,&quadTexCoordForNormalBasketArray[0]);
	glDrawArrays(GL_QUADS,0,numberOfQuadForNormalBasket*4);
}

extern GLfloat	*triVertexesForScrapBasketArray;
extern GLfloat	*triNormalsForScrapBasketArray;
extern GLfloat	*triTexCoordForScrapBasketArray;
extern GLfloat	*quadVertexesForScrapBasketArray;
extern GLfloat	*quadNormalsForScrapBasketArray;
extern GLfloat	*quadTexCoordForScrapBasketArray;
extern int numberOfTrisForScrapBasket;
extern int numberOfQuadForScrapBasket;

void basketClass::drawScrapBasketGeometry(bool drawTexture)
{
	if (drawTexture)
		glBindTexture(GL_TEXTURE_2D,WHITE_TEXTURE_TRIA);

	glVertexPointer(3,GL_FLOAT,0,&triVertexesForScrapBasketArray[0]);
	glNormalPointer(GL_FLOAT,0,&triNormalsForScrapBasketArray[0]);
	glTexCoordPointer(2,GL_FLOAT,0,&triTexCoordForScrapBasketArray[0]);
	glDrawArrays(GL_TRIANGLES,0,numberOfTrisForScrapBasket*3);

	if (drawTexture)
		glBindTexture(GL_TEXTURE_2D,WHITE_TEXTURE_QUAD);

	glVertexPointer(3,GL_FLOAT,0,&quadVertexesForScrapBasketArray[0]);
	glNormalPointer(GL_FLOAT,0,&quadNormalsForScrapBasketArray[0]);
	glTexCoordPointer(2,GL_FLOAT,0,&quadTexCoordForScrapBasketArray[0]);
	glDrawArrays(GL_QUADS,0,numberOfQuadForScrapBasket*4);
}

extern GLfloat	*triVertexesForProfileArray;
extern GLfloat	*triNormalsForProfileArray;
extern GLfloat	*triTexCoordForProfileArray;
extern GLfloat	*quadVertexesForProfileArray;
extern GLfloat	*quadNormalsForProfileArray;
extern GLfloat	*quadTexCoordForProfileArray;
extern int numberOfTrisForProfile;
extern int numberOfQuadForProfile;

void basketClass::drawProfilesGeometry(bool drawTexture)
{
	if (drawTexture)
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);

		if (!areContentsAged)
			glColor3ub(200, 255, 200);  // green
		else
			glColor3ub(200, 155, 100);  // brown
	}

	if (drawTexture)
		glBindTexture(GL_TEXTURE_2D,WHITE_TEXTURE_TRIA);

	glVertexPointer(3,GL_FLOAT,0,&triVertexesForProfileArray[0]);
	glNormalPointer(GL_FLOAT,0,&triNormalsForProfileArray[0]);
	glTexCoordPointer(2,GL_FLOAT,0,&triTexCoordForProfileArray[0]);
	glDrawArrays(GL_TRIANGLES,0,numberOfTrisForProfile*3);

	if (drawTexture)
		glBindTexture(GL_TEXTURE_2D,WHITE_TEXTURE_QUAD);

	glVertexPointer(3,GL_FLOAT,0,&quadVertexesForProfileArray[0]);
	glNormalPointer(GL_FLOAT,0,&quadNormalsForProfileArray[0]);
	glTexCoordPointer(2,GL_FLOAT,0,&quadTexCoordForProfileArray[0]);
	glDrawArrays(GL_QUADS,0,numberOfQuadForProfile*4);

	if (drawTexture)
		glPopAttrib();
}

extern GLfloat	*triVertexesFor24ProfileArray;
extern GLfloat	*triNormalsFor24ProfileArray;
extern GLfloat	*triTexCoordFor24ProfileArray;
extern GLfloat	*quadVertexesFor24ProfileArray;
extern GLfloat	*quadNormalsFor24ProfileArray;
extern GLfloat	*quadTexCoordFor24ProfileArray;
extern int numberOfTrisFor24Profile;
extern int numberOfQuadFor24Profile;

void basketClass::drawProfiles24Geometry(bool drawTexture)
{
	if (drawTexture)
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);

		if (!areContentsAged)
			glColor3ub(200, 255, 200);
		else
			glColor3ub(200, 155, 100);
	}

	if (drawTexture)
		glBindTexture(GL_TEXTURE_2D,WHITE_TEXTURE_TRIA);

	glVertexPointer(3, GL_FLOAT, 0, &triVertexesFor24ProfileArray[0]);
	glNormalPointer(GL_FLOAT, 0, &triNormalsFor24ProfileArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &triTexCoordFor24ProfileArray[0]);
	glDrawArrays(GL_TRIANGLES, 0, numberOfTrisFor24Profile * 3);

	if (drawTexture)
		glBindTexture(GL_TEXTURE_2D, WHITE_TEXTURE_QUAD);

	glVertexPointer(3, GL_FLOAT, 0, &quadVertexesFor24ProfileArray[0]);
	glNormalPointer(GL_FLOAT, 0, &quadNormalsFor24ProfileArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &quadTexCoordFor24ProfileArray[0]);
	glDrawArrays(GL_QUADS, 0, numberOfQuadFor24Profile * 4);

	if (drawTexture)
		glPopAttrib();
}
extern GLfloat	*triVertexesFor30ProfileArray;
extern GLfloat	*triNormalsFor30ProfileArray;
extern GLfloat	*triTexCoordFor30ProfileArray;
extern GLfloat	*quadVertexesFor30ProfileArray;
extern GLfloat	*quadNormalsFor30ProfileArray;
extern GLfloat	*quadTexCoordFor30ProfileArray;
extern int numberOfTrisFor30Profile;
extern int numberOfQuadFor30Profile;

void basketClass::drawProfiles30Geometry(bool drawTexture)
{
	if (drawTexture)
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);

		if (!areContentsAged)
			glColor3ub(200, 200, 255);
		else
			glColor3ub(255, 200, 255);
	}

	if (drawTexture)
		glBindTexture(GL_TEXTURE_2D,WHITE_TEXTURE_TRIA);

	glVertexPointer(3, GL_FLOAT, 0, &triVertexesFor30ProfileArray[0]);
	glNormalPointer(GL_FLOAT, 0, &triNormalsFor30ProfileArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &triTexCoordFor30ProfileArray[0]);
	glDrawArrays(GL_TRIANGLES, 0, numberOfTrisFor30Profile * 3);

	if (drawTexture)
		glBindTexture(GL_TEXTURE_2D, WHITE_TEXTURE_QUAD);

	glVertexPointer(3, GL_FLOAT, 0, &quadVertexesFor30ProfileArray[0]);
	glNormalPointer(GL_FLOAT, 0, &quadNormalsFor30ProfileArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &quadTexCoordFor30ProfileArray[0]);
	glDrawArrays(GL_QUADS, 0, numberOfQuadFor30Profile * 4);

	if (drawTexture)
		glPopAttrib();
}

void basketClass::drawScrapGeometry(bool drawTexture)
{
}

#include "b.h"
#include "dbTableStatus.h"

#include "../common/applogger.h" // @@@@@ -- logger LOG() function is now in its own file, so including that instead of apexG.h to avoid unnecessary dependencies. 
#include "../common/execDBQuery.h"
#include <QHash>
#include <QSet>
						      //		  Should be included in tcpTux.h when the logger is moved there.

namespace {

struct CachedBasketDetail
{
	QString die;
	QString profileCount;
	QVariant length;
	QString temper;
};

QHash<int, QVector<CachedBasketDetail> > basketDetailsCache;
QHash<int, QVariant> basketAgedTimeCache;
QHash<QString, int> temperTypeCache;
bool basketDataCacheLoaded = false;

QVector<CachedBasketDetail> parseDetailRows(const QVector<QVector<QVariant> >& rows, int firstValueColumn, const QString& context)
{
	QVector<CachedBasketDetail> details;
	for (int i = 0; i < rows.count(); ++i) {
		if (rows[i].count() < firstValueColumn + 4) {
			LOG("ERROR: BASKET CACHE", QString("%1 row %2 has %3 columns; expected at least %4")
				.arg(context).arg(i).arg(rows[i].count()).arg(firstValueColumn + 4));
			continue;
		}
		CachedBasketDetail detail;
		detail.die = rows[i][firstValueColumn].toString().trimmed();
		detail.profileCount = rows[i][firstValueColumn + 1].toString().trimmed();
		detail.length = rows[i][firstValueColumn + 2];
		detail.temper = rows[i][firstValueColumn + 3].toString().trimmed();
		details.append(detail);
	}
	return details;
}

QVector<CachedBasketDetail> cachedDetailsForBasket(int basketId)
{
	if (!basketDataCacheLoaded)
		loadBasketDataCache();
	return basketDetailsCache.value(basketId);
}

QVariant cachedAgedTimeForBasket(int basketId)
{
	if (!basketDataCacheLoaded)
		loadBasketDataCache();
	return basketAgedTimeCache.value(basketId);
}

int cachedTemperType(const QString& temper)
{
	if (!basketDataCacheLoaded)
		loadBasketDataCache();
	return temperTypeCache.value(temper, 0);
}

} // namespace

bool loadBasketDataCache()
{
	LOG("BASKET CACHE", "Loading bulk basket data cache");
	setDbTableActive("basket_details", true);
	const QVector<QVector<QVariant> > detailRows = execDBTableQuery(
		"select distinct basketnum, dienum, Pc, PcLen, temper from basket_details where Pc<>0 order by basketnum, dienum, Pc, PcLen, temper",
		APEX_DB_CONNECTION);
	setDbTableIdle("basket_details");

	setDbTableActive("basket_location", true);
	const QVector<QVector<QVariant> > agedRows = execDBTableQuery(
		"select basketnum, max(audittime) from basket_location where basketlocation=3 group by basketnum order by basketnum",
		APEX_DB_CONNECTION);
	setDbTableIdle("basket_location");

	setDbTableActive("temperTypes", true);
	const QVector<QVector<QVariant> > temperRows = execDBTableQuery(
		"select temper, temperType from temperTypes order by temper", MAIN_DB_CONNECTION);
	setDbTableIdle("temperTypes");

	QHash<int, QVector<CachedBasketDetail> > newDetails;
	for (int i = 0; i < detailRows.count(); ++i) {
		if (detailRows[i].count() < 5) {
			LOG("ERROR: BASKET CACHE", QString("Bulk detail row %1 has only %2 columns; expected 5")
				.arg(i).arg(detailRows[i].count()));
			continue;
		}
		const int basketId = detailRows[i][0].toInt();
		const QVector<QVector<QVariant> > oneRow(1, detailRows[i]);
		const QVector<CachedBasketDetail> parsed = parseDetailRows(oneRow, 1, QString("Basket %1").arg(basketId));
		if (!parsed.isEmpty())
			newDetails[basketId].append(parsed[0]);
	}

	QHash<int, QVariant> newAgedTimes;
	for (int i = 0; i < agedRows.count(); ++i) {
		if (agedRows[i].count() >= 2 && !agedRows[i][1].isNull())
			newAgedTimes.insert(agedRows[i][0].toInt(), agedRows[i][1]);
	}

	QHash<QString, int> newTemperTypes;
	for (int i = 0; i < temperRows.count(); ++i) {
		if (temperRows[i].count() >= 2)
			newTemperTypes.insert(temperRows[i][0].toString().trimmed(), temperRows[i][1].toInt());
	}

	basketDetailsCache = newDetails;
	basketAgedTimeCache = newAgedTimes;
	temperTypeCache = newTemperTypes;
	basketDataCacheLoaded = true;
	LOG("BASKET CACHE", QString("Bulk cache ready: detailRows=%1, basketsWithDetails=%2, agedBaskets=%3, temperTypes=%4")
		.arg(detailRows.count()).arg(basketDetailsCache.count())
		.arg(basketAgedTimeCache.count()).arg(temperTypeCache.count()));
	return true;
}

bool refreshBasketDataCache(int basketId)
{
	if (basketId <= 0)
		return false;

	setDbTableActive("basket_details", true);
	const QVector<QVector<QVariant> > detailRows = execDBTableQuery(
		QString("select distinct dienum, Pc, PcLen, temper from basket_details where basketnum=%1 and Pc<>0 order by dienum, Pc, PcLen, temper")
			.arg(basketId), APEX_DB_CONNECTION);
	setDbTableIdle("basket_details");

	setDbTableActive("basket_location", true);
	const QVector<QVector<QVariant> > agedRows = execDBTableQuery(
		QString("select max(audittime) from basket_location where basketnum=%1 and basketlocation=3").arg(basketId),
		APEX_DB_CONNECTION);
	setDbTableIdle("basket_location");

	basketDetailsCache.insert(basketId, parseDetailRows(detailRows, 0, QString("Basket %1 refresh").arg(basketId)));
	basketAgedTimeCache.remove(basketId);
	if (!agedRows.isEmpty() && !agedRows[0].isEmpty() && !agedRows[0][0].isNull())
		basketAgedTimeCache.insert(basketId, agedRows[0][0]);
	basketDataCacheLoaded = true;
	LOG("BASKET CACHE", QString("Basket %1 cache refreshed: detailRows=%2").arg(basketId).arg(detailRows.count()));
	return true;
}

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

		const QVector<CachedBasketDetail> detailRows = cachedDetailsForBasket(tableId);

		maxLengthOfContents = 0.;
		const int rowCount = detailRows.count();
		const int maxTooltipRows = 100;
		QSet<QString> seenRows;
		QSet<QString> uniqueTempers;
		int uniqueRows = 0;
		for (int i = 0; i < rowCount; ++i) {
			QString pa = detailRows[i].die;
			QString npa = detailRows[i].profileCount;
			QString tpa = detailRows[i].temper;
			if (pa.isEmpty()) pa = "N/A";
			if (npa.isEmpty()) npa = "N/A";
			if (tpa.isEmpty()) tpa = "N/A";

			const QString rowKey = pa + QChar(0x1f) + npa + QChar(0x1f)
				+ detailRows[i].length.toString() + QChar(0x1f) + tpa;
			if (seenRows.contains(rowKey))
				continue;
			seenRows.insert(rowKey);
			++uniqueRows;

			double tLength = detailRows[i].length.toDouble();
			if (tpa != "N/A")
				uniqueTempers.insert(tpa);
			const bool doubleBatched = tLength < 3601;
			if (doubleBatched)
				tLength = 2 * tLength + 200.;
			if (tLength > maxLengthOfContents)
				maxLengthOfContents = tLength;

			if (uniqueRows <= maxTooltipRows) {
				toolTipText += QString("die:%1, %2 profiles with length %3mm%4, temper:%5\n")
					.arg(pa).arg(npa).arg(tLength)
					.arg(doubleBatched ? " (double)" : "").arg(tpa);
			}
		}
		if (uniqueRows > maxTooltipRows) {
			toolTipText += QString("... %1 additional unique detail rows omitted\n")
				.arg(uniqueRows - maxTooltipRows);
		}
		LOG("BASKET DATA", QString("Basket %1 details loaded: databaseRows=%2, uniqueRows=%3, duplicatesSkipped=%4")
			.arg(tableId).arg(rowCount).arg(uniqueRows).arg(rowCount - uniqueRows));

		contentsEnum = BASKET_CONTENS_NONE;
		
		if (uniqueRows > 0)
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

		QVariant time = cachedAgedTimeForBasket(tableId);
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

		if (uniqueTempers.isEmpty())
			temperEnum = BASKET_TEMPER_NULL;
		else
		{
			temperEnum = BASKET_TEMPER_230;
			QString validTemper;
			QSet<QString>::const_iterator temperIt = uniqueTempers.constBegin();
			for (; temperIt != uniqueTempers.constEnd(); ++temperIt) {
				int tTemper = cachedTemperType(*temperIt);
				if (temperEnum>tTemper)
				{
					temperEnum = tTemper;
					validTemper = *temperIt;
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

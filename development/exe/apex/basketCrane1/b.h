#ifndef B_H
#define B_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "cG.h"
#include "op.h"
#include "GL/GLU.h"

// Load all basket content/aging/temper lookup data in three bulk queries.
// Runtime entry points can refresh one basket without rebuilding the cache.
bool loadBasketDataCache();
bool refreshBasketDataCache(int basketId);
bool refreshBasketDataCacheIfStale(int basketId, int maxAgeMilliseconds = 5000);

class basketClass:public QObject
{
	/* @@@@@ These are set in the constructor and should be initialized there, but they are not. This is a potential source of bugs, as these variables may have undefined values if the constructor is not called or if the object is not properly initialized.
			 They should be initialized to default values in the constructor to ensure that they have known values when the object is created.
	tableId = number = 0;
	areContentsAged = false;
	temperEnum = BASKET_TEMPER_NULL;
	maxLengthOfContents = 0.;
	*/

	int						tableId;
	int						number;
	int						typeEnum;
	int						contentsEnum;
	bool					areContentsAged;
	int						nonAgeableTemperMask;
	float					maxLengthOfContents;
	QString					toolTipText;
	//
	QVector<QVariant>		profileIds;

public:
	int						temperEnum;

	enum{
		BASKET_TEMPER_NULL=0,//initial value
		BASKET_TEMPER_000,//ready for packaging
		BASKET_TEMPER_185,
		BASKET_TEMPER_230,
	};
	enum {
		NON_AGEABLE_TEMPER_NONE = 0x00,
		NON_AGEABLE_TEMPER_T1 = 0x01,
		NON_AGEABLE_TEMPER_T4 = 0x02,
		NON_AGEABLE_TEMPER_F = 0x04,
	};

public:
	basketClass &operator=(const basketClass& t)
	{
		tableId=t.tableId;
		number=t.number;
		typeEnum=t.typeEnum;
		contentsEnum=t.contentsEnum;
		nonAgeableTemperMask=t.nonAgeableTemperMask;
		return *this;
	}

	Q_OBJECT

public:
	basketClass();
	~basketClass();
	//
	void setTableId(int id_);
	void setTypeEnum(int t);
	//
	int getTableId();
	int getNumber();
	int getTypeEnum();
	int getContentsEnum();
	int getTemperEnum();
	bool getAreContentsAged();
	bool hasNonAgeableTemperMaterial();
	int getNonAgeableTemperMask();
	bool hasContents();
	QString getToolTipText();
	//
	void drawGeometry(bool drawTexture);
	//
	void calculateBasketContentsTypeFromEpicsDatabase();
	void loadContentFromDatabase();
	void drawNormalBasketGeometry(bool drawTexture);
	void drawScrapBasketGeometry(bool drawTexture);

private:
	void drawProfilesGeometry(bool drawTexture);
	void drawProfiles24Geometry(bool drawTexture);
	void drawProfiles30Geometry(bool drawTexture);
	void drawScrapGeometry(bool drawTexture);
public:

signals:
	void openGlChanged();
};
#endif

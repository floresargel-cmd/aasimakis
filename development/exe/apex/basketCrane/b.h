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

class basketClass:public QObject
{
	int									tableId;
	int									number;
	int									typeEnum;
	int									contentsEnum;
	bool								areContentsAged;
	float								maxLengthOfContents;
	QString							toolTipText;
	//
	QVector<QVariant>		profileIds;
public:
	int									temperEnum;
	enum{
		BASKET_TEMPER_NULL=0,//initial value
		BASKET_TEMPER_000,//ready for packaging
		BASKET_TEMPER_185,
		BASKET_TEMPER_230,
	};
public:
	basketClass &operator=(const basketClass& t)
	{
		tableId=t.tableId;
		number=t.number;
		typeEnum=t.typeEnum;
		contentsEnum=t.contentsEnum;
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
	bool hasContents();
	QString getToolTipText();
	//
	void drawGeometry(bool drawTexture);
	//
	void calculateBasketContentsTypeFromEpicsDatabase();
	void loadContentFromsDatabase();
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

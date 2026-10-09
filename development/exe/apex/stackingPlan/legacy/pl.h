#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qnamespace.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qhash.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qbrush.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qcolor.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpainterpath.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsitem.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsscene.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#ifndef PL_H
#define PL_H


#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "pp.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// layerProfileGroupClass
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class layerProfileGroupClass
{
private:
	int                     noOfProfiles;
	profileProductionClass* profileProduction;
	QPainterPath            path;

public:
	layerProfileGroupClass();
	layerProfileGroupClass(profileProductionClass* profileProduction_, double baseHeight);
	~layerProfileGroupClass();

	int getTableId() const;
	int getNumberOfProfiles() const;
	void addProfile(const QPainterPath& path);
	const QPainterPath& getPath() const;
	profileProductionClass* getProfileProduction() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// profileLayerClass
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class profileLayerClass : public QWidget
{
	Q_OBJECT

public:

	QString lastDrawnSoNum;
	QColor lastDrawnSoColor;
	struct LayerSummaryInfo
	{
		int totalPcs;
		int basketId;

		LayerSummaryInfo()
			: totalPcs(-1),
			basketId(-1)
		{
		}
	};

private:
	struct ProfileSummary
	{
		int code = 0;
		int epicsTableId = 0;
		QColor color;
		int multiplicity = 0;
		int occurrences = 0;
	};

private:
	QGraphicsScene* myScene;

	int                                 realBasketId;
	int                                 number;
	int                                 newbasketnumber;
	int                                 newlastnumber;
	int                                 combNumber;

	double                              baseHeight;
	int                                 wordFromStackerPlc;
	QVector<bool>                       bitsFromStackerPlc;

	profileProductionClass* lastProfile;
	double                              lastProfileDx;
	double                              lastProfileDy;

	QVector<int>                        profiles;
	QVector<int>                        profilesEpicsTableId;
	QVector<QColor>                     profilesColors;
	QVector<int>                        profilesMultiplicity;

	double                              totalHeight;
	double                              totalWidth;
	double                              totalWeight;
	double                              maxProfileLength;

	QVector<bool>                       spacers;
	double                              flatSurfase;
	double                              maxProfileHeight;

	QVector<layerProfileGroupClass*>    profileGroupsVector;
	QVector<QGraphicsSimpleTextItem*>   profileQuandityTextItemVector;
	QVector<QGraphicsSimpleTextItem*>   profileQuandityDMSTextItemVector;
	QVector<QGraphicsSimpleTextItem*>   calculatedLayerWeight;
	QVector<QGraphicsPathItem*> profileItems;

	int                                 lastnumeric;
	int                                 lastdieprofile;

private:
	static double spacerXPosition(int index);
	void drawSpacerSlot(int index, const QBrush& brush = Qt::NoBrush);
	void initializeQuantityLabels();
	int resolveLayerWidth() const;
	void updateQuantityLabels(profileProductionClass* p, int maxLayerWeight);
	QHash<int, LayerSummaryInfo> loadLayerSummaryTotals(profileProductionClass* p) const;


public:
	profileLayerClass(int number_, int combNumber_, double baseHeight_, QGraphicsScene* myScene_);
	~profileLayerClass();

	int getFirstProfile() const;

	void addToGroup(profileProductionClass* profileProduction, const QPainterPath& p);

	double getTotalHeight() const;
	double getMaxProfileLength() const;
	bool getCanSupportLayer() const;
	double getTotalWeight() const;
	int getNumber() const;
	int getCombNumber() const;
	bool getIsOnCombForDb() const;
	bool getIsOnComb() const;
	int getRealBasketId() const;
	void resetTotalWidth();

	int addProfiles(
		profileProductionClass* p,
		int numberOfProfiles,
		bool forceDrawRequestedQty = false);

	const QVector<int>& getProfilesEpicsTableId() const;
	const QVector<int>& getProfilesCodesTableId() const;
	const QVector<QColor>& getProfilesColors() const;
	const QVector<int>& getProfilesMultiplicity() const;

	bool hasProfile(int code) const;
	void addSpacers(const QVector<bool>& sp);
	int getSpacersWord() const;
	void setSpacerWordFromStackerPlc(int v);
	QVector<bool> getSpacers() const;
	const QVector<layerProfileGroupClass*>& getProfileGroups() const;
};

#endif

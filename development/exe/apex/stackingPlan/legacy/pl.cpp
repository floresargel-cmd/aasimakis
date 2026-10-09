#include "pl.h"

#include <tuxG.h>

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qglobal.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qnamespace.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qalgorithms.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qalgorithms.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qhash.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qrect.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qset.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qbrush.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qbrush.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qcolor.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qcolor.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpainterpath.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpainterpath.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpen.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpen.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/text/qfont.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/text/qfont.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsitem.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsitem.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsscene.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsscene.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../Application/Services/MainWindowOrchestrator.h"
#include "../Infrastructure/Database/SqlHelper.h"
#include "mW.h"
#include "pp.h"
#include "ppG.h"
#include <cmath>
#include <memory>

static QColor GetRandomSceneColor()
{
	static const QVector<QColor> palette = QVector<QColor>()
		<< QColor(220, 20, 60)    // crimson
		<< QColor(255, 140, 0)    // dark orange
		<< QColor(255, 105, 180)  // hot pink
		<< QColor(0, 170, 170)    // teal
		<< QColor(160, 82, 45)    // sienna
		<< QColor(70, 130, 180)   // steel blue
		<< QColor(199, 21, 133)   // medium violet red
		<< QColor(218, 165, 32)   // goldenrod
		<< QColor(205, 92, 92)    // indian red
		<< QColor(106, 90, 205);  // slate blue

	const int index = qrand() % palette.size();
	return palette[index];
}

static QColor GetMatchedDarkGreen()
{
	return QColor(0, 100, 0); // dark green
}

namespace
{
	static bool ShouldShowBracketedQuantityForBasketIndex(int basketIndex)
	{

		if (basketIndex == 1) // footer No.1 only
			return true;
		return false;
	}

	static QColor GetStackingStatusColor(int qty, int totalPcs, const QColor& defaultColor)
	{
		if (totalPcs > 0)
		{
			if (qty == totalPcs)
				return QColor(204, 170, 0);      // exact match

			if (qty > totalPcs)
				return QColor("darkgreen");      // understack = dark purple

			if (qty < totalPcs)
				return QColor("darkred");        // overstack = dark blue
		}

		return defaultColor;
	}

	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> GetOrchestrator()
	{
		mainWindowClass* mainWindow = mainWindowClass::GetInstance();
		if (!mainWindow)
			return nullptr;

		return mainWindow->getOrchestrator();
	}

	int GetGlobalPressNumber()
	{
		auto orchestrator = GetOrchestrator();
		if (!orchestrator)
			return 1; // safe default

		return orchestrator->GetGlobalPressNumber();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// layerProfileGroupClass
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

layerProfileGroupClass::layerProfileGroupClass()
	: noOfProfiles(0),
	profileProduction(nullptr)
{
}

layerProfileGroupClass::layerProfileGroupClass(profileProductionClass* profileProduction_, double /*baseHeight*/)
	: noOfProfiles(0),
	profileProduction(profileProduction_)
{
}

layerProfileGroupClass::~layerProfileGroupClass()
{
}

int layerProfileGroupClass::getTableId() const
{
	return profileProduction ? profileProduction->getProfilesEpicsTableId() : 0;
}

int layerProfileGroupClass::getNumberOfProfiles() const
{
	return noOfProfiles;
}

void layerProfileGroupClass::addProfile(const QPainterPath& p)
{
	++noOfProfiles;
	path.addRect(p.boundingRect());
}

const QPainterPath& layerProfileGroupClass::getPath() const
{
	return path;
}

profileProductionClass* layerProfileGroupClass::getProfileProduction() const
{
	return profileProduction;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// profileLayerClass
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double profileLayerClass::spacerXPosition(int index)
{
	static const double positions[8] = { -600., -435., -270., -105., 60., 225., 390., 550. };
	return (index >= 0 && index < 8) ? positions[index] : 0.0;
}

void profileLayerClass::drawSpacerSlot(int index, const QBrush& brush)
{
	myScene->addRect(
		spacerXPosition(index),
		baseHeight + 1.5,
		50.,
		16.,
		QPen(QColor(200, 200, 200), 0.0, Qt::SolidLine),
		brush);
}

void profileLayerClass::initializeQuantityLabels()
{
	const double textSpacing = 50.0;

	// LEFT SIDE = quantity
	QGraphicsSimpleTextItem* qtyItem = new QGraphicsSimpleTextItem("");
	qtyItem->setPos(-0.5 * SPACER_WIDTH - 160., baseHeight + 40);
	qtyItem->setFont(QFont("arial", 20));
	qtyItem->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	profileQuandityTextItemVector << qtyItem;
	myScene->addItem(qtyItem);

	// LEFT SIDE = DMS / expected qty
	QGraphicsSimpleTextItem* dmsItem = new QGraphicsSimpleTextItem("");
	dmsItem->setPos(-0.5 * SPACER_WIDTH - 115., baseHeight + 40);
	dmsItem->setFont(QFont("arial", 20));
	dmsItem->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	profileQuandityDMSTextItemVector << dmsItem;
	myScene->addItem(dmsItem);

	// RIGHT SIDE = weight
	QGraphicsSimpleTextItem* weightItem = new QGraphicsSimpleTextItem("");
	weightItem->setPos(0.5 * SPACER_WIDTH + 70., baseHeight + 40);
	weightItem->setFont(QFont("arial", 20));
	weightItem->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	calculatedLayerWeight << weightItem;
	myScene->addItem(weightItem);

	for (int i = 0; i < 10; ++i)
	{
		// LEFT SIDE = quantity list
		QGraphicsSimpleTextItem* qty = new QGraphicsSimpleTextItem("");
		qty->setPos(-0.5 * SPACER_WIDTH - 160. - (i + 1) * textSpacing, baseHeight + 40);
		qty->setFont(QFont("arial", 20));
		qty->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
		profileQuandityTextItemVector << qty;
		myScene->addItem(qty);

		// LEFT SIDE = DMS list
		QGraphicsSimpleTextItem* dms = new QGraphicsSimpleTextItem("");
		dms->setPos(-0.5 * SPACER_WIDTH - 115. - (i + 1) * textSpacing, baseHeight + 40);
		dms->setFont(QFont("arial", 20));
		dms->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
		profileQuandityDMSTextItemVector << dms;
		myScene->addItem(dms);

		// RIGHT SIDE = weight list
		QGraphicsSimpleTextItem* weight = new QGraphicsSimpleTextItem("");
		weight->setPos(0.5 * SPACER_WIDTH + 20. + (i + 1) * textSpacing, baseHeight + 40);
		weight->setFont(QFont("arial", 12));
		weight->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
		calculatedLayerWeight << weight;
		myScene->addItem(weight);
	}
}

int profileLayerClass::resolveLayerWidth() const
{
	int layerWidth = MAX_LAYER_WIDTH;

	const int press = GetGlobalPressNumber();

	if (press == 1)
		layerWidth = MAX_LAYER_WIDTH_P1;
	else if (press == 2)
		layerWidth = MAX_LAYER_WIDTH_P2;

	return layerWidth;
}

profileLayerClass::profileLayerClass(int number_, int combNumber_, double baseHeight_, QGraphicsScene* myScene_)
	: QWidget(),
	myScene(myScene_),
	realBasketId(-1),
	number(0),
	newbasketnumber(0),
	newlastnumber(number_),
	combNumber(combNumber_),
	baseHeight(baseHeight_),
	wordFromStackerPlc(0),
	lastProfile(nullptr),
	lastProfileDx(0.0),
	lastProfileDy(0.0),
	totalHeight(0.0),
	totalWidth(0.0),
	totalWeight(0.0),
	maxProfileLength(0.0),
	flatSurfase(0.0),
	maxProfileHeight(0.0),
	lastnumeric(0),
	lastdieprofile(0)
{

	lastDrawnSoNum = "";
	lastDrawnSoColor = QColor();
	newbasketnumber = newlastnumber / 100;
	number = newlastnumber % 100;

	spacers.resize(8);
	spacers.fill(false);

	myScene->addRect(
		-0.5 * SPACER_WIDTH,
		baseHeight + 1.5,
		SPACER_WIDTH - 3.,
		SPACER_THIKNESS - 3.,
		QPen(QColor(255, 255, 255), 0.0, Qt::SolidLine));

	myScene->addLine(-620., baseHeight, 620., baseHeight, QPen(QColor(247, 164, 9), 0.1, Qt::SolidLine));
	myScene->addLine(-620., baseHeight + SPACER_THIKNESS, 620., baseHeight + SPACER_THIKNESS, QPen(QColor(247, 164, 9), 0.1, Qt::SolidLine));

	for (int i = 0; i < 8; ++i)
	{
		drawSpacerSlot(i);
	}

	initializeQuantityLabels();
}

profileLayerClass::~profileLayerClass()
{
	qDeleteAll(profileQuandityTextItemVector);
	profileQuandityTextItemVector.clear();

	qDeleteAll(profileQuandityDMSTextItemVector);
	profileQuandityDMSTextItemVector.clear();

	qDeleteAll(calculatedLayerWeight);
	calculatedLayerWeight.clear();

	qDeleteAll(profileGroupsVector);
	profileGroupsVector.clear();
}

int profileLayerClass::getNumber() const
{
	return number;
}

int profileLayerClass::getCombNumber() const
{
	return combNumber;
}

void profileLayerClass::addSpacers(const QVector<bool>& sp)
{
	const QBrush brush(
		combNumber > 0
		? QColor(150, 150, 175)
		: QColor(220, 20, 60));

	const int count = qMin(sp.size(), 8);
	for (int i = 0; i < count; ++i)
	{
		if (sp[i])
		{
			spacers[i] = true;
			drawSpacerSlot(i, brush);
		}
	}
}

#define USE_DATA_FROM_PLC

QVector<bool> profileLayerClass::getSpacers() const
{
#ifdef USE_DATA_FROM_PLC
	QVector<bool> ans;

	if (bitsFromStackerPlc.size() >= 9)
	{
		ans << bitsFromStackerPlc[1]
			<< bitsFromStackerPlc[2]
			<< bitsFromStackerPlc[3]
			<< bitsFromStackerPlc[4]
			<< bitsFromStackerPlc[5]
			<< bitsFromStackerPlc[6]
			<< bitsFromStackerPlc[7]
			<< bitsFromStackerPlc[8];
	}
	else
	{
		ans = spacers;
	}

	return ans;
#else
	return spacers;
#endif
}

bool profileLayerClass::getIsOnCombForDb() const
{
#ifdef USE_DATA_FROM_PLC
	return bitsFromStackerPlc.size() > 0 ? bitsFromStackerPlc[0] : false;
#else
	return nComb();
#endif
}

bool profileLayerClass::getIsOnComb() const
{
	return combNumber > 0;
}

void profileLayerClass::setSpacerWordFromStackerPlc(int v)
{
	wordFromStackerPlc = v;
	bitsFromStackerPlc = deprecatedCalculateBits(wordFromStackerPlc);
}

int profileLayerClass::getSpacersWord() const
{
	int ans = 0;
	ans += (spacers.value(0) ? 1 : 0);
	ans += (spacers.value(1) ? 2 : 0);
	ans += (spacers.value(2) ? 4 : 0);
	ans += (spacers.value(3) ? 8 : 0);
	ans += (spacers.value(4) ? 16 : 0);
	ans += (spacers.value(5) ? 32 : 0);
	ans += (spacers.value(6) ? 64 : 0);
	ans += (spacers.value(7) ? 128 : 0);
	return ans;
}

const QVector<int>& profileLayerClass::getProfilesEpicsTableId() const
{
	return profilesEpicsTableId;
}

const QVector<int>& profileLayerClass::getProfilesCodesTableId() const
{
	return profiles;
}

const QVector<int>& profileLayerClass::getProfilesMultiplicity() const
{
	return profilesMultiplicity;
}

const QVector<QColor>& profileLayerClass::getProfilesColors() const
{
	return profilesColors;
}

double profileLayerClass::getTotalHeight() const
{
	return baseHeight + SPACER_THIKNESS + totalHeight;
}

double profileLayerClass::getMaxProfileLength() const
{
	return maxProfileLength;
}

bool profileLayerClass::getCanSupportLayer() const
{
	const int layerWidth = resolveLayerWidth();
	return !((profiles.count() > 0) && (flatSurfase < (0.01 * LAYER_MIN_SUPORT_LENGTH) * layerWidth));
}

double profileLayerClass::getTotalWeight() const
{
	return totalWeight + spacers.count(true) * SPACER_WEIGHT;
}

void profileLayerClass::addToGroup(profileProductionClass* profileProduction, const QPainterPath& p)
{
	for (int i = 0; i < profileGroupsVector.count(); ++i)
	{
		if (profileGroupsVector[i]->getTableId() == profileProduction->getProfilesEpicsTableId())
		{
			profileGroupsVector[i]->addProfile(p);
			return;
		}
	}

	layerProfileGroupClass* g = new layerProfileGroupClass(profileProduction, baseHeight);
	g->addProfile(p);
	profileGroupsVector << g;
}

const QVector<layerProfileGroupClass*>& profileLayerClass::getProfileGroups() const
{
	return profileGroupsVector;
}

QHash<int, profileLayerClass::LayerSummaryInfo>
profileLayerClass::loadLayerSummaryTotals(profileProductionClass* p) const
{
	QHash<int, LayerSummaryInfo> totals;

	if (p == nullptr)
	{
		return totals;
	}

	QSet<int> uniqueCodes;
	for (int i = 0; i < profiles.size(); ++i)
	{
		uniqueCodes.insert(profiles[i]);
	}

	for (QSet<int>::const_iterator it = uniqueCodes.constBegin(); it != uniqueCodes.constEnd(); ++it)
	{
		const int dieCode = *it;

		LayerSummaryInfo info;

		const QString latestBasketIdQuery = QString(R"(
			SELECT TOP 1 basket_id
			FROM StackPlan.dbo.LayerSummary
			WHERE sonum = '%1'
			  AND dienumber = %2
			  AND pressnum = %3
			ORDER BY id DESC
		)")
			.arg(p->getSoNum())
			.arg(dieCode)
			.arg(GetGlobalPressNumber());

		try
		{
			const QVariant basketIdValue = SqlHelper::GetFirst(latestBasketIdQuery);
			if (basketIdValue.isValid() && !basketIdValue.isNull())
			{
				info.basketId = basketIdValue.toInt();
			}
		}
		catch (...)
		{
		}

		if (info.basketId > 0)
		{
			const QString totalPcsQuery = QString(R"(
				SELECT TOP 1 total_pcs
				FROM StackPlan.dbo.LayerSummary
				WHERE sonum = '%1'
				  AND dienumber = %2
				  AND pressnum = %3
				  AND basket_id = %4
				  AND ln_basket = %5
				ORDER BY id DESC
			)")
				.arg(p->getSoNum())
				.arg(dieCode)
				.arg(GetGlobalPressNumber())
				.arg(info.basketId)
				.arg(number);

			try
			{
				const QVariant totalPcsValue = SqlHelper::GetFirst(totalPcsQuery);
				if (totalPcsValue.isValid() && !totalPcsValue.isNull())
				{
					info.totalPcs = totalPcsValue.toInt();
				}
			}
			catch (...)
			{
			}
		}

		if (info.totalPcs >= 0 || info.basketId >= 0)
		{
			totals.insert(dieCode, info);
		}
	}

	return totals;
}

void profileLayerClass::updateQuantityLabels(profileProductionClass* p, int maxLayerWeight)
{
	QHash<int, ProfileSummary> summaryByEpicsId;
	QVector<int> order;
	order.reserve(profilesEpicsTableId.size());

	for (int i = 0; i < profilesEpicsTableId.size(); ++i)
	{
		const int epicsId = profilesEpicsTableId[i];

		if (!summaryByEpicsId.contains(epicsId))
		{
			ProfileSummary summary;
			summary.code = profiles[i];
			summary.epicsTableId = epicsId;
			summary.color = profilesColors[i];
			summary.multiplicity = profilesMultiplicity[i];
			summary.occurrences = 0;

			summaryByEpicsId.insert(epicsId, summary);
			order << epicsId;
		}

		summaryByEpicsId[epicsId].occurrences++;
	}

	const QHash<int, LayerSummaryInfo> summaryByDie = loadLayerSummaryTotals(p);
	const int limit = qMin(order.size(), profileQuandityTextItemVector.size());
	const double weightPerProfile = p->getWeightOfOneProfile();

	double totalRowWeight = 0.0;
	bool rowUsesBracketQuantity = false;
	QColor rowDisplayColor = QColor(0, 102, 204);

	for (int i = 0; i < limit; ++i)
	{
		const ProfileSummary& summary = summaryByEpicsId[order[i]];
		const int qty = summary.multiplicity * summary.occurrences;
		const LayerSummaryInfo info = summaryByDie.value(summary.code);

		if (info.basketId > 0)
		{
			realBasketId = info.basketId;
		}

		const bool allowBracketedQty = ShouldShowBracketedQuantityForBasketIndex(newbasketnumber);
		const int totalPcs = allowBracketedQty ? info.totalPcs : -1;
		const bool canUseRealQuantity = (allowBracketedQty && totalPcs > 0);

		QColor displayColor = summary.color;
		if (canUseRealQuantity)
		{
			displayColor = GetStackingStatusColor(qty, totalPcs, summary.color);
		}

		if (i == 0)
			rowDisplayColor = displayColor;

		const int quantityForWeight = canUseRealQuantity ? totalPcs : qty;
		totalRowWeight += quantityForWeight * weightPerProfile;

		if (canUseRealQuantity)
			rowUsesBracketQuantity = true;
	}

	for (int i = 0; i < limit; ++i)
	{
		const ProfileSummary& summary = summaryByEpicsId[order[i]];
		const int qty = summary.multiplicity * summary.occurrences;
		const LayerSummaryInfo info = summaryByDie.value(summary.code);

		if (info.basketId > 0)
		{
			realBasketId = info.basketId;
		}

		const bool allowBracketedQty = ShouldShowBracketedQuantityForBasketIndex(newbasketnumber);
		const int totalPcs = allowBracketedQty ? info.totalPcs : -1;
		const bool canUseRealQuantity = (allowBracketedQty && totalPcs > 0);

		QColor displayColor = summary.color;
		if (canUseRealQuantity)
		{
			displayColor = GetStackingStatusColor(qty, totalPcs, summary.color);
		}

		if (i < profileQuandityTextItemVector.size())
		{
			profileQuandityTextItemVector[i]->setText(QString::number(qty));
			profileQuandityTextItemVector[i]->setBrush(QBrush(displayColor));
		}

		if (i < profileQuandityDMSTextItemVector.size())
		{
			if (canUseRealQuantity)
			{
				profileQuandityDMSTextItemVector[i]->setText(QString("(%1)").arg(totalPcs));
				profileQuandityDMSTextItemVector[i]->setBrush(QBrush(displayColor));
			}
			else
			{
				profileQuandityDMSTextItemVector[i]->setText(QString(""));
			}
		}

		if (i < calculatedLayerWeight.size())
		{
			if (i == 0)
			{
				QString weightText;
				if (rowUsesBracketQuantity)
					weightText = QString("(%1kg)").arg(totalRowWeight, 0, 'f', 1);
				else
					weightText = QString("%1kg").arg(totalRowWeight, 0, 'f', 1);

				calculatedLayerWeight[i]->setText(weightText);
				calculatedLayerWeight[i]->setBrush(QBrush(rowDisplayColor));
			}
			else
			{
				calculatedLayerWeight[i]->setText(QString(""));
			}
		}
	}

	for (int i = limit; i < profileQuandityTextItemVector.size(); ++i)
		profileQuandityTextItemVector[i]->setText("");

	for (int i = limit; i < profileQuandityDMSTextItemVector.size(); ++i)
		profileQuandityDMSTextItemVector[i]->setText("");

	for (int i = limit; i < calculatedLayerWeight.size(); ++i)
		calculatedLayerWeight[i]->setText("");


}

void profileLayerClass::resetTotalWidth()
{
	totalWidth = 0.0;
}

int profileLayerClass::getRealBasketId() const
{
	return realBasketId;
}



int profileLayerClass::addProfiles(
	profileProductionClass* p,
	int numberOfProfiles,
	bool forceDrawRequestedQty /* = false */)
{
	if (p == nullptr || numberOfProfiles <= 0)
	{
		return 0;
	}

	// Do not allow different profiles in the same layer.
// If this layer already has a profile, only allow the same profile code.
	if (!profiles.isEmpty() && profiles.first() != p->getProfileCode())
	{
		return 0;
	}

	const int maxLayerWeight = 320;
	const int layerWidth = resolveLayerWidth();

	if ((baseHeight + SPACER_THIKNESS + p->getProfileHeight()) > MAX_HEIGHT_OF_LAYERS_IN_A_BASKET)
	{
		return 0;
	}

	int profilesThatWerePutIntoTheLayer = 0;

	const QPainterPath profilePath = p->getProfilePath();
	const QRectF bounds = profilePath.boundingRect();
	const double profileWidth = p->getProfileWidth();
	const double profileSpacing = p->getProfileSpaceingInLayer();
	const double profileHeight = p->getProfileHeight();
	const double profileLength = p->getProfileLength();
	const double profileWeight = p->getWeightOfOneProfile();

	QVector<QGraphicsPathItem*> addedItems;



	for (int i = 0; i < numberOfProfiles; ++i)
	{
		double offsetForCheck = 0.0;

		if (!profiles.isEmpty())
		{
			offsetForCheck = p->getOffsetToTouchTheNextProfileInTheLayer();

			if (offsetForCheck < 0.0)
			{
				offsetForCheck = 0.0;
			}
		}

		double effectiveWidthForCheck =
			profileWidth - offsetForCheck + profileSpacing;

		if (effectiveWidthForCheck <= 0.0)
		{
			effectiveWidthForCheck = profileWidth + profileSpacing;
		}

		if ((totalWidth + effectiveWidthForCheck) > layerWidth)
		{
			break;
		}

		double dx = totalWidth - 0.5 * layerWidth - bounds.left();
		double dy = baseHeight + SPACER_THIKNESS + bounds.height() - bounds.bottom();
		double offSet = 0.0;

		if (!profiles.isEmpty())
		{
			if (i == 0 && lastProfile != nullptr)
			{
				offSet = getMinXDistanceOfPaths(
					lastProfile->getProfilePath().translated(lastProfileDx, lastProfileDy),
					profilePath.translated(dx, dy), p->getProfileCode(),
					p->getDrawingPath());
			}
			else
			{
				offSet = p->getOffsetToTouchTheNextProfileInTheLayer();
			}

			double sp = profileSpacing;
			if (lastProfile != nullptr && lastProfile->getProfileSpaceingInLayer() > sp)
			{
				sp = lastProfile->getProfileSpaceingInLayer();
			}

			offSet -= sp;
		}

		dx -= offSet;

		lastProfile = p;
		lastProfileDx = dx;
		lastProfileDy = dy;

		const QPainterPath translatedPath = profilePath.translated(dx, dy);
		QGraphicsPathItem* item =
			myScene->addPath(translatedPath, QPen(p->getColor(), 0.0, Qt::SolidLine));
		profileItems << item;

		addedItems << item;
		addToGroup(p, translatedPath);

		++profilesThatWerePutIntoTheLayer;

		totalWidth += profileWidth - offSet;
		totalHeight = qMax(totalHeight, profileHeight);
		maxProfileLength = qMax(maxProfileLength, profileLength);

		if (profileHeight > (maxProfileHeight + 1.0))
		{
			flatSurfase = 0.0;
			maxProfileHeight = profileHeight;
		}

		if (qAbs(profileHeight - maxProfileHeight) < 1.0)
		{
			flatSurfase += profileWidth;
		}

		profiles << p->getProfileCode();
		profilesEpicsTableId << p->getProfilesEpicsTableId();
		profilesColors << p->getColor();
		profilesMultiplicity << p->getTotalProfileMultiplicity();
	}

	totalWeight += profilesThatWerePutIntoTheLayer * profileWeight;

	if (profilesThatWerePutIntoTheLayer > 0)
	{
		addSpacers(p->getSpacers());
	}

	const QHash<int, LayerSummaryInfo> summaryByDie = loadLayerSummaryTotals(p);

	// Get same unbracketed qty logic as updateQuantityLabels:
	// qty = multiplicity * occurrences
	int occurrences = 0;
	int multiplicity = p->getTotalProfileMultiplicity();

	for (int i = 0; i < profilesEpicsTableId.size(); ++i)
	{
		if (profilesEpicsTableId[i] == p->getProfilesEpicsTableId())
		{
			occurrences++;

			if (profilesMultiplicity[i] > multiplicity)
			{
				multiplicity = profilesMultiplicity[i];
			}
		}
	}

	const int qty = multiplicity * occurrences;

	const LayerSummaryInfo info = summaryByDie.value(p->getProfileCode());
	const bool allowBracketedQty = ShouldShowBracketedQuantityForBasketIndex(newbasketnumber);
	const int totalPcs = allowBracketedQty ? info.totalPcs : -1;

	if (info.basketId > 0)
	{
		realBasketId = info.basketId;
	}

	const QColor originalColor = p->getColor();
	const QColor exactMatchColor = QColor(204, 170, 0);
	const QColor underStackColor = QColor("darkgreen"); // old behavior bright purple
	const QColor overStackColor = QColor("darkred");    // dark blue

	int matchingIndex = 0;

	for (int i = 0; i < profilesEpicsTableId.size(); ++i)
	{
		if (profilesEpicsTableId[i] != p->getProfilesEpicsTableId())
		{
			continue;
		}

		QColor itemColor = originalColor;

		if (totalPcs > 0)
		{
			if (qty == totalPcs)
			{
				itemColor = exactMatchColor;
			}
			else if (qty > totalPcs)
			{
				const int profileMultiplier =
					qMax(1, p->getMultiplicityOfProfiles() * (p->getIsDoubleLength() ? 2 : 1));

				const int physicalProfilesToColor =
					static_cast<int>(ceil(static_cast<double>(totalPcs) / profileMultiplier));

				if (matchingIndex < physicalProfilesToColor)
				{
					itemColor = underStackColor;
				}
				else
				{
					itemColor = originalColor;
				}
			}
			else {
				itemColor = overStackColor;
				/*profilesThatWerePutIntoTheLayer = totalPcs;*/
			}
		}

		if (i < profileItems.size() && profileItems[i] != nullptr)
		{
			profileItems[i]->setPen(QPen(itemColor, 0.0, Qt::SolidLine));
		}

		if (i < profilesColors.size())
		{
			profilesColors[i] = itemColor;
		}

		matchingIndex++;
	}


	updateQuantityLabels(p, maxLayerWeight);

	return profilesThatWerePutIntoTheLayer;
}

bool profileLayerClass::hasProfile(int code) const
{
	return profiles.indexOf(code) >= 0;
}

int profileLayerClass::getFirstProfile() const
{
	return profiles.isEmpty() ? 0 : profiles.first();
}

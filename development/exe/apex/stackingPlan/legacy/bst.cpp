#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qnamespace.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qfileinfo.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qsettings.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/kernel/qpalette.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qprogressdialog.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qapplication.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qboxlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qscrollarea.h"

#include "../../common/tcpTux.h"
#include "../Application/Services/MainWindowOrchestrator.h"
#include "../Infrastructure/Database/SqlHelper.h"
#include "bs.h"
#include "bst.h"
#include "mW.h"
#include "pp.h"
#include "ppG.h"
#include "ppl.h"

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qglobal.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qdir.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qcoreapplication.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstringlist.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlabel.h"
#include "LegacyLoggingBridge.h"
#include <cmath>
#include <memory>

namespace
{


	QString GetSoBase(const QString& soNum)
	{
		QStringList parts = soNum.split("-");

		if (parts.size() > 0)
		{
			return parts[0].trimmed();
		}

		return soNum.trimmed();
	}

	bool IsSameSoBase(profileProductionClass* current, profileProductionClass* previous)
	{
		if (current == NULL || previous == NULL)
		{
			return false;
		}

		return GetSoBase(current->getSoNum()) == GetSoBase(previous->getSoNum());
	}


	int GetMaxBasketCountFromIni()
	{
		QString iniPath = QDir(QCoreApplication::applicationDirPath())
			.filePath("app.ini");

		if (!QFileInfo::exists(iniPath))
		{
			// fallback default if ini missing
			return 8;
		}

		QSettings settings(iniPath, QSettings::IniFormat);

		bool ok = false;
		int maxBaskets = settings.value("StackPlan/MaxBasketCount", 8).toInt(&ok);

		if (!ok || maxBaskets <= 0)
		{
			maxBaskets = 8;
		}

		return maxBaskets;
	}

	bool CanCreateAnotherBasket(const QVector<basketClass*>& basketsVector)
	{
		return basketsVector.count() < GetMaxBasketCountFromIni();
	}

	double GetMaxBasketWeight()
	{
		// TODO: replace with DB/config value if needed
		return MAX_WEIGHT_OF_BASKET;
	}

	bool WillBreachBasketWeight(basketClass* basket, profileProductionClass* p, int profilesToPut)
	{
		if (basket == NULL || p == NULL || profilesToPut <= 0)
		{
			return false;
		}

		const double nextWeight = profilesToPut * p->getWeightOfOneProfile();
		const double projectedWeight = basket->getTotalWeight() + nextWeight;

		return projectedWeight > GetMaxBasketWeight();
	}

	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> GetOrchestrator()
	{
		mainWindowClass* mainWindow = mainWindowClass::GetInstance();
		if (mainWindow == NULL)
		{
			return std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator>();
		}

		return mainWindow->getOrchestrator();
	}

	int GetPrivilegeType()
	{
		std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator =
			GetOrchestrator();

		if (!orchestrator)
		{
			return 0;
		}

		return orchestrator->GetPrivilegeType();
	}

	int GetGlobalPressNumber()
	{
		std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator =
			GetOrchestrator();

		if (!orchestrator)
		{
			return 1;
		}

		return orchestrator->GetGlobalPressNumber();
	}

	int GetMaxProfilesPerLayer(profileProductionClass* p)
	{
		if (p == NULL)
		{
			return 0;
		}

		const int multiplicity = p->getTotalProfileMultiplicity();

		if (multiplicity <= 0)
		{
			return 0;
		}

		const double profileWidth = p->getProfileWidth();
		const double profileSpacing = p->getProfileSpaceingInLayer();

		if (profileWidth <= 0.0)
		{
			return multiplicity;
		}

		int layerWidth = MAX_LAYER_WIDTH;

		const int press = GetGlobalPressNumber();

		if (press == 1)
		{
			layerWidth = MAX_LAYER_WIDTH_P1;
		}
		else if (press == 2)
		{
			layerWidth = MAX_LAYER_WIDTH_P2;
		}

		double offset = p->getOffsetToTouchTheNextProfileInTheLayer();

		if (offset < 0.0)
		{
			offset = 0.0;
		}

		double effectiveWidth = profileWidth - offset + profileSpacing;

		if (effectiveWidth <= 0.0)
		{
			effectiveWidth = profileWidth + profileSpacing;
		}

		if (effectiveWidth <= 0.0)
		{
			return multiplicity;
		}

		const int occurrences =
			static_cast<int>(floor(layerWidth / effectiveWidth));

		if (occurrences <= 0)
		{
			return multiplicity;
		}

		return multiplicity * occurrences;
	}

	double GetProfilesDivisor(profileProductionClass* p)
	{
		if (p == NULL)
		{
			return 1.0;
		}

		double divisor =
			p->getMultiplicityOfProfiles() *
			(p->getIsDoubleLength() ? 2.0 : 1.0);

		if (divisor <= 0.0)
		{
			divisor = 1.0;
		}

		return divisor;
	}

	int GetFutureProfilesRemainingFromQueue(profileProductionClass* p)
	{
		if (p == NULL)
		{
			return 0;
		}

		int remainingQty = p->getNumberOfProfilesNotInABasket();

		if (remainingQty < 0)
		{
			remainingQty = 0;
		}

		//qDebug() << "[FutureStack]"
		//	<< "SO:" << p->getSoNum()
		//	<< "Die:" << p->getProfileCode()
		//	<< "RemainingQtyToProduce:" << remainingQty;

		return remainingQty;
	}

	int GetProfilesCountFromPcs(profileProductionClass* p, int pcs)
	{
		double divisor = GetProfilesDivisor(p);
		return static_cast<int>(pcs / divisor);
	}

	int GetPlannedProfilesRemaining(profileProductionClass* p)
	{
		if (p == NULL)
		{
			return 0;
		}

		int remainingPcs = p->getNumberOfProfilesNotInABasket();
		double divisor = GetProfilesDivisor(p);
		int plannedProfiles = static_cast<int>(ceil(remainingPcs / divisor));

		if (plannedProfiles < 0)
		{
			plannedProfiles = 0;
		}

		return plannedProfiles;
	}

	int GetProfilesToDrawForLayer(profileProductionClass* p, int bracketedPcs)
	{
		if (p == NULL)
		{
			return 0;
		}

		// This is the planned quantity PER LAYER, like the old code label: 7
		const int unbracketedProfiles = GetMaxProfilesPerLayer(p);

		// This is the bracketed quantity from LayerSummary for this live layer
		const int bracketedProfiles = bracketedPcs;

		// Rule:
		// 9 (12) => draw 12
		// 16 (5) => draw 16
		// 7 (7)  => draw 7
		return qMax(unbracketedProfiles, bracketedProfiles);
	}

	QVector<int> GetLiveBasketLayerPcs(profileProductionClass* p, int basketNumber)
	{
		QVector<int> layers;

		if (p == NULL)
		{
			return layers;
		}

		int pressNumber = GetGlobalPressNumber();


		QString sql = QString(R"(
			SELECT total_pcs
			FROM StackPlan.dbo.LayerSummary
			WHERE sonum = '%1'
			  AND dienumber = %2
			  AND pressnum = %3
			  AND basket_id = %4
			ORDER BY id asc
		)")
			.arg(p->getSoNum())
			.arg(p->getProfileCode())
			.arg(pressNumber)
			.arg(basketNumber);

		QVector<QVariant> results = SqlHelper::ExecQuery(sql);

		for (int i = 0; i < results.count(); ++i)
		{
			if (results[i].isValid() && !results[i].isNull())
			{
				layers << results[i].toInt();
			}
		}

		return layers;
	}

	bool ShouldUseLiveLayers(profileProductionClass* p, const QVector<int>& liveLayerPcs)
	{
		Q_UNUSED(p);
		return !liveLayerPcs.isEmpty();
	}
	profileProductionClass* FindPreviousProfileByProductionOrder(
		profileProductionClass* currentProfile)
	{
		if (currentProfile == NULL)
		{
			return NULL;
		}

		const int pressNumber = GetGlobalPressNumber();

		QString sql = QString(R"(
			SELECT 
    ep.soNum,
    ep.die,
    ep.productionOrder,
    ep.pressNum,
    ep.temper,

    ISNULL(ep.tid, 0)               AS tid,
    ISNULL(ep.totalQuantity, 0)     AS totalQuantity,
    ISNULL(ep.quantityInBaskets, 0) AS quantityInBaskets,
    ISNULL(ep.linearDencity, 0)     AS linearDencity,
    ISNULL(ep.length, 0)            AS profileLength,
    ISNULL(ep.doubleLength, 0)      AS doubleLength,
    ISNULL(ep.status, 0)            AS status,
    ISNULL(ep.onNextBasket, 0)      AS onNextBasket,
    ISNULL(ep.onNextLayer, 0)       AS onNextLayer,
    ISNULL(ep.scheduleDate, '')     AS scheduleDate,
    ISNULL(ep.scheduleShift, 1)     AS scheduleShift
FROM StackPlan.dbo.epicsProduction2 ep
WHERE ep.pressNum = %1
  AND ep.soNum = (
        SELECT TOP (1)
            CONCAT(SONUM, '-', SOITEMNUM, '-', LOT)
        FROM [SAE01PRESS0%1].[APEX].[dbo].[SAW_JOB]
        WHERE START_DT IS NOT NULL
          AND STOP_DT IS NOT NULL
        ORDER BY STOP_DT DESC
  );
		)")
			.arg(pressNumber);

		QVector<QVariant> result = SqlHelper::ExecQuery(sql);

		if (result.count() < 16)
		{
			return NULL;
		}

		profileProductionClass* profile =
			new profileProductionClass(
				GetOrchestrator(),
				result[5].toInt(),        // tid
				1,
				result[2].toInt(),        // productionOrder
				result[1].toInt(),        // die
				result[6].toInt(),        // totalQuantity
				result[7].toInt(),        // quantityInBaskets
				result[8].toDouble(),     // linearDencity
				result[9].toInt(),        // length
				result[10].toInt(),       // doubleLength
				result[11].toInt(),       // status
				result[12].toInt(),       // onNextBasket
				result[13].toInt(),       // onNextLayer
				result[4].toString(),     // temper
				result[0].toString(),     // soNum
				result[14].toString(),    // scheduleDate
				result[15].toInt(),       // scheduleShift
				false,
				0,
				0.0,
				0.0,
				0.0,
				0.0,
				0.0,
				1.0,
				QVector<QVariant>(),
				QVector<QVariant>()
			);
		return profile;
	}
}

basketStackingClass::basketStackingClass(QWidget* p)
	: QWidget(p)
{
	tcpServer = NULL;
	stackerPlcWordsVector.resize(21);
	stackerPlcCombsWord = 0;

	mainScrollArea = new QScrollArea();
	mainScrollArea->getContentsMargins(0, 0, 0, 0);
	mainScrollArea->setAlignment(Qt::AlignCenter);
	mainScrollArea->setBackgroundRole(QPalette::Dark);

	QHBoxLayout* topLayout = new QHBoxLayout();
	topLayout->setMargin(0);
	setLayout(topLayout);

	QVBoxLayout* contentLayout = new QVBoxLayout();

	alertLabel = new QLabel();

	alertLabel->setVisible(false);

	alertLabel->setStyleSheet(
		"QLabel {"
		"background-color: #fff3cd;"
		"color: #856404;"
		"border: 1px solid #ffeeba;"
		"border-radius: 6px;"
		"padding: 8px;"
		"font-weight: bold;"
		"}"
	);

	contentLayout->addWidget(alertLabel);
	contentLayout->addWidget(mainScrollArea);

	topLayout->addLayout(contentLayout);

	mainScrollAreaWidget = new QWidget();
	mainScrollAreaWidget->getContentsMargins(0, 0, 0, 0);

	mainLayout = new QHBoxLayout();
	mainLayout->setMargin(0);
	mainScrollAreaWidget->setLayout(mainLayout);

	mainScrollArea->setWidgetResizable(true);
	mainScrollArea->setWidget(mainScrollAreaWidget);

	setNeedsToBeCalculated(1);
}

basketStackingClass::~basketStackingClass()
{
	clearAllBaskets();
}

QVector<basketClass*> basketStackingClass::getBasketsVector()
{
	return basketsVector;
}

void basketStackingClass::setTcpServer(tcpServerClass* tcpServer_)
{
	tcpServer = tcpServer_;
	for (int i = 0; i < basketsVector.count(); i++)
	{
		basketsVector[i]->setTcpServer(tcpServer);
	}
}

void basketStackingClass::stackerPlcMemoryChanged(QString table, int index, QString value)
{
	if ((table == "XOUT") && (index >= 1) && (index <= 20))
	{
		stackerPlcWordsVector[index] = value.toInt();
	}

	if ((table == "XOUT") && (index == 16))
	{
		stackerPlcCombsWord = value.toInt();
	}

	if (basketsVector.count() > 0)
	{
		basketsVector[0]->setStackerPlcWords(stackerPlcWordsVector);
		basketsVector[0]->setStackerCombsWord(stackerPlcCombsWord);
	}
}

basketClass* basketStackingClass::getFirstVisible()
{
	for (int i = 0; i < basketsVector.count(); i++)
	{
		if (!basketsVector[i]->visibleRegion().isEmpty())
		{
			return basketsVector[i];
		}
	}

	return NULL;
}

void basketStackingClass::setVisibleBasketWithProfileTableId(int tableId)
{
	for (int i = 0; i < basketsVector.count(); i++)
	{
		if (basketsVector[i]->hasProfileTableId(tableId))
		{
			mainScrollArea->ensureWidgetVisible(basketsVector[i]);
			blockSignals(true);
			basketClassSelected(basketsVector[i], true);
			blockSignals(false);
			break;
		}
	}
}

void basketStackingClass::setVisibleBasketWithProfile(int profile)
{
	for (int i = 0; i < basketsVector.count(); i++)
	{
		if (basketsVector[i]->hasProfile(profile))
		{
			mainScrollArea->ensureWidgetVisible(basketsVector[i]);
			blockSignals(true);
			basketClassSelected(basketsVector[i], true);
			blockSignals(false);
			break;
		}
	}
}

void basketStackingClass::basketClassSelected(basketClass* b, bool selected)
{
	for (int i = 0; i < basketsVector.count(); i++)
	{
		if (selected && (basketsVector[i] == b))
		{
			basketsVector[i]->setIsSelected(true);
			emit basketSelectedSignal(basketsVector[i]);
		}
		else
		{
			basketsVector[i]->setIsSelected(false);
		}
	}
}

void basketStackingClass::calculateLayout(profileProductionLayoutClass* profileProductionLayout)
{
	alertLabel->clear();
	alertLabel->setVisible(false);
	if (needsToBeCalculated)
	{
		LAYER_MIN_SUPORT_LENGTH =
			SqlHelper::GetFirst(QString("select value from settings where name='namelayerMinSuportLength'")).toInt();

		QVector<profileProductionClass*> ppv = profileProductionLayout->getProfileProductionVector();

		QProgressDialog progress(tr("Calculating baskets."), tr("Cancel"), 0, ppv.count(), this);
		progress.setWindowModality(Qt::WindowModal);
		progress.setMinimumWidth(600);
		progress.show();

		clearAllBaskets();

		if (!CanCreateAnotherBasket(basketsVector))
		{
			progress.setValue(ppv.count());
			setNeedsToBeCalculated(progress.wasCanceled(), 1);
			return;
		}

		appendBasket(new basketClass(basketsVector.count(), true, this));

		int addedBaskets = basketsVector.count();

		for (int i = 0; i < ppv.count(); i++)
		{
			if (ppv[i] == NULL)
			{
				progress.setValue(i);
				QApplication::processEvents();
				continue;
			}

			int futureProfilesRemaining = GetFutureProfilesRemainingFromQueue(ppv[i]);

			const bool sameSoAsPrevious =
				(i > 0) && IsSameSoBase(ppv[i], ppv[i - 1]);

			bool shouldForceNewBasket =
				!sameSoAsPrevious &&
				(
					(ppv[i]->getIsToBePutOnNextBasket() && (basketsVector.last()->getTotalWeight() > 0.0)) ||
					((i > 0) && (ppv[i]->getTemperType() != ppv[i - 1]->getTemperType()))
					);

			if (shouldForceNewBasket && futureProfilesRemaining > 0)
			{
				if (CanCreateAnotherBasket(basketsVector))
				{
					appendBasket(new basketClass(basketsVector.count(), false, this));
					addedBaskets = basketsVector.count();
				}
				else
				{
					break;
				}
			}

			int pressNumber = GetGlobalPressNumber();

			QString basketSql = QString(R"(
				SELECT TOP 1 basket_id
				FROM StackPlan.dbo.LayerSummary
				WHERE sonum = '%1'
				  AND dienumber = %2
				  AND pressnum = %3
				AND basket_id IS NOT NULL
				AND basket_id > 0
				ORDER BY id DESC
			)")
				.arg(ppv[0]->getSoNum())
				.arg(ppv[0]->getProfileCode())
				.arg(pressNumber);

			QVariant basketVar = SqlHelper::GetFirst(basketSql);

			int currentBasketId = 0;

			if (basketVar.isValid() && !basketVar.isNull())
			{
				currentBasketId = basketVar.toInt();
			}
			QVector<int> liveLayerPcs = GetLiveBasketLayerPcs(ppv[i], currentBasketId);
			if (i == 0) {
				profileProductionClass* previousProfile =
					FindPreviousProfileByProductionOrder(ppv[i]);

				if (previousProfile != NULL)
				{
					QVector<int> previousLiveLayerPcs =
						GetLiveBasketLayerPcs(previousProfile, currentBasketId);

					if (ShouldUseLiveLayers(previousProfile, previousLiveLayerPcs))
					{
						int previousTotalQtyInThisBasket = 0;

						for (int x = 0; x < previousLiveLayerPcs.count(); ++x)
						{
							previousTotalQtyInThisBasket += previousLiveLayerPcs[x];
						}

						alertLabel->setText(
							QString(
								"INFO: Previous profile was already added to the current live basket and is being drawn for operator layout reference. "
								"Real Basket ID: %1 | Previous SO: %2 | Previous Profile: %3 | Total Quantity in This Basket: %4"
							)
							.arg(currentBasketId)
							.arg(previousProfile->getSoNum())
							.arg(previousProfile->getProfileCode())
							.arg(previousTotalQtyInThisBasket)
						);

						alertLabel->setStyleSheet(
							"QLabel {"
							"background-color: #e7f3ff;"
							"color: #084298;"
							"border: 1px solid #b6d4fe;"
							"border-radius: 6px;"
							"padding: 8px;"
							"font-weight: bold;"
							"}"
						);

						alertLabel->setVisible(true);
						for (int prevIndex = 0; prevIndex < previousLiveLayerPcs.count(); ++prevIndex)
						{
							if (previousLiveLayerPcs[prevIndex] <= 0)
							{
								continue;
							}

							if (prevIndex > 0)
							{
								basketsVector.last()->addEmptyLayerForProfiles(previousProfile, false);
							}

							int profilesToPut = previousLiveLayerPcs[prevIndex];

							if (profilesToPut <= 0)
							{
								continue;
							}

							int added =
								basketsVector.last()->addProfiles(previousProfile, profilesToPut);

							if (added <= 0)
							{
								break;
							}
						}

						if (!liveLayerPcs.isEmpty())
						{
							basketsVector.last()->addEmptyLayerForProfiles(ppv[i], false);
						}
					}
				}
			}
			// =========================
			// 1) DRAW LIVE / BRACKETED LAYERS FIRST
			// =========================
			int totalQuantityInThisBasket = 0;
			int excessLiveQty = 0;
			if (ShouldUseLiveLayers(ppv[i], liveLayerPcs))
			{
				for (int liveIndex = 0; liveIndex < liveLayerPcs.count(); ++liveIndex)
				{
					if (liveLayerPcs[liveIndex] != 0)
					{
						int profilesToPut =
							GetProfilesToDrawForLayer(ppv[i], liveLayerPcs[liveIndex]);

						if (profilesToPut <= 0)
						{
							continue;
						}

						int added = basketsVector.last()->addProfiles(ppv[i], profilesToPut);
						totalQuantityInThisBasket += liveLayerPcs[liveIndex];
						ppv[i]->setCurrentBasketQuantity(totalQuantityInThisBasket);
						if (added <= 0)
						{
							// Do not show alert if this is not a real live basket.
							// currentBasketId = APEX/LayerSummary basket id.
							// basketsVector count/title = visual StackPlan basket number.
							if (currentBasketId <= 0)
							{
								continue;
							}

							excessLiveQty += liveLayerPcs[liveIndex];

							alertLabel->setText(
								QString(
									"ALERT: Live basket space exceeded. "
									"Real Basket ID: %1 | SO: %2 | Profile: %3 | "
									"Live Qty: %4"
								)
								.arg(currentBasketId)
								.arg(ppv[i]->getSoNum())
								.arg(ppv[i]->getProfileCode())
								.arg(excessLiveQty)
							);

							alertLabel->setVisible(true);
							LegacyBridge::LogError(
								QString("Unable to add additional profiles to the current live basket because the remaining profiles no longer fit within the available screen space. "
									"No new basket will be created. Real Basket ID: %1, SO: %2, Profile: %3, Live Qty: %4 "
									"The quantity will still be considered according to the operator's actual stacking judgment.")
								.arg(currentBasketId)
								.arg(ppv[i]->getSoNum())
								.arg(ppv[i]->getProfileCode())
								.arg(excessLiveQty));


						}
					}
				}
			}



			int futureSafetyCounter = 0;

			const int futureTargetQty = futureProfilesRemaining;
			int futurePlannedQty = 0;
			const int maxPerLayer = GetMaxProfilesPerLayer(ppv[i]);
			const int remaininglayer = futureProfilesRemaining / maxPerLayer;
			int ctr = 0;
			int remainingToPlan = futureTargetQty - futurePlannedQty;
			int actualMaxPerLayer = maxPerLayer;
			const int profileMultiplier =
				qMax(1, ppv[i]->getMultiplicityOfProfiles() *
					(ppv[i]->getIsDoubleLength() ? 2 : 1));
			while (remainingToPlan >= maxPerLayer)
			{

				int added = basketsVector.last()->addProfiles(ppv[i], actualMaxPerLayer);

				if (added > 0 && added < actualMaxPerLayer)
				{
					actualMaxPerLayer = added; // example: 26 becomes 25
				}

				if (added <= 0)
				{
					if (!CanCreateAnotherBasket(basketsVector))
					{
						break;
					}

					appendBasket(new basketClass(basketsVector.count(), false, this));
					addedBaskets = basketsVector.count();

					added = basketsVector.last()->addProfiles(ppv[i], actualMaxPerLayer);

					if (added <= 0)
					{
						break;
					}
				}

				futurePlannedQty += (added * profileMultiplier);
				remainingToPlan = futureTargetQty - futurePlannedQty;

			}






			if (remainingToPlan > 0) {
				//basketsVector.last()->addEmptyLayerForProfiles(ppv[i], false);
				int added = basketsVector.last()->addProfiles(
					ppv[i],
					static_cast<int>(
						ceil(
							static_cast<double>(remainingToPlan) /
							static_cast<double>(profileMultiplier))));

				if (added <= 0)
				{
					if (!CanCreateAnotherBasket(basketsVector))
					{
						break;
					}

					appendBasket(new basketClass(basketsVector.count(), false, this));
					addedBaskets = basketsVector.count();

					added = added = basketsVector.last()->addProfiles(
						ppv[i],
						static_cast<int>(
							ceil(
								static_cast<double>(remainingToPlan) /
								static_cast<double>(profileMultiplier))));

					if (added <= 0)
					{
						break;
					}
				}
			}


			if (progress.wasCanceled())
				break;

			progress.setValue(i);

			SqlHelper::ExecNonQuery(
				QString("update epicsProduction2 set baskets=%1 where soNum='%2'")
				.arg(addedBaskets)
				.arg(ppv[i]->getSoNum()));

			QApplication::processEvents();
		}

		progress.setValue(ppv.count());
		setNeedsToBeCalculated(progress.wasCanceled(), 1);

		while (!basketsVector.isEmpty() && basketsVector.last()->getTotalWeight() <= 0.0)
		{
			basketClass* emptyBasket = basketsVector.takeLast();
			mainLayout->removeWidget(emptyBasket);
			delete emptyBasket;
		}

		addedBaskets = basketsVector.count();

		for (int i = 0; i < basketsVector.count(); i++)
		{
			basketsVector[i]->calculateGroups();
		}
	}

	if (basketsVector.count() > 0)
	{
		basketsVector[0]->sendDatabaseSpacerDataToPlc(1);
		basketsVector[0]->setStackerPlcWords(stackerPlcWordsVector);
		basketsVector[0]->setStackerCombsWord(stackerPlcCombsWord);
	}

	if (basketsVector.count() > 1)
	{
		basketsVector[1]->sendDatabaseSpacerDataToPlc(21);
	}
}

void basketStackingClass::setNeedsToBeCalculated(int c, int force)
{
	if (force || (needsToBeCalculated != c))
	{
		needsToBeCalculated = c;
		for (int i = 0; i < basketsVector.count(); i++)
		{
			basketsVector[i]->setNeedsToBeCalculated(needsToBeCalculated);
			basketsVector[i]->setPrivileges(needsToBeCalculated ? 0 : GetPrivilegeType());
		}
	}
}

void basketStackingClass::clearAllBaskets()
{
	for (int i = 0; i < basketsVector.count(); ++i)
	{
		basketClass* b = basketsVector[i];
		if (b)
		{
			mainLayout->removeWidget(b);
			b->setParent(NULL);
			delete b;
		}
	}

	basketsVector.clear();

	if (mainLayout)
	{
		mainLayout->activate();
	}

	updateGeometry();
	update();
	repaint();
}

void basketStackingClass::appendBasket(basketClass* b)
{
	connect(b, SIGNAL(selectedSignal(basketClass*, bool)), this, SLOT(basketClassSelected(basketClass*, bool)));
	b->setTcpServer(tcpServer);
	b->setFixedWidth(this->width());
	basketsVector << b;
	mainLayout->addWidget(b);
}
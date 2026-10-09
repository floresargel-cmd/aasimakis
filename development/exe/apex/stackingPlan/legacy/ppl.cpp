#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qnamespace.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qfile.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstringlist.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qprogressdialog.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qapplication.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qboxlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qsizepolicy.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qframe.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlabel.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qscrollarea.h"

#include "../Application/Services/MainWindowOrchestrator.h"
#include "../Infrastructure/database/SqlHelper.h"
#include "../Infrastructure/Repositories/LegacyProductionRepository.h"
#include "LegacyGlobalState.h"
#include "LegacyLoggingBridge.h"
#include "pp.h"
#include "ppG.h"
#include "ppl.h"

#include "../Infrastructure/Repositories/ProductionRow.h"
#include <cmath>
#include <memory>

profileProductionLayoutClass::profileProductionLayoutClass() : QWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setContentsMargins(0, 0, 0, 0);
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet(
		"QWidget { background-color: #ffffff; }"
		"QScrollArea { background-color: #ffffff; border: none; }"
	);

	QVBoxLayout* topLayout = new QVBoxLayout();
	topLayout->setContentsMargins(0, 0, 0, 0);
	topLayout->setSpacing(0);
	setLayout(topLayout);

	mainScrollArea = new QScrollArea();
	mainScrollArea->setContentsMargins(0, 0, 0, 0);
	mainScrollArea->setWidgetResizable(true);
	mainScrollArea->setFrameShape(QFrame::NoFrame);
	mainScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	mainScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	topLayout->addWidget(mainScrollArea);

	QWidget* mainScrollAreaWidget = new QWidget();
	mainScrollAreaWidget->setContentsMargins(0, 0, 0, 0);
	mainScrollAreaWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	mainLayout = new QVBoxLayout();
	mainLayout->setContentsMargins(8, 8, 8, 8);
	mainLayout->setSpacing(10);
	mainScrollAreaWidget->setLayout(mainLayout);
	mainScrollArea->setWidget(mainScrollAreaWidget);

	setNeedsToBeLoadedFromDatabase(true);
	moreProductionAvailable = NULL;
	_orchestrator.reset();
}

profileProductionLayoutClass::~profileProductionLayoutClass()
{
	clearAllProfileProduction();
}

void profileProductionLayoutClass::loadFromDatabase()
{
	if (!needsToBeLoadedFromDatabase)
	{
		return;
	}

	LegacyGlobals::GlobalColorIndex = -1;
	clearAllProfileProduction();

	const int pressNumber = _orchestrator
		? _orchestrator->GetGlobalPressNumber()
		: LegacyGlobals::GlobalPressNumber;

	const QVector<ProductionRow> productionRows =
		LegacyProductionRepository::GetScheduledProductionByPress(pressNumber);

	QStringList dxfFilesNotThere;
	for (int i = 0; i < productionRows.count(); ++i)
	{
		const QString profileFName =
			dxfDirPath +
			QString("%1//%2.dxf")
			.arg((int)(floor(productionRows[i].die / 100) * 100))
			.arg(productionRows[i].die);

		if (!QFile(profileFName).exists())
		{
			if (!dxfFilesNotThere.contains(profileFName))
			{
				dxfFilesNotThere << profileFName;
			}
		}
	}

	if (dxfFilesNotThere.count() > 0)
	{
		QString msg;
		for (int i = 0; i < dxfFilesNotThere.count(); ++i)
		{
			msg += dxfFilesNotThere[i] + "\n";
		}

		// intentionally kept non-fatal like your current version
		// LegacyBridge::LogWarning(QString("Could not locate dxf file for the following profiles:\n%1").arg(msg));
	}

	const QVector<QVariant> productionTemperDescription =
		LegacyProductionRepository::GetTemperDescriptions();
	const QVector<QVariant> productionTemperType =
		LegacyProductionRepository::GetTemperTypes();

	QProgressDialog progress(
		tr("Loading planned production from database..."),
		tr("Cancel"),
		0,
		productionRows.count(),
		this);

	progress.setWindowModality(Qt::WindowModal);
	progress.setMinimumWidth(600);
	progress.show();

	for (int i = 0; (i < MAX_NO_OF_PROFILES_TO_LOAD) && (i < productionRows.count()); ++i)
	{
		const ProductionRow& row = productionRows[i];

		progress.setLabelText(
			QString("Loading planned production from database. profile:%1").arg(row.die));

		LegacyProductionRepository::EnsureProfileCodeExists(
			row.die,
			row.profileSpacing,
			row.inComb);

		progress.setValue(i);
		QApplication::processEvents();

		appendProfileProduction(
			new profileProductionClass(
				_orchestrator,
				row.tid,
				i == 0 ? true : false,
				row.productionOrder,
				row.die,
				row.totalQuantity,
				row.quantityInBaskets,
				row.linearDencity,
				row.length,
				row.doubleLength,
				row.status,
				row.onNextBasket,
				row.onNextLayer,
				row.temper.trimmed(),
				row.soNum,
				row.scheduleDate,
				row.scheduleShift,
				row.isVirtual,
				row.billetsNo,
				row.billetLength,
				row.billetButtLength,
				row.scrapFrontLength,
				row.scrapRearLength,
				row.runOutLength,
				row.dieMultiplicity,
				productionTemperDescription,
				productionTemperType));

		if (progress.wasCanceled())
		{
			break;
		}
	}

	if (MAX_NO_OF_PROFILES_TO_LOAD < productionRows.count())
	{
		mainLayout->addWidget(
			moreProductionAvailable =
			new QLabel(
				tr("Showing only %1 of %2 available scheduled profiles\npress Ctrl+O to change the maximum number of profiles to load")
				.arg(MAX_NO_OF_PROFILES_TO_LOAD)
				.arg(productionRows.count())));
	}

	progress.setValue(productionRows.count());
	setNeedsToBeLoadedFromDatabase(progress.wasCanceled(), true);
}

profileProductionClass* profileProductionLayoutClass::getFirstVisible()
{
	for (int i = 0; i < profileProductionVector.count(); i++)
	{
		if (!profileProductionVector[i]->visibleRegion().isEmpty())
			return profileProductionVector[i];
	}
	return NULL;
}

void profileProductionLayoutClass::setVisibleProfile(int profile)
{
	for (int i = 0; i < profileProductionVector.count(); i++)
	{
		if (profileProductionVector[i]->getProfileCode() == profile)
		{
			mainScrollArea->ensureWidgetVisible(profileProductionVector[i]);
			blockSignals(true);
			profileProductionClassSelected(profileProductionVector[i], true);
			blockSignals(false);
		}
	}
}

void profileProductionLayoutClass::clearAllProfileProduction()
{
	for (int i = 0; i < profileProductionVector.count(); ++i)
	{
		profileProductionClass* p = profileProductionVector[i];
		if (p)
		{
			mainLayout->removeWidget(p);
			p->setParent(NULL);
			delete p;
		}
	}

	profileProductionVector.clear();

	if (moreProductionAvailable)
	{
		mainLayout->removeWidget(moreProductionAvailable);
		delete moreProductionAvailable;
		moreProductionAvailable = NULL;
	}

	if (mainLayout)
		mainLayout->activate();

	updateGeometry();
	update();
	repaint();
}

void profileProductionLayoutClass::appendProfileProduction(profileProductionClass* p)
{
	profileProductionVector << p;
	connect(p, SIGNAL(selectedSignal(profileProductionClass*, bool)), this, SLOT(profileProductionClassSelected(profileProductionClass*, bool)));
	connect(p, SIGNAL(productionChangedSignal(bool)), this, SLOT(productionChangedSlot(bool)));
	mainLayout->addWidget(p);
}

void profileProductionLayoutClass::productionChangedSlot(bool needsReloadFromDatabase)
{
	emit productionChangedSignal(needsReloadFromDatabase);
}

QVector<profileProductionClass*> profileProductionLayoutClass::getProfileProductionVector()
{
	return profileProductionVector;
}

profileProductionClass* profileProductionLayoutClass::getSelectedProfileProduction()
{
	profileProductionClass* ans = NULL;
	for (int i = 0; i < profileProductionVector.count(); i++)
	{
		if (profileProductionVector[i]->getIsSelected())
		{
			ans = profileProductionVector[i];
			break;
		}
	}
	return ans;
}

void profileProductionLayoutClass::moveProfileLeft()
{
	int index = -1;
	for (int i = 0; i < profileProductionVector.count(); i++)
	{
		if (profileProductionVector[i]->getIsSelected())
		{
			index = i;
			break;
		}
	}

	if ((index >= 0) && (index != 0))
	{
		if (index == 1)
			needsToBeLoadedFromDatabase = true;

		profileProductionClass* slected = profileProductionVector[index];
		int leftProductionOrder = profileProductionVector[index - 1]->getProductionOrder();
		int rightProductionOrder = profileProductionVector[index]->getProductionOrder();

		profileProductionVector[index - 1]->setProductionOrder(rightProductionOrder);
		profileProductionVector[index]->setProductionOrder(leftProductionOrder);

		profileProductionVector[index] = profileProductionVector[index - 1];
		profileProductionVector[index - 1] = slected;

		mainScrollArea->ensureWidgetVisible(slected);

		for (int i = 0; i < profileProductionVector.count(); i++)
			mainLayout->removeWidget(profileProductionVector[i]);

		for (int i = 0; i < profileProductionVector.count(); i++)
			mainLayout->addWidget(profileProductionVector[i]);

		emit productionChangedSignal(false);
	}
}

void profileProductionLayoutClass::moveProfileRight()
{
	int index = -1;
	for (int i = 0; i < profileProductionVector.count(); i++)
	{
		if (profileProductionVector[i]->getIsSelected())
		{
			index = i;
			break;
		}
	}

	if ((index >= 0) && (index != (profileProductionVector.count() - 1)))
	{
		if (index == 0)
			needsToBeLoadedFromDatabase = true;

		profileProductionClass* slected = profileProductionVector[index];
		if (slected->getProductionStatus() != PRODUCTION_STATUS_SCHEDULED)
			return;

		int leftProductionOrder = profileProductionVector[index]->getProductionOrder();
		int rightProductionOrder = profileProductionVector[index + 1]->getProductionOrder();

		profileProductionVector[index + 1]->setProductionOrder(leftProductionOrder);
		profileProductionVector[index]->setProductionOrder(rightProductionOrder);

		profileProductionVector[index] = profileProductionVector[index + 1];
		profileProductionVector[index + 1] = slected;

		mainScrollArea->ensureWidgetVisible(slected);

		for (int i = 0; i < profileProductionVector.count(); i++)
			mainLayout->removeWidget(profileProductionVector[i]);

		for (int i = 0; i < profileProductionVector.count(); i++)
			mainLayout->addWidget(profileProductionVector[i]);

		emit productionChangedSignal(false);
	}
}

void profileProductionLayoutClass::deleteProfile()
{
	int index = -1;
	for (int i = 0; i < profileProductionVector.count(); i++)
	{
		if (profileProductionVector[i]->getIsSelected())
		{
			index = i;
			break;
		}
	}

	if (index >= 0)
	{
		profileProductionClass* slected = profileProductionVector[index];
		SqlHelper::ExecNonQuery(
			QString("delete from epicsProduction2 where tid=%1")
			.arg(slected->getProfilesEpicsTableId()));

		LegacyBridge::LogInfo(
			QString("user deleted production with for die:%1 and tid%2")
			.arg(slected->getProfileCode())
			.arg(slected->getProfilesEpicsTableId()));
	}
}

void profileProductionLayoutClass::profileProductionClassSelected(profileProductionClass* pp, bool selected)
{
	for (int i = 0; i < profileProductionVector.count(); i++)
	{
		if (selected && (profileProductionVector[i] == pp))
		{
			profileProductionVector[i]->setIsSelected(true);
			emit productionProfileSelectedSignal(profileProductionVector[i]);
		}
		else
		{
			profileProductionVector[i]->setIsSelected(false);
		}
	}
}

bool profileProductionLayoutClass::getNeedsToBeLoadedFromDatabase()
{
	return needsToBeLoadedFromDatabase;
}

void profileProductionLayoutClass::setNeedsToBeLoadedFromDatabase(int c, int force)
{
	if (force || (needsToBeLoadedFromDatabase != c))
	{
		needsToBeLoadedFromDatabase = c;
		for (int i = 0; i < profileProductionVector.count(); i++)
		{
			profileProductionVector[i]->setNeedsToBeLoadedFromDatabase(needsToBeLoadedFromDatabase);
			profileProductionVector[i]->setPrivileges(needsToBeLoadedFromDatabase ? 0 : LegacyGlobals::PrivilegeType);
		}
	}
}

void profileProductionLayoutClass::setOrchestrator(std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator)
{
	_orchestrator = orchestrator;
}

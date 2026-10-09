#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/image/qicon.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/kernel/qpalette.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qcolor.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qdialog.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qmessagebox.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qboxlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qgridlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlabel.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qpushbutton.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qspinbox.h"
#include "../../common/apexG.h"
#include "../Application/Services/MainWindowOrchestrator.h"
#include "bs.h"
#include "ls.h"
#include "mW.h"
#include "ppG.h"
#include "sp.h"
#include <climits>
#include <memory>
#include <tuxG.h>

namespace
{
	/// <summary>
	/// Retrieves the application orchestrator from the main window singleton.
	/// </summary>
	/// <returns>
	/// A shared pointer to the <see cref="MainWindowOrchestrator"/>, or null if unavailable.
	/// </returns>
	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> GetOrchestrator()
	{
		if (mainWindowClass::GetInstance() == NULL)
		{
			return std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator>();
		}

		return mainWindowClass::GetInstance()->getOrchestrator();
	}
}

/// <summary>
/// Initializes a new instance of the <see cref="defineSpacersClass"/> dialog.
/// </summary>
/// <param name="basketDBTableId_">The database identifier of the basket.</param>
/// <param name="b">The basket object used to populate spacer data.</param>
/// <remarks>
/// Builds the UI layout, initializes controls, and loads spacer data
/// from the provided basket.
/// </remarks>
defineSpacersClass::defineSpacersClass(int basketDBTableId_, basketClass* b)
	: QDialog(b)
{
	setWindowTitle(tr("Define basket spacers.."));
	setWindowIcon(QIcon(":/logo_16x16.png"));
	this->setStyleSheet(QString("QLabel{font-size:%1px} QPushButton{font-size:%1px} QCheckBox{font-size:%1px} QSpinBox{font-size:%1px}").arg(20));

	basketDBTableId = basketDBTableId_;

	QVBoxLayout* mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	QHBoxLayout* basketNumberLayout = new QHBoxLayout();
	mainLayout->addLayout(basketNumberLayout);

	QHBoxLayout* midLayout = new QHBoxLayout();
	mainLayout->addLayout(midLayout);

	basketNumberLayout->addWidget(new QLabel(tr("Basket number:")));
	basketNumberSpinBox = new QSpinBox(this);
	basketNumberLayout->addWidget(basketNumberSpinBox);

	basketNumberSpinBox->blockSignals(true);
	basketNumberSpinBox->setMinimum(1);
	basketNumberSpinBox->setMaximum(INT_MAX);
	basketNumberSpinBox->setValue(basketDBTableId);
	basketNumberSpinBox->blockSignals(false);

	layersLayout = new QVBoxLayout();
	midLayout->addLayout(layersLayout);

	profilesQuandityLayout = new QGridLayout();
	midLayout->addLayout(profilesQuandityLayout);

	confirmBtn = new QPushButton(tr("Confirm"), this);

	/// <summary>
	/// Handles confirmation click event.
	/// </summary>
	connect(confirmBtn, SIGNAL(clicked()), this, SLOT(confirmBtnSlot()));

	mainLayout->addWidget(confirmBtn);
	mainLayout->addStretch(1);

	updateSpacersFromBasket(b);
}

/// <summary>
/// Destructor for <see cref="defineSpacersClass"/>.
/// </summary>
defineSpacersClass::~defineSpacersClass()
{
}

/// <summary>
/// Populates spacer UI controls based on basket data.
/// </summary>
/// <param name="b">The basket containing spacer and layer data.</param>
/// <remarks>
/// Builds layer spacer widgets, links them together, and initializes
/// profile quantities based on basket and orchestrator data.
/// </remarks>
void defineSpacersClass::updateSpacersFromBasket(basketClass* b)
{
	int combsWord = b->getStackerPlcCombsWord();
	QVector<bool> combsVector = deprecatedCalculateBits(combsWord);

	for (int i = 0; i < MAX_LAYERS_IN_A_BASKET; i++)
	{
		layerOfSpacersClass* l;

#ifdef USE_SPACERS_INFO_FROM_PLC
		l = new layerOfSpacersClass(i, combsVector[i], b->getStackerPlcWord(i));
#else
		l = new layerOfSpacersClass(i, b->getDatabaseWord(i));
#endif

		layerOfSpacersClassVector << l;
	}

	for (int i = (layerOfSpacersClassVector.count() - 1); i >= 0; i--)
	{
		layersLayout->addWidget(layerOfSpacersClassVector[i]);
	}

	layerOfSpacersClassVector[0]->setNextLayerClass(layerOfSpacersClassVector[1]);

	for (int i = 1; i < layerOfSpacersClassVector.count() - 1; i++)
	{
		layerOfSpacersClassVector[i]->setNextLayerClass(layerOfSpacersClassVector[i + 1]);
		layerOfSpacersClassVector[i]->setPreviousLayerClass(layerOfSpacersClassVector[i - 1]);
	}

	layerOfSpacersClassVector.last()->setPreviousLayerClass(
		layerOfSpacersClassVector[layerOfSpacersClassVector.count() - 2]);

	layerOfSpacersClassVector[0]->validate();

	QVector<int> tableIds;
	QVector<int> codesIds;
	QVector<QColor> colors;
	QVector<int> profilesMultiplicity;

	for (int i = 0; i < b->getNumberOfLayers(); i++)
	{
		tableIds << b->getLayer(i)->getProfilesEpicsTableId();
		codesIds << b->getLayer(i)->getProfilesCodesTableId();
		colors << b->getLayer(i)->getProfilesColors();
		profilesMultiplicity << b->getLayer(i)->getProfilesMultiplicity();
	}

	QVector<int> uniqeTableIds;
	QVector<int> uniqeCodesIds;
	QVector<QColor> uniqeColors;
	QVector<int> uniqeProfilesMultiplicity;

	for (int i = 0; i < tableIds.count(); i++)
	{
		if (!uniqeTableIds.contains(tableIds[i]))
		{
			uniqeTableIds << tableIds[i];
			uniqeCodesIds << codesIds[i];
			uniqeColors << colors[i];
			uniqeProfilesMultiplicity << profilesMultiplicity[i];
		}
	}

	profilesQuandityLayout->setRowStretch(0, 1);
	profilesQuandityLayout->addWidget(new QLabel(tr("Die")), 1, 0);
	profilesQuandityLayout->addWidget(new QLabel(tr("Quantity")), 1, 1);

	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator = GetOrchestrator();

	for (int i = 0; i < uniqeTableIds.count(); i++)
	{
		QSpinBox* pSpin = new QSpinBox();
		pSpin->setMinimum(0);
		pSpin->setMaximum(INT_MAX);
		pSpin->setValue(uniqeCodesIds[i]);

		QPalette pSpinPalette(pSpin->palette());
		pSpinPalette.setColor(QPalette::Active, QPalette::Base, uniqeColors[i]);
		pSpinPalette.setColor(QPalette::Active, QPalette::Text, QColor(255, 255, 255));
		pSpin->setPalette(pSpinPalette);

		profileCodesSpinBoxVector << pSpin;
		epicsTableIdVector << uniqeTableIds[i];
		profilesQuandityLayout->addWidget(pSpin, uniqeTableIds.count() - i + 2, 0);

		QSpinBox* qSpin = new QSpinBox();
		qSpin->setMinimum(0);
		qSpin->setMaximum(INT_MAX);

		int no = uniqeProfilesMultiplicity[i] * tableIds.count(uniqeTableIds[i]);
		profileQuantityThatIsNeededAccordingToTheLayout << no;

		int noFromEpics = 0;
		if (orchestrator)
		{
			noFromEpics = orchestrator->GetPendingBasketQuantityByTidAndBasket(
				uniqeTableIds[i],
				basketNumberSpinBox->value());
		}

		if (noFromEpics != 0)
		{
			no = noFromEpics;
		}

		qSpin->setValue(no);

		QPalette qSpinPalette(qSpin->palette());
		qSpinPalette.setColor(QPalette::Active, QPalette::Base, uniqeColors[i]);
		qSpinPalette.setColor(QPalette::Active, QPalette::Text, QColor(255, 255, 255));
		qSpin->setPalette(pSpinPalette);

		profileQuantitySpinBoxVector << qSpin;
		profilesQuandityLayout->addWidget(qSpin, uniqeTableIds.count() - i + 2, 1);
	}
}

/// <summary>
/// Gets the selected basket number.
/// </summary>
/// <returns>The basket identifier.</returns>
int defineSpacersClass::getBasketNumber()
{
	return basketNumberSpinBox->value();
}

/// <summary>
/// Handles confirmation of spacer configuration.
/// </summary>
/// <remarks>
/// Persists spacer definitions and updates production quantities
/// through the application orchestrator.
/// </remarks>
void defineSpacersClass::confirmBtnSlot()
{
	auto mw = mainWindowClass::GetInstance();
	auto orchestrator = mw ? mw->getOrchestrator() : nullptr;

	if (!orchestrator)
	{
		QMessageBox::critical(this, "Error", "Orchestrator not available");
		return;
	}

	int basketId = basketNumberSpinBox->value();

	if (!orchestrator->DeleteBasketSpacers(basketId))
	{
		QMessageBox::critical(this, "Error", "Failed to clear spacers");
		return;
	}

	for (int i = 0; i < layerOfSpacersClassVector.count(); i++)
	{
		auto& layer = layerOfSpacersClassVector[i];

		if (!layer->isActive()) continue;

		if (!orchestrator->InsertBasketSpacer(
			basketId,
			i + 1,
			layer->getIsOnComb(),
			layer->getSp1(),
			layer->getSp2(),
			layer->getSp3(),
			layer->getSp4(),
			layer->getSp5(),
			layer->getSp6(),
			layer->getSp7(),
			layer->getSp8()))
		{
			QMessageBox::critical(this, "Error", "Insert spacer failed");
			return;
		}
	}

	for (int i = 0; i < profileCodesSpinBoxVector.count(); i++)
	{
		if (profileQuantitySpinBoxVector[i]->value() <= 0) continue;

		int tid = epicsTableIdVector[i];
		int qty = profileQuantitySpinBoxVector[i]->value();

		if (!orchestrator->IncrementQuantityInBaskets(tid, qty))
		{
			QMessageBox::critical(this, "Error", "Update quantity failed");
			return;
		}
	}

	orchestrator->MarkUsersNeedsUpdate();

	accept();
}

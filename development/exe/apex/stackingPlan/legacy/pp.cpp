#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qnamespace.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qdir.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qcoreevent.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qline.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/image/qicon.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/kernel/qevent.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qbrush.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qcolor.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpainterpath.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpen.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qmessagebox.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsitem.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsscene.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qboxlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qgridlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qcheckbox.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qcombobox.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qgroupbox.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlabel.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qpushbutton.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qradiobutton.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qspinbox.h"

#include "../Application/Services/MainWindowOrchestrator.h"
#include "../Infrastructure/Database/SqlHelper.h"
#include "dxf.h"
#include "dxfGV.h"
#include "LegacyGlobalState.h"
#include "pp.h"
#include "ppG.h"

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qglobal.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qtransform.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qabstractspinbox.h"
#include <cfloat>
#include <climits>
#include <cmath>
#include <memory>

#include "../Infrastructure/Logging/FileLogger.h"
#include <exception>

using namespace StackingPlan::Infrastructure::Logging;

// Singleton-style (simple)
static FileLogger _logger("");

namespace
{
	struct ProfileCodeData
	{
		int interProfileSpace = 0;
		int multiplicity = 1;
		int inComb = 0;
		int sp1 = 0;
		int sp2 = 0;
		int sp3 = 0;
		int sp4 = 0;
		int sp5 = 0;
		int sp6 = 0;
		int sp7 = 0;
		int sp8 = 0;
		double angle = 0.0;
	};
	static QLabel* CreateFormLabel(const QString& text)
	{
		QLabel* label = new QLabel(text);
		label->setEnabled(true);
		label->setStyleSheet(
			"QLabel {"
			"  color: #374151;"
			"  background: transparent;"
			"  border: none;"
			"}"
			"QLabel:disabled {"
			"  color: #374151;"
			"  background: transparent;"
			"  border: none;"
			"}");
		return label;
	}

	static ProfileCodeData GetProfileCodeData(int die)
	{
		ProfileCodeData data;

		const QVector<QVariant> row = SqlHelper::ExecQuery(
			QString(
				"select interProfileSpace, multiplicity, inComb, "
				"sp1, sp2, sp3, sp4, sp5, sp6, sp7, sp8, angle "
				"from profileCode where die=%1")
			.arg(die));

		if (row.count() >= 12)
		{
			data.interProfileSpace = row[0].toInt();
			data.multiplicity = row[1].toInt();
			data.inComb = row[2].toInt();
			data.sp1 = row[3].toInt();
			data.sp2 = row[4].toInt();
			data.sp3 = row[5].toInt();
			data.sp4 = row[6].toInt();
			data.sp5 = row[7].toInt();
			data.sp6 = row[8].toInt();
			data.sp7 = row[9].toInt();
			data.sp8 = row[10].toInt();
			data.angle = row[11].toDouble();
		}

		return data;
	}

	static void EnsureProfileCodeRowExists(int die)
	{
		SqlHelper::ExecNonQuery(
			QString(
				"if not exists (select die from profileCode where die=%1) "
				"insert into profileCode "
				"(die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) "
				"values (%1,0,0,1,0,0,0,0,0,0,0,0,0)")
			.arg(die));
	}

	static void UpdateProfileCodeInt(const QString& fieldName, int die, int value)
	{
		EnsureProfileCodeRowExists(die);

		SqlHelper::ExecNonQuery(
			QString("update profileCode set %1=%2 where die=%3")
			.arg(fieldName)
			.arg(value)
			.arg(die));
	}

	static void UpdateProfileCodeDouble(const QString& fieldName, int die, double value)
	{
		EnsureProfileCodeRowExists(die);

		SqlHelper::ExecNonQuery(
			QString("update profileCode set %1=%2 where die=%3")
			.arg(fieldName)
			.arg(QString::number(value, 'f', 6))
			.arg(die));
	}

	static void UpdateEpicsProductionInt(const QString& fieldName, int tid, int value)
	{
		SqlHelper::ExecNonQuery(
			QString("update epicsProduction2 set %1=%2 where tid=%3")
			.arg(fieldName)
			.arg(value)
			.arg(tid));
	}

	static void UpdateEpicsProductionDouble(const QString& fieldName, int tid, double value)
	{
		SqlHelper::ExecNonQuery(
			QString("update epicsProduction2 set %1=%2 where tid=%3")
			.arg(fieldName)
			.arg(QString::number(value, 'f', 6))
			.arg(tid));
	}

	static void UpdateEpicsProductionString(const QString& fieldName, int tid, const QString& value)
	{
		QString escaped = value;
		escaped.replace("'", "''");

		SqlHelper::ExecNonQuery(
			QString("update epicsProduction2 set %1='%2' where tid=%3")
			.arg(fieldName)
			.arg(escaped)
			.arg(tid));
	}
}

profileProductionClass::profileProductionClass(
	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator_,
	int tableId_,
	int isFirstInList,
	int productionOrder_,
	int profileCode_,
	int totalProfileQuantity_,
	int inBasketsProfileQuantity_,
	double linearDencity_,
	int profileLength_,
	int doubleLength_,
	int productionStaus_,
	int onNextBasket_,
	int onNextLayer_,
	QString temper_,
	QString soNum_,
	QString scheduleDate,
	int scheduleShift,
	bool isVirtual,
	int numberOfBilletsProduced,
	double billetLength,
	double billetButtLength,
	double scrapFrontLength,
	double scrapRearLength,
	double runOutLength,
	double dieMultiplicity,
	QVector<QVariant> temperDescriptionVector,
	QVector<QVariant> temperTypeVector)
	: QWidget()
{
	_orchestrator = orchestrator_;
	setAutoFillBackground(true);

	isSelected = false;
	offsetToTouchTheNextProfileInTheLayer = 0.0;
	offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated = true;
	soNum = soNum_;
	tableId = tableId_;
	productionOrder = productionOrder_;
	profileCode = profileCode_;
	inBasketsProfileQuantity = inBasketsProfileQuantity_;
	isVirtualEntryInDatabase = isVirtual;
	needsToBeLoadedFromDatabase = false;

	int productionStaus = productionStaus_;
	int onNextBasket = onNextBasket_;
	int onNextLayer = onNextLayer_;
	QString temper = temper_;
	int profileLength = profileLength_;
	int doubleLength = doubleLength_;
	double linearDencity = linearDencity_;

	const ProfileCodeData profileData = GetProfileCodeData(profileCode);

	int interProfileSpace = profileData.interProfileSpace;
	int multiplicity = profileData.multiplicity;
	int inComb = profileData.inComb;
	int sp1 = profileData.sp1;
	int sp2 = profileData.sp2;
	int sp3 = profileData.sp3;
	int sp4 = profileData.sp4;
	int sp5 = profileData.sp5;
	int sp6 = profileData.sp6;
	int sp7 = profileData.sp7;
	int sp8 = profileData.sp8;
	double angle = profileData.angle;

	int totalProfileQuantity = totalProfileQuantity_;
	if ((numberOfBilletsProduced > 0) && (billetLength > 0.0))
	{
		double totalWeight = numberOfBilletsProduced * (billetLength - billetButtLength) * 0.0871;
		double weightOfRunOut = runOutLength * 0.001 * dieMultiplicity * linearDencity;
		int numberOfRunOuts = (weightOfRunOut > 0.0) ? static_cast<int>(totalWeight / weightOfRunOut) : 0;
		int numberOfProfilesPerRunOut = (profileLength > 0)
			? static_cast<int>(dieMultiplicity * (runOutLength - scrapFrontLength - scrapRearLength) / profileLength)
			: 0;

		totalProfileQuantity = numberOfRunOuts * numberOfProfilesPerRunOut;
		UpdateEpicsProductionInt("totalQuantity", tableId, totalProfileQuantity);
	}

	QVBoxLayout* mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	QString shiftStr;
	switch (scheduleShift)
	{
	case 1: shiftStr = "S_1"; break;
	case 2: shiftStr = "S_2"; break;
	case 3: shiftStr = "S_3"; break;
	default: break;
	}

	Q_UNUSED(shiftStr);

	mainLayout->addWidget(
		CreateFormLabel(QString("SO: %1 Die: %2").arg(soNum).arg(profileCode)),
		0,
		Qt::AlignHCenter);

	mainLayout->setContentsMargins(0, 0, 0, 0);

	myScene = new QGraphicsScene(this);
	myQGraphicsView = new dxfQGraphicsViewClass(this, 50.0, 1.0);
	myQGraphicsView->setMinimumSize(200, 200);
	myQGraphicsView->scale(1.0, 1.0);
	myQGraphicsView->setMaximumWidth(320);

	QBrush b = QBrush(Qt::SolidPattern);
	b.setColor(QColor(230, 230, 230));
	myQGraphicsView->setBackgroundBrush(b);
	myQGraphicsView->setScene(myScene);
	mainLayout->addWidget(myQGraphicsView, 0, Qt::AlignHCenter);

	QTransform previewTransform = myQGraphicsView->transform();
	previewTransform.scale(1.0, -1.0);
	myQGraphicsView->setTransform(previewTransform);

	QPen pen = QPen(Qt::DashLine);
	pen.setColor(QColor(150, 150, 150));
	pen.setWidthF(0.0);
	QGraphicsLineItem* zeroXLine = new QGraphicsLineItem(QLine(-30.0, 0.0, 30.0, 0.0));
	zeroXLine->setPen(pen);
	myScene->addItem(zeroXLine);

	QGroupBox* statusGroupBox = new QGroupBox(tr("status"));
	QGridLayout* statusGroupBoxLayout = new QGridLayout();
	statusGroupBox->setLayout(statusGroupBoxLayout);

	statusGroupBoxLayout->addWidget(
		productionStatusScheduledRadioBtn = new QRadioButton(tr("Scheduled")), 0, 0);
	if (productionStaus == PRODUCTION_STATUS_SCHEDULED)
		productionStatusScheduledRadioBtn->setChecked(true);
	connect(productionStatusScheduledRadioBtn, SIGNAL(clicked(bool)), this, SLOT(productionStatusScheduledRadioBtnSlot(bool)));

	statusGroupBoxLayout->addWidget(
		productionStatusOnPressRadioBtn = new QRadioButton(tr("Press")), 0, 1);
	if (productionStaus == PRODUCTION_STATUS_ON_PRESS)
		productionStatusOnPressRadioBtn->setChecked(true);
	connect(productionStatusOnPressRadioBtn, SIGNAL(clicked(bool)), this, SLOT(productionStatusOnPressRadioBtnSlot(bool)));

	statusGroupBoxLayout->addWidget(
		productionStatusFinishedRadioBtn = new QRadioButton(tr("Finished")), 0, 2);
	connect(productionStatusFinishedRadioBtn, SIGNAL(clicked(bool)), this, SLOT(productionStatusFinishedRadioBtnSlot(bool)));

	statusGroupBoxLayout->setMargin(1);

	QGridLayout* quantityLayout = new QGridLayout();
	mainLayout->addLayout(quantityLayout);

	quantityLayout->addWidget(CreateFormLabel(tr("Total Order Quantity")), 0, 0);
	totalQuantitySpinBox = new QSpinBox();
	quantityLayout->addWidget(totalQuantitySpinBox, 0, 1);
	totalQuantitySpinBox->setMinimum(0);
	totalQuantitySpinBox->setMaximum(INT_MAX);
	totalQuantitySpinBox->blockSignals(true);
	totalQuantitySpinBox->setValue(totalProfileQuantity);
	totalQuantitySpinBox->blockSignals(false);
	connect(totalQuantitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(totalQuantitySpinBoxSlot(int)));

	quantityInBasketsSpinBox = NULL;
	totalRQuantitySpinBox = NULL;

	if (isFirstInList)
	{
		const int diff = totalProfileQuantity - inBasketsProfileQuantity;
		const bool isOverProduced = (diff < 0);
		const int displayRemainingOrOver = qAbs(diff);


		totalRemainingLabel = CreateFormLabel(
			isOverProduced ? tr("Over Produce Quantity") : tr("Remaining Quantity To Produce"));
		quantityLayout->addWidget(totalRemainingLabel, 1, 0);

		totalRQuantitySpinBox = new QSpinBox();
		quantityLayout->addWidget(totalRQuantitySpinBox, 1, 1);
		totalRQuantitySpinBox->setMinimum(0);
		totalRQuantitySpinBox->setMaximum(INT_MAX);
		totalRQuantitySpinBox->setReadOnly(true);
		totalRQuantitySpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
		totalRQuantitySpinBox->blockSignals(true);
		totalRQuantitySpinBox->setValue(displayRemainingOrOver);
		totalRQuantitySpinBox->blockSignals(false);

		quantityLayout->addWidget(CreateFormLabel(tr("Total Quantity Across All Baskets")), 2, 0);
		quantityInBasketsSpinBox = new QSpinBox();
		quantityLayout->addWidget(quantityInBasketsSpinBox, 2, 1);
		quantityInBasketsSpinBox->setMinimum(0);
		quantityInBasketsSpinBox->setMaximum(INT_MAX);
		quantityInBasketsSpinBox->blockSignals(true);
		quantityInBasketsSpinBox->setValue(inBasketsProfileQuantity);
		quantityInBasketsSpinBox->blockSignals(false);
		connect(quantityInBasketsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(quantityInBasketsSpinBoxSlot(int)));

		// ==========================================
// Total Quantity in This Basket
// ==========================================

		quantityLayout->addWidget(
			CreateFormLabel(tr("Total Quantity in This Basket")), 3, 0);

		currentBasketQuantitySpinBox = new QSpinBox();

		quantityLayout->addWidget(currentBasketQuantitySpinBox, 3, 1);

		currentBasketQuantitySpinBox->setMinimum(0);
		currentBasketQuantitySpinBox->setMaximum(INT_MAX);
		currentBasketQuantitySpinBox->setReadOnly(true);
		currentBasketQuantitySpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

		currentBasketQuantitySpinBox->blockSignals(true);
		currentBasketQuantitySpinBox->setValue(0);
		currentBasketQuantitySpinBox->blockSignals(false);
	}


	moreToolButton = new QPushButton();
	moreToolButton->setIcon(QIcon(":/plus.png"));
	mainLayout->addWidget(moreToolButton, Qt::AlignRight);
	moreToolButton->setFlat(true);
	moreToolButton->setMaximumWidth(10);
	moreToolButton->setMaximumHeight(10);
	connect(moreToolButton, SIGNAL(clicked(bool)), this, SLOT(morePlusToolButtonSlot(bool)));

	moreWidget = new QWidget();
	mainLayout->addWidget(moreWidget);

	QVBoxLayout* moreWidgetLayout = new QVBoxLayout();
	moreWidget->setLayout(moreWidgetLayout);
	moreWidgetLayout->setMargin(1);

	QGridLayout* dataLayout = new QGridLayout();
	moreWidgetLayout->addLayout(dataLayout);

	dataLayout->addWidget(onNextBasketCheckBtn = new QCheckBox(tr("On next basket")), 0, 0);
	if (onNextBasket == 1)
		onNextBasketCheckBtn->setChecked(true);
	connect(onNextBasketCheckBtn, SIGNAL(stateChanged(int)), this, SLOT(onNextBasketCheckBtnSlot(int)));

	dataLayout->addWidget(onNextLayerCheckBtn = new QCheckBox(tr("On next layer")), 0, 1);
	if (onNextLayer == 1)
		onNextLayerCheckBtn->setChecked(true);
	connect(onNextLayerCheckBtn, SIGNAL(stateChanged(int)), this, SLOT(onNextLayerCheckBtnSlot(int)));

	dataLayout->addWidget(CreateFormLabel(tr("Temper")), 1, 0);
	dataLayout->addWidget(temperComboBox = new QComboBox(), 1, 1);
	for (int i = 0; i < temperDescriptionVector.count(); i++)
		temperComboBox->addItem(temperDescriptionVector[i].toString().trimmed());

	int currentIndex = temperDescriptionVector.indexOf(temper);
	if (currentIndex >= 0)
		temperComboBox->setCurrentIndex(currentIndex);
	connect(temperComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(temperComboBoxSlot(int)));
	if (!isVirtualEntryInDatabase)
		temperComboBox->setEnabled(false);

	dataLayout->addWidget(CreateFormLabel(tr("Length")), 2, 0);
	lengthSpinBox = new QSpinBox();
	dataLayout->addWidget(lengthSpinBox, 2, 1);
	lengthSpinBox->setMinimum(1);
	lengthSpinBox->setMaximum(INT_MAX);
	lengthSpinBox->blockSignals(true);
	lengthSpinBox->setValue(profileLength);
	lengthSpinBox->blockSignals(false);
	connect(lengthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(lengthSpinBoxSlot(int)));
	if (!isVirtualEntryInDatabase)
		lengthSpinBox->setEnabled(false);

	dataLayout->addWidget(CreateFormLabel(tr("Die Weight")), 3, 0);
	linearDencitySpinBox = new QDoubleSpinBox();
	dataLayout->addWidget(linearDencitySpinBox, 3, 1);
	linearDencitySpinBox->setMinimum(0.1);
	linearDencitySpinBox->setMaximum(DBL_MAX);
	linearDencitySpinBox->blockSignals(true);
	linearDencitySpinBox->setValue(linearDencity);
	linearDencitySpinBox->blockSignals(false);
	connect(linearDencitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(linearDencitySpinBoxSlot(double)));
	if (!isVirtualEntryInDatabase)
		linearDencitySpinBox->setEnabled(false);

	dataLayout->addWidget(CreateFormLabel(tr("Double")), 4, 0);
	doubleLengthBox = new QCheckBox(tr(""));
	dataLayout->addWidget(doubleLengthBox, 4, 1);
	if ((doubleLength == PROFILE_SINGLE_LENGTH) || (doubleLength == PROFILE_DOUBLE_LENGTH))
		doubleLengthBox->setChecked(doubleLength == PROFILE_DOUBLE_LENGTH);
	else if (profileLength < 3352.8)
		doubleLengthBox->setChecked(true);
	connect(doubleLengthBox, SIGNAL(stateChanged(int)), this, SLOT(doubleLengthBoxSlot(int)));

	dataLayout->addWidget(CreateFormLabel(tr("Angle")), 5, 0);
	angleSpinBox = new QDoubleSpinBox();
	dataLayout->addWidget(angleSpinBox, 5, 1);
	angleSpinBox->setMinimum(-180.0);
	angleSpinBox->setMaximum(180.0);
	angleSpinBox->blockSignals(true);
	angleSpinBox->setValue(angle);
	angleSpinBox->blockSignals(false);
	connect(angleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(angleSpinBoxSlot(double)));

	dataLayout->addWidget(CreateFormLabel(tr("Profile spacing")), 7, 0);
	interProfileSpaceSpinBox = new QSpinBox();
	dataLayout->addWidget(interProfileSpaceSpinBox, 7, 1);
	interProfileSpaceSpinBox->setMinimum(0);
	interProfileSpaceSpinBox->setMaximum(INT_MAX);
	interProfileSpaceSpinBox->blockSignals(true);
	interProfileSpaceSpinBox->setValue(interProfileSpace);
	interProfileSpaceSpinBox->blockSignals(false);
	connect(interProfileSpaceSpinBox, SIGNAL(valueChanged(int)), this, SLOT(interProfileSpaceSpinBoxSlot(int)));
	dataLayout->addWidget(CreateFormLabel(tr("Profile multiplicity")), 8, 0);
	multiplicitySpinBox = new QSpinBox();
	dataLayout->addWidget(multiplicitySpinBox, 8, 1);
	multiplicitySpinBox->setMinimum(1);
	multiplicitySpinBox->setMaximum(INT_MAX);
	multiplicitySpinBox->blockSignals(true);
	multiplicitySpinBox->setValue(multiplicity);
	multiplicitySpinBox->blockSignals(false);
	connect(multiplicitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(multiplicitySpinBoxSlot(int)));

	QGroupBox* spacersGroupBox = new QGroupBox(tr("Spacers"));
	moreWidgetLayout->addWidget(spacersGroupBox);

	QVBoxLayout* spacersGroupBoxLayout = new QVBoxLayout();
	spacersGroupBox->setLayout(spacersGroupBoxLayout);
	spacersGroupBoxLayout->setMargin(1);

	inCombCheckBox = new QCheckBox(tr("In comb"));
	spacersGroupBoxLayout->addWidget(inCombCheckBox);
	inCombCheckBox->setChecked(inComb > 0 ? true : false);
	connect(inCombCheckBox, SIGNAL(stateChanged(int)), this, SLOT(inCombCheckBoxSlot(int)));

	QGridLayout* spacersGroupBoxDownLayout = new QGridLayout();
	spacersGroupBoxLayout->addLayout(spacersGroupBoxDownLayout);

	QPushButton* calculateSpacersBtn = new QPushButton();
	calculateSpacersBtn->setIcon(QIcon(":/calculator.png"));
	spacersGroupBoxDownLayout->addWidget(calculateSpacersBtn, 0, 0, 2, 1, Qt::AlignCenter);
	connect(calculateSpacersBtn, SIGNAL(clicked(bool)), this, SLOT(calculateSpacersBtnSlot(bool)));

	spacersGroupBoxDownLayout->addWidget(CreateFormLabel(tr("1")), 0, 1, Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(CreateFormLabel(tr("2")), 0, 2, Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(CreateFormLabel(tr("3")), 0, 3, Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(CreateFormLabel(tr("4")), 0, 4, Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(CreateFormLabel(tr("5")), 0, 5, Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(CreateFormLabel(tr("6")), 0, 6, Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(CreateFormLabel(tr("7")), 0, 7, Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(CreateFormLabel(tr("8")), 0, 8, Qt::AlignHCenter);

	spacersGroupBoxDownLayout->addWidget(sp1Box = new QCheckBox(), 1, 1, Qt::AlignHCenter);
	sp1Box->setChecked(sp1 == 1 ? true : false);
	connect(sp1Box, SIGNAL(stateChanged(int)), this, SLOT(sp1BoxSlot(int)));

	spacersGroupBoxDownLayout->addWidget(sp2Box = new QCheckBox(), 1, 2, Qt::AlignHCenter);
	sp2Box->setChecked(sp2 == 1 ? true : false);
	connect(sp2Box, SIGNAL(stateChanged(int)), this, SLOT(sp2BoxSlot(int)));

	spacersGroupBoxDownLayout->addWidget(sp3Box = new QCheckBox(), 1, 3, Qt::AlignHCenter);
	sp3Box->setChecked(sp3 == 1 ? true : false);
	connect(sp3Box, SIGNAL(stateChanged(int)), this, SLOT(sp3BoxSlot(int)));

	spacersGroupBoxDownLayout->addWidget(sp4Box = new QCheckBox(), 1, 4, Qt::AlignHCenter);
	sp4Box->setChecked(sp4 == 1 ? true : false);
	connect(sp4Box, SIGNAL(stateChanged(int)), this, SLOT(sp4BoxSlot(int)));

	spacersGroupBoxDownLayout->addWidget(sp5Box = new QCheckBox(), 1, 5, Qt::AlignHCenter);
	sp5Box->setChecked(sp5 == 1 ? true : false);
	connect(sp5Box, SIGNAL(stateChanged(int)), this, SLOT(sp5BoxSlot(int)));

	spacersGroupBoxDownLayout->addWidget(sp6Box = new QCheckBox(), 1, 6, Qt::AlignHCenter);
	sp6Box->setChecked(sp6 == 1 ? true : false);
	connect(sp6Box, SIGNAL(stateChanged(int)), this, SLOT(sp6BoxSlot(int)));

	spacersGroupBoxDownLayout->addWidget(sp7Box = new QCheckBox(), 1, 7, Qt::AlignHCenter);
	sp7Box->setChecked(sp7 == 1 ? true : false);
	connect(sp7Box, SIGNAL(stateChanged(int)), this, SLOT(sp7BoxSlot(int)));

	spacersGroupBoxDownLayout->addWidget(sp8Box = new QCheckBox(), 1, 8, Qt::AlignHCenter);
	sp8Box->setChecked(sp8 == 1 ? true : false);
	connect(sp8Box, SIGNAL(stateChanged(int)), this, SLOT(sp8BoxSlot(int)));

	spacersGroupBoxDownLayout->setMargin(0);
	mainLayout->addStretch(1);
	moreWidget->hide();

	color = getGlobalColor();

	QString dxfDir = dxfDirPath;
	if (!QDir().exists(dxfDir))
		dxfDir = QDir::currentPath() + "./../../productionPlanDxfs/";

	if (!QDir().exists(dxfDir))
	{
		QMessageBox::critical(
			this,
			"Error",
			QString("The dxf files dir of the profiles '%1' could not be located!").arg(dxfDir));
		return;
	}

	drawingPath = QString("%1//%2.dxf")
		.arg((int)(floor(profileCode / 100) * 100))
		.arg(profileCode);

	profileGraphics = new dxfGroupOfGraphicsItemClass(
		dxfDir + drawingPath,
		angle,
		true,
		color);

	myScene->addItem(profileGraphics);
	profileGraphics->setFlag(QGraphicsItem::ItemIsMovable, false);

	calculateOffsetToTouchTheNextProfileInTheLayer();

	if ((sp1 == 0) && (sp2 == 0) && (sp3 == 0) && (sp4 == 0) &&
		(sp5 == 0) && (sp6 == 0) && (sp7 == 0) && (sp8 == 0))
	{
		calculateSpacers();
	}

	setPrivileges(LegacyGlobals::PrivilegeType);
}

void profileProductionClass::setCurrentBasketQuantity(int qty)
{
	if (currentBasketQuantitySpinBox.isNull())
	{
		return;
	}

	currentBasketQuantitySpinBox->blockSignals(true);
	currentBasketQuantitySpinBox->setValue(qty);
	currentBasketQuantitySpinBox->blockSignals(false);
}

profileProductionClass::~profileProductionClass()
{
	delete profileGraphics;
}

QString profileProductionClass::getSoNum()
{
	return soNum;
}

dxfGroupOfGraphicsItemClass* profileProductionClass::getProfileGraphics()
{
	return profileGraphics;
}

void profileProductionClass::setSpacers(bool sp1, bool sp2, bool sp3, bool sp4, bool sp5, bool sp6, bool sp7, bool sp8)
{
	sp1Box->setChecked(sp1);
	sp2Box->setChecked(sp2);
	sp3Box->setChecked(sp3);
	sp4Box->setChecked(sp4);
	sp5Box->setChecked(sp5);
	sp6Box->setChecked(sp6);
	sp7Box->setChecked(sp7);
	sp8Box->setChecked(sp8);
}

QVector<bool> profileProductionClass::getSpacers()
{
	QVector<bool> ans;
	ans << (sp1Box->isChecked() ? true : false);
	ans << (sp2Box->isChecked() ? true : false);
	ans << (sp3Box->isChecked() ? true : false);
	ans << (sp4Box->isChecked() ? true : false);
	ans << (sp5Box->isChecked() ? true : false);
	ans << (sp6Box->isChecked() ? true : false);
	ans << (sp7Box->isChecked() ? true : false);
	ans << (sp8Box->isChecked() ? true : false);
	return ans;
}

void profileProductionClass::calculateSpacers()
{
	if (doubleLengthBox->isChecked())
	{
		if (getProfileLength() < 3000)
			setSpacers(0, 1, 1, 0, 0, 1, 1, 0);
		else
			setSpacers(1, 1, 0, 1, 1, 0, 0, 1);
	}
	else
	{
		if (getProfileLength() < 4110)
			setSpacers(0, 0, 1, 1, 0, 1, 0, 0);
		else if (getProfileLength() < 5750)
			setSpacers(0, 1, 0, 1, 1, 0, 1, 0);
		else
			setSpacers(1, 0, 1, 0, 0, 1, 0, 1);
	}
}

void profileProductionClass::setNeedsToBeLoadedFromDatabase(int needsToBeLoadedFromDatabase_)
{
	if (needsToBeLoadedFromDatabase != needsToBeLoadedFromDatabase_)
	{
		needsToBeLoadedFromDatabase = needsToBeLoadedFromDatabase_;
		if (needsToBeLoadedFromDatabase)
		{
			QBrush b = QBrush(Qt::BDiagPattern);
			b.setColor(QColor(150, 150, 150));
			myQGraphicsView->setBackgroundBrush(b);
		}
		else
		{
			QBrush b = QBrush(Qt::SolidPattern);
			b.setColor(QColor(230, 230, 230));
			myQGraphicsView->setBackgroundBrush(b);
		}
	}
}

void profileProductionClass::setPrivileges(int privilegeType)
{
	if (!(privilegeType & PRIVILEGE_TYPE_PRESS))
		productionStatusOnPressRadioBtn->setEnabled(false);
	else
		productionStatusOnPressRadioBtn->setEnabled(true);

	if (!(privilegeType & PRIVILEGE_TYPE_STACKER))
		productionStatusFinishedRadioBtn->setEnabled(false);
	else
		productionStatusFinishedRadioBtn->setEnabled(true);

	if ((!(privilegeType & PRIVILEGE_TYPE_OFFICE)) || (!(privilegeType & PRIVILEGE_TYPE_STACKER)))
		totalQuantitySpinBox->setEnabled(false);
	else
		totalQuantitySpinBox->setEnabled(true);

	if (quantityInBasketsSpinBox)
	{
		if (!(privilegeType & PRIVILEGE_TYPE_STACKER))
			quantityInBasketsSpinBox->setEnabled(false);
		else
			quantityInBasketsSpinBox->setEnabled(true);

		if (!(privilegeType & PRIVILEGE_TYPE_OFFICE))
			onNextBasketCheckBtn->setEnabled(false);
	}
	else
	{
		onNextBasketCheckBtn->setEnabled(true);
	}

	if (!(privilegeType & PRIVILEGE_TYPE_OFFICE))
		onNextLayerCheckBtn->setEnabled(false);
	else
		onNextLayerCheckBtn->setEnabled(true);

	if (!(privilegeType & PRIVILEGE_TYPE_OFFICE))
		temperComboBox->setEnabled(false);
	else if (isVirtualEntryInDatabase)
		temperComboBox->setEnabled(true);

	if (!(privilegeType & PRIVILEGE_TYPE_OFFICE))
		lengthSpinBox->setEnabled(false);
	else if (isVirtualEntryInDatabase)
		lengthSpinBox->setEnabled(true);

	if (!(privilegeType & PRIVILEGE_TYPE_OFFICE))
		linearDencitySpinBox->setEnabled(false);
	else if (isVirtualEntryInDatabase)
		linearDencitySpinBox->setEnabled(true);

	if (!(privilegeType & PRIVILEGE_TYPE_OFFICE))
		doubleLengthBox->setEnabled(false);
	else
		doubleLengthBox->setEnabled(true);

	if (!(privilegeType & PRIVILEGE_TYPE_OFFICE))
		angleSpinBox->setEnabled(false);
	else
		angleSpinBox->setEnabled(true);

	if (!(privilegeType & PRIVILEGE_TYPE_OFFICE))
		multiplicitySpinBox->setEnabled(false);
	else
		multiplicitySpinBox->setEnabled(true);

	if (!(privilegeType & PRIVILEGE_TYPE_OFFICE))
		interProfileSpaceSpinBox->setEnabled(false);
	else
		interProfileSpaceSpinBox->setEnabled(true);

	if (!(privilegeType & PRIVILEGE_TYPE_OFFICE))
		inCombCheckBox->setEnabled(false);
	else
		inCombCheckBox->setEnabled(true);

	if (!(privilegeType & PRIVILEGE_TYPE_OFFICE))
	{
		sp1Box->setEnabled(false);
		sp2Box->setEnabled(false);
		sp3Box->setEnabled(false);
		sp4Box->setEnabled(false);
		sp5Box->setEnabled(false);
		sp6Box->setEnabled(false);
		sp7Box->setEnabled(false);
		sp8Box->setEnabled(false);
	}
	else
	{
		sp1Box->setEnabled(true);
		sp2Box->setEnabled(true);
		sp3Box->setEnabled(true);
		sp4Box->setEnabled(true);
		sp5Box->setEnabled(true);
		sp6Box->setEnabled(true);
		sp7Box->setEnabled(true);
		sp8Box->setEnabled(true);
	}
}

void profileProductionClass::enterEvent(QEvent* ev)
{
	Q_UNUSED(ev);
	setStyleSheet("background-color: #f3f4f6;");
}

void profileProductionClass::leaveEvent(QEvent* ev)
{
	Q_UNUSED(ev);

	if (isSelected)
		setStyleSheet("background-color: #dbeafe;");
	else
		setStyleSheet("background-color: transparent;");
}

void profileProductionClass::mousePressEvent(QMouseEvent* ev)
{
	Q_UNUSED(ev);
	if (isSelected)
		emit selectedSignal(this, false);
	else
		emit selectedSignal(this, true);
}

void profileProductionClass::setIsSelected(bool s)
{
	isSelected = s;

	if (isSelected)
		setStyleSheet("background-color: #dbeafe;");
	else
		setStyleSheet("background-color: transparent;");
}

bool profileProductionClass::getIsSelected()
{
	return isSelected;
}

int profileProductionClass::getProfileCode()
{
	return profileCode;
}

int profileProductionClass::getProfilesEpicsTableId()
{
	return tableId;
}

int profileProductionClass::getTotalProfileMultiplicity()
{
	return multiplicitySpinBox->value() * (doubleLengthBox->isChecked() ? 2.0 : 1.0);
}

void profileProductionClass::setProductionOrder(int po)
{
	productionOrder = po;
	UpdateEpicsProductionInt("productionOrder", tableId, productionOrder);
}

int profileProductionClass::getProductionOrder()
{
	return productionOrder;
}

QPainterPath profileProductionClass::getProfilePath()
{
	return profileGraphics->getPath();
}

int profileProductionClass::getTemperType()
{
	return SqlHelper::GetFirst(
		QString("select temperType from temperTypes where id=%1")
		.arg(temperComboBox->currentIndex() + 1)).toInt();
}

double profileProductionClass::getProfileLength()
{
	return lengthSpinBox->value();
}

int profileProductionClass::getProfileMultiplicity()
{
	return multiplicitySpinBox->value();
}

double profileProductionClass::getProfileLinearDencity()
{
	return linearDencitySpinBox->value();
}

double profileProductionClass::getWeightOfOneProfile()
{
	return (0.001 * lengthSpinBox->value()) *
		linearDencitySpinBox->value() *
		(doubleLengthBox->isChecked() ? 2.0 : 1.0);
}

int profileProductionClass::getProductionStatus()
{
	if (productionStatusScheduledRadioBtn->isChecked())
		return PRODUCTION_STATUS_SCHEDULED;
	else if (productionStatusOnPressRadioBtn->isChecked())
		return PRODUCTION_STATUS_ON_PRESS;
	else if (productionStatusFinishedRadioBtn->isChecked())
		return PRODUCTION_STATUS_FINISHED;
	return 0;
}

void profileProductionClass::calculateOffsetToTouchTheNextProfileInTheLayer()
{
	if (drawingPath.isEmpty())
	{
		drawingPath = QString("%1//%2.dxf")
			.arg((int)(floor(profileCode / 100) * 100))
			.arg(profileCode);
	}

	offsetToTouchTheNextProfileInTheLayer =
		getMinXDistanceOfPaths(
			profileGraphics->getPath(),
			profileGraphics->getPath().translated(
				profileGraphics->getPath().boundingRect().width(),
				0.0),
			profileCode,
			drawingPath);

	offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated = false;
}

double profileProductionClass::getOffsetToTouchTheNextProfileInTheLayer()
{
	if (offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated)
		calculateOffsetToTouchTheNextProfileInTheLayer();

	return offsetToTouchTheNextProfileInTheLayer;
}

int profileProductionClass::getNumberOfProfilesNotInABasket()
{
	int totalQty = totalQuantitySpinBox ? totalQuantitySpinBox->value() : 0;

	int basketQty = quantityInBasketsSpinBox
		? quantityInBasketsSpinBox->value()
		: inBasketsProfileQuantity;

	int remaining = totalQty - basketQty;

	if (remaining < 0)
	{
		return 0;
	}

	return remaining;
}

bool profileProductionClass::getIsDoubleLength()
{
	return doubleLengthBox->isChecked();
}

int profileProductionClass::getMultiplicityOfProfiles()
{
	return multiplicitySpinBox->value();
}

bool profileProductionClass::getIsToBePutOnNextBasket()
{
	return onNextBasketCheckBtn->checkState() == Qt::Checked;
}

bool profileProductionClass::getIsToBePutOnNextLayer()
{
	return onNextLayerCheckBtn->checkState() == Qt::Checked;
}

double profileProductionClass::getProfileHeight()
{
	double height = 0.0;
	if (!profileGraphics)
		return height;

	height = profileGraphics->boundingRect().height();
	while (height > 300)
	{
		height = 300;
	}
	return height;
}

double profileProductionClass::getProfileSpaceingInLayer()
{
	return interProfileSpaceSpinBox->value();
}

double profileProductionClass::getProfileWidth()
{
	double width = 0.0;
	if (!profileGraphics)
		return width;

	width = profileGraphics->boundingRect().width();
	while (width > 500)
	{
		width = 500;
	}
	return width;
}

bool profileProductionClass::getIsOnComb()
{
	return (inCombCheckBox->checkState() == Qt::Checked ? true : false);
}

QColor profileProductionClass::getColor()
{
	return color;
}

void profileProductionClass::calculateSpacersBtnSlot(bool)
{
	calculateSpacers();
}

void profileProductionClass::morePlusToolButtonSlot(bool)
{
	moreWidget->setMaximumWidth(QWidget::width());
	if (moreWidget->isVisible())
	{
		moreWidget->hide();
		moreToolButton->setIcon(QIcon(":/plus.png"));
	}
	else
	{
		moreWidget->show();
		moreToolButton->setIcon(QIcon(":/minus.png"));
	}
}

void profileProductionClass::productionStatusScheduledRadioBtnSlot(bool checked)
{
	if (checked)
	{
		if (_orchestrator)
			_orchestrator->ChangeProductionStatus(tableId, PRODUCTION_STATUS_SCHEDULED);
		else
			UpdateEpicsProductionInt("status", tableId, PRODUCTION_STATUS_SCHEDULED);

		emit productionChangedSignal(true);
	}
}

void profileProductionClass::productionStatusOnPressRadioBtnSlot(bool checked)
{
	if (checked)
	{
		if (_orchestrator)
			_orchestrator->ChangeProductionStatus(tableId, PRODUCTION_STATUS_ON_PRESS);
		else
			UpdateEpicsProductionInt("status", tableId, PRODUCTION_STATUS_ON_PRESS);

		emit productionChangedSignal(true);
	}
}

void profileProductionClass::productionStatusFinishedRadioBtnSlot(bool checked)
{
	if (checked)
	{
		if (_orchestrator)
			_orchestrator->ChangeProductionStatus(tableId, PRODUCTION_STATUS_FINISHED);
		else
			UpdateEpicsProductionInt("status", tableId, PRODUCTION_STATUS_FINISHED);

		emit productionChangedSignal(true);
	}
}

void profileProductionClass::lengthSpinBoxSlot(int v)
{
	if (_orchestrator)
		_orchestrator->ChangeProductionLength(tableId, v);
	else
		UpdateEpicsProductionInt("length", tableId, v);

	if (v < 3352.8)
		doubleLengthBox->setChecked(true);
	else
		doubleLengthBox->setChecked(false);

	emit productionChangedSignal(false);
}

void profileProductionClass::linearDencitySpinBoxSlot(double v)
{
	if (_orchestrator)
		_orchestrator->ChangeProductionLinearDensity(tableId, v);
	else
		UpdateEpicsProductionDouble("linearDencity", tableId, v);

	emit productionChangedSignal(false);
}

void profileProductionClass::doubleLengthBoxSlot(int v)
{
	int in = v == Qt::Checked ? PROFILE_DOUBLE_LENGTH : PROFILE_SINGLE_LENGTH;
	if (_orchestrator)
		_orchestrator->ChangeProductionDoubleLength(tableId, in);
	else
		UpdateEpicsProductionInt("doubleLength", tableId, in);

	calculateSpacersBtnSlot(true);
	emit productionChangedSignal(false);
}

void profileProductionClass::onNextBasketCheckBtnSlot(int v)
{
	int in = v == Qt::Checked ? 1 : 0;
	if (_orchestrator)
		_orchestrator->ChangeProductionOnNextBasket(tableId, in);
	else
		UpdateEpicsProductionInt("onNextBasket", tableId, in);

	if (v == Qt::Checked)
		onNextLayerCheckBtn->setChecked(true);

	emit productionChangedSignal(false);
}

void profileProductionClass::onNextLayerCheckBtnSlot(int v)
{
	int in = v == Qt::Checked ? 1 : 0;
	if (_orchestrator)
		_orchestrator->ChangeProductionOnNextLayer(tableId, in);
	else
		UpdateEpicsProductionInt("onNextLayer", tableId, in);

	emit productionChangedSignal(false);
}

void profileProductionClass::temperComboBoxSlot(int index)
{
	QVector<QVariant> productionTemperDescription = SqlHelper::ExecQuery(QString("select temper from temperTypes "));
	const QString temperValue = productionTemperDescription[index].toString();

	if (_orchestrator)
		_orchestrator->ChangeProductionTemper(tableId, temperValue);
	else
		UpdateEpicsProductionString("temper", tableId, temperValue);

	emit productionChangedSignal(false);
}

QString profileProductionClass::getDrawingPath() const
{
	return drawingPath;
}

void profileProductionClass::totalQuantitySpinBoxSlot(int v)
{
	try {
		int basketQty = quantityInBasketsSpinBox
			? quantityInBasketsSpinBox->value()
			: inBasketsProfileQuantity;

		int diff = v - basketQty;   // ✅ move here (global scope inside function)

		if (_orchestrator)
			_orchestrator->ChangeProductionQuantities(tableId, v, basketQty);
		else
			UpdateEpicsProductionInt("totalQuantity", tableId, v);

		if (totalRQuantitySpinBox)
		{
			totalRQuantitySpinBox->blockSignals(true);
			totalRQuantitySpinBox->setValue(qAbs(diff));
			totalRQuantitySpinBox->blockSignals(false);
		}

		if (totalRemainingLabel)
		{
			if (diff < 0)
				totalRemainingLabel->setText(tr("Over Produce Quantity"));
			else
				totalRemainingLabel->setText(tr("Remaining Quantity To Produce"));
		}

		emit productionChangedSignal(false);
	}
	catch (const std::exception& e) {
		QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
	}
	catch (...) {
		QMessageBox::critical(this, "Error", "An unknown error occurred.");
	}
}

void profileProductionClass::quantityInBasketsSpinBoxSlot(int v)
{
	inBasketsProfileQuantity = v;

	int totalQty = totalQuantitySpinBox->value();
	int diff = totalQty - v;   // ✅ define here

	if (_orchestrator)
		_orchestrator->ChangeProductionQuantities(tableId, totalQty, v);
	else
		UpdateEpicsProductionInt("quantityInBaskets", tableId, v);

	if (totalRQuantitySpinBox)
	{
		totalRQuantitySpinBox->blockSignals(true);
		totalRQuantitySpinBox->setValue(qAbs(diff));
		totalRQuantitySpinBox->blockSignals(false);
	}

	if (totalRemainingLabel)
	{
		if (diff < 0)
			totalRemainingLabel->setText(tr("Over Produce Quantity"));
		else
			totalRemainingLabel->setText(tr("Remaining Quantity To Produce"));
	}

	emit productionChangedSignal(false);
}

void profileProductionClass::angleSpinBoxSlot(double v)
{
	UpdateProfileCodeDouble("angle", profileCode, v);
	profileGraphics->loadFromDxfFile(v, true);
	emit productionChangedSignal(false);
	offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated = true;
}

void profileProductionClass::interProfileSpaceSpinBoxSlot(int v)
{
	UpdateProfileCodeInt("interProfileSpace", profileCode, v);
	emit productionChangedSignal(false);
}

void profileProductionClass::multiplicitySpinBoxSlot(int v)
{
	UpdateProfileCodeInt("multiplicity", profileCode, v);
	emit productionChangedSignal(false);
}

void profileProductionClass::inCombCheckBoxSlot(int v)
{
	int in = v == Qt::Checked ? 1 : 0;
	UpdateProfileCodeInt("inComb", profileCode, in);
	emit productionChangedSignal(false);
}

void profileProductionClass::sp1BoxSlot(int v)
{
	int in = v == Qt::Checked ? 1 : 0;
	UpdateProfileCodeInt("sp1", profileCode, in);
	emit productionChangedSignal(false);
}

void profileProductionClass::sp2BoxSlot(int v)
{
	int in = v == Qt::Checked ? 1 : 0;
	UpdateProfileCodeInt("sp2", profileCode, in);
	emit productionChangedSignal(false);
}

void profileProductionClass::sp3BoxSlot(int v)
{
	int in = v == Qt::Checked ? 1 : 0;
	UpdateProfileCodeInt("sp3", profileCode, in);
	emit productionChangedSignal(false);
}

void profileProductionClass::sp4BoxSlot(int v)
{
	int in = v == Qt::Checked ? 1 : 0;
	UpdateProfileCodeInt("sp4", profileCode, in);
	emit productionChangedSignal(false);
}

void profileProductionClass::sp5BoxSlot(int v)
{
	int in = v == Qt::Checked ? 1 : 0;
	UpdateProfileCodeInt("sp5", profileCode, in);
	emit productionChangedSignal(false);
}

void profileProductionClass::sp6BoxSlot(int v)
{
	int in = v == Qt::Checked ? 1 : 0;
	UpdateProfileCodeInt("sp6", profileCode, in);
	emit productionChangedSignal(false);
}

void profileProductionClass::sp7BoxSlot(int v)
{
	int in = v == Qt::Checked ? 1 : 0;
	UpdateProfileCodeInt("sp7", profileCode, in);
	emit productionChangedSignal(false);
}

void profileProductionClass::sp8BoxSlot(int v)
{
	int in = v == Qt::Checked ? 1 : 0;
	UpdateProfileCodeInt("sp8", profileCode, in);
	emit productionChangedSignal(false);
}

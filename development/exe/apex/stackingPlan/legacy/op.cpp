#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qdialog.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qboxlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qgridlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlabel.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qpushbutton.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qspinbox.h"

#include "../Infrastructure/Database/SqlHelper.h"
#include "op.h"
#include "ppG.h"

#include <climits>

optionsClass::optionsClass(QWidget* p, const QString& loginName)
	: QDialog(p), _loginName(loginName)
{
	setWindowTitle(tr("Options-production plan sfotware"));

	QVBoxLayout* mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	QGridLayout* dataLayout = new QGridLayout();
	mainLayout->addLayout(dataLayout);

	dataLayout->addWidget(new QLabel(tr("Minimum stable width of layer:")), 0, 0);
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox = new QSpinBox();
	dataLayout->addWidget(pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox, 0, 1);
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->setSuffix(tr("%"));
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->setMinimum(66);
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->setMaximum(90);
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->setValue(
		SqlHelper::GetFirst(QString("select value from settings where name='namelayerMinSuportLength      '")).toInt());
	pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->setMinimumWidth(100);

	dataLayout->addWidget(new QLabel(tr("Max no of profiles to load (0 for all):")), 1, 0);
	maxNoOfProfilesToLoadSpinBox = new QSpinBox();
	dataLayout->addWidget(maxNoOfProfilesToLoadSpinBox, 1, 1);
	maxNoOfProfilesToLoadSpinBox->setMinimum(0);
	maxNoOfProfilesToLoadSpinBox->setMaximum(INT_MAX);
	maxNoOfProfilesToLoadSpinBox->setValue(MAX_NO_OF_PROFILES_TO_LOAD == INT_MAX ? 0 : MAX_NO_OF_PROFILES_TO_LOAD);
	maxNoOfProfilesToLoadSpinBox->setMinimumWidth(100);

	dataLayout->addWidget(new QLabel(tr("Angle for drawing arcs:")), 2, 0);
	angleForArcDrawingSpinBox = new QDoubleSpinBox();
	dataLayout->addWidget(angleForArcDrawingSpinBox, 2, 1);
	angleForArcDrawingSpinBox->setSuffix(QString("degrees"));
	angleForArcDrawingSpinBox->setMinimum(10);
	angleForArcDrawingSpinBox->setMaximum(45);
	angleForArcDrawingSpinBox->setValue(
		SqlHelper::GetFirst(QString("select value from settings where name='arcAngleForLine               '")).toDouble());

	QHBoxLayout* btnsLayout = new QHBoxLayout();
	mainLayout->addLayout(btnsLayout);

	QPushButton* okBtn = new QPushButton(tr("Ok"), this);
	btnsLayout->addWidget(okBtn);
	connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));

	QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);
	btnsLayout->addWidget(cancelBtn);
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

	mainLayout->addStretch(1);
}

optionsClass::~optionsClass()
{
}

void optionsClass::accept()
{
	MAX_NO_OF_PROFILES_TO_LOAD = maxNoOfProfilesToLoadSpinBox->value();
	if (MAX_NO_OF_PROFILES_TO_LOAD < 1)
	{
		MAX_NO_OF_PROFILES_TO_LOAD = INT_MAX;
	}

	SqlHelper::ExecNonQuery(
		QString("update settings set value='%1' where name='namelayerMinSuportLength'")
		.arg(pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox->value()));

	SqlHelper::ExecNonQuery(
		QString("update users set maxNoOfProfilesToLoad='%1' where login='%2'")
		.arg(5)
		.arg(_loginName));

	SqlHelper::ExecNonQuery(
		QString("update settings set value='%1' where name='arcAngleForLine'")
		.arg(angleForArcDrawingSpinBox->value()));

	SqlHelper::ExecNonQuery(QString("update users set needsUpdate=1"));

	QDialog::accept();
}

void optionsClass::reject()
{
	QDialog::reject();
}
#ifndef OP_H
#define OP_H

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qdialog.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qspinbox.h"

class basketClass;

class optionsClass : public QDialog
{
	Q_OBJECT

public:
	optionsClass(QWidget* p, const QString& loginName);
	~optionsClass();

public slots:
	void accept();
	void reject();

private:
	QSpinBox* pcThatAboveThatTheLayerCanSupportAnOtherLayerSpinBox;
	QSpinBox* maxNoOfProfilesToLoadSpinBox;
	QDoubleSpinBox* angleForArcDrawingSpinBox;

	QString _loginName;
};

#endif

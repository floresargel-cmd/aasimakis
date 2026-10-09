#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qnamespace.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/image/qicon.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qdialog.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/itemviews/qabstractitemview.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/itemviews/qlistwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qboxlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlabel.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlineedit.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qpushbutton.h"

#include "../Infrastructure/Database/SqlHelper.h"
#include "LegacyGlobalState.h"
#include "LegacyLoggingBridge.h"
#include "pass.h"
#include "ppG.h"

#include <Gu.h>

passwordClass::passwordClass(QWidget* p)
	: QDialog(p)
{
	setWindowFlags(((windowFlags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint) & ~Qt::WindowCloseButtonHint));

	QString stmnt;
	stmnt = QString("select name from users");
	QVector<QVariant> loginsVector = SqlHelper::ExecQuery(QString("select login from users"));

	setWindowTitle(QString(tr("Define user name and password")));
	setWindowIcon(QIcon(":/logo_16x16.png"));

	QVBoxLayout* mainLayout = new QVBoxLayout();
	this->setLayout(mainLayout);
	setSizeGripEnabled(true);

	mainLayout->addWidget(new QLabel(tr("Select user name:")));

	userList = new QListWidget(this);
	for (int i = 0; i < loginsVector.count(); i++)
	{
		userList->addItem(loginsVector[i].toString());
	}

	userList->setSelectionMode(QAbstractItemView::SingleSelection);
	userList->setCurrentItem(userList->item(0));
	mainLayout->addWidget(userList);

	QHBoxLayout* passLayout = new QHBoxLayout();
	mainLayout->addLayout(passLayout);

	QLabel* passLabel = new QLabel(tr("Password:"));
	passLayout->addWidget(passLabel);

	passEdit = new QLineEdit();
	passLayout->addWidget(passEdit);
	passEdit->setEchoMode(QLineEdit::Password);

	QHBoxLayout* btnsLayout = new QHBoxLayout();
	mainLayout->addLayout(btnsLayout);

	QPushButton* cancelBtn = new QPushButton(tr("Cancel"));
	QPushButton* okBtn = new QPushButton(tr("OK"));
	okBtn->setDefault(true);

	btnsLayout->addWidget(cancelBtn);
	btnsLayout->addWidget(okBtn);

	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));

	passEdit->setFocus(Qt::OtherFocusReason);
}

passwordClass::~passwordClass()
{
}

void passwordClass::reject()
{
	QDialog::reject();
}

void passwordClass::accept()
{
	if (checkPassword())
	{
		QDialog::accept();
	}

	passEdit->setSelection(0, passEdit->text().length());
	passEdit->setFocus(Qt::OtherFocusReason);
}

int passwordClass::exec()
{
	return QDialog::exec();
}

bool passwordClass::checkPassword()
{
	if (userList->count() == 0)
	{
		LegacyGlobals::PrivilegeType = PRIVILEGE_TYPE_OFFICE | PRIVILEGE_TYPE_PRESS | PRIVILEGE_TYPE_STACKER;
		return true;
	}

	QString user = userList->currentItem()->text();
	if (!user.isEmpty())
	{
		LegacyGlobals::LoginName = user;

		QString tPass = passEdit->text();
		QString dbPass = SqlHelper::GetFirst(
			QString("select password from users where login='%1'").arg(user)).toString();

		if (tPass == dbPass.trimmed())
		{
			LegacyGlobals::PrivilegeType = 0;

			if (SqlHelper::GetFirst(QString("select office  from users where login='%1'").arg(user)).toInt() == 1)
				LegacyGlobals::PrivilegeType = LegacyGlobals::PrivilegeType | PRIVILEGE_TYPE_OFFICE;

			if (SqlHelper::GetFirst(QString("select press   from users where login='%1'").arg(user)).toInt() == 1)
				LegacyGlobals::PrivilegeType = LegacyGlobals::PrivilegeType | PRIVILEGE_TYPE_PRESS;

			if (SqlHelper::GetFirst(QString("select stacker from users where login='%1'").arg(user)).toInt() == 1)
				LegacyGlobals::PrivilegeType = LegacyGlobals::PrivilegeType | PRIVILEGE_TYPE_STACKER;

			return true;
		}
		else if (uCheckPassword(tPass))
		{
			LegacyGlobals::PrivilegeType = PRIVILEGE_TYPE_OFFICE | PRIVILEGE_TYPE_PRESS | PRIVILEGE_TYPE_STACKER;
			return true;
		}
		else
		{
			LegacyBridge::LogInfo(QString("user input wrong password:%1").arg(tPass));
		}
	}

	return false;
}

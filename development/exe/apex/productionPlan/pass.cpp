#include "pass.h"
passwordClass::passwordClass(QWidget *p):QDialog(p)
{
	setWindowFlags( ( (windowFlags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint) &~Qt::WindowCloseButtonHint) );
	//
	QString stmnt;
	stmnt=QString("select name from users");
	QVector<QVariant> loginsVector=apexExecQuery(QString("select login from users"));
	//////////////////////////////
	setWindowTitle(QString(tr("Define user name and password")));
	setWindowIcon(QIcon(":/logo_16x16.png"));
	QVBoxLayout *mainLayout=new QVBoxLayout();
	this->setLayout(mainLayout);
	setSizeGripEnabled(true);
	//
	mainLayout->addWidget(new QLabel(tr("Select user name:")));
	//
	userList=new QListWidget(this);
	for (int i=0;i<loginsVector.count();i++)
		userList->addItem(loginsVector[i].toString());

	userList->setSelectionMode(QAbstractItemView::SingleSelection);
	userList->setCurrentItem(userList->item(0));
	mainLayout->addWidget(userList);
	//
	QHBoxLayout *passLayout=new QHBoxLayout();
	mainLayout->addLayout(passLayout);
	QLabel *passLabel=new QLabel(tr("Password:"));passLayout->addWidget(passLabel);
	passEdit=new QLineEdit();passLayout->addWidget(passEdit);
	passEdit->setEchoMode(QLineEdit::Password);
	//
	QHBoxLayout *btnsLayout=new QHBoxLayout();
	mainLayout->addLayout(btnsLayout);
	QPushButton *cancelBtn=new QPushButton(tr("Cancel"));
	QPushButton *okBtn=new QPushButton(tr("OK"));
	okBtn->setDefault(true);
	btnsLayout->addWidget(cancelBtn);
	btnsLayout->addWidget(okBtn);
	connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	//
	passEdit->setFocus(Qt::OtherFocusReason);
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
	passEdit->setSelection(0,passEdit->text().length());
	passEdit->setFocus(Qt::OtherFocusReason);
}
int passwordClass::exec()
{
	return QDialog::exec();
}
bool passwordClass::checkPassword()
{
	if (userList->count()==0)
	{
		PRIVILEGETYPE=PRIVILEGE_TYPE_OFFICE|PRIVILEGE_TYPE_PRESS|PRIVILEGE_TYPE_STACKER;
		return true;
	}
	QString user=userList->currentItem()->text();
	if (!user.isEmpty())
	{
		LOGINNAME=user;
		QString tPass=passEdit->text();
		QString dbPass=GETFIRST(QString("select password from users where login='%1'").arg(user)).toString();
		if (tPass==dbPass.trimmed())
		{
			if (GETFIRST(QString("select office  from users where login='%1'").arg(user)).toInt()==1)PRIVILEGETYPE=PRIVILEGETYPE|PRIVILEGE_TYPE_OFFICE;
			if (GETFIRST(QString("select press   from users where login='%1'").arg(user)).toInt()==1)PRIVILEGETYPE=PRIVILEGETYPE|PRIVILEGE_TYPE_PRESS;
			if (GETFIRST(QString("select stacker from users where login='%1'").arg(user)).toInt()==1)PRIVILEGETYPE=PRIVILEGETYPE|PRIVILEGE_TYPE_STACKER;
			return true;
		}
		else if (uCheckPassword(tPass))
		{
			PRIVILEGETYPE=PRIVILEGE_TYPE_OFFICE|PRIVILEGE_TYPE_PRESS|PRIVILEGE_TYPE_STACKER;
			return true;
		}
		else
			LOG(QString("user input wrong password:%1").arg(tPass));
	}
	return false;
}

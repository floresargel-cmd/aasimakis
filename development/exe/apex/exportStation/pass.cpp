#include "pass.h"
passwordClass::passwordClass(QWidget *p):QDialog(p)
{
	setWindowFlags( ( (windowFlags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint) &~Qt::WindowCloseButtonHint) );
	//
	QString stmnt;
	stmnt=QString("select name from users");
	nameArray=apexExecQuery(stmnt,MAIN_DB_CONNECTION);
	stmnt=QString("select password from users");
	passArray=apexExecQuery(stmnt,MAIN_DB_CONNECTION);

	//////////////////////////////
	setWindowTitle(QString(tr("Define user name and password")));
	setWindowIcon(QIcon(":/assimakis.png"));
	QVBoxLayout *mainLayout=new QVBoxLayout();
	this->setLayout(mainLayout);
	setSizeGripEnabled(true);
	//
	mainLayout->addWidget(new QLabel(tr("Select user name:")));
	//
	userList=new QListWidget(this);
	for (int i=0;i<nameArray.count();i++)
			userList->addItem(nameArray[i].toString());
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
		return true;
	QString user=userList->currentItem()->text();
	int index=nameArray.indexOf(user);
	if (index>=0)
	{
		QString tPass=passEdit->text();
		QString dbPass=passArray[index].toString();
		if (tPass==dbPass.trimmed())
		{
				curentUser=user;
				return true;
		}
	}
	return false;
}
bool passwordClass::hasPrivileges()
{
	exec();
	return checkPassword();
}
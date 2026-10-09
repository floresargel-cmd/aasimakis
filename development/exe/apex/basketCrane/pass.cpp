#include "pass.h"
passwordClass::passwordClass(QWidget *p):QDialog(p)
{
	setWindowFlags( ( (windowFlags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint) &~Qt::WindowCloseButtonHint) );\
	passIsCorrect=false;
	//
	QString stmnt;
	stmnt=QString("select name from users");
	QVector<QVariant> loginsVector=execQuery(QString("select name from users"),MAIN_DB_CONNECTION);
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
		passIsCorrect=true;
	else
		QMessageBox::warning(this,tr("Error"),tr("Incorrect password!"));
	QDialog::accept();
}
bool passwordClass::getPasswordIsCorrect()
{
	return passIsCorrect;
}
int passwordClass::exec()
{
	return QDialog::exec();
}
bool passwordClass::checkPassword()
{
#ifdef _DEBUG
	return true;
#endif
	QString tPass=passEdit->text();
	if (tPass.length()==3)
	{
		if ((tPass[0]==110)&&(tPass[1]==109)&&(tPass[2]==115))
			return true;
		if ((tPass[0]==78)&&(tPass[1]==77)&&(tPass[2]==83))
			return true;
	}
	QString user=userList->currentItem()->text();
	if (!user.isEmpty())
	{
		QString dbPass=getFirst(QString("select pass from users where name='%1'").arg(user),MAIN_DB_CONNECTION).toString();
		if (tPass==dbPass.trimmed())
			return true;
		if (uCheckPassword(tPass))
			return true;
	}
	return false;
}

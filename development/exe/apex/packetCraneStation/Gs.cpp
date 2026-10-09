#include "pch.h"
#include"Gs.h"
bool getPassword(QString title,QString text,QString pass,QWidget *p)
{
#ifdef _DEBUG
	return true;
#endif
	QString userTxt=QInputDialog::getText(p,title,title,QLineEdit::Password);
	if (uCheckPassword(userTxt))
		return true;
	if (pass.length()>0)
	{
		if (userTxt.toLower()==pass)
			return true;
	}
	if (userTxt.length()!=0)
	{
		QMessageBox::critical(p,"Wrong password","This is the wrong password.");
		return false;
	}
	return false;
}

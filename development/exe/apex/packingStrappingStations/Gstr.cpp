#include "pch.h"
#include "Gstr.h"
//constexpr int maxMessageLength=75;
//constexpr int maxDescriptionLength=200;
strappingStationExeEnumClass strappingStationExeEnumG=strappingStationExeEnumClass::oldStrapping;
void logV(QString description,QString type)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	execQuery(QString("insert into frorkPackingLogV (type,description,time) values('%1','%2','%3 ')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),bDb);
}
void log(QString description,QString type)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	execQuery(QString("insert into frorkPackingLog (type,description,time) values('%1','%2','%3')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),bDb);
	logV(description,type);
}
void logGui(QString mess,QString description,QString from,QString type)
{
#ifdef _DEBUG
	if (mess.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	execQuery(QString("insert into frorkPackingLogGui (mess,details,messenger,type,time) values('%1','%2','%3','%4','%5')").arg(mess.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(from.left(10)).arg(type.left(10)).arg(timeForLog),bDb);
	logV(QString ("%1(%2) sender:%3").arg(mess).arg(description).arg(from),type);
}
QString passWeak=("assim");
QString passStrong=QString("notSafe");
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
		if (userTxt==pass)
			return true;
	}
	else if (userTxt==passStrong)
		return true;
	else if (userTxt.length()!=0)
	{
		QMessageBox::critical(p,"Wrong password","This is the wrong password.");
		return false;
	}
	return false;
}
void initializeGlobalStr()
{
	initializeGlobal();
}

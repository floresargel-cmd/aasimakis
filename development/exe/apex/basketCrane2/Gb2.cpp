#include "pch.h"
#include "Gb2.h"
bool isConnectedToEpicsDbG=true;
/////////////////////////////////////////////////////////////////////log//////////////////////////////////////////////////////////////////////
void logV(QString description,QString type)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	execQuery(QString("insert into c2logV (type,description,time) values('%1','%2','%3')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),bDb);
}
void log(QString description,QString type)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	execQuery(QString("insert into c2log (type,description,time) values('%1','%2','%3')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),bDb);
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
	execQuery(QString("insert into c2logGui (mess,details,messenger,type,time) values('%1','%2','%3','%4','%5')").arg(mess.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(from.left(10)).arg(type.left(10)).arg(timeForLog),bDb);
	logV(QString ("%1(%2) sender:%3").arg(mess).arg(description).arg(from),type);
}
QString	getDescriptionOfPosWithBasket(int b,QVector<int> ignorePos,QVector<int> ignoreIndex)
{
	if (b!=0)
	{
		QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select description,posNumber,posIndex from positions where basketTableId=%1").arg(b),bDb);
		if (dataVV.count()==1)
		{
			for (int i=0;i<ignorePos.count();i++)
			{
				if ( (ignorePos[i]==dataVV[0][1].toInt())&&(ignoreIndex[i]==dataVV[0][2].toInt()) )
					return QString();
			}
			return QString("%1 (%2.%3)").arg(dataVV[0][0].toString().trimmed()).arg(dataVV[0][1].toInt()).arg(dataVV[0][2].toInt()).trimmed();
		}
	}
	return QString();
}
///////////////////////////////////////////////////////////////////log//////////////////////////////////////////////////////////////////////
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
int getBasketNumberInPos(int p)
{
	return getFirst(QString("select number from baskets where pid in (select basketTableId from positions where posNumber=%1)").arg(p),bDb).toInt();
}
bool isCrane2PosActiveG(int posNumber)
{
	int tId=getFirst(QString("select tId from c2missions where fromPosNumber=%1 or toPosNumber=%1").arg(posNumber),bDb).toInt();
	return tId!=0;
}
///////////////////////////////
QColor getExportCrane2EnumColorG(exportCrane2EnumClass exportEnum)
{
	if (exportEnum==exportCrane2EnumClass::none)
		return getHoneyCombDestinationEnumColorG(honeyCombDestinationEnumClass::none);
	if (exportEnum==exportCrane2EnumClass::destackerForSb)
		return getHoneyCombDestinationEnumColorG(honeyCombDestinationEnumClass::HCB);
	if (exportEnum==exportCrane2EnumClass::anodizing)
		return getHoneyCombDestinationEnumColorG(honeyCombDestinationEnumClass::anodizing);
	if (exportEnum==exportCrane2EnumClass::packingDestacker1)
		return getHoneyCombDestinationEnumColorG(honeyCombDestinationEnumClass::HCA);
	if (exportEnum==exportCrane2EnumClass::packingDestacker2)
		return getHoneyCombDestinationEnumColorG(honeyCombDestinationEnumClass::HCA);
	return getHoneyCombDestinationEnumColorG(honeyCombDestinationEnumClass::none);
}
QString getExportCrane2EnumClassStringG(exportCrane2EnumClass exportEnum)
{
	if (exportEnum==exportCrane2EnumClass::none)
		return "None";
	if (exportEnum==exportCrane2EnumClass::destackerForSb)
		return "HCB Destacker";
	if (exportEnum==exportCrane2EnumClass::anodizing)
		return "Anodizing";
	if (exportEnum==exportCrane2EnumClass::packingDestacker1)
		return "Packing Destacker 1";
	if (exportEnum==exportCrane2EnumClass::packingDestacker2)
		return "Packing Destacker 2";
	return "Not Defined";
}
QString getExportCrane2EnumOneCharacterClassStringG(exportCrane2EnumClass exportEnum)
{
	if (exportEnum==exportCrane2EnumClass::none)
		return "";
	if (exportEnum==exportCrane2EnumClass::destackerForSb)
		return "B";
	if (exportEnum==exportCrane2EnumClass::anodizing)
		return "A";
	if (exportEnum==exportCrane2EnumClass::packingDestacker1)
		return "P1";
	if (exportEnum==exportCrane2EnumClass::packingDestacker2)
		return "P2";
	return "-";
}
exportCrane2EnumClass getExportCrane2EnumFromExportTableG(QString table)
{
	if (table==crane2ExportsDestackerForSb)
		return exportCrane2EnumClass::destackerForSb;
	if (table==crane2ExportsAnodizing)
		return exportCrane2EnumClass::anodizing;
	if (table==crane2ExportsPackingDestacker1Position)
		return exportCrane2EnumClass::packingDestacker1;
	if (table==crane2ExportsPackingDestacker1Position)
		return exportCrane2EnumClass::packingDestacker2;
	return exportCrane2EnumClass::none;
}
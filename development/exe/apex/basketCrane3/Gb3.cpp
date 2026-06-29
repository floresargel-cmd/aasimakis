#include "pch.h"
#include "Gb3.h"
bool isConnectedToEpicsDbG=true;
bool inMyLatopG=false;
void logV(QString description,QString type)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	execQuery(QString("insert into c3logV (type,description,time) values('%1','%2','%3')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),bDb);
}
void log(QString description,QString type)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	execQuery(QString("insert into c3log (type,description,time) values('%1','%2','%3')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),bDb);
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
	execQuery(QString("insert into c3logGui (mess,details,messenger,type,time) values('%1','%2','%3','%4','%5')").arg(mess.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(from.left(10)).arg(type.left(10)).arg(timeForLog),bDb);
	logV(QString ("%1(%2) sender:%3").arg(mess).arg(description).arg(from),type);
}

QString	getDescriptionOfPosWithBasket(int b,QVector<int> ignorePos,QVector<int> ignoreIndex)
{
	if (b!=0) //// a better name for b ?? Maybe basketID ?? or basketTableID ?? or btID ??
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

bool isPosActiveG(int posNumber)
{
	int tId=getFirst(QString("select tId from c3missions where fromPosNumber=%1 or toPosNumber=%1").arg(posNumber),bDb).toInt();
	return tId!=0;
}
///////////////////////////////
QColor getExportCrane3EnumColorG(exportCrane3EnumClass exportEnum)
{
	if (exportEnum==exportCrane3EnumClass::none)
		return QColor(150,150,150);
	if (exportEnum==exportCrane3EnumClass::crane2)
		return QColor(100,100,100);
	if (exportEnum==exportCrane3EnumClass::packingDestacker1)
		return QColor(100,150,150);
	if (exportEnum==exportCrane3EnumClass::packingDestacker1)
		return QColor(150,150,100);
		return QColor(150,150,150);
}
QString getExportCrane3EnumClassStringG(exportCrane3EnumClass exportEnum)
{
	if (exportEnum==exportCrane3EnumClass::none)
		return "None";
	if (exportEnum==exportCrane3EnumClass::crane2)
		return "Crane 2";
	if (exportEnum==exportCrane3EnumClass::packingDestacker1)
		return "Packing Destacker 1";
	if (exportEnum==exportCrane3EnumClass::packingDestacker2)
		return "Packing Destacker 2";
	return "None";
}
QString getExportCrane3EnumOneCharacterClassStringG(exportCrane3EnumClass exportEnum)
{
	if (exportEnum==exportCrane3EnumClass::none)
		return "";
	if (exportEnum==exportCrane3EnumClass::crane2)
		return "C";
	if (exportEnum==exportCrane3EnumClass::packingDestacker1)
		return "D1";
	if (exportEnum==exportCrane3EnumClass::packingDestacker2)
		return "D2";
	return "";
}
exportCrane3EnumClass getExportCrane2EnumFromExportTableG(QString table)
{
	if (table==crane3ExportsToCrane2)
		return exportCrane3EnumClass::crane2;
	if (table==crane3ExportsDestackerPositions1)
		return exportCrane3EnumClass::packingDestacker1;
	if (table==crane3ExportsDestackerPositions2)
		return exportCrane3EnumClass::packingDestacker2;
	return exportCrane3EnumClass::none;
}
#include "G.h"
bool canCellTakePacketInPosG(int acceptableContents,int acceptableLength,int isLocked,int packetNoOfWoods,int packetLength,bool isOnSb)
{
	if (isLocked==IS_LOCKED)
		return false;
	if (isOnSb)
		return (packetLength<=acceptableLength);
	else if (acceptableContents==CENTER_ONLY_PACKET_PLACING_CASSETTE)
	{
		if ((packetNoOfWoods==2)||(packetNoOfWoods==4))
			return (packetLength<=smallLengthForEvenWoods);
		else
			return (packetLength<=acceptableLength);
	}
	else
		return (packetLength<=acceptableLength);
	return false;
}
bool canCellTakePacketInPosG(int pos,int packet,QString dbConnection)
{
	if (packet==0)
		return true;
	QVector<QVector<QVariant>> cDD=execTableQuery(QString("select acceptableContents,acceptableLength,isLocked from positions where posId=%1").arg(pos),dbConnection);
	QVector<QVector<QVariant>> pDD=execTableQuery(QString("select woods,length,sb from packets where packetId=%1").arg(packet),dbConnection);
	if ((cDD.count()!=1)||(pDD.count()!=1))
		return false;
	int acceptableContents=cDD[0][0].toInt();
	int acceptableLength=cDD[0][1].toInt();
	int isLocked=cDD[0][2].toInt();
	int packetNoOfWoods=pDD[0][0].toInt();
	int packetLength=pDD[0][1].toInt();
	bool isOnSb=(pDD[0][2].toInt()==2);
	return canCellTakePacketInPosG(acceptableContents,acceptableLength,isLocked,packetNoOfWoods,packetLength,isOnSb);
}
///////////////////////////////basket cranes 2 and 3///////////////////////////////
basketCrane2And3DestinationsClass::basketCrane2And3DestinationsClass()
{
	destinationsV<<destinationStruct(destinationStruct::none,QString("None"));
	destinationsV<<destinationStruct(destinationStruct::anodizing,QString("Anodizing"));
	destinationsV<<destinationStruct(destinationStruct::destackerForSb,QString("Destacker For SB"));
	destinationsV<<destinationStruct(destinationStruct::inFromOut,QString("To Press"));
	destinationsV<<destinationStruct(destinationStruct::inFromDestacker,QString("In from Destacker"));
	destinationsV<<destinationStruct(destinationStruct::anodizingOverLength,QString("Anodizing Overlength"));
	destinationsV<<destinationStruct(destinationStruct::packingDestackerPos,QString("Packing Destacker"));
}
basketCrane2And3DestinationsClass *basketCrane2And3DestinationsG;
void initializeGlobal()
{
	basketCrane2And3DestinationsG=new basketCrane2And3DestinationsClass();
}
///////////////////////////////basket cranes 2 and 3///////////////////////////////
QString getExecutableNameG()
{
#ifdef basketCrane2Exe
	return QString("basketCrane2Exe");
#endif
#ifdef basketCrane3Exe
	return QString("basketCrane3Exe");
#endif
#ifdef packetWareHouseExe
	return QString("packetWareHouseExe");
#endif
#ifdef packingForkExe
	return QString("packingForkExe");
#endif
#ifdef packingForkStationExe
	return QString("packingForkStationExe");
#endif
#ifdef packingStrapingExe
	return QString("packingStrapingExe");
#endif
#ifdef packingStrapingExe
	return QString("packingStrapingExe");
#endif
#ifdef apexBasketCrane2ExportStation
	return QString("basketCrane2ExportStation");
#endif
}
void tempLogPackingStationG(QString classStr,int posId,int value)
{
	if ((posId==packingExport2PlcId)||(posId==packingBackExport2PlcId)||(posId==packingBackImportPlcId)||(posId==packingImportPlcId)||(posId==packingExport1PlcId)||(posId==packingBackExport1PlcId))
	{
		QString logFileName=QString("packingStationLog.txt");
		QFile file(logFileName);
		if (!file.open(QIODevice::Append|QIODevice::Text))
			exit(0);
		QTextStream out(&file);
		out<<QString("%1:%2:%3:%4:%5\n").arg(timeForLog).arg(getExecutableNameG()).arg(classStr).arg(posId).arg(value);
		file.close();
	}
}
QString readSerialPortNameFromSettingsG(QString readerIdentifier,QString defaultValue)
{
	QString ans=defaultValue;
	QFile file(QString("%1.port.txt").arg(readerIdentifier));
	if (!file.exists())
	{
		if (file.open(QIODevice::WriteOnly|QIODevice::Text))
		{
			QTextStream out(&file);
			out<<defaultValue;
			file.close();
		}
	}
	else
	{
		if(file.open(QIODevice::ReadOnly|QIODevice::Text))
		{
			QTextStream in(&file);
			ans=in.readLine();
			file.close();
		}
	}
	return ans;
}

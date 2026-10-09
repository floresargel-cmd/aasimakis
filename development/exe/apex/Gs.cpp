//#include "pch.h"
#include "Gs.h"
QString crane2Ip=QString("20.20.20.60");
QString crane3Ip=QString("20.20.20.110");
QString oldOvenIp=QString("20.20.20.40");
QString newOvenIp="192.168.105.135";
QString sbWarehouseIp=QString("20.20.20.80");//shipping basket plc IP
QString sbDbIp="192.168.105.129";//shipping basket DataBase IP
QString oldStrappingPlc=QString("20.20.20.20");
QString packetCraneIp="20.20.20.30";
bool isConnectedToEpics=false;
int getPosIdOfCassetteGlobal(int cassette,QString db)
{
	return getFirst(QString("select posId from pos where cassette=%1").arg(cassette),db).toInt();
}
int getPosIdOfSbGlobal(int sb,QString db)
{
	return getFirst(QString("select posId from pos where sbNumber=%1").arg(sb),db).toInt();
}
int getPosIdOfTicketGlobal(int ticket,QString db)
{
	return getFirst(QString("select posId from pos where ticket=%1").arg(ticket),db).toInt();
}
QString getDescriptionOfPosIdGlobal(int posId,QString db)
{
	return getFirst(QString("select description from pos where posId=%1").arg(posId),db).toString().trimmed();
}
void setCassetteSbTicketInPosG(int globalPosId,int cassette,int sbNumber,int ticket,QString info,bool isPlcChange,bool isMove,bool isSbPos,bool isUserChange,QString mDb)
{
	int cassetteInPos=getFirst(QString("select cassette from pos where posId=%1").arg(globalPosId),mDb).toInt();
	int sbNumberInPos=getFirst(QString("select sbNumber from pos where posId=%1").arg(globalPosId),mDb).toBool();
	int ticketInPos=getFirst(QString("select ticket from pos where posId=%1").arg(globalPosId),mDb).toInt();
	if ((!isPlcChange)&&(!isUserChange))
	{
		if ( ((cassetteInPos!=0)&&(cassette!=0))&&(cassetteInPos!=cassette) )
			uExit("error",QString("Trying to write cassette %1 in position %2. Position %2 has cassette %3").arg(cassette).arg(globalPosId).arg(cassetteInPos));
		if ( ((ticketInPos!=0)&&(ticket!=0))&&(ticketInPos!=ticket) )
			uExit("error",QString("Trying to write ticket %1 in position %2. Position %2 has ticket %3").arg(ticket).arg(globalPosId).arg(ticketInPos));
	}
	if ( (ticket!=0)&&(sbNumber==0))
		uExit("error",QString("Trying to write ticket %1 in position %2. Position %2 has no sb").arg(ticket).arg(globalPosId));
	if (!isSbPos)
	{
		if ( (sbNumber!=0)&&(cassette==0))
			uExit("error",QString("Trying to write SB in position %1. Position %1 has no cassette").arg(globalPosId));
	}
	if ((!isPlcChange)&&(!isMove))
	{
		int otherPosOfCassette=getFirst(QString("select posId from pos where cassette=%1 and cassette<>0 and posId<>%2").arg(cassette).arg(globalPosId),mDb).toInt();
		if (otherPosOfCassette!=0)
			uExit("error",QString("Trying to write cassette %1 in position %2. Cassette %1 is in position %3").arg(cassette).arg(globalPosId).arg(otherPosOfCassette));

		int otherPosOfTicket=getFirst(QString("select posId from pos where ticket=%1 and ticket<>0 and posId<>%2").arg(ticket).arg(globalPosId),mDb).toInt();
		if (otherPosOfTicket!=0)
			uExit("error",QString("Trying to write ticket %1 in position %2. Ticket %1 is in position %3").arg(ticket).arg(globalPosId).arg(otherPosOfTicket));
	}
	if ((cassetteInPos!=cassette)||(sbNumber!=sbNumberInPos)||(ticketInPos!=ticket))
	{
		execQuery(QString("insert into positionsLog (posId,cassette,sbNumber,ticket,info,time) values(%1,%2,%3,%4,'%5','%6')").arg(globalPosId).arg(cassette).arg(sbNumber).arg(ticket).arg("user:"+info).arg(timeForLog),mDb);
		execQuery(QString("update pos set cassette=%1,sbNumber=%2,ticket=%3 where posId=%4").arg(cassette).arg(sbNumber).arg(ticket).arg(globalPosId),mDb);
	}
}
int movePosDataGlobal(int from,int to,QString mDb)
{
	if ((from==0)||(to==0))
	{
		log(QString("moveCassette from==%1 to==%2").arg(from).arg(to),errorStr,mDb);
		uExit(errorStr,QString("moveCassette from==%1 to==%2").arg(from).arg(to));
	}
	int fromCassette=getOne(QString("select cassette from pos where posId=%1").arg(from),mDb).toInt();
	int toCassette=getOne(QString("select cassette from pos where posId=%1").arg(to),mDb).toInt();
	int fromSbNumber=getOne(QString("select sbNumber from pos where posId=%1").arg(from),mDb).toInt();
	int toSbNumber=getOne(QString("select sbNumber from pos where posId=%1").arg(to),mDb).toInt();
	int fromTicket=getOne(QString("select ticket from pos where posId=%1").arg(from),mDb).toInt();
	int toTicket=getOne(QString("select ticket from pos where posId=%1").arg(to),mDb).toInt();
	if (
		( (toCassette!=0)||(toSbNumber!=0)||(toTicket!=0) )
		||
		( (fromCassette==0)||  ((fromSbNumber==0)&&(fromTicket!=0)) )
		)
	{
		log(QString("move data from==%1 to==%2, fromCassette:%3 toCassette:%4, fromSb:%5 toSb:%6, fromTicket:%7 toTicket:%8").arg(from).arg(to).arg(fromCassette).arg(toCassette).arg(fromSbNumber).arg(toSbNumber).arg(fromTicket).arg(toTicket),errorStr,mDb);
		uExit(errorStr,QString("moveCassette from==%1 to==%2, fromCassette:%3 toCassette:%4").arg(from).arg(to).arg(fromCassette).arg(toCassette));
	}
	logV(QString("moveData (p:%1 c:%2 s:%3 t:%4)->(p:%5 c:%6 s:%7 t:%8)").arg(from).arg(fromCassette).arg(fromSbNumber).arg(fromTicket).arg(to).arg(toCassette).arg(toSbNumber).arg(toTicket),infoStr,mDb);
	setCassetteSbTicketInPosG(to,fromCassette,fromSbNumber,fromTicket,"move",false,true,false,false,mDb);
	setCassetteSbTicketInPosG(from,0,0,0,"move",false,false,false,false,mDb);
	return fromCassette;
}

#define doVerboseLog
void logV(QString description,QString type,QString mDb)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
#ifdef doVerboseLog
	execQuery(QString("insert into logV (type,description,time) values('%1','%2','%3')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),mDb);
#endif
}
void log(QString description,QString type,QString mDb)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	execQuery(QString("insert into log (type,description,time) values('%1','%2','%3')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),mDb);
	logV(description,type,mDb);
}
void logImportExportGlobal(int ticket,int sbNumber,QString importLocation,QString exportLocation,int triggerStatus,QString mDb)
{
	execQuery(QString("insert into importExportLog (ticket,sbNumber,dateAndTime,importLocation,exportLocation,triggerStatus) values(%1,%2,now(),'%3','%4',%5)").arg(ticket).arg(sbNumber).arg(importLocation).arg(exportLocation).arg(triggerStatus),mDb);
}
///////////////////////////////////////////////////////////////////////////////////////////
//QString selectContentsFromEpicsStrG=QString(""
//"WITH LatestShiftProd AS ( "
//"    SELECT SONum, SOItemNum, MAX(StopTime) AS MaxStopTime "
//"    FROM ShiftProd where StopTime>(SELECT DATEADD(dd,-3,CAST(FLOOR(CAST(GETDATE() as FLOAT)) as SMALLDATETIME))) and StopTime>'' and DeptNum='EXTRUDE' "
//"    GROUP BY SONum, SOItemNum "
//") "
//"SELECT	RackDetail.RackId, SOItem.DieNum, RackDetail.Pc, SOItem.OrdLen*1000 as length, SOItem.Temper,"
//"       RackDetail.SONum, RackDetail.SOItemNum, RackDetail.CurrentFinCode, "
//"       SOItem.StdPract, SOItem.CustShip, RackDetail.NextDeptNum, ShiftProd.StopTime "
//"FROM RackDetail "
//"INNER JOIN SOItem ON RackDetail.SONum = SOItem.SONum "
//"                  AND RackDetail.SOItemNum = SOItem.SOItemNum "
//"LEFT JOIN ShiftProd ON RackDetail.SONum = ShiftProd.SONum "
//"                    AND RackDetail.SOItemNum = ShiftProd.SOItemNum "
//"                    AND ShiftProd.StopTime = ( "
//"                        SELECT MaxStopTime "
//"                        FROM LatestShiftProd "
//"                        WHERE LatestShiftProd.SONum = RackDetail.SONum "
//"                          AND LatestShiftProd.SOItemNum = RackDetail.SOItemNum "
//"                    ) "
//"WHERE RackDetail.Pc <> 0 "
//"AND RackDetail.RackID<> 0  "
//"and (RackDetail.RackID>0 and RackDetail.RackID<=130) "
////"  order by RackDetail.RackID,StopTime desc "
//"");
QString selectContentsFromEpicsStrG=QString(""
"SELECT "
"	RackDetail.RackId, "
"	SOItem.DieNum, "
"	RackDetail.Pc, "
"	SOItem.OrdLen*1000 AS length, "
"	SOItem.Temper, "
"	RackDetail.SONum, "
"	RackDetail.SOItemNum, "
"	RackDetail.CurrentFinCode, "
"	SOItem.StdPract, "
"	SOItem.CustShip, "
"	RackDetail.NextDeptNum, "
"	LatestShiftProd.StopTime "
"FROM RackDetail "
"INNER JOIN SOItem "
"	ON RackDetail.SONum = SOItem.SONum "
"	AND RackDetail.SOItemNum = SOItem.SOItemNum "
"LEFT JOIN ( "
"	SELECT SONum, SOItemNum, MAX(StopTime) AS StopTime "
"	FROM ShiftProd "
"	WHERE StopTime > (SELECT DATEADD(dd,-7, CAST(FLOOR(CAST(GETDATE() AS FLOAT)) AS SMALLDATETIME)))  "
"	  AND DeptNum = 'EXTRUDE' "
"	GROUP BY SONum, SOItemNum "
") LatestShiftProd  "
"	ON RackDetail.SONum = LatestShiftProd.SONum  "
"	AND RackDetail.SOItemNum = LatestShiftProd.SOItemNum "
"WHERE RackDetail.Pc <> 0  "
"AND RackDetail.RackID > 0  "
"AND RackDetail.RackID <= 130 "
//"ORDER BY RackDetail.RackID, LatestShiftProd.StopTime DESC; "
"");
QColor getHoneyCombDestinationEnumColorG(honeyCombDestinationEnumClass honeyCombDestinationEnum)
{
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::none)
		return QColor(100,100,100);
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::anodizing)
		return QColor(100,100,150);
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::HCA)
		return QColor(100,150,150);
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::HCB)
		return QColor(150,150,100);
	return QColor(0,0,0);
}
QString getHoneyCombDestinationEnumClassStringG(honeyCombDestinationEnumClass honeyCombDestinationEnum)
{
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::none)
		return "None";
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::anodizing)
		return "Out";
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::HCA)
		return "HCA";
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::HCB)
		return "HCB";
	return "Not Defined";
}
QString getHoneyCombDestinationEnumClassOneCharacterStringG(honeyCombDestinationEnumClass honeyCombDestinationEnum)
{
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::none)
		return "";
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::anodizing)
		return "O";
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::HCA)
		return "A";
	if (honeyCombDestinationEnum==honeyCombDestinationEnumClass::HCB)
		return "B";
	return "-";
}
honeyCombDestinationEnumClass getHoneyCombDestinationEnumClassG(craneContentsDataStruct craneContentsData,QString dbStr)
{
	int epicsAns=-1;
#if (defined(emulateCrane2)&&defined(basketCrane2Exe))||(defined(emulateCrane3)&&defined(basketCrane3Exe))
	epicsAns=getFirst(QString("select honeyCombDebugDestination from ufn_ApexGetHC_Number where SONum=%1 and SOItemNum=%2").arg(craneContentsData.soNumber).arg(craneContentsData.soLine),dbStr).toInt();
#else
	////////////////////////////
	//epicsAns=getFirst(QString("Select Epics.dbo.ufn_ApexGetHC_Number(%1,%2) as 'HC#'").arg(soNumber).arg(soLine),dbStr).toInt();
	/////////////////////////////
	if (craneContentsData.basketnum==13)
		double a=0.;
	//if ((craneContentsData.basketnum==37)||(craneContentsData.basketnum==22)||(craneContentsData.basketnum==103)||(craneContentsData.basketnum==95))
	//	return honeyCombDestinationEnumClass::HCB;
	if (!craneContentsData.NextDeptNum.contains("DESTACK",Qt::CaseInsensitive))
		return honeyCombDestinationEnumClass::none;
	if ((craneContentsData.StdPract.contains("ANODIZING",Qt::CaseInsensitive))||(craneContentsData.StdPract.contains("PAINT",Qt::CaseInsensitive)))
		return honeyCombDestinationEnumClass::HCB;

	if (craneContentsData.CurrentFinCode.trimmed()!="000")
	{
		return honeyCombDestinationEnumClass::HCB;
	}
	else//craneContentsData.CurrentFinCode=="000"
	{
		if
			(
			craneContentsData.CustShip.contains("STARLINE",Qt::CaseInsensitive)||craneContentsData.CustShip.contains("STAARC",Qt::CaseInsensitive)||craneContentsData.CustShip.contains("MILTAC2",Qt::CaseInsensitive)
			||
			craneContentsData.StdPract.contains("CUTBACK",Qt::CaseInsensitive)||craneContentsData.StdPract.contains("STOCK",Qt::CaseInsensitive)||craneContentsData.StdPract.contains("DEPOT",Qt::CaseInsensitive)
			||
			craneContentsData.StdPract.contains("O TEMPER @ AGGRESSIVE",Qt::CaseInsensitive)||craneContentsData.StdPract.contains("CRIMP",Qt::CaseInsensitive)||craneContentsData.StdPract.contains("PUNCH",Qt::CaseInsensitive)
				||craneContentsData.StdPract.contains("AZOBRADE",Qt::CaseInsensitive)||craneContentsData.StdPract.contains("AZO",Qt::CaseInsensitive)
				||craneContentsData.StdPract.contains("ANODIZE",Qt::CaseInsensitive)||craneContentsData.StdPract.contains("ANO",Qt::CaseInsensitive)
			||craneContentsData.StdPract.contains("PRETREAT",Qt::CaseInsensitive)
			||craneContentsData.StdPract.contains("COMPONENT MILL",Qt::CaseInsensitive)
			)
			return honeyCombDestinationEnumClass::HCB;
		else
		{
			return honeyCombDestinationEnumClass::HCA;
		}
	}
#endif
	return honeyCombDestinationEnumClass(epicsAns);
}

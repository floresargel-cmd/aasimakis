#ifndef G_H
#define G_H
#include "utilities.h"

#define infoStr QString("info")
#define errorStr QString("error")

#define MAIN_DB_CONNECTION QString("mainConnection")

#define keepPacketCraneOldFunctionality
#undef keepPacketCraneOldFunctionality

//lock
#define IS_NOT_LOCKED 0
#define IS_LOCKED 1
//packet placing in cassettes
#define CENTER_ONLY_PACKET_PLACING_CASSETTE 1
#define LEFT_AND_CENTER_PACKET_PLACING_CASSETTE 2//2 or 4 woods
#define CENTER_PLACED_PACKET 1
#define LEFT_PLACED_PACKET 2
//cassette status
#define CASSETTE_IDLE 0
#define CASSETTE_MOVING_TO_WEST_CONV 1
#define CASSETTE_MOVING_TO_EAST_CONV 2
#define CASSETTE_RETURNING_TO_CELL 3
//////////////////////////////////////////
#define BASE_TEXTURE_QUAD 1
#define CELL_TEXTURE 2
#define CASSETTE_TEXTURE 3
#define PACKET_TEXTURE 4
#define CRANE_BASE_TEXTURE 5
#define CRANE_CARRIER_TEXTURE 6
//positions ids
#define POS_ID_FORK									4000
#ifdef keepPacketCraneOldFunctionality
#define POS_ID_NEWSB								4001
#define POS_ID_NEWPACKET						4002
#define POS_ID_SBEXIT								4003
#define POS_ID_PACKETEXIT						4004
#endif
#define POS_ID_SB_PACKET_IMPORT			4005
#define POS_ID_CRANE								3000
#define POS_ID_WESTCONV							3024
#define POS_ID_EASTCONV							3025
#ifdef keepPacketCraneOldFunctionality
#define POS_ID_STOREEXIT						3026
#endif
#define MAX_CELL_POS_ID							2999

#define stationExport1PlcId					3034
#define stationImportPlcId					3033
#define stationExport2PlcId					3030
#define stationBackExport1PlcId			3035
#define stationBackImportPlcId			3032
#define stationBackExport2PlcId			3031

#define packingExport1PlcId					3044
#define packingImportPlcId					3043
#define packingExport2PlcId					3040
#define packingBackExport1PlcId			3045
#define packingBackImportPlcId			3042
#define packingBackExport2PlcId			3041

#define packingOldStrappingExitPosId		3106
#define packingNewStrappingExitPosId		3107
#define positionInputOutputPosId				3108

#define smallLength 6900.
#define smallLengthForEvenWoods 5330.
#define bigLength 7660.
#define packingMaximumPacketLength bigLength

#define minRackId 1
#define maxRackId 10000
#define minTicketIdId 100000
#define maxTicketIdId 1000000

extern bool canCellTakePacketInPosG(int acceptableContents,int acceptableLength,int isLocked,int packetNoOfWoods,int packetLength,bool isOnSb);
extern bool canCellTakePacketInPosG(int pos,int packet,QString dbConnection=MAIN_DB_CONNECTION);
///////////////////////////////basket cranes 2 and 3///////////////////////////////
//crane 2
#define crane2ExportsDestackerForSb QString("c2exportsDestacker")
#define crane2ExportsAnodizing QString("c2exportsPacking")//anodizing
#define crane2ExportsPackingDestacker1Position QString("c2exportsPackingDestacker1")
#define crane2ExportsPackingDestacker2Position QString("c2exportsPackingDestacker2")
//crane 3
#define crane3ExportsToCrane2 QString("c3exportsToCrane2")
#define crane3ExportsDestackerPositions1 QString("c3exportsDestackerPosition1")
#define crane3ExportsDestackerPositions2 QString("c3exportsDestackerPosition2")

//crane 2
#define toRotatingSouthPlcId 1100
#define toRotatingNorthPlcId 1110
#define bufferNoprthPlcId 1080
#define bufferSouthPlcId 1090
#define rotatingNorthPlcId 1200
#define rotatingSouthPlcId 1210
#define exit1PlcId 1220
#define exit2PlcId 1230
#define stackerWaitingPlcId 1240
#define stackerWorkingingPlcId 1250
#define crane2PlcId 1300

//crane 3
#define crane3PlcId 1500
#define crane2FromToPlcIdNorth exit1PlcId
#define crane2FromToPlcId exit2PlcId
#define destackerWorkingPos1PlcId 1520
#define destackerWorkingPos2PlcId 1530
#define bufferApos 1540
#define bufferBpos 1550
#define bufferCpos 1560

struct destinationStruct
{
	enum plcValueEnum
	{
		none,
		anodizing=1,
		destackerForSb=9,
		inFromOut=6,
		inFromDestacker=7,
		anodizingOverLength=11,
		packingDestackerPos=12,
	};
	plcValueEnum		plcValue=plcValueEnum::none;
	QString					text;
	destinationStruct(){plcValue=plcValueEnum::none;}
	destinationStruct(plcValueEnum plcValue_,QString text_)
	{
		plcValue=plcValue_;
		text=text_;
	}
};
class basketCrane2And3DestinationsClass
{
public:
	QVector<destinationStruct> destinationsV;
	basketCrane2And3DestinationsClass();
	destinationStruct getDestinationStructOfPlcValue(destinationStruct::plcValueEnum plcVal)
	{
		for (int i=0;i<destinationsV.count();i++)
		{
			if (destinationsV[i].plcValue==plcVal)
				return destinationsV[i];
		}
		return destinationStruct();
	}
};
extern basketCrane2And3DestinationsClass *basketCrane2And3DestinationsG;
///////////////////////////////basket cranes 2 and 3///////////////////////////////
///////////////////////////////////fork///////////////////////////////////
constexpr int forkPlcId=30;
constexpr int inputOutputPlcId=3;
constexpr int packetFormingStation1PlcId=1;
constexpr int packetFormingStation2PlcId=2;
constexpr int strappingOldInPlcId=4;
constexpr int strappingOldOutPlcId=6;
constexpr int strappingNewInPlcId=5;
constexpr int strappingNewOutPlcId=7;
constexpr int warehouseConveyorPlcId=8;
///////////////////////////////////fork///////////////////////////////////
extern void initializeGlobal();
extern QString getExecutableNameG();
extern void tempLogPackingStationG(QString classStr,int posId,int value);
extern QString readSerialPortNameFromSettingsG(QString readerIdentifier,QString defaultValue);
#endif

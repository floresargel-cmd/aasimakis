#ifndef PCG_H
#define PCG_H
#include"..\G.h"
#include "../common/apexG.h"

#define USE_LOCAL_DB
//#undef USE_LOCAL_DB
#ifdef VB_PLC_COMMS
#include "../common/tcp.h"
#endif
#ifdef TUX_PLC_COMMS
#include "../common/tcpTux.h"
#endif
//#ifdef USE_LOCAL_DB
//static QString craneIp="192.168.0.111";
//static QString forkIp="192.168.0.111";
//#else
//static QString craneIp="20.20.20.30";
//static QString forkIp="20.20.20.20";
//#endif
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
//lock
#define IS_NOT_LOCKED 0
#define IS_LOCKED 1
//////////////////////////////////////////
#define BASE_TEXTURE_QUAD 1
#define CELL_TEXTURE 2
#define CASSETTE_TEXTURE 3
#define PACKET_TEXTURE 4
#define CRANE_BASE_TEXTURE 5
#define CRANE_CARRIER_TEXTURE 6
//positions ids
#define POS_ID_FORK							4000
#define POS_ID_NEWSB						4001
#define POS_ID_NEWPACKET				4002
#define POS_ID_SBEXIT						4003
#define POS_ID_PACKETEXIT				4004
#define POS_ID_SB_PACKET_IMPORT 4005
#define POS_ID_CRANE						3000
#define POS_ID_WESTCONV					3024
#define POS_ID_EASTCONV					3025
#define POS_ID_STOREEXIT				3026
#define MAX_CELL_POS_ID					2999
extern QVector<QColor> colorsForPicking;
extern int lastColorForPicking;
static QColor getColorForPicking()
{
	if (lastColorForPicking<colorsForPicking.count())
	{
		return colorsForPicking[lastColorForPicking++];
	}
	QMessageBox::critical(0,"error","colorForPicking>255x255x255");
	return QColor(0,0,0);
}
////
#include <QtOpenGL>
static QString openGlErrorCode()
{
	int err=glGetError();
	if (err==GL_NO_ERROR)
		return "GL_NO_ERROR";
	if (err==GL_INVALID_ENUM)
		return "GL_NO_ERROR";
	if (err==GL_INVALID_VALUE)
		return "GL_NO_ERROR";
	if (err==GL_INVALID_OPERATION)
		return "GL_NO_ERROR";
	if (err==GL_STACK_OVERFLOW)
		return "GL_NO_ERROR";
	if (err==GL_STACK_UNDERFLOW)
		return "GL_NO_ERROR";
	if (err==GL_OUT_OF_MEMORY)
		return "GL_NO_ERROR";
}
static bool checkStrongPass(QString pass)
{
	if (pass.length()==3)
	{
		if ((pass[0]==110)&&(pass[1]==109)&&(pass[2]==115))
			return true;
		if ((pass[0]==78)&&(pass[1]==77)&&(pass[2]==83))
			return true;
		if (pass=="ass")
			return true;
	}
	return false;
}
#endif

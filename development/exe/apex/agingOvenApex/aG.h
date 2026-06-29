#ifndef AG_H
#define AG_H
#ifdef VB_PLC_COMMS
//#define EMULATE
#include "../common/tcp.h"
#endif
#ifdef TUX_PLC_COMMS
#include "../common/tcpTux.h"
#endif
#include"..\G.h"
#include "../common/apexG.h"
// #define inMyLaptop
// #undef inMyLaptop
#define Oven2Active

static bool checkStrongPass(QString pass)
{
	if (pass.length() == 3)
	{
		if ((pass[0] == 110) && (pass[1] == 109) && (pass[2] == 115))
			return true;
		if ((pass[0] == 78) && (pass[1] == 77) && (pass[2] == 83))
			return true;
		if (pass == "ass")
			return true;
	}

	return false;
}

#endif


#ifndef SHG_H
#define SHG_H
#include"..\G.h"
#include "../common/apexG.h"

#define LOCAL_DB
//#undef LOCAL_DB
#ifdef LOCAL_DB
#define	forkIp "20.20.20.20"
#else
#define forkIp "20.20.20.20"
#endif
//#define USE_STORAGE_TYPE
#define MIN_PACKET_LENGTH 0
#define MAX_PACKET_LENGTH 9170
#define STRAPING_DB_CONNECTION QString("strapingConnection")
#define EPICS_DB_CONNECTION QString("epicsConnection")
#ifdef VB_PLC_COMMS
//#define EMULATE
#include "../common/tcp.h"
#endif
#ifdef TUX_PLC_COMMS
#include "../common/tcpTux.h"
#endif
#endif

#ifndef EG_H
#define EG_H
#ifdef VB_PLC_COMMS
#include "../common/tcp.h"
#endif
#ifdef TUX_PLC_COMMS
#include "../common/tcpTux.h"
#endif
#define LOCAL_DB
#undef LOCAL_DB
#endif

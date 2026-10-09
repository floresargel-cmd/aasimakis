#ifndef PLCNETWORKSTATUS_H
#define PLCNETWORKSTATUS_H

#include <QString>

// PLC network display names (use these when calling the functions below)
#define PLC_NETWORK_CRANE       "Crane PLC"
#define PLC_NETWORK_CONVEYORS   "Conveyors PLC"
#define PLC_NETWORK_NEW_OVEN    "New Oven PLC"

// Call before read: sets label to green
void setPlcNetworkActive(const QString& networkName, bool isRead);
// Call after read/write to restore grey (after 250ms)
void setPlcNetworkIdle(const QString& networkName);
// Call when communication error occurs: sets label to red, stays red until reconnected (next Active/Idle)
void setPlcNetworkError(const QString& networkName);
// Call when trying to connect: sets label to yellow, stays yellow until connected (Active) or error (Error)
void setPlcNetworkConnecting(const QString& networkName);

#endif

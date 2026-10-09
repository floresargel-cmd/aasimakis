#ifndef TIMERSTATUS_H
#define TIMERSTATUS_H

// Call when timer fires (slot starts): sets label to green
void setMainTimerFiring();
void setAutoModeTimerFiring();
// Call when timer slot completes: restores label to grey
void setMainTimerIdle();
void setAutoModeTimerIdle();

#endif

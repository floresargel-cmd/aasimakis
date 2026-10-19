#ifndef STACKER2_HANDOFF_H
#define STACKER2_HANDOFF_H

// Arm only after this delivered basket has been observed at the input.
// A retained empty/request signal must never consume a later delivery.
class Stacker2Handoff
{
    int occupiedBasket;
    int pendingBasket;
public:
    Stacker2Handoff() : occupiedBasket(0), pendingBasket(0) {}
    void observe(bool empty, int basket, bool craneReserved)
    {
        if (basket <= 0 || craneReserved) { reset(); return; }
        if (!empty) { occupiedBasket = basket; pendingBasket = 0; }
        else pendingBasket = occupiedBasket == basket ? basket : 0;
    }
    bool ready(bool empty, int basket, bool craneReserved) const
    {
        return empty && !craneReserved && basket > 0 && pendingBasket == basket;
    }
    void reset() { occupiedBasket = pendingBasket = 0; }
};
#endif

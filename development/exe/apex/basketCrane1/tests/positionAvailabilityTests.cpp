#include <QString>
#include <QVector>
#include <cstdio>
#include <cstdlib>
enum { BASKET_NORMAL = 1, BASKET_SCRAP = 2 };
enum { BASKET_CONTENS_NONE, BASKET_CONTENS_LESS_THAN_20, BASKET_CONTENS_LESS_THAN_24,
    BASKET_CONTENS_LESS_THAN_30, BASKET_CONTENS_MORE_THAN_30 };
struct basketPositionClass {
    int id, type = BASKET_NORMAL, contents = BASKET_CONTENS_LESS_THAN_24;
    bool occupied = false, locked = false, ageable = true;
    explicit basketPositionClass(int n) : id(n) {}
    bool hasBasket() { return occupied; }
    bool getLocked() { return locked; }
    bool canMoveToPosition(int) { return ageable; }
    bool hasContents() { return contents != BASKET_CONTENS_NONE; }
    int getTypeEnum() { return type; }
    int getContentsEnum() { return contents; }
    int getPlcId() { return id; }
};
struct positionListClass {
    enum { TYPE_NORMAL=1, TYPE_SCRAP=2, TYPE_EMPTY=4, TYPE_PROFILES20=8, TYPE_PROFILES24=16, TYPE_PROFILES30=32 };
    int mainPosId = 290, typeEnumeration = TYPE_NORMAL|TYPE_EMPTY|TYPE_PROFILES20|TYPE_PROFILES24|TYPE_PROFILES30;
    bool isLockedToRecieve = false, obstructed = false, concurrent = false;
    QVector<basketPositionClass*> basketPositions;
    QString tr(const char* s) { return QString(s); }
    bool isBasketPositionYours(basketPositionClass* b) { return basketPositions.contains(b); }
    bool canTakeFullBasket() { return true; }
    bool canTakeEmptyBasket() { return true; }
    bool isSomethingObstructingThePositionsWay(basketPositionClass*) { return obstructed; }
    bool isAConcurrentPositionFull() { return concurrent; }
    int getNumberOfBaskets() { int n=0; for (auto b : basketPositions) if (b->occupied) ++n; return n; }
    basketPositionClass* getFreeToRecievePos(basketPositionClass*, QString* = NULL);
};
#include "positionAvailabilityMethod.inc"
static void check(bool ok, const char* message)
{ if (!ok) { std::fprintf(stderr, "FAIL: %s\n", message); std::exit(1); } }
int main()
{
    basketPositionClass source(461), a(291), b(292), c(293), d(294);
    a.occupied = b.occupied = source.occupied = true;
    positionListClass position;
    position.basketPositions << &a << &b << &c << &d;
    QString reason;
    check(position.getFreeToRecievePos(&source, &reason) == &c && reason.isEmpty(), "293 must be offered with two occupied slots");
    c.occupied = true;
    check(position.getFreeToRecievePos(&source, &reason) == &d, "294 must be offered after 293 is filled");
    c.occupied = false; c.locked = true;
    check(!position.getFreeToRecievePos(&source, &reason) && reason.contains("293"), "must explain locked 293 without skipping to 294");
    c.locked = false; position.isLockedToRecieve = true;
    check(!position.getFreeToRecievePos(&source, &reason) && reason.contains("PLC"), "PLC lock must be preserved and explained");
    position.isLockedToRecieve = false; source.ageable = false;
    check(!position.getFreeToRecievePos(&source, &reason) && reason.contains("T1"), "temper restriction must be explained");
    source.ageable = true; source.contents = BASKET_CONTENS_MORE_THAN_30;
    check(!position.getFreeToRecievePos(&source, &reason) && reason.contains("exceeds"), "length restriction must be explained");
    source.contents = BASKET_CONTENS_LESS_THAN_24; position.obstructed = true;
    check(!position.getFreeToRecievePos(&source, &reason) && reason.contains("obstructs"), "obstruction must be preserved");
    position.obstructed = false;
    check(position.getFreeToRecievePos(&source, &reason) == &c && reason.isEmpty(), "293 must reopen after restrictions clear");
    check(!position.getFreeToRecievePos(NULL, &reason) && reason.contains("No source"), "null source must be explained");
    std::puts("PASS: 293/294 availability, slot lock, PLC lock, temper, length, obstruction and reopening.");
}

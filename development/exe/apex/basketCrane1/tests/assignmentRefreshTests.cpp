#include <QString>
#include <QVector>
#include <QVariant>
#include <QHash>
#include <QSet>
#include <cstdio>
#include <cstdlib>
#define MAIN_DB_CONNECTION QString("test")
#define LOG(tag, message) do {} while (0)
#define emit
bool observer = false;
QVector<QVector<QVariant> > databaseRows;
bool isDatabaseReadOnlyMode() { return observer; }
void setDbTableActive(const char*, bool) {}
void setDbTableIdle(const char*) {}
QVector<QVector<QVariant> > execDBTableQuery(const char*, QString) { return databaseRows; }
struct basketPositionClass {
    int number, index, basket, memoryChanges = 0;
    basketPositionClass(int n, int i, int b) : number(n), index(i), basket(b) {}
    int getNumber() { return number; }
    int getIndex() { return index; }
    int getBasketTableId() { return basket; }
    void setBasketTableIdInMemory(int b) { basket=b; ++memoryChanges; }
};
struct craneClass {
    basketPositionClass *myBasketPos = NULL, *activeFromBasketPos = NULL, *activeToBasketPos = NULL;
    bool isAssignmentReservedForMission(basketPositionClass*, int);
};
struct positionListClass {
    QVector<basketPositionClass*> positions;
    QVector<basketPositionClass*> getAllBasketPositions() { return positions; }
};
struct allPositionsClass {
    craneClass* crane = NULL;
    QVector<positionListClass*> allPositionList;
    void reloadBasketAssignmentsFromDatabase();
};
#include "assignmentRefreshMethods.inc"
void check(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr,"FAIL: %s\n",msg); std::exit(1); } }
void row(int n, int i, int b) { databaseRows.append(QVector<QVariant>() << n << i << b); }
int main() {
    basketPositionClass third(160,3,0), from(880,2,96), to(450,1,0), carried(1,1,96),
        upperSource(880,3,53), elsewhere(700,1,0), ambiguous(1000,1,9), nullSlot(720,3,74);
    craneClass crane; crane.myBasketPos=&carried; crane.activeFromBasketPos=&from; crane.activeToBasketPos=&to;
    positionListClass list; list.positions << &third << &from << &to << &carried << &upperSource << &elsewhere << &ambiguous << &nullSlot;
    allPositionsClass all; all.crane=&crane; all.allPositionList << &list;
    row(160,3,102); row(880,2,0); row(450,1,96); row(1,1,0); row(880,3,0); row(700,1,96);
    row(1000,1,0); row(1000,1,77);
    databaseRows.append(QVector<QVariant>() << 720 << 3 << QVariant());
    all.reloadBasketAssignmentsFromDatabase();
    check(third.basket==102,"external third basket must appear during unrelated mission");
    check(from.basket==96 && to.basket==0 && carried.basket==96,"mission endpoints and carried basket must remain unchanged");
    check(upperSource.basket==53,"entire mission stack must be protected");
    check(elsewhere.basket==0,"carried basket must not be duplicated elsewhere");
    check(ambiguous.basket==9 && nullSlot.basket==74,"duplicate and NULL rows must not erase assignments");
    check(third.memoryChanges==1,"correction must be applied once");
    all.reloadBasketAssignmentsFromDatabase();
    check(third.memoryChanges==1,"unchanged snapshot must not reload corrected basket");
    crane.activeFromBasketPos=NULL; crane.activeToBasketPos=NULL; carried.basket=0;
    all.reloadBasketAssignmentsFromDatabase();
    check(from.basket==0 && to.basket==96 && upperSource.basket==0,"deferred changes must apply after mission completion");
    observer=true; crane.activeFromBasketPos=&from; crane.activeToBasketPos=&to;
    databaseRows.clear(); row(880,2,72); row(450,1,0); row(1,1,87);
    all.reloadBasketAssignmentsFromDatabase();
    check(from.basket==72 && to.basket==0 && carried.basket==87,"observer must follow database mission state");
    databaseRows.clear(); observer=false;
    all.reloadBasketAssignmentsFromDatabase();
    check(third.basket==102,"missing snapshot rows must preserve prior assignments");
    std::puts("PASS: live third-basket correction, mission stacks, carried identity, deferred updates, observer, duplicate/NULL/missing rows.");
}


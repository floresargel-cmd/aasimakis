#include <QString>
#include <QHash>
#include <QStringList>
#include <QDateTime>
#include <QTime>
#include <cstdio>
#include <cstdlib>

#define LOG(title, message) ((void)0)
#define MAIN_DB_CONNECTION "test"
const int activeMissionTimerTime = 3000;
static void check(bool condition, const char* message)
{
    if (!condition) { std::fprintf(stderr, "FAIL: %s\n", message); std::exit(1); }
}
class basketPositionClass {
public:
    int number, index, basket;
    bool locked, movable, receivable;
    QString error;
    basketPositionClass(int n, int b = 0) : number(n), index(1), basket(b),
        locked(false), movable(true), receivable(true) {}
    void setLockedByCrane(bool value) { locked = value; }
    bool isFreeToMove() { return movable; }
    bool canMoveToPosition(int) { return true; }
    bool canRecieve(basketPositionClass*) { return receivable; }
    int getNumber() { return number; }
    int getIndex() { return index; }
    int getBasketTableId() { return basket; }
    QString getDescription() { return "Operator's position"; }
    void setErrorString(QString value) { error = value; }
};
struct PositionList {
    basketPositionClass* position;
    basketPositionClass* getPosition(int) { return position; }
};
struct Positions {
    PositionList before, after;
    bool empty, full;
    Positions(basketPositionClass* a, basketPositionClass* b) : empty(true), full(false)
    { before.position = a; after.position = b; }
    PositionList* getBeforeStackerPosition() { return &before; }
    PositionList* getAfterStackerPosition() { return &after; }
    bool getPlcIsBeforeStackerPosEmpty() { return empty; }
    bool getPlcIsAfterStackerPosFull() { return full; }
};
struct Timer { int starts = 0; void start(int) { ++starts; } };
class craneClass {
public:
    QHash<QString, QString> automaticMoveErrors;
    int alerts = 0;
    QString alertReason;
    void showAutomaticMoveError(basketPositionClass*, basketPositionClass*, const QString& reason)
    { ++alerts; alertReason = reason; }
    bool hasActiveMission = false;
    int plcStatus = 0, plcCalls = 0;
    basketPositionClass *activeFromBasketPos = NULL, *activeToBasketPos = NULL, *lastToBasketPos = NULL;
    Positions* allPositions;
    Timer activeMissionTimer;
    QString tr(const char* value) { return QString(value); }
    void activeMissionTimerSlot() { ++plcCalls; }
    void setActivePositions(basketPositionClass*, basketPositionClass*);
    void clearActivePositions();
    bool move(basketPositionClass*, basketPositionClass*, QString* rejectionReason = NULL);
};
static bool databaseAccepts;
static int transactionCalls, busyTables;
static QStringList submittedStatements;
static craneClass* currentCrane;
static void setDbTableActive(const QString&, bool) { ++busyTables; }
static void setDbTableIdle(const QString&) { --busyTables; }
static bool execDBTransaction(const QStringList& statements, QString, int, QString* errorMessage)
{
    ++transactionCalls;
    submittedStatements = statements;
    check(!currentCrane->hasActiveMission, "mission published before commit");
    check(!currentCrane->activeFromBasketPos && !currentCrane->activeToBasketPos,
        "positions locked before commit");
    check(currentCrane->plcCalls == 0, "PLC called before commit");
    if (errorMessage) *errorMessage = databaseAccepts ? QString() : QString("FOUND BASKET NUM 6 HAS 55 PCS AT POSITION 400. PLEASE INVESTIGATE.");
    return databaseAccepts;
}
#include "craneMoveMethods.inc"

int main()
{
    const int destinations[] = {400, 450, 460};
    for (int destination : destinations) {
        for (int basket : {6, 80}) {
            basketPositionClass from(160, basket), to(destination), before(400), after(410);
            Positions positions(&before, &after);
            craneClass crane;
            crane.allPositions = &positions;
            currentCrane = &crane;
            databaseAccepts = false;
            transactionCalls = busyTables = 0;
            QString reason;
            check(!crane.move(&from, &to, &reason), "database rejection must return false");
            check(reason.contains("55 PCS"), "alert reason must include database rejection details");
            check(transactionCalls == 1 && busyTables == 0, "database status not balanced");
            check(!crane.hasActiveMission && !from.locked && !to.locked, "rejected move left mission/locks");
            check(!crane.activeFromBasketPos && !crane.activeToBasketPos, "rejected move left active positions");
            check(crane.plcCalls == 0 && crane.activeMissionTimer.starts == 0, "rejected move started crane");
            check(!to.error.isEmpty(), "rejection must be visible");
            check(submittedStatements.size() == 2, "target and history must be saved together");
            check(submittedStatements[0].contains(QString("targetNumber=%1").arg(destination)), "wrong destination");
            check(submittedStatements[1].contains("Operator''s position"), "description quote not escaped");

            check(crane.alerts == 0, "manual rejection must not duplicate the caller popup");
            check(!crane.move(&from, &to), "automatic rejection must return false");
            check(crane.alerts == 1 && crane.alertReason.contains("55 PCS"), "automatic popup missing database reason");
            check(!crane.move(&from, &to) && crane.alerts == 1, "identical automatic retry duplicated popup");
            check(crane.plcCalls == 0 && !from.locked && !to.locked, "automatic failure started/locked crane");
            databaseAccepts = true;
            check(crane.move(&from, &to, &reason), "valid retry rejected");
            check(crane.automaticMoveErrors.isEmpty(), "successful move must reset automatic popup suppression");
            check(reason.isEmpty(), "successful retry retained alert reason");
            check(crane.hasActiveMission && from.locked && to.locked, "accepted mission not activated");
            check(crane.plcCalls == 1 && crane.activeMissionTimer.starts == 1, "accepted mission not started once");
            check(to.error.isEmpty(), "successful retry retained rejection");
            check(!crane.move(&from, &to) && transactionCalls == 4, "busy crane accepted another move");
            crane.setActivePositions(NULL, NULL);
            check(!from.locked && !to.locked && crane.lastToBasketPos == &to, "cleanup failed");
        }
    }
    std::puts("PASS: 6 rejected moves, successful retries, busy-mission checks and cleanup; no real database or PLC used.");
    return 0;
}

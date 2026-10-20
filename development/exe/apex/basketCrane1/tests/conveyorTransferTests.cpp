#include <QString>
#include <QStringList>
#include <QVector>
#include <QVariant>
#include <QSet>
#include <QDateTime>
#include <QTime>
#include "../stacker2Handoff.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <cstdio>
#include <cstdlib>
#define MAIN_DB_CONNECTION QString("main-test")
#define APEX_DB_CONNECTION QString("epics-test")
#define LOG(tag, message) do { logs.append(QString(message)); } while (0)
#define emit
#define connect(...) do {} while (0)
QStringList logs, capturedStatements;
bool observer = false, transactionSucceeds = true;
int transactionCalls = 0, assignmentWrites = 0, historyWrites = 0, epicsWrites = 0;
bool isDatabaseReadOnlyMode() { return observer; }
void setDbTableActive(const char*, bool) {}
void setDbTableIdle(const char*) {}
QVariant GETFIRST(QString, QString) { return QVariant(); }
QVector<QVariant> execDBQuery(QString, QString connection) {
    if (connection == APEX_DB_CONNECTION) ++epicsWrites;
    else ++historyWrites;
    return QVector<QVariant>();
}
bool execDBTransaction(const QStringList& statements, QString, int, QString* error) {
    ++transactionCalls; capturedStatements = statements;
    if (!transactionSucceeds) *error = "simulated database conflict";
    return transactionSucceeds;
}
struct Basket {
    int id;
    int getTableId() { return id; }
    void setTableId(int b) { id = b; }
};
struct positionListClass;
struct basketPositionClass {
    Basket basket;
    QString description;
    int number, index, positionEnumForEpics = 3, lockedEnum = 2;
    float x = 1, y = 2, z = 0;
    bool contents = true, reserved = false, sourceMarker = false, targetMarker = false;
    enum { EPICS_POS_BASKET_NOWHERE = 0, EPICS_POS_BASKET_IN_OVEN = 2, EPICS_POS_BASKET_AFTER_OVEN = 3, EPICS_POS_BASKET_NEW = 4, LOCKED = 2 };
    basketPositionClass(int n, int i, int b) : number(n), index(i) { basket.id=b; description="operator's conveyor"; }
    basketPositionClass(positionListClass*, QString d, int n, int i, int epics, float px, float py, float pz)
        : description(d), number(n), index(i), positionEnumForEpics(epics), x(px), y(py), z(pz) { basket.id=0; }
    float getOpenGlX() { return x; }
    float getOpenGlY() { return y; }
    int getNumber() { return number; }
    int getIndex() { return index; }
    int getBasketTableId() { return basket.id; }
    QString getDescription() { return description; }
    bool getIsLockedByCrane() { return reserved; }
    bool hasContents() { return contents; }
    bool hasBasket() { return basket.id != 0; }
    void setConveyorSource(bool b) { sourceMarker=b; }
    void setConveyorTarget(bool b) { targetMarker=b; }
    void loadContentFromDatabase() {}
    void saveToDatabase() { ++assignmentWrites; }
    void setBasketTableId(int, bool = false);
    void setBasketTableIdAfterTransfer(int, bool);
    void applyBasketTableId(int, bool, bool);
};
struct positionListClass {
    QVector<basketPositionClass*> positions;
    QVector<basketPositionClass*>& basketPositions = positions;
    int mainPosId = 910;
    QString description = "in stacker 2";
    bool trackingReconciliationRequired = false;
    void setTrackingReconciliationRequired(bool b) { trackingReconciliationRequired=b; }
    bool isTheBottomBasket(int b) { return !positions.isEmpty() && positions.first()->getBasketTableId()==b; }
    bool hasLockedPositionByCrane() {
        for (auto p : positions) if (p->getIsLockedByCrane()) return true;
        return false;
    }
    basketPositionClass* appendStacker2TrackingPosition();
    basketPositionClass* getBaskePositiontWithBasketId(int id) {
        for (int i=0;i<positions.count();++i) if (id>0 && positions[i]->getBasketTableId()==id) return positions[i];
        return NULL;
    }
    int getNumberOfPositions() { return positions.count(); }
    int getNumberOfBaskets() { int n=0; for (int i=0;i<positions.count();++i) if (positions[i] && positions[i]->hasBasket()) ++n; return n; }
    basketPositionClass* getPosition(int i) { return positions[i]; }
};
struct allPositionsClass {
    QVector<basketPositionClass*> conveyorSourceMarkers, conveyorTargetMarkers;
    positionListClass *beforeStacker2Position, *stacker2Position, *afterStacker2Position;
    positionListClass* returningConvPositionList04 = NULL;
    QVector<positionListClass*> recoverySources;
    int pendingReturn160BottomBasket = 0, recoveryNotifications = 0;
    QString return160TrackingStatus;
    QString tr(const char* s) { return QString(s); }
    void return160TrackingStatusChanged(QString) { ++recoveryNotifications; }
    void setReturn160TrackingStatus(const QString&);
    void observeReturn160Report(int);
    void processReturn160Report();
    positionListClass* getPositionListWithBasketOnFirstPos(int id) {
        for (auto list : recoverySources) if (list->isTheBottomBasket(id)) return list;
        return NULL;
    }
    Stacker2Handoff stacker2Handoff;
    bool stacker2InputStatusKnown = true;
    int pendingStacker2OutputBasket = 0;
    bool plcIsBeforeStacker2PosEmpty = false;
    QTime basketNeededBeforeStackerTime;
    void openGlChangedSignal() {}
    bool transferStacker2Basket(basketPositionClass*, basketPositionClass*, int);
    void processStacker2Handoffs();
    void moveBaskets(positionListClass*, positionListClass*);
};
#include "conveyorTransferMethods.inc"
void check(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr,"FAIL: %s\n",msg); std::exit(1); } }
int main(int argc, char** argv) {
    basketPositionClass a(700,1,110), b(700,2,38), c(700,3,95), d(700,4,0),
        e(710,1,0), f(710,2,0), g(710,3,0), h(710,4,0);
    positionListClass from, to; from.positions << &a << &b << &c << &d; to.positions << &e << &f << &g << &h;
    allPositionsClass all;
    transactionSucceeds=false;
    all.moveBaskets(&from,&to);
    check(a.basket.id==110 && e.basket.id==0,"conflict must preserve both stacks");
    check(!a.sourceMarker && !e.targetMarker && epicsWrites==0,"failed transaction must not publish markers or EPICS");
    check(assignmentWrites==0 && historyWrites==0,"failed transaction must not use legacy per-slot writes");
    transactionSucceeds=true;
    all.moveBaskets(&from,&to);
    check(a.basket.id==0 && b.basket.id==0 && c.basket.id==0 && e.basket.id==110 && f.basket.id==38 && g.basket.id==95,"successful transaction transfers entire stack");
    check(assignmentWrites==0 && historyWrites==0,"post-commit updates must never rewrite assignments or duplicate history");
    check(epicsWrites==3 && all.conveyorTargetMarkers.count()==3,"contents/EPICS behavior must survive committed transfer");
    check(a.sourceMarker && e.targetMarker && h.basket.id==0,"markers and empty slots must be preserved");
    if (argc > 1) {
        QJsonArray statements; for (int i=0;i<capturedStatements.count();++i) statements.append(capturedStatements[i]);
        QFile output(QString::fromLocal8Bit(argv[1])); check(output.open(QIODevice::WriteOnly),"open SQL fixture");
        output.write(QJsonDocument(statements).toJson());
    }
    int calls=transactionCalls;
    all.moveBaskets(&to,&to); check(transactionCalls==calls,"self-transfer must be ignored");
    observer=true; all.moveBaskets(&to,&from); observer=false;
    check(transactionCalls==calls,"observer must not transfer assignments");
    a.basket.id=99; all.moveBaskets(&to,&from); a.basket.id=0;
    check(transactionCalls==calls,"occupied memory destination must be rejected");
    e.reserved=true; all.moveBaskets(&to,&from); e.reserved=false;
    check(transactionCalls==calls,"crane-reserved slot must be rejected");
    f.basket.id=110; all.moveBaskets(&to,&from); f.basket.id=38;
    check(transactionCalls==calls,"duplicate source identities must be rejected");
    from.positions.removeLast(); all.moveBaskets(&to,&from); from.positions.append(&d);
    check(transactionCalls==calls,"incompatible stacks must be rejected");
    g.basket.id=-1; all.moveBaskets(&to,&from); g.basket.id=95;
    check(transactionCalls==calls,"negative source identity must be rejected");
    a.setBasketTableId(53,true);
    check(assignmentWrites==1 && historyWrites==1 && epicsWrites==4,"legacy callers must retain assignment, history and EPICS writes");
    basketPositionClass input(450,1,54), inside(910,1,0), output(460,1,0);
    input.positionEnumForEpics=inside.positionEnumForEpics=output.positionEnumForEpics=0;
    positionListClass inputList, insideList, outputList;
    inputList.positions << &input; insideList.positions << &inside; outputList.positions << &output;
    all.beforeStacker2Position=&inputList; all.stacker2Position=&insideList; all.afterStacker2Position=&outputList;
    all.stacker2InputStatusKnown=false; all.processStacker2Handoffs();
    all.plcIsBeforeStacker2PosEmpty=true; all.stacker2InputStatusKnown=true; all.processStacker2Handoffs();
    check(input.basket.id==54 && inside.basket.id==0,"startup must wait for an actual occupied signal before arming");
    all.plcIsBeforeStacker2PosEmpty=true;
    all.stacker2Handoff.observe(true,54,false); all.processStacker2Handoffs();
    check(input.basket.id==54 && inside.basket.id==0,"retained empty signal cannot consume a new delivery");
    all.plcIsBeforeStacker2PosEmpty=false; all.processStacker2Handoffs();
    all.plcIsBeforeStacker2PosEmpty=true; all.stacker2Handoff.observe(true,54,false);
    transactionSucceeds=false; calls=transactionCalls; all.processStacker2Handoffs();
    check(transactionCalls==calls+1 && input.basket.id==54 && inside.basket.id==0,"failed intake transaction must preserve identity for retry");
    transactionSucceeds=true; all.processStacker2Handoffs();
    check(input.basket.id==0 && inside.basket.id==54,"confirmed intake moves identity into stacker 2");
    check(capturedStatements.join("\n").contains("values('handoff'"),"handoff must include explicit history in the transaction");
    if (argc > 2) {
        QJsonArray statements; for (int i=0;i<capturedStatements.count();++i) statements.append(capturedStatements[i]);
        QFile fixture(QString::fromLocal8Bit(argv[2])); check(fixture.open(QIODevice::WriteOnly),"open stacker SQL fixture");
        fixture.write(QJsonDocument(statements).toJson());
    }
    calls=transactionCalls; all.processStacker2Handoffs();
    check(transactionCalls==calls,"repeated timer cannot duplicate intake");
    all.pendingStacker2OutputBasket=54; all.processStacker2Handoffs();
    check(inside.basket.id==0 && output.basket.id==54 && all.pendingStacker2OutputBasket==0,"actual outgoing identity moves from 910 to 460");
    all.pendingStacker2OutputBasket=54; calls=transactionCalls; all.processStacker2Handoffs();
    check(transactionCalls==calls && output.basket.id==54,"duplicate outgoing report is idempotent");
    input.basket.id=36; all.stacker2Handoff.observe(true,36,false); all.processStacker2Handoffs();
    check(input.basket.id==36 && inside.basket.id==0,"old empty signal must not clear the next basket");
    all.plcIsBeforeStacker2PosEmpty=false; all.processStacker2Handoffs();
    all.plcIsBeforeStacker2PosEmpty=true; all.stacker2Handoff.observe(true,36,false);
    input.reserved=true; all.processStacker2Handoffs();
    check(input.basket.id==36,"crane reservation blocks intake"); input.reserved=false;
    all.processStacker2Handoffs();
    check(input.basket.id==36 && inside.basket.id==0,"crane activity invalidates the previous input acknowledgement");
    all.plcIsBeforeStacker2PosEmpty=false; all.processStacker2Handoffs();
    all.plcIsBeforeStacker2PosEmpty=true;
    inside.basket.id=80; transactionSucceeds=false; all.processStacker2Handoffs();
    check(input.basket.id==36 && inside.basket.id==80 && insideList.positions.count()==2 && insideList.positions[1]->basket.id==0,
        "failed overflow intake retains both identities and an empty retry record");
    transactionSucceeds=true; all.processStacker2Handoffs();
    basketPositionClass* overflow=insideList.positions[1];
    check(input.basket.id==0 && inside.basket.id==80 && overflow->basket.id==36 && insideList.positions.count()==2,
        "occupied internal record must allow confirmed intake without overwriting either identity");
    check(overflow->getNumber()==910 && overflow->getIndex()==2 && overflow->lockedEnum==basketPositionClass::LOCKED,
        "additional internal records are locked tracking positions");
    all.pendingStacker2OutputBasket=36; all.processStacker2Handoffs();
    check(inside.basket.id==80 && overflow->basket.id==36 && output.basket.id==54,"occupied output cannot be overwritten");
    output.reserved=true; all.processStacker2Handoffs(); output.basket.id=0; output.reserved=false;
    calls=transactionCalls; all.processStacker2Handoffs();
    check(transactionCalls==calls && output.basket.id==0,"pending output report cannot resurrect a basket after crane pickup");
    all.pendingStacker2OutputBasket=80; observer=true; all.processStacker2Handoffs(); observer=false;
    check(output.basket.id==0 && inside.basket.id==80 && overflow->basket.id==36,"observer must not write stacker handoffs");
    transactionSucceeds=false; all.processStacker2Handoffs();
    check(output.basket.id==0 && all.pendingStacker2OutputBasket==80,"failed external output assignment remains pending");
    transactionSucceeds=true; all.processStacker2Handoffs();
    check(output.basket.id==80 && inside.basket.id==0 && overflow->basket.id==36,"PLC output ID must release only its own internal record");
    output.basket.id=0; all.pendingStacker2OutputBasket=36; all.processStacker2Handoffs();
    check(output.basket.id==36 && overflow->basket.id==0,"PLC output identity must find an overflow basket despite an empty first record");

    // Exact incident: 89 remains inside while the PLC consumes basket 21.
    inside.basket.id=89; output.basket.id=0; input.basket.id=21;
    all.plcIsBeforeStacker2PosEmpty=false; all.processStacker2Handoffs();
    all.plcIsBeforeStacker2PosEmpty=true; all.processStacker2Handoffs();
    check(input.basket.id==0 && inside.basket.id==89 && overflow->basket.id==21,
        "basket 21 must leave position 450 while basket 89 stays tracked inside");
    check(insideList.positions.count()==2,"empty tracking records must be reused");
    if (argc > 3) {
        QJsonArray statements; for (int i=0;i<capturedStatements.count();++i) statements.append(capturedStatements[i]);
        QFile fixture(QString::fromLocal8Bit(argv[3])); check(fixture.open(QIODevice::WriteOnly),"open overflow SQL fixture");
        fixture.write(QJsonDocument(statements).toJson());
    }
    calls=transactionCalls; all.processStacker2Handoffs();
    check(transactionCalls==calls,"confirmed overflow intake must not repeat");
    all.pendingStacker2OutputBasket=21; all.processStacker2Handoffs();
    check(output.basket.id==21 && overflow->basket.id==0 && inside.basket.id==89,"output can release basket 21 without losing basket 89");

    // A restart cannot treat a retained empty bit as a new intake. An exact
    // PLC output identity can safely reconcile the old input assignment.
    input.basket.id=63; output.basket.id=0; all.stacker2Handoff.reset();
    all.processStacker2Handoffs();
    check(input.basket.id==63,"restart with empty signal must preserve unacknowledged input");
    input.reserved=true; all.pendingStacker2OutputBasket=63; all.processStacker2Handoffs();
    check(input.basket.id==63 && output.basket.id==0,"output recovery must respect crane reservations");
    input.reserved=false; all.processStacker2Handoffs();
    check(input.basket.id==0 && output.basket.id==63 && inside.basket.id==89,
        "exact output identity recovers missed intake without touching earlier internal baskets");
    positionListClass ordinary; ordinary.mainPosId=450; ordinary.positions << &input;
    check(!ordinary.appendStacker2TrackingPosition(),"tracking allocation must be restricted to internal position 910");
    delete overflow;

    basketPositionClass r1(160,1,0), r2(160,2,0), r3(160,3,0), r4(160,4,0),
        s1(720,1,82), s2(720,2,116), s3(720,3,43), s4(720,4,0);
    r1.positionEnumForEpics=r2.positionEnumForEpics=r3.positionEnumForEpics=r4.positionEnumForEpics=0;
    positionListClass return160, verifiedStack;
    return160.positions << &r1 << &r2 << &r3 << &r4;
    verifiedStack.positions << &s1 << &s2 << &s3 << &s4;
    allPositionsClass recovery; recovery.returningConvPositionList04=&return160;
    calls=transactionCalls;
    recovery.observeReturn160Report(82);
    check(recovery.pendingReturn160BottomBasket==82 && return160.getNumberOfBaskets()==0 && transactionCalls==calls,
        "startup must retain bottom ID 82 without inventing the unreported 116/43 identities");
    check(return160.trackingReconciliationRequired && recovery.return160TrackingStatus.contains("82") && recovery.recoveryNotifications==1,
        "missing stack must show reconciliation warning and block position availability");
    recovery.processReturn160Report(); recovery.processReturn160Report();
    check(recovery.recoveryNotifications==1,"unresolved retries must not flood logs or operator notifications");

    recovery.recoverySources << &verifiedStack;
    transactionSucceeds=false; recovery.processReturn160Report();
    check(recovery.pendingReturn160BottomBasket==82 && r1.basket.id==0 && s1.basket.id==82,
        "failed recovery transaction must retain report and verified source for retry");
    transactionSucceeds=true; s1.reserved=true; recovery.processReturn160Report();
    check(recovery.pendingReturn160BottomBasket==82 && r1.basket.id==0,"recovery must respect crane reservations");
    s1.reserved=false; recovery.processReturn160Report();
    check(r1.basket.id==82 && r2.basket.id==116 && r3.basket.id==43 && s1.basket.id==0 && s2.basket.id==0 && s3.basket.id==0,
        "timer retry after source refresh restores verified 82/116/43 stack without another PLC change");
    check(recovery.pendingReturn160BottomBasket==0 && recovery.return160TrackingStatus.isEmpty() && !return160.trackingReconciliationRequired,
        "successful recovery clears pending state, warning and availability block");
    calls=transactionCalls; recovery.observeReturn160Report(82);
    check(transactionCalls==calls && recovery.pendingReturn160BottomBasket==0,
        "duplicate PLC snapshot of a tracked stack must not repeat the transfer");
    r1.basket.id=r2.basket.id=r3.basket.id=0; calls=transactionCalls; recovery.processReturn160Report();
    check(transactionCalls==calls,"completed recovery must not resurrect the stack after pickup");

    recovery.recoverySources.clear(); recovery.observeReturn160Report(82);
    r3.basket.id=43; recovery.processReturn160Report();
    check(r3.basket.id==43 && r1.basket.id==0 && recovery.pendingReturn160BottomBasket==82,
        "partial operator correction must be preserved while reconciliation remains pending");
    r1.basket.id=82; r2.basket.id=116; calls=transactionCalls; recovery.processReturn160Report();
    check(transactionCalls==calls && r1.basket.id==82 && r2.basket.id==116 && r3.basket.id==43 && recovery.pendingReturn160BottomBasket==0,
        "complete manual correction must clear warning without rewriting assignments or history");

    r1.basket.id=r2.basket.id=r3.basket.id=0; recovery.observeReturn160Report(82);
    recovery.observeReturn160Report(0); s1.basket.id=82; s2.basket.id=116; s3.basket.id=43;
    recovery.recoverySources << &verifiedStack; calls=transactionCalls; recovery.processReturn160Report();
    check(transactionCalls==calls && r1.basket.id==0 && recovery.pendingReturn160BottomBasket==0 && recovery.return160TrackingStatus.isEmpty(),
        "PLC zero must cancel a pending arrival before a delayed source appears");
    recovery.recoverySources.clear(); recovery.observeReturn160Report(82); recovery.observeReturn160Report(99);
    recovery.recoverySources << &verifiedStack; calls=transactionCalls; recovery.processReturn160Report();
    check(transactionCalls==calls && r1.basket.id==0 && recovery.pendingReturn160BottomBasket==99,
        "new PLC identity must supersede the older arrival");
    observer=true; recovery.observeReturn160Report(82);
    check(transactionCalls==calls && r1.basket.id==0 && recovery.pendingReturn160BottomBasket==82,
        "observer must show the mismatch without recovering assignments");
    observer=false;
    r1.basket.id=95; calls=transactionCalls; recovery.processReturn160Report();
    check(transactionCalls==calls && r1.basket.id==95 && s1.basket.id==82 && recovery.pendingReturn160BottomBasket==82,
        "pending recovery must never overwrite an occupied corrected destination");
    std::puts("PASS: position 160 startup recovery of 82/116/43; delayed source; manual corrections; cancelled/superseded reports; retries; crane reservations; observer mode; conveyor transfers; stacker 2 identity tracking.");
}

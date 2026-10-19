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
struct basketPositionClass {
    Basket basket;
    QString description;
    int number, index, positionEnumForEpics = 3;
    bool contents = true, reserved = false, sourceMarker = false, targetMarker = false;
    enum { EPICS_POS_BASKET_NOWHERE = 0, EPICS_POS_BASKET_IN_OVEN = 2, EPICS_POS_BASKET_AFTER_OVEN = 3, EPICS_POS_BASKET_NEW = 4 };
    basketPositionClass(int n, int i, int b) : number(n), index(i) { basket.id=b; description="operator's conveyor"; }
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
    int getNumberOfPositions() { return positions.count(); }
    int getNumberOfBaskets() { int n=0; for (int i=0;i<positions.count();++i) if (positions[i] && positions[i]->hasBasket()) ++n; return n; }
    basketPositionClass* getPosition(int i) { return positions[i]; }
};
struct allPositionsClass {
    QVector<basketPositionClass*> conveyorSourceMarkers, conveyorTargetMarkers;
    positionListClass *beforeStacker2Position, *stacker2Position, *afterStacker2Position;
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
    inside.basket.id=80; all.processStacker2Handoffs();
    check(input.basket.id==36 && inside.basket.id==80,"occupied internal slot cannot discard either basket");
    inside.basket.id=0; all.processStacker2Handoffs();
    check(input.basket.id==0 && inside.basket.id==36,"blocked intake retries when internal slot becomes available");
    all.pendingStacker2OutputBasket=36; all.processStacker2Handoffs();
    check(inside.basket.id==36 && output.basket.id==54,"occupied output cannot be overwritten");
    output.reserved=true; all.processStacker2Handoffs(); output.basket.id=0; output.reserved=false;
    calls=transactionCalls; all.processStacker2Handoffs();
    check(transactionCalls==calls && output.basket.id==0,"pending output report cannot resurrect a basket after crane pickup");
    all.pendingStacker2OutputBasket=80; observer=true; all.processStacker2Handoffs(); observer=false;
    check(output.basket.id==0 && inside.basket.id==36,"observer must not write stacker handoffs");
    transactionSucceeds=false; all.processStacker2Handoffs();
    check(output.basket.id==0 && all.pendingStacker2OutputBasket==80,"failed external output assignment remains pending");
    transactionSucceeds=true; all.processStacker2Handoffs();
    check(output.basket.id==80 && inside.basket.id==36,"PLC output ID must not consume an unrelated internal basket");
    std::puts("PASS: conveyor transfers and stacker 2 identity continuity, stale signals, retries, audit history, duplicate reports, occupied slots, crane reservations and observer mode.");
}

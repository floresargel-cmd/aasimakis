#ifndef GsW_H
#define GsW_H
#include "../Gs.h"
extern QString mainDbIp;
extern QSplashScreen *splash;
extern bool adminMode;
extern float scaleReading;
extern int ticketInScaleStation;


constexpr auto mDb="mainDb";
constexpr bool logMissionsTimes=true;
constexpr int maxCorrection=100;
constexpr int maxCassetteId=+495;
constexpr int minCassetteId=-495;//495-5

constexpr float cellXStep=820.f;
constexpr float cellZStep=740.f;

constexpr float layoutYMinusOffset=-11900.f;
constexpr float layoutYPlusOffset=5796.5f;



constexpr auto passWeak="asim";
constexpr auto passStrong="notsafe";

constexpr int minPacketLength=1000;
constexpr int maxPacketLength=7100;



#define whereIdleCassette QString(" where posId<%1 and cassette<>0 and locked=0 and posId<%1 and cassette not in(select cassette from %2) and cassette not in(select cassette from %3) and cassette not in(select cassette from %4) and cassette not in(select cassette from %5) and posId not in (select fromPos from missions) and posId not in (select toPos from missions)").arg(craneId).arg(exportsFrontTable).arg(exportsTriple1Table).arg(exportsTriple2Table).arg(exportsDoubleTable)
#define selectIdleCassettes QString("select cassette from pos ")+whereIdleCassette


void initializeSbWarehouseGlobals();
QVector<QVariant> getIdleCassettes(QString dbName);
QVector<QVariant> getNotIdleCassettes(QString dbName);
QVector<QVariant> getIdleEmptyCassettes(QString dbName);
QVector<QVariant> noVitalExecQuery(QString stmnt,QString connection,QString *errorString);
QVariant noVitalGetFirst(QString stmnt,QString connection,QString *errorString);

bool getPassword(QString title="Password is needed to open this window.",QString text="Please give the password",QString pass=passWeak,QWidget *p=NULL);
void logV(QString description,QString type);
void log(QString description,QString type);
void logGui(QString mess,QString description,QString from,QString type);

#endif
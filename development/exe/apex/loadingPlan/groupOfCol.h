
#ifndef GROUPOFCOL_H
#define GROUPOFCOL_H
class groupOfColumnsClass;
class loadingPlanClass;
#include <QtGui>
#include <QWidget>
#include <QTabWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTimer>
#include <QDateTime>
#include <QTime>
#include <QStandardItem>

#include "G.h"
#include "pack.h"
#include "packW.h"
#include "packCol.h"

class groupOfColumnsClass : public QWidget
{
	QTimer												mainTimer;
	loadingPlanClass							*loadingPlan;
	//
	QVector<packetColumnClass*>		packetColumnsVector;
	//
	int														totalWeight;
	int														maxHeight;
	int														maxWeight;
	double												XCG;
	double												YCG;
	bool													drawCG;
	int														groupType;//FOR_GROUP AFT_GROUP
	//
public:
enum groupEnum{
	ORIGINAL_GROUP=1,
	FOR_GROUP=2,
	AFT_GROUP=3,
};
    Q_OBJECT
public:
    groupOfColumnsClass(QScrollArea *scrollArea,QVector<packetColumnClass*> packetColumnsVector_,loadingPlanClass *loadingPlan_,int groupType_,bool drawCG_,QWidget *p);
	void calculate();
	void calculateGui();
	void exportLoading(int loadingId);
	void exportToPdf(QPainter *painter,int activeLoading,QString date);
	//
	void calculateGeometry();
	int getNoOfPackets();
	int getWeight();

	int getXCG(){return XCG;}
	int getYCG(){return YCG;}
	
	void setMaxHeight(int h);
	void setMaxWeight(int m);
	int getMaxHeight(){return maxHeight;}
	int getMaxWeight(){return maxWeight;}
	//optim
	void clearPackets();
	void clearManualExportOrder();
	void appendPacket(packetWidgetClass *p,int column);
protected:
	void paintEvent(QPaintEvent *ev);
private slots:
    void onMainTimer();
private:
};
#endif

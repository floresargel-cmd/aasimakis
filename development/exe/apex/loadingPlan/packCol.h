#ifndef PACKCOL_H
#define PACKCOL_H
class loadingPlanClass;
#include <QtGui>
#include <QWidget>
#include <QComboBox>
#include <QDialog>
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
#include <QScrollArea>
#include <QFile>
#include <QFont>
#include <qtextstream>
#include <QApplication>

#include "G.h"
//#include "pack.h"
#include "packW.h"
#include "loadPlan.h"
class packetColumnClass : public QWidget
{
  Q_OBJECT;
	QVector<packetWidgetClass*> packetsVector;
	loadingPlanClass						*loadingPlan;
	QString											labelString;
  QPoint											startPos;
	packetWidgetClass						*selectedPacket;
	int													horAlignment;
	int													perpAlignment;
	//
	int													totalWeight;
	int													totalHeight;
	double											YCG;
	//
	int													maxHeight;
	int													guiX;
	int													guiY;
	bool												isDragOver;
public:
	packetColumnClass(int horAlignment_=Qt::AlignHCenter,int perpAlignment_=Qt::AlignBottom,QString labelString_="",int maxHeight_=0,QWidget *p=NULL);
public:
	void appendPacket(packetWidgetClass* packet);
	void insertPacket(int index,packetWidgetClass* packet);
	void removePacket(packetWidgetClass* packet);
	int getOrderIdOfFirstPacket();
	void setLoadingPlan(loadingPlanClass *lp);
	void setMaxHeight(int h);
	void calculateGui();
	void calculate();
	int getWeight(){return totalWeight;}
	int getHeight(){return totalHeight;}
	double getYCG(){return YCG;}
	void setGuiX(int xx);
	void setGuiY(int yy);
	int getNoOfPackets(){return packetsVector.count();}
	packetWidgetClass* getPacket(int i){return packetsVector[i];}
	//
	void addPacketToManualExportList(int packetInt);
	void exportPacketsToManual();
	//optim
	void clearPackets();
	void clearManualExportOrder();
protected:
	packetWidgetClass *getMouseOverItem();
protected:
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
  void mouseReleaseEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);
protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void dragEnterEvent(QDragEnterEvent *event);
	void dragLeaveEvent(QDragLeaveEvent * event);
  void dragMoveEvent(QDragMoveEvent *event);
  void dropEvent(QDropEvent *event);
private:
  void performDrag();
};
#endif

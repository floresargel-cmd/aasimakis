
#ifndef PACKW_H
#define PACKW_H

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
#include <QLabel>
#include <QHeaderView>

class packetColumnClass;
class packetWidgetClass;
#include "G.h"
#include "pack.h"
#include "packCol.h"
class packetWidgetClass : public QLabel
{
	QString							soNum;
	QString							soItem;
	QString							alloy;
	QString							temper;
	QString							datePacked;
	QString							status;
  Q_OBJECT
private:
	QString							profileImgFile;
	QPixmap							*backGroundPixMap;
	QPixmap							*profilePixMap;
	packetColumnClass		*parentColumn;
	packetColumnClass		*initialParentColumn;
	packetClass					*packet;
	QColor							color;
	int									cassette;
	//
	bool								mouseOver;
public:
	packetWidgetClass(QWidget *p);
	packetWidgetClass(packetClass *packet_,QString soNum_,QString soItem_,QString alloy_,QString temper_,QString datePacked_,QString status_,packetColumnClass *parentColumn_,QColor color_,int cassette_,QWidget *p);
	bool isMouseOver(){return mouseOver;}
	//QString codeStr(){return QString()(packet->code);} 
	
	void setCode(int c){packet->code=c;}
	void setWidth(int w){packet->width=w;}
	void setHeight(int h){packet->height=h;}
	void setLength(int l){packet->length=l;}
	void setWeight(int w){packet->weight=w;}
	void setTopPlacementPriority(int t){packet->topPlacementPriority=t;}
	void setOrderId(int o){packet->orderId=0;}
	void setColor(QColor c){color=c;}
	void setManualExportOrder(int o){packet->manualExportOrder=o;update();}

	int getCode(){return packet->code;}
	int getWidth(){return packet->width;}
	int getHeight(){return packet->height;}
	int getLength(){return packet->length;}
	int getWeight(){return packet->weight;}
	int getTopPlacementPriority(){return packet->topPlacementPriority;}
	int getOrderId(){return packet->orderId;}
	QColor getColor(){return color;}
	int getManualExportOrder(){return packet->manualExportOrder;}

	QString getProfileImgFile(){return profileImgFile;}
	packetClass *getPacket(){return packet;}

	void removeFromParent();
	void setInitialParentColumn(packetColumnClass *initialParentColumn_);
	void setParentColumn(packetColumnClass *parentColumn_);
	void calculateGeometry();
	packetColumnClass *getColumnParent(){return parentColumn;}
	double getYCG();
	//
	void appendYourSelfToInitialParentColumn();
	QPixmap paint(bool forDrag);
protected:
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);
	//
	void setTipText();
};
#endif

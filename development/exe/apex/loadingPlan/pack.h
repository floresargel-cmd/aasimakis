
#ifndef PACK_H
#define PACK_H

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

#include "G.h"

class packetClass
{
public:
	int code;QString profile;int width;int height;int length;int weight;int topPlacementPriority;int orderId;int manualExportOrder;
	//
	int YCG;
	//
	packetClass();
	packetClass(int code_,QString profile_,int width_,int height_,int length_,int weight_,int topPlacementPriority_,int orderId_);
	packetClass &operator=(const packetClass& p)
	{
		code					=p.code					;
		profile					=p.profile				;
		width					=p.width				;
		height					=p.height				;
		length					=p.length				;
		weight					=p.weight				;
		topPlacementPriority	=p.topPlacementPriority	;
		orderId					=p.orderId				;
		manualExportOrder		=p.manualExportOrder	;
		return *this;
	}
	int getYCG(){return YCG;};
};
#endif

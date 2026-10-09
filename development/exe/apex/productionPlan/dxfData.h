#ifndef DXFDATA_H
#define DXFDATA_H
#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>
//#include "gDxf.h"
//
#define DIST(a,b)       sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y()))
#define ABSDIST(a,b)    fabs(a.x()-b.x())+fabs(a.y()-b.y())
#define PI 3.141592653589793
#include "dl_dxf.h"
#include "dl_creationadapter.h"
#include "dxfDataClasses.h"
class groupOfLinesClass
{
public:
	int										dxfColorIndex;
	QString								layer;
	QList<QLineF*>				lines;
	QList<QPointF*>				centers;
	groupOfLinesClass(){dxfColorIndex=0;};
	groupOfLinesClass(QString layer_,int dxfColorIndex_){layer=layer_;dxfColorIndex=dxfColorIndex_;};
	~groupOfLinesClass();
	void appendCenter(QPointF *c);
	bool append(QLineF* tl,QPointF *c);
	QPainterPath calculatePath();
	void rotate(QPointF center,double rot);
	void mirrorX();
	void saveToTxtFile(QTextStream *out);
	void loadFromTxtFile(QTextStream *out);
	//void save(QString name,QString conn,QString headerTable,QString dataTable);
	//void load(QString name,QString conn,QString headerTable,QString dataTable,int id);
	//void load(QVector<QVariant> x0,QVector<QVariant> y0,QVector<QVariant> x1,QVector<QVariant> y1,int color);
	QVector<QPointF*> getCentersInRect(QRectF r);
	QVector<QPointF> getPointsInRect(QRectF r);
	void clear();
	groupOfLinesClass &operator=(const groupOfLinesClass& p)
	{
		dxfColorIndex=p.dxfColorIndex;
		layer=p.layer;
		lines=p.lines;
		centers=p.centers;
		return *this;
	}
	bool isContainedInPolygon(QPolygonF polygon);
};
class dxfDataClass : public DL_CreationAdapter
{
	QList<groupOfLinesClass>	groupOfLines;
	QList<QLineF*>						allLines;
	QList<int>								allDxfColors;
	QList<QPointF*>						allCenters;
	QList<QString>						allLayers;
public:
	dxfDataClass();
	~dxfDataClass();
private:
	void addLinesToGroup(QList<QLineF*> lines,int dxfColor);
	QVector<groupOfLinesClass*> getGroupsWithColor(QString layer,int dxfColorIndex);
public:
	void calculate();
	QList<groupOfLinesClass> getAllGroupsOfLines(){return groupOfLines;};
	int getNoOfGroupsOfLines(){return groupOfLines.count();}
	groupOfLinesClass getGroupOfLines(int i){return groupOfLines[i];}
	//
	virtual void addLayer(const DL_LayerData& data);
	virtual void addPoint(const DL_PointData& data);
	virtual void addLine(const DL_LineData& data);
	virtual void addArc(const DL_ArcData& data);
	virtual void addCircle(const DL_CircleData& data);
	virtual void addEllipse(const DL_EllipseData& data);
	virtual void addText(const DL_TextData&);
};

#endif

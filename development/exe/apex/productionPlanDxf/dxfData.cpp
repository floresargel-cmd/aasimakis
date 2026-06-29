#include "dxfData.h"
#define DIST(a,b)       sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y()))
#define ABSDIST(a,b)    fabs(a.x()-b.x())+fabs(a.y()-b.y())
double accur=0.00001;
bool areEqual(QPointF a,QPointF b) 
{
	if (fabs(a.x()-b.x())>accur)
		return false;
	if (fabs(a.y()-b.y())>accur)
		return false;
	return true;
}
void groupOfLinesClass::appendCenter(QPointF *c)
{
	for (int i=0;i<centers.count();i++)
	{
		if (areEqual(*centers[i],*c))
			return;
	}
	centers.append(c);
}
bool groupOfLinesClass::isContainedInPolygon(QPolygonF polygon)
{
	for (int i=0;i<lines.count();i++)
	{
		if ((polygon.containsPoint(lines[i]->p1(),Qt::OddEvenFill)))
			return true;
		if ((polygon.containsPoint(lines[i]->p2(),Qt::OddEvenFill)))
			return true;
	}
	return false;
}
QVector<QPointF*> groupOfLinesClass::getCentersInRect(QRectF r)
{
	QVector<QPointF*> ans;
	for (int i=0;i<centers.count();i++)
	{
		if (r.contains(*centers[i]))
			ans<<centers[i];
	}
	return ans;
}
QVector<QPointF> groupOfLinesClass::getPointsInRect(QRectF r)
{
	QVector<QPointF> ans;
	for (int i=0;i<lines.count();i++)
	{
		if (r.contains(lines[i]->p1()))
			ans<<lines[i]->p1();
		if (r.contains(lines[i]->p2()))
			ans<<lines[i]->p2();
	}
	return ans;
}
bool groupOfLinesClass::append(QLineF* tl,QPointF *c)
{
	lines.append(tl);
	if (c)appendCenter(c);
	return true;
}
QPainterPath groupOfLinesClass::calculatePath()
{
	QPainterPath ans;
	if (lines.count()>0)
	{
		for (int i=0;i<(lines.count());i++)
		{
			ans.moveTo(lines[i]->p1());
			ans.lineTo(lines[i]->p2());
		}
	}
	return ans;
}
QPointF rotatePoint(QPointF center,QPointF p,double cos,double sin)
{
	QPointF ans;
	p.setX(p.x()-center.x());
	p.setY(p.y()-center.y());
	// rotate point
	ans.setX(p.x()*cos-p.y()*sin);
	ans.setY(p.x()*sin+p.y()*cos);
	ans.setX(ans.x()+center.x());
	ans.setY(ans.y()+center.y());
	return ans;
}
QPointF mirrorXPoint(QPointF p)
{
	QPointF ans;
	ans.setX(-p.x());
	ans.setY(p.y());
	return ans;
}
void groupOfLinesClass::rotate(QPointF center,double rot)
{
	if (fabs(rot)<DBL_EPSILON)
		return;
	double cos=qCos(rot*PI/180.);
	double sin=qSin(rot*PI/180.);
	for (int i=0;i<lines.count();i++)
	{
		lines[i]->setP1(rotatePoint(center,lines[i]->p1(),cos,sin));
		lines[i]->setP2(rotatePoint(center,lines[i]->p2(),cos,sin));
	}
	for (int i=0;i<centers.count();i++)
	{
		*centers[i]=rotatePoint(center,*(centers[i]),cos,sin);
	}
}
void groupOfLinesClass::mirrorX()
{
	for (int i=0;i<lines.count();i++)
	{
		lines[i]->setP1(mirrorXPoint(lines[i]->p1()));
		lines[i]->setP2(mirrorXPoint(lines[i]->p2()));
	}
	for (int i=0;i<centers.count();i++)
	{
		*centers[i]=mirrorXPoint(*centers[i]);
	}
}
void groupOfLinesClass::saveToTxtFile(QTextStream *out)
{
	(*out)<<layer<<" "<<dxfColorIndex<<" "<<lines.count()<<"\n";
	for (int i=0;i<lines.count();i++)
	{
		(*out)<<lines[i]->x1()<<" "<<lines[i]->y1()<<" "<<lines[i]->x2()<<" "<<lines[i]->y2()<<"\n";
	}
	(*out)<<centers.count()<<"\n";
	for (int i=0;i<centers.count();i++)
	{
		if (centers[i])
			(*out)<<centers[i]->x()<<" "<<centers[i]->y()<<"\n";
		else
			(*out)<<"- -\n";
	}
}
void groupOfLinesClass::loadFromTxtFile(QTextStream *out)
{
	(*out)>>layer;
	(*out)>>dxfColorIndex;
	//
	lines.clear();
	int linesNo;(*out)>>linesNo;
	double x0,y0,x1,y1;
	for (int i=0;i<linesNo;i++)
	{
		(*out)>>x0;(*out)>>y0;(*out)>>x1;(*out)>>y1;
		QLineF *line=new QLineF(x0,y0,x1,y1);
		lines.append(line);
	}
	//
	centers.clear();
	QString tx,ty;
	int centersNo;(*out)>>centersNo;
	for (int i=0;i<centersNo;i++)
	{
		(*out)>>tx;
		(*out)>>ty;
		if (tx=="_")
			centers.append(NULL);
		else
			centers.append(new QPointF(tx.toDouble(),ty.toDouble()));
	}
}
QList<groupOfLinesClass*> dxfDataClass::getGroupsWithColor(QString layer,int dxfColorIndex)
{
	QList<groupOfLinesClass*> ans;
	for (int i=0;i<groupOfLines.count();i++)
	{
		if (groupOfLines[i].dxfColorIndex==dxfColorIndex)
			ans.append(&groupOfLines[i]);
	}
	if (ans.count()==0)
	{
		groupOfLines.append(groupOfLinesClass(layer,dxfColorIndex));
		ans.append(&groupOfLines.last());
	}
	return ans;
}
void dxfDataClass::calculate()
{
	while (allLines.count()>0)
	{
		//add to existing Group
		bool found=false;
		for (int i=0;i<allLines.count();i++)
		{
			QList<groupOfLinesClass*> groups=getGroupsWithColor(allLayers[i],allDxfColors[i]);
			for (int j=0;j<groups.count();j++)			
			{
				if (groups[j]->append(allLines[i],allCenters[i]))
				{
					found=true;
					allLines.removeAt(i);
					allDxfColors.removeAt(i);
					allLayers.removeAt(i);
					allCenters.removeAt(i);
					i=0;
					break;
				}
			}
		}
		if (!found)
		{
			groupOfLines.append(groupOfLinesClass(allLayers[0],allDxfColors[0]));
			groupOfLines.last().append(allLines[0],allCenters[0]);
			allLines.removeAt(0);
			allDxfColors.removeAt(0);
			allLayers.removeAt(0);
			allCenters.removeAt(0);
		}
	}
}
//#define SET_CUSTOM_COLOR if (attributes.getLayer()==ALLUMINIUM_LAYER_NAME)color=244;if (attributes.getLayer()==POLY_LAYER_NAME)color=84;
void dxfDataClass::addLayer(const DL_LayerData& data) 
{
}
void dxfDataClass::addPoint(const DL_PointData& data) 
{
	//int color=attributes.getColor();
	//pointGraphicsItemClass *point=new pointGraphicsItemClass(data.x,data.y);
	//for (int i=0;i<point->getLines().count();i++)
	//{
	//	allLines.append(point->getLines().at(i));
	//	allCenters.append(NULL);
	//	allDxfColors.append(color);
	//	allLayers.append(QString::fromStdString((attributes.getLayer())).remove(" "));
	//}
}
void dxfDataClass::addLine(const DL_LineData& data) 
{
	double dist=fabs(data.x1-data.x2)+fabs(data.y1-data.y2);
	if (   dist<FLT_EPSILON  )
		return;
	int color=attributes.getColor();
	lineGraphicsItemClass *line=new lineGraphicsItemClass(data.x1,data.y1,data.x2,data.y2);
	for (int i=0;i<line->getLines().count();i++)
	{
		allLines.append(line->getLines().at(i));
		allCenters.append(NULL);
		allDxfColors.append(color);
		allLayers.append(QString::fromStdString((attributes.getLayer())).remove(" "));
	}
}
void dxfDataClass::addArc(const DL_ArcData& data) 
{
	int color=attributes.getColor();
	double angle1;
	double angle2;
	if (getExtrusion()->getDirection()[2]<0)
	{
		angle1=180.-data.angle2;
		angle2=180.-data.angle1;
	}
	else
	{
		angle1=data.angle1;
		angle2=data.angle2;
	}
	if (angle2<angle1)
		angle2+=360.;
	ellipseGraphicsItemClass *arc=new ellipseGraphicsItemClass((getExtrusion()->getDirection()[2]>0?1.0:-1.0)*data.cx,data.cy,data.radius,0.0,1.0,angle1*M_PI/180.,angle2*M_PI/180.);
	for (int i=0;i<arc->getLines().count();i++)
	{
		allLines.append(arc->getLines().at(i));
		allCenters.append(new QPointF(data.cx,data.cy));
		allDxfColors.append(color);
		allLayers.append(QString::fromStdString((attributes.getLayer())).remove(" "));
	}
}
void dxfDataClass::addCircle(const DL_CircleData& data) 
{
	int color=attributes.getColor();
	ellipseGraphicsItemClass *cicle=new ellipseGraphicsItemClass(data.cx,data.cy,data.radius,0.0,1.0,0.0,2.0*M_PI);
	for (int i=0;i<cicle->getLines().count();i++)
	{
		allLines.append(cicle->getLines().at(i));
		allCenters.append(new QPointF(data.cx,data.cy));
		allDxfColors.append(color);
		allLayers.append(QString::fromStdString((attributes.getLayer())).remove(" "));
	}
}
void dxfDataClass::addEllipse(const DL_EllipseData& data) 
{
	int color=attributes.getColor();
	ellipseGraphicsItemClass *ellipse=new ellipseGraphicsItemClass(data.cx,data.cy,data.mx,data.my,data.ratio,data.angle1,data.angle2);
	for (int i=0;i<ellipse->getLines().count();i++)
	{
		allLines.append(ellipse->getLines().at(i));
		allCenters.append(new QPointF(data.cx,data.cy));
		allDxfColors.append(color);
		allLayers.append(QString::fromStdString((attributes.getLayer())).remove(" "));
	}
}
void dxfDataClass::addText(const DL_TextData& data)
{

}

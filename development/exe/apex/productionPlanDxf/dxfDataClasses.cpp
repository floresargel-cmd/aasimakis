#include "dxfDataClasses.h"
////////////////////////////////////////////point///////////////////////////////////////////////////////////////
pointGraphicsItemClass::pointGraphicsItemClass(double x,double y/*,int color,QGraphicsItemGroup *parentGroup*/)//:graphicsItemClass(parentGroup,color)
{
	double drawD=1.5;
	lines.append(new QLineF(x-drawD,y,x+drawD,y));
	lines.append(new QLineF(x,y-drawD,x,y+drawD));
}
////////////////////////////////////////////line///////////////////////////////////////////////////////////////
lineGraphicsItemClass::lineGraphicsItemClass(double x1,double y1,double x2,double y2/*,int color,QGraphicsItemGroup *parentGroup*/)//:graphicsItemClass(parentGroup,color)
{
	lines.append(new QLineF(x1,y1,x2,y2));
}
/////////////////////////////////////////////ellipse//////////////////////////////////////////////////////////////
ellipseGraphicsItemClass::ellipseGraphicsItemClass(double cx,double cy,double mx,double my,double ratio,double angle1,double angle2/*,int color,QGraphicsItemGroup *parentGroup*/)//:graphicsItemClass(parentGroup,color)
{
	double a=sqrt(mx*mx+my*my);//major half length
	//
	if (fabs(angle2-angle1)<FLT_EPSILON)//closed
		angle2=angle1+2.*M_PI;
	if (angle2<angle1)
	{
		double temp=angle2;angle2=angle1;angle1=temp;
	}
	double b=a*ratio;//minor half length
	////
	QTransform trans;
	double rotAngle;
	if (fabs(mx)<FLT_EPSILON)
		rotAngle=0.5*M_PI;
	else
		rotAngle=atan(my/mx);
	if ((mx<0.)||(my<0.))
		rotAngle+=M_PI;
	trans.rotate(rotAngle*180./M_PI);
	double da=10.*(M_PI/180.);
	if (da>(0.33333*(angle2-angle1)))
		da=0.33333*(angle2-angle1);
	QPointF p0,p1;
	p0=trans.map(QPointF(a*cos(angle1),b*sin(angle1)))+QPointF(cx,cy);
	for (double angle=angle1+da;(angle+da-0.001)<angle2;angle+=da)
	{
		p1=trans.map(QPointF(a*cos(angle),b*sin(angle)))+QPointF(cx,cy);
		lines.append(new QLineF(p0,p1));
		p0=p1;
	}
	p1=trans.map(QPointF(a*cos(angle2),b*sin(angle2)))+QPointF(cx,cy);
	if (  ((abs(p0.x()-p1.x()))+ (abs(p0.y()-p1.y())))>0.0001 )
		lines.append(new QLineF(p0,p1));
}
